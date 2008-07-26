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
#include "mod_chxj.h"
#include "chxj_encoding.h"
#include "chxj_xhtml_mobile_1_0.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_buffered_write.h"
#include "chxj_str_util.h"

#define GET_XHTML(X) ((xhtml_t*)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { xhtml->out = BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { xhtml->out = (X) ? BUFFERED_WRITE_VALUE(xhtml->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(xhtml->conf); W_V(nlcode); } while (0)

static char *s_xhtml_1_0_start_html_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_html_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_p_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_p_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_pre_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_pre_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_ul_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_ul_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h1_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h1_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h2_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h2_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h3_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h3_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h4_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h4_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h5_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h5_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h6_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h6_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_ol_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_ol_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_li_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_li_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_meta_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_meta_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_head_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_head_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_title_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_title_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_base_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_base_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_body_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_body_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_a_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_a_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_br_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_br_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_tr_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_tr_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_font_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_font_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_form_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_form_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_input_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_input_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_center_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_center_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_hr_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_hr_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_img_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_img_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_select_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_select_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_option_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_option_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_div_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_div_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_textarea_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_textarea_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_b_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_b_tag           (void *pdoc, Node *node);
static char *s_xhtml_1_0_chxjif_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_blockquote_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dir_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dir_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dl_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dl_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dt_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dt_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dd_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dd_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_menu_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_menu_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_blink_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_blink_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_marquee_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_marquee_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_newline_mark       (void *pdoc, Node *node);
static char *s_xhtml_1_0_link_tag           (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_span_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_span_tag        (void *pdoc, Node *node);

static void  s_init_xhtml(xhtml_t *xhtml, Doc *doc, request_rec *r, device_table *spec);
static int   s_xhtml_search_emoji(xhtml_t *xhtml, char *txt, char **rslt);
static char *s_xhtml_1_0_text_tag(void *pdoc, Node *child);
static css_prop_list_t *s_xhtml_1_0_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_xhtml_1_0_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
/* pend */


tag_handler xhtml_handler[] = {
  /* tagHTML */
  {
    s_xhtml_1_0_start_html_tag,
    s_xhtml_1_0_end_html_tag,
  },
  /* tagMETA */
  {
    s_xhtml_1_0_start_meta_tag,
    s_xhtml_1_0_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_xhtml_1_0_start_textarea_tag,
    s_xhtml_1_0_end_textarea_tag,
  },
  /* tagP */
  {
    s_xhtml_1_0_start_p_tag,
    s_xhtml_1_0_end_p_tag,
  },
  /* tagPRE */
  {
    s_xhtml_1_0_start_pre_tag,
    s_xhtml_1_0_end_pre_tag,
  },
  /* tagUL */
  {
    s_xhtml_1_0_start_ul_tag,
    s_xhtml_1_0_end_ul_tag,
  },
  /* tagLI */
  {
    s_xhtml_1_0_start_li_tag,
    s_xhtml_1_0_end_li_tag,
  },
  /* tagOL */
  {
    s_xhtml_1_0_start_ol_tag,
    s_xhtml_1_0_end_ol_tag,
  },
  /* tagH1 */
  {
    s_xhtml_1_0_start_h1_tag,
    s_xhtml_1_0_end_h1_tag,
  },
  /* tagH2 */
  {
    s_xhtml_1_0_start_h2_tag,
    s_xhtml_1_0_end_h2_tag,
  },
  /* tagH3 */
  {
    s_xhtml_1_0_start_h3_tag,
    s_xhtml_1_0_end_h3_tag,
  },
  /* tagH4 */
  {
    s_xhtml_1_0_start_h4_tag,
    s_xhtml_1_0_end_h4_tag,
  },
  /* tagH5 */
  {
    s_xhtml_1_0_start_h5_tag,
    s_xhtml_1_0_end_h5_tag,
  },
  /* tagH6 */
  {
    s_xhtml_1_0_start_h6_tag,
    s_xhtml_1_0_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_xhtml_1_0_start_head_tag,
    s_xhtml_1_0_end_head_tag,
  },
  /* tagTITLE */
  {
    s_xhtml_1_0_start_title_tag,
    s_xhtml_1_0_end_title_tag,
  },
  /* tagBASE */
  {
    s_xhtml_1_0_start_base_tag,
    s_xhtml_1_0_end_base_tag,
  },
  /* tagBODY */
  {
    s_xhtml_1_0_start_body_tag,
    s_xhtml_1_0_end_body_tag,
  },
  /* tagA */
  {
    s_xhtml_1_0_start_a_tag,
    s_xhtml_1_0_end_a_tag,
  },
  /* tagBR */
  {
    s_xhtml_1_0_start_br_tag,
    s_xhtml_1_0_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_xhtml_1_0_start_tr_tag,
    s_xhtml_1_0_end_tr_tag,
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
    s_xhtml_1_0_start_font_tag,
    s_xhtml_1_0_end_font_tag,
  },
  /* tagFORM */
  {
    s_xhtml_1_0_start_form_tag,
    s_xhtml_1_0_end_form_tag,
  },
  /* tagINPUT */
  {
    s_xhtml_1_0_start_input_tag,
    s_xhtml_1_0_end_input_tag,
  },
  /* tagCENTER */
  {
    s_xhtml_1_0_start_center_tag,
    s_xhtml_1_0_end_center_tag,
  },
  /* tagHR */
  {
    s_xhtml_1_0_start_hr_tag,
    s_xhtml_1_0_end_hr_tag,
  },
  /* tagIMG */
  {
    s_xhtml_1_0_start_img_tag,
    s_xhtml_1_0_end_img_tag,
  },
  /* tagSELECT */
  {
    s_xhtml_1_0_start_select_tag,
    s_xhtml_1_0_end_select_tag,
  },
  /* tagOPTION */
  {
    s_xhtml_1_0_start_option_tag,
    s_xhtml_1_0_end_option_tag,
  },
  /* tagDIV */
  {
    s_xhtml_1_0_start_div_tag,
    s_xhtml_1_0_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_xhtml_1_0_chxjif_tag,
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
    s_xhtml_1_0_start_span_tag,
    s_xhtml_1_0_end_span_tag,
  },
  /* tagTEXT */
  {
    s_xhtml_1_0_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_xhtml_1_0_start_b_tag,
    s_xhtml_1_0_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_xhtml_1_0_start_dt_tag,
    s_xhtml_1_0_end_dt_tag,
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
    s_xhtml_1_0_start_blockquote_tag,
    s_xhtml_1_0_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_xhtml_1_0_start_dir_tag,
    s_xhtml_1_0_end_dir_tag,
  },
  /* tagDL */
  {
    s_xhtml_1_0_start_dl_tag,
    s_xhtml_1_0_end_dl_tag,
  },
  /* tagDD */
  {
    s_xhtml_1_0_start_dd_tag,
    s_xhtml_1_0_end_dd_tag,
  },
  /* tagMENU */
  {
    s_xhtml_1_0_start_menu_tag,
    s_xhtml_1_0_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_xhtml_1_0_start_plaintext_tag,
    s_xhtml_1_0_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_xhtml_1_0_start_blink_tag,
    s_xhtml_1_0_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_xhtml_1_0_start_marquee_tag,
    s_xhtml_1_0_end_marquee_tag,
  },
  /* tagLINK */
  {
    s_xhtml_1_0_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_xhtml_1_0_newline_mark,
    NULL,
  },
};
 
/**
 * converts from CHTML to XHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_xhtml_mobile_1_0(
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
  xhtml_t   xhtml;
  Doc       doc;

  DBG(r,"start chxj_convert_xhtml_mobile_1_0()");
  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst != NULL) {
    DBG(r,"end chxj_convert_xhtml_mobile_1_0() (found qrcode.xml)");
    return dst;
  }

  /*--------------------------------------------------------------------------*/
  /* The XHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_xhtml(&xhtml, &doc, r, spec);

  xhtml.entryp = entryp;
  xhtml.cookie = cookie;

  chxj_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss,   0, srclen + 1);
  memcpy(ss, src, srclen);

  if (IS_CSS_ON(xhtml.entryp)) {
    /* current property list */
    xhtml.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML->XHTML", ss, srclen);
#endif
  qs_parse_string(&doc,ss, strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to XHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void *)&xhtml, &doc, qs_get_root(&doc), 0);
  xhtml.out = chxj_buffered_write_flush(xhtml.out, &doc.buf);
  dst = apr_pstrdup(r->pool, xhtml.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (! dst) {
    return apr_pstrdup(r->pool,ss);
  }

  if (! *dst) {
    dst = apr_psprintf(r->pool, "\n");
  }
  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML->XHTML", dst, *dstlen);
#endif

  DBG(r,"end chxj_convert_xhtml_mobile_1_0()");
  return dst;
}


/**
 * The XHTML structure is initialized.
 *
 * @param xhtml [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_xhtml(xhtml_t *xhtml, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(xhtml, 0, sizeof(xhtml_t));

  doc->r      = r;
  xhtml->doc  = doc;
  xhtml->spec = spec;
  xhtml->out  = qs_alloc_zero_byte_string(r->pool);
  xhtml->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  xhtml->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param xhtml   [i]   The pointer to the XHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_xhtml_search_emoji(xhtml_t *xhtml, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = xhtml->spec;

  len = strlen(txt);
  r = xhtml->doc->r;

  if (spec == NULL) {
    DBG(r,"spec is NULL");
  }

  for (ee = xhtml->conf->emoji;
       ee;
       ee = ee->next) {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (!ee->imode) {
      DBG(r,"emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }

      if (strcasecmp(xhtml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      } 
      else
      if (strcasecmp(xhtml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeB);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeC);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeD);
        return strlen(ee->imode->string);
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }
      return 0;
    }
    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;
    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeA);
        return 2;
      }

      if (strcasecmp(xhtml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeA);
        return 2;
      } 
      else
      if (strcasecmp(xhtml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeB);
        return 2;
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeC);
        return 2;
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeD);
        return 2;
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeD);
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
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_html_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;

  /*--------------------------------------------------------------------------*/
  /* Add XML Declare                                                          */
  /*--------------------------------------------------------------------------*/
  W_L("<?xml version=\"1.0\" encoding=\"Windows-31J\"?>");
  W_NLCODE();
  /*--------------------------------------------------------------------------*/
  /* Add DocType                                                              */
  /*--------------------------------------------------------------------------*/
  W_L("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"");
  W_NLCODE();
  W_L(" \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">");
  W_NLCODE();
  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html xmlns=\"http://www.w3.org/1999/xhtml\"");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('l','L',"lang", name)) {
      W_L(" xml:lang=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"version", name)) {
      W_L(" version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"");
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;
  W_L("</html>");
  return xhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_meta_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;
  int           content_type_flag = 0;

  W_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name", name) && value && *value) {
      W_L(" ");
      W_V(name);
      W_L("=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"http-equiv", name) && value && *value) {
      W_L(" ");
      W_V(name);
      W_L("=\"");
      W_V(value);
      W_L("\"");
      if (STRCASEEQ('c','C', "content-type", value)) {
        content_type_flag = 1;
      }
    }
    else if (STRCASEEQ('c','C',"content", name) && value && *value) {
      if (content_type_flag) {
        W_L(" ");
        W_V(name);
        W_L("=\"");
        W_L("text/html; charset=Windows-31J");
        W_L("\"");
      }
      else {
        W_L(" ");
        W_V(name);
        W_L("=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(" />");
  return xhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("<head>");
  return xhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</head>");
  return xhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t      *xhtml = GET_XHTML(pdoc);
  Doc          *doc   = xhtml->doc;

  W_L("<title>");
  return xhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</title>");

  return xhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_base_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;

  W_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href",name)) {
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
      break;
    }
  }
  W_L(" />");

  return xhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_body_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml = GET_XHTML(pdoc);
  Doc         *doc   = xhtml->doc;
  Attr        *attr;
  char        *attr_bgcolor = NULL;
  char        *attr_text    = NULL;
  char        *attr_link    = NULL;
  char        *attr_style   = NULL;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B',"bgcolor", name) && value && *value) {
      attr_bgcolor = value;
    }
    else if (STRCASEEQ('t','T',"text",name) && value && *value) {
      attr_text = value;
    }
    else if (STRCASEEQ('l','L',"link", name) && value && *value) {
      attr_link = value;
    }
    else if (STRCASEEQ('a','A',"alink", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    if (xhtml->style) {
      css_stylesheet_t *pseudos = chxj_find_pseudo_selectors(doc, xhtml->style);
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
  if (attr_bgcolor || attr_text) {
    W_L(" style=\"");
    if (attr_bgcolor) {
      attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      W_L("background-color:");
      W_V(attr_bgcolor);
      W_L(";");
    }
    if (attr_text) {
      attr_text = chxj_css_rgb_func_to_value(doc->pool, attr_text);
      W_L("color:");
      W_V(attr_text);
      W_L(";");
    }
    W_L("\"");
  }
  if (attr_link) {
    attr_link = chxj_css_rgb_func_to_value(doc->pool, attr_link);
    W_L(" link=\"");
    W_V(attr_link);
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</body>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_a_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml = GET_XHTML(pdoc);
  Doc         *doc   = xhtml->doc;
  request_rec *r     = doc->r;
  Attr        *attr;
  char        *attr_style = NULL;

  W_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name) && value && *value) {
      W_L(" id=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"href", name) && value && *value) {
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, xhtml->cookie);
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cti",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ijam", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('t','T',"telbook",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('k','K',"kana",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('e','E',"email",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ista",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  W_L(">");

  if (IS_CSS_ON(xhtml->entryp)) {
    s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</a>");

  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_br_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

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
  W_L(" />");

  return xhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<br />");

  return xhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_font_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;
  char    *attr_style = NULL;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color",name) && value && *value) {
      attr_color = apr_pstrdup(doc->buf.pool, value);
    }
    else if (STRCASEEQ('s','S',"size",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_size = apr_pstrdup(doc->buf.pool, value);
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = apr_pstrdup(doc->buf.pool, value);
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop  = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_size = apr_pstrdup(doc->pool, cur->value);
          if (STRCASEEQ('x','X',"xx-small",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "1");
          }
          else if (STRCASEEQ('x','X',"x-small",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "2");
          }
          else if (STRCASEEQ('s','S',"small",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "3");
          }
          else if (STRCASEEQ('m','M',"medium",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "4");
          }
          else if (STRCASEEQ('l','L',"large",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "5");
          }
          else if (STRCASEEQ('x','X',"x-large",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "6");
          }
          else if (STRCASEEQ('x','X',"xx-large",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, "7");
          }
        }
      }
    }
  }

  xhtml_flags_t *flg = (xhtml_flags_t *)apr_palloc(doc->pool, sizeof(*flg));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    W_L("<font color=\"");
    W_V(attr_color);
    W_L("\">");
    flg->font_color_flag = 1;
  }
  if (attr_size) {
    flg->font_size_flag = 1;
    switch(*attr_size) {
    case '1': W_L("<span style=\"font-size: xx-small\">"); break;
    case '2': W_L("<span style=\"font-size: x-small\">");  break;
    case '3': W_L("<span style=\"font-size: small\">");    break;
    case '4': W_L("<span style=\"font-size: medium\">");   break;
    case '5': W_L("<span style=\"font-size: large\">");    break;
    case '6': W_L("<span style=\"font-size: x-large\">");  break;
    case '7': W_L("<span style=\"font-size: xx-large\">"); break;
    case '-':
      if (*(attr_size + 1) == '1') {
        W_L("<span style=\"font-size: small\">");
        break;
      }
      if (*(attr_size + 1) == '2') {
        W_L("<span style=\"font-size: x-small\">");
        break;
      }
      if (*(attr_size + 1) == '3') {
        W_L("<span style=\"font-size: xx-small\">");
        break;
      }
      flg->font_size_flag = 0;
      break;

    case '+':
      if (*(attr_size + 1) == '1') {
        W_L("<span style=\"font-size: large\">");
        break;
      }
      if (*(attr_size + 1) == '2') {
        W_L("<span style=\"font-size: x-large\">");
        break;
      }
      if (*(attr_size + 1) == '3') {
        W_L("<span style=\"font-size: xx-large\">");
        break;
      }
      flg->font_size_flag = 0;
      break;

    default:
      WRN(doc->r, "invlalid font size. [%s] != (1|2|3|4|5|6|7|+1|+2|+3|-1|-2|-3)", attr_size);
      flg->font_size_flag = 0;
    }
  }
  node->userData = flg;

  return xhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_font_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  xhtml_flags_t *flg = (xhtml_flags_t *)node->userData;
  if (flg && flg->font_size_flag) {
    W_L("</span>");
  }
  if (flg && flg->font_color_flag) {
    W_L("</font>");
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_form_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml = GET_XHTML(pdoc);
  Doc         *doc   = xhtml->doc;
  request_rec *r     = doc->r;
  Attr        *attr;
  char        *attr_action = NULL;
  char        *attr_method = NULL;
  char        *attr_style  = NULL;
  char        *attr_color  = NULL;
  char        *attr_align  = NULL;
  char        *attr_name   = NULL;
  char        *new_hidden_tag = NULL;

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

    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_name = value;
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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
  W_L("<form");
  if (attr_action) {
    attr_action = chxj_encoding_parameter(r, attr_action);
    attr_action = chxj_add_cookie_parameter(r, attr_action, xhtml->cookie);
    char *q;
    q = strchr(attr_action, '?');
    if (q) {
      new_hidden_tag = chxj_form_action_to_hidden_tag(doc->pool, attr_action, 0);
      *q = 0;
    }
    W_L(" action=\"");
    W_V(attr_action);
    W_L("\"");
  }
  if (attr_method) {
    W_L(" method=\"");
    W_V(attr_method);
    W_L("\"");
  }
  if (attr_name) {
    W_L(" name=\"");
    W_V(attr_name);
    W_L("\"");
  }
  W_L(">");

  xhtml_flags_t *flg = (xhtml_flags_t *)apr_palloc(doc->pool, sizeof(xhtml_flags_t));
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
  if (new_hidden_tag) {
    W_V(new_hidden_tag);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_form_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  xhtml_flags_t *flg = (xhtml_flags_t *)node->userData;
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</form>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_input_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml       = GET_XHTML(pdoc);
  Doc         *doc         = xhtml->doc;
  request_rec *r           = doc->r;
  Attr        *attr;
  char        *attr_max_length  = NULL;
  char        *attr_type        = NULL;
  char        *attr_name        = NULL;
  char        *attr_value       = NULL;
  char        *attr_istyle      = NULL;
  char        *attr_size        = NULL;
  char        *attr_checked     = NULL;
  char        *attr_accesskey   = NULL;
  char        *attr_style       = NULL;

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

  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_nopush_and_get_now_style(pdoc, node, attr_style);
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
    attr_type = qs_trim_string(doc->buf.pool, attr_type);
    if (attr_type && (STRCASEEQ('t','T',"text",    attr_type) ||
                      STRCASEEQ('p','P',"password",attr_type) ||
                      STRCASEEQ('c','C',"checkbox",attr_type) ||
                      STRCASEEQ('r','R',"radio",   attr_type) ||
                      STRCASEEQ('h','H',"hidden",  attr_type) ||
                      STRCASEEQ('s','S',"submit",  attr_type) ||
                      STRCASEEQ('r','R',"reset",   attr_type))) {
      W_L(" type=\"");
      W_V(attr_type);
      W_L("\"");
    }
  }
  if (attr_size && *attr_size) {
    W_L(" size=\"");
    W_V(attr_size);
    W_L("\"");
  }
  if (attr_name && *attr_name) {
    W_L(" name=\"");
    W_V(attr_name);
    W_L("\"");
  }
  if (attr_value && *attr_value) {
    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool, attr_value));
    W_L("\"");
  }
  if (attr_accesskey && *attr_accesskey) {
    W_L(" accesskey=\"");
    W_V(attr_accesskey);
    W_L("\"");
  }
  if (attr_istyle && *attr_istyle && (*attr_istyle == '1' || *attr_istyle == '2' || *attr_istyle == '3' || *attr_istyle == '4')) {
    char *fmt = qs_conv_istyle_to_format(r,attr_istyle);
    if (attr_max_length && *attr_max_length) {
      int ii;
      for (ii=0; (unsigned int)ii<strlen(attr_max_length); ii++) {
        if (attr_max_length[ii] < '0' || attr_max_length[ii] > '9') {
          attr_max_length = apr_psprintf(r->pool, "0");
          break;
        }
      }

      if (strcmp(attr_max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%d%s\"", atoi(attr_max_length), fmt);
        W_V(vv);
      }
    }
    else {
      W_L(" FORMAT=\"");
      W_L("*");
      W_V(fmt);
      W_L("\"");
    }
  }
  else {
    if (attr_max_length && *attr_max_length) {
      if (chxj_chk_numeric(attr_max_length) != 0) {
        attr_max_length = apr_psprintf(r->pool, "0");
      }
      if (strcmp(attr_max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%dm\"", atoi(attr_max_length));
        W_V(vv);
      }
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (attr_type && (attr_istyle == NULL || *attr_istyle == 0) && STRCASEEQ('p','P',"password", attr_type) && ! xhtml->entryp->pc_flag) {
    if (attr_max_length) {
      W_L(" FORMAT=\"");
      W_V(attr_max_length);
      W_L("N\"");
    }
    else {
      W_L(" FORMAT=\"*N\"");
    }
  }
  if (attr_checked) {
    W_L(" checked=\"checked\"");
  }
  W_L(" />");
#if 0
  W_L("<input");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  type       = qs_get_type_attr(doc, node, doc->buf.pool);
  name       = qs_get_name_attr(doc, node, doc->buf.pool);
  value      = qs_get_value_attr(doc,node, doc->buf.pool);
  istyle     = qs_get_istyle_attr(doc,node,doc->buf.pool);
  max_length = qs_get_maxlength_attr(doc,node,doc->buf.pool);
  checked    = qs_get_checked_attr(doc,node, doc->buf.pool);
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
  if (istyle && *istyle && (*istyle == '1' || *istyle == '2' || *istyle == '3' || *istyle == '4')) {
    char *fmt = qs_conv_istyle_to_format(r,istyle);
    if (max_length && *max_length) {
      int ii;
      for (ii=0; (unsigned int)ii<strlen(max_length); ii++) {
        if (max_length[ii] < '0' || max_length[ii] > '9') {
          max_length = apr_psprintf(r->pool, "0");
          break;
        }
      }

      if (strcmp(max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%d%s\"", atoi(max_length), fmt);
        W_V(vv);
      }
    }
    else {
      W_L(" FORMAT=\"");
      W_L("*");
      W_V(fmt);
      W_L("\"");
    }
  }
  else {
    if (max_length && *max_length) {
      if (chxj_chk_numeric(max_length) != 0) {
        max_length = apr_psprintf(r->pool, "0");
      }
      if (strcmp(max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%dm\"", atoi(max_length));
        W_V(vv);
      }
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (type && (istyle == NULL || *istyle == 0) && STRCASEEQ('p','P',"password", type) && ! xhtml->entryp->pc_flag) {
    if (max_length) {
      W_L(" FORMAT=\"");
      W_V(max_length);
      W_L("N\"");
    }
    else {
      W_L(" FORMAT=\"*N\"");
    }
  }
  if (checked) {
    W_L(" checked=\"checked\"");
  }
  W_L(" />");
#endif

  return xhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_center_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml;
  Doc     *doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;

  xhtml = GET_XHTML(pdoc);
  doc    = xhtml->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop       = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_size = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }

  W_L("<center");
  if (attr_size || attr_color) {
    W_L(" style=\"");
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  
  return xhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_center_tag(void *pdoc, Node *UNUSED(node))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</center>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_hr_tag(void *pdoc, Node *node)
{
  Attr        *attr;
  xhtml_t     *xhtml;
  Doc         *doc;
  request_rec *r;
  char        *attr_align   = NULL;
  char        *attr_size    = NULL;
  char        *attr_width   = NULL;
  char        *attr_noshade = NULL;
  char        *attr_style   = NULL;
  char        *attr_color   = NULL;

  xhtml   = GET_XHTML(pdoc);
  doc     = xhtml->doc;
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
      if (strcasecmp(name, "color") == 0 && value && *value) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        attr_color = value;
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_nopush_and_get_now_style(pdoc, node, attr_style);
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
          attr_size = apr_pstrdup(doc->pool, tmp);
        }
      }
      for (cur = width_prop->next; cur != width_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) {
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
  if (attr_size || attr_width || attr_noshade) {
    W_L(" style=\"");
    if (attr_size) {
      W_L("height:");
      W_V(attr_size);
      if (!strstr(attr_size, "px")) {
        W_L("px");
      }
      W_L(";");
    }
    if (attr_width) {
      W_L("width:");
      W_V(attr_width);
      if (!strstr(attr_width, "px") && !strstr(attr_width, "%")) {
        W_L("px");
      }
      W_L(";");
    }
    if (attr_noshade) {
      W_L("border-style:solid;");
    }
    W_L("\"");
  }
  if (attr_color) {
    W_L(" color=\"");
    W_V(attr_color);
    W_L("\"");
  }
  W_L(" />");

  return xhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
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
s_xhtml_1_0_start_pre_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }

  if (IS_CSS_ON(xhtml->entryp)) {
    s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
  }

  xhtml->pre_flag++;
  W_L("<pre>");
  return xhtml->out;
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
s_xhtml_1_0_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</pre>");
  xhtml->pre_flag--;
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
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
s_xhtml_1_0_start_p_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_align = NULL;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_blink = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
  if ((attr_align && *attr_align) || (attr_color && *attr_color) || (attr_blink && *attr_blink)) {
    W_L(" style=\"");
    if (attr_align) {
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_blink) {
      W_L("text-decoration:");
      W_V(attr_blink);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
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
s_xhtml_1_0_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</p>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_ul_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char        *attr_type = NULL;
  char        *attr_style = NULL;
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        attr_type = value;
      }
    }
    else if (value && *value && STRCASEEQ('s','S',"style", name)) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('d','D',"disc",cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "disc");
        }
        else if (STRCASEEQ('c','C',"circle",cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "circle");
        }
        else if (STRCASEEQ('s','S',"square",cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "square");
        }
      }
    }
  }
  W_L("<ul");
  if (attr_type) {
    W_L(" style=\"");
    W_L("list-style-type:");
    W_V(attr_type);
    W_L(";");
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</ul>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h1_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h1>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h2_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h2>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h3_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h3>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h4_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h4>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h5_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h5>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h6_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_align = NULL;

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
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" style=\"");
    W_L("text-align:");
    W_V(attr_align);
    W_L(";");
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h6>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_ol_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_start = NULL;
  char    *attr_type  = NULL;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value) {
      if (*value == '1') {
        attr_type = apr_pstrdup(doc->pool, "decimal");
      }
      else if (*value == 'a') {
        attr_type = apr_pstrdup(doc->pool, "lower-alpha");
      }
      else if (*value == 'A') {
        attr_type = apr_pstrdup(doc->pool, "upper-alpha");
      }
    }
    else if (STRCASEEQ('s','S',"start",name) && value && *value) {
      attr_start = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('d','D',"decimal", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "decimal");
        }
        else if (STRCASEEQ('u','U',"upper-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "upper-alpha");
        }
        else if (STRCASEEQ('l','L',"lower-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "lower-alpha");
        }
      }
    }
  }
  W_L("<ol");
  if (attr_type) {
    W_L(" style=\"");
    W_L("list-style-type:");
    W_V(attr_type);
    W_L(";");
    W_L("\"");
  }
  if (attr_start) {
    W_L(" start=\"");
    W_V(attr_start);
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</ol>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_li_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_type  = NULL;
  char    *attr_value = NULL;
  char    *attr_style = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (*value == '1' || *value == 'a' || *value == 'A' || STRCASEEQ('d','D',"disc",value) || STRCASEEQ('s','S',"square",value) || STRCASEEQ('c','C',"circle",value))) {
        if (*value == '1') {
          attr_type = apr_pstrdup(doc->pool, "decimal");
        }
        else if (*value == 'a') {
          attr_type = apr_pstrdup(doc->pool, "lower-alpha");
        }
        else if (*value == 'A') {
          attr_type = apr_pstrdup(doc->pool, "upper-alpha");
        }
        else {
          attr_type = value;
        }
      }
    }
    else if (STRCASEEQ('v','V',"value", name) && value && *value) {
      attr_value = value;
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (STRCASEEQ('d','D',"decimal", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "decimal");
        }
        else if (STRCASEEQ('u','U',"upper-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "upper-alpha");
        }
        else if (STRCASEEQ('l','L',"lower-alpha", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "lower-alpha");
        }
        else if (STRCASEEQ('d','D',"disc", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "disc");
        }
        else if (STRCASEEQ('s','S',"square", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "square");
        }
        else if (STRCASEEQ('c','C',"circle", cur->value)) {
          attr_type = apr_pstrdup(doc->pool, "circle");
        }
      }
    }
  }


  W_L("<li");
  if (attr_type) {
    W_L(" style=\"");
    W_L("list-style-type:");
    W_V(attr_type);
    W_L(";");
    W_L("\"");
  }
  if (attr_value) {
    W_L(" value=\"");
    W_V(attr_value);
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 ** It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</li>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_img_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;
  request_rec   *r     = doc->r;
  Attr          *attr;
  char          *attr_src    = NULL;
  char          *attr_alt    = NULL;
  char          *attr_height = NULL;
  char          *attr_width  = NULL;
  char          *attr_align  = NULL;
  char          *attr_style  = NULL;
  char          *attr_hspace = NULL;
  char          *attr_vspace = NULL;

#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = xhtml->spec;
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"src",name)) {
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, xhtml->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
#ifdef IMG_NOT_CONVERT_FILENAME
      attr_src = value;

#else

      {
        attr_src = chxj_img_conv(r,spec,value);
      }

#endif
    }
    else 
    if (STRCASEEQ('a','A',"align",name)) {
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
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      attr_alt = value;
    }
    else if (STRCASEEQ('w','W',"width",name) && value && *value) {
      attr_width = value;
    }
    else if (STRCASEEQ('h','H',"height",name) && value && *value) {
      attr_height = value;
    }
    else if (STRCASEEQ('h','H',"hspace",name) && value && *value) {
      attr_hspace = value;
    }
    else if (STRCASEEQ('v','V',"vspace",name) && value && *value) {
      attr_vspace = value;
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_nopush_and_get_now_style(pdoc, node, attr_style);
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
  if (attr_alt) {
    W_L(" alt=\"");
    W_V(attr_alt);
    W_L("\"");
  }
  else {
    W_L(" alt=\"\"");
  }
  W_L(" />");
  return xhtml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_select_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml    = GET_XHTML(pdoc);
  Doc     *doc      = xhtml->doc;
  char    *size     = NULL;
  char    *name     = NULL;
  char    *multiple = NULL;
  Attr    *attr;
  char    *attr_style = NULL;

  W_L("<select");
  for (attr = qs_get_attr(doc,node);
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
    else if (STRCASEEQ('s','S',"style",nm) && val && *val) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple",nm)) {
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
    /* "true" is *NOT* W3C. it is specification of WAP2.0 for EZWEB */
    W_L(" multiple=\"true\"");
  }
  W_L(">");
  if (IS_CSS_ON(xhtml->entryp)) {
    s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</select>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_option_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;

  char *selected   = NULL;
  char *value      = NULL;

  W_L("<option");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"selected",nm)) {
      /* CHTML version 2.0 */
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('s','S',"style",nm) && val && *val) {
      /* CHTML version 2.0 */
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /* CHTML version 2.0 */
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected=\"selected\"");
  }
  W_L(">");
  if (IS_CSS_ON(xhtml->entryp)) {
    s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</option>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }

  return xhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_div_tag(void *pdoc, Node *node)
{
  xhtml_t     *xhtml = GET_XHTML(pdoc);
  Doc         *doc   = xhtml->doc;
  Attr        *attr;
  char        *attr_style             = NULL;
  char        *attr_align             = NULL;
  char        *attr_display           = NULL;
  char        *attr_decoration        = NULL;
  char        *attr_wap_marquee_style = NULL;
  char        *attr_wap_marquee_dir   = NULL;
  char        *attr_wap_marquee_loop  = NULL;
  char        *attr_color             = NULL;
  char        *attr_font_size         = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",nm)) {
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

  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *display_prop           = chxj_css_get_property_value(doc, style, "display");
      css_property_t *text_decoration_prop   = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *color_prop             = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_align_prop        = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *font_size_prop         = chxj_css_get_property_value(doc, style, "font-size");

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
      for (cur = font_size_prop->next; cur != font_size_prop; cur = cur->next) {
        if (   STRCASEEQ('x','X',"xx-small",cur->value)
            || STRCASEEQ('x','X',"x-small",cur->value)
            || STRCASEEQ('s','S',"small",cur->value)
            || STRCASEEQ('m','M',"medium",cur->value)
            || STRCASEEQ('l','L',"large",cur->value)
            || STRCASEEQ('x','X',"x-large",cur->value)
            || STRCASEEQ('x','X',"xx-large",cur->value)) {
          attr_font_size = apr_pstrdup(doc->pool, cur->value);
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
            attr_wap_marquee_dir = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('r','R',"rtl",cur->value)) {
            attr_wap_marquee_dir = apr_pstrdup(doc->pool, cur->value);
          }
        }
        for (cur = wap_marquee_loop_prop->next; cur != wap_marquee_loop_prop; cur = cur->next) {
          attr_wap_marquee_loop = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }  
  W_L("<div");
  if (attr_align || attr_display || attr_decoration || attr_wap_marquee_style || attr_wap_marquee_dir || attr_wap_marquee_loop || attr_color || attr_font_size) {
    W_L(" style=\"");
    if (attr_align) {
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if (attr_display) {
      W_L("display:");
      W_V(attr_display);
      W_L(";");
    }
    if (attr_decoration) {
      W_L("text-decoration:");
      W_V(attr_decoration);
      W_L(";");
    }
    if (attr_wap_marquee_style) {
      W_L("-wap-marquee-style:");
      W_V(attr_wap_marquee_style);
      W_L(";");
    }
    if (attr_wap_marquee_dir) {
      W_L("-wap-marquee-dir:");
      W_V(attr_wap_marquee_dir);
      W_L(";");
    }
    if (attr_wap_marquee_loop) {
      W_L("-wap-marquee-loop:");
      W_V(attr_wap_marquee_loop);
      W_L(";");
    }
    if (attr_color) {
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_font_size) {
      W_L("font-size:");
      W_V(attr_font_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</div>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_b_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<div style=\"font-weight:bold\">");
  return xhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_b_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</div>");
  return xhtml->out;
}


/**
 * It is a handler who processes the CHXJ:IF tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CHXJ:IF tag node is specified.
 */
static char *
s_xhtml_1_0_chxjif_tag(void *pdoc, Node *node)
{
  xhtml_t      *xhtml = GET_XHTML(pdoc);
  Doc          *doc   = xhtml->doc;
  Node         *child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_xhtml_1_0_chxjif_tag(xhtml, child);
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
s_xhtml_1_0_start_textarea_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *attr_accesskey = NULL;
  char    *attr_name      = NULL;
  char    *attr_rows      = NULL;
  char    *attr_cols      = NULL;
  char    *attr_istyle    = NULL;
  char    *attr_style     = NULL;

  xhtml->textarea_flag++;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name) && value && *value) {
      attr_name = value;
    }
    else if (STRCASEEQ('r','R',"rows",name) && value && *value) {
      attr_rows = value;
    }
    else if (STRCASEEQ('c','C',"cols",name) && value && *value) {
      attr_cols = value;
    }
    else if (STRCASEEQ('i','I',"istyle", name) && value && (*value == '1' || *value == '2' || *value == '3' || *value == '4')) {
      attr_istyle = value;
#if 0
      char *fmt = qs_conv_istyle_to_format(doc->r,value);
      W_L(" FORMAT=\"*");
      W_V(fmt);
      W_L("\"");
#endif
    }
    else if (STRCASEEQ('a','A',"accesskey",name) && value && *value != 0) {
      attr_accesskey = value;
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value != 0) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_nopush_and_get_now_style(pdoc, node, attr_style);
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
    char *fmt = qs_conv_istyle_to_format(doc->r, attr_istyle);
    W_L(" FORMAT=\"*");
    W_V(fmt);
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
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
s_xhtml_1_0_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</textarea>");
  xhtml->textarea_flag--;

  return xhtml->out;
}

static char *
s_xhtml_1_0_text_tag(void *pdoc, Node *child)
{
  xhtml_t     *xhtml   = GET_XHTML(pdoc);
  Doc         *doc     = xhtml->doc;
  request_rec *r       = doc->r;
  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[2];
  int         ii;
  int         tdst_len;
  
  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return xhtml->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char *out;
    int rtn = s_xhtml_search_emoji(xhtml, &textval[ii], &out);
    if (rtn != 0) {
      DBG(r,"[%s][%d]", out, rtn);
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
    else if (xhtml->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (xhtml->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  W_V(tdst);
  return xhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_blockquote_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml;
  Doc      *doc;
  Attr     *attr;
  char     *attr_style = NULL;
  char     *attr_color = NULL;
  char     *attr_size  = NULL;

  xhtml  = GET_XHTML(pdoc);
  doc     = xhtml->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *font_size_prop = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = font_size_prop->next; cur != font_size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<blockquote");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</blockquote>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dir_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc       *doc        = xhtml->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_type  = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        attr_type = value;
      }
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_type = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<dir");
  if (attr_type || attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_type) {
      W_L("list-style-type:");
      W_V(attr_type);
      W_L(";");
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</dir>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dl_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc     *doc        = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<dl");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  W_L("</dl>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dt_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc     *doc        = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<dt");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</dt>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handder who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dd_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc     *doc        = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<dd");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</dd>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_menu_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc       *doc        = xhtml->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_type  = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        attr_type = value;
      }
    }
    else if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "list-style-type");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = list_style_type_prop->next; cur != list_style_type_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_type = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<menu");
  if (attr_type || attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_type) {
      W_L("list-style-type:");
      W_V(attr_type);
      W_L(";");
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a hanmenuer who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</menu>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_plaintext_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  W_L("<plaintext>");
  s_xhtml_1_0_start_plaintext_tag_inner(pdoc,node);
  return xhtml->out;
}

static char *
s_xhtml_1_0_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  Node    *child;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_xhtml_1_0_start_plaintext_tag_inner(pdoc, child);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  W_L("</plaintext>");
  return xhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_blink_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml      = GET_XHTML(pdoc);
  Doc     *doc        = xhtml->doc;
  Attr    *attr;
  char    *attr_style = NULL;
  char    *attr_color = NULL;
  char    *attr_size  = NULL;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop           = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop            = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<blink");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_blink_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</blink>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_marquee_tag(void *pdoc, Node *node)
{
  xhtml_t   *xhtml = GET_XHTML(pdoc);
  Doc       *doc = xhtml->doc;
  Attr      *attr;
  char      *attr_direction = NULL;
  char      *attr_behavior  = NULL;
  char      *attr_loop      = NULL;
  char      *attr_style     = NULL;
  char      *attr_color     = NULL;
  char      *attr_size      = NULL;
  char      *attr_bgcolor   = NULL;
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('d','D',"direction", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          attr_direction = "rtl";
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          attr_direction = "ltr";
        }
      }
    }
    else if (STRCASEEQ('b','B',"behavior",name) && value && *value) {
      if (STRCASEEQ('s','S',"scroll",value) || STRCASEEQ('s','S',"slide",value) || STRCASEEQ('a','A',"alternate",value)) {
        attr_behavior = value;
      }
    }
    else if (STRCASEEQ('l','L',"loop",name) && value && *value) {
      attr_loop = value;
    }
    else if (STRCASEEQ('b','B',"bgcolor",name)) {
      if (value && *value) {
        attr_bgcolor = value;
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop  = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *bgcolor_prop  = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *direction_prop  = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
      css_property_t *behavior_prop  = chxj_css_get_property_value(doc, style, "-wap-marquee-style");
      css_property_t *loop_prop  = chxj_css_get_property_value(doc, style, "-wap-marquee-loop");
      css_property_t *cur;
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_color = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = direction_prop->next; cur != direction_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_direction = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = behavior_prop->next; cur != behavior_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('s','S',"scroll",cur->value) || STRCASEEQ('s','S',"slide",cur->value) || STRCASEEQ('a','A',"alternate",cur->value)) {
            attr_behavior = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
      for (cur = loop_prop->next; cur != loop_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          attr_loop = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if (STRCASEEQ('x','X',"xx-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('s','S',"small",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('m','M',"medium",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('l','L',"large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"x-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
          else if (STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<marquee");
  if (attr_color || attr_size || attr_direction || attr_bgcolor || attr_behavior || attr_loop) {
    W_L(" style=\"");
    if (attr_direction) {
      W_L("-wap-marquee-dir:");
      W_V(attr_direction);
      W_L(";");
    }
    if (attr_behavior) {
      W_L("-wap-marquee-style:");
      W_V(attr_behavior);
      W_L(";");
    }
    if (attr_loop) {
      W_L("-wap-marquee-loop:");
      W_V(attr_loop);
      W_L(";");
    }
    if (attr_bgcolor) {
      attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      W_L("background-color:");
      W_V(attr_bgcolor);
      W_L(";");
    }
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return xhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_marquee_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</marquee>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}


/**
 * It is handler who processes the New Line Code.
 */
static char *
s_xhtml_1_0_newline_mark(void *pdoc, Node *UNUSED(node))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  W_NLCODE();
  return xhtml->out;
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
s_xhtml_1_0_link_tag(void *pdoc, Node *node)
{
  xhtml_t       *xhtml;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  xhtml = GET_XHTML(pdoc);
  doc   = xhtml->doc;

  if (! IS_CSS_ON(xhtml->entryp)) {
    return xhtml->out;
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
    xhtml->style = chxj_css_parse_from_uri(doc->r, doc->pool, xhtml->style, href);
    DBG(doc->r, "end load CSS. url:[%s]", href);
  }

  return xhtml->out;
}

static css_prop_list_t *
s_xhtml_1_0_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(xhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, xhtml->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(xhtml->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(xhtml->css_prop_stack);

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
s_xhtml_1_0_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(xhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, xhtml->style, node);
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
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_span_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml;
  Doc *doc;
  Attr *attr;
  char *attr_style = NULL;
  char *attr_color = NULL;
  char *attr_size = NULL;
  char *attr_align = NULL;
  char *attr_blink = NULL;
  char *attr_marquee = NULL;
  char *attr_marquee_dir = NULL;
  char *attr_marquee_style = NULL;
  char *attr_marquee_loop = NULL;

  xhtml = GET_XHTML(pdoc);
  doc     = xhtml->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(xhtml->entryp)) {
    css_prop_list_t *style = s_xhtml_1_0_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop = chxj_css_get_property_value(doc, style, "font-size");
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
      for (cur = size_prop->next; cur != size_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if ( STRCASEEQ('x','X',"xx-small",cur->value)
            || STRCASEEQ('x','X',"x-small", cur->value)
            || STRCASEEQ('s','S',"small",   cur->value)
            || STRCASEEQ('m','M',"medium",  cur->value)
            || STRCASEEQ('l','L',"large",   cur->value)
            || STRCASEEQ('x','X',"x-large", cur->value)
            || STRCASEEQ('x','X',"xx-large",cur->value)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
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
          if ( STRCASEEQ('l','L',"ltr",cur->value)
            || STRCASEEQ('r','R',"rtl",cur->value)) {
            attr_marquee_dir = apr_pstrdup(doc->pool, cur->value);
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
          attr_marquee_loop = apr_pstrdup(doc->pool, cur->value);
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

  W_L("<span");
  if (attr_color || attr_size || attr_align || attr_blink || attr_marquee) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    if (attr_align) {
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if (attr_blink) {
      W_L("text-decoration:");
      W_V("blink");
      W_L(";");
    }
    if (attr_marquee) {
      W_L("display:-wap-marquee;");
      if (attr_marquee_dir) {
        W_L("-wap-marquee-dir:");
        W_V(attr_marquee_dir);
        W_L(";");
      }
      if (attr_marquee_style) {
        W_L("-wap-marquee-style:");
        W_V(attr_marquee_style);
        W_L(";");
      }
      if (attr_marquee_loop) {
        W_L("-wap-marquee-loop:");
        W_V(attr_marquee_loop);
        W_L(";");
      }
    }
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the SPAN tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_span_tag(void *pdoc, Node *UNUSED(node))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;

  W_L("</span>");
  if (IS_CSS_ON(xhtml->entryp)) {
    chxj_css_pop_prop_list(xhtml->css_prop_stack);
  }
  return xhtml->out;
}
/*
 * vim:ts=2 et
 */
