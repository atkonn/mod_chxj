/*
 * Copyright (C) 2005-2011 Atsushi Konno All rights reserved.
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
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
#include <limits.h>
#include <errno.h>

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
#include "apr_file_info.h"

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
#include "chxj_chtml40.h"
#include "chxj_chtml50.h"
#include "chxj_ixhtml10.h"
#include "chxj_jhtml.h"
#include "chxj_jxhtml.h"
#include "chxj_iphone.h"
#include "chxj_android.h"
#include "chxj_img_conv_format.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_apply_convrule.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"
#include "chxj_dump_string.h"
#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#include "chxj_serf.h"
#include "chxj_add_device_env.h"
#include "chxj_header_inf.h"
#include "chxj_jreserved_tag.h"
#include "chxj_google.h"


#define CHXJ_VERSION_PREFIX PACKAGE_NAME "/"
#define CHXJ_VERSION        PACKAGE_VERSION
#define CHXJ_POST_MAX       (0x1000000)

converter_t convert_routine[] = {
  {
    /* CHXJ_SPEC_UNKNOWN          */
    .converter            = NULL,
    .encoder              = NULL,
    .emoji_only_converter = NULL,
  },
  {
    /* CHXJ_SPEC_Chtml_1_0        */
    .converter            = chxj_convert_chtml10,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_chtml10_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Chtml_2_0        */
    .converter            = chxj_convert_chtml20,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_chtml20_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Chtml_3_0        */
    .converter            = chxj_convert_chtml30,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_chtml30_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Chtml_4_0        */
    .converter            = chxj_convert_chtml40,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_chtml40_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Chtml_5_0        */
    .converter            = chxj_convert_chtml50,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_chtml50_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Chtml_6_0        */
    .converter = chxj_convert_ixhtml10,
    .encoder  = chxj_encoding,
    .emoji_only_converter = chxj_chtml50_emoji_only_converter, /* XXX: TODO */
  },
  {
    /* CHXJ_SPEC_Chtml_7_0        */
    .converter = chxj_convert_ixhtml10,
    .encoder  = chxj_encoding,
    .emoji_only_converter = chxj_chtml50_emoji_only_converter, /* XXX: TODO */
  },
  {
    /* CHXJ_SPEC_XHtml_Mobile_1_0 */
    .converter            = chxj_convert_xhtml_mobile_1_0,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_xhtml_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Hdml             */
    .converter            = chxj_convert_hdml,
    .encoder              = chxj_encoding,
    .emoji_only_converter = NULL,
  },
  {
    /* CHXJ_SPEC_Jhtml            */
    .converter            = chxj_convert_jhtml,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_jhtml_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_Jxhtml            */
    .converter            = chxj_convert_jxhtml,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_jxhtml_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_iPhone2          */
    .converter            = chxj_convert_iphone,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_iphone_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_iPhone3          */
    .converter            = chxj_convert_iphone,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_iphone_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_iPhone4          */
    .converter            = chxj_convert_iphone,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_iphone_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_softbank_android          */
    .converter            = chxj_convert_android,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_android_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_au_android          */
    .converter            = chxj_convert_android,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_android_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_docomo_android          */
    .converter            = chxj_convert_android,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_android_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_android          */
    .converter            = chxj_convert_android,
    .encoder              = chxj_encoding,
    .emoji_only_converter = chxj_android_emoji_only_converter,
  },
  {
    /* CHXJ_SPEC_HTML             */
    .converter = NULL,
    .encoder  = NULL,
    .emoji_only_converter = NULL,
  },
};

static int chxj_convert_input_header(request_rec *r,chxjconvrule_entry *entryp, device_table *spec);
static void s_convert_guid_parameter_to_header(request_rec *r, const char *param, device_table *spec);
static void s_add_cookie_id_if_has_location_header(request_rec *r, cookie_t *cookie);
static void s_clear_cookie_header(request_rec *r, device_table *spec);
static void s_add_no_cache_headers(request_rec *r, chxjconvrule_entry  *entryp);

mod_chxj_req_config *
chxj_get_req_config(request_rec *r)
{
  mod_chxj_req_config *request_conf; 
  request_conf = (mod_chxj_req_config *)chxj_get_module_config(r->request_config, &chxj_module);
  if (! request_conf) {
    request_conf = apr_pcalloc(r->pool, sizeof(mod_chxj_req_config));
    request_conf->spec = NULL;
    request_conf->user_agent = NULL;
    request_conf->f = NULL;
    request_conf->entryp = NULL;
    chxj_set_module_config(r->request_config, &chxj_module, request_conf);
  }
  return request_conf;
}

/**
 * Only when User-Agent is specified, the User-Agent header is camouflaged. 
 *
 * @param r   [i]
 */
static apr_status_t 
chxj_headers_fixup(request_rec *r)
{
  mod_chxj_config*    dconf; 
  mod_chxj_req_config *request_conf; 
  chxjconvrule_entry* entryp;
  char*               user_agent;
  device_table*       spec;
  char                *contentType;
  char                *contentLength;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r), __func__);

  request_conf = chxj_get_req_config(r);
  if (r->main) {
    DBG(r, "REQ[%X] detect internal redirect.", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()",  TO_ADDR(r),__func__);
    return DECLINED;
  }
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);

  /*
   * check and get mobile type.
   * and request_conf->user_agent , request_conf->spec is set.
   */
  spec = chxj_specified_device(r, user_agent);

  contentType = (char *)apr_table_get(r->headers_in, "Content-Type");
  if (contentType
      && strncasecmp("multipart/form-data", contentType, 19) == 0) {
    DBG(r, "REQ[%X] detect multipart/form-data ==> no target", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return DECLINED;
  }
  if (dconf->use_google_analytics) {
    if (!dconf->google_analytics_target) {
      ERR(r, "Please set ChxjGoogleAnalyticsTarget.");
    }
    else if (!dconf->google_analytics_account) {
      ERR(r, "Please set ChxjGoogleAnalyticsAccount.");
    }
    else if (r->uri && dconf->google_analytics_target && strcasecmp(r->uri, dconf->google_analytics_target) == 0) {
      r->proxyreq = PROXYREQ_NONE;
      r->handler = apr_psprintf(r->pool, "chxj-google-analytics-handler");
  
      DBG(r, "REQ[%X] detect google analytics handler", TO_ADDR(r));
      DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  
      return OK;
    }
  }


  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
  case CHXJ_SPEC_Jxhtml:
  case CHXJ_SPEC_iPhone2:
  case CHXJ_SPEC_iPhone3:
  case CHXJ_SPEC_iPhone4:
  case CHXJ_SPEC_softbank_android:
  case CHXJ_SPEC_au_android:
  case CHXJ_SPEC_docomo_android:
  case CHXJ_SPEC_android:
    request_conf->entryp = entryp = chxj_apply_convrule(r, dconf->convrules);
    if (dconf->image != CHXJ_IMG_ON) {
      if (! entryp) {
        DBG(r, "REQ[%X] end %s() (no pattern)", TO_ADDR(r), __func__);
        return DECLINED;
      }
      if (!(entryp->action & CONVRULE_ENGINE_ON_BIT) && !(entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
        DBG(r, "REQ[%X] end %s() (engine off)", TO_ADDR(r), __func__);
        return DECLINED;
      }
      if (entryp->action & CONVRULE_EMOJI_ONLY_BIT) {
        DBG(r, "REQ[%X] end %s() (emoji only)", TO_ADDR(r), __func__);
        return DECLINED;
      } 
    } 
    apr_table_setn(r->headers_in, 
                   CHXJ_HTTP_USER_AGENT, 
                   user_agent);
  
    if (entryp && entryp->user_agent)
      apr_table_setn(r->headers_in, 
                     HTTP_USER_AGENT, 
                     entryp->user_agent);
    /*
     * this clear process must do before chxj_convert_input_header function.
     */
    if (entryp) {
      if ((entryp->action & CONVRULE_COOKIE_ON_BIT) || (entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
        if (r->method_number == M_POST) {
          if (! apr_table_get(r->headers_in, "X-Chxj-Forward")) {
            s_clear_cookie_header(r, spec);
          }
        }
        else {
          s_clear_cookie_header(r, spec);
        }
      }
    }

    chxj_convert_input_header(r,entryp, spec);

    break;
  
  default:
    DBG(r, "REQ[%X] end %s() (not mobile) spec->device_name[%s] User-Agent:[%s]", TO_ADDR(r),__func__, spec->device_name, user_agent);
    return DECLINED;

  }

  char *x_client_type = (char *)apr_table_get(r->headers_out, "X-Client-Type");
  apr_table_unset(r->headers_out, "X-Client-Type");
  if (x_client_type) {
    apr_table_setn(r->headers_in, "X-Client-Type", x_client_type);
  }
  else {
    apr_table_setn(r->headers_in, "X-Client-Type", "");
  }

  if (r->method_number == M_POST) {
    if (! apr_table_get(r->headers_in, "X-Chxj-Forward")) {
      DBG(r, "REQ[%X] set Input handler old:[%s] proxyreq:[%d] uri:[%s] filename:[%s]", TO_ADDR(r), r->handler, r->proxyreq, r->uri, r->filename);
      r->proxyreq = PROXYREQ_NONE;
      r->handler = apr_psprintf(r->pool, "chxj-input-handler");
    }
    else {
      char *client_ip = (char *)apr_table_get(r->headers_in, CHXJ_HEADER_ORIG_CLIENT_IP);
      if (client_ip) {
        if (dconf->post_log) {
          apr_table_setn(r->subprocess_env, dconf->post_log, dconf->post_log);
        }
        else {
          apr_table_setn(r->subprocess_env, "chxj-post-log", "chxj-post-log");
        }
        apr_sockaddr_t *address = NULL;
        apr_status_t rv = apr_sockaddr_info_get(&address, ap_get_server_name(r), APR_UNSPEC, ap_get_server_port(r), 0, r->pool);
        if (rv != APR_SUCCESS) {
          char buf[256];
          ERR(r, "REQ[%X] %s:%d apr_sockaddr_info_get() failed: rv:[%d|%s]", TO_ADDR(r), APLOG_MARK, rv, apr_strerror(rv, buf, 256));
          DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
          return DECLINED;
        }
        char *addr;
        if (dconf->forward_server_ip) {
          addr = dconf->forward_server_ip;
        }
        else {
          apr_sockaddr_ip_get(&addr, address);
        }
        DBG(r, "REQ[%X] Client IP:[%s] vs Orig Client IP:[%s] vs Server IP:[%s]", TO_ADDR(r), r->connection->remote_ip, client_ip, addr);
        if (strcmp(addr, r->connection->remote_ip) == 0) {
          r->connection->remote_ip = apr_pstrdup(r->connection->pool, client_ip);
          /* For mod_cidr_lookup */
          if (entryp && (entryp->action & CONVRULE_OVERWRITE_X_CLIENT_TYPE_BIT)) {
            char *client_type = (char *)apr_table_get(r->headers_in, CHXJ_HEADER_ORIG_CLIENT_TYPE);
            DBG(r, "REQ[%X] Overwrite X-Client-Type to [%s]", TO_ADDR(r), client_type);
            if (client_type) {
              apr_table_setn(r->subprocess_env, "X_CLIENT_TYPE", client_type);
              apr_table_setn(r->headers_in, "X-Client-Type", client_type);
            }
            else {
              apr_table_unset(r->headers_in, "X-Client-Type");
            }
          }
        }
        if (! apr_table_get(r->headers_in, "Content-Length")) {
          contentLength = (char *)apr_table_get(r->headers_in, "X-Chxj-Content-Length");
          if (contentLength) {
            apr_table_set(r->headers_in, "Content-Length", contentLength);
          }
        }
      }
    }
  }
  else{
    if(r->content_type && strncmp(r->content_type,"image/",6) == 0){
      if (dconf->image_rewrite == CHXJ_IMG_REWRITE_ON && !apr_table_get(r->headers_in, CHXJ_IMG_X_HTTP_IMAGE_FILENAME)){
        if(dconf->image_rewrite_mode == CHXJ_IMG_REWRITE_MODE_ALL){
          // all image
          apr_table_set(r->headers_in, CHXJ_IMG_X_HTTP_IMAGE_FILENAME, r->filename);
          apr_table_set(r->headers_in, CHXJ_IMG_X_HTTP_IMAGE_TYPE,r->content_type);
          r->filename = apr_pstrcat(r->pool,"img-redirect:",dconf->image_rewrite_url,NULL);
          r->handler = "chxj-image-redirect-handler";
          return OK;
        }
        else{
          //   has _chxj_imgrewrite=on in args
          char *args_tmp = chxj_url_decode(r->pool, r->args);
          if (strstr(args_tmp,CHXJ_IMG_REWRITE_URL_STRING)){
            apr_table_set(r->headers_in, CHXJ_IMG_X_HTTP_IMAGE_FILENAME, r->filename);
            apr_table_set(r->headers_in, CHXJ_IMG_X_HTTP_IMAGE_TYPE,r->content_type);
            r->filename = apr_pstrcat(r->pool,"img-redirect:",dconf->image_rewrite_url,NULL);
            r->handler = "chxj-image-redirect-handler";
            return OK;
          }
        }
      }
    }
  }

  chxj_add_device_env(r, spec);

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

  return DECLINED;
}

static int
chxj_image_redirect_handler(request_rec *r)
{

  if (strcmp(r->handler, "chxj-image-redirect-handler")) {
    return DECLINED;
  }

  if (strncmp(r->filename, "img-redirect:", 13) != 0) {
    return DECLINED;
  }
  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  ap_internal_redirect(apr_pstrcat(r->pool, r->filename+13,
                                       r->args ? "?" : NULL, r->args, NULL), r);
  DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
  return OK;
}


static void
s_clear_cookie_header(request_rec *r, device_table *spec)
{
  char *sv;
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Jhtml:
  case CHXJ_SPEC_Jxhtml:
    sv = (char *)apr_table_get(r->headers_in, "Cookie");
    if (sv) {
      apr_table_setn(r->headers_in, "X-Chxj-Org-Cookie", sv);
    }
    apr_table_unset(r->headers_in, "Cookie");
    break;
  case CHXJ_SPEC_iPhone2:
  case CHXJ_SPEC_iPhone3:
  case CHXJ_SPEC_iPhone4:
  case CHXJ_SPEC_softbank_android:
  case CHXJ_SPEC_au_android:
  case CHXJ_SPEC_docomo_android:
  case CHXJ_SPEC_android:
  default:
    break;
  }
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


/**
 * It converts it from CHTML into XXML corresponding to each model. 
 *
 * @param r   [i]
 * @param src [i]   It is former HTML character string. 
 * @param len [i/o] It is length of former HTML character string. 
 */
static char * 
chxj_convert(request_rec *r, const char **src, apr_size_t *len, device_table *spec, const char *ua, cookie_t **cookiep)
{
  char                *user_agent;
  char                *dst;
  char                *tmp;
  cookie_t            *cookie;
  mod_chxj_config     *dconf; 
  mod_chxj_req_config *request_conf; 
  chxjconvrule_entry  *entryp;

  DBG(r,"REQ[%X] start %s()", TO_ADDR(r),__func__);

  chxj_dump_string(r, APLOG_MARK, "INPUT Data", *src, *len);

  dst  = apr_pstrcat(r->pool, (char *)*src, NULL);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  request_conf = chxj_get_req_config(r);


  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (request_conf->user_agent) {
    entryp = request_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }

  if (!entryp 
      || (     !(entryp->action & CONVRULE_ENGINE_ON_BIT) 
            && !(entryp->action & CONVRULE_COOKIE_ONLY_BIT)
            && !(entryp->action & CONVRULE_EMOJI_ONLY_BIT)
         )) {
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
    return (char *)*src;
  }


  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  if (entryp->user_agent)
    user_agent = (char *)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
  else
    user_agent = (char *)apr_table_get(r->headers_in, HTTP_USER_AGENT);

  DBG(r,"REQ[%X] User-Agent:[%s]", TO_ADDR(r), user_agent);
  DBG(r,"REQ[%X] content type is [%s]", TO_ADDR(r), r->content_type);


  if (  ! STRNCASEEQ('t','T', "text/html", r->content_type, sizeof("text/html")-1)
    &&  ! STRNCASEEQ('a','A', "application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)) {
    DBG(r,"REQ[%X] no convert. content type is [%s]", TO_ADDR(r), r->content_type);
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
    return (char *)*src;
  }

  if (ua && user_agent && strcasecmp(user_agent, ua) != 0) {
    /* again */
    spec = chxj_specified_device(r, user_agent);
  }
  else {
    spec = request_conf->spec;
  }

  /*
   * save cookie.
   */
  cookie = NULL;
  if ((entryp->action & CONVRULE_COOKIE_ON_BIT 
      && !(entryp->action & CONVRULE_EMOJI_ONLY_BIT)
      && !(entryp->action & CONVRULE_ENVINFO_ONLY_BIT))
      || (entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
    switch(spec->html_spec_type) {
    case CHXJ_SPEC_Chtml_1_0:
    case CHXJ_SPEC_Chtml_2_0:
    case CHXJ_SPEC_Chtml_3_0:
    case CHXJ_SPEC_Chtml_4_0:
    case CHXJ_SPEC_Chtml_5_0:
    case CHXJ_SPEC_Chtml_6_0:
    case CHXJ_SPEC_Chtml_7_0:
    case CHXJ_SPEC_XHtml_Mobile_1_0:
    case CHXJ_SPEC_Jhtml:
    case CHXJ_SPEC_Jxhtml:
      cookie = chxj_save_cookie(r);
      break;
    case CHXJ_SPEC_iPhone2:
    case CHXJ_SPEC_iPhone3:
    case CHXJ_SPEC_iPhone4:
    case CHXJ_SPEC_softbank_android:
    case CHXJ_SPEC_au_android:
    case CHXJ_SPEC_docomo_android:
    case CHXJ_SPEC_android:
    default:
      break;
    }
  }

  if (!r->header_only) {

    if ((entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
      if (cookie) {
        dst = chxj_cookie_only_mode(r, *src, (apr_size_t *)len, cookie);
      }
      else {
        /* ignore */
      }
    }
    else if (!(entryp->action & CONVRULE_ENGINE_ON_BIT) && (entryp->action & CONVRULE_EMOJI_ONLY_BIT)) {
      /*---------------------------------------------------------------------*/
      /* force emoji only mode                                               */
      /*---------------------------------------------------------------------*/
      tmp = NULL;
      if (convert_routine[spec->html_spec_type].encoder)
        tmp = convert_routine[spec->html_spec_type].encoder(r, 
                                                            *src, 
                                                            (apr_size_t *)len);
      if (convert_routine[spec->html_spec_type].emoji_only_converter) {
        dst = convert_routine[spec->html_spec_type].emoji_only_converter(r,spec, tmp,*len);
        if (dst != NULL) {
          *len = strlen(dst);
        }
        else {
          dst = apr_palloc(r->pool, 1);
          *dst = 0;
          *len = 0;
        }
      }
    }
    else {
      tmp = NULL;
      if (convert_routine[spec->html_spec_type].encoder)
        tmp = convert_routine[spec->html_spec_type].encoder(r, 
                                                            *src, 
                                                            (apr_size_t *)len);
  
      if (entryp->action & CONVRULE_EMOJI_ONLY_BIT) {
        if (tmp) {
          tmp = chxj_node_convert_chxjif_only(r, spec, (const char*)tmp, (apr_size_t *)len);
        }
        else {
          tmp = chxj_node_convert_chxjif_only(r, spec, (const char *)*src, (apr_size_t *)len);
        }
        if (convert_routine[spec->html_spec_type].emoji_only_converter) {
          dst = convert_routine[spec->html_spec_type].emoji_only_converter(r,spec, tmp,*len);
          if (dst != NULL) {
            *len = strlen(dst);
          }
          else {
            dst = apr_palloc(r->pool, 1);
            *dst = 0;
            *len = 0;
          }
        }
        DBG(r, "REQ[%X] end %s()(emoji only)", TO_ADDR(r),__func__);
      }
  
      if (   !(entryp->action & CONVRULE_EMOJI_ONLY_BIT) 
          && !(entryp->action & CONVRULE_ENVINFO_ONLY_BIT)) {
        if (convert_routine[spec->html_spec_type].converter) {
          if (tmp)
            dst = convert_routine[spec->html_spec_type].converter(r, 
                                                                  spec, 
                                                                  tmp, 
                                                                  *len, 
                                                                  len, 
                                                                  entryp, 
                                                                  cookie);
          else
            dst = convert_routine[spec->html_spec_type].converter(r,
                                                                  spec, 
                                                                  *src, 
                                                                  *len, 
                                                                  len, 
                                                                  entryp, 
                                                                  cookie);
        }
      }
    }
  }
  if (*len > 0){
    if (strcasecmp(spec->output_encoding,"UTF-8") == 0){
      dst = chxj_iconv(r,r->pool,dst,len,"CP932","UTF-8");
    }
  }


  ap_set_content_length(r, *len);

  if (*len == 0) {
    dst = apr_psprintf(r->pool, "\n");
    *len = 1;
  }
  dst[*len] = 0;
  if (cookie) {
    *cookiep = cookie;
  }



  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

  return dst;
}


/**
 * convert GUID parameter.
 *
 */
static void
s_convert_guid_parameter_to_header(request_rec *r, const char *param, device_table *spec)
{
  DBG(r, "REQ[%X] start %s() param:[%s]", TO_ADDR(r),__func__, param);
  if (strcasecmp(param, "guid") == 0) {
    switch(spec->html_spec_type) {
    case CHXJ_SPEC_XHtml_Mobile_1_0:
      do {
        char *x_up_subno = (char *)apr_table_get(r->headers_in, "x-up-subno");
        if (x_up_subno) {
          apr_table_setn(r->headers_in, "X-DCMGUID", x_up_subno);
        }
      } while(0);
      break;
    case CHXJ_SPEC_Jhtml:
    case CHXJ_SPEC_Jxhtml:
      do {
        char *x_jphone_uid = (char *)apr_table_get(r->headers_in, "x-jphone-uid");
        if (x_jphone_uid) {
          apr_table_setn(r->headers_in, "X-DCMGUID", x_jphone_uid);
        }
      } while(0);
      break;
    default:
      break;
    }
  }
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}

/**
 * It converts it from HEADER.
 *
 * @param r   [i]
 */
static int
chxj_convert_input_header(request_rec *r,chxjconvrule_entry *entryp, device_table *spec) 
{

  char       *buff;
  char       *buff_pre;
  apr_size_t urilen;
  char       *result;
  char       *pair;
  char       *name;
  char       *value;
  char       *pstate;
  char       *vstate;
  cookie_t   *cookie = NULL;
  int        no_update_flag = 0;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  if (! r->args) {
    DBG(r, "REQ[%X] r->args=[null]", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return 0;
  }
  urilen = strlen(r->args);

  result = qs_alloc_zero_byte_string(r->pool);

  buff_pre = apr_pstrdup(r->pool, r->args);

  for (;;) {
    char *pair_sv;

    pair = apr_strtok(buff_pre, "&", &pstate);
    if (pair == NULL)
      break;

    buff_pre = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (! name) continue;
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0 || strcasecmp(name, chxj_url_encode(r->pool, CHXJ_COOKIE_NOUPDATE_PARAM)) == 0) {
      DBG(r, "REQ[%X] found cookie no update parameter", TO_ADDR(r));
      no_update_flag++;
    }
  }

  buff = apr_pstrdup(r->pool, r->args);
  DBG(r, "REQ[%X] r->args=[%s]", TO_ADDR(r), buff);

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) {
    char *pair_sv;

    pair = apr_strtok(buff, "&", &pstate);
    if (pair == NULL)
      break;

    buff = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (! name) continue;
    name = chxj_safe_to_jreserved_tag(r, name);
    if (strncasecmp(name, "_chxj", 5) != 0 && strncasecmp(name, "%5Fchxj", sizeof("%5Fchxj")-1) != 0) {
      if (strlen(result) != 0) 
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (entryp && strcasecmp(entryp->encoding, "NONE") != 0) {
        apr_size_t dlen;
        char *dvalue;
        char *dname;

        if (value && *value != 0) {
          value = chxj_url_decode(r->pool, value);
          dlen   = strlen(value);
          DBG(r, "REQ[%X] ************ before encoding[%s]", TO_ADDR(r),value);
  
          dvalue = chxj_rencoding(r, value, &dlen,spec->output_encoding);
          dvalue = chxj_url_encode(r->pool, dvalue);
  
          DBG(r, "REQ[%X] ************ after encoding[%s]", TO_ADDR(r),dvalue);
        }
        else {
          dvalue = "";
        }

        if (name && *name != 0) {
          name = chxj_url_decode(r->pool, name);
          dlen = strlen(name);
          dname = chxj_rencoding(r, name, &dlen,spec->output_encoding);
          dname = chxj_url_encode(r->pool, dname);
        }
        else {
          dname = "";
        }
        s_convert_guid_parameter_to_header(r, dname, spec);
        result = apr_pstrcat(r->pool, result, dname, "=", dvalue, NULL);
      }
      else {
        s_convert_guid_parameter_to_header(r, name, spec);
        if (strcmp(name, pair_sv) != 0) {
          result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
        } 
        else {
          result = apr_pstrcat(r->pool, result, name, NULL);
        }
      }
    }
    else
    if ( (strncasecmp(name, "_chxj_c_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fc%5F", sizeof("%5Fchxj%5Fc%5F")-1) == 0)
      || (strncasecmp(name, "_chxj_r_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fr%5F", sizeof("%5Fchxj%5Fr%5F")-1) == 0)
      || (strncasecmp(name, "_chxj_s_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fs%5F", sizeof("%5Fchxj%5Fs%5F")-1) == 0)) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);

      result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0 || strcasecmp(name, "%5Fchxj%5Fcc") == 0) {
      if (! cookie) {
        s_clear_cookie_header(r, spec);
        DBG(r, "REQ[%X] found cookie parameter[%s]", TO_ADDR(r), value);
        cookie_lock_t *lock = chxj_cookie_lock(r);
        cookie = chxj_load_cookie(r, value);
        if (! no_update_flag && cookie) {
          cookie = chxj_update_cookie(r, cookie);
        }
        chxj_cookie_unlock(r, lock);
      }
      if (cookie && cookie->cookie_id) {
        if (strlen(result) != 0)
          result = apr_pstrcat(r->pool, result, "&", NULL);
        result = apr_pstrcat(r->pool, result, name, "=", cookie->cookie_id, NULL);
      }
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0) {
      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);
      result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
    }
  }
  apr_table_setn(r->headers_in, "X-Chxj-Cookie-No-Update", "true");
  if (! no_update_flag) {
    result = apr_pstrcat(r->pool, result, "&_chxj_nc=true", NULL);
  }
  r->args = result;

  DBG(r, "REQ[%X] result r->args=[%s]", TO_ADDR(r), r->args);
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  return 0;
}


/**
 * It converts it from POSTDATA .
 *
 * @param r   [i]
 * @param src [i]   It is POSTDATA character string.
 * @param len [i/o] It is length of former HTML character string.
 */
static char *
chxj_input_convert(
  request_rec         *r, 
  const char          **src, 
  apr_size_t          *len, 
  chxjconvrule_entry  *entryp,
  device_table        *spec)
{
  char     *pair;
  char     *name;
  char     *value;
  char     *pstate;
  char     *vstate;
  char     *s;
  char     *result;
  cookie_t *cookie = NULL;
  char     *buff_pre;
  int      no_update_flag = 0;
  apr_size_t ilen = 0;
  apr_pool_t *pool;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  if (! *src) {
    DBG(r, "REQ[%X] end %s() (input is null)", TO_ADDR(r),__func__);
    return apr_pstrdup(r->pool, "");
  }

  apr_pool_create(&pool, r->pool);

  s        = apr_pstrdup(pool, *src);
  ilen     = strlen(s);
  buff_pre = apr_pstrdup(pool, *src);

  result   = qs_alloc_zero_byte_string(pool);

  chxj_dump_string(r, APLOG_MARK, "BEFORE input convert source", s, ilen);

  for (;;) {
    char *pair_sv;

    pair = apr_strtok(buff_pre, "&", &pstate);
    if (pair == NULL)
      break;

    buff_pre = NULL;

    pair_sv = apr_pstrdup(pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (! name) continue;
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0 || strcasecmp(name, chxj_url_encode(r->pool, CHXJ_COOKIE_NOUPDATE_PARAM)) == 0) {
      DBG(r, "REQ[%X] found cookie no update parameter", TO_ADDR(r));
      no_update_flag++;
    }
  }

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
    if (! name) continue;
    name  = chxj_safe_to_jreserved_tag(r, name);

    if (strncasecmp(name, "_chxj", 5) != 0 && strncasecmp(name, "%5Fchxj", sizeof("%5Fchxj")-1) != 0) {
      if (strlen(result) != 0) 
        result = apr_pstrcat(pool, result, "&", NULL);

      if (entryp && strcasecmp(entryp->encoding, "NONE") != 0) {
        apr_size_t dlen;
        char *dvalue;
        char *dname;

        if (value && *value != 0) {
          value = chxj_url_decode(pool, value);
          dlen   = strlen(value);
          dvalue = chxj_rencoding(r, value, &dlen,spec->output_encoding);
          dvalue = chxj_url_encode(pool, dvalue);
        }
        else {
          dvalue = "";
        }

        if (name && *name != 0) {
          name = chxj_url_decode(pool, name);
          dlen = strlen(name);
          dname = chxj_rencoding(r, name, &dlen,spec->output_encoding);
          dname = chxj_url_encode(pool, dname);
        }
        else {
          dname = "";
        }


        result = apr_pstrcat(pool, result, dname, "=", dvalue, NULL);
      }
      else {
        result = apr_pstrcat(pool, result, name, "=", value, NULL);
      }
    }
    else
    if ( (strncasecmp(name, "_chxj_c_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fc%5F", sizeof("%5Fchxj%5Fc%5F")-1) == 0)
      || (strncasecmp(name, "_chxj_r_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fr%5F", sizeof("%5Fchxj%5Fr%5F")-1) == 0)
      || (strncasecmp(name, "_chxj_s_", 8) == 0 || strncasecmp(name, "%5Fchxj%5Fs%5F", sizeof("%5Fchxj%5Fs%5F")-1) == 0)) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(pool, result, "&", NULL);

      if (entryp && strcasecmp(entryp->encoding, "NONE") != 0 && value && strlen(value)) {
        apr_size_t dlen;
        char       *dvalue;

        dlen   = strlen(value);
        value = chxj_url_decode(pool, value);
        dvalue = chxj_rencoding(r, value, &dlen,spec->output_encoding);
        dvalue = chxj_url_encode(pool,dvalue);
        result = apr_pstrcat(pool, result, &name[8], "=", dvalue, NULL);

      }
      else {
        result = apr_pstrcat(pool, result, &name[8], "=", value, NULL);
      }
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0 || strcasecmp(name, "%5Fchxj%5Fcc") == 0) {
      if (! cookie) {
        s_clear_cookie_header(r, spec);
        DBG(r, "REQ[%X] found cookie parameter[%s]",    TO_ADDR(r), value);
        cookie_lock_t *lock = chxj_cookie_lock(r);
        cookie = chxj_load_cookie(r, value);
        if (! no_update_flag && cookie) {
          cookie = chxj_update_cookie(r, cookie);
        }
        chxj_cookie_unlock(r, lock);
      }
      if (cookie && cookie->cookie_id) {
        if (strlen(result) != 0)
          result = apr_pstrcat(pool, result, "&", NULL);
        result = apr_pstrcat(pool, result, name, "=", cookie->cookie_id, NULL);
      }
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0) {
      if (strlen(result) != 0)
        result = apr_pstrcat(pool, result, "&", NULL);
      result = apr_pstrcat(pool, result, name, "=", value, NULL);
    }
    else
    if ( strncasecmp(name, CHXJ_QUERY_STRING_PARAM_PREFIX,     sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX)-1) == 0) {
      apr_size_t dlen = 0;
      char*      dvalue;
      if (value) {
        dlen   = strlen(value);
      }
      if (dlen) {
        value = chxj_url_decode(pool, value);
        dvalue = chxj_rencoding(r, value, &dlen,spec->output_encoding);
        dvalue = chxj_url_encode(pool,dvalue);
        if (r->args && strlen(r->args) > 0) {
          r->args = apr_pstrcat(pool, r->args, "&", &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX)-1], "=", dvalue, NULL);
        }
        else {
          r->args = apr_pstrcat(pool, &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX)-1], "=", dvalue, NULL);
        }
        s_convert_guid_parameter_to_header(r, &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX)-1], spec);
      }
    }
    else
    if (strncasecmp(name, CHXJ_QUERY_STRING_PARAM_PREFIX_ENC, sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX_ENC)-1) == 0) {
      apr_size_t dlen;
      char*      dvalue;
      dlen   = strlen(value);
      if (dlen && value) {
        value = chxj_url_decode(pool, value);
        dvalue = chxj_rencoding(r, value, &dlen,spec->output_encoding);
        dvalue = chxj_url_encode(pool,dvalue);
        if (r->args && strlen(r->args) > 0) {
          r->args = apr_pstrcat(pool, r->args, "&", &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX_ENC)-1], "=", dvalue, NULL);
        }
        else {
          r->args = apr_pstrcat(pool, &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX_ENC)-1], "=", dvalue, NULL);
        }
        s_convert_guid_parameter_to_header(r, &name[sizeof(CHXJ_QUERY_STRING_PARAM_PREFIX_ENC)-1], spec);
      }
    }
    DBG(r, "REQ[%X] ************************ name:[%s]", TO_ADDR(r), name);
  }
  *len = strlen(result);
  apr_table_setn(r->headers_in, "X-Chxj-Cookie-No-Update", "true");
  if (! no_update_flag) {
    result = apr_pstrcat(pool, result, "&_chxj_nc=true", NULL);
  }

  DBG(r, "REQ[%X] AFTER input convert result = [%s]", TO_ADDR(r), result);
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

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
  request_rec         *r = f->r;
  conn_rec            *c = r->connection;
  apr_status_t        rv;
  apr_bucket_brigade  *bb;
  apr_bucket          *b;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  chxj_header_inf_clear(r);

  bb = apr_brigade_create(r->pool, c->bucket_alloc);
  b  = apr_bucket_transient_create(data, len, c->bucket_alloc);

  APR_BRIGADE_INSERT_TAIL(bb, b);
  b = apr_bucket_eos_create(f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(bb, b);

  rv = ap_pass_brigade(f->next, bb);
  if (rv != APR_SUCCESS) {
    DBG(r, "REQ[%X] end %s() (apr_pass_brigade)", TO_ADDR(r),__func__);
    return rv;
  }

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

  return rv;
}

/**
 * Add No Cache Header
 */
static void
s_add_no_cache_headers(request_rec *r, chxjconvrule_entry  *entryp)
{
  if (entryp && (entryp->action & CONVRULE_NOCACHE_ON_BIT)) {
    apr_table_unset(r->headers_out,     "Pragma");
    apr_table_unset(r->err_headers_out, "Pragma");
    apr_table_unset(r->headers_out,     "Expires");
    apr_table_unset(r->err_headers_out, "Expires");
    apr_table_unset(r->headers_out,     "Cache-Control");
    apr_table_unset(r->err_headers_out, "Cache-Control");

    apr_table_setn(r->err_headers_out, "Pragma", "no-cache");
    apr_table_setn(r->err_headers_out, "Expires", "Thu, 01 Jan 1970 00:00:00 GMT");
    apr_table_setn(r->err_headers_out, "Cache-Control", "no-cache, no-store");
  }
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
  request_rec         *r;
  apr_status_t        rv;
  apr_bucket          *b;
  const char          *data;
  char                *user_agent = NULL;
  apr_size_t          len;
  mod_chxj_ctx        *ctx = (mod_chxj_ctx *)f->ctx;
  cookie_t            *cookie = NULL;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp = NULL;
  device_table        *spec = NULL;
  apr_pool_t          *pool;

  r  = f->r;
  DBG(f->r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  rv = APR_SUCCESS;

  apr_pool_create(&pool, r->pool);

  entryp = ctx->entryp;
  spec   = ctx->spec;
  dconf  = chxj_get_module_config(r->per_dir_config, &chxj_module);


  if (apr_table_get(r->headers_out, "Location") || apr_table_get(r->err_headers_out, "Location")) {
    DBG(r, "REQ[%X] found Location header", TO_ADDR(r));
    if ((entryp->action & CONVRULE_COOKIE_ON_BIT) || (entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
      cookie_lock_t *lock = NULL;
      DBG(r, "REQ[%X] entryp->action == COOKIE_ON_BIT", TO_ADDR(r));
      switch(spec->html_spec_type) {
      case CHXJ_SPEC_Chtml_1_0:
      case CHXJ_SPEC_Chtml_2_0:
      case CHXJ_SPEC_Chtml_3_0:
      case CHXJ_SPEC_Chtml_4_0:
      case CHXJ_SPEC_Chtml_5_0:
      case CHXJ_SPEC_Chtml_6_0:
      case CHXJ_SPEC_Chtml_7_0:
      case CHXJ_SPEC_XHtml_Mobile_1_0:
      case CHXJ_SPEC_Jhtml:
      case CHXJ_SPEC_Jxhtml:
        lock = chxj_cookie_lock(r);
        cookie = chxj_save_cookie(r);
        s_add_cookie_id_if_has_location_header(r, cookie);
        chxj_cookie_unlock(r, lock);
        break;
      case CHXJ_SPEC_iPhone2:
      case CHXJ_SPEC_iPhone3:
      case CHXJ_SPEC_iPhone4:
      case CHXJ_SPEC_softbank_android:
      case CHXJ_SPEC_au_android:
      case CHXJ_SPEC_docomo_android:
      case CHXJ_SPEC_android:
      default:
        break;
      }
    }
    if (! ap_is_HTTP_REDIRECT(r->status)) {
      r->status = HTTP_MOVED_TEMPORARILY;
    }
    s_add_no_cache_headers(r, entryp);
    /* must not send body. */
    rv = pass_data_to_filter(f, "", 0);
    DBG(f->r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return rv;
  }


  if (r->content_type) {
    if (! STRNCASEEQ('t','T',"text/html",r->content_type, sizeof("text/html")-1)
    &&  ! STRNCASEEQ('t','T',"text/xml", r->content_type, sizeof("text/xml")-1)
    &&  ! STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)
    &&  ! (dconf->image == CHXJ_IMG_ON
          && ! apr_table_get(r->headers_in, "CHXJ_IMG_CONV")
          && STRNCASEEQ('i','I',"image/",  r->content_type, sizeof("image/") -1)
          && ( STRCASEEQ('j','J',"jpeg",            &r->content_type[6])         /* JPEG */
            || STRCASEEQ('j','J',"jp2",             &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('j','J',"jpeg2000",        &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('j','J',"jpeg2000-image",  &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('x','X',"x-jpeg2000-image",&r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('x','X',"x-ms-bmp",        &r->content_type[6])         /* BMP */
            || STRCASEEQ('p','P',"png",             &r->content_type[6])         /* PNG */
            || STRCASEEQ('x','X',"x-png",           &r->content_type[6])         /* PNG */
            || STRCASEEQ('g','G',"gif",             &r->content_type[6])))) {     /* GIF */
      
      DBG(r, "REQ[%X] not convert content-type:[%s] dconf->image:[%d]", TO_ADDR(r), r->content_type, dconf->image);
      if (entryp->action & CONVRULE_COOKIE_ON_BIT) {
        cookie_lock_t *lock = NULL;
        DBG(r, "REQ[%X] entryp->action == COOKIE_ON_BIT", TO_ADDR(r));
        switch(spec->html_spec_type) {
        case CHXJ_SPEC_Chtml_1_0:
        case CHXJ_SPEC_Chtml_2_0:
        case CHXJ_SPEC_Chtml_3_0:
        case CHXJ_SPEC_Chtml_4_0:
        case CHXJ_SPEC_Chtml_5_0:
        case CHXJ_SPEC_Chtml_6_0:
        case CHXJ_SPEC_Chtml_7_0:
        case CHXJ_SPEC_XHtml_Mobile_1_0:
        case CHXJ_SPEC_Jhtml:
        case CHXJ_SPEC_Jxhtml:
          lock = chxj_cookie_lock(r);
          cookie = chxj_save_cookie(r);
          s_add_cookie_id_if_has_location_header(r, cookie);
          chxj_cookie_unlock(r, lock);
          break;
        case CHXJ_SPEC_iPhone2:
        case CHXJ_SPEC_iPhone3:
        case CHXJ_SPEC_iPhone4:
        case CHXJ_SPEC_softbank_android:
        case CHXJ_SPEC_au_android:
        case CHXJ_SPEC_docomo_android:
        case CHXJ_SPEC_android:
        default:
          break;
        }
      }
      if (apr_table_get(r->headers_out, "Location") || apr_table_get(r->err_headers_out, "Location")) {
        if (! ap_is_HTTP_REDIRECT(r->status)) {
          r->status = HTTP_MOVED_TEMPORARILY;
        }
      }
      s_add_no_cache_headers(r, entryp);
      ap_pass_brigade(f->next, bb);
      DBG(f->r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
      return APR_SUCCESS;
    }
  }
  else {
    DBG(r, "REQ[%X] not convert content-type:[(null)]", TO_ADDR(r));
    ap_pass_brigade(f->next, bb);
    DBG(f->r, "REQ[%X] end %s()", TO_ADDR(f->r),__func__);
    return APR_SUCCESS;
  }


  for (b = APR_BRIGADE_FIRST(bb);
       b != APR_BRIGADE_SENTINEL(bb); 
       b = APR_BUCKET_NEXT(b)) {

    if (apr_bucket_read(b, &data, &len, APR_BLOCK_READ) == APR_SUCCESS) {
      chxj_dump_string(r, APLOG_MARK, "READ Data", data, len);

      /*--------------------------------------------------------------------*/
      /* append data                                                        */
      /*--------------------------------------------------------------------*/
      char *tmp;
      DBG(r, "REQ[%X] append data start", TO_ADDR(r));
      ctx = (mod_chxj_ctx *)f->ctx;

      if (len > 0) {
        tmp = apr_palloc(r->pool, ctx->len);
        memcpy(tmp, ctx->buffer, ctx->len);

        ctx->buffer = apr_palloc(pool, ctx->len + len);

        memcpy(ctx->buffer, tmp, ctx->len);
        memcpy(&ctx->buffer[ctx->len], data, len);

        ctx->len += len;
      }
      DBG(r, "REQ[%X] append data end", TO_ADDR(r));
    }

    if (APR_BUCKET_IS_EOS(b)) {

      DBG(r, "REQ[%X] eos", TO_ADDR(r));
      /*----------------------------------------------------------------------*/
      /* End Of File                                                          */
      /*----------------------------------------------------------------------*/
      if (ctx) {
        cookie_lock_t *lock = NULL;
        ctx = (mod_chxj_ctx *)f->ctx;

        DBG(r, "REQ[%X] content_type=[%s]", TO_ADDR(r), r->content_type);
        lock = chxj_cookie_lock(r);

        if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN 
            && r->content_type 
            && (STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)
            ||  STRNCASEEQ('t','T',"text/html", r->content_type, sizeof("text/html")-1))) {
          DBG(r, "REQ[%X] detect convert target:[%s]", TO_ADDR(r), r->content_type);

          if (ctx->len) {
            char *tmp;

            tmp = apr_palloc(pool, ctx->len + 1);

            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);

            ctx->buffer = chxj_convert(r, 
                                       (const char **)&tmp, 
                                       (apr_size_t *)&ctx->len,
                                       spec,
                                       user_agent, &cookie);

          }
          else {
            ctx->buffer = apr_psprintf(r->pool, "\n");
            ctx->len += 1;
            ctx->buffer = chxj_convert(r, 
                                       (const char **)&ctx->buffer, 
                                       (apr_size_t *)&ctx->len,
                                       spec,
                                       user_agent, &cookie);

          }
        }
        if (r->content_type
            && *(char *)r->content_type == 't'
            && strncmp(r->content_type, "text/xml",   8) == 0) {
          DBG(r, "REQ[%X] text/XML", TO_ADDR(r));

          Doc       doc;
          Node      *root;
          Node      *child;
          qr_code_t qrcode;
          int       sts;
      
          memset(&doc,    0, sizeof(Doc));
          memset(&qrcode, 0, sizeof(qr_code_t));
          doc.r = r;
          doc.parse_mode  = PARSE_MODE_CHTML;
          qrcode.doc      = &doc;
          qrcode.r        = r;
      
          qs_init_malloc(&doc);
      
          root = qs_parse_string(&doc, ctx->buffer, ctx->len);

          sts = 0;
          for (child = qs_get_child_node(&doc,root);
               child ;
               child = qs_get_next_node(&doc,child)) {
            char *name = qs_get_node_name(&doc,child);
            if (strcasecmp("qrcode",name) == 0) {
              sts++;
              break;
            }
          }
          qs_all_free(&doc,QX_LOGMARK);
          if (sts) {
            r->handler = apr_psprintf(r->pool, "chxj-qrcode");
            chxj_qrcode_node_to_qrcode(&qrcode, root);
            sts = chxj_qrcode_create_image_data(&qrcode, &ctx->buffer, &ctx->len);
            if (sts != OK) {
              ERR(r, "REQ[%X] qrcode create failed.", TO_ADDR(r));
              chxj_cookie_unlock(r, lock);
              DBG(f->r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
              return sts;
            }
            r->content_type = apr_psprintf(r->pool, "image/jpeg");
          }
        }

        if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN 
            && r->content_type 
            && ( *r->content_type == 'i' || *r->content_type == 'I')
            && dconf->image == CHXJ_IMG_ON
            && strncasecmp("image/", r->content_type, 6) == 0
            && ( STRCASEEQ('j','J',"jpeg",            &r->content_type[6])         /* JPEG */
              || STRCASEEQ('j','J',"jp2",             &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('j','J',"jpeg2000",        &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('j','J',"jpeg2000-image",  &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('x','X',"x-jpeg2000-image",&r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('p','P',"png",             &r->content_type[6])         /* PNG */
              || STRCASEEQ('x','X',"x-png",           &r->content_type[6])         /* PNG */
              || STRCASEEQ('x','X',"x-ms-bmp",     &r->content_type[6])         /* BMP */
              || STRCASEEQ('g','G',"gif",             &r->content_type[6]))) {     /* GIF */
          DBG(r, "REQ[%X] detect convert target:[%s]", TO_ADDR(r), r->content_type);
          if (ctx->len) {
            char *tmp;

            DBG(r, "REQ[%X] ctx->len[%d]", TO_ADDR(r), (unsigned int)ctx->len);
            tmp = apr_palloc(pool, ctx->len + 1);

            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);
            ctx->buffer = 
              chxj_convert_image(r, 
                                  (const char **)&tmp,
                                  (apr_size_t *)&ctx->len);
          }
        }

        apr_table_unset(r->headers_out, "Content-Length");
        apr_table_unset(r->err_headers_out, "Content-Length");
        ap_set_content_length(r, (apr_off_t)ctx->len);

        
        if (ctx->len > 0) {
          DBG(r, "REQ[%X] call %s()", TO_ADDR(r),__func__);
          s_add_cookie_id_if_has_location_header(r, cookie);
          if (apr_table_get(r->headers_out, "Location") || apr_table_get(r->err_headers_out, "Location")) {
            if (! ap_is_HTTP_REDIRECT(r->status)) {
              r->status = HTTP_MOVED_TEMPORARILY;
            }
          }
          if (ctx->len && ap_is_HTTP_REDIRECT(r->status)) {
            ctx->buffer = apr_pstrdup(pool, "");
            ctx->len    = 0;
            ap_set_content_length(r, (apr_off_t)ctx->len);
          }
          chxj_cookie_unlock(r,lock);
          s_add_no_cache_headers(r, entryp);
          rv = pass_data_to_filter(f, 
                                   (const char *)ctx->buffer, 
                                   (apr_size_t)ctx->len);
        }
        else {
          ctx->buffer = apr_pstrdup(pool, "");
          ctx->len    = 0;
          ap_set_content_length(r, (apr_off_t)ctx->len);
          chxj_cookie_unlock(r, lock);
          s_add_no_cache_headers(r, entryp);
          rv = pass_data_to_filter(f, 
                                   (const char *)ctx->buffer, 
                                   (apr_size_t)ctx->len);
        }
        DBG(f->r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
        return rv;
      }
      else {
        DBG(r, "REQ[%X] SAVE COOKIE[%x]", TO_ADDR(r), entryp->action);

        /*
         * save cookie.
         */
        if (entryp->action & CONVRULE_COOKIE_ON_BIT) {
          cookie_lock_t *lock = NULL;
          DBG(r, "REQ[%X] entryp->action == COOKIE_ON_BIT", TO_ADDR(r));
          switch(spec->html_spec_type) {
          case CHXJ_SPEC_Chtml_1_0:
          case CHXJ_SPEC_Chtml_2_0:
          case CHXJ_SPEC_Chtml_3_0:
          case CHXJ_SPEC_Chtml_4_0:
          case CHXJ_SPEC_Chtml_5_0:
          case CHXJ_SPEC_Chtml_6_0:
          case CHXJ_SPEC_Chtml_7_0:
          case CHXJ_SPEC_XHtml_Mobile_1_0:
          case CHXJ_SPEC_Jhtml:
          case CHXJ_SPEC_Jxhtml:
            lock = chxj_cookie_lock(r);
            cookie = chxj_save_cookie(r);
            /*
             * Location Header Check to add cookie parameter.
             */
            s_add_cookie_id_if_has_location_header(r, cookie);
            chxj_cookie_unlock(r, lock);
#if 0
            apr_table_unset(r->headers_out, "Set-Cookie");
            apr_table_unset(r->err_headers_out, "Set-Cookie");
#endif
            break;

          case CHXJ_SPEC_iPhone2:
          case CHXJ_SPEC_iPhone3:
          case CHXJ_SPEC_iPhone4:
          case CHXJ_SPEC_softbank_android:
          case CHXJ_SPEC_au_android:
          case CHXJ_SPEC_docomo_android:
          case CHXJ_SPEC_android:
          default:
            break;
          }
        }
        if (apr_table_get(r->headers_out, "Location") || apr_table_get(r->err_headers_out, "Location")) {
          if (! ap_is_HTTP_REDIRECT(r->status)) {
            r->status = HTTP_MOVED_TEMPORARILY;
          }
        }
        apr_table_setn(r->headers_out, "Content-Length", "0");
        s_add_no_cache_headers(r, entryp);
        rv = pass_data_to_filter(f, (const char *)"", (apr_size_t)0);
        return rv;
      }
    }
  }
  apr_brigade_destroy(bb);

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

  return APR_SUCCESS;
}

/**
 * Add Cookie_id if it has location header.
 */
static void
s_add_cookie_id_if_has_location_header(request_rec *r, cookie_t *cookie)
{
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  char *location_header = (char *)apr_table_get(r->headers_out, "Location");
  if (! location_header) {
    location_header = (char *)apr_table_get(r->err_headers_out, "Location");
  }
  if (cookie && location_header) {
    DBG(r, "REQ[%X] Location Header=[%s]", TO_ADDR(r), location_header);
    location_header = chxj_add_cookie_parameter(r,
                                                location_header,
                                                cookie);
    apr_table_unset(r->headers_out, "Location");
    apr_table_setn(r->headers_out, "Location", location_header);
    DBG(r, "REQ[%X] Location Header=[%s]", TO_ADDR(r), location_header);
    if (!ap_is_HTTP_REDIRECT(r->status)) {
      r->status = HTTP_MOVED_TEMPORARILY;
    }
  }
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}

/**
 * It is the main loop of the input filter handler. 
 *
 */
static apr_status_t
chxj_input_handler(request_rec *r)
{
  mod_chxj_config     *dconf;
  mod_chxj_req_config *request_conf;
  chxjconvrule_entry  *entryp = NULL;
  device_table        *spec   = NULL;
  char                *post_data = NULL;
  apr_size_t          post_data_len = 0;
  char                *response;
  char                *user_agent;
  apr_pool_t          *pool;
  int                 response_code = 0;
  
  DBG(r, "REQ[%X] start %s()",TO_ADDR(r),__func__);

  if (strcasecmp(r->handler, "chxj-input-handler")) {
    DBG(r, "REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }
  apr_pool_create(&pool, r->pool);

  dconf      = chxj_get_module_config(r->per_dir_config, &chxj_module);
  request_conf = chxj_get_req_config(r);
  user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
  if (!user_agent) {
    user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  }
  if (user_agent) {
    if (!request_conf->spec || !request_conf->user_agent) {
      spec = chxj_specified_device(r, user_agent);
    }
    else if (request_conf->user_agent && strcmp(user_agent, request_conf->user_agent)) {
      spec = chxj_specified_device(r, user_agent);
    }
    else {
      spec = request_conf->spec;
    }
  }

  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (request_conf->user_agent) {
    entryp = request_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }

  post_data = apr_pstrdup(pool, "");
  if (ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK) == OK) {
    if (ap_should_client_block(r)) {
      while (post_data_len < CHXJ_POST_MAX) {
#define BUFSZ (256)
        char buffer[BUFSZ];
        int read_bytes = ap_get_client_block(r, buffer, BUFSZ-1);
        if (read_bytes<=0) {
          break;
        }
        buffer[read_bytes] = '\0';
        post_data = apr_pstrcat(pool, post_data, buffer, NULL);
        post_data_len += read_bytes;
#undef BUFSZ
      }
    }
  }

  /* 
   * now convert.
   */
  if (post_data_len > 0) {
    post_data = chxj_input_convert(r, (const char**)&post_data, (apr_size_t*)&post_data_len, entryp, spec);
    DBG(r, "REQ[%X] (in:exchange)POSTDATA:[%s]", TO_ADDR(r),post_data);
  }

  char *url_path;
  if (dconf->forward_url_base) {
    url_path = apr_psprintf(pool, "%s%s", dconf->forward_url_base, r->uri);
  }
  else {
    url_path = apr_psprintf(pool, "%s://%s:%d%s", chxj_apache_run_http_scheme(r), ap_get_server_name(r), ap_get_server_port(r), r->uri);
  }
  if (r->args) {
    url_path = apr_pstrcat(pool, url_path, "?", r->args, NULL);
  }
  DBG(r, "REQ[%X] ==> new url_path:[%s]", TO_ADDR(r),url_path);

  apr_size_t res_len;
  apr_table_setn(r->headers_in, CHXJ_HEADER_ORIG_CLIENT_IP, r->connection->remote_ip);
  char *x_client_type = (char *)apr_table_get(r->headers_in, "X-Client-Type");
  if (x_client_type) {
    apr_table_setn(r->headers_in, CHXJ_HEADER_ORIG_CLIENT_TYPE, x_client_type); /* for mod_cidr_lookup */
  }
  else {
    apr_table_unset(r->headers_in, "X-Client-Type");
  }
  apr_table_unset(r->headers_in, "Content-Length");
  apr_table_setn(r->headers_in, "Content-Length", apr_psprintf(pool, "%" APR_SIZE_T_FMT, post_data_len));
  response = chxj_serf_post(r, pool, url_path, post_data, post_data_len, 1, &res_len, &response_code);

  char *chunked;
  if ((chunked = (char *)apr_table_get(r->headers_out, "Transfer-Encoding")) != NULL) {
    if (strcasecmp(chunked, "chunked") == 0) {
      r->chunked = 1;  
      apr_table_unset(r->headers_out, "Transfer-Encoding");
    }
  }
  if (ap_is_HTTP_ERROR(response_code)) {
    DBG(r, "REQ[%X] end %s() (HTTP-ERROR received. response code:[%d])", TO_ADDR(r), __func__,response_code);
    return response_code;
  }
  {
    apr_pool_t *wpool;
    apr_pool_create(&wpool, r->pool);
    apr_bucket_brigade *bb;
    apr_bucket *e;
    apr_status_t rv;
    conn_rec *c = r->connection;
    
    bb = apr_brigade_create(wpool, c->bucket_alloc);
    e  = apr_bucket_transient_create(response, res_len, c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, e);
    e = apr_bucket_eos_create(c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, e);
    if ((rv = ap_pass_brigade(r->output_filters, bb)) != APR_SUCCESS) {
      ERR(r, "REQ[%X] %s:%d failed ap_pass_brigade()", TO_ADDR(r), APLOG_MARK);
      return rv;
    }
    apr_brigade_cleanup(bb);
  }

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  return APR_SUCCESS;
}

static mod_chxj_global_config *
chxj_global_config_create(apr_pool_t *pool, server_rec *s)
{
  mod_chxj_global_config *conf;

  SDBG(s, "start chxj_global_config_create()");

  /*--------------------------------------------------------------------------*/
  /* allocate an own subpool which survives server restarts                   */
  /*--------------------------------------------------------------------------*/
  conf = (mod_chxj_global_config *)apr_palloc(pool, 
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
                 apr_pool_t *UNUSED(plog), 
                 apr_pool_t *UNUSED(ptemp), 
                 server_rec *s)
{
  void *user_data;
  apr_status_t rv;

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

  if ((rv = apr_proc_mutex_create(&global_cookie_mutex, NULL,  APR_LOCK_FCNTL, s->process->pool)) != APR_SUCCESS) {
    char errstr[255];
    SERR(s, "%s:%d end chxj_init_module(). mutex create failure.(%d:%s)",APLOG_MARK, rv,apr_strerror(rv,errstr,255));
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  SDBG(s, "end  chxj_init_module()");

  return OK;
}


static void 
chxj_child_init(apr_pool_t *UNUSED(p), server_rec *s)
{
  apr_status_t rv;
  SDBG(s, "start chxj_child_init()");
  if ((rv = apr_proc_mutex_child_init(&global_cookie_mutex, NULL, s->process->pool)) != APR_SUCCESS) {
    char errstr[255];
    SERR(s, "%s:%d ERROR end chxj_init_module(). mutex create failure.(%d:%s)", APLOG_MARK, rv,apr_strerror(rv,errstr,255));
  }
  SDBG(s, "end   chxj_child_init()");
}


/**
 * A set structure of each server is generated. 
 * 
 * @param p
 * @param s
 */
static void *
chxj_config_server_create(apr_pool_t *p, server_rec *s)
{
  mod_chxj_global_config *gc;

  gc = chxj_global_config_create(p,s);

  return gc;
}


static int
chxj_translate_name(request_rec *r)
{
  DBG(r, "REQ[%X] =======================================================================", TO_ADDR(r));
  DBG(r, "REQ[%X] ",                                                                        TO_ADDR(r));
  DBG(r, "REQ[%X] START REQUEST (uri:[%s] args:[%s])",                                      TO_ADDR(r), r->unparsed_uri, r->args ? r->args : "");
  DBG(r, "REQ[%X] METHOD [%s]",                                                             TO_ADDR(r), r->method);
  DBG(r, "REQ[%X] ",                                                                        TO_ADDR(r));
  DBG(r, "REQ[%X] =======================================================================", TO_ADDR(r));

  mod_chxj_config *dconf;
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  /*
  if (dconf->image_rewrite ==CHXJ_IMG_REWRITE_ON ){
    if(r->args && strcasecmp(r->args,"rewrite") == 0){
      DBG(r, "image rewrite is ON [%s] - %s", dconf->image_rewrite_url ,r->content_type);
      r->filename = apr_pstrcat(r->pool,dconf->image_rewrite_url,NULL);
      return OK;
    }
  }
  */

#if 0
  return chxj_trans_name(r);
#else
  return DECLINED;
#endif
}


static void 
chxj_insert_filter(request_rec *r)
{
  char                *user_agent;
  device_table        *spec;
  mod_chxj_config     *dconf;
  mod_chxj_req_config *req_conf;
  chxjconvrule_entry  *entryp;
  mod_chxj_ctx        *ctx;
  apr_status_t        rv;
  char                *contentType;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  req_conf = chxj_get_req_config(r);

  /* we get User-Agent from CHXJ_HTTP_USER_AGENT header if any */
  user_agent = (char *)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
  if (!user_agent) {
    user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  }

  contentType = (char *)apr_table_get(r->headers_in, "Content-Type");
  if (contentType
      && strncasecmp("multipart/form-data", contentType, 19) == 0) {
    DBG(r, "REQ[%X] detect multipart/form-data ==> no target", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return;
  }

  if (user_agent) {
    if (!req_conf->spec || !req_conf->user_agent) {
      spec = chxj_specified_device(r, user_agent);
    }
    else if (req_conf->user_agent && strcmp(user_agent, req_conf->user_agent)) {
      spec = chxj_specified_device(r, user_agent);
    }
    else {
      spec = req_conf->spec;
    }
  }
  req_conf = chxj_get_req_config(r);
  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (req_conf->user_agent) {
    entryp = req_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }
  if (!entryp && dconf->image != CHXJ_IMG_ON) {
    DBG(r, "REQ[%X] entryp is NULL and ChxjImageEngine Off", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return;
  }
  ctx = apr_palloc(r->pool, sizeof(*ctx));
  memset(ctx, 0, sizeof(*ctx));
  if ((rv = apr_pool_create(&ctx->pool, r->pool)) != APR_SUCCESS) {
    ERR(r, "REQ[%X] %s:%d: failed: new pool create. rv:[%d]", TO_ADDR(r),__FILE__,__LINE__,rv);
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return;
  }
  ctx->entryp = entryp;
  ctx->spec   = spec;
  ctx->buffer = apr_palloc(ctx->pool, 1);
  ctx->buffer[0] = 0;

  if (dconf->image != CHXJ_IMG_ON 
      && (    !entryp 
          || (   !(entryp->action & CONVRULE_ENGINE_ON_BIT) 
              && !(entryp->action & CONVRULE_COOKIE_ONLY_BIT) 
              && !(entryp->action & CONVRULE_EMOJI_ONLY_BIT)
             ))) {
    DBG(r,"REQ[%X] EngineOff and ChxjImageEngine Off and No EmojiOnly", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
    return;
  }
  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
  case CHXJ_SPEC_Jxhtml:
  case CHXJ_SPEC_iPhone2:
  case CHXJ_SPEC_iPhone3:
  case CHXJ_SPEC_iPhone4:
  case CHXJ_SPEC_softbank_android:
  case CHXJ_SPEC_au_android:
  case CHXJ_SPEC_docomo_android:
  case CHXJ_SPEC_android:
    break;

  default:
    DBG(r, "REQ[%X] end %s() Unknown spec type(%d).", TO_ADDR(r), __func__,spec->html_spec_type);
    return;
  }


  if (! apr_table_get(r->headers_in, "X-Chxj-Forward")) {
    req_conf->f = ap_add_output_filter("chxj_output_filter", ctx, r, r->connection);
    DBG(r, "REQ[%X] added Output Filter", TO_ADDR(r));
  }

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


void 
chxj_remove_filter(request_rec *r)
{
  mod_chxj_req_config *req_conf;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  req_conf = chxj_get_req_config(r);
  if (req_conf && req_conf->f) {
    ap_remove_output_filter(req_conf->f);
    DBG(r, "REQ[%X] REMOVE Output Filter", TO_ADDR(r));
  }
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


/**
 * The hook is registered.
 *
 * @param p
 */
static void 
chxj_register_hooks(apr_pool_t *UNUSED(p))
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
  ap_hook_insert_filter(chxj_insert_filter, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_img_conv_format_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_qr_code_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_input_handler, NULL, NULL, APR_HOOK_MIDDLE);

  ap_hook_handler(chxj_image_redirect_handler, NULL, NULL, APR_HOOK_MIDDLE);

  ap_hook_handler(chxj_google_analytics_handler, NULL, NULL, APR_HOOK_MIDDLE);

  ap_hook_translate_name(chxj_translate_name, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_fixups(chxj_headers_fixup, NULL, NULL, APR_HOOK_FIRST);

}


/**
 * A set structure according to the directory is generated. 
 *
 * @param p
 * @param arg
 */
static void * 
chxj_create_per_dir_config(apr_pool_t *p, char *arg) 
{
  mod_chxj_config *conf;

  conf = apr_pcalloc(p, sizeof(mod_chxj_config));
  conf->device_data_file = NULL;
  conf->devices          = NULL;
  conf->emoji_data_file  = NULL;
  conf->emoji            = NULL;
  conf->emoji_tail       = NULL;
  conf->imode_emoji_color = CHXJ_IMODE_EMOJI_COLOR_NONE;
  conf->image            = CHXJ_IMG_NONE;
  conf->image_cache_dir  = apr_psprintf(p, "%s",DEFAULT_IMAGE_CACHE_DIR);
  conf->image_cache_limit = 0;
  conf->server_side_encoding = NULL;
  conf->cookie_db_dir    = NULL;
  conf->cookie_timeout   = 0;
  conf->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  conf->cookie_lazy_mode  = 0;
  conf->cookie_dbm_type  = NULL;
  
  conf->detect_device_type = CHXJ_ADD_DETECT_DEVICE_TYPE_NONE;
  
#if defined(USE_MYSQL_COOKIE)
  memset((void *)&conf->mysql, 0, sizeof(mysql_t));
  conf->mysql.port       = MYSQL_PORT;
  conf->mysql.host       = NULL;
#endif
#if defined(USE_MEMCACHE_COOKIE)
  memset((void *)&conf->memcache, 0, sizeof(memcache_t));
  conf->memcache.host    = NULL;
  conf->memcache.port    = 0;
#endif
  conf->forward_url_base = NULL;
  conf->forward_server_ip = NULL;
  conf->allowed_cookie_domain = NULL;
  conf->post_log              = NULL;

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

  conf->image_rewrite = CHXJ_IMG_REWRITE_NONE;
  conf->image_rewrite_mode = CHXJ_IMG_REWRITE_MODE_NONE;
  conf->image_rewrite_url = NULL;

  conf->use_emoji_image = 0;
  conf->emoji_image_url = NULL;

  conf->use_google_analytics = 0;
  conf->google_analytics_account = NULL;
  conf->google_analytics_target = NULL;
  conf->google_analytics_debug = NULL;

  return conf;
}


/*
 *  Merge per-directory CHXJ configurations
 */
static void *
chxj_merge_per_dir_config(apr_pool_t *p, void *basev, void *addv)
{
  mod_chxj_config *base;
  mod_chxj_config *add;
  mod_chxj_config *mrg;

  base = (mod_chxj_config *)basev;
  add  = (mod_chxj_config *)addv;
  mrg  = (mod_chxj_config *)apr_palloc(p, sizeof(mod_chxj_config));

  mrg->device_data_file = NULL;
  mrg->devices          = NULL;
  mrg->emoji_data_file  = NULL;
  mrg->image            = CHXJ_IMG_NONE;
  mrg->image_cache_dir  = NULL;
  mrg->image_copyright  = NULL;
  mrg->image_cache_limit  = 0;
  mrg->emoji            = NULL;
  mrg->emoji_tail       = NULL;
  mrg->imode_emoji_color = CHXJ_IMODE_EMOJI_COLOR_NONE;
  mrg->new_line_type    = NLTYPE_NIL;
  mrg->forward_url_base = NULL;
  mrg->forward_server_ip = NULL;
  mrg->allowed_cookie_domain = NULL;
  mrg->post_log         = NULL;
  mrg->cookie_dbm_type  = NULL;
  
  mrg->device_keys      = NULL;
  mrg->device_hash      = NULL;

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

  if (add->image == CHXJ_IMG_NONE) {
    mrg->image = base->image;
  }
  else {
    mrg->image = add->image;
  }

  if (strcasecmp(add->image_cache_dir ,DEFAULT_IMAGE_CACHE_DIR)==0) {
    mrg->image_cache_dir = apr_pstrdup(p, base->image_cache_dir);
  }
  else {
    mrg->image_cache_dir = apr_pstrdup(p, add->image_cache_dir);
  }

  if (add->image_cache_limit) {
    mrg->image_cache_limit = add->image_cache_limit;
  }
  else {
    mrg->image_cache_limit = base->image_cache_limit;
  }

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

  if (add->cookie_db_dir) {
    mrg->cookie_db_dir = apr_pstrdup(p, add->cookie_db_dir);
  }
  else
  if (base->cookie_db_dir) {
    mrg->cookie_db_dir = apr_pstrdup(p, base->cookie_db_dir);
  }
  else {
    mrg->cookie_db_dir = NULL;
  }

  if (add->cookie_timeout) {
    mrg->cookie_timeout   = add->cookie_timeout;
  }
  else
  if (base->cookie_db_dir) {
    mrg->cookie_timeout   = base->cookie_timeout;
  }
  else {
    mrg->cookie_timeout   = 0;
  }

#if defined(USE_MYSQL_COOKIE)
  if (add->mysql.host) {
    mrg->mysql.host = apr_pstrdup(p, add->mysql.host);
  }
  else if (base->mysql.host) {
    mrg->mysql.host = apr_pstrdup(p, base->mysql.host);
  }
  else {
    mrg->mysql.host = NULL;
  }
  if (add->mysql.port) {
    mrg->mysql.port = add->mysql.port;
  }
  else if (base->mysql.port) {
    mrg->mysql.port = base->mysql.port;
  }
  else {
    mrg->mysql.port = 0;
  }

  if (add->mysql.database) {
    mrg->mysql.database = apr_pstrdup(p, add->mysql.database);
  }
  else if (base->mysql.database) {
    mrg->mysql.database = apr_pstrdup(p, base->mysql.database);
  }
  else {
    mrg->mysql.database = NULL;
  }

  if (add->mysql.username) {
    mrg->mysql.username = apr_pstrdup(p, add->mysql.username);
  }
  else if (base->mysql.username) {
    mrg->mysql.username = apr_pstrdup(p, base->mysql.username);
  }
  else {
    mrg->mysql.username = NULL;
  }

  if (add->mysql.password) {
    mrg->mysql.password = apr_pstrdup(p, add->mysql.password);
  }
  else if (base->mysql.password) {
    mrg->mysql.password = apr_pstrdup(p, base->mysql.password);
  }
  else {
    mrg->mysql.password = NULL;
  }

  if (add->mysql.tablename) {
    mrg->mysql.tablename = apr_pstrdup(p, add->mysql.tablename);
  }
  else if (base->mysql.tablename) {
    mrg->mysql.tablename = apr_pstrdup(p, base->mysql.tablename);
  }
  else {
    mrg->mysql.tablename = NULL;
  }

  if (add->mysql.socket_path) {
    mrg->mysql.socket_path = apr_pstrdup(p, add->mysql.socket_path);
  }
  else if (base->mysql.socket_path) {
    mrg->mysql.socket_path = apr_pstrdup(p, base->mysql.socket_path);
  }
  else {
    mrg->mysql.socket_path = NULL;
  }

  if (add->mysql.charset) {
    mrg->mysql.charset = apr_pstrdup(p, add->mysql.charset);
  }
  else if (base->mysql.charset) {
    mrg->mysql.charset = apr_pstrdup(p, base->mysql.charset);
  }
  else {
    mrg->mysql.charset = NULL;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (add->memcache.host) {
    mrg->memcache.host = apr_pstrdup(p, add->memcache.host);
  }
  else if (base->memcache.host) {
    mrg->memcache.host = apr_pstrdup(p, base->memcache.host);
  }
  else {
    mrg->memcache.host = NULL;
  }
  if (add->memcache.port) {
    mrg->memcache.port = add->memcache.port;
  }
  else if (base->memcache.port) {
    mrg->memcache.port = base->memcache.port;
  }
  else {
    mrg->memcache.port = 0;
  }
#endif
  if (add->cookie_store_type) {
    mrg->cookie_store_type = add->cookie_store_type;
  }
  else if (base->cookie_store_type) {
    mrg->cookie_store_type = base->cookie_store_type;
  }
  else {
    mrg->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  }
  if (add->cookie_lazy_mode) {
    mrg->cookie_lazy_mode = add->cookie_lazy_mode;
  }
  else if (base->cookie_lazy_mode) {
    mrg->cookie_lazy_mode = base->cookie_lazy_mode;
  }
  else {
    mrg->cookie_lazy_mode = 0;
  }
  if (add->new_line_type) {
    mrg->new_line_type = add->new_line_type;
  }
  else if (base->new_line_type) {
    mrg->new_line_type = base->new_line_type;
  }
  else {
    mrg->new_line_type = NLTYPE_NIL;
  }

  if (add->forward_url_base) {
    mrg->forward_url_base = add->forward_url_base;
  }
  else if (base->forward_url_base) {
    mrg->forward_url_base = base->forward_url_base;
  }

  if (add->allowed_cookie_domain) {
    mrg->allowed_cookie_domain = add->allowed_cookie_domain;
  }
  else {
    mrg->allowed_cookie_domain = base->allowed_cookie_domain;
  }
  if (add->post_log) {
    mrg->post_log = add->post_log;
  }
  else {
    mrg->post_log = base->post_log;
  }
  if (add->cookie_dbm_type) {
    mrg->cookie_dbm_type = add->cookie_dbm_type;
  }
  else {
    mrg->cookie_dbm_type = base->cookie_dbm_type;
  }
  
  if (add->imode_emoji_color == CHXJ_IMODE_EMOJI_COLOR_NONE) {
    mrg->imode_emoji_color = base->imode_emoji_color;
  }
  else {
    mrg->imode_emoji_color = add->imode_emoji_color;
  }
  
  if (add->detect_device_type == CHXJ_ADD_DETECT_DEVICE_TYPE_NONE) {
    mrg->detect_device_type = base->detect_device_type;
  }
  else {
    mrg->detect_device_type = add->detect_device_type;
  }
  
  if (add->device_keys) {
    mrg->device_keys = add->device_keys;
  }
  else{
    mrg->device_keys = base->device_keys;
  }
  
  if (add->device_hash) {
    mrg->device_hash = add->device_hash;
  }
  else{
    mrg->device_hash = base->device_hash;
  }
  
  if (add->image_rewrite == CHXJ_IMG_REWRITE_NONE){
    mrg->image_rewrite = base->image_rewrite;
  }
  else{
    mrg->image_rewrite = add->image_rewrite;
  }

  if (add->image_rewrite_url) {
    mrg->image_rewrite_url = add->image_rewrite_url;
  }
  else{
    mrg->image_rewrite_url = base->image_rewrite_url;
  }

  if (add->image_rewrite_mode == CHXJ_IMG_REWRITE_MODE_NONE){
    mrg->image_rewrite_mode = base->image_rewrite_mode;
  }
  else{
    mrg->image_rewrite_mode = add->image_rewrite_mode;
  }

  if (add->use_emoji_image != 1){
    mrg->use_emoji_image = base->use_emoji_image;
  }
  else{
    mrg->use_emoji_image = add->use_emoji_image;
  }

  if (add->emoji_image_url){
    mrg->emoji_image_url = add->emoji_image_url;
  }
  else{
    mrg->emoji_image_url = base->emoji_image_url;
  }

  if (add->use_google_analytics != 1){
    mrg->use_google_analytics = base->use_google_analytics;
  }
  else{
    mrg->use_google_analytics = add->use_google_analytics;
  }

  if (add->google_analytics_account){
    mrg->google_analytics_account = add->google_analytics_account;
  }
  else{
    mrg->google_analytics_account = base->google_analytics_account;
  }

  if (add->google_analytics_target){
    mrg->google_analytics_target = add->google_analytics_target;
  }
  else{
    mrg->google_analytics_target = base->google_analytics_target;
  }
  if (add->google_analytics_debug){
    mrg->google_analytics_debug = add->google_analytics_debug;
  }
  else{
    mrg->google_analytics_debug = base->google_analytics_debug;
  }

  return mrg;
}


static int
chxj_command_parse_take5(
  const char *arg, 
  char       **prm1, 
  char       **prm2, 
  char       **prm3, 
  char       **prm4, 
  char       **prm5)
{
  int  isquoted;
  char *strp;

  strp = (char *)arg;

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
static const char * 
cmd_load_device_data(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config  *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "mod_chxj: device data filename too long.";

  conf = (mod_chxj_config *)mconfig;
  conf->device_data_file = apr_pstrdup(parms->pool, arg);

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc *)&doc, (const char *)arg);
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
static const char * 
cmd_load_emoji_data(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  char            *rtn;
  Doc              doc;

  doc.r = NULL;


  if (strlen(arg) > 256) 
    return "mod_chxj: emoji data filename too long.";

  conf = (mod_chxj_config *)mconfig;
  conf->emoji_data_file = apr_pstrdup(parms->pool, arg);
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc *)&doc, (const char *)arg);

  rtn = chxj_load_emoji_data(&doc,parms->pool, conf);

  qs_all_free(&doc, QX_LOGMARK);


  return rtn;
}


static const char * 
cmd_set_image_engine(cmd_parms * UNUSED(parms), void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "image uri is too long.";

  conf = (mod_chxj_config*)mconfig;
  if (strcasecmp("ON", arg) == 0) {
    conf->image = CHXJ_IMG_ON;
  }
  else {
    conf->image = CHXJ_IMG_OFF;
  }

  return NULL;
}


static const char * 
cmd_set_image_cache_dir(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "cache dir name is too long.";
  
  apr_finfo_t info;
  apr_status_t res = apr_stat(&info,arg,APR_FINFO_TYPE,parms->pool);
  if(res != APR_SUCCESS){
    return apr_psprintf(parms->pool,"ChxjImageCacheDir [%s]: not found ",arg);
  }
  else{
    if(info.filetype != APR_DIR){
      return apr_psprintf(parms->pool,"ChxjImageCacheDir [%s]: is not directory ",arg);
    }
  }

  conf = (mod_chxj_config *)mconfig;
  conf->image_cache_dir = apr_pstrdup(parms->pool, arg);

  return NULL;
}


static const char * 
cmd_set_image_cache_limit(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > IMAGE_CACHE_LIMIT_FMT_LEN) 
    return "cache size is too long.";

  conf = (mod_chxj_config *)mconfig;
  errno = 0;
  /* 
   * I use strtol function because strtoul is not portable function. 
   */
  conf->image_cache_limit = (unsigned long)strtol(arg, NULL, 10);
  switch (errno) {
  case EINVAL:
    return apr_psprintf(parms->pool, "ChxjImageCacheLimit invalid value [%s] errno:[%d]", arg, errno);
  case ERANGE:
    return apr_psprintf(parms->pool, "ChxjImageCacheLimit Out of range [%s] errno:[%d]", arg, errno);
  default:
    break;
  }
  return NULL;
}


static const char * 
cmd_set_image_copyright(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "Copyright Flag is too long.";

  conf = (mod_chxj_config *)mconfig;
  conf->image_copyright = apr_pstrdup(parms->pool, arg);

  return NULL;
}


static const char *
cmd_convert_rule(cmd_parms *cmd, void *mconfig, const char *arg)
{
  int                 mode;
  ap_regex_t          *regexp;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *newrule;
  char                *prm1;
  char                *prm2;
  char                *prm3;
  char                *prm4;
  char                *prm5;
  char                *pstate;
  char                *action;
  char                *pp;

  dconf = (mod_chxj_config *)mconfig;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjConvertRule: is too long.";

  dconf = (mod_chxj_config *)mconfig;
  if (dconf->convrules == NULL)
    dconf->convrules   = apr_array_make(cmd->pool, 
                                        2, 
                                        sizeof(chxjconvrule_entry));

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
      else
      if (strcasecmp(CONVRULE_EMOJI_ONLY_CMD, action) == 0) {
        newrule->action |= CONVRULE_EMOJI_ONLY_BIT;
      }
      else
      if (strcasecmp(CONVRULE_ENVINFO_ONLY_CMD, action) == 0) {
        newrule->action |= CONVRULE_ENVINFO_ONLY_BIT;
      }
      break;

    case 'O':
    case 'o':
      if (strcasecmp(CONVRULE_OVERWRITE_X_CLIENT_TYPE_CMD, action) == 0) {
        newrule->action |= CONVRULE_OVERWRITE_X_CLIENT_TYPE_BIT;
      }
      break;

    case 'C':
    case 'c':
      if (strcasecmp(CONVRULE_COOKIE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_COOKIE_ON_BIT;
      }
      else if (strcasecmp(CONVRULE_COOKIE_OFF_CMD, action) == 0) {
        newrule->action &= (0xffffffff ^ CONVRULE_COOKIE_ON_BIT);
      }
      else if (strcasecmp(CONVRULE_CSS_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_CSS_ON_BIT;
      }
      else if (strcasecmp(CONVRULE_CSS_OFF_CMD, action) == 0) {
        newrule->action &= (0xffffffff ^ CONVRULE_CSS_ON_BIT);
      }
      else if (strcasecmp(CONVRULE_COOKIE_ONLY_CMD, action) == 0) {
        newrule->action |= CONVRULE_COOKIE_ONLY_BIT;
      }
      break;

    case 'J':
    case 'j':
      if (strcasecmp(CONVRULE_JRCONV_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_JRCONV_OFF_BIT;
      }
      break;

    case 'N':
    case 'n':
      if (strcasecmp(CONVRULE_NOCACHE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_NOCACHE_ON_BIT;
      }
      break;

    case 'Q':
    case 'q':
      if (strcasecmp(CONVRULE_QSCONV_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_QSCONV_OFF_BIT;
      }
      break;

    case 'Z':
    case 'z':
      if (strcasecmp(CONVRULE_Z2H_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_OFF_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_ALPHA_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_ALPHA_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_ALPHA_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_ALPHA_OFF_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_NUM_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_NUM_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_NUM_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_NUM_OFF_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_ALL_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_ON_BIT | CONVRULE_Z2H_ALPHA_ON_BIT | CONVRULE_Z2H_NUM_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_Z2H_NUM_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_Z2H_OFF_BIT | CONVRULE_Z2H_ALPHA_OFF_BIT | CONVRULE_Z2H_NUM_OFF_BIT;
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
  if ((regexp = ap_pregcomp((apr_pool_t *)cmd->pool, (const char *)pp, mode)) == NULL)
    return "RewriteRule: cannot compile regular expression ";

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


static const char *
cmd_set_cookie_dir(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;


  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieDir is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->cookie_db_dir = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_timeout(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieTimeout is too long.";

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieTimeout is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->cookie_timeout = atoi(arg);

  return NULL;
}


#if defined(USE_MYSQL_COOKIE)
static const char *
cmd_set_cookie_mysql_database(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlDatabase is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.database = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_username(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlUsername is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.username = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_password(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlPassword is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.password = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_table_name(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlTableName is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.tablename = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_set_cookie_mysql_port(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlPort is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieMysqlPort is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.port = chxj_atoi(arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_host(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlHost is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.host = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_socket_path(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieMysqlSocketPath is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.socket_path = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_charset(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlCharset is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.charset = apr_pstrdup(cmd->pool, arg);

  return NULL;
}
#endif
#if defined(USE_MEMCACHE_COOKIE)
static const char *
cmd_set_cookie_memcache_port(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMemcachePort is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieMemcachePort is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->memcache.port = (apr_port_t)chxj_atoi(arg);

  return NULL;
}


static const char *
cmd_set_cookie_memcache_host(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMemcacheHost is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->memcache.host = apr_pstrdup(cmd->pool, arg);

  return NULL;
}
#endif

static const char *
cmd_set_cookie_lazy_mode(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieLazyMode is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (strcasecmp("TRUE",arg) == 0) {
    dconf->cookie_lazy_mode = COOKIE_LAZY_ON;
  }
  else {
    dconf->cookie_lazy_mode = COOKIE_LAZY_OFF;
  }

  return NULL;
}

static const char *
cmd_set_cookie_store_type(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieStoreType is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_DBM, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_DBM;
  }
  else if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_MYSQL, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_MYSQL;
  }
  else if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_MEMCACHE, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_MEMCACHE;
  }
  else {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  }

  return NULL;
}

static const char *
cmd_set_forward_url_base(
  cmd_parms   *cmd,
  void        *mconfig,
  const char  *arg)
{
 mod_chxj_config *dconf;

  if (strlen(arg) > 255)
    return "mod_chxj: ChxjForwardUrlBase is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->forward_url_base = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_set_forward_server_ip(
  cmd_parms   *cmd,
  void        *mconfig,
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255)
    return "mod_chxj: ChxjForwardServerIp is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->forward_server_ip = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_allowed_cookie_domain(
  cmd_parms   *cmd,
  void        *mconfig,
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255)
    return "mod_chxj: ChxjAllowedCookieDomain is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->allowed_cookie_domain = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_set_new_line_type(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;
  if (strlen(arg) > 255)
    return "mod_chxj: ChxjNewLineType is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (strcasecmp(CHXJ_NEW_LINE_TYPE_CRLF, arg) == 0) {
    dconf->new_line_type = NLTYPE_CRLF;
  }
  else if (strcasecmp(CHXJ_NEW_LINE_TYPE_LF, arg) == 0) {
    dconf->new_line_type = NLTYPE_LF;
  }
  else if (strcasecmp(CHXJ_NEW_LINE_TYPE_CR, arg) == 0) {
    dconf->new_line_type = NLTYPE_CR;
  }
  else if (strcasecmp(CHXJ_NEW_LINE_TYPE_NONE, arg) == 0) {
    dconf->new_line_type = NLTYPE_NONE;
  }
  else {
    return "mod_chxj: invalid value (ChxjNewLineType)";
  }
  return NULL;
}

static const char *
cmd_post_log_env(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;
  if (strlen(arg) > 255)
    return "mod_chxj: ChxjPostLogEnv is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->post_log = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_cookie_dbm_type(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;
  if (strlen(arg) > 255)
    return "mod_chxj: ChxjCookieDbmType is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->cookie_dbm_type = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_imode_emoji_color(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;
  
  if (strlen(arg) > 256) 
    return "imode emoji color is too long.";

  dconf = (mod_chxj_config *)mconfig;
  if (strcasecmp("ON", arg) == 0) {
    dconf->imode_emoji_color = CHXJ_IMODE_EMOJI_COLOR_ON;
  }
  else if(strcasecmp("AUTO",arg) == 0) {
    dconf->imode_emoji_color = CHXJ_IMODE_EMOJI_COLOR_AUTO;
  }
  else {
    dconf->imode_emoji_color = CHXJ_IMODE_EMOJI_COLOR_OFF;
  }
  
  return NULL;
}

static const char *
cmd_add_device_data_tsv(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config  *conf;
  
  if (strlen(arg) > 256) 
    return "mod_chxj: device tsv filename too long.";

  conf = (mod_chxj_config *)mconfig;
  
  conf->detect_device_type = CHXJ_ADD_DETECT_DEVICE_TYPE_TSV;
  
  apr_finfo_t info;
  apr_status_t res = apr_stat(&info,arg,APR_FINFO_TYPE,parms->pool);
  if(res != APR_SUCCESS){
    return apr_psprintf(parms->pool,"ChxjDeviceTSV [%s]: not found ",arg);
  }
  else{
    if(info.filetype != APR_REG ){
      return apr_psprintf(parms->pool,"ChxjDeviceTSV [%s]: is not file ",arg);
    }
  }
  apr_file_t *fp;
  apr_file_open(&fp, arg, APR_READ|APR_BUFFERED, APR_OS_DEFAULT, parms->pool);
  
  chxj_load_device_tsv_data(fp,parms->pool,conf);
  
  apr_file_close(fp);
  return NULL;
}

static const char *
cmd_image_rewrite(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set rewrite too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  if (strcasecmp("ON", arg) == 0) {
    conf->image_rewrite = CHXJ_IMG_REWRITE_ON;
  }
  else if(strcasecmp("OFF",arg) == 0) {
    conf->image_rewrite = CHXJ_IMG_REWRITE_OFF;
  }
  else {
    conf->image_rewrite = CHXJ_IMG_REWRITE_NONE;
  }
  return NULL;
}

static const char *
cmd_image_rewrite_url(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set rewrite url too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  conf->image_rewrite_url = apr_pstrdup(parms->pool, arg);;
  return NULL;
}

static const char *
cmd_image_rewrite_mode(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set rewrite mode is too long.";
  }

  conf = (mod_chxj_config *)mconfig;
  if (strcasecmp("all",arg) == 0) {
    conf->image_rewrite_mode = CHXJ_IMG_REWRITE_MODE_ALL;
  }
  else if (strcasecmp("user",arg) == 0) {
    conf->image_rewrite_mode = CHXJ_IMG_REWRITE_MODE_USER;
  }
  else if (strcasecmp("tag",arg) == 0) {
    conf->image_rewrite_mode = CHXJ_IMG_REWRITE_MODE_TAG;
  }
  else{
    conf->image_rewrite_mode = CHXJ_IMG_REWRITE_MODE_NONE;
  }
  return NULL;
}


static const char *
cmd_use_emoji_image(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  conf = (mod_chxj_config *)mconfig;
  if (strcasecmp("on", arg) == 0) {
    conf->use_emoji_image = 1;
  }
  return NULL;
}


static const char *
cmd_emoji_image_url(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set ChxjEmojiImageUrl is too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  conf->emoji_image_url = apr_pstrdup(parms->pool, arg);
  return NULL;
}


static const char *
cmd_google_analytics(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  conf = (mod_chxj_config *)mconfig;
  if (strcasecmp("on", arg) == 0) {
    conf->use_google_analytics = 1;
  }
  return NULL;
}
static const char *
cmd_google_analytics_account(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set ChxjGoogleAnalyticsAccount is too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  conf->google_analytics_account = apr_pstrdup(parms->pool, arg);
  return NULL;
}
static const char *
cmd_google_analytics_target(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set ChxjGoogleAnalyticsTarget is too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  conf->google_analytics_target = apr_pstrdup(parms->pool, arg);
  return NULL;
}
static const char *
cmd_google_analytics_debug(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config *conf;
  if (strlen(arg) > 256){
    return "mod_chxj: set ChxjGoogleAnalyticsDebug is too long.";
  }
  conf = (mod_chxj_config *)mconfig;
  conf->google_analytics_debug = apr_pstrdup(parms->pool, arg);
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
    "ChxjImageCacheLimit",
    cmd_set_image_cache_limit,
    NULL,
    OR_ALL,
    "Image Cache Limit"),
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
  AP_INIT_TAKE1(
    "ChxjCookieDir",
    cmd_set_cookie_dir,
    NULL,
    OR_ALL,
    "save cookie.db directory."),
  AP_INIT_TAKE1(
    "ChxjCookieTimeout",
    cmd_set_cookie_timeout,
    NULL,
    OR_ALL,
    "The compulsion time-out time of the cookie is specified. "),
  AP_INIT_TAKE1(
    "ChxjCookieStoreType",
    cmd_set_cookie_store_type,
    NULL,
    OR_ALL,
    "It specifies preserving of the cookie ahead. (DBM/MYSQL/MEMCACHE)"),
  AP_INIT_TAKE1(
    "ChxjCookieLazyMode",
    cmd_set_cookie_lazy_mode,
    NULL,
    OR_ALL,
    "OneTimeID is negligently done. (TRUE/FALSE)"),
#if defined(USE_MYSQL_COOKIE)
  AP_INIT_TAKE1(
    "ChxjCookieMysqlHost",
    cmd_set_cookie_mysql_host,
    NULL,
    OR_ALL,
    "The MySQL database host used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlPort",
    cmd_set_cookie_mysql_port,
    NULL,
    OR_ALL,
    "The MySQL database port used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlDatabase",
    cmd_set_cookie_mysql_database,
    NULL,
    OR_ALL,
    "The MySQL database name used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlUsername",
    cmd_set_cookie_mysql_username,
    NULL,
    OR_ALL,
    "The MySQL username used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlPassword",
    cmd_set_cookie_mysql_password,
    NULL,
    OR_ALL,
    "The MySQL password used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlTableName",
    cmd_set_cookie_mysql_table_name,
    NULL,
    OR_ALL,
    "The MySQL table name used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlSocketPath",
    cmd_set_cookie_mysql_socket_path,
    NULL,
    OR_ALL,
    "The MySQL socket path used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlCharset",
    cmd_set_cookie_mysql_charset,
    NULL,
    OR_ALL,
    "The MySQL charset used by saving Cookie"),
#endif
#if defined(USE_MEMCACHE_COOKIE)
  AP_INIT_TAKE1(
    "ChxjCookieMemcacheHost",
    cmd_set_cookie_memcache_host,
    NULL,
    OR_ALL,
    "The Memcached host used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMemcachePort",
    cmd_set_cookie_memcache_port,
    NULL,
    OR_ALL,
    "The Memcached port used by saving Cookie"),
#endif
  AP_INIT_TAKE1(
    "ChxjNewLineType",
    cmd_set_new_line_type,
    NULL,
    OR_ALL,
    "HTML new line type (NONE|CRLF|LF|CR). default is CRLF"),
  AP_INIT_TAKE1(
    "ChxjForwardUrlBase",
    cmd_set_forward_url_base,
    NULL,
    OR_ALL,
    "The forward url base(default: {request protocol}://{this server}:{this server port}"),
  AP_INIT_TAKE1(
    "ChxjForwardServerIp",
    cmd_set_forward_server_ip,
    NULL,
    OR_ALL,
    "The forward server ip(default: this server ip)"),
  AP_INIT_TAKE1(
    "ChxjAllowedCookieDomain",
    cmd_allowed_cookie_domain,
    NULL,
    OR_ALL,
    "Domain that permits parameter addition for cookie besides hostname.(Default:hostname Only)"),
  AP_INIT_TAKE1(
    "ChxjPostLogEnv",
    cmd_post_log_env,
    NULL,
    OR_ALL,
    "for CustomLog directive. mod_chxj's internal POST log environment name.(Default:chxj-post-log)"),
  AP_INIT_TAKE1(
    "ChxjCookieDbmType",
    cmd_cookie_dbm_type,
    NULL,
    OR_ALL,
    "Kind of DBM used with Cookie simulator.(default|GDBM|SDBM|DB|NDBM)"),
  AP_INIT_TAKE1(
    "ChxjImodeEmojiColor",
    cmd_imode_emoji_color,
    NULL,
    OR_ALL,
    "Auto i-mode emoji color"),
  AP_INIT_TAKE1(
    "ChxjAddDeviceDataTSV",
    cmd_add_device_data_tsv,
    NULL,
    OR_ALL,
    "Additional devices TSV data"),
  AP_INIT_TAKE1(
    "ChxjImageRewrite",
    cmd_image_rewrite,
    NULL,
    OR_ALL,
    "Rewrite Image"
   ),
  AP_INIT_TAKE1(
    "ChxjImageRewriteUrl",
    cmd_image_rewrite_url,
    NULL,
    OR_ALL,
    "Set rewrite Image url"
   ),
  AP_INIT_TAKE1(
    "ChxjImageRewriteMode",
    cmd_image_rewrite_mode,
    NULL,
    OR_ALL,
    "Set rewrite Image rewrite url mode"
   ),
  AP_INIT_TAKE1(
    "ChxjUseEmojiImage",
    cmd_use_emoji_image,
    NULL,
    OR_ALL,
    "It is specified whether to use image for emoji. (Default:off)"
   ),
  AP_INIT_TAKE1(
    "ChxjEmojiImageUrl",
    cmd_emoji_image_url,
    NULL,
    OR_ALL,
    "When image is used for emoji, url with the emoji image is specified. "
   ),
  AP_INIT_TAKE1(
    "ChxjGoogleAnalytics",
    cmd_google_analytics,
    NULL,
    OR_ALL,
    "Set 'On' or 'Off' (Default:off) When you use google analytics."
   ),
  AP_INIT_TAKE1(
    "ChxjGoogleAnalyticsAccount",
    cmd_google_analytics_account,
    NULL,
    OR_ALL,
    "Set Google Analytics Account ID for mobile."
   ),
  AP_INIT_TAKE1(
    "ChxjGoogleAnalyticsTarget",
    cmd_google_analytics_target,
    NULL,
    OR_ALL,
    "Set img target for Google Analytics. i.e. `/ga.pl'"
   ),
  AP_INIT_TAKE1(
    "ChxjGoogleAnalyticsDebug",
    cmd_google_analytics_debug,
    NULL,
    OR_ALL,
    "Set filename which is outputed response from google analytics."
   ),
  {NULL,{NULL},NULL,0,0,NULL},
};


/*----------------------------------------------------------------------------*/
/* Dispatch list for API hooks                                                */
/*----------------------------------------------------------------------------*/
module AP_MODULE_DECLARE_DATA chxj_module = {
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
