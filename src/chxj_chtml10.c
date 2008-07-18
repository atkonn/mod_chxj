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
#include "chxj_chtml10.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_cookie.h"
#include "chxj_encoding.h"
#include "chxj_buffered_write.h"
#include "chxj_str_util.h"

#define GET_CHTML10(X) ((chtml10_t *)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { chtml10->out = BUFFERED_WRITE_LITERAL(chtml10->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { chtml10->out = (X) ? BUFFERED_WRITE_VALUE(chtml10->out, &doc->buf, (X))  \
                                                  : BUFFERED_WRITE_LITERAL(chtml10->out, &doc->buf, ""); } while(0)

#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(chtml10->conf); W_V(nlcode); } while (0)

static char *s_chtml10_start_html_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_html_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_meta_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_meta_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_textarea_tag (void *pdoc, Node *node);
static char *s_chtml10_end_textarea_tag   (void *pdoc, Node *node);
static char *s_chtml10_start_p_tag        (void *pdoc, Node *node);
static char *s_chtml10_end_p_tag          (void *pdoc, Node *node);
static char *s_chtml10_start_pre_tag      (void *pdoc, Node *node);
static char *s_chtml10_end_pre_tag        (void *pdoc, Node *node);
static char *s_chtml10_start_ul_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_ul_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_li_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_li_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_ol_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_ol_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h1_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h1_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h2_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h2_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h3_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h3_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h4_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h4_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h5_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h5_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_h6_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_h6_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_head_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_head_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_title_tag    (void *pdoc, Node *node);
static char *s_chtml10_end_title_tag      (void *pdoc, Node *node);
static char *s_chtml10_start_base_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_base_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_body_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_body_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_a_tag        (void *pdoc, Node *node);
static char *s_chtml10_end_a_tag          (void *pdoc, Node *node);
static char *s_chtml10_start_br_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_br_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_tr_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_tr_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_input_tag    (void *pdoc, Node *node);
static char *s_chtml10_end_input_tag      (void *pdoc, Node *node);
static char *s_chtml10_start_form_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_form_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_center_tag   (void *pdoc, Node *node);
static char *s_chtml10_end_center_tag     (void *pdoc, Node *node);
static char *s_chtml10_start_hr_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_hr_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_img_tag      (void *pdoc, Node *node);
static char *s_chtml10_end_img_tag        (void *pdoc, Node *node);
static char *s_chtml10_start_select_tag   (void *pdoc, Node *node);
static char *s_chtml10_end_select_tag     (void *pdoc, Node *node);
static char *s_chtml10_start_option_tag   (void *pdoc, Node *node);
static char *s_chtml10_end_option_tag     (void *pdoc, Node *node);
static char *s_chtml10_start_div_tag      (void *pdoc, Node *node);
static char *s_chtml10_end_div_tag        (void *pdoc, Node *node);
static char *s_chtml10_start_blockquote_tag(void *pdoc, Node *node);
static char *s_chtml10_end_blockquote_tag (void *pdoc, Node *node);
static char *s_chtml10_start_dir_tag      (void *pdoc, Node *node);
static char *s_chtml10_end_dir_tag        (void *pdoc, Node *node);
static char *s_chtml10_start_dl_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_dl_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_dt_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_dt_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_dd_tag       (void *pdoc, Node *node);
static char *s_chtml10_end_dd_tag         (void *pdoc, Node *node);
static char *s_chtml10_start_menu_tag     (void *pdoc, Node *node);
static char *s_chtml10_end_menu_tag       (void *pdoc, Node *node);
static char *s_chtml10_start_plaintext_tag(void *pdoc, Node *node);
static char *s_chtml10_start_plaintext_tag_inner(void  *pdoc, Node *node);
static char *s_chtml10_end_plaintext_tag  (void *pdoc, Node *node);
static char *s_chtml10_link_tag           (void *pdoc, Node *node);
static char *s_chtml10_style_tag          (void *pdoc, Node *node);
static char *s_chtml10_newline_mark       (void *pdoc, Node *node);

static void  s_init_chtml10(chtml10_t *chtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_chtml10_search_emoji(chtml10_t *chtml, char *txt, char **rslt);
static char *s_chtml10_chxjif_tag        (void *pdoc, Node *node);
static char *s_chtml10_text              (void *pdoc, Node *node);
static css_prop_list_t *s_chtml10_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_chtml10_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);

/* pend */

tag_handler chtml10_handler[] = {
  /* tagHTML */
  {
    s_chtml10_start_html_tag,
    s_chtml10_end_html_tag,
  },
  /* tagMETA */
  {
    s_chtml10_start_meta_tag,
    s_chtml10_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_chtml10_start_textarea_tag,
    s_chtml10_end_textarea_tag,
  },
  /* tagP */
  {
    s_chtml10_start_p_tag,
    s_chtml10_end_p_tag,
  },
  /* tagPRE */
  {
    s_chtml10_start_pre_tag,
    s_chtml10_end_pre_tag,
  },
  /* tagUL */
  {
    s_chtml10_start_ul_tag,
    s_chtml10_end_ul_tag,
  },
  /* tagLI */
  {
    s_chtml10_start_li_tag,
    s_chtml10_end_li_tag,
  },
  /* tagOL */
  {
    s_chtml10_start_ol_tag,
    s_chtml10_end_ol_tag,
  },
  /* tagH1 */
  {
    s_chtml10_start_h1_tag,
    s_chtml10_end_h1_tag,
  },
  /* tagH2 */
  {
    s_chtml10_start_h2_tag,
    s_chtml10_end_h2_tag,
  },
  /* tagH3 */
  {
    s_chtml10_start_h3_tag,
    s_chtml10_end_h3_tag,
  },
  /* tagH4 */
  {
    s_chtml10_start_h4_tag,
    s_chtml10_end_h4_tag,
  },
  /* tagH5 */
  {
    s_chtml10_start_h5_tag,
    s_chtml10_end_h5_tag,
  },
  /* tagH6 */
  {
    s_chtml10_start_h6_tag,
    s_chtml10_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_chtml10_start_head_tag,
    s_chtml10_end_head_tag,
  },
  /* tagTITLE */
  {
    s_chtml10_start_title_tag,
    s_chtml10_end_title_tag,
  },
  /* tagBASE */
  {
    s_chtml10_start_base_tag,
    s_chtml10_end_base_tag,
  },
  /* tagBODY */
  {
    s_chtml10_start_body_tag,
    s_chtml10_end_body_tag,
  },
  /* tagA */
  {
    s_chtml10_start_a_tag,
    s_chtml10_end_a_tag,
  },
  /* tagBR */
  {
    s_chtml10_start_br_tag,
    s_chtml10_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_chtml10_start_tr_tag,
    s_chtml10_end_tr_tag,
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
    NULL,
    NULL,
  },
  /* tagFORM */
  {
    s_chtml10_start_form_tag,
    s_chtml10_end_form_tag,
  },
  /* tagINPUT */
  {
    s_chtml10_start_input_tag,
    s_chtml10_end_input_tag,
  },
  /* tagCENTER */
  {
    s_chtml10_start_center_tag,
    s_chtml10_end_center_tag,
  },
  /* tagHR */
  {
    s_chtml10_start_hr_tag,
    s_chtml10_end_hr_tag,
  },
  /* tagIMG */
  {
    s_chtml10_start_img_tag,
    s_chtml10_end_img_tag,
  },
  /* tagSELECT */
  {
    s_chtml10_start_select_tag,
    s_chtml10_end_select_tag,
  },
  /* tagOPTION */
  {
    s_chtml10_start_option_tag,
    s_chtml10_end_option_tag,
  },
  /* tagDIV */
  {
    s_chtml10_start_div_tag,
    s_chtml10_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_chtml10_chxjif_tag,
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
    s_chtml10_style_tag,
    NULL,
  },
  /* tagSPAN */
  {
    NULL,
    NULL,
  },
  /* tagTEXT */
  {
    s_chtml10_text,
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
    s_chtml10_start_dt_tag,
    s_chtml10_end_dt_tag,
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
    s_chtml10_start_blockquote_tag,
    s_chtml10_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_chtml10_start_dir_tag,
    s_chtml10_end_dir_tag,
  },
  /* tagDL */
  {
    s_chtml10_start_dl_tag,
    s_chtml10_end_dl_tag,
  },
  /* tagDD */
  {
    s_chtml10_start_dd_tag,
    s_chtml10_end_dd_tag,
  },
  /* tagMENU */
  {
    s_chtml10_start_menu_tag,
    s_chtml10_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_chtml10_start_plaintext_tag,
    s_chtml10_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    NULL,
    NULL,
  },
  /* tagMARQUEE */
  {
    NULL,
    NULL,
  },
  /* tagLINK */
  {
    s_chtml10_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_chtml10_newline_mark,
    NULL,
  },
};


/**
 * converts from CHTML5.0 to CHTML1.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_chtml10(
  request_rec         *r,
  device_table        *spec,
  const char          *src,
  apr_size_t          srclen,
  apr_size_t          *dstlen,
  chxjconvrule_entry  *entryp,
  cookie_t*           cookie
)
{
  char      *dst;
  char      *ss;
  chtml10_t chtml10;
  Doc       doc;
  apr_time_t t;

  dst = NULL;

  t = apr_time_now();
  DBG(r, "start chxj_convert_chtml10() cookie_id=[%s]", (cookie) ? cookie->cookie_id : "");

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"i found qrcode xml");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml10(&chtml10, &doc, r, spec);
  chtml10.entryp    = entryp;
  chtml10.cookie    = cookie;

  chxj_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  if (IS_CSS_ON(chtml10.entryp)) {
    /* current property list */
    chtml10.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0,   srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML1.0", ss, srclen);
#endif

  qs_parse_string(&doc,ss, strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void *)&chtml10, &doc, qs_get_root(&doc), 0);
  chtml10.out = chxj_buffered_write_flush(chtml10.out, &doc.buf);
  dst = apr_pstrdup(r->pool, chtml10.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (!dst) {
    return apr_pstrdup(r->pool,ss);
  }

  if (strlen(dst) == 0) {
    dst = apr_psprintf(r->pool, "\n");
  }
  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> CHTML1.0", dst, *dstlen);
#endif

  DBG(r, "end   chxj_convert_chtml10() cookie_id=[%s] time=[%" APR_TIME_T_FMT "]", (cookie) ? cookie->cookie_id : "", apr_time_now() - t);

  return dst;
}


/**
 * The CHTML structure is initialized.
 *
 * @param chtml10 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_chtml10(
  chtml10_t     *chtml10, 
  Doc           *doc, 
  request_rec   *r, 
  device_table  *spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml10, 0, sizeof(chtml10_t));

  doc->r      = r;
  chtml10->doc  = doc;
  chtml10->spec = spec;
  chtml10->out  = qs_alloc_zero_byte_string(r->pool);
  chtml10->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  chtml10->doc->parse_mode = PARSE_MODE_CHTML;
  chtml10->style = NULL;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml10   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_chtml10_search_emoji(chtml10_t *chtml10, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = chtml10->spec;

  len = strlen(txt);
  r = chtml10->doc->r;

  if (!spec) {
    DBG(r,"spec is NULL");
  }

  for (ee = chtml10->conf->emoji;
       ee;
       ee = ee->next) {

    if (!ee->imode) {
      DBG(r,"emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string
    &&  txt
    &&  strlen(ee->imode->string) > 0
    &&  *ee->imode->string == *txt
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (!spec || !spec->emoji_type) {
        *rslt = apr_palloc(r->pool, 3);
        (*rslt)[0] = ee->imode->hex1byte & 0xff;
        (*rslt)[1] = ee->imode->hex2byte & 0xff;
        (*rslt)[2] = 0;
        return strlen(ee->imode->string);
      }

      return 0;
    }
  }
  return 0;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_start_html_tag(void *pdoc, Node *node) 
{
  Doc             *doc;
  request_rec     *r;
  chtml10_t       *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html>");
  return chtml10->out;
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
s_chtml10_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  W_L("</html>");
  return chtml10->out;
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
s_chtml10_start_meta_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  /* ignore */

  return chtml10->out;
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
s_chtml10_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("<head>");

  return chtml10->out;
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
s_chtml10_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</head>");

  return chtml10->out;
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
s_chtml10_start_ol_tag(void *pdoc, Node *node) 
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  W_L("<ol>");

  return chtml10->out;
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
s_chtml10_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</ol>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_ul_tag(void *pdoc, Node *node)
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml10    = GET_CHTML10(pdoc);
  doc        = chtml10->doc;
  r          = doc->r;


  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  W_L("<ul>");

  return chtml10->out;
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
s_chtml10_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</ul>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_li_tag(void *pdoc, Node *node)
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }
  W_L("<li>");

  return chtml10->out;
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
s_chtml10_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t  *chtml10 = GET_CHTML10(pdoc);
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }
  return chtml10->out;
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
s_chtml10_start_h1_tag(void *pdoc, Node *node) 
{
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  chtml10_t     *chtml10;
  char          *attr_style = NULL;
  char          *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</h1>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_h2_tag(void *pdoc, Node *node) 
{
  Doc           *doc;
  Attr          *attr;
  chtml10_t     *chtml10;
  char          *attr_style = NULL;
  char          *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  request_rec   *r;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</h2>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_h3_tag(void *pdoc, Node *node) 
{
  Doc           *doc;
  Attr          *attr;
  chtml10_t     *chtml10;
  char          *attr_style = NULL;
  char          *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t     *chtml10;
  Doc           *doc;
  request_rec   *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</h3>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_h4_tag(void *pdoc, Node *node)
{
  Doc       *doc;
  Attr      *attr;
  chtml10_t *chtml10;
  char      *attr_style = NULL;
  char      *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  chtml10_t     *chtml10;

  chtml10  = GET_CHTML10(pdoc);
  doc      = chtml10->doc;

  W_L("</h4>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_h5_tag(void *pdoc, Node *node) 
{
  Doc       *doc;
  Attr      *attr;
  chtml10_t *chtml10;
  char      *attr_style = NULL;
  char      *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc       *doc;
  chtml10_t *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  W_L("</h5>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_h6_tag(void *pdoc, Node *node)
{
  Doc       *doc;
  Attr      *attr;
  chtml10_t *chtml10;
  char      *attr_style = NULL;
  char      *attr_align = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return chtml10->out;
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
s_chtml10_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  W_L("</h6>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  Doc          *doc;
  chtml10_t    *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  W_L("<title>");

  return chtml10->out;
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
s_chtml10_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  W_L("</title>");

  return chtml10->out;
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
s_chtml10_start_base_tag(void *pdoc, Node *node) 
{
  Attr          *attr;
  chtml10_t     *chtml10;
  Doc           *doc;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  
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

  return chtml10->out;
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
s_chtml10_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_start_body_tag(void *pdoc, Node *node) 
{
  chtml10_t    *chtml10;
  Doc          *doc;
  Attr         *attr;
  char         *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "alink") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 4.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'b':
    case 'B':
      if (strcasecmp(name, "bgcolor") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "text") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'l':
    case 'L':
      if (strcasecmp(name, "link") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
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

    case 's':
    case 'S':
      if (strcasecmp("style", name) == 0 && value && *value) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  W_L("<body>");

  return chtml10->out;
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
s_chtml10_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc           *doc;
  chtml10_t     *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  W_L("</body>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_a_tag(void *pdoc, Node *node) 
{
  chtml10_t     *chtml10;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml10->cookie);
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

    case 'i':
    case 'I':
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
      else
      if (strcasecmp(name, "ijam") == 0) {
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

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }
  return chtml10->out;
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
s_chtml10_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</a>");

  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_br_tag(void *pdoc, Node *node) 
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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

  return chtml10->out;
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
s_chtml10_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("<br>");

  return chtml10->out;
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
s_chtml10_start_form_tag(void *pdoc, Node *node) 
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style  = NULL;
  char        *attr_action = NULL;
  char        *attr_method = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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
      if (strcasecmp(name, "style") == 0 && value && *value) {
        attr_style = value;
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }
  W_L("<form");
  if (attr_action) {
    attr_action = chxj_encoding_parameter(r, attr_action);
    attr_action = chxj_add_cookie_parameter(r, attr_action, chtml10->cookie);
    W_L(" action=\"");
    W_V(attr_action);
    W_L("\"");
  }
  if (attr_method) {
    W_L(" method=\"");
    W_V(attr_method);
    W_L("\"");
  }
  W_L(">");

  return chtml10->out;
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
s_chtml10_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</form>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_input_tag(void *pdoc, Node *node) 
{
  chtml10_t     *chtml10;
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

  chtml10     = GET_CHTML10(pdoc);
  doc         = chtml10->doc;
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
    type = qs_trim_string(doc->buf.pool, type);
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

  if (size && *size != 0) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }

  if (name && *name != 0) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }

  if (value && *value != 0) {
    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool, value));
    W_L("\"");
  }

  if (accesskey && *accesskey != 0) {
    W_L(" accesskey=\"");
    W_V(accesskey);
    W_L("\"");
  }

  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    /* ignore */
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length && *max_length != 0) {
    W_L(" maxlength=\"");
    W_V(max_length);
    W_L("\"");
  }

  if (checked) {
    W_L(" checked");
  }

  W_L(">");
  return chtml10->out;
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
s_chtml10_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_start_center_tag(void *pdoc, Node *node)
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  char         *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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

  W_L("<center>");
  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  return chtml10->out;
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
s_chtml10_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t     *chtml10;
  Doc           *doc;
  request_rec   *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</center>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_hr_tag(void *pdoc, Node *node) 
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align   = NULL;
  char        *attr_size    = NULL;
  char        *attr_width   = NULL;
  char        *attr_noshade = NULL;
  char        *attr_style   = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_nopush_and_get_now_style(pdoc, node, attr_style);
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
  return chtml10->out;
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
s_chtml10_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  return chtml10->out;
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
s_chtml10_start_img_tag(void *pdoc, Node *node) 
{
  chtml10_t   *chtml10;
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

  chtml10 = GET_CHTML10(pdoc);
#ifndef IMG_NOT_CONVERT_FILENAME
  spec    = chtml10->spec;
#endif
  doc     = chtml10->doc;
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
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml10->cookie);
        if (value) {
          value = apr_psprintf(doc->buf.pool, 
                               "%s%c%s=true", 
                               value, 
                               (strchr(value, '?')) ? '&' : '?',
                               CHXJ_COOKIE_NOUPDATE_PARAM);
        }
        attr_src = value;
#else
        value = chxj_img_conv(r, spec, value);
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml10->cookie);
        if (value) {
          value = apr_psprintf(doc->buf.pool,
                               "%s%c%s=true",
                               value,
                               (strchr(value, '?')) ? '&' : '?',
                               CHXJ_COOKIE_NOUPDATE_PARAM);
        }
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

  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_nopush_and_get_now_style(pdoc, node, attr_style);
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
  return chtml10->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  return chtml10->out;
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
s_chtml10_start_select_tag(void *pdoc, Node *child)
{
  chtml10_t    *chtml10;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;

  char         *size;
  char         *name;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  size    = NULL;
  name    = NULL;

  W_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name (doc,attr);
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
        /* Ignore */
      }
      break;

    default:
      break;
    }
  }

  if (size && *size != 0) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }

  if (name && *name != 0) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }

  W_L(">");
  return chtml10->out;
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
s_chtml10_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</select>");
  return chtml10->out;
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
s_chtml10_start_option_tag(void *pdoc, Node *child)
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *selected;
  char        *value;

  chtml10   = GET_CHTML10(pdoc);
  doc       = chtml10->doc;
  r         = doc->r;

  selected  = NULL;
  value     = NULL;

  W_L("<option");

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name (doc,attr);
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

  if (value && *value != 0) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }

  if (selected) {
    W_L(" selected");
  }

  W_L(">");
  return chtml10->out;
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
s_chtml10_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);

  /* Don't close */

  return chtml10->out;
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
s_chtml10_start_div_tag(void *pdoc, Node *child)
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *align;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  align   = NULL;

  W_L("<div");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", nm)) {
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
  return chtml10->out;
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
s_chtml10_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</div>");
  return chtml10->out;
}


/**
 * It is a handler who processes the CHXJ:IF tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CHXJ:IF tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_chxjif_tag(void *pdoc, Node *node)
{
  chtml10_t   *chtml10;
  Doc         *doc;
  Node        *child;
  request_rec *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_chtml10_chxjif_tag(chtml10, child);
  }

  return chtml10->out;
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
s_chtml10_start_pre_tag(void *pdoc, Node *node)
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;
  Attr        *attr;
  char        *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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

  if (IS_CSS_ON(chtml10->entryp)) {
    s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
  }
  chtml10->pre_flag++;
  W_L("<pre>");
  return chtml10->out;
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
s_chtml10_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</pre>");
  chtml10->pre_flag--;
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }

  return chtml10->out;
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
s_chtml10_start_p_tag(void *pdoc, Node *node) 
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;
  Attr        *attr;
  char        *attr_align = NULL;
  char        *attr_style = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
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
    else if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *style = s_chtml10_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *text_align = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *cur;
      for (cur = text_align->next; cur != text_align; cur = cur->next) {
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
    }
  }
  W_L("<p");
  if (attr_align) {
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  W_L(">");
  return chtml10->out;
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
s_chtml10_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</p>");
  if (IS_CSS_ON(chtml10->entryp)) {
    chxj_css_pop_prop_list(chtml10->css_prop_stack);
  }
  return chtml10->out;
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
s_chtml10_start_textarea_tag(void *pdoc, Node *node) 
{
  Doc             *doc;
  request_rec     *r;
  chtml10_t       *chtml10;
  Attr            *attr;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  chtml10->textarea_flag++;

  W_L("<textarea");

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name (doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "accesskey") == 0 && value && *value != 0) {
        W_L(" accesskey=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0 && value && *value != 0) {
        W_L(" name=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'r':
    case 'R':
      if (strcasecmp(name, "rows") == 0 && value && *value != 0) {
        W_L(" rows=\"");
        W_V(value);
        W_L("\"");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cols") == 0 && value && *value != 0) {
        W_L(" cols=\"");
        W_V(value);
        W_L("\"");
      }
      break;
    
    default:
      break;
    }
  }
  W_L(">");
  return chtml10->out;
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
s_chtml10_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc         *doc;
  request_rec *r;
  chtml10_t   *chtml10;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  W_L("</textarea>");
  chtml10->textarea_flag--;

  return chtml10->out;
}


static char *
s_chtml10_text(void *pdoc, Node *child)
{
  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[2];
  int         ii;
  int         tdst_len;
  chtml10_t   *chtml10;
  Doc         *doc;
  request_rec *r;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  r       = doc->r;

  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return chtml10->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char *out;
    int   rtn;

    rtn = s_chtml10_search_emoji(chtml10, &textval[ii], &out);
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
    if (chtml10->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (chtml10->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }

  W_V(tdst);
  return chtml10->out;
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
s_chtml10_start_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<blockquote>");
  return chtml10->out;
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
s_chtml10_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("</blockquote>");
  return chtml10->out;
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
s_chtml10_start_dir_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<dir>");
  return chtml10->out;
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
s_chtml10_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("</dir>");
  return chtml10->out;
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
s_chtml10_start_dl_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<dl>");
  return chtml10->out;
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
s_chtml10_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("</dl>");
  return chtml10->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_start_dt_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<dt>");
  return chtml10->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  
  return chtml10->out;
}


/**
 * It is a handder who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_start_dd_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<dd>");
  return chtml10->out;
}


/**
 * It is a handder who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  return chtml10->out;
}


/**
 * It is a hanmenuer who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_start_menu_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10;
  Doc *doc;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<menu>");
  return chtml10->out;
}


/**
 * It is a hanmenuer who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml10_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  Doc *doc = chtml10->doc;
  W_L("</menu>");
  return chtml10->out;
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
s_chtml10_start_plaintext_tag(void *pdoc, Node *node)
{
  chtml10_t *chtml10;
  Doc *doc;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  W_L("<plaintext>");
  s_chtml10_start_plaintext_tag_inner(pdoc,node);
  return chtml10->out;
}

static char *
s_chtml10_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  chtml10_t *chtml10;
  Doc *doc;
  Node *child;
  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_chtml10_start_plaintext_tag_inner(pdoc, child);
  }
  return chtml10->out;
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
s_chtml10_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  return chtml10->out;
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
s_chtml10_link_tag(void *pdoc, Node *node)
{
  chtml10_t     *chtml10;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  if (! IS_CSS_ON(chtml10->entryp)) {
    return chtml10->out;
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
    chtml10->style = chxj_css_parse_from_uri(doc->r, doc->pool, chtml10->style, href);
    DBG(doc->r, "end load CSS. url:[%s]", href);
  }

  return chtml10->out;
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
s_chtml10_style_tag(void *pdoc, Node *node)
{
  chtml10_t     *chtml10;
  Doc           *doc;
  Attr          *attr;
  char          *type = NULL;

  chtml10 = GET_CHTML10(pdoc);
  doc     = chtml10->doc;

  if (! IS_CSS_ON(chtml10->entryp)) {
    return chtml10->out;
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
      DBG(doc->r, "start load CSS. buf:[%s]", value);
      chtml10->style = chxj_css_parse_style_value(doc, chtml10->style, value);
      DBG(doc->r, "end load CSS. value:[%s]", value);
    }
  }
  return chtml10->out;
}


static char *
s_chtml10_newline_mark(void *pdoc, Node *UNUSED(node))
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  Doc *doc = chtml10->doc;
  W_NLCODE();
  return chtml10->out;
}

static css_prop_list_t *
s_chtml10_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  Doc *doc = chtml10->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(chtml10->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, chtml10->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(chtml10->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(chtml10->css_prop_stack);

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
s_chtml10_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  chtml10_t *chtml10 = GET_CHTML10(pdoc);
  Doc *doc = chtml10->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(chtml10->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(chtml10->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, chtml10->style, node);
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
