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
#include "chxj_hdml.h"
#include "chxj_tag_util.h"
#include "chxj_str_util.h"
#include <unistd.h>
#include "chxj_img_conv.h"
#include "chxj_dump.h"
#define CHECK_BOX_PREFIX "_chxj_c_"
#define RADIO_BUTTON_PREFIX "_chxj_r_"

static void chxj_init_hdml(Hdml* hdml, Doc* doc, request_rec* r, device_table* spec);
static char* hdml_1_0_node_exchange   (Hdml* doc, Node* node, int indent);
static char* hdml_1_0_start_html_tag  (Hdml* doc, Node* child) ;
static char* hdml_1_0_end_html_tag    (Hdml* doc, Node* child) ;
static char* hdml_1_0_start_meta_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_meta_tag    (Hdml* doc, Node* node) ;
static char* hdml_1_0_start_head_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_head_tag    (Hdml* doc, Node* node) ;
static char* hdml_1_0_start_title_tag (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_title_tag   (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_base_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_base_tag    (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_body_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_body_tag    (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_a_tag     (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_a_tag       (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_br_tag    (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_br_tag      (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_font_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_font_tag    (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_form_tag  (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_form_tag    (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_input_tag (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_input_tag   (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_select_tag(Hdml* doc, Node* node) ;
static char* hdml_1_0_end_select_tag  (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_option_tag(Hdml* doc, Node* node) ;
static char* hdml_1_0_end_option_tag  (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_center_tag(Hdml* doc, Node* node) ;
static char* hdml_1_0_end_center_tag  (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_hr_tag    (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_hr_tag      (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_img_tag   (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_img_tag     (Hdml* doc,  Node* node) ;
static char* hdml_1_0_start_div_tag   (Hdml* doc, Node* node) ;
static char* hdml_1_0_end_div_tag     (Hdml* doc,  Node* node) ;
static char* qs_get_form_no(request_rec* r, Hdml* hdml);
static void hdml_1_0_count_radio_tag  (Hdml* hdml, Node* node) ;
static void hdml_do_input_text_tag    (Hdml* hdml, Node* tag);
static void hdml_do_input_password_tag(Hdml* hdml, Node* tag);
static void hdml_do_input_submit_tag  (Hdml* hdml, Node* tag);
static void hdml_do_input_hidden_tag  (Hdml* hdml, Node* tag);
static void hdml_do_input_radio_tag   (Hdml* hdml, Node* tag);
static void hdml_do_input_checkbox_tag(Hdml* hdml, Node* tag);
static void hdml_tag_output_upper_half(Hdml* hdml, Node* node);
static Hdml* qs_output_to_hdml_out    (Hdml* hdml, char* s);
static Hdml* qs_output_to_hdml_card   (Hdml* hdml, char* s);
static void qs_output_to_postdata     (Hdml* hdml, char* s);
static void qs_output_to_init_vars    (Hdml* hdml, char* s);
static int hdml_search_emoji          (Hdml* hdml, char* txt, char** rslt);
static void hdml_1_0_chxjif_tag       (Hdml* hdml, Node* node);
/**
 * converts from CHTML to hdml.
 *
 * @param r    [i] Requet_rec is appointed.
 * @param spec [i] The result of the device specification processing which 
 *                 was done in advance is appointed.
 * @param src  [i] The character string before the converting is appointed.
 * @return         The character string after the converting is returned.
 */
char*
chxj_exchange_hdml(request_rec* r, 
                   device_table *spec, 
                   const char* src, 
                   apr_size_t srclen, 
                   apr_size_t* dstlen)
{
  char*     dst = NULL;
  char*     buf = NULL;
  Doc       doc;
  Hdml      hdml;
  void*     shm_segment;
  int*      shm;
  mod_chxj_global_config* gconf;

  /*--------------------------------------------------------------------------*/
  /* initialize hdml structure                                                */
  /*--------------------------------------------------------------------------*/
  chxj_init_hdml(&hdml,&doc,r, spec);

  /*--------------------------------------------------------------------------*/
  /* DEBUG                                                                    */
  /*--------------------------------------------------------------------------*/
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML->HDML", src, srclen);
#endif


  /*--------------------------------------------------------------------------*/
  /* It is examined whether there is a location header. It ends without doing */
  /* anything when is.                                                        */
  /*--------------------------------------------------------------------------*/
  buf = (char*)apr_table_get(r->headers_out, (const char*)"Location");
  if (buf != NULL) 
  {
    /*------------------------------------------------------------------------*/
    /* The Location header generates tag in an initial HDML machine for the   */
    /* uncorrespon dence.                                                     */
    /*------------------------------------------------------------------------*/
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "Location is not null[Location:%s]", buf);
    qs_output_to_hdml_out(&hdml, 
        "<HDML VERSION=3.0 MARKABLE=TRUE PUBLIC=TRUE>\n"
        "<NODISPLAY MARKABLE=TRUE PUBLIC=TRUE TITLE=\" \">\n"
        "<ACTION TYPE=ACCEPT TASK=GO DEST=\""
        );
    qs_output_to_hdml_out(&hdml, buf);
    qs_output_to_hdml_out(&hdml,
        "\">\n"
        "</NODISPLAY>\n"
        "</HDML>\n"
        );
    dst = apr_pstrdup(r->pool, hdml.out);
  }
  else 
  {
    /*------------------------------------------------------------------------*/
    /* Here, the parsing of the received character string is done             */
    /*------------------------------------------------------------------------*/
    char *ss = apr_pstrdup(r->pool, src);
    qs_init_malloc(&doc); 
    qs_init_root_node(&doc);
    ss[srclen] = '\0';
    qs_parse_string(&doc,ss);

    /*------------------------------------------------------------------------*/
    /* The number of radiobuttons is counted.                                 */
    /*------------------------------------------------------------------------*/
    hdml_1_0_count_radio_tag(&hdml, qs_get_root(&doc));

    dst = hdml_1_0_node_exchange(&hdml, qs_get_root(&doc), 0);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "tmp=[%s]", dst);
    qs_all_free(&doc,QX_LOGMARK);
  }

  /*--------------------------------------------------------------------------*/
  /* DEBUG                                                                    */
  /*--------------------------------------------------------------------------*/
#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML->HDML", hdml.out, hdml.out_len);
#endif

  /*--------------------------------------------------------------------------*/
  /* When there is no processing result, former character string is copied    */
  /* and it returns it.                                                       */
  /*--------------------------------------------------------------------------*/
  if (dst == NULL) 
  {
    *dstlen = srclen;
    return apr_pstrdup(r->pool,src);
  }

  *dstlen = hdml.out_len;

  /*--------------------------------------------------------------------------*/
  /* for HDML Counter                                                         */
  /*--------------------------------------------------------------------------*/
  gconf = ap_get_module_config(r->server->module_config, &chxj_module);

  apr_global_mutex_lock(gconf->client_lock);
  shm_segment = apr_shm_baseaddr_get(gconf->client_shm);
  shm = (int*)shm_segment;
  hdml.form_cnt = *shm;
  *shm = hdml.form_cnt + hdml.pure_form_cnt + 1;
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "form_cnt[%d] -> [%d]", hdml.form_cnt,
                                          *shm);
  apr_global_mutex_unlock(gconf->client_lock); 

  /*--------------------------------------------------------------------------*/
  /* Null is set at the end of the character string to make sure.             */
  /*--------------------------------------------------------------------------*/
  dst[hdml.out_len] = 0;
  return dst;
}

/**
 * The HDML structure is initialized. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure that wants to be 
 *                   initialized is specified. 
 * @param doc  [i]   The Doc structure that should be set to the initialized 
 *                   HDML structure is specified. 
 * @param r    [i]   To use POOL, the pointer to request_rec is specified. 
 * @param spec [i]   The pointer to the device_table
 */
static void 
chxj_init_hdml(Hdml* hdml, Doc* doc, request_rec* r, device_table* spec)
{
  int     ii;
  int     jj;
  void*   shm_segment;
  int*    shm;
  mod_chxj_global_config* gconf;

  /*--------------------------------------------------------------------------*/
  /* init hdml structure value                                                */
  /*--------------------------------------------------------------------------*/
  memset(hdml, 0, sizeof(Hdml));
  hdml->doc      = doc;
  hdml->card     = qs_alloc_zero_byte_string(r);
  hdml->spec     = spec;
  hdml->conf     = ap_get_module_config(r->per_dir_config, &chxj_module);

  for (ii=0; ii<MAX_FORM_COUNT; ii++) 
  {
    hdml->var_cnt[ii]     = 0;
    hdml->postdata[ii]    = qs_alloc_zero_byte_string(r);
  }

  for (ii=0; ii<MAX_RADIO_COUNT; ii++) 
  {
    for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) 
    {
      hdml->radio_value_list[ii][jj] = NULL;
    }
    hdml->radio_name_list[ii] = NULL;
    hdml->radio_out_cnt[ii] = 0;
    hdml->radio_checked_value[ii] = NULL;
  }
  hdml->init_vars      = qs_alloc_zero_byte_string(r);

  doc->r               = r;

  /*--------------------------------------------------------------------------*/
  /* for HDML Counter                                                         */
  /*--------------------------------------------------------------------------*/
  gconf = ap_get_module_config(r->server->module_config, &chxj_module);
  apr_global_mutex_lock(gconf->client_lock);
  shm_segment = apr_shm_baseaddr_get(gconf->client_shm);
  shm = (int*)shm_segment;
  hdml->form_cnt = *shm;
  apr_global_mutex_unlock(gconf->client_lock); 

  hdml->out = qs_alloc_zero_byte_string(r);
}

/**
 * It is main processing converted from CHTML to HDML. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The route node is specified. 
 * @param indent [i]   Depth from the route node is specified. At present, 
 *                     other use doesn't exist. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_node_exchange(Hdml* hdml, Node* node,  int indent) 
{
  Node*         child;
  Doc*          doc   = hdml->doc; 
  request_rec*  r     = doc->r;

  for (child = qs_get_child_node(doc,node); child != NULL; child = qs_get_next_node(doc,child)) 
  {
    char* name = qs_get_node_name(doc,child);

    /*----------------------------------------------------------------------*/
    /* When </ HDML > tag has already been output, it doesn't process it.   */
    /*----------------------------------------------------------------------*/
    if (hdml->hdml_end_flag == 1) 
    {
      continue;
    }

    /*------------------------------------------------------------------------*/
    /* <HTML>                                                                 */
    /*------------------------------------------------------------------------*/
    if (strcasecmp(name, "html") == 0) 
    {
      hdml->out = hdml_1_0_start_html_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_html_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "meta") == 0) 
    {
      hdml->out = hdml_1_0_start_meta_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_end_meta_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HEAD>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "head") == 0) 
    {
      hdml->out = hdml_1_0_start_head_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_head_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "title") == 0) 
    {
      hdml->out = hdml_1_0_start_title_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_title_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BASE>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "base") == 0) 
    {
      hdml->out = hdml_1_0_start_base_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_end_base_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BODY>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "body") == 0) 
    {
      hdml->out = hdml_1_0_start_body_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_body_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "a") == 0) 
    {
      hdml->out = hdml_1_0_start_a_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_a_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "br") == 0) 
    {
      hdml->out = hdml_1_0_start_br_tag(hdml, child);
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_br_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FONT>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "font") == 0) 
    {
      hdml->out = hdml_1_0_start_font_tag(hdml, child);
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_font_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FORM>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "form") == 0) 
    {
      hdml->out = hdml_1_0_start_form_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_form_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <INPUT>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "input") == 0) 
    {
      hdml->out = hdml_1_0_start_input_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_input_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "select") == 0) 
    {
      hdml->out = hdml_1_0_start_select_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_select_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "option") == 0) 
    {
      hdml->out = hdml_1_0_start_option_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_option_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "div") == 0) 
    {
      hdml->out = hdml_1_0_start_div_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_node_exchange(hdml, child, indent+1);
      hdml->out = hdml_1_0_end_div_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "hr") == 0) 
    {
      hdml->out = hdml_1_0_start_hr_tag(hdml, child);
      hdml->out = hdml_1_0_end_hr_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <CENTER>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "center") == 0) 
    {
      hdml->out = hdml_1_0_start_center_tag(hdml, child);
      hdml->out = hdml_1_0_node_exchange(hdml, child,indent+1);
      hdml->out = hdml_1_0_end_center_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <IMG>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "img") == 0) 
    {
      hdml->out = hdml_1_0_start_img_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = hdml_1_0_end_img_tag  (hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <CHXJ:IF>                                                              */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "chxj:if") == 0) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag found");
      if (chxj_chxjif_is_mine(hdml->spec, doc, child))
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
        hdml_1_0_chxjif_tag(hdml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* NORMAL TEXT                                                            */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "text") == 0) 
    {
      char* textval;
      char* tmp;
      char* tdst;
      char one_byte[2];
      int ii;
      int tdst_len = 0;

      textval = qs_get_node_value(doc,child);
      textval = qs_trim_string(r, textval);
      if (strlen(textval) == 0)
      {
        continue;
      }

      if (hdml->option_flag == 1) 
      {
        continue;
      }

      tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
      memset(tmp, 0, qs_get_node_size(doc,child)+1);
      tdst = apr_palloc(r->pool, 1);
      tdst[0] = '\0';
      one_byte[0] = '\0';
      one_byte[1] = '\0';

      for (ii=0; ii<qs_get_node_size(doc,child); ii++) 
      {
        char* out;
        int rtn = hdml_search_emoji(hdml, &textval[ii], &out);
        if (rtn != 0)
        {
          tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
          ii += (rtn-1);
          continue;
        }
        if (is_sjis_kanji(textval[ii])) 
        {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          one_byte[0] = textval[ii+1];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          ii++;
        }
        else if (textval[ii] != '\r' && textval[ii] != '\n') 
        {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        }
      }
      memcpy(tmp, textval, strlen(textval)-1);

      hdml_tag_output_upper_half(hdml, node);
      qs_output_to_hdml_out(hdml, tdst);
      hdml->hdml_br_flag = 0;
    }
  }
  hdml->out[hdml->out_len] = 0;
  return hdml->out;
}

static int
hdml_search_emoji(Hdml* hdml, char* txt, char** rslt)
{
  emoji_t* ee;
  request_rec* r;
  device_table* spec;
  int len;

  spec = hdml->spec;

  len = strlen(txt);
  r = hdml->doc->r;

  if (spec == NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");
  }

  for (ee = hdml->conf->emoji;
       ee;
       ee = ee->next) 
  {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (ee->imode == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }
    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0)
    {
      if (spec == NULL || spec->emoji_type == NULL)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }

      if (strcasecmp(hdml->spec->emoji_type, "a") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      } 
      else
      if (strcasecmp(hdml->spec->emoji_type, "b") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeB);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "c") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeC);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "d") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeD);
        return strlen(ee->imode->string);
      }
      else
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }
      return 0;
    }
    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) 
    {
      if (spec == NULL || spec->emoji_type == NULL)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return 2;
      }

      if (strcasecmp(hdml->spec->emoji_type, "a") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return 2;
      } 
      else
      if (strcasecmp(hdml->spec->emoji_type, "b") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeB);
        return 2;
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "c") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeC);
        return 2;
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "d") == 0)
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeD);
        return 2;
      }
      else
      {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return 2;
      }
      return 0;
    }
  }
  return 0;
}
/**
 * It is a handler who processes the HTML tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_html_tag(Hdml* hdml, Node* node) 
{
  qs_output_to_hdml_out(hdml, 
    "<HDML VERSION=3.0 TTL=0 MARKABLE=TRUE>\n"
    "<NODISPLAY NAME=D0>\n"
    "<ACTION TYPE=ACCEPT TASK=GOSUB DEST=#D1 NEXT=#D2 CLEAR=TRUE>\n"
    "</NODISPLAY>\n"
    );

  hdml->card_cnt = 2;

  return hdml->out;
}

/**
 * It is a handler who processes the HTML tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_html_tag(Hdml* hdml, Node* child) 
{
  qs_output_to_hdml_card(hdml, 
                  "<NODISPLAY NAME=D1>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN VARS=\""
                  );

  if (strlen(hdml->init_vars) != 0) 
  {
    qs_output_to_hdml_card(hdml, hdml->init_vars   );
  }
  else 
  {
    qs_output_to_hdml_card(hdml, "_chxj_dmy="            );
  }

  qs_output_to_hdml_card(hdml,   
                  "\" CLEAR=TRUE>\n"
                  "</NODISPLAY>\n"
                  );

  qs_output_to_hdml_out(hdml, hdml->card );
  qs_output_to_hdml_out(hdml, "</HDML>\n");

  hdml->hdml_end_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the META tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_meta_tag(Hdml* hdml, Node* node) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the META tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_meta_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_head_tag(Hdml* hdml, Node* node) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_head_tag(Hdml* hdml, Node* child) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_title_tag(Hdml* hdml, Node* node) 
{
  qs_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"");
  hdml->found_title = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_title_tag(Hdml* hdml, Node* child) 
{
  qs_output_to_hdml_out(hdml, "\">\n");

  return hdml->out;
}

/**
 * It is a handler who processes the BASE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_base_tag(Hdml* hdml, Node* node) 
{
  return hdml->out;
}


/**
 * It is a handler who processes the BASE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_base_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the BODY tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_body_tag(Hdml* hdml, Node* node) 
{
  Doc*         doc  = hdml->doc;
  Attr*        attr;

  if (hdml->found_title == 0)
  {
    qs_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"NO TITLE\">\n");
  }
  qs_output_to_hdml_out(hdml, "<ACTION TYPE=ACCEPT TASK=NOOP LABEL=\" \"");

  /*--------------------------------*/
  /* Get Attributes                 */
  /*--------------------------------*/
  for (attr = qs_get_attr(doc,node); attr != NULL; attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    if (strcasecmp(name, "bgcolor")     == 0) 
    {
      /* ignore */
    }
    else if (strcasecmp(name, "text")   == 0) 
    {
      /* ignore */
    }
    else if (strcasecmp(name, "link")   == 0) 
    {
      /* ignore */
    }
    else if (strcasecmp(name, "alink")  == 0) 
    {
      /* ignore */
    }
    else if (strcasecmp(name, "vlink")  == 0) 
    {
      /* ignore */
    }
  }

  qs_output_to_hdml_out(hdml, ">\n");

  return hdml->out;
}

/**
 * It is a handler who processes the BODY tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_body_tag(Hdml* hdml, Node* child) 
{
  qs_output_to_hdml_out(hdml, "\n</DISPLAY>\n");

  return hdml->out;
}

/**
 * It is a handler who processes the A tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_a_tag(Hdml* hdml, Node* node) 
{
  Doc*         doc  = hdml->doc;
  Attr*        attr;

  hdml_tag_output_upper_half(hdml,node);

  qs_output_to_hdml_out(hdml, "<A");


  /*------------------------------------*/
  /* Get Attributes                     */
  /*------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "name") == 0) 
    {
      /* IGNORE */
    }
    else if (strcasecmp(name, "href") == 0) 
    {
      if (strncasecmp(value, "mailto:", 7) == 0) 
      {
        qs_output_to_hdml_out(hdml, " TASK=GO DEST=\""     );
        qs_output_to_hdml_out(hdml, value                  );
        qs_output_to_hdml_out(hdml, "\" "                  );
      }
      else 
      if (strncasecmp(value, "tel:", 4) == 0) 
      {

        qs_output_to_hdml_out(hdml,  " TASK=CALL NUMBER=\"");
        qs_output_to_hdml_out(hdml, &value[4]              );
        qs_output_to_hdml_out(hdml, "\" "                  );
      }
      else 
      {
        qs_output_to_hdml_out(hdml, " TASK=GO DEST=\""     );
        qs_output_to_hdml_out(hdml, value                  );
        qs_output_to_hdml_out(hdml, "\""                   );
      }
    }
    else
    if (strcasecmp(name, "accesskey") == 0) 
    {
      if (strcasecmp(value, "0") != 0) {
        qs_output_to_hdml_out(hdml, " ACCESSKEY="          );
        qs_output_to_hdml_out(hdml, value                  );
        qs_output_to_hdml_out(hdml, ""                     );
      }
    }
    else
    if (strcasecmp(name, "cti") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ijam") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "utn") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "telbook") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "kana") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "email") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ista") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ilet") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "iswf") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "irst") == 0) 
    {
      /* ignore */
    }
  }
  qs_output_to_hdml_out(hdml, ">"  );

  hdml->hdml_a_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the A tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_a_tag(Hdml* hdml, Node* child) 
{
  qs_output_to_hdml_out(hdml, "</A>\n");

  hdml->hdml_a_flag = 0;

  return hdml->out;
}

/**
 * It is a handler who processes the BR tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_br_tag(Hdml* hdml, Node* node) 
{
  if (hdml->in_center > 0) 
  {
    hdml->in_center = 0;
  }
  if (hdml->div_in_center > 0) 
  {
    hdml->div_in_center = 0;
  }

  qs_output_to_hdml_out(hdml, "<BR>\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the BR tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_br_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FONT tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_font_tag(Hdml* hdml, Node* node) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FONT tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_end_font_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FORM tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
hdml_1_0_start_form_tag(Hdml* hdml, Node* node) 
{
  request_rec* r = hdml->doc->r;
  Attr* attr;
  Doc* doc = hdml->doc;
  char* act = NULL;

  hdml->form_tmp = apr_psprintf(r->pool,
                  "<NODISPLAY NAME=F%d>\n",
                  hdml->pure_form_cnt);
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "<ACTION TYPE=ACCEPT TASK=GO METHOD=POST DEST=\"",NULL);
  /* Get Attributes */
  for (attr = qs_get_attr(doc,node); attr != NULL; attr = qs_get_next_attr(doc,attr)) 
  {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "action") == 0) 
    {
      act = apr_psprintf(r->pool, "%s", value);
      break;
    }
  }
  if (act != NULL) 
  {
    hdml->form_tmp = apr_pstrcat(r->pool,
                             hdml->form_tmp,
                             act,
                             NULL);
  }
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "\" ",NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "POSTDATA=\"", NULL);

  return hdml->out;
}

/**
 * handler of the form end tag. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param child [i]  unused.
 * @return The HDML output result after it edits it is returned. 
 */
static char*
hdml_1_0_end_form_tag(Hdml* hdml, Node* child) 
{
  request_rec* r = hdml->doc->r;

  qs_output_to_postdata(hdml, "_chxj_dmy=");

  hdml->form_tmp = apr_pstrcat(r->pool, 
                             hdml->form_tmp,
                             hdml->postdata[hdml->pure_form_cnt],
                             NULL);

  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "\" CLEAR=TRUE >\n", 
                               NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "</NODISPLAY>\n",
                               NULL);

  qs_output_to_hdml_card(hdml, hdml->form_tmp);

  hdml->form_tmp = NULL;
  hdml->pure_form_cnt++;

  return hdml->out;
}

/**
 * It is a handler that takes charge of the processing of the input tag. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param node [i]   The tag node to be processed is specified. 
 */
static char*
hdml_1_0_start_input_tag(Hdml* hdml, Node* node) 
{
  Doc* doc = hdml->doc;
  Attr* attr;

  /*--------------------------------------------------------------------------*/
  /* The attribute of the input tag is acquired.                              */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); attr != NULL; attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "type") == 0) 
    {
      if (strcasecmp(value, "text") == 0)
      {
        /*--------------------------------------------------------------------*/
        /* "input type ='text'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        hdml_do_input_text_tag(hdml, node);
      }
      if (strcasecmp(value, "password") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='password'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        hdml_do_input_password_tag(hdml, node);
      }
      else
      if (strcasecmp(value, "submit") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='submit'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        hdml_do_input_submit_tag(hdml, node);
      }
      else 
      if (strcasecmp(value, "hidden") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='hidden'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        hdml_do_input_hidden_tag(hdml, node);
      }
      else
      if (strcasecmp(value, "radio") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='radio'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        hdml_do_input_radio_tag(hdml, node);
      }
      else 
      if (strcasecmp(value, "checkbox") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='checkbox'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        hdml_do_input_checkbox_tag(hdml, node);
      }
    }
    else 
    if (strcasecmp(name, "name")      == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "value")     == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "size")      == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "maxlength") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "checked")   == 0) 
    {
      /* ignore */
    }
    else 
    if (strcasecmp(name, "accesskey") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "istyle")    == 0) 
    {
      /* ignore */
    }
  }
  return hdml->out;
}

/**
 * The substitution processing of tag "input type = text" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=text is specified. 
 */
static void
hdml_do_input_text_tag(Hdml* hdml, Node* tag)
{
  Doc*          doc   = hdml->doc;
  request_rec*  r     = doc->r;
  char*         mlen;
  char*         val;
  char*         is;
  char*         nm;
  char*         fmt;
  int           ii;
  hdml_tag_output_upper_half(hdml, tag);

  hdml->card_cnt++;
  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool,
                          "<A TASK=GOSUB LABEL=入力 DEST=#D%d "
                          "VARS=\"V=$%s%02d\" RECEIVE=%s%02d>",
                          hdml->card_cnt,
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt],
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]
                  ));

  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "[$%s%02d]</A>\n"  , 
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  /*--------------------------------------------------------------------------*/
  /* ENTRY CARD is output here.                                               */
  /*--------------------------------------------------------------------------*/
  qs_output_to_hdml_card(hdml, "<ENTRY NAME="                               );
  qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, "D%d ", hdml->card_cnt));
  qs_output_to_hdml_card(hdml, " KEY=V DEFAULT=$V "                         );

  mlen = NULL;
  is   = NULL;
  val  = NULL;
  fmt  = NULL;
  nm = qs_get_name_attr(doc, tag, r);
  if (nm == NULL)
  {
    nm = qs_alloc_zero_byte_string(r);
  }

  qs_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, 
                          "%s=$%s%02d", 
                          nm,
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r);
  is   = qs_get_istyle_attr     (doc, tag, r);
  val  = qs_get_value_attr      (doc, tag, r);

  fmt  = qs_conv_istyle_to_format(r, is);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "qs_conv_istyle_to_format end");
        
  if (fmt != NULL) 
  {
    if (mlen != NULL) 
    {
      for (ii=0; ii<strlen(mlen); ii++)
      {
        if (mlen[ii] < '0' || mlen[ii] > '9')
        {
          mlen = apr_psprintf(r->pool, "0");
          break;
        }
      }
      qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=%d%s", atoi(mlen), fmt));
    }
    else 
    {
      qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );
    }
  }

  qs_output_to_hdml_card(hdml, 
                  " MARKABLE=FALSE>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\n"
                  "</ENTRY>\n");

  if (val != NULL) 
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=%s", 
                        qs_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt],
                        ap_escape_uri(r->pool,val)));
  }
  else 
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=", 
                        qs_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt]));
  }
  hdml->var_cnt[hdml->pure_form_cnt]++;
}

/**
 * The substitution processing of tag "input type = password" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=password is specified. 
 */
static void
hdml_do_input_password_tag(Hdml* hdml, Node* tag)
{
  Doc*            doc = hdml->doc;
  request_rec*    r   = doc->r;
  char*           mlen;
  char*           val;
  char*           is;
  char*           nm;
  char*           fmt;
  hdml_tag_output_upper_half(hdml, tag);

  hdml->card_cnt++;
  qs_output_to_hdml_out(hdml, "<A TASK=GOSUB LABEL=入力 DEST="              );
  qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, "#D%d ", hdml->card_cnt));
  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "VARS=\"V=$%s%02d\" ", 
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "RECEIVE=%s%02d>"  , 
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));
  qs_output_to_hdml_out(hdml, "[****]</A>\n"          );

  qs_output_to_hdml_card(hdml, "<ENTRY NAME="                               );
  qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, "D%d ", hdml->card_cnt));
  qs_output_to_hdml_card(hdml, " KEY=V DEFAULT=$V "                         );

  mlen = NULL;
  is   = NULL;
  val  = NULL;
  fmt  = NULL;

  nm = qs_get_name_attr(doc, tag, r);
  if (nm == NULL)
  {
    nm = qs_alloc_zero_byte_string(r);
  }
  qs_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, 
                          "%s=$%s%02d", 
                          nm,
                          qs_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r);
  val  = qs_get_value_attr      (doc, tag, r);
  /*--------------------------------------------------------------------------*/
  /* Default is a figure input.                                               */
  /*--------------------------------------------------------------------------*/
  fmt = apr_psprintf(r->pool, "N");
  if (mlen != NULL) 
  {
    if (chxj_chk_numeric(mlen) != 0)
    {
      mlen = apr_psprintf(r->pool, "0");
    }
    qs_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=%d%s", chxj_atoi(mlen), fmt));
  }
  else 
  {
    qs_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );
  }
  qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, " NOECHO=TRUE "));

  qs_output_to_hdml_card(hdml, 
                  " MARKABLE=FALSE>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\n"
                  "</ENTRY>\n");

  if (val != NULL) 
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=%s", 
                        qs_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt], 
                        ap_escape_uri(r->pool,val)));
  }
  else 
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=", 
                        qs_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt]));
  }
  hdml->var_cnt[hdml->pure_form_cnt]++;
}

/**
 * The substitution processing of tag "input type = submit" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=submit is specified. 
 */
static void
hdml_do_input_submit_tag(Hdml* hdml, Node* tag)
{
  Doc*          doc = hdml->doc;
  request_rec*  r   = doc->r;
  char*         nm  = NULL;
  char*         val = NULL;
  hdml_tag_output_upper_half(hdml, tag);

  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GO LABEL=OK DEST=#F%d ",
                          hdml->pure_form_cnt));

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  if (nm != NULL && val != NULL) 
  {
    qs_output_to_hdml_out(hdml, 
                    apr_psprintf(r->pool, "VARS=\"%s=%s\" ", 
                            nm, 
                            ap_escape_uri(r->pool,val)));
    if (strstr(hdml->postdata[hdml->pure_form_cnt], nm) == NULL) 
    {
      qs_output_to_postdata(hdml, apr_psprintf(r->pool,"%s=$%s", nm, nm));
    }
  }
  qs_output_to_hdml_out(hdml, ">"         );
  qs_output_to_hdml_out(hdml, val);
  qs_output_to_hdml_out(hdml, "</A>\n"    );
}

/**
 * The substitution processing of tag "input type = hidden" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=hidden is specified. 
 */
static void
hdml_do_input_hidden_tag(Hdml* hdml, Node* tag)
{
  Doc*          doc = hdml->doc;
  request_rec*  r   = doc->r;
  char*         nm  = NULL;
  char*         val = NULL;

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  if (nm != NULL && val != NULL) 
  {
    qs_output_to_postdata(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            ap_escape_uri(r->pool, val)));
  }
}

/**
 * The substitution processing of tag "input type = radio" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=radio is specified. 
 */
static void
hdml_do_input_radio_tag(Hdml* hdml, Node* tag)
{
  Doc*          doc       = hdml->doc;
  request_rec*  r         = doc->r;
  char*         nm        = NULL;
  char*         val       = NULL;
  int           ii;
  int           jj;
  int           kk;
  int           r_cnt;
  hdml_tag_output_upper_half(hdml, tag);

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  /*--------------------------------------------------------------------------*/
  /* The same name is searched out from the list made beforehand.             */
  /*--------------------------------------------------------------------------*/
  for (ii=0; ii<MAX_RADIO_COUNT; ii++) 
  {
    if (hdml->radio_name_list[ii] == NULL) 
    {
      /* @todo Oops..  */
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, 
                      "%s:%d Oops... radio list is null", 
                      APLOG_MARK);
      /*----------------------------------------------------------------------*/
      /* Processing is ended because it doesn't happen off the fly.           */
      /*----------------------------------------------------------------------*/
      return;
    }

    if (strcasecmp(hdml->radio_name_list[ii], nm) == 0) 
    {
      break;
    }
  }
  if (ii == MAX_RADIO_COUNT) 
  {
    /* @todo Oops.. */
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, 
                    "%s:%d Oops... The same name was not in the list. ", 
                    APLOG_MARK);
    /*------------------------------------------------------------------------*/
    /* Processing is ended because it doesn't happen off the fly.             */
    /*------------------------------------------------------------------------*/
    return;
  }

  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GOSUB "
                          "LABEL=\"入力\" "
                          "DEST=#R%d VARS=\"VAL=%s\" "
                          "RECEIVE=\"%s;", 
                          ii, 
                          val, 
                          nm));

  if (hdml->radio_out_cnt[ii] == 0) 
  {
    qs_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool,
                            "<NODISPLAY NAME=R%d>\n",
                            ii));
    qs_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<ACTION "
                            "TYPE=ACCEPT TASK=RETURN RETVALS=\"$VAL;"));
  }

  kk = hdml->radio_out_cnt[ii];
  for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) 
  {
    if (hdml->radio_value_list[ii][jj] == NULL) 
    {
      break;
    }
  }
  r_cnt = jj;

  for (jj=0; jj<r_cnt; jj++) 
  {
    if (hdml->radio_value_list[ii][jj] == NULL)
    {
      break;
    }
    if (jj != 0) 
    {
      qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, ";"));
      if (hdml->radio_out_cnt[ii] == 0) 
      {
        qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, ";"));
      }
    }

    qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, "%s_%02d", nm, kk));
    if (hdml->radio_out_cnt[ii] == 0) 
    {
      if (jj == 0) 
      {
        qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, "X"));
      }
      else 
      {
        qs_output_to_hdml_card(hdml, apr_psprintf(r->pool, "_"));
      }
    }
    kk++;
    if (kk >= r_cnt) 
    {
      kk=0;
    }
  }
  qs_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "\" >$%s_%02d</A>", 
                          nm, 
                          hdml->radio_out_cnt[ii]));
  if (hdml->radio_out_cnt[ii] == 0) 
  {
    qs_output_to_hdml_card(hdml, "\">\n"         );
    qs_output_to_hdml_card(hdml, "</NODISPLAY>\n");

    qs_output_to_postdata(hdml,  apr_psprintf(r->pool, "%s%s=$%s", RADIO_BUTTON_PREFIX, nm, nm));

    for (jj=0; jj<r_cnt; jj++) 
    {
      if (hdml->radio_value_list[ii][jj] != NULL 
      &&  hdml->radio_checked_value[ii] != NULL) 
      {
        if (strcasecmp(hdml->radio_value_list[ii][jj], 
                       hdml->radio_checked_value[ii]) == 0)
        {
          qs_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=X", 
                                  nm, 
                                  jj));
        }
        else 
        {
          qs_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=_", 
                                  nm, 
                                  jj));
        }
      }
      else 
      {
        qs_output_to_init_vars(hdml, 
                        apr_psprintf(r->pool, 
                                "%s_%02d=_", 
                                nm, 
                                jj));
      }
    }

    if (hdml->radio_checked_value[ii] != NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "radio button is checked. checked value is [%s]", 
                    hdml->radio_checked_value[ii]);
      qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            hdml->radio_checked_value[ii]));
    }
    else
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "radio button is not checked. checked value is []");
      qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=", 
                            nm));
    }
  }
  hdml->radio_out_cnt[ii]++;
}

/**
 * The substitution processing of tag "input type = checkbox" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=checkbox is specified. 
 */
static void
hdml_do_input_checkbox_tag(Hdml* hdml, Node* tag)
{
  Doc*          doc       = hdml->doc;
  request_rec*  r         = doc->r;
  char*         nm        = NULL;
  char*         val       = NULL;
  int           chk;

  /*--------------------------------------------------------------------------*/
  /* It is posted to the one without the checked attribute.                   */
  /* However, they were able to be removed with INPUT FILTER.                 */
  /*--------------------------------------------------------------------------*/
  if (hdml->has_checkbox == 0) 
  {
    hdml->has_checkbox++;
    qs_output_to_hdml_card(hdml, 
                    "<NODISPLAY NAME=\"_chk\">\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_uchk;$V;X\" >\n"
                    "</NODISPLAY>\n"
                    "<NODISPLAY NAME=\"_uchk\">\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_chk;;_\" >\n"
                    "</NODISPLAY>\n"
                    );
  }
        
  /*--------------------------------------------------------------------------*/
  /* It is examined whether it is CHECKED.                                    */
  /*--------------------------------------------------------------------------*/
  chk = qs_is_checked_checkbox_attr(doc, tag, r);

  /*--------------------------------------------------------------------------*/
  /* The value of the name attribute and the value attribute is acquired      */
  /* respectively.                                                            */
  /*--------------------------------------------------------------------------*/
  val = qs_get_value_attr(doc, tag, r);
  nm  = qs_get_name_attr(doc, tag, r);

  if (val == NULL) 
  {
    val    = qs_alloc_zero_byte_string(r);
  }
  if (nm == NULL)
  {
    nm   = qs_alloc_zero_byte_string(r);
  }
  qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                "<A TASK=GOSUB LABEL=\"ﾁｪｯｸ\" "
                                   "DEST=\"#$%s%02d\" "
                                   "VARS=\"V=%s\" "
                                   "RECEIVE=\"%s%02d;%s%02d;%s%02d\">"
                                   "$%s%02d</A>\n",
                                qs_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 0,
                                val,
                                qs_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 0,
                                qs_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 1,
                                qs_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 2,
                                qs_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 2));
  if (chk) 
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s%02d=_uchk&%s%02d=%s&%s%02d=X",
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 0,
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 1,
                            ap_escape_uri(r->pool,val),
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 2
                            ));
  }
  else
  {
    qs_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s%02d=_chk&%s%02d=&%s%02d=_",
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 0,
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 1,
                            qs_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 2
                            ));
  }

  qs_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, "%s%s=$%s%02d",
                                  CHECK_BOX_PREFIX,
                                  nm,
                                  qs_get_form_no(r, hdml),
                                  hdml->var_cnt[hdml->pure_form_cnt] + 1));
  hdml->var_cnt[hdml->pure_form_cnt] += 3;
}

/**
 * The ISTYLE attribute is converted into the HDML form.
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param is   [i]   The value of the ISTYLE attribute is specified. 
 * @return The ISTYLE attribute converted into the HDML form is returned. 
 */
char*
qs_conv_istyle_to_format(request_rec* r, char* is)
{
  char* fmt;
  if (is == NULL)
  {
    return NULL;
  }

  if (strcasecmp(is, "1") == 0)
  {
    fmt = apr_psprintf(r->pool, "M");
  }
  else
  if (strcasecmp(is, "2") == 0)
  {
    fmt = apr_psprintf(r->pool, "M");
  }
  else
  if (strcasecmp(is, "3") == 0)
  {
    fmt = apr_psprintf(r->pool, "m");
  }
  else
  if (strcasecmp(is, "4") == 0)
  {
    fmt = apr_psprintf(r->pool, "N");
  }
  else 
  {
    return NULL;
  }
  return fmt;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_end_input_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_start_center_tag(Hdml* hdml, Node* node) 
{
  hdml->center++;
  hdml->in_center++;

  if (hdml->hdml_br_flag == 0) 
  {
    hdml = qs_output_to_hdml_out(hdml, "<BR>\n");
  }
  hdml = qs_output_to_hdml_out(hdml, "<CENTER>");

  return hdml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_end_center_tag(Hdml* hdml, Node* child) 
{
  hdml->center = 0;
  hdml->in_center = 0;

  hdml = qs_output_to_hdml_out(hdml, "<BR>\n");
  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_start_hr_tag(Hdml* hdml, Node* node) 
{
  if (hdml->hdml_br_flag == 0) 
  {
    qs_output_to_hdml_out(hdml, "<BR>\n");
    if (hdml->in_center > 0) 
    {
      hdml->in_center--;
    }
    else
    if (hdml->div_in_center > 0)
    {
      hdml->div_in_center--;
    }
  }

  qs_output_to_hdml_out(hdml, "<CENTER>────────<BR>\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_end_hr_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_start_img_tag(Hdml* hdml, Node* node) 
{
  Doc*          doc  = hdml->doc;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table* spec = hdml->spec;
#endif
  Attr*         attr;

  hdml_tag_output_upper_half(hdml, node);

  qs_output_to_hdml_out(hdml, "<img");

  /* Get Attributes */
  for (attr = qs_get_attr(doc,node); attr != NULL; attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "src") == 0) 
    {
      qs_output_to_hdml_out(hdml, " src=\"");
#ifdef IMG_NOT_CONVERT_FILENAME
      qs_output_to_hdml_out(hdml, value    );
#else
      qs_output_to_hdml_out(hdml, chxj_img_conv(doc->r, spec,value));
#endif
      qs_output_to_hdml_out(hdml, "\""     );
    }
    else 
    if (strcasecmp(name, "align" ) == 0) 
    {
      qs_output_to_hdml_out(hdml, " align=\"" );
      qs_output_to_hdml_out(hdml, value       );
      qs_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "width" ) == 0) 
    {
      qs_output_to_hdml_out(hdml, " width=\"");
      qs_output_to_hdml_out(hdml, value      );
      qs_output_to_hdml_out(hdml, "\""       );
    }
    else
    if (strcasecmp(name, "height") == 0) 
    {
      qs_output_to_hdml_out(hdml, " height=\"");
      qs_output_to_hdml_out(hdml, value       );
      qs_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "hspace") == 0) 
    {
      qs_output_to_hdml_out(hdml, " hspace=\"");
      qs_output_to_hdml_out(hdml, value       );
      qs_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "vspace") == 0) 
    {
      qs_output_to_hdml_out(hdml, " vspace=\"");
      qs_output_to_hdml_out(hdml, value       );
      qs_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) 
    {
      qs_output_to_hdml_out(hdml, " alt=\""   );
      qs_output_to_hdml_out(hdml, value       );
      qs_output_to_hdml_out(hdml, "\""        );
    }
  }
  qs_output_to_hdml_out(hdml, ">"             );

  return hdml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
hdml_1_0_end_img_tag(Hdml* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_start_select_tag(Hdml* hdml, Node* node)  
{
  Doc*         doc = hdml->doc;
  request_rec* r   = doc->r;
  Attr*        attr;

  hdml_tag_output_upper_half(hdml, node);

  hdml->card_cnt++;

  qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                   "<A TASK=GOSUB LABEL=選択 "
                                   "VARS=\"V=$%s%02d\" DEST=#D%d "
                                   "RECEIVE=\"%s%02d;%s%02d\">"
                                   "$%s%02d</A>\n",
                                   qs_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+0, 
                                   hdml->card_cnt,
                                   qs_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+0, 
                                   qs_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+1, 
                                   qs_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+1));

  qs_output_to_hdml_card(hdml, 
                  apr_psprintf(r->pool, 
                          "<CHOICE KEY=V NAME=D%d ", hdml->card_cnt));
  qs_output_to_hdml_card(hdml, 
                  apr_psprintf(r->pool, 
                          "DEFAULT=$V METHOD=ALPHA MARKABLE=FALSE>\n"));
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr != NULL; 
       attr=qs_get_next_attr(doc,attr)) 
  {
    char* name      = qs_get_attr_name(doc,attr);
    char* value     = qs_get_attr_value(doc,attr);
    char* selval    = NULL;
    char* selvaltxt = NULL;
    if (strcasecmp(name, "name") == 0) 
    {

      qs_output_to_postdata(hdml, 
                      apr_psprintf(r->pool, "%s=$%s%02d", 
                              value,
                              qs_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt]));
      selval = qs_get_selected_value(doc, node, r);
      if (selval == NULL) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "selected value not found");
        selval = qs_alloc_zero_byte_string(r);
      }
      else
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "selected value found[%s]" , selval);
      }
      selvaltxt = qs_get_selected_value_text(doc, node, r);
      if (selvaltxt == NULL)
      {
        selvaltxt = qs_alloc_zero_byte_string(r);
      }
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "selvaltxt:[%s]" ,selvaltxt);

      qs_output_to_init_vars(hdml, 
                      apr_psprintf(r->pool, 
                              "%s%02d=%s&%s%02d=%s",
                              qs_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt] + 0,
                              selval, 
                              qs_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt] + 1,
                              selvaltxt));

      hdml->var_cnt[hdml->pure_form_cnt] += 2;
      break;
    }
  }
  return hdml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_end_select_tag(Hdml* hdml,  Node* node)  
{
  qs_output_to_hdml_card(hdml, "</CHOICE>\n");

  return hdml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_start_option_tag(Hdml* hdml, Node* node) 
{
  request_rec* r     = hdml->doc->r;
  Doc*         doc   = hdml->doc;
  Node*        child;
  char*        val;
  char*        txtval;

  hdml->card_cnt++;

  hdml->option_flag = 1;
  val = qs_get_value_attr(doc, node, r);

  /*--------------------------------------------------------------------------*/
  /* The child node of the object tag node acquires the value in assumption   */
  /* that is the TEXT node.                                                   */
  /*--------------------------------------------------------------------------*/
  child = qs_get_child_node(doc, node);
  if (child == NULL) 
  {
    txtval    = apr_palloc(r->pool, 1);
    txtval[0] = 0;
  }
  else 
  {
    txtval = qs_get_node_value(doc, child);
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "txtval:[%s]" , txtval);

  if (val != NULL && txtval != NULL) 
  {
    qs_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<CE TASK=RETURN VALUE=\"%s\" "
                            "RETVALS=\"$V;%s\">%s</CE>\n", 
                            val, 
                            qs_trim_string(r,txtval), 
                            qs_trim_string(r,txtval)));
  }
 
  return hdml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_end_option_tag(Hdml* hdml,  Node* node)  
{
  hdml->option_flag = 0;
  return hdml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_start_div_tag(Hdml* hdml, Node* node) 
{
  Doc*    doc   = hdml->doc;
  Attr*   attr;

  /*--------------------------------------------------------------------------*/
  /* If the br tag is not output immediately before the div tag appears, the  */
  /* br tag is output.                                                        */
  /*--------------------------------------------------------------------------*/
  if (hdml->hdml_br_flag == 0)
  {
    hdml->hdml_br_flag = 1;
    qs_output_to_hdml_out(hdml, "<BR>\n");
  }

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "align") == 0) 
    {
      if (strcasecmp(value, "right") == 0) 
      {
        hdml->div_right_flag = 1;
        qs_output_to_hdml_out(hdml, "<RIGHT>");
        hdml->hdml_br_flag = 0;
        break;
      }
      else if (strcasecmp(value, "center") == 0)
      {
        hdml->div_center_flag = 1;
        qs_output_to_hdml_out(hdml, "<CENTER>");
        hdml->hdml_br_flag = 0;
        break;
      }
    }
  }
 
  return hdml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
hdml_1_0_end_div_tag(Hdml* hdml,  Node* node)  
{
  request_rec* r = hdml->doc->r;


  if (hdml->div_right_flag == 1) 
  {
    qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\n"));
    hdml->div_right_flag = 0;
  }
  if (hdml->div_center_flag == 1) 
  {
    qs_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\n"));
    hdml->div_center_flag = 0;
    hdml->div_in_center   = 0;
  }
  return hdml->out;
}


/**
 * It is a wrapper of the apr_pstrcat function. 
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param o    [i]   The character string of connected origin is specified.
 * @param s    [i]   The character string connected with 'o' parameter is 
 *                   specified.
 * @param len  [i/o] The pointer to the area where the character string 
 *                   length is stored is specified. 
 * @return The character string after it connects it is returned. 
 */
char*
qs_out_apr_pstrcat(request_rec* r, char* o, char* s, int* len)
{
  *len = (strlen(s) + *len);
  return apr_pstrcat(r->pool, o, s, NULL);
}

/**
 * The prepositive character string used to generate the variable for HDML is 
 * generated. 
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param hdml [i]   The pointer to the HDML structure that maintains the seed 
 *                   to generate it is specified.
 * @return The character string after it generates it is returned. 
 */
static char*
qs_get_form_no(request_rec* r, Hdml* hdml) 
{
  char *result;
  int  fc = hdml->form_cnt + hdml->pure_form_cnt;

  result = apr_pcalloc(r->pool, 4);
  memset(result, 0, 4);

  result[0] = 'A' + ((fc / 100) % 26);
  result = apr_pstrcat(r->pool, result, 
                  apr_psprintf(r->pool, "%02d", (fc % 100)), NULL);
  return result;
}

/**
 * The number of tag nodes .."Input type =' radio '".. is counted. 
 *
 * @param hdml [i] The pointer to the HDML structure is specified.
 * @param node [i] The first parents node that counts the radio tag node is 
 *                 specified. 
 */
static void
hdml_1_0_count_radio_tag(Hdml* hdml, Node* node) 
{
  Node*         child;
  Doc*          doc       = hdml->doc; 
  request_rec*  r         = doc->r;

  /*--------------------------------------------------------------------------*/
  /* All the child nodes of the specified node are scanned.                   */
  /*--------------------------------------------------------------------------*/
  for (child =  qs_get_child_node(doc,node); 
       child != NULL; 
       child =  qs_get_next_node(doc,child)) 
  {
    char*     type;
    char*     rname;
    char*     rvalue;
    char*     chkd;
    char*     name;
    int       ii;
    int       jj;

    name = qs_get_node_name(doc,child);
    if (strcasecmp(name, "input") != 0)
    {
      hdml_1_0_count_radio_tag(hdml, child);
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found input tag");
    type = qs_get_type_attr(doc, child, r);
    if (type == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR,0,r,
        "Oops! The input tag without the type attribute has been found."
        "Please give a type.");
      continue;
    }
    if (strcasecmp(type, "radio") != 0)
    {
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found type=radio");
    rname  = qs_get_name_attr (doc, child, r);
    rvalue = qs_get_value_attr(doc, child, r);
    if (rname == NULL)
    {
      /*----------------------------------------------------------------------*/
      /* Oops!. The input tag without the name attribute has been found.      */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
        "Oops!. The input tag without the name attribute has been found."
        "Please give a name.");
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found name attribute");
    /*------------------------------------------------------------------------*/
    /* It scans in radio_name_list. When the same value exists, the           */
    /* value is overwrited.                                                   */
    /* The value is written in a new area when not is.                        */
    /*------------------------------------------------------------------------*/
    for (ii=0; ii<MAX_RADIO_COUNT; ii++) 
    {
      if (hdml->radio_name_list[ii] == NULL) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "new name:[%s]", rname);
        break;
      }
      if (strcasecmp(hdml->radio_name_list[ii], rname) == 0) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "already registered name:[%s]", rname);
        break;
      }
    }
    if (ii == MAX_RADIO_COUNT)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                      apr_psprintf(r->pool,
                        "I do not understand the name of the radiobutton "
                        "of %d piece or more. Please decrease "
                        "the number of radiobuttons.",
                        MAX_RADIO_COUNT));
      continue;
    }
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "add radio name:[%s]" ,rname);
    hdml->radio_name_list[ii] = apr_pstrdup(r->pool, rname);

    /*------------------------------------------------------------------------*/
    /* It adds it to radio_value_list.                                        */
    /*------------------------------------------------------------------------*/
    for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++)
    {
      if (hdml->radio_value_list[ii][jj] == NULL) 
      {
        break;
      }
    }
    if (jj == MAX_RADIO_VALUE_COUNT)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                      apr_psprintf(r->pool,
                        "I do not understand the value of the radiobutton "
                        "of %d piece or more. Please decrease "
                        "the number of radiobuttons.",
                        MAX_RADIO_VALUE_COUNT));
      continue;
    }
    hdml->radio_value_list[ii][jj] = apr_pstrdup(r->pool, rvalue);

    /*------------------------------------------------------------------------*/
    /* Now let's be the checked attribute or scan.                            */
    /*------------------------------------------------------------------------*/
    chkd = qs_get_checked_attr(hdml->doc, child, hdml->doc->r);
    if (chkd != NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      apr_psprintf(r->pool,
                              "The tag scanned now had the checked "
                              "attribute. The value is [%s].",
                              rvalue));
      hdml->radio_checked_value[ii] = apr_pstrdup(r->pool, rvalue);
    }
  }
}

/**
 * The character string is added, and output to the out member of the HDML 
 * structure.
 *
 * @param hdml [i/o] The pointer to the HDML structure that maintains the out 
 *                   member at the output destination is specified. 
 * @param s    [i]   The character string that should be output is specified. 
 * @return The pointer to the HDML structure after it processes it is returned.
 */
static Hdml* 
qs_output_to_hdml_out(Hdml* hdml, char* s)
{
  hdml->out = qs_out_apr_pstrcat(hdml->doc->r, hdml->out, s, &hdml->out_len);
  return hdml;
}

/**
 * The character string is added, and output to the card member of the HDML 
 * structure.
 *
 * @param hdml [i/o] The pointer to the HDML structure that maintains the 
 *                   card member at the output destination is specified. 
 * @param s    [i]   The character string that should be output is specified. 
 * @return The pointer to the HDML structure after it processes it is returned.
 */
static Hdml*
qs_output_to_hdml_card(Hdml* hdml, char* s)
{
  hdml->card = qs_out_apr_pstrcat(hdml->doc->r, hdml->card, s, &hdml->card_len);
  return hdml;
}

/**
 * The data for the post is added, and output. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                    destination is specified.
 * @param s    [i]   The character string that wants to output to postdata is 
 *                   specified.
 */
static void
qs_output_to_postdata(Hdml* hdml, char* s)
{
  request_rec*          r = hdml->doc->r;

  if (strlen(hdml->postdata[hdml->pure_form_cnt]) != 0)
  {
    hdml->postdata[hdml->pure_form_cnt] =
                  apr_pstrcat(r->pool,
                              hdml->postdata[hdml->pure_form_cnt],
                              "&",
                              NULL);
  }
  hdml->postdata[hdml->pure_form_cnt] =
          apr_pstrcat(r->pool, 
                          hdml->postdata[hdml->pure_form_cnt],
                          qs_trim_string(r, s),
                          NULL);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                  "POSTDATA:[%s]", 
                  hdml->postdata[hdml->pure_form_cnt] );
}

/**
 * The tag output upper half.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 */
static void
hdml_tag_output_upper_half(Hdml* hdml, Node* node)
{
  if (hdml->hdml_br_flag   == 1 
  &&  hdml->div_right_flag == 1) 
  {
    qs_output_to_hdml_out(hdml, "<RIGHT>");
    hdml->hdml_br_flag = 0;
  }

  if (hdml->hdml_br_flag == 1 
  &&  hdml->center > 0 
  &&  hdml->in_center == 0) 
  {
    qs_output_to_hdml_out(hdml, "<CENTER>");
    hdml->in_center++;
    hdml->hdml_br_flag = 0;
  }
  else
  if (hdml->hdml_br_flag == 1 
  &&  hdml->div_center_flag > 0 
  &&  hdml->div_in_center == 0) 
  {
    qs_output_to_hdml_out(hdml, "<CENTER>");
    hdml->div_in_center++;
    hdml->hdml_br_flag = 0;
  }
}

/**
 * The data for the init_vars is added, and output.
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                    destination is specified.
 * @param s    [i]   The character string that wants to output to postdata is
 *                   specified.
 */
static void
qs_output_to_init_vars(Hdml* hdml, char* s)
{
  request_rec*    r = hdml->doc->r;

  if (strlen(hdml->init_vars) > 0)
  {
    hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, "&", NULL);
  }
  hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, qs_trim_string(r,s), NULL);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "INIT_VARS:[%s]",
                  hdml->init_vars);
}

static void
hdml_1_0_chxjif_tag(Hdml* hdml, Node* node)
{
  Doc*         doc   = hdml->doc;
  Node*        child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    qs_output_to_hdml_out(hdml, child->otext);
    hdml_1_0_chxjif_tag(hdml, child);
  }
}
/*
 * vim:ts=2 et
 */
