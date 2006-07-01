/*
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 * Copyright (C) 2005 Atsushi Konno All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <unistd.h>
#include <string.h>

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_log.h"
#include "ap_config.h"
#include "apr_strings.h"
#include "util_filter.h"
#include "apr_buckets.h"
#include "apr_lib.h"
#include "apr_tables.h"
#include "apr_dso.h"
#include "apr_general.h"
#include "apr_pools.h"

#include "mod_chxj.h"
#include "chxj_encoding.h"
#include "qs_ignore_sp.h"
#include "qs_log.h"
#include "qs_malloc.h"
#include "qs_parse_attr.h"
#include "qs_parse_file.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "chxj_load_device_data.h"
#include "chxj_load_emoji_data.h"
#include "chxj_specified_device.h"
#include "chxj_tag_util.h"
#include "chxj_xhtml_mobile_1_0.h"
#include "chxj_hdml.h"
#include "chxj_chtml10.h"
#include "chxj_chtml20.h"
#include "chxj_chtml30.h"
#include "chxj_jhtml.h"

#include "chxj_img_conv_format.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_apply_convrule.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"


#define CHXJ_VERSION_PREFIX PACKAGE_NAME "/"
#define CHXJ_VERSION        PACKAGE_VERSION

converter_t convert_routine[] = {
  {
    /* CHXJ_SPEC_UNKNOWN          */
    .converter = NULL,
    .encoder  = NULL,
  },
  {
    /* CHXJ_SPEC_Chtml_1_0        */
    .converter = chxj_exchange_chtml10,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_2_0        */
    .converter = chxj_exchange_chtml20,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_3_0        */
    .converter = chxj_exchange_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_4_0        */
    .converter = chxj_exchange_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_5_0        */
    .converter = chxj_exchange_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_XHtml_Mobile_1_0 */
    .converter = chxj_exchange_xhtml_mobile_1_0,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Hdml             */
    .converter = chxj_exchange_hdml,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Jhtml            */
    .converter = chxj_exchange_jhtml,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_HTML             */
    .converter = NULL,
    .encoder  = NULL,
  },
  {
    NULL
  }
};

static int chxj_convert_input_header(request_rec *r,chxjconvrule_entry* entryp);

/**
 * Only when User-Agent is specified, the User-Agent header is camouflaged. 
 *
 * @param r   [i]
 */
static apr_status_t 
chxj_headers_fixup(request_rec *r)
{
  mod_chxj_config*    dconf; 
  chxjconvrule_entry* entryp;
  char*               user_agent;
  device_table*       spec;

  DBG(r, "start chxj_headers_fixup()");
  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  spec = chxj_specified_device(r, user_agent);

  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
    entryp = chxj_apply_convrule(r, dconf->convrules);
    if (! entryp) {
      DBG(r, "end chxj_headers_fixup() no pattern");
      return DECLINED;
    }
  
    apr_table_setn(r->headers_in, 
                   CHXJ_HTTP_USER_AGENT, 
                   user_agent);
  
    if (entryp->user_agent)
      apr_table_setn(r->headers_in, 
                     HTTP_USER_AGENT, 
                     entryp->user_agent);

    chxj_convert_input_header(r,entryp);

    break;
  
  default:
    break;

  }

  DBG(r, "end chxj_headers_fixup()");

  return DECLINED;
}


/**
 * It converts it from CHTML into XXML corresponding to each model. 
 *
 * @param r   [i]
 * @param src [i]   It is former HTML character string. 
 * @param len [i/o] It is length of former HTML character string. 
 */
static char* 
chxj_exchange(request_rec *r, const char** src, apr_size_t* len)
{
  char*               user_agent;
  char*               dst;
  char*               tmp;
  char*               cookie_id;
  mod_chxj_config*    dconf; 
  chxjconvrule_entry* entryp;

  dst  = apr_pstrcat(r->pool, (char*)*src, NULL);

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);


  entryp = chxj_apply_convrule(r, dconf->convrules);

  if (!entryp || !(entryp->action & CONVRULE_ENGINE_ON_BIT))
    return (char*)*src;


  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  if (entryp->user_agent)
    user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
  else
    user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);

  DBG1(r,"User-Agent:[%s]", user_agent);
  DBG(r, "start chxj_exchange()");
  DBG1(r,"content type is %s", r->content_type);


  if (*(char*)r->content_type == 't' 
  && strncmp(r->content_type, "text/html",   9) != 0) {
    DBG1(r,"content type is %s", r->content_type);
    return (char*)*src;
  }

  /*
   * save cookie.
   */
  cookie_id = NULL;
  if (entryp->action & CONVRULE_COOKIE_ON_BIT)
    cookie_id = chxj_save_cookie(r);

  if (!r->header_only) {
    device_table* spec = chxj_specified_device(r, user_agent);

    tmp = NULL;
    if (convert_routine[spec->html_spec_type].encoder)
      tmp = convert_routine[spec->html_spec_type].encoder(r, 
                                                          *src, 
                                                          (apr_size_t*)len);

    if (convert_routine[spec->html_spec_type].converter) {
      if (tmp)
        dst = convert_routine[spec->html_spec_type].converter(r, 
                                                              spec, 
                                                              tmp, 
                                                              *len, 
                                                              len, 
                                                              entryp, 
                                                              cookie_id);
      else
        dst = convert_routine[spec->html_spec_type].converter(r,
                                                              spec, 
                                                              tmp, 
                                                              *len, 
                                                              len, 
                                                              entryp, 
                                                              cookie_id);
    }
  }

  if (*len == 0) {
    dst = apr_psprintf(r->pool, "\n");
    *len = 1;
  }
  dst[*len] = 0;

  DBG(r, "end chxj_exchange()");

  return dst;
}


/**
 * It converts it from HEADER.
 *
 * @param r   [i]
 */
static int
chxj_convert_input_header(request_rec *r,chxjconvrule_entry* entryp) 
{

  char*      buff;
  apr_size_t urilen;
  char*      result;
  char*      pair;
  char*      name;
  char*      value;
  char*      pstate;
  char*      vstate;

  DBG(r, "start chxj_convert_input_header()");

  if (! r->args) {
    DBG(r, "r->args=[null]");
    DBG(r, "end   chxj_convert_input_header()");
    return 0;
  }
  urilen = strlen(r->args);

  result = qs_alloc_zero_byte_string(r);

  buff = apr_pstrdup(r->pool, r->args);
  DBG1(r, "r->args=[%s]", buff);

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) {

    pair = apr_strtok(buff, "&", &pstate);
    if (pair == NULL)
      break;

    buff = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strncasecmp(name, "_chxj", 5) != 0) {
      if (strlen(result) != 0) 
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (strcasecmp(entryp->encoding, "NONE") != 0 && value && strlen(value)) {
        apr_size_t dlen;
        char* dvalue;

        dlen   = strlen(value);
        value = chxj_url_decode(r, value);
        DBG1(r, "************ before encoding[%s]", value);

        dvalue = chxj_rencoding(r, value, &dlen);
        dvalue = chxj_url_encode(r, dvalue);

        DBG1(r, "************ after encoding[%s]", dvalue);

        result = apr_pstrcat(r->pool, result, name, "=", dvalue, NULL);
      }
      else {
        if (strcmp(name, pair) != 0)
          result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
        else
          result = apr_pstrcat(r->pool, result, name, NULL);
      }
    }
    else
    if (strncasecmp(name, "_chxj_c_", 8) == 0 
    ||  strncasecmp(name, "_chxj_r_", 8) == 0
    ||  strncasecmp(name, "_chxj_s_", 8) == 0) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);

      result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0) {
      DBG1(r, "found cookie parameter[%s]", value);
      chxj_load_cookie(r, value);
    }
  }
  r->args = result;

  DBG1(r, "result r->args=[%s]", r->args);
  DBG(r, "end   chxj_convert_input_header()");
  return 0;
}


/**
 * It converts it from POSTDATA .
 *
 * @param r   [i]
 * @param src [i]   It is POSTDATA character string.
 * @param len [i/o] It is length of former HTML character string.
 */
static char*
chxj_input_convert(
  request_rec*        r, 
  const char**        src, 
  apr_size_t*         len, 
  chxjconvrule_entry* entryp)
{
  char* pair;
  char* name;
  char* value;
  char* pstate;
  char* vstate;
  char* s;
  char* result;

  s = apr_pstrdup(r->pool, *src);

  result = qs_alloc_zero_byte_string(r);

  DBG1(r, "BEFORE input convert source = [%s]", s);

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) {
    pair = apr_strtok(s, "&", &pstate);
    if (pair == NULL)
      break;
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strncasecmp(name, "_chxj", 5) != 0) {
      if (strlen(result) != 0) 
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (strcasecmp(entryp->encoding, "NONE") != 0 
      &&  value && strlen(value)) {
        apr_size_t dlen;
        char*      dvalue;

        dlen   = strlen(value);
        value = chxj_url_decode(r, value);
        DBG1(r, "************ before encoding[%s]", value);

        dvalue = chxj_rencoding(r, value, &dlen);
        dvalue = chxj_url_encode(r,dvalue);

        DBG1(r, "************ after encoding[%s]", dvalue);

        result = apr_pstrcat(r->pool, result, name, "=", dvalue, NULL);

      }
      else {
        result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
      }
    }
    else
    if (strncasecmp(name, "_chxj_c_", 8) == 0 
    ||  strncasecmp(name, "_chxj_r_", 8) == 0
    ||  strncasecmp(name, "_chxj_s_", 8) == 0) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (strcasecmp(entryp->encoding, "NONE") != 0 && value && strlen(value)) {
        apr_size_t dlen;
        char*      dvalue;

        dlen   = strlen(value);
        value = chxj_url_decode(r, value);
        DBG1(r, "************ before encoding[%s]", value);

        dvalue = chxj_rencoding(r, value, &dlen);
        dvalue = chxj_url_encode(r,dvalue);

        DBG1(r, "************ after encoding[%s]", dvalue);

        result = apr_pstrcat(r->pool, result, &name[8], "=", dvalue, NULL);

      }
      else {
        result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
      }
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0) {
      DBG1(r, "found cookie parameter[%s]", value);
      chxj_load_cookie(r, value);
    }
  }
  *len = strlen(result);

  DBG1(r, "AFTER input convert result = [%s]", result);

  return result;
}


/**
 * The received data is returned to the filter.
 *
 * @param f    [i/o] It is a filter. 
 * @param data [i]   It is data returned to the filter. 
 * @param len  [i]   It is length of the data returned to the filter. 
 */
static apr_status_t 
pass_data_to_filter(ap_filter_t *f, const char *data, 
                                        apr_size_t len)
{
  request_rec*        r = f->r;
  conn_rec*           c = r->connection;
  apr_status_t        rv;
  apr_bucket_brigade* bb;
  apr_bucket*         b;

  DBG(r, "start pass_data_to_filter()");

  bb = apr_brigade_create(r->pool, c->bucket_alloc);
  b  = apr_bucket_transient_create(data, len, c->bucket_alloc);

  APR_BRIGADE_INSERT_TAIL(bb, b);
  b = apr_bucket_eos_create(f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(bb, b);

  rv = ap_pass_brigade(f->next, bb);
  if (rv != APR_SUCCESS) {
    DBG(r, "ap_pass_brigade()");
    return rv;
  }

  DBG(r, "end pass_data_to_filter()");

  return rv;
}


/**
 * It is the main loop of the output filter. 
 *
 * @param f   [i/o] It is a filter.
 * @param bb  [i]   
 */
static apr_status_t 
chxj_output_filter(ap_filter_t *f, apr_bucket_brigade *bb)
{
  request_rec*        r;
  apr_status_t        rv;
  apr_bucket*         b;
  const char*         data;
  char*               contentLength;
  apr_size_t          len;
  mod_chxj_ctx*       ctx;
  char*               cookie_id;
  char*               location_header;
  mod_chxj_config*    dconf;
  chxjconvrule_entry* entryp;



  DBG(r, "start of chxj_output_filter()");

  r  = f->r;
  rv = APR_SUCCESS;

  if (!f->ctx) {
    if ((f->r->proto_num >= 1001) 
    &&  !f->r->main 
    &&  !f->r->prev) 
      f->r->chunked = 1;
  }

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  entryp = chxj_apply_convrule(r, dconf->convrules);


  for (b = APR_BRIGADE_FIRST(bb);
       b != APR_BRIGADE_SENTINEL(bb); 
       b = APR_BUCKET_NEXT(b)) {
    if (APR_BUCKET_IS_EOS(b)) {
      DBG(r, "eos");
      /*----------------------------------------------------------------------*/
      /* End Of File                                                          */
      /*----------------------------------------------------------------------*/
      if (f->ctx) {
        ctx = (mod_chxj_ctx*)f->ctx;
        DBG1(r, "content_type=[%s]", r->content_type);
        if (r->content_type 
        && *(char*)r->content_type == 't' 
        && strncmp(r->content_type, "text/html",   9) == 0) {
          if (ctx->len) {
            char* tmp = apr_palloc(r->pool, ctx->len + 1);
            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);

            DBG2(r, "input data=[%s] len=[%d]", tmp, ctx->len);

            ctx->buffer = chxj_exchange(r, (const char**)&tmp, (apr_size_t*)&ctx->len);

            DBG2(r, "output data=[%.*s]", ctx->len,ctx->buffer);
          }
          else {
            ctx->buffer = apr_psprintf(r->pool, "\n");
            ctx->len += 1;
            ctx->buffer = chxj_exchange(r, 
                                        (const char**)&ctx->buffer, 
                                        (apr_size_t*)&ctx->len);

          }
        }

        if (r->content_type 
        && *(char*)r->content_type == 'i' 
        && strncmp(r->content_type, "image/", 6) == 0) {
          if (ctx->len) {
            char* tmp = apr_palloc(r->pool, ctx->len + 1);
            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);

            DBG1(r, "input data=[%s]", tmp);

            ctx->buffer = 
              chxj_exchange_image(r, (const char**)&tmp,(apr_size_t*)&ctx->len);
            if (ctx->buffer == NULL) {
              ctx->buffer = tmp;
            }

            DBG2(r, "output data=[%.*s]", ctx->len,ctx->buffer);
          }
        }

        contentLength = apr_psprintf(r->pool, "%d", ctx->len);
        apr_table_setn(r->headers_out, "Content-Length", contentLength);
        
        if (ctx->len > 0) {
          rv = pass_data_to_filter(f, 
                                   (const char*)ctx->buffer, 
                                   (apr_size_t)ctx->len);
        }
        f->ctx = NULL;
        DBG(r, " ");
        return rv;
      }
      else {
        DBG1(r, " SAVE COOKIE[%x]", entryp->action);
        /*
         * save cookie.
         */
        if (entryp->action & CONVRULE_COOKIE_ON_BIT) {
          DBG(r, "entryp->action == COOKIE_ON_BIT");

          cookie_id = chxj_save_cookie(r);

          /*
           * Location Header Check to add cookie parameter.
           */
          location_header = (char*)apr_table_get(r->headers_out, "Location");
          if (location_header) {
            DBG1(r, "Location Header=[%s]", location_header);
            location_header = chxj_add_cookie_parameter(r,
                                                        location_header,
                                                        cookie_id);
            apr_table_setn(r->headers_out, "Location", location_header);
            DBG1(r, "Location Header=[%s]", location_header);
          }
        }

        apr_table_setn(r->headers_out, "Content-Length", "0");
        rv = pass_data_to_filter(f, (const char*)"", (apr_size_t)0);
        DBG(r, " ");
        return rv;
      }
    }
    else
    if (apr_bucket_read(b, &data, &len, APR_BLOCK_READ) == APR_SUCCESS) {
      DBG2(r, "read data[%.*s]",len, data);

      if (f->ctx == NULL) {
        /*--------------------------------------------------------------------*/
        /* Start                                                              */
        /*--------------------------------------------------------------------*/
        DBG(r, "new context");
        ctx = (mod_chxj_ctx*)apr_palloc(r->pool, sizeof(mod_chxj_ctx));
        if (len > 0) {
          ctx->buffer = apr_palloc(r->pool, len);
          memcpy(ctx->buffer, data, len);
        }
        else {
          ctx->buffer = apr_palloc(r->pool, 1);
          ctx->buffer = '\0';
        }
        ctx->len = len;
        f->ctx = (void*)ctx;
      }
      else {
        /*--------------------------------------------------------------------*/
        /* append data                                                        */
        /*--------------------------------------------------------------------*/
        char* tmp;
        DBG(r, "append data start");
        ctx = (mod_chxj_ctx*)f->ctx;

        if (len > 0) {
          tmp = apr_palloc(r->pool, ctx->len);
          memcpy(tmp, ctx->buffer, ctx->len);

          ctx->buffer = apr_palloc(r->pool, ctx->len + len);

          memcpy(ctx->buffer, tmp, ctx->len);
          memcpy(&ctx->buffer[ctx->len], data, len);

          ctx->len += len;
        }
        DBG(r, "append data end");
      }
    }
  }
  apr_brigade_destroy(bb);

  DBG(r, "end of output filter");

  return APR_SUCCESS;
}

/**
 * It is the main loop of the input filter. 
 *
 * @param f         [i/o] It is a filter.
 * @param bb        [i]   brigade
 * @param mode      [i]   mode
 * @param block     [i]   block
 * @param readbytes [i]   readbyte
 */
static apr_status_t 
chxj_input_filter(ap_filter_t*        f, 
                 apr_bucket_brigade*  bb,
                 ap_input_mode_t      mode, 
                 apr_read_type_e      block,
                 apr_off_t            readbytes)
{
  request_rec*        r = f->r;
  apr_status_t        rv;
  conn_rec*           c = r->connection;
  apr_bucket*         b;
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for output                                        */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade* ibb;            
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for input                                         */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade* obb;
  apr_size_t          len;
  apr_bucket*         tmp_heap;
  apr_bucket*         eos;
  const char*         data;
  char*               data_bucket;
  char*               data_brigade;
  char*               content_type;
  device_table*       spec ;
  char*               user_agent;
  mod_chxj_config*    dconf;
  chxjconvrule_entry* entryp;

  DBG(r, "start of chxj_input_filter()");

  data_brigade = qs_alloc_zero_byte_string(r);


  ibb = apr_brigade_create(r->pool, c->bucket_alloc);
  obb = apr_brigade_create(r->pool, c->bucket_alloc);

  content_type = (char*)apr_table_get(r->headers_in, "Content-Type");
  if (content_type 
  && strncasecmp("multipart/form-data", content_type, 19) == 0) {

    DBG(r, "detect multipart/form-data");
    ap_remove_input_filter(f);

    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (!entryp || !(entryp->action & CONVRULE_ENGINE_ON_BIT)) {
    DBG(r,"EngineOff");

    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }

  user_agent = (char*)apr_table_get(r->headers_in, "User-Agent");
  spec = chxj_specified_device(r, user_agent);

  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
    break;

  default:
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }


  rv = ap_get_brigade(f->next, ibb, mode, block, readbytes);
  if (rv != APR_SUCCESS) {
    DBG(r, "ap_get_brigade() failed");
    return rv;
  }

  APR_BRIGADE_FOREACH(b, ibb) {
    rv = apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
    if (rv != APR_SUCCESS) {
      DBG(r, "apr_bucket_read() failed");
      return rv;
    }

    if (data != NULL) {
      data_bucket = apr_palloc(r->pool, len+1);
      memset((void*)data_bucket, 0, len+1);
      memcpy(data_bucket, data, len);
      DBG1(r, "(in)POSTDATA:[%s]", data_bucket);
  
      data_brigade = apr_pstrcat(r->pool, data_brigade, data_bucket, NULL);
    }

    if (APR_BUCKET_IS_EOS(b)) {
      break;
    }
  }
  apr_brigade_cleanup(ibb);


  len = strlen(data_brigade);
  if (len == 0) {
    DBG(r,"data_brigade length is 0");
    DBG(r,"end of chxj_input_filter()");
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }
  data_brigade = chxj_input_convert(
    r, 
    (const char**)&data_brigade, 
    (apr_size_t*)&len,
    entryp
    );

  if (len > 0) {
    DBG1(r, "(in:exchange)POSTDATA:[%s]", data_brigade);

    obb = apr_brigade_create(r->pool, c->bucket_alloc);

    tmp_heap = apr_bucket_heap_create(data_brigade, 
                                      len, 
                                      NULL, 
                                      f->c->bucket_alloc);
    eos      = apr_bucket_eos_create(f->c->bucket_alloc);

    APR_BRIGADE_INSERT_TAIL(obb, tmp_heap);
    APR_BRIGADE_INSERT_TAIL(obb, eos);
    APR_BRIGADE_CONCAT(bb, obb);
  }

  DBG(r, "end of chxj_input_filter()");

  return APR_SUCCESS;
}


static mod_chxj_global_config*
chxj_global_config_create(apr_pool_t* pool, server_rec* s)
{
  mod_chxj_global_config* conf;

  SDBG(s, "start chxj_global_config_create()");

  /*--------------------------------------------------------------------------*/
  /* allocate an own subpool which survives server restarts                   */
  /*--------------------------------------------------------------------------*/
  conf = (mod_chxj_global_config*)apr_palloc(pool, 
                  sizeof(mod_chxj_global_config));
#if 0
  conf->cookie_db_lock = NULL;
#endif
  SDBG(s, "end   chxj_global_config_create()");

  return conf;
}

/**
 * initialize chxj module
 */
static int 
chxj_init_module(apr_pool_t *p, 
                  apr_pool_t *plog, 
                  apr_pool_t *ptemp, 
                  server_rec *s)
{
  mod_chxj_global_config* conf;
  void *user_data;

  SDBG(s, "start chxj_init_module()");

  apr_pool_userdata_get(&user_data, CHXJ_MOD_CONFIG_KEY, s->process->pool);
  SDBG(s, " ");
  if (user_data == NULL) {
    SDBG(s, " ");
    /*
     * dummy user_data set.
     */
    apr_pool_userdata_set(
      (const void *)(1), 
      CHXJ_MOD_CONFIG_KEY, 
      apr_pool_cleanup_null, 
      s->process->pool);
    SDBG(s, "end  chxj_init_module()");
    return OK;
  }

  ap_add_version_component(p, CHXJ_VERSION_PREFIX CHXJ_VERSION);


#if 0
  conf = (mod_chxj_global_config *)ap_get_module_config(s->module_config, 
                                                        &chxj_module);

  if (apr_global_mutex_create(&(conf->cookie_db_lock), 
                              NULL, APR_LOCK_DEFAULT, p) != APR_SUCCESS) {
    SERR(s, "end  chxj_init_module()");
    return HTTP_INTERNAL_SERVER_ERROR;
  }

#ifdef AP_NEED_SET_MUTEX_PERMS
  if (unixd_set_global_mutex_perms(conf->cookie_db_lock) != APR_SUCCESS) {
    SERR(s, "end  chxj_init_module()");
    return HTTP_INTERNAL_SERVER_ERROR;
  }
#endif
#endif


  SDBG(s, "end  chxj_init_module()");

  return OK;
}

static void 
chxj_child_init(apr_pool_t *p, server_rec *s)
{
#if 0
  mod_chxj_global_config* conf;
#endif

  SDBG(s, "start chxj_child_init()");

#if 0
  conf = (mod_chxj_global_config*)ap_get_module_config(s->module_config, 
                                                       &chxj_module);

  if (apr_global_mutex_child_init(&conf->cookie_db_lock, NULL, p) 
  != APR_SUCCESS) {
    SERR(s, "Can't attach global mutex.");
    return;
  }
#endif

  SDBG(s, "end   chxj_child_init()");
}


/**
 * A set structure of each server is generated. 
 * 
 * @param p
 * @param s
 */
static void*
chxj_config_server_create(apr_pool_t *p, server_rec *s)
{
  mod_chxj_global_config *gc;

  gc = chxj_global_config_create(p,s);

  return gc;
}


static int
chxj_translate_name(request_rec *r)
{
  return chxj_trans_name(r);
}


/**
 * The hook is registered.
 *
 * @param p
 */
static void 
chxj_register_hooks(apr_pool_t *p)
{
  ap_hook_post_config(chxj_init_module,
                      NULL,
                      NULL,
                      APR_HOOK_REALLY_FIRST);
  ap_hook_child_init(chxj_child_init, 
                     NULL, 
                     NULL, 
                     APR_HOOK_REALLY_FIRST);
  ap_register_output_filter (
                      "chxj_output_filter", 
                      chxj_output_filter, 
                      NULL, 
                      AP_FTYPE_RESOURCE);
  ap_register_input_filter(
                      "chxj_input_filter", 
                      chxj_input_filter, 
                      NULL, 
                      AP_FTYPE_RESOURCE);
  ap_hook_handler(chxj_img_conv_format_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_qr_code_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_translate_name(chxj_translate_name, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_fixups(chxj_headers_fixup, NULL, NULL, APR_HOOK_LAST);
}

/**
 * A set structure according to the directory is generated. 
 *
 * @param p
 * @param arg
 */
static void* 
chxj_create_per_dir_config(apr_pool_t *p, char *arg) 
{
  mod_chxj_config* conf;

  conf = apr_pcalloc(p, sizeof(mod_chxj_config));
  conf->device_data_file = NULL;
  conf->devices          = NULL;
  conf->emoji_data_file  = NULL;
  conf->emoji            = NULL;
  conf->emoji_tail       = NULL;
  conf->image            = CHXJ_IMG_OFF;
  conf->image_cache_dir  = apr_psprintf(p, "%s",DEFAULT_IMAGE_CACHE_DIR);
  conf->server_side_encoding = NULL;
  if (arg == NULL) {
    conf->dir                  = NULL;
  }
  else {
    conf->dir                  = apr_pcalloc(p, strlen(arg)+1);
    memset(conf->dir, 0, strlen(arg)+1);
    strcpy(conf->dir, arg);
  }
  conf->convrules   = apr_array_make(p, 2, sizeof(chxjconvrule_entry));

  /* Default is copyleft */
  conf->image_copyright = NULL; 

  return conf;
}
/*
 *  Merge per-directory CHXJ configurations
 */
static void*
chxj_merge_per_dir_config(apr_pool_t *p, void *basev, void *addv)
{
  mod_chxj_config *base = (mod_chxj_config*)basev;
  mod_chxj_config *add  = (mod_chxj_config*)addv;
  mod_chxj_config *mrg  = (mod_chxj_config*)apr_palloc(p, sizeof(mod_chxj_config));

  mrg->device_data_file = NULL;
  mrg->devices          = NULL;
  mrg->emoji_data_file  = NULL;
  mrg->image            = CHXJ_IMG_OFF;
  mrg->image_cache_dir  = NULL;
  mrg->image_copyright  = NULL;
  mrg->emoji            = NULL;
  mrg->emoji_tail       = NULL;

  mrg->dir = apr_pstrdup(p, add->dir);

  if (! add->device_data_file) {
    mrg->devices = base->devices;
    mrg->device_data_file = apr_pstrdup(p, base->device_data_file);
  }
  else {
    mrg->devices = add->devices;
    mrg->device_data_file = apr_pstrdup(p, add->device_data_file);
  }

  if (! add->emoji_data_file) {
    mrg->emoji = base->emoji;
    mrg->emoji_tail = base->emoji_tail;
    mrg->emoji_data_file = apr_pstrdup(p, base->emoji_data_file);
  }
  else {
    mrg->emoji = add->emoji;
    mrg->emoji_tail = add->emoji_tail;
    mrg->emoji_data_file = apr_pstrdup(p, add->emoji_data_file);
  }

  if (add->image == CHXJ_IMG_OFF) 
    mrg->image = base->image;
  else 
    mrg->image = add->image;


  if (strcasecmp(add->image_cache_dir ,DEFAULT_IMAGE_CACHE_DIR)==0) 
    mrg->image_cache_dir = apr_pstrdup(p, base->image_cache_dir);
  else 
    mrg->image_cache_dir = apr_pstrdup(p, add->image_cache_dir);

  if (add->image_copyright) 
    mrg->image_copyright = apr_pstrdup(p, add->image_copyright);
  else
    mrg->image_copyright = apr_pstrdup(p, base->image_copyright);

  if (add->server_side_encoding) {
    mrg->server_side_encoding = apr_pstrdup(p, add->server_side_encoding);
  }
  else 
  if (base->server_side_encoding) {
    mrg->server_side_encoding = apr_pstrdup(p, base->server_side_encoding);
  }
  else {
    mrg->server_side_encoding = apr_pstrdup(p, DEFAULT_SERVER_SIDE_ENCODING);
  }

  mrg->convrules    = apr_array_append(p, add->convrules, base->convrules);

  return mrg;
}


static int
chxj_command_parse_take5(
  const char* arg, 
  char** prm1, 
  char** prm2, 
  char** prm3, 
  char** prm4, 
  char** prm5)
{
  int isquoted;
  char* strp;

  strp = (char*)arg;

  for (;*strp == ' '||*strp == '\t'; strp++) ;

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }

  *prm1 = strp;

  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm2 = strp;
    *prm3 = strp;
    *prm4 = strp;
    *prm5 = strp;
    return 1;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++) ;

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }

  *prm2 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm3 = strp;
    *prm4 = strp;
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm3 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm4 = strp;
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm4 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm5 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }
  *strp = '\0';

  return 0;
}

/**
 * The device definition file is loaded. 
 *
 * @param arg     [i]   The name of the device definition file is specified.
 * @param mconfig [i/o] The pointer to a set structure is specified. 
 * @param parms   [i]   
 */
static const char* 
cmd_load_device_data(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;
  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "device data filename too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->device_data_file = apr_pstrdup(parms->pool, arg);

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc*)&doc, (const char*)arg);
  chxj_load_device_data(&doc,parms->pool, conf);
  qs_all_free(&doc, QX_LOGMARK);

  return NULL;
}

/**
 * Device definition information is loaded. 
 *
 * @param parms 
 * @param arg     [i]   The name of the device definition file is specified. 
 * @param mconfig [i/o] The pointer to a set structure is specified. 
 * @return 
 */
static const char* 
cmd_load_emoji_data(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  char* rtn;
  Doc doc;
  doc.r = NULL;


  if (strlen(arg) > 256) 
    return "emoji data filename too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->emoji_data_file = apr_pstrdup(parms->pool, arg);
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc*)&doc, (const char*)arg);
  rtn = chxj_load_emoji_data(&doc,parms->pool, conf);
  qs_all_free(&doc, QX_LOGMARK);


  return rtn;
}

static const char* 
cmd_set_image_engine(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;


  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "image uri is too long.";

  conf = (mod_chxj_config*)mconfig;
  if (strcasecmp("ON", arg) == 0)
    conf->image = CHXJ_IMG_ON;
  else
    conf->image = CHXJ_IMG_OFF;


  return NULL;
}

static const char* 
cmd_set_image_cache_dir(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;
  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "cache dir name is too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->image_cache_dir = apr_pstrdup(parms->pool, arg);

  return NULL;
}

static const char* 
cmd_set_image_copyright(cmd_parms *parms, void* mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;

  doc.r = NULL;
  if (strlen(arg) > 256) 
    return "Copyright Flag is too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->image_copyright = apr_pstrdup(parms->pool, arg);

  return NULL;
}

static const char*
cmd_convert_rule(cmd_parms *cmd, void* mconfig, const char *arg)
{
  mod_chxj_config* dconf;
  char* prm1;
  char* prm2;
  char* prm3;
  char* prm4;
  char* prm5;
  int mode;
  char* pstate;
  char* action;
  char* pp;
  ap_regex_t *regexp;
  chxjconvrule_entry* newrule;

  dconf = (mod_chxj_config*)mconfig;

  if (strlen(arg) > 4096) 
    return "ChxjConvertRule: is too long.";

  dconf = (mod_chxj_config*)mconfig;
  if (dconf->convrules == NULL)
    dconf->convrules   = apr_array_make(cmd->pool, 2, sizeof(chxjconvrule_entry));


  newrule = apr_array_push(dconf->convrules);

  newrule->flags  = 0;
  newrule->action = 0;

  if (chxj_command_parse_take5(arg, &prm1, &prm2, &prm3, &prm4, &prm5))
    return "ChxjConvertRule: bad argument line";

  newrule->pattern = apr_pstrdup(cmd->pool, prm1);

  /* Parse action */
  for (;;) {
    if ((action = apr_strtok(prm2, ",", &pstate)) == NULL)
      break;
    prm2 = NULL;
    switch(*action) {
    case 'e':
    case 'E':
      if (strcasecmp(CONVRULE_ENGINE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_ENGINE_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_ENGINE_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_ENGINE_OFF_BIT;
      }
      break;

    case 'C':
    case 'c':
      if (strcasecmp(CONVRULE_COOKIE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_COOKIE_ON_BIT;
      }
      break;

    default:
      break;
    }
  }
  
  pp = prm1;
  if (*pp == '!') {
    newrule->flags |= CONVRULE_FLAG_NOTMATCH;
    pp++;
  }

  mode = AP_REG_EXTENDED;
  if ((regexp = ap_pregcomp(cmd->pool, pp, mode)) == NULL) {
    return "RewriteRule: cannot compile regular expression ";
  }

  newrule->regexp = regexp;
  if (*prm3)
    newrule->encoding = apr_pstrdup(cmd->pool, prm3);
  else
    newrule->encoding = apr_pstrdup(cmd->pool, "none");

  newrule->pc_flag = CONVRULE_PC_FLAG_OFF_BIT;
  if (*prm4)
    if (strcasecmp(CONVRULE_PC_FLAG_ON_CMD, prm4) == 0)
      newrule->pc_flag = CONVRULE_PC_FLAG_ON_BIT;

  newrule->user_agent = NULL;
  if (*prm5)
    newrule->user_agent = apr_pstrdup(cmd->pool, prm5);
    
  return NULL;
}


static const command_rec cmds[] = {
  AP_INIT_TAKE1(
    "ChxjLoadDeviceData",
    cmd_load_device_data,
    NULL,
    OR_ALL,
    "Load Device Data"),
  AP_INIT_TAKE1(
    "ChxjLoadEmojiData",
    cmd_load_emoji_data,
    NULL,
    OR_ALL,
    "Load Emoji Data"),
  AP_INIT_TAKE1(
    "ChxjImageEngine",
    cmd_set_image_engine,
    NULL,
    OR_ALL,
    "Convert Target URI"),
  AP_INIT_TAKE1(
    "ChxjImageCacheDir",
    cmd_set_image_cache_dir,
    NULL,
    OR_ALL,
    "Image Cache Directory"),
  AP_INIT_TAKE1(
    "ChxjImageCopyright",
    cmd_set_image_copyright,
    NULL,
    OR_ALL,
    "Copyright Flag"),
  AP_INIT_RAW_ARGS(
    "ChxjConvertRule",
    cmd_convert_rule,
    NULL, 
    OR_FILEINFO,
    "an URL-applied regexp-pattern and a substitution URL"),
  { NULL },
};


/*----------------------------------------------------------------------------*/
/* Dispatch list for API hooks                                                */
/*----------------------------------------------------------------------------*/
module AP_MODULE_DECLARE_DATA chxj_module = 
{
  STANDARD20_MODULE_STUFF, 
  chxj_create_per_dir_config,          /* create per-dir    config structures */
  chxj_merge_per_dir_config,           /* merge  per-dir    config structures */
  chxj_config_server_create,           /* create per-server config structures */
  NULL,                                /* merge  per-server config structures */
  cmds,                                /* table of config file commands       */
  chxj_register_hooks                  /* register hooks                      */
};
/*
 * vim:ts=2 et
 */
