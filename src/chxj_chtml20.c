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
#include "chxj_chtml20.h"
#include "chxj_hdml.h"
#include "chxj_str_util.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_buffered_write.h"
#include "chxj_css.h"
#include "chxj_header_inf.h"
#include "chxj_conv_z2h.h"
#include "chxj_google.h"


#define GET_CHTML20(X) ((chtml20_t*)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { chtml20->out = BUFFERED_WRITE_LITERAL(chtml20->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { chtml20->out = (X) ? BUFFERED_WRITE_VALUE(chtml20->out, &doc->buf, (X))  \
                                                  : BUFFERED_WRITE_LITERAL(chtml20->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(chtml20->conf); W_V(nlcode); } while (0)

#define CHTML20_START_OF_TAG_WITH_CSS_FONT_COLOR(tagname) \
  do { \
    chtml20_t *chtml20; \
    Doc       *doc; \
    Attr      *attr; \
    char      *attr_style = NULL; \
    char      *attr_color = NULL; \
   \
    chtml20 = GET_CHTML20(pdoc); \
    doc     = chtml20->doc; \
    for (attr = qs_get_attr(doc,node); \
         attr; \
         attr = qs_get_next_attr(doc,attr)) { \
      char *nm  = qs_get_attr_name(doc,attr); \
      char *val = qs_get_attr_value(doc,attr); \
      if (val && STRCASEEQ('s','S',"style", nm)) { \
        attr_style = val; \
      } \
    } \
    if (IS_CSS_ON(chtml20->entryp)) { \
      css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style); \
      if (style) { \
        css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color"); \
        css_property_t *cur; \
        for (cur = color_prop->next; cur != color_prop; cur = cur->next) { \
          if (cur->value && *cur->value) { \
            attr_color = apr_pstrdup(doc->pool, cur->value); \
          } \
        } \
      } \
    } \
    W_L(tagname); \
    chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t)); \
    memset(flg, 0, sizeof(*flg)); \
    if (attr_color) { \
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color); \
      W_L("<font color=\""); \
      W_V(attr_color); \
      W_L("\">"); \
      flg->with_font_flag = 1; \
    } \
    node->userData = (void *)flg; \
    return chtml20->out; \
  } while(0) \

#define CHTML20_END_OF_NO_CLOSE_TAG_WITH_FONT_TAG() \
  do {                                                        \
    chtml20_t *chtml20 = GET_CHTML20(pdoc);                   \
    Doc       *doc     = chtml20->doc;                        \
    chtml20_flags_t *flg = (chtml20_flags_t *)node->userData; \
    if (flg && flg->with_font_flag) {                         \
      W_L("</font>");                                         \
    }                                                         \
    if (IS_CSS_ON(chtml20->entryp)) {                         \
      chxj_css_pop_prop_list(chtml20->css_prop_stack);        \
    }                                                         \
    return chtml20->out;                                      \
  } while(0)

#define CHTML20_END_OF_CLOSE_TAG_WITH_FONT_TAG(tagname) \
  do {                                                        \
    chtml20_t *chtml20 = GET_CHTML20(pdoc);                   \
    Doc       *doc     = chtml20->doc;                        \
    chtml20_flags_t *flg = (chtml20_flags_t *)node->userData; \
    if (flg && flg->with_font_flag) {                         \
      W_L("</font>");                                         \
    }                                                         \
    W_L(tagname);                                             \
    if (IS_CSS_ON(chtml20->entryp)) {                         \
      chxj_css_pop_prop_list(chtml20->css_prop_stack);        \
    }                                                         \
    return chtml20->out;                                      \
  } while(0)

static char *s_chtml20_start_html_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_html_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_meta_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_meta_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_textarea_tag(void *pdoc, Node *node);
static char *s_chtml20_end_textarea_tag  (void *pdoc, Node *node);
static char *s_chtml20_start_p_tag       (void *pdoc, Node *node);
static char *s_chtml20_end_p_tag         (void *pdoc, Node *node);
static char *s_chtml20_start_pre_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_pre_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_h1_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h1_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h2_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h2_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h3_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h3_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h4_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h4_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h5_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h5_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h6_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h6_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_ul_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_ul_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_ol_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_ol_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_li_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_li_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_head_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_head_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_title_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_title_tag     (void *pdoc, Node *node);
static char *s_chtml20_start_base_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_base_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_body_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_body_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_a_tag       (void *pdoc, Node *node);
static char *s_chtml20_end_a_tag         (void *pdoc, Node *node);
static char *s_chtml20_start_br_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_br_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_tr_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_tr_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_font_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_font_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_form_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_form_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_input_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_input_tag     (void *pdoc, Node *node);
static char *s_chtml20_start_center_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_center_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_hr_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_hr_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_img_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_img_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_select_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_select_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_option_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_option_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_div_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_div_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_blockquote_tag(void *pdoc, Node *node);
static char *s_chtml20_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_chtml20_start_dir_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_dir_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_dl_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_dl_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_dt_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_dt_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_dd_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_dd_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_menu_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_menu_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_chtml20_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_chtml20_start_blink_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_blink_tag     (void *pdoc, Node *node);
static char *s_chtml20_start_marquee_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_marquee_tag     (void *pdoc, Node *node);
static char *s_chtml20_newline_mark       (void *pdoc, Node *node);
static char *s_chtml20_style_tag          (void *pdoc, Node *node);
static char *s_chtml20_link_tag           (void *pdoc, Node *node);
static char *s_chtml20_start_span_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_span_tag      (void *pdoc, Node *node);

static void  s_init_chtml20(chtml20_t *chtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_chtml20_search_emoji(chtml20_t *chtml, char *txt, char **rslt, Node *node);

static char *s_chtml20_chxjif_tag(void *pdoc, Node *node); 
static char *s_chtml20_text_tag(void *pdoc, Node *node);
static css_prop_list_t *s_chtml20_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_chtml20_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
/* pend */


tag_handler chtml20_handler[] = {
  /* tagHTML */
  {
    s_chtml20_start_html_tag,
    s_chtml20_end_html_tag,
  },
  /* tagMETA */
  {
    s_chtml20_start_meta_tag,
    s_chtml20_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_chtml20_start_textarea_tag,
    s_chtml20_end_textarea_tag,
  },
  /* tagP */
  {
    s_chtml20_start_p_tag,
    s_chtml20_end_p_tag,
  },
  /* tagPRE */
  {
    s_chtml20_start_pre_tag,
    s_chtml20_end_pre_tag,
  },
  /* tagUL */
  {
    s_chtml20_start_ul_tag,
    s_chtml20_end_ul_tag,
  },
  /* tagLI */
  {
    s_chtml20_start_li_tag,
    s_chtml20_end_li_tag,
  },
  /* tagOL */
  {
    s_chtml20_start_ol_tag,
    s_chtml20_end_ol_tag,
  },
  /* tagH1 */
  {
    s_chtml20_start_h1_tag,
    s_chtml20_end_h1_tag,
  },
  /* tagH2 */
  {
    s_chtml20_start_h2_tag,
    s_chtml20_end_h2_tag,
  },
  /* tagH3 */
  {
    s_chtml20_start_h3_tag,
    s_chtml20_end_h3_tag,
  },
  /* tagH4 */
  {
    s_chtml20_start_h4_tag,
    s_chtml20_end_h4_tag,
  },
  /* tagH5 */
  {
    s_chtml20_start_h5_tag,
    s_chtml20_end_h5_tag,
  },
  /* tagH6 */
  {
    s_chtml20_start_h6_tag,
    s_chtml20_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_chtml20_start_head_tag,
    s_chtml20_end_head_tag,
  },
  /* tagTITLE */
  {
    s_chtml20_start_title_tag,
    s_chtml20_end_title_tag,
  },
  /* tagBASE */
  {
    s_chtml20_start_base_tag,
    s_chtml20_end_base_tag,
  },
  /* tagBODY */
  {
    s_chtml20_start_body_tag,
    s_chtml20_end_body_tag,
  },
  /* tagA */
  {
    s_chtml20_start_a_tag,
    s_chtml20_end_a_tag,
  },
  /* tagBR */
  {
    s_chtml20_start_br_tag,
    s_chtml20_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_chtml20_start_tr_tag,
    s_chtml20_end_tr_tag,
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
    s_chtml20_start_font_tag,
    s_chtml20_end_font_tag,
  },
  /* tagFORM */
  {
    s_chtml20_start_form_tag,
    s_chtml20_end_form_tag,
  },
  /* tagINPUT */
  {
    s_chtml20_start_input_tag,
    s_chtml20_end_input_tag,
  },
  /* tagCENTER */
  {
    s_chtml20_start_center_tag,
    s_chtml20_end_center_tag,
  },
  /* tagHR */
  {
    s_chtml20_start_hr_tag,
    s_chtml20_end_hr_tag,
  },
  /* tagIMG */
  {
    s_chtml20_start_img_tag,
    s_chtml20_end_img_tag,
  },
  /* tagSELECT */
  {
    s_chtml20_start_select_tag,
    s_chtml20_end_select_tag,
  },
  /* tagOPTION */
  {
    s_chtml20_start_option_tag,
    s_chtml20_end_option_tag,
  },
  /* tagDIV */
  {
    s_chtml20_start_div_tag,
    s_chtml20_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_chtml20_chxjif_tag,
    NULL,
  },
  /* tagCHXJRAW */
  {
    s_chtml20_chxjif_tag,
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
    s_chtml20_style_tag,
    NULL,
  },
  /* tagSPAN */
  {
    s_chtml20_start_span_tag,
    s_chtml20_end_span_tag,
  },
  /* tagTEXT */
  {
    s_chtml20_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    NULL,
    NULL,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_chtml20_start_dt_tag,
    s_chtml20_end_dt_tag,
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
    s_chtml20_start_blockquote_tag,
    s_chtml20_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_chtml20_start_dir_tag,
    s_chtml20_end_dir_tag,
  },
  /* tagDL */
  {
    s_chtml20_start_dl_tag,
    s_chtml20_end_dl_tag,
  },
  /* tagDD */
  {
    s_chtml20_start_dd_tag,
    s_chtml20_end_dd_tag,
  },
  /* tagMENU */
  {
    s_chtml20_start_menu_tag,
    s_chtml20_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_chtml20_start_plaintext_tag,
    s_chtml20_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_chtml20_start_blink_tag,
    s_chtml20_end_blink_tag,
  },
  /* tagMAQUEE */
  {
    s_chtml20_start_marquee_tag,
    s_chtml20_end_marquee_tag,
  },
  /* tagLINK */
  {
    s_chtml20_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_chtml20_newline_mark,
    NULL,
  },
  /* tagObject */
  {
    NULL,
    NULL,
  },
  /* tagParam */
  {
    NULL,
    NULL,
  },
  /* tagCAPTION */
  {
    NULL,
    NULL,
  },
};

/**
 * converts from CHTML5.0 to CHTML2.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_chtml20(
  request_rec        *r,
  device_table       *spec,
  const char         *src,
  apr_size_t         srclen,
  apr_size_t         *dstlen,
  chxjconvrule_entry *entryp,
  cookie_t           *cookie
)
{
  char      *dst = NULL;
  char      *ss;
  chtml20_t chtml20;
  Doc       doc;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"REQ[%X] found qrcode xml",TO_ADDR(r));
    return dst;
  }

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml20(&chtml20, &doc, r, spec);

  chtml20.entryp = entryp;
  chtml20.cookie = cookie;

  chxj_set_content_type(r, chxj_header_inf_set_content_type(r, "text/html; charset=Windows-31J"));

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0, srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML2.0", ss, srclen);
#endif

  if (IS_CSS_ON(chtml20.entryp)) {
    /* current property list */
    chtml20.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }

  qs_parse_string(&doc,ss, strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void *)&chtml20, &doc, qs_get_root(&doc), 0);
  chtml20.out = chxj_buffered_write_flush(chtml20.out, &doc.buf);
  dst = apr_pstrdup(r->pool, chtml20.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL)  {
    return apr_pstrdup(r->pool,ss);
  }

  if (strlen(dst) == 0) {
    dst = apr_psprintf(r->pool, "\n");
  }

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> CHTML2.0", dst, *dstlen);
#endif

  return dst;
}

/**
 * The CHTML structure is initialized.
 *
 * @param chtml20 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_chtml20(chtml20_t *chtml20, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml20, 0, sizeof(chtml20_t));

  doc->r        = r;
  chtml20->doc  = doc;
  chtml20->spec = spec;
  chtml20->out  = qs_alloc_zero_byte_string(r->pool);
  chtml20->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  chtml20->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml20   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @param node    [i]   The current node to check whether tag is span/font for CHXJ_IMODE_EMOJI_COLOR_AUTO.
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_chtml20_search_emoji(chtml20_t *chtml20, char *txt, char **rslt, Node *node)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = chtml20->spec;

  len = strlen(txt);
  r   = chtml20->doc->r;

  if (spec == NULL)
    DBG(r,"REQ[%X] spec is NULL",TO_ADDR(r));

  for (ee = chtml20->conf->emoji;
       ee;
       ee = ee->next) {

    if (ee->imode == NULL) {
      DBG(r,"REQ[%X] emoji->imode is NULL",TO_ADDR(r));
      continue;
    }

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_palloc(r->pool, 3);
        (*rslt)[0] = ee->imode->hex1byte & 0xff;
        (*rslt)[1] = ee->imode->hex2byte & 0xff;
        (*rslt)[2] = 0;
        
        if(chtml20->conf->imode_emoji_color >= CHXJ_IMODE_EMOJI_COLOR_AUTO ){
          if(ee->imode->color != NULL){
            if(chtml20->conf->imode_emoji_color == CHXJ_IMODE_EMOJI_COLOR_AUTO && node != NULL ){
              if(strcasecmp(node->parent->name, "span") == 0 ||
                 strcasecmp(node->parent->name, "font")  == 0 ){
                return strlen(ee->imode->string);
              }
            }
            char *tmp = apr_pstrdup(r->pool,*rslt);
            *rslt = apr_psprintf(r->pool,
                        "<font color=\"%s\">%s</font>",ee->imode->color,tmp);
          }
        }
        return strlen(ee->imode->string);
      }

      return 0;
    }
  }

  return 0;
}

char *
chxj_chtml20_emoji_only_converter(request_rec *r, device_table *spec, const char *src, apr_size_t len)
{
  apr_size_t ii;
  Doc __doc;
  Doc *doc;
  chtml20_t __chtml20;
  chtml20_t *chtml20;
  char one_byte[2];
  char two_byte[3];
  apr_pool_t *pool;

  chtml20 = &__chtml20;
  doc     = &__doc;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  memset(doc,     0, sizeof(Doc));
  memset(chtml20, 0, sizeof(chtml20_t));

  doc->r        = r;
  chtml20->doc  = doc;
  chtml20->spec = spec;
  chtml20->out  = qs_alloc_zero_byte_string(r->pool);
  chtml20->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  chtml20->doc->parse_mode = PARSE_MODE_CHTML;

  apr_pool_create(&pool, r->pool);

  chxj_buffered_write_init(pool, &doc->buf);

  for (ii=0; ii<len; ii++) {
    char *out;
    int   rtn;

    rtn = s_chtml20_search_emoji(chtml20, (char *)&src[ii], &out, NULL);
    if (rtn) {
      W_V(out);
      ii+=(rtn - 1);
      continue;
    }

    if (is_sjis_kanji(src[ii])) {
      two_byte[0] = src[ii+0];
      two_byte[1] = src[ii+1];
      two_byte[2] = 0;
      W_V(two_byte);
      ii++;
    }
    else {
      one_byte[0] = src[ii+0];
      one_byte[1] = 0;
      W_V(one_byte);
    }
  }
  chtml20->out = chxj_buffered_write_flush(chtml20->out, &doc->buf);

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return chtml20->out;
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
s_chtml20_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  Doc         *doc;
  request_rec *r;
  chtml20_t   *chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html>");
  return chtml20->out;
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
s_chtml20_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc *doc;
  request_rec *r;
  chtml20_t *chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("</html>");
  return chtml20->out;
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
s_chtml20_start_meta_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  int         content_type_flag;
  int         refresh_flag;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  content_type_flag = 0;
  refresh_flag      = 0;

  W_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
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
        if (STRCASEEQ('c','C',"content-type", value)) {
          content_type_flag = 1;
        }
        if (STRCASEEQ('r','R',"refresh", value)) {
          refresh_flag = 1;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0 && value && *value) {
        if (content_type_flag) {
          W_L(" ");
          W_V(name);
          W_L("=\"");
          W_V(chxj_header_inf_set_content_type(r, "text/html; charset=SHIFT_JIS"));
          W_L("\"");
        }
        else if (refresh_flag) {
          char *buf = apr_pstrdup(r->pool, value);
          char *sec;
          char *url;

          url = strchr(buf, ';');
          if (url) {
            sec = apr_pstrdup(r->pool, buf);
            sec[url-buf] = 0;
            url++;
            url = chxj_encoding_parameter(r, url, 0);
            url = chxj_add_cookie_parameter(r, url, chtml20->cookie);
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

  return chtml20->out;
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
s_chtml20_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);

  return chtml20->out;
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
s_chtml20_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  W_L("<head>");

  return chtml20->out;
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
s_chtml20_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("</head>");

  return chtml20->out;
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
s_chtml20_start_title_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("<title>");

  if (chtml20->conf->use_google_analytics) {
    chtml20->pagetitle = "";
    Node         *child;
    for (child = qs_get_child_node(doc,node);
         child;
         child = qs_get_next_node(doc,child)) {
      char *textval = qs_get_node_value(doc,child);
      chtml20->pagetitle = apr_pstrcat(doc->r->pool, chtml20->pagetitle, textval, NULL);
    }
  }

  return chtml20->out;
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
s_chtml20_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("</title>");

  return chtml20->out;
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
s_chtml20_start_base_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href", name)) {
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");

  return chtml20->out;
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
s_chtml20_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_body_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_bgcolor = NULL;
  char        *attr_text    = NULL;
  char        *attr_link    = NULL;
  char        *attr_style   = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'b':
    case 'B':
      if (strcasecmp(name, "bgcolor") == 0 && value && *value != 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        attr_bgcolor = value;
      }
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "text") == 0 && value && *value != 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        attr_text = value;
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "style") == 0 && value && *value != 0) {
        attr_style = value;
      }
      break;

    case 'l':
    case 'L':
      if (strcasecmp(name, "link") == 0 && value && *value != 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        attr_link = value;
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "alink") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 4.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(name, "vlink") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 4.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *bgcolor_prop    = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_text = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
    if (chtml20->style) {
      css_stylesheet_t *pseudos = chxj_find_pseudo_selectors(doc, chtml20->style);
      css_selector_t *cur_sel;
      for (cur_sel = pseudos->selector_head.next; cur_sel != &pseudos->selector_head; cur_sel = cur_sel->next) {
        if (cur_sel->name && strcasecmp(cur_sel->name, "a:link") == 0) {
          css_property_t *cur;
          for (cur = cur_sel->property_head.next; cur != &cur_sel->property_head; cur = cur->next) {
            if (cur->name && strcasecmp(cur->name, "color") == 0) {
              attr_link = apr_pstrdup(doc->pool, cur->value);
            }
          }
        }
      }
    }
  }
  W_L("<body");
  if (attr_bgcolor) {
    attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
    W_L(" bgcolor=\"");
    W_V(attr_bgcolor);
    W_L("\"");
  }
  if (attr_text) {
    attr_text = chxj_css_rgb_func_to_value(doc->pool, attr_text);
    W_L(" text=\"");
    W_V(attr_text);
    W_L("\"");
  }
  if (attr_link) {
    attr_link = chxj_css_rgb_func_to_value(doc->pool, attr_link);
    W_L(" link=\"");
    W_V(attr_link);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
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
s_chtml20_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  if (chtml20->conf->use_google_analytics) {
    char *src = chxj_google_analytics_get_image_url(r, chtml20->pagetitle);
    W_L("<img src=\"");
    W_V(src);
    W_L("\" />");
  }

  W_L("</body>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
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
s_chtml20_start_a_tag(void *pdoc, Node *node) 
{
  Attr        *attr;
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        W_L(" name=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "href") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        value = chxj_encoding_parameter(r, value, 0);
        if (! chxj_starts_with(value, "mailto:") && ! chxj_starts_with(value, "tel:")) {
          value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        }
        W_L(" href=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "accesskey") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        W_L(" accesskey=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cti") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 2.0                                                          */
        /*--------------------------------------------------------------------*/
        W_L(" cti=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'i':
    case 'I':
      if (strcasecmp(name, "ijam") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "ista") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "ilet") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "iswf") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "irst") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "utn") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "telbook") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'k':
    case 'K':
      if (strcasecmp(name, "kana") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "email") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "style") == 0 && value && *value) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }
  W_L(">");

  if (IS_CSS_ON(chtml20->entryp)) {
    s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
  }

  return chtml20->out;
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
s_chtml20_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("</a>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
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
s_chtml20_start_br_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

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

  return chtml20->out;
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
s_chtml20_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("<br>");

  return chtml20->out;
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
s_chtml20_start_font_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_color = NULL;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0 && value && *value) {
        attr_color = apr_pstrdup(doc->buf.pool, value);
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else if (strcasecmp(name, "style") == 0 && value && *value) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    chtml20_flags_t *flg = apr_palloc(doc->pool, sizeof(*flg));
    memset(flg, 0, sizeof(*flg));
    flg->with_font_flag = 1;
    node->userData = flg;
  }
  else {
    node->userData = NULL;
  }
  return chtml20->out;
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
s_chtml20_end_font_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
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
s_chtml20_start_form_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_action = NULL;
  char        *attr_method = NULL;
  char        *attr_style  = NULL;
  char        *attr_color  = NULL;
  char        *attr_align  = NULL;
  char        *new_hidden_tag = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name;
    char *value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "action") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_action = value;
      }
      break;

    case 'm':
    case 'M':
      if (strcasecmp(name, "method") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_method = value;
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "utn") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "style") == 0) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *text_align_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = text_align_prop->next; cur != text_align_prop; cur = cur->next) {
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
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        attr_color = apr_pstrdup(doc->pool, cur->value);
      }
    }
  }

  int post_flag = (attr_method && strcasecmp(attr_method, "post") == 0) ? 1 : 0;

  W_L("<form");
  if (attr_action) {
    attr_action = chxj_encoding_parameter(r, attr_action, 0);
    attr_action = chxj_add_cookie_parameter(r, attr_action, chtml20->cookie);
    char *q;
    char *new_query_string = NULL;
    q = strchr(attr_action, '?');
    if (q) {
      new_hidden_tag = chxj_form_action_to_hidden_tag(r, doc->pool, attr_action, 0, post_flag, &new_query_string, CHXJ_TRUE, CHXJ_FALSE, chtml20->entryp);
      if (new_hidden_tag || new_query_string) {
        *q = 0;
      }
    }
    W_L(" action=\"");
    W_V(attr_action);
    if (new_query_string) {
      W_L("?");
      W_V(new_query_string);
    }
    W_L("\"");
  }
  if (attr_method) {
    W_L(" method=\"");
    W_V(attr_method);
    W_L("\"");
  }
  W_L(">");
  if (new_hidden_tag) {
    W_V(new_hidden_tag);
  }

  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  if (attr_align) {
    W_L("<div align=\"");
    W_V(attr_align);
    W_L("\">");
    flg->with_div_flag = 1;
  }
  node->userData = flg;

  return chtml20->out;
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
s_chtml20_end_form_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</form>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
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
s_chtml20_start_input_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_accesskey  = NULL;
  char        *attr_max_length = NULL;
  char        *attr_type       = NULL;
  char        *attr_name       = NULL;
  char        *attr_value      = NULL;
  char        *attr_istyle     = NULL;
  char        *attr_size       = NULL;
  char        *attr_checked    = NULL;
  char        *attr_style      = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value && *value) {
      char *tmp_type = qs_trim_string(doc->buf.pool, value);
      if (tmp_type && (STRCASEEQ('t','T',"text",    tmp_type) ||
                       STRCASEEQ('p','P',"password",tmp_type) ||
                       STRCASEEQ('c','C',"checkbox",tmp_type) ||
                       STRCASEEQ('r','R',"radio",   tmp_type) ||
                       STRCASEEQ('h','H',"hidden",  tmp_type) ||
                       STRCASEEQ('s','S',"submit",  tmp_type) ||
                       STRCASEEQ('r','R',"reset",   tmp_type))) {
        attr_type = tmp_type;
      }
    }
    else if (STRCASEEQ('n','N',"name",name) && value && *value) {
      attr_name = value;
    }
    else if (STRCASEEQ('v','V',"value",name) && value && *value) {
      attr_value = value;
    }
    else if (STRCASEEQ('i','I',"istyle",name) && value && *value) {
      attr_istyle = value;
    }
    else if (STRCASEEQ('m','M',"maxlength",name) && value && *value) {
      attr_max_length = value;
    }
    else if (STRCASEEQ('c','C',"checked", name)) {
      attr_checked = value;
    }
    else if (STRCASEEQ('a','A',"accesskey", name) && value && *value) {
      attr_accesskey = value;
    }
    else if (STRCASEEQ('s','S',"size", name) && value && *value) {
      attr_size = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_nopush_and_get_now_style(pdoc, node, attr_style);
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

  W_L("<input");
  if (attr_type) {
    W_L(" type=\"");
    W_V(attr_type);
    W_L("\"");
  }
  if (attr_size) {
    W_L(" size=\"");
    W_V(attr_size);
    W_L("\"");
  }
  if (attr_name) {
    W_L(" name=\"");
    W_V(attr_name);
    W_L("\"");
  }
  if (attr_value) {
    if (attr_type && (STRCASEEQ('s','S',"submit",attr_type) || STRCASEEQ('r','R',"reset",attr_type))) {
      apr_size_t value_len = strlen(attr_value);
      attr_value = chxj_conv_z2h(r, attr_value, &value_len, chtml20->entryp);
    }

    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool, attr_value));
    W_L("\"");
  }
  if (attr_accesskey) {
    W_L(" accesskey=\"");
    W_V(attr_accesskey);
    W_L("\"");
  }
  if (attr_istyle) {
    if (*attr_istyle == '1' || *attr_istyle == '2' || *attr_istyle == '3' || *attr_istyle == '4') {
      W_L(" istyle=\"");
      W_V(attr_istyle);
      W_L("\"");
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (attr_max_length) {
    if (chxj_chk_numeric(attr_max_length) != 0) {
      attr_max_length = apr_psprintf(doc->buf.pool, "0");
    }
    if (attr_istyle && *attr_istyle == '1') {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(attr_max_length) * 2);
      W_V(vv);
    }
    else {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(attr_max_length));
      W_V(vv);
    }
  }
  if (attr_checked) {
    W_L(" checked");
  }
  W_L(">");

  return chtml20->out;
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
s_chtml20_end_input_tag(void *pdoc, Node *UNUSED(node))
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_center_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }

  W_L("<center>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = flg;

  return chtml20->out;
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
s_chtml20_end_center_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</center>");

  return chtml20->out;
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
s_chtml20_start_ul_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
  }

  W_L("<ul>");

  return chtml20->out;
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
s_chtml20_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t     *chtml20;
  Doc           *doc;
  request_rec   *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  W_L("</ul>");

  return chtml20->out;
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
s_chtml20_start_ol_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_start = NULL;
  char        *attr_type = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml20->out;
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
s_chtml20_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W_L("</ol>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_li_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_type = NULL;
  char        *attr_value = NULL;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

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
    else if (STRCASEEQ('v','V',"value", name) && value && *value) {
      attr_value = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  return chtml20->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
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
s_chtml20_start_hr_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align   = NULL;
  char        *attr_size    = NULL;
  char        *attr_width   = NULL;
  char        *attr_noshade = NULL;
  char        *attr_style   = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name (doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "align") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
          attr_align = value;
        }
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        if (value && *value) {
          attr_size = value;
        }
      }
      else if (strcasecmp(name, "style") == 0) {
        if (value && *value) {
          attr_style = value;
        }
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        if (value && *value) {
          attr_width = value;
        }
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(name, "noshade") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_noshade = apr_pstrdup(doc->pool, "noshade");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *border_style_prop = chxj_css_get_property_value(doc, style, "border-style");
      css_property_t *height_prop       = chxj_css_get_property_value(doc, style, "height");
      css_property_t *width_prop        = chxj_css_get_property_value(doc, style, "width");
      css_property_t *cur;
      for (cur = border_style_prop->next; cur != border_style_prop; cur = cur->next) {
        if (STRCASEEQ('s','S',"solid",cur->value)) {
          attr_noshade = "noshade";
        }
      }
      for (cur = height_prop->next; cur != height_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) { 
          *tmpp = 0;
          attr_size = apr_pstrdup(doc->pool, tmp);
        }
      }
      for (cur = width_prop->next; cur != width_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) {
          *tmpp = 0;
          attr_width = apr_pstrdup(doc->pool, tmp);
        }
        else {
          tmpp = strstr(tmp, "%");
          if (tmpp) {
            attr_width = apr_pstrdup(doc->pool, tmp);
          }
        }
      }
    }
  }
  W_L("<hr");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  if (attr_size) {
    W_L(" size=\"");
    W_V(attr_size);
    W_L("\"");
  }
  if (attr_width) {
    W_L(" width=\"");
    W_V(attr_width);
    W_L("\"");
  }
  if (attr_noshade) {
    W_L(" noshade");
  }
  W_L(">");
  return chtml20->out;
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
s_chtml20_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  return chtml20->out;
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
s_chtml20_start_img_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_src    = NULL;
  char        *attr_align  = NULL;
  char        *attr_style  = NULL;
  char        *attr_alt    = NULL;
  char        *attr_width  = NULL;
  char        *attr_height = NULL;
  char        *attr_hspace = NULL;
  char        *attr_vspace = NULL;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table *spec;
#endif

  chtml20 = GET_CHTML20(pdoc);
#ifndef IMG_NOT_CONVERT_FILENAME
  spec    = chtml20->spec;
#endif
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name (doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 's':
    case 'S':
      if (strcasecmp(name, "src") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
        value = chxj_encoding_parameter(r, value, 0);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        value = chxj_add_cookie_no_update_parameter(r, value, 0);
        value = chxj_img_rewrite_parameter(r,chtml20->conf,value);
        attr_src = value;
#else
        value = chxj_img_conv(r, spec, value);
        value = chxj_encoding_parameter(r, value, 0);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        value = chxj_add_cookie_no_update_parameter(r, value);
        value = chxj_img_rewrite_parameter(r,chtml20->conf,value);
        attr_src = value;
#endif
      }
      else if (strcasecmp(name,"style") == 0 && value && *value) {
        attr_style = value;
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "align" ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        if (value) {
          if (STRCASEEQ('t','T',"top",   value) ||
              STRCASEEQ('m','M',"middle",value) ||
              STRCASEEQ('b','B',"bottom",value) ||
              STRCASEEQ('l','L',"left",  value) ||
              STRCASEEQ('r','R',"right", value)) {
            attr_align = value;
          }
          else if (STRCASEEQ('c','C',"center",  value)) {
            attr_align = apr_pstrdup(doc->pool, "middle");
          }
        }
      }
      else if (strcasecmp(name, "alt"   ) == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_alt = value;
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width" ) == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_width = value;
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "height") == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_height = value;
      }
      else
      if (strcasecmp(name, "hspace") == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_hspace = value;
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(name, "vspace") == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_vspace = value;
      }
      break;

    default:
      break;
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *height_prop = chxj_css_get_property_value(doc, style, "height");
      css_property_t *width_prop  = chxj_css_get_property_value(doc, style, "width");
      css_property_t *valign_prop = chxj_css_get_property_value(doc, style, "vertical-align");
      css_property_t *cur;
      for (cur = height_prop->next; cur != height_prop; cur = cur->next) {
        attr_height = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = width_prop->next; cur != width_prop; cur = cur->next) {
        attr_width = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = valign_prop->next; cur != valign_prop; cur = cur->next) {
        attr_align = apr_pstrdup(doc->pool, cur->value);
      }
    }
  }

  W_L("<img");
  if (attr_src) {
    W_L(" src=\"");
    W_V(attr_src);
    W_L("\"");
  }
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  if (attr_alt) {
    W_L(" alt=\"");
    W_V(attr_alt);
    W_L("\"");
  }
  if (attr_width) {
    W_L(" width=\"");
    W_V(attr_width);
    W_L("\"");
  }
  if (attr_height) {
    W_L(" height=\"");
    W_V(attr_height);
    W_L("\"");
  }
  if (attr_hspace) {
    W_L(" hspace=\"");
    W_V(attr_hspace);
    W_L("\"");
  }
  if (attr_vspace) {
    W_L(" vspace=\"");
    W_V(attr_vspace);
    W_L("\"");
  }
  W_L(">");
  return chtml20->out;
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
s_chtml20_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);

  return chtml20->out;
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
s_chtml20_start_select_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char *size      = NULL;
  char *name      = NULL;
  char *multiple  = NULL;

  W_L("<select");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        size = apr_pstrdup(doc->buf.pool, val);
      }
      else if (strcasecmp(nm, "style") == 0 && val && *val) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        attr_style = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(nm, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        name = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'm':
    case 'M':
      if (strcasecmp(nm, "multiple") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        multiple = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    default:
      break;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
  }
  return chtml20->out;
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
s_chtml20_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t   *chtml20;
  Doc         *doc;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  W_L("</select>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
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
s_chtml20_start_option_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char *selected   = NULL;
  char *value      = NULL;

  W_L("<option");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "selected") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        selected = apr_pstrdup(doc->buf.pool, val);
      }
      else if (strcasecmp(nm, "style") == 0 && val && *val) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        attr_style = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(nm, "value") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        value = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    default:
      break;
    }
  }

  if (value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }

  if (selected) {
    W_L(" selected");
  }
  W_L(">");
  if (IS_CSS_ON(chtml20->entryp)) {
    s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
  }
  return chtml20->out;
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
s_chtml20_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);

  /* Don't close */
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
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
s_chtml20_start_div_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style             = NULL;
  char        *attr_align             = NULL;
  char        *attr_display           = NULL;
  char        *attr_decoration        = NULL;
  char        *attr_wap_marquee_style = NULL;
  char        *attr_wap_marquee_dir   = NULL;
  char        *attr_wap_marquee_loop  = NULL;
  char        *attr_color             = NULL;
  char        *attr_bgcolor           = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A', "align", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        attr_align = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('s','S',"style",nm) && val && *val) {
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *display_prop           = chxj_css_get_property_value(doc, style, "display");
      css_property_t *text_decoration_prop   = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *color_prop             = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_align_prop        = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *background_color_prop  = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *background_prop        = chxj_css_get_property_value(doc, style, "background");

      css_property_t *cur;
      for (cur = display_prop->next; cur != display_prop; cur = cur->next) {
        if (strcasecmp("-wap-marquee", cur->value) == 0) {
          attr_display = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = text_decoration_prop->next; cur != text_decoration_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"blink", cur->value)) {
          attr_decoration = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        attr_color = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = text_align_prop->next; cur != text_align_prop; cur = cur->next) {
        attr_align = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = background_color_prop->next; cur != background_color_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      }
      for (cur = background_prop->next; cur != background_prop; cur = cur->next) {
        char *ss = strchr(cur->value, '#');
        if (!ss) {
          ss = strstr(cur->value, "rgb");
        }
        if (ss) {
          attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
          attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
        }
      }
      if (attr_display) {
        css_property_t *wap_marquee_style_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-style");
        css_property_t *wap_marquee_dir_prop   = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
        css_property_t *wap_marquee_loop_prop  = chxj_css_get_property_value(doc, style, "-wap-marquee-loop");
        for (cur = wap_marquee_style_prop->next; cur != wap_marquee_style_prop; cur = cur->next) {
          if (STRCASEEQ('s','S',"scroll", cur->value) || STRCASEEQ('s','S',"slide",cur->value) || STRCASEEQ('a','A',"alternate",cur->value)) {
            attr_wap_marquee_style = apr_pstrdup(doc->pool, cur->value);
          }
        }
        for (cur = wap_marquee_dir_prop->next; cur != wap_marquee_dir_prop; cur = cur->next) {
          if (STRCASEEQ('l','L',"ltr",cur->value)) {
            attr_wap_marquee_dir = apr_pstrdup(doc->pool, "right");
          }
          else if (STRCASEEQ('r','R',"rtl",cur->value)) {
            attr_wap_marquee_dir = apr_pstrdup(doc->pool, "left");
          }
        }
        for (cur = wap_marquee_loop_prop->next; cur != wap_marquee_loop_prop; cur = cur->next) {
          if (STRCASEEQ('i','I',"infinite",cur->value)) {
            attr_wap_marquee_loop = apr_pstrdup(doc->pool, "16");
          }
          else {
            attr_wap_marquee_loop = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }  
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_align) {
    W_L("<div");
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
    W_L(">");
    flg->with_div_flag = 1;
  }
  else {
    W_L("<div>");
    flg->with_div_flag = 1;
  }
  if (attr_color) {
    if (attr_bgcolor && (STRCASEEQ('w','W',"white",attr_color) || STRCASEEQ('#','#',"#ffffff",attr_color))) {
      /* nothing */
    }
    else {
      W_L("<font color=\"");
      W_V(attr_color);
      W_L("\">");
      flg->with_font_flag = 1;
    }
  }
  if (attr_decoration) {
    W_L("<blink>");
    flg->with_blink_flag = 1;
  }
  if (attr_display) {
    W_L("<marquee");
    if (attr_wap_marquee_style) {
      W_L(" behavior=\"");
      W_V(attr_wap_marquee_style);
      W_L("\"");
    }
    if (attr_wap_marquee_dir) {
      W_L(" direction=\"");
      W_V(attr_wap_marquee_dir);
      W_L("\"");
    }
    if (attr_wap_marquee_loop) {
      W_L(" loop=\"");
      W_V(attr_wap_marquee_loop);
      W_L("\"");
    }
    W_L(">");
    flg->with_marquee_flag = 1;
  }
  node->userData = flg;

  return chtml20->out;
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
s_chtml20_end_div_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  chtml20_flags_t *flg = node->userData;
  if (flg && flg->with_marquee_flag) {
    W_L("</marquee>");
  }
  if (flg && flg->with_blink_flag) {
    W_L("</blink>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  node->userData = NULL;

  return chtml20->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h1_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;


  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h1");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20;
  Doc         *doc;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  W_L("</h1>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h2_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h2");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{ 
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</h2>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h3_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h3");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</h3>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h4_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h4");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</h4>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h5_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h5");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</h5>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_h6_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<h6");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");

  return chtml20->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</h6>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_pre_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;
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

  if (IS_CSS_ON(chtml20->entryp)) {
    s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
  }
  chtml20->pre_flag++;
  W_L("<pre>");
  return chtml20->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</pre>");
  chtml20->pre_flag--;
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }

  return chtml20->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_p_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align = NULL;
  char        *attr_style = NULL;
  char        *attr_color = NULL;
  char        *attr_blink = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

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
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
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
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
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
  return chtml20->out;
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
s_chtml20_end_p_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  if (flg && flg->with_blink_flag) {
    W_L("</blink>");
  }
  W_L("</p>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


static char *
s_chtml20_chxjif_tag(void *pdoc, Node *node)
{
  chtml20_t   *chtml20;
  Doc         *doc;
  Node        *child;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {

    W_V(child->otext);
    s_chtml20_chxjif_tag(pdoc, child);
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
s_chtml20_start_textarea_tag(void *pdoc, Node *node) 
{
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_accesskey = NULL;
  char        *attr_name      = NULL;
  char        *attr_rows      = NULL;
  char        *attr_cols      = NULL;
  char        *attr_istyle    = NULL;
  char        *attr_style     = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->textarea_flag++;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "accesskey") == 0 && value && *value != 0) {
        attr_accesskey = value;
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0 && value && *value != 0) {
        attr_name = value;
      }
      break;

    case 'r':
    case 'R':
      if (strcasecmp(name, "rows") == 0 && value && *value != 0) {
        attr_rows = value;
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cols") == 0 && value && *value != 0) {
        attr_cols = value;
      }
      break;

    case 'i':
    case 'I':
      if (strcasecmp(name, "istyle") == 0 && value && (*value == '1' || *value == '2' || *value == '3' || *value == '4')) {
        attr_istyle = value;
      }
      break;
    case 's':
    case 'S':
      if (strcasecmp(name, "style") == 0 && value && *value) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }

  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_nopush_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" istyle=\"");
    W_V(attr_istyle);
    W_L("\"");
  }
  W_L(">");
  return chtml20->out;
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
s_chtml20_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t   *chtml20 = GET_CHTML20(pdoc);
  Doc         *doc     = chtml20->doc;

  W_L("</textarea>");
  chtml20->textarea_flag--;

  return chtml20->out;
}


static char *
s_chtml20_text_tag(void *pdoc, Node *child)
{       
  chtml20_t   *chtml20;
  Doc         *doc;
  request_rec *r;
  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[2];
  int         ii;
  int         tdst_len;
  apr_size_t  z2h_input_len;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;
  
  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return chtml20->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char *out;
    int rtn = s_chtml20_search_emoji(chtml20, &textval[ii], &out, child);
    if (rtn != 0) {
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
    if (chtml20->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (chtml20->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  z2h_input_len = strlen(tdst);
  tdst = chxj_conv_z2h(r, tdst, &z2h_input_len, chtml20->entryp);

  W_V(tdst);
  return chtml20->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_blockquote_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<blockquote>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_blockquote_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;
  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</blockquote>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_dir_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<dir>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_dir_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;

  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</dir>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_dl_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<dl>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_dl_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;
  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</dl>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_dt_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<dt>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_dt_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;
  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_dd_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }
  W_L("<dd>");
  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
  CHTML20_START_OF_TAG_WITH_CSS_FONT_COLOR("<dd>");
}



/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_dd_tag(void *pdoc, Node *node)
{
  CHTML20_END_OF_NO_CLOSE_TAG_WITH_FONT_TAG();
}



/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_menu_tag(void *pdoc, Node *node)
{
  CHTML20_START_OF_TAG_WITH_CSS_FONT_COLOR("<menu>");
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_menu_tag(void *pdoc, Node *node)
{
  CHTML20_END_OF_CLOSE_TAG_WITH_FONT_TAG("</menu>");
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_plaintext_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;

  W_L("<plaintext>");
  s_chtml20_start_plaintext_tag_inner(pdoc,node);
  return chtml20->out;
}

static char *
s_chtml20_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;
  Node      *child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_chtml20_start_plaintext_tag_inner(pdoc, child);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  return chtml20->out;
}

/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_blink_tag(void *pdoc, Node *node)
{
  CHTML20_START_OF_TAG_WITH_CSS_FONT_COLOR("<blink>");
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_blink_tag(void *pdoc, Node *node)
{
  CHTML20_END_OF_CLOSE_TAG_WITH_FONT_TAG("</blink>");
}


/**
 * It is a hanmarqueeer who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_marquee_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc = chtml20->doc;
  Attr      *attr;
  char      *attr_direction = NULL;
  char      *attr_behavior  = NULL;
  char      *attr_loop      = NULL;
  char      *attr_style     = NULL;
  char      *attr_color     = NULL;
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
        attr_direction = value;
      }
    }
    else if (STRCASEEQ('b','B',"behavior",name)) {
      if (value && (STRCASEEQ('s','S',"scroll",value) || STRCASEEQ('s','S',"slide",value) || STRCASEEQ('a','A',"alternate",value))) {
        attr_behavior = value;
      }
    }
    else if (STRCASEEQ('l','L',"loop",name)) {
      if (value && *value) {
        attr_loop = value;
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop     = chxj_css_get_property_value(doc, style, "color");
      css_property_t *direction_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
      css_property_t *style_prop     = chxj_css_get_property_value(doc, style, "-wap-marquee-style");
      css_property_t *loop_prop      = chxj_css_get_property_value(doc, style, "-wap-marquee-loop");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = style_prop->next; cur != style_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_behavior = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = loop_prop->next; cur != loop_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_loop = apr_pstrdup(doc->pool, cur->value);
          if (STRCASEEQ('i','I',"infinite",attr_loop)) {
            attr_loop = "16";
          }
        }
      }
      for (cur = direction_prop->next; cur != direction_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('l','L',"ltr",cur->value)) {
            attr_direction = "right";
          }
          else if (STRCASEEQ('r','R',"rtl",cur->value)) {
            attr_direction = "left";
          }
        }
      }
    }
  }
  W_L("<marquee");
  if (attr_direction) {
    W_L(" direction=\"");
    W_V(attr_direction);
    W_L("\"");
  }
  if (attr_behavior) {
    W_L(" behavior=\"");
    W_V(attr_behavior);
    W_L("\"");
  }
  if (attr_loop) {
    W_L(" loop=\"");
    W_V(attr_loop);
    W_L("\"");
  }
  W_L(">");

  chtml20_flags_t *flg = (chtml20_flags_t *)apr_palloc(doc->pool, sizeof(chtml20_flags_t));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->with_font_flag = 1;
  }
  node->userData = (void *)flg;
  return chtml20->out;
}


/**
 * It is a hanmarqueeer who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_marquee_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc       *doc     = chtml20->doc;
  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</marquee>");
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 *  * It is handler who processes the New Line Code.
 *   */
static char *
s_chtml20_newline_mark(void *pdoc, Node *UNUSED(node))
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc *doc = chtml20->doc;
  W_NLCODE();
  return chtml20->out;
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
s_chtml20_link_tag(void *pdoc, Node *node)
{
  chtml20_t     *chtml20;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  if (! IS_CSS_ON(chtml20->entryp)) {
    return chtml20->out;
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
    DBG(doc->r, "REQ[%X] start load CSS. url:[%s]", TO_ADDR(doc->r),href);
    chtml20->style = chxj_css_parse_from_uri(doc->r, doc->pool, chtml20->style, href);
    DBG(doc->r, "REQ[%X] end load CSS. url:[%s]", TO_ADDR(doc->r),href);
  }

  return chtml20->out;
}


static css_prop_list_t *
s_chtml20_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc *doc = chtml20->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(chtml20->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, chtml20->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(chtml20->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(chtml20->css_prop_stack);
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
s_chtml20_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc *doc = chtml20->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(chtml20->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, chtml20->style, node);
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


/**
 * It is a handler who processes the SPAN tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_start_span_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc *doc;
  Attr *attr;
  char *attr_style = NULL;
  char *attr_color = NULL;
  char *attr_align = NULL;
  char *attr_blink = NULL;
  char *attr_marquee = NULL;
  char *attr_marquee_dir = NULL;
  char *attr_marquee_style = NULL;
  char *attr_marquee_loop = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    css_prop_list_t *style = s_chtml20_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_align_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *decoration_prop = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *display_prop = chxj_css_get_property_value(doc, style, "display");
      css_property_t *marquee_dir_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
      css_property_t *marquee_style_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-style");
      css_property_t *marquee_loop_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-loop");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        attr_color = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = decoration_prop->next; cur != decoration_prop; cur = cur->next) {
        if (cur->value && STRCASEEQ('b','B',"blink",cur->value)) {
          attr_blink = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = display_prop->next; cur != display_prop; cur = cur->next) {
        if (cur->value && strcasecmp("-wap-marquee",cur->value) == 0) {
          attr_marquee = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = marquee_dir_prop->next; cur != marquee_dir_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('l','L',"ltr",cur->value)) {
            attr_marquee_dir = "right";
          }
          else if (STRCASEEQ('r','R',"rtl",cur->value)) {
            attr_marquee_dir = "left";
          }
        }
      }
      for (cur = marquee_style_prop->next; cur != marquee_style_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if ( STRCASEEQ('s','S',"scroll",cur->value)
            || STRCASEEQ('s','S',"slide",cur->value)
            || STRCASEEQ('a','A',"alternate",cur->value)) {
            attr_marquee_style = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
      for (cur = marquee_loop_prop->next; cur != marquee_loop_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('i','I',"infinite",cur->value)) {
            attr_marquee_loop = "16";
          }
          else {
            attr_marquee_loop = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
      for (cur = text_align_prop->next; cur != text_align_prop; cur = cur->next) {
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
  if (attr_color || attr_align || attr_blink || attr_marquee) {
    chtml20_flags_t *flg = apr_palloc(doc->pool, sizeof(*flg));
    memset(flg, 0, sizeof(*flg));
    if (attr_blink) {
      W_L("<blink>");
      flg->with_blink_flag = 1;
    }
    if (attr_marquee) {
      W_L("<marquee");
      if (attr_marquee_dir) {
        W_L(" direction=\"");
        W_V(attr_marquee_dir);
        W_L("\"");
      }
      if (attr_marquee_style) {
        W_L(" behavior=\"");
        W_V(attr_marquee_style);
        W_L("\"");
      }
      if (attr_marquee_loop) {
        W_L(" loop=\"");
        W_V(attr_marquee_loop);
        W_L("\"");
      }
      W_L(">");
      flg->with_marquee_flag = 1;
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("<font color=\"");
      W_V(attr_color);
      W_L("\">");
      flg->with_font_flag = 1;
    }
    if (attr_align) {
      W_L("<div align=\"");
      W_V(attr_align);
      W_L("\">");
      flg->with_div_flag = 1;
    }
    node->userData = flg;
  }
  else {
    node->userData = NULL;
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the SPAN tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_span_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20 = GET_CHTML20(pdoc);
  Doc *doc = chtml20->doc;

  chtml20_flags_t *flg = (chtml20_flags_t *)node->userData;
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  if (flg && flg->with_marquee_flag) {
    W_L("</marquee>");
  }
  if (flg && flg->with_blink_flag) {
    W_L("</blink>");
  }
  if (IS_CSS_ON(chtml20->entryp)) {
    chxj_css_pop_prop_list(chtml20->css_prop_stack);
  }
  return chtml20->out;
}


/**
 * It is a handler who processes the STYLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The STYLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_style_tag(void *pdoc, Node *node)
{
  chtml20_t     *chtml20;
  Doc           *doc;
  Attr          *attr;
  char          *type = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;

  if (! IS_CSS_ON(chtml20->entryp)) {
    return chtml20->out;
  }

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type", name)) {
      if (value && *value && STRCASEEQ('t','T',"text/css",value)) {
        type = value;
      }
    }
  }

  Node *child = qs_get_child_node(doc, node);
  if (type && child) {
    char *name  = qs_get_node_name(doc, child);
    if (STRCASEEQ('t','T',"text", name)) {
      char *value = qs_get_node_value(doc, child);
      DBG(doc->r, "REQ[%X] start load CSS. buf:[%s]", TO_ADDR(doc->r),value);
      chtml20->style = chxj_css_parse_style_value(doc, chtml20->style, value);
      DBG(doc->r, "REQ[%X] end load CSS. value:[%s]", TO_ADDR(doc->r),value);
    }
  }
  return chtml20->out;
}
/*
 * vim:ts=2 et
 */
