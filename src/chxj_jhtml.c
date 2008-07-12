/*
 * Copyright (C) 2005-2008 Atsushi Konno All rights reserved.
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
#include "chxj_jhtml.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"


#define GET_JHTML(X) ((jhtml_t *)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { jhtml->out = BUFFERED_WRITE_LITERAL(jhtml->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { jhtml->out = (X) ? BUFFERED_WRITE_VALUE(jhtml->out, &doc->buf, (X))  \
                                              : BUFFERED_WRITE_LITERAL(jhtml->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(jhtml->conf); W_V(nlcode); } while (0)

static char *s_jhtml_start_html_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_html_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_meta_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_meta_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_head_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_head_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_title_tag    (void *pdoc, Node *node);
static char *s_jhtml_end_title_tag      (void *pdoc, Node *node);
static char *s_jhtml_start_base_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_base_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_body_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_body_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_a_tag        (void *pdoc, Node *node);
static char *s_jhtml_end_a_tag          (void *pdoc, Node *node);
static char *s_jhtml_start_pre_tag      (void *pdoc, Node *node);
static char *s_jhtml_end_pre_tag        (void *pdoc, Node *node);
static char *s_jhtml_start_p_tag        (void *pdoc, Node *node);
static char *s_jhtml_end_p_tag          (void *pdoc, Node *node);
static char *s_jhtml_start_ul_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_ul_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_ol_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_ol_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_li_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_li_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_br_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_br_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_tr_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_tr_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_font_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_font_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_form_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_form_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_input_tag    (void *pdoc, Node *node);
static char *s_jhtml_end_input_tag      (void *pdoc, Node *node);
static char *s_jhtml_start_center_tag   (void *pdoc, Node *node);
static char *s_jhtml_end_center_tag     (void *pdoc, Node *node);
static char *s_jhtml_start_hr_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_hr_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_img_tag      (void *pdoc, Node *node);
static char *s_jhtml_end_img_tag        (void *pdoc, Node *node);
static char *s_jhtml_start_select_tag   (void *pdoc, Node *node);
static char *s_jhtml_end_select_tag     (void *pdoc, Node *node);
static char *s_jhtml_start_option_tag   (void *pdoc, Node *node);
static char *s_jhtml_end_option_tag     (void *pdoc, Node *node);
static char *s_jhtml_start_div_tag      (void *pdoc, Node *node);
static char *s_jhtml_end_div_tag        (void *pdoc, Node *node);
static char *s_jhtml_start_textarea_tag (void *pdoc, Node *node);
static char *s_jhtml_end_textarea_tag   (void *pdoc, Node *node);
static char *s_jhtml_start_b_tag        (void *pdoc, Node *node);
static char *s_jhtml_end_b_tag          (void *pdoc, Node *node);
static char *s_jhtml_chxjif_tag         (void *pdoc, Node *node); 
static char *s_jhtml_text_tag           (void *pdoc, Node *node);
static char *s_jhtml_start_blockquote_tag(void *pdoc, Node *node);
static char *s_jhtml_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_jhtml_start_dir_tag      (void *pdoc, Node *node);
static char *s_jhtml_end_dir_tag        (void *pdoc, Node *node);
static char *s_jhtml_start_dl_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_dl_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_dt_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_dt_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_dd_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_dd_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h1_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h1_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h2_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h2_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h3_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h3_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h4_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h4_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h5_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h5_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_h6_tag       (void *pdoc, Node *node);
static char *s_jhtml_end_h6_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_menu_tag     (void *pdoc, Node *node);
static char *s_jhtml_end_menu_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_jhtml_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_jhtml_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_jhtml_start_blink_tag  (void *pdoc, Node *node);
static char *s_jhtml_end_blink_tag    (void *pdoc, Node *node);
static char *s_jhtml_start_marquee_tag(void *pdoc, Node *node);
static char *s_jhtml_end_marquee_tag  (void *pdoc, Node *node);
static char *s_jhtml_newline_mark       (void *pdoc, Node *node);
static char *s_jhtml_link_tag           (void *pdoc, Node *node);

static void  s_init_jhtml(jhtml_t *jhtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_jhtml_search_emoji(jhtml_t *jhtml, char *txt, char **rslt);

static char *chxj_istyle_to_mode(apr_pool_t *p, const char *s);
static css_prop_list_t *s_jhtml_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_jhtml_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);



tag_handler jhtml_handler[] = {
  /* tagHTML */
  {
    s_jhtml_start_html_tag,
    s_jhtml_end_html_tag,
  },
  /* tagMETA */
  {
    s_jhtml_start_meta_tag,
    s_jhtml_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_jhtml_start_textarea_tag,
    s_jhtml_end_textarea_tag,
  },
  /* tagP */
  {
    s_jhtml_start_p_tag,
    s_jhtml_end_p_tag,
  },
  /* tagPRE */
  {
    s_jhtml_start_pre_tag,
    s_jhtml_end_pre_tag,
  },
  /* tagUL */
  {
    s_jhtml_start_ul_tag,
    s_jhtml_end_ul_tag,
  },
  /* tagLI */
  {
    s_jhtml_start_li_tag,
    s_jhtml_end_li_tag,
  },
  /* tagOL */
  {
    s_jhtml_start_ol_tag,
    s_jhtml_end_ol_tag,
  },
  /* tagH1 */
  {
    s_jhtml_start_h1_tag,
    s_jhtml_end_h1_tag,
  },
  /* tagH2 */
  {
    s_jhtml_start_h2_tag,
    s_jhtml_end_h2_tag,
  },
  /* tagH3 */
  {
    s_jhtml_start_h3_tag,
    s_jhtml_end_h3_tag,
  },
  /* tagH4 */
  {
    s_jhtml_start_h4_tag,
    s_jhtml_end_h4_tag,
  },
  /* tagH5 */
  {
    s_jhtml_start_h5_tag,
    s_jhtml_end_h5_tag,
  },
  /* tagH6 */
  {
    s_jhtml_start_h6_tag,
    s_jhtml_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_jhtml_start_head_tag,
    s_jhtml_end_head_tag,
  },
  /* tagTITLE */
  {
    s_jhtml_start_title_tag,
    s_jhtml_end_title_tag,
  },
  /* tagBASE */
  {
    s_jhtml_start_base_tag,
    s_jhtml_end_base_tag,
  },
  /* tagBODY */
  {
    s_jhtml_start_body_tag,
    s_jhtml_end_body_tag,
  },
  /* tagA */
  {
    s_jhtml_start_a_tag,
    s_jhtml_end_a_tag,
  },
  /* tagBR */
  {
    s_jhtml_start_br_tag,
    s_jhtml_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_jhtml_start_tr_tag,
    s_jhtml_end_tr_tag,
  },
  /* tagTD */
  {
    NULL,
    NULL,
  },
  /* tagTBODY */
  {
    NULL,
    NULL,
  },
  /* tagFONT */
  {
    s_jhtml_start_font_tag,
    s_jhtml_end_font_tag,
  },
  /* tagFORM */
  {
    s_jhtml_start_form_tag,
    s_jhtml_end_form_tag,
  },
  /* tagINPUT */
  {
    s_jhtml_start_input_tag,
    s_jhtml_end_input_tag,
  },
  /* tagCENTER */
  {
    s_jhtml_start_center_tag,
    s_jhtml_end_center_tag,
  },
  /* tagHR */
  {
    s_jhtml_start_hr_tag,
    s_jhtml_end_hr_tag,
  },
  /* tagIMG */
  {
    s_jhtml_start_img_tag,
    s_jhtml_end_img_tag,
  },
  /* tagSELECT */
  {
    s_jhtml_start_select_tag,
    s_jhtml_end_select_tag,
  },
  /* tagOPTION */
  {
    s_jhtml_start_option_tag,
    s_jhtml_end_option_tag,
  },
  /* tagDIV */
  {
    s_jhtml_start_div_tag,
    s_jhtml_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_jhtml_chxjif_tag,
    NULL,
  },
  /* tagNOBR */
  {
    NULL,
    NULL,
  },
  /* tagSMALL */
  {
    NULL,
    NULL,
  },
  /* tagSTYLE */
  {
    NULL,
    NULL,
  },
  /* tagSPAN */
  {
    NULL,
    NULL,
  },
  /* tagTEXT */
  {
    s_jhtml_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_jhtml_start_b_tag,
    s_jhtml_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_jhtml_start_dt_tag,
    s_jhtml_end_dt_tag,
  },
  /* tagLEGEND */
  {
    NULL,
    NULL,
  },
  /* tagLABEL */
  {
    NULL,
    NULL,
  },
  /* tagBLOCKQUOTE */
  {
    s_jhtml_start_blockquote_tag,
    s_jhtml_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_jhtml_start_dir_tag,
    s_jhtml_end_dir_tag,
  },
  /* tagDL */
  {
    s_jhtml_start_dl_tag,
    s_jhtml_end_dl_tag,
  },
  /* tagDD */
  {
    s_jhtml_start_dd_tag,
    s_jhtml_end_dd_tag,
  },
  /* tagMENU */
  {
    s_jhtml_start_menu_tag,
    s_jhtml_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_jhtml_start_plaintext_tag,
    s_jhtml_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_jhtml_start_blink_tag,
    s_jhtml_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_jhtml_start_marquee_tag,
    s_jhtml_end_marquee_tag,
  },
  /* tagLINK */
  {
    s_jhtml_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_jhtml_newline_mark,
    NULL,
  },
};


/**
 * converts from CHTML5.0 to JHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_jhtml(
  request_rec         *r,
  device_table        *spec,
  const char          *src,
  apr_size_t          srclen,
  apr_size_t          *dstlen,
  chxjconvrule_entry  *entryp,
  cookie_t            *cookie
)
{
  char      *dst;
  char      *ss;
  jhtml_t   jhtml;
  Doc       doc;
  apr_pool_t *pool;

  dst = NULL;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"I found qrcode xml");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_jhtml(&jhtml, &doc, r, spec);

  jhtml.entryp = entryp;
  jhtml.cookie = cookie;

  chxj_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  apr_pool_create(&pool, r->pool);
  ss = apr_pcalloc(pool, srclen + 1);

  memset(ss,   0, srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> JHTML", ss, srclen);
#endif
  if (IS_CSS_ON(jhtml.entryp)) {
    /* current property list */
    jhtml.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }

  qs_parse_string(&doc,ss,strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to JHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void*)&jhtml, &doc, qs_get_root(&doc), 0);
  jhtml.out = chxj_buffered_write_flush(jhtml.out, &doc.buf);
  dst = apr_pstrdup(pool, jhtml.out);
  chxj_buffered_write_terminate(&doc.buf);


  qs_all_free(&doc,QX_LOGMARK);

  if (! dst) 
    return apr_pstrdup(pool,ss);

  if (! strlen(dst)) 
    dst = apr_psprintf(pool, "\n");

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> JHTML", dst, *dstlen);
#endif

  return dst;
}


/**
 * The CHTML structure is initialized.
 *
 * @param jhtml [i/o] The pointer to the JHTML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   JHTML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_jhtml(jhtml_t *jhtml, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(jhtml, 0, sizeof(jhtml_t));

  doc->r      = r;
  jhtml->doc  = doc;
  jhtml->spec = spec;
  jhtml->out  = qs_alloc_zero_byte_string(r->pool);
  jhtml->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  jhtml->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param jhtml   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_jhtml_search_emoji(jhtml_t *jhtml, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = jhtml->spec;

  len = strlen(txt);
  r = jhtml->doc->r;

  if (! spec) DBG(r,"spec is NULL");

  for (ee = jhtml->conf->emoji;
       ee;
       ee = ee->next) {

    unsigned char hex1byte;
    unsigned char hex2byte;

    if (! ee->imode) { 
      DBG(r,"emoji->imode is NULL");
      continue;
    }

    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;

    if (ee->imode->string
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
        return strlen(ee->imode->string);
      }

      return 0;
    }

    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
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
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;


  jhtml  = GET_JHTML(pdoc);
  doc    = jhtml->doc;
  r      = doc->r;
  DBG(r, "start s_jhtml_start_html_tag()");

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html>");

  DBG(r, "end s_jhtml_start_html_tag()");

  return jhtml->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t       *jhtml = GET_JHTML(pdoc);
  Doc           *doc = jhtml->doc;

  W_L("</html>");

  return jhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_meta_tag(void *pdoc, Node *node) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  int          content_type_flag;
  int          refresh_flag;

  jhtml             = GET_JHTML(pdoc);
  doc               = jhtml->doc;
  r                 = doc->r;
  refresh_flag      = 0;
  content_type_flag = 0;

  W_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'h':
    case 'H':
      if (strcasecmp(name, "http-equiv") == 0 && value && *value) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W_L(" http-equiv=\"");
        W_V(value);
        W_L("\"");
        if (STRCASEEQ('c','C',"content-type",value)) {
          content_type_flag = 1;
        }
        if (STRCASEEQ('r','R',"refresh",value)) {
          refresh_flag = 1;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0 && value && *value) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        if (content_type_flag)  {
          W_L(" ");
          W_V(name);
          W_L("=\"");
          W_L("text/html; charset=Windows-31J");
          W_L("\"");
        }
        else
        if (refresh_flag) {
          char *buf;
          char *sec;
          char *url;
  
          buf = apr_pstrdup(r->pool, value);
  
          url = strchr(buf, ';');
          if (url) {
            sec = apr_pstrdup(r->pool, buf);
            sec[url-buf] = 0;
            url++;
            url = chxj_encoding_parameter(r, url);
            url = chxj_add_cookie_parameter(r, url, jhtml->cookie);
            W_L(" ");
            W_V(name);
            W_L("=\"");
            W_V(sec);
            W_L(";");
            W_V(url);
            W_L("\"");
          }
        }
        else {
          W_L(" ");
          W_V(name);
          W_L("=\"");
          W_V(value);
          W_L("\"");
        }
      }
      break;
    
    default:
      break;
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);

  return jhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<head>");
  return jhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</head>");
  return jhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<title>");
  return jhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</title>");
  return jhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_base_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml;
  Attr          *attr;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href",name)) {
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_body_tag(void *pdoc, Node *node) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<body");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B',"bgcolor",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" bgcolor=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('t','T',"text",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" text=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('l','L',"link",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" link=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"alink",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</body>");
  return jhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_a_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W_L(" name=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"href",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cti",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" cti=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('i','I',"ijam",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      W_L(" utn ");
    }
    else if (STRCASEEQ('t','T',"telbook",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('k','K',"kana",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('e','E',"email",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('i','I',"ista",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</a>");
  return jhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_br_tag(void *pdoc, Node *node)
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<br");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"clear",name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('a','A',"all",value))) {
        W_L(" clear=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<br>");
  return jhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_font_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *color = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color",name) && value && *value) {
      color = apr_pstrdup(doc->buf.pool, value);
      break;
    }
    else if (STRCASEEQ('s','S',"size",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  if (color) {
    W_L("<font color=\"");
    W_V(color);
    W_L("\">");
    jhtml->font_flag++;
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_font_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t      *jhtml;
  request_rec  *r;
  Doc          *doc;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = jhtml->doc->r;

  if (jhtml->font_flag) {
    W_L("</font>");
    jhtml->font_flag--;
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_form_tag(void *pdoc, Node *node) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  int          dcflag = 0;
  char         *dc = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<form");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"action",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" action=\"");
      W_V(value);
      W_L("\"");
      dc = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      if (strcmp(dc, value)) {
        dcflag = 1;
      } 
    }
    else if (STRCASEEQ('m','M',"method",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" method=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  W_L(">");
  /*-------------------------------------------------------------------------*/
  /* ``action=""''                                                           */
  /*-------------------------------------------------------------------------*/
  if (! dc) {
    dcflag = 1;
  }
  /*-------------------------------------------------------------------------*/
  /* Add cookie parameter                                                    */
  /*-------------------------------------------------------------------------*/
  if (jhtml->cookie && jhtml->cookie->cookie_id && dcflag == 1) {
    char *vv = apr_psprintf(doc->buf.pool, "<input type='hidden' name='%s' value='%s'>",
                            CHXJ_COOKIE_PARAM,
                            chxj_url_decode(doc->buf.pool, jhtml->cookie->cookie_id));
    W_V(vv);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  W_L("</form>");
  return jhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_input_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  char          *max_length;
  char          *type;
  char          *name;
  char          *value;
  char          *istyle;
  char          *size;
  char          *checked;
  char          *accesskey;

  jhtml       = GET_JHTML(pdoc);
  doc         = jhtml->doc;
  r           = doc->r;
  max_length  = NULL;
  type        = NULL;
  name        = NULL;
  value       = NULL;
  istyle      = NULL;
  size        = NULL;
  checked     = NULL;
  accesskey   = NULL;

  W_L("<input");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  type       = qs_get_type_attr(doc, node, doc->buf.pool);
  name       = qs_get_name_attr(doc, node, doc->buf.pool);
  value      = qs_get_value_attr(doc,node,doc->buf.pool);
  istyle     = qs_get_istyle_attr(doc,node,doc->buf.pool);
  max_length = qs_get_maxlength_attr(doc,node,doc->buf.pool);
  checked    = qs_get_checked_attr(doc,node,doc->buf.pool);
  accesskey  = qs_get_accesskey_attr(doc, node, doc->buf.pool);
  size       = qs_get_size_attr(doc, node, doc->buf.pool);

  if (type) {
    if (type && (STRCASEEQ('t','T',"text",    type) ||
                 STRCASEEQ('p','P',"password",type) ||
                 STRCASEEQ('c','C',"checkbox",type) ||
                 STRCASEEQ('r','R',"radio",   type) ||
                 STRCASEEQ('h','H',"hidden",  type) ||
                 STRCASEEQ('s','S',"submit",  type) ||
                 STRCASEEQ('r','R',"reset",   type))) {
      W_L(" type=\"");
      W_V(type);
      W_L("\"");
    }
  }
  if (size && *size) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }
  if (name && *name) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool, value));
    W_L("\"");
  }
  if (accesskey && *accesskey) {
    W_L(" accesskey=\"");
    W_V(accesskey);
    W_L("\"");
  }
  if (istyle && (*istyle == '1' || *istyle == '2' || *istyle == '3' || *istyle == '4')) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (type && STRCASEEQ('p','P',"password", type) && ! jhtml->entryp->pc_flag ) {
      W_L(" mode=\"");
      W_L("numeric");
      W_L("\"");
    }
    else {
      char *vv = chxj_istyle_to_mode(doc->buf.pool,istyle);
      W_L(" mode=\"");
      W_V(vv);
      W_L("\"");
    }
  }
  else if (type && STRCASEEQ('p','P',"password",type)) {
    W_L(" mode=\"");
    W_L("numeric");
    W_L("\"");
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length && *max_length) {
    if (chxj_chk_numeric(max_length) == 0) {
      W_L(" maxlength=\"");
      W_V(max_length);
      W_L("\"");
    }
  }

  if (checked) {
    W_L(" checked");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_center_tag(void *pdoc, Node *UNUSED(node)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  W_L("<center>");
  return jhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</center>");
  return jhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_li_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_type = NULL;
  char        *attr_value = NULL;
  char        *attr_style = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value && (*value == '1' || *value == 'a' || *value == 'A')) {
      attr_type = value;
    }
    else if (STRCASEEQ('v','V',"value", name) && value && *value) {
      attr_value = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('d','D',"decimal", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "1");
        }
        else if (STRCASEEQ('u','U',"upper-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "A");
        }
        else if (STRCASEEQ('l','L',"lower-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "a");
        }
      }
    }
  }

  W_L("<li");
  if (attr_type) {
    W_L(" type=\"");
    W_V(attr_type);
    W_L("\"");
  }
  if (attr_value) {
    W_L(" value=\"");
    W_V(attr_value);
    W_L("\"");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</li>");
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_ol_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_start = NULL;
  char        *attr_type  = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value && (*value == '1' || *value == 'a' || *value == 'A')) {
      attr_type = value;
    }
    else if (STRCASEEQ('s','S',"start",name) && value && *value) {
      attr_start = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('d','D',"decimal", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "1");
        }
        else if (STRCASEEQ('u','U',"upper-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "A");
        }
        else if (STRCASEEQ('l','L',"lower-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "a");
        }
      }
    }
  }
  W_L("<ol");
  if (attr_type) {
    W_L(" type=\"");
    W_V(attr_type);
    W_L("\"");
  }
  if (attr_start) {
    W_L(" start=\"");
    W_V(attr_start);
    W_L("\"");
  }
  W_L(">");

  return jhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</ol>");
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_p_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align = NULL;
  char        *attr_style = NULL;
  char        *attr_color = NULL;
  char        *attr_blink = NULL;


  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        attr_align = apr_pstrdup(doc->buf.pool, val);
        break;
      }
    }
    else if (STRCASEEQ('s','S',"style", nm) && val && *val) {
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *text_align_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_deco_prop  = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *cur;
      for (cur = text_align_prop->next; cur != text_align_prop; cur = cur->next) {
        if (STRCASEEQ('l','L',"left",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "left");
        }
        else if (STRCASEEQ('c','C',"center",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "center");
        }
        else if (STRCASEEQ('r','R',"right",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "right");
        }
      }
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = text_deco_prop->next; cur != text_deco_prop; cur = cur->next) {
        if (cur->value && *cur->value && STRCASEEQ('b','B',"blink",cur->value)) {
          attr_blink = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<p");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  jhtml_flags_t *flg = (jhtml_flags_t *)apr_palloc(doc->pool, sizeof(jhtml_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  if (attr_blink) {
    W_L("<blink>");
    flg->with_blink_flag = 1;
  }
  node->userData = (void *)flg;

  return jhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_p_tag(void *pdoc, Node *node)
{
  jhtml_t   *jhtml = GET_JHTML(pdoc);
  Doc       *doc   = jhtml->doc;

  jhtml_flags_t *flg = (jhtml_flags_t *)node->userData;
  if (flg->with_font_flag) {
    W_L("</font>");
  }
  if (flg->with_blink_flag) {
    W_L("</blink>");
  }
  W_L("</p>");

  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }

  return jhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_pre_tag(void *pdoc, Node *node)
{
  jhtml_t  *jhtml = GET_JHTML(pdoc);
  Doc      *doc   = jhtml->doc;
  Attr      *attr;
  char      *attr_style = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }

  if (IS_CSS_ON(jhtml->entryp)) {
    s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  jhtml->pre_flag++;
  W_L("<pre>");
  return jhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;

  W_L("</pre>");
  jhtml->pre_flag--;
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }

  return jhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_ul_tag(void *pdoc, Node *node)
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  Attr        *attr;
  char        *attr_style = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }

  if (IS_CSS_ON(jhtml->entryp)) {
    s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  W_L("<ul>");
  return jhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;

  W_L("</ul>");
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_hr_tag(void *pdoc, Node *node) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  Attr    *attr;

  W_L("<hr");
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('s','S',"size",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" size=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('w','W',"width",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" width=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('n','N',"noshade",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" noshade");
    }
    else if (STRCASEEQ('c','C',"color",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" color=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_img_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml = GET_JHTML(pdoc);
  Doc           *doc   = jhtml->doc;
  request_rec   *r     = doc->r;
  Attr          *attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = jhtml->spec;
#endif

  W_L("<img");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"src",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
      W_L(" src=\"");
      W_V(value);
      W_L("\"");
#else
      value = chxj_img_conv(r, spec, value);
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
      W_L(" src=\"");
      W_V(value);
      W_L("\"");
#endif
    }
    else if (STRCASEEQ('a','A',"align",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value) {
        if (STRCASEEQ('t','T',"top",   value) ||
            STRCASEEQ('m','M',"middle",value) ||
            STRCASEEQ('b','B',"bottom",value) ||
            STRCASEEQ('l','L',"left",  value) ||
            STRCASEEQ('r','R',"right", value)) {
          W_L(" align=\"");
          W_V(value);
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" align=\"");
          W_L("middle");
          W_L("\"");
        }
      }
    }
    else if (STRCASEEQ('w','W',"width",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" width=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"height",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" height=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"hspace",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vspace",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" alt=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_select_tag(void *pdoc, Node *child)
{
  jhtml_t *jhtml    = GET_JHTML(pdoc);
  Doc     *doc      = jhtml->doc;
  Attr    *attr;
  char    *size     = NULL;
  char    *name     = NULL;
  char    *multiple = NULL;

  W_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"size",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      multiple = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (size && *size) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }
  if (name && *name) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }
  if (multiple) {
    W_L(" multiple");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;

  W_L("</select>");
  return jhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_option_tag(void *pdoc, Node *child)
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  Attr    *attr;

  char *selected   = NULL;
  char *value      = NULL;

  W_L("<option");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"selected",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  /* Don't close */
  return jhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_div_tag(void *pdoc, Node *child)
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  char         *align = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<div");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        align = apr_pstrdup(doc->buf.pool, val);
      }
    }
  }
  if (align) {
    W_L(" align=\"");
    W_V(align);
    W_L("\"");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</div>");
  return jhtml->out;
}


static char *
chxj_istyle_to_mode(apr_pool_t *p, const char *s)
{
  char *tmp;

  if (s) {
    switch (s[0]) {
    case '1': return apr_psprintf(p, "hiragana");
    case '2': return apr_psprintf(p, "hankakukana");
    case '3': return apr_psprintf(p, "alphabet");
    case '4': return apr_psprintf(p, "numeric");
    default: 
      tmp = apr_palloc(p, 1);
      tmp[0] = '\0';
      return apr_pstrdup(p, tmp);
    }
  }

  tmp = apr_palloc(p, 1);
  tmp[0] = '\0';
  return apr_pstrdup(p,tmp);
}


static char *
s_jhtml_chxjif_tag(void *pdoc, Node *node)
{
  jhtml_t *jhtml;
  Doc     *doc;
  Node    *child;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_jhtml_chxjif_tag(jhtml, child);
  }
  return NULL;
}


/**
 * It is a handler who processes the TEXTARE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_textarea_tag(void *pdoc, Node *node) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_accesskey = NULL;
  char          *attr_name      = NULL;
  char          *attr_rows      = NULL;
  char          *attr_cols      = NULL;
  char          *attr_istyle    = NULL;
  char          *attr_style     = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->textarea_flag++;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"accesskey",name) && value && *value != 0) {
      attr_accesskey = value;
    }
    else if (STRCASEEQ('i','I',"istyle", name) && value && (*value == '1' || *value == '2' || *value == '3' || *value == '4')) {
      attr_istyle = value;
    }
    else if (STRCASEEQ('n','N',"name", name) && value && *value) {
      attr_name = value;
    }
    else if (STRCASEEQ('r','R',"rows", name) && value && *value) {
      attr_rows = value;
    }
    else if (STRCASEEQ('c','C',"cols", name) && value && *value) {
      attr_cols = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *wap_input_format = chxj_css_get_property_value(doc, style, "-wap-input-format");
      css_property_t *cur;
      for (cur = wap_input_format->next; cur != wap_input_format; cur = cur->next) {
        if (strcasestr(cur->value, "<ja:n>")) {
          attr_istyle = "4";
        }
        else if (strcasestr(cur->value, "<ja:en>")) {
          attr_istyle = "3";
        }
        else if (strcasestr(cur->value, "<ja:hk>")) {
          attr_istyle = "2";
        }
        else if (strcasestr(cur->value, "<ja:h>")) {
          attr_istyle = "1";
        }
      }
    }
  }
  W_L("<textarea");
  if (attr_accesskey) {
    W_L(" accesskey=\"");
    W_V(attr_accesskey);
    W_L("\"");
  }
  if (attr_name) {
    W_L(" name=\"");
    W_V(attr_name);
    W_L("\"");
  }
  if (attr_rows) {
    W_L(" rows=\"");
    W_V(attr_rows);
    W_L("\"");
  }
  if (attr_cols) {
    W_L(" cols=\"");
    W_V(attr_cols);
    W_L("\"");
  }
  if (attr_istyle) {
    char *vv = chxj_istyle_to_mode(doc->buf.pool,attr_istyle);
    W_L(" mode=\"");
    W_V(vv);
    W_L("\"");
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the TEXTAREA tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("</textarea>");
  jhtml->textarea_flag--;

  return jhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_start_b_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  W_L("<b>");
  return jhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_end_b_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

  W_L("</b>");
  return jhtml->out;
}

static char*
s_jhtml_text_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml;
  Doc*         doc;
  char*        textval;
  char*        tmp;
  char*        tdst;
  char         one_byte[2];
  int          ii;
  int          tdst_len;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;
  DBG(r, "start s_jhtml_text_tag()");

  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return jhtml->out;
  }

  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);

  tdst     = qs_alloc_zero_byte_string(r->pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;

  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_jhtml_search_emoji(jhtml, &textval[ii], &out);
    if (rtn) {
      tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
      ii+=(rtn - 1);
      continue;
    }

    if (is_sjis_kanji(textval[ii])) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      one_byte[0] = textval[ii+1];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      ii++;
    }
    else 
    if (jhtml->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else
    if (jhtml->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else {
      if (textval[ii] != '\r' && textval[ii] != '\n') {
        one_byte[0] = textval[ii+0];
        tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      }
    }
  }
  W_V(tdst);
  DBG(r, "end s_jhtml_text_tag()");
  return jhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  W_L("<blockquote>");
  return jhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  W_L("</blockquote>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_dir_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("<dir>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("</dir>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_dl_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("<dl>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("</dl>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_dt_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc   = jhtml->doc;
  W_L("<dt>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_dd_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("<dd>");
  return jhtml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h1_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        attr_align = value;
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('l','L',"left", cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "left");
        }
        else if (STRCASEEQ('c','C',"center",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "center");
        }
        else if (STRCASEEQ('r','R',"right",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "right");
        }
      }
    }
  }
  if (attr_align) {
    W_L("<div align=\"");
    W_V(attr_align);
    W_L("\">");
    jhtml_flags_t *flg = apr_palloc(doc->pool, sizeof(*flg));
    memset(flg, 0, sizeof(*flg));
    flg->with_div_align_flag = 1;
    node->userData = (void *)flg;
  }
  else {
    node->userData = NULL;
  }

  return jhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h1_tag(void *pdoc, Node *node)
{
  jhtml_t*    jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;
  
  jhtml_flags_t *flg = node->userData;
  if (flg && flg->with_div_align_flag) {
    W_L("</div>");
    node->userData = NULL; 
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h2_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        attr_align = value;
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *style = s_jhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('l','L',"left", cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "left");
        }
        else if (STRCASEEQ('c','C',"center",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "center");
        }
        else if (STRCASEEQ('r','R',"right",cur->value)) {
          attr_align = apr_pstrdup(doc->pool, "right");
        }
      }
    }
  }
  if (attr_align) {
    W_L("<div align=\"");
    W_V(attr_align);
    W_L("\">");
    jhtml_flags_t *flg = apr_palloc(doc->pool, sizeof(*flg));
    memset(flg, 0, sizeof(*flg));
    flg->with_div_align_flag = 1;
    node->userData = (void *)flg;
  }
  else {
    node->userData = NULL;
  }

  return jhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h2_tag(void *pdoc, Node *node)
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  jhtml_flags_t *flg = node->userData;
  if (flg && flg->with_div_align_flag) {
    W_L("</div>");
    node->userData = NULL;
  }
  if (IS_CSS_ON(jhtml->entryp)) {
    chxj_css_pop_prop_list(jhtml->css_prop_stack);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h3_tag(void *pdoc, Node *node)
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        jhtml->h3_align_flag++;
        align = apr_pstrdup(doc->buf.pool, value);
        break;
      }
    }
  }
  if (align) {
    W_L("<div align=\"");
    W_V(align);
    W_L("\">");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t*    jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;
  
  if (jhtml->h3_align_flag) {
    jhtml->h3_align_flag--;
    W_L("</div>");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h4_tag(void *pdoc, Node *node)
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        jhtml->h4_align_flag++;
        align = apr_pstrdup(doc->buf.pool, value);
        break;
      }
    }
  }
  if (align) {
    W_L("<div align=\"");
    W_V(align);
    W_L("\">");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t      *jhtml;
  Doc          *doc;
  request_rec  *r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;
  
  if (jhtml->h4_align_flag) {
    jhtml->h4_align_flag--;
    W_L("</div>");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h5_tag(void *pdoc, Node *node)
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        jhtml->h5_align_flag++;
        align = apr_pstrdup(doc->buf.pool, value);
        break;
      }
    }
  }
  if (align) {
    W_L("<div align=\"");
    W_V(align);
    W_L("\">");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t*    jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;
  
  if (jhtml->h5_align_flag) {
    jhtml->h5_align_flag--;
    W_L("</div>");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_h6_tag(void *pdoc, Node *node)
{
  jhtml_t       *jhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *align = NULL;

  jhtml   = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        jhtml->h6_align_flag++;
        align = apr_pstrdup(doc->buf.pool, value);
        break;
      }
    }
  }
  if (align) {
    W_L("<div align=\"");
    W_V(align);
    W_L("\">");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  jhtml_t     *jhtml;
  Doc         *doc;
  request_rec *r;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  r       = doc->r;
  
  if (jhtml->h6_align_flag) {
    jhtml->h6_align_flag--;
    W_L("</div>");
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_menu_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc = jhtml->doc;
  W_L("<menu>");
  return jhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc = jhtml->doc;
  W_L("</menu>");
  return jhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_plaintext_tag(void *pdoc, Node *node)
{
  jhtml_t *jhtml;
  Doc *doc;

  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  W_L("<plaintext>");
  s_jhtml_start_plaintext_tag_inner(pdoc,node);
  return jhtml->out;
}

static char *
s_jhtml_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  jhtml_t *jhtml;
  Doc *doc;
  Node *child;
  jhtml = GET_JHTML(pdoc);
  doc     = jhtml->doc;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_jhtml_start_plaintext_tag_inner(pdoc, child);
  }
  return jhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  return jhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_blink_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc = jhtml->doc;
  W_L("<blink>");
  return jhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_blink_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc     *doc = jhtml->doc;
  W_L("</blink>");
  return jhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_start_marquee_tag(void *pdoc, Node *node)
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  Attr *attr;
  W_L("<marquee");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('d','D',"direction", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value))) {
        W_L(" direction=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('b','B',"behavior",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('l','L',"loop",name)) {
      /* ignore */
    }
  }
  W_L(">");
  return jhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_end_marquee_tag(void *pdoc, Node *UNUSED(child))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_L("</marquee>");
  return jhtml->out;
}


/**
 * It is handler who processes the New Line Code.
 */
static char *
s_jhtml_newline_mark(void *pdoc, Node *UNUSED(node))
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  W_NLCODE();
  return jhtml->out;
}


/**
 * It is a handler who processes the LINK tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jhtml_link_tag(void *pdoc, Node *node)
{
  jhtml_t       *jhtml;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;

  if (! IS_CSS_ON(jhtml->entryp)) {
    return jhtml->out;
  }

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('r','R',"rel", name)) {
      if (value && *value && STRCASEEQ('s','S',"stylesheet", value)) {
        rel = value;
      }
    }
    else if (STRCASEEQ('h','H',"href", name)) {
      if (value && *value) {
        href = value;
      }
    }
    else if (STRCASEEQ('t','T',"type", name)) {
      if (value && *value && STRCASEEQ('t','T',"text/css",value)) {
        type = value;
      }
    }
  }

  if (rel && href && type) {
    DBG(doc->r, "start load CSS. url:[%s]", href);
    jhtml->style = chxj_css_parse_from_uri(doc->r, doc->pool, jhtml->style, href);
    DBG(doc->r, "end load CSS. url:[%s]", href);
  }

  return jhtml->out;
}


static css_prop_list_t *
s_jhtml_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(jhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, jhtml->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(jhtml->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(jhtml->css_prop_stack);

    if (style_attr_value) {
      css_stylesheet_t *ssheet = chxj_css_parse_style_attr(doc, NULL, apr_pstrdup(doc->pool, node->name), NULL, NULL, apr_pstrdup(doc->pool, style_attr_value));
      if (ssheet) {
        chxj_css_prop_list_merge_property(doc, last_css, ssheet->selector_head.next);
      }
    }
  }
  return last_css;
}


static css_prop_list_t *
s_jhtml_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  jhtml_t *jhtml = GET_JHTML(pdoc);
  Doc *doc = jhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(jhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(jhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, jhtml->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    last_css = dup_css;

    if (style_attr_value) {
      css_stylesheet_t *ssheet = chxj_css_parse_style_attr(doc, NULL, apr_pstrdup(doc->pool, node->name), NULL, NULL, apr_pstrdup(doc->pool, style_attr_value));
      if (ssheet) {
        chxj_css_prop_list_merge_property(doc, last_css, ssheet->selector_head.next);
      }
    }
  }
  return last_css;
}
/*
 * vim:ts=2 et
 */
