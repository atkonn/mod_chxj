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
#include "chxj_ixhtml10.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"


#define GET_IXHTML10(X) ((ixhtml10_t *)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { ixhtml10->out = BUFFERED_WRITE_LITERAL(ixhtml10->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { ixhtml10->out = (X) ? BUFFERED_WRITE_VALUE(ixhtml10->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(ixhtml10->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(ixhtml10->conf); W_V(nlcode); } while (0)

static char *s_ixhtml10_start_html_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_html_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_meta_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_meta_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_head_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_head_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_title_tag    (void *pdoc, Node *node);
static char *s_ixhtml10_end_title_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_start_base_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_base_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_body_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_body_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_a_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_end_a_tag          (void *pdoc, Node *node);
static char *s_ixhtml10_start_pre_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_end_pre_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_start_p_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_end_p_tag          (void *pdoc, Node *node);
static char *s_ixhtml10_start_ul_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_ul_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_ol_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_ol_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_li_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_li_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_br_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_br_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_tr_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_tr_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_font_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_font_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_form_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_form_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_input_tag    (void *pdoc, Node *node);
static char *s_ixhtml10_end_input_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_start_center_tag   (void *pdoc, Node *node);
static char *s_ixhtml10_end_center_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_start_hr_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_hr_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_img_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_end_img_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_start_select_tag   (void *pdoc, Node *node);
static char *s_ixhtml10_end_select_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_start_option_tag   (void *pdoc, Node *node);
static char *s_ixhtml10_end_option_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_start_div_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_end_div_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_start_textarea_tag (void *pdoc, Node *node);
static char *s_ixhtml10_end_textarea_tag   (void *pdoc, Node *node);
static char *s_ixhtml10_start_b_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_end_b_tag          (void *pdoc, Node *node);
static char *s_ixhtml10_chxjif_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_text_tag           (void *pdoc, Node *node);
static char *s_ixhtml10_start_blockquote_tag (void *pdoc, Node *node);
static char *s_ixhtml10_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_ixhtml10_start_dir_tag      (void *pdoc, Node *node);
static char *s_ixhtml10_end_dir_tag        (void *pdoc, Node *node);
static char *s_ixhtml10_start_dl_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_dl_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_dt_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_dt_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_dd_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_dd_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h1_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h1_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h2_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h2_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h3_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h3_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h4_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h4_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h5_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h5_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_h6_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_end_h6_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_menu_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_menu_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_ixhtml10_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_ixhtml10_start_blink_tag  (void *pdoc, Node *node);
static char *s_ixhtml10_end_blink_tag    (void *pdoc, Node *node);
static char *s_ixhtml10_start_marquee_tag (void *pdoc, Node *node);
static char *s_ixhtml10_end_marquee_tag  (void *pdoc, Node *node);
static char *s_ixhtml10_newline_mark       (void *pdoc, Node *node);
static char *s_ixhtml10_link_tag           (void *pdoc, Node *node);
static char *s_ixhtml10_start_span_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_span_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_style_tag       (void *pdoc, Node *node);
static char *s_ixhtml10_create_style_data(apr_pool_t *pool, const char *style_data);
static char *s_ixhtml10_start_nobr_tag     (void *pdoc, Node *node);
static char *s_ixhtml10_end_nobr_tag       (void *pdoc, Node *node);

static void  s_init_ixhtml10(ixhtml10_t *ixhtml10, Doc *doc, request_rec *r, device_table *spec);

static int   s_ixhtml10_search_emoji(ixhtml10_t *ixhtml10, char *txt, char **rslt);

static char *s_ixhtml10_istyle_to_mode(apr_pool_t *p, const char *s);
static char *s_ixhtml10_istyle_to_wap_input_format(apr_pool_t *p, const char *s);
static css_prop_list_t *s_ixhtml10_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_ixhtml10_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);


/*pend*/

tag_handler ixhtml10_handler[] = {
  /* tagHTML */
  {
    s_ixhtml10_start_html_tag,
    s_ixhtml10_end_html_tag,
  },
  /* tagMETA */
  {
    s_ixhtml10_start_meta_tag,
    s_ixhtml10_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_ixhtml10_start_textarea_tag,
    s_ixhtml10_end_textarea_tag,
  },
  /* tagP */
  {
    s_ixhtml10_start_p_tag,
    s_ixhtml10_end_p_tag,
  },
  /* tagPRE */
  {
    s_ixhtml10_start_pre_tag,
    s_ixhtml10_end_pre_tag,
  },
  /* tagUL */
  {
    s_ixhtml10_start_ul_tag,
    s_ixhtml10_end_ul_tag,
  },
  /* tagLI */
  {
    s_ixhtml10_start_li_tag,
    s_ixhtml10_end_li_tag,
  },
  /* tagOL */
  {
    s_ixhtml10_start_ol_tag,
    s_ixhtml10_end_ol_tag,
  },
  /* tagH1 */
  {
    s_ixhtml10_start_h1_tag,
    s_ixhtml10_end_h1_tag,
  },
  /* tagH2 */
  {
    s_ixhtml10_start_h2_tag,
    s_ixhtml10_end_h2_tag,
  },
  /* tagH3 */
  {
    s_ixhtml10_start_h3_tag,
    s_ixhtml10_end_h3_tag,
  },
  /* tagH4 */
  {
    s_ixhtml10_start_h4_tag,
    s_ixhtml10_end_h4_tag,
  },
  /* tagH5 */
  {
    s_ixhtml10_start_h5_tag,
    s_ixhtml10_end_h5_tag,
  },
  /* tagH6 */
  {
    s_ixhtml10_start_h6_tag,
    s_ixhtml10_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_ixhtml10_start_head_tag,
    s_ixhtml10_end_head_tag,
  },
  /* tagTITLE */
  {
    s_ixhtml10_start_title_tag,
    s_ixhtml10_end_title_tag,
  },
  /* tagBASE */
  {
    s_ixhtml10_start_base_tag,
    s_ixhtml10_end_base_tag,
  },
  /* tagBODY */
  {
    s_ixhtml10_start_body_tag,
    s_ixhtml10_end_body_tag,
  },
  /* tagA */
  {
    s_ixhtml10_start_a_tag,
    s_ixhtml10_end_a_tag,
  },
  /* tagBR */
  {
    s_ixhtml10_start_br_tag,
    s_ixhtml10_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_ixhtml10_start_tr_tag,
    s_ixhtml10_end_tr_tag,
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
    s_ixhtml10_start_font_tag,
    s_ixhtml10_end_font_tag,
  },
  /* tagFORM */
  {
    s_ixhtml10_start_form_tag,
    s_ixhtml10_end_form_tag,
  },
  /* tagINPUT */
  {
    s_ixhtml10_start_input_tag,
    s_ixhtml10_end_input_tag,
  },
  /* tagCENTER */
  {
    s_ixhtml10_start_center_tag,
    s_ixhtml10_end_center_tag,
  },
  /* tagHR */
  {
    s_ixhtml10_start_hr_tag,
    s_ixhtml10_end_hr_tag,
  },
  /* tagIMG */
  {
    s_ixhtml10_start_img_tag,
    s_ixhtml10_end_img_tag,
  },
  /* tagSELECT */
  {
    s_ixhtml10_start_select_tag,
    s_ixhtml10_end_select_tag,
  },
  /* tagOPTION */
  {
    s_ixhtml10_start_option_tag,
    s_ixhtml10_end_option_tag,
  },
  /* tagDIV */
  {
    s_ixhtml10_start_div_tag,
    s_ixhtml10_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_ixhtml10_chxjif_tag,
    NULL,
  },
  /* tagNOBR */
  {
    s_ixhtml10_start_nobr_tag,
    s_ixhtml10_end_nobr_tag,
  },
  /* tagSMALL */
  {
    NULL,
    NULL,
  },
  /* tagSTYLE */
  {
    s_ixhtml10_style_tag,
    NULL,
  },
  /* tagSPAN */
  {
    s_ixhtml10_start_span_tag,
    s_ixhtml10_end_span_tag,
  },
  /* tagTEXT */
  {
    s_ixhtml10_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_ixhtml10_start_b_tag,
    s_ixhtml10_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_ixhtml10_start_dt_tag,
    s_ixhtml10_end_dt_tag,
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
    s_ixhtml10_start_blockquote_tag,
    s_ixhtml10_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_ixhtml10_start_dir_tag,
    s_ixhtml10_end_dir_tag,
  },
  /* tagDL */
  {
    s_ixhtml10_start_dl_tag,
    s_ixhtml10_end_dl_tag,
  },
  /* tagDD */
  {
    s_ixhtml10_start_dd_tag,
    s_ixhtml10_end_dd_tag,
  },
  /* tagMENU */
  {
    s_ixhtml10_start_menu_tag,
    s_ixhtml10_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_ixhtml10_start_plaintext_tag,
    s_ixhtml10_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_ixhtml10_start_blink_tag,
    s_ixhtml10_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_ixhtml10_start_marquee_tag,
    s_ixhtml10_end_marquee_tag,
  },
  /* tagLINK */
  {
    s_ixhtml10_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_ixhtml10_newline_mark,
    NULL,
  },
};


/**
 * converts from CHTML5.0 to IXHTML10.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_ixhtml10(
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
  ixhtml10_t   ixhtml10;
  Doc       doc;

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
  s_init_ixhtml10(&ixhtml10, &doc, r, spec);

  ixhtml10.entryp = entryp;
  ixhtml10.cookie = cookie;

  chxj_set_content_type(r, "application/xhtml+xml; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);

  memset(ss,   0, srclen + 1);
  memcpy(ss, src, srclen);

  if (IS_CSS_ON(ixhtml10.entryp)) {
    /* current property list */
    ixhtml10.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> IXHTML10", ss, srclen);
#endif

  qs_parse_string(&doc,ss,strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to IXHTML10.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void*)&ixhtml10, &doc, qs_get_root(&doc), 0);
  ixhtml10.out = chxj_buffered_write_flush(ixhtml10.out, &doc.buf);
  dst = apr_pstrcat(r->pool, (! ixhtml10.prev_style_data) ? "" : ixhtml10.prev_style_data,
                             s_ixhtml10_create_style_data(doc.pool, ixhtml10.style_data),
                             ixhtml10.out, NULL);
  chxj_buffered_write_terminate(&doc.buf);


  qs_all_free(&doc,QX_LOGMARK);

  if (! dst)
    return apr_pstrdup(r->pool,ss);

  if (! strlen(dst))
    dst = apr_psprintf(r->pool, "\n");

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> IXHTML10", dst, *dstlen);
#endif

  return dst;
}


/**
 * The IXHTML10 structure is initialized.
 *
 * @param ixhtml10 [i/o] The pointer to the IXHTML10 structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   IXHTML10 structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_ixhtml10(ixhtml10_t *ixhtml10, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(ixhtml10, 0, sizeof(ixhtml10_t));

  doc->r      = r;
  ixhtml10->doc  = doc;
  ixhtml10->spec = spec;
  ixhtml10->out  = qs_alloc_zero_byte_string(r->pool);
  ixhtml10->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  ixhtml10->doc->parse_mode = PARSE_MODE_CHTML;
  ixhtml10->prev_style_data = NULL;
  ixhtml10->style_data = NULL;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved.
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param ixhtml10   [i]   The pointer to the IXHTML10 structure is specified.
 * @param txt     [i]   The character string to want to examine whether it is
 *                      EMOJI is specified.
 * @param rslt    [o]   The pointer to the pointer that stores the result is
 *                      specified.
 * @return When corresponding EMOJI exists, it returns it excluding 0.
 */
static int
s_ixhtml10_search_emoji(ixhtml10_t *ixhtml10, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = ixhtml10->spec;

  len = strlen(txt);
  r   = ixhtml10->doc->r;

  if (!spec) {
    DBG(r,"spec is NULL");
  }

  for (ee = ixhtml10->conf->emoji;
       ee;
       ee = ee->next) {
    if (ee->imode == NULL) {
      DBG(r, "emoji->imode is NULL");
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
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_html_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t       *ixhtml10;
  Doc           *doc;
  request_rec   *r;


  ixhtml10  = GET_IXHTML10(pdoc);
  doc    = ixhtml10->doc;
  r      = doc->r;
  DBG(r, "start s_ixhtml10_start_html_tag()");

  W_L("<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>");
  W_NLCODE();
  W_L("<!DOCTYPE html PUBLIC \"-//i-mode group (ja)//DTD XHTML i-XHTML(Locale/Ver.=ja/1.0) 1.0//EN\" \"i-xhtml_4ja_10.dtd\">");
  W_NLCODE();

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html xmlns=\"http://www.w3.org/1999/xhtml\">");

  DBG(r, "end s_ixhtml10_start_html_tag()");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_html_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t      *ixhtml10 = GET_IXHTML10(pdoc);
  Doc           *doc = ixhtml10->doc;

  W_L("</html>");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_meta_tag(void *pdoc, Node *node)
{
  ixhtml10_t   *ixhtml10;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  int          content_type_flag;
  int          refresh_flag;

  ixhtml10          = GET_IXHTML10(pdoc);
  doc               = ixhtml10->doc;
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
          W_L("application/xhtml+xml; charset=Shift_JIS");
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
  W_L(" />");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_meta_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);

  return ixhtml10->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_head_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("<head>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_head_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t       *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  ixhtml10->out = chxj_buffered_write_flush(ixhtml10->out, &doc->buf);
  ixhtml10->prev_style_data = apr_pstrdup(doc->pool, ixhtml10->out);
  ixhtml10->out = qs_alloc_zero_byte_string(r->pool);

  W_L("</head>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_title_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t      *ixhtml10;
  Doc          *doc;
  request_rec  *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("<title>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_title_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</title>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_base_tag(void *pdoc, Node *node)
{
  ixhtml10_t      *ixhtml10;
  Attr          *attr;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
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
  W_L(" />");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_base_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_body_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_bgcolor = NULL;
  char        *attr_text    = NULL;
  char        *attr_link    = NULL;
  char        *attr_vlink   = NULL;
  char        *attr_alink   = NULL;
  char        *attr_style   = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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
      attr_bgcolor = value;
    }
    else if (STRCASEEQ('t','T',"text",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_text = value;
    }
    else if (STRCASEEQ('l','L',"link",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_link = value;
    }
    else if (STRCASEEQ('a','A',"alink",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_alink = value;
    }
    else if (STRCASEEQ('v','V',"vlink",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_vlink = value;
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
    if (ixhtml10->style) {
      css_stylesheet_t *pseudos = chxj_find_pseudo_selectors(doc, ixhtml10->style);
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
        else if (cur_sel->name && strcasecmp(cur_sel->name, "a:visited") == 0) {
          css_property_t *cur;
          for (cur = cur_sel->property_head.next; cur != &cur_sel->property_head; cur = cur->next) {
            if (cur->name && strcasecmp(cur->name, "color") == 0) {
              attr_vlink = apr_pstrdup(doc->pool, cur->value);
            }
          }
        }
        else if (cur_sel->name && strcasecmp(cur_sel->name, "a:focus") == 0) {
          css_property_t *cur;
          for (cur = cur_sel->property_head.next; cur != &cur_sel->property_head; cur = cur->next) {
            if (cur->name && strcasecmp(cur->name, "color") == 0) {
              attr_alink = apr_pstrdup(doc->pool, cur->value);
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
  W_L(">");
  char *style_data = NULL;
  if (attr_link) {
    attr_link = chxj_css_rgb_func_to_value(doc->pool, attr_link);
    style_data = apr_psprintf(doc->pool, "a:link { color:%s; }", attr_link);
  }
  if (attr_vlink) {
    attr_vlink = chxj_css_rgb_func_to_value(doc->pool, attr_vlink);
    style_data = apr_pstrcat(doc->pool, (style_data) ? style_data : "",
                                        apr_psprintf(doc->pool, "a:visited { color:%s; }", attr_vlink), NULL);
  }
  if (attr_alink) {
    attr_alink = chxj_css_rgb_func_to_value(doc->pool, attr_alink);
    style_data = apr_pstrcat(doc->pool, (style_data) ? style_data : "",
                                        apr_psprintf(doc->pool, "a:focus { color:%s; }", attr_alink), NULL);
  }
  if (style_data) {
    ixhtml10->style_data = apr_pstrcat(doc->pool, (ixhtml10->style_data) ? ixhtml10->style_data : "",
                                                  style_data,
                                                  NULL);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_body_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t       *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</body>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_a_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
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
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_style = value;
    }
  }
  W_L(">");

  if (IS_CSS_ON(ixhtml10->entryp)) {
    s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_a_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t      *ixhtml10;
  Doc          *doc;
  request_rec  *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</a>");

  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_br_tag(void *pdoc, Node *node)
{
  ixhtml10_t     *ixhtml10;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  char         *attr_style = NULL;
  char         *attr_clear = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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
        attr_clear = value;
      }
    }
    else if (STRCASEEQ('s','S',"style",name)) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *clear_prop = chxj_css_get_property_value(doc, style, "clear");
      css_property_t *cur;
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          if ( STRCASEEQ('l','L',"left",  cur->value)
            || STRCASEEQ('r','R',"right", cur->value)
            || STRCASEEQ('b','B',"both"  ,cur->value)) {
            attr_clear = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<br");
  if (attr_clear) {
    W_L(" style=\"");
    W_L("clear:");
    if (STRCASEEQ('a','A',"all",attr_clear)) {
      W_L("both");
    }
    else {
      W_V(attr_clear);
    }
    W_L(";");
    W_L("\"");
  }
  W_L(" />");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_br_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_tr_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t      *ixhtml10;
  Doc          *doc;
  request_rec  *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("<br />");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_tr_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_font_tag(void *pdoc, Node *node)
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_color = NULL;
  char          *attr_size  = NULL;
  char          *attr_style = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
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
      attr_color = apr_pstrdup(doc->buf.pool, value);
    }
    else if (STRCASEEQ('s','S',"size",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      switch (*value) {
      case '1':
        attr_size = "xx-small";
        break;
      case '2':
        attr_size = "x-small";
        break;
      case '3':
        attr_size = "small";
        break;
      case '4':
        attr_size = "medium";
        break;
      case '5':
        attr_size = "large";
        break;
      case '6':
        attr_size = "x-large";
        break;
      case '7':
        attr_size = "xx-large";
        break;
      case '+':
      case '-':
        if (chxj_chk_numeric(&value[1]) == 0) {
          attr_size = apr_pstrdup(doc->pool, value);
        }
        break;
      default:
        break;
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = apr_pstrdup(doc->buf.pool, value);
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
          if ( STRCASEEQ('x','X',"xx-small",attr_size)
            || STRCASEEQ('x','X',"x-small", attr_size)
            || STRCASEEQ('s','S',"small",   attr_size)
            || STRCASEEQ('m','M',"medium",  attr_size)
            || STRCASEEQ('l','L',"large",   attr_size)
            || STRCASEEQ('x','X',"x-large", attr_size)
            || STRCASEEQ('x','X',"xx-large",attr_size)) {
            attr_size = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
    }
  }
  W_L("<span");
  if (attr_color || attr_size) {
    W_L(" style=\"");
    if (attr_color) {
      attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
      W_L("color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_size) {
      switch(*attr_size) {
      case '-':
        if (*(attr_size + 1) == '1') {
          attr_size = "small";
          break;
        }
        if (*(attr_size + 1) == '2') {
          attr_size = "x-small";
          break;
        }
        if (*(attr_size + 1) == '3') {
          attr_size = "xx-small";
          break;
        }
        break;
      case '+':
        if (*(attr_size + 1) == '1') {
          attr_size = "large";
          break;
        }
        if (*(attr_size + 1) == '2') {
          attr_size = "x-large";
          break;
        }
        if (*(attr_size + 1) == '3') {
          attr_size = "xx-large";
          break;
        }
        break;
      default:
        break;
      }
      W_L("font-size:");
      W_V(attr_size);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_font_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t      *ixhtml10;
  request_rec  *r;
  Doc          *doc;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = ixhtml10->doc->r;

  W_L("</span>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_form_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_action = NULL;
  char        *attr_method = NULL;
  char        *attr_style  = NULL;
  char        *attr_color  = NULL;
  char        *attr_align  = NULL;
  char        *attr_name   = NULL;
  char        *new_hidden_tag = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
    attr_action = chxj_encoding_parameter(r, attr_action);
    attr_action = chxj_add_cookie_parameter(r, attr_action, ixhtml10->cookie);
    char *q;
    q = strchr(attr_action, '?');
    if (q) {
      new_hidden_tag = chxj_form_action_to_hidden_tag(r, doc->pool, attr_action, 1, post_flag);
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

  ixhtml10_flags_t *flg = (ixhtml10_flags_t *)apr_palloc(doc->pool, sizeof(ixhtml10_flags_t));
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_form_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc    = ixhtml10->doc;

  ixhtml10_flags_t *flg = (ixhtml10_flags_t *)node->userData;
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</form>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_input_tag(void *pdoc, Node *node)
{
  ixhtml10_t     *ixhtml10;
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

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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

  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_nopush_and_get_now_style(pdoc, node, attr_style);
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
    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool, attr_value));
    W_L("\"");
  }
  if (attr_accesskey) {
    W_L(" accesskey=\"");
    W_V(attr_accesskey);
    W_L("\"");
  }
  if (attr_istyle && (*attr_istyle == '1' || *attr_istyle == '2' || *attr_istyle == '3' || *attr_istyle == '4')) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (attr_type && STRCASEEQ('p','P',"password", attr_type) && ! ixhtml10->entryp->pc_flag ) {
      W_L(" style=\"-wap-input-format:&quot;*&lt;ja:n&gt;&quot;;\"");
    }
    else {
      char *vv = s_ixhtml10_istyle_to_wap_input_format(doc->buf.pool,attr_istyle);
      W_L(" style=\"");
      W_L("-wap-input-format:");
      W_V(vv);
      W_L(";");
      W_L("\"");
    }
  }
  else if (attr_type && STRCASEEQ('p','P',"password",attr_type)) {
    W_L(" style=\"-wap-input-format:&quot;*&lt;ja:n&gt;&quot;;\"");
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (attr_max_length && *attr_max_length) {
    if (chxj_chk_numeric(attr_max_length) == 0) {
      W_L(" maxlength=\"");
      W_V(attr_max_length);
      W_L("\"");
    }
  }
  if (attr_checked) {
    W_L(" checked=\"checked\"");
  }
  W_L(" />");
#if 0
  ixhtml10_t       *ixhtml10;
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

  ixhtml10       = GET_IXHTML10(pdoc);
  doc         = ixhtml10->doc;
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
  value      = qs_get_value_attr(doc,node, doc->buf.pool);
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
    if (type && STRCASEEQ('p','P',"password", type) && ! ixhtml10->entryp->pc_flag ) {
      W_L(" style=\"-wap-input-format: &quot;*&lt;ja:n&gt;&quot;;\"");
    }
    else {
      char *vv = s_ixhtml10_istyle_to_wap_input_format(doc->buf.pool,istyle);
      W_L(" style=\"");
      W_L("-wap-input-format: ");
      W_V(vv);
      W_L(";");
      W_L("\"");
    }
  }
  else if (type && STRCASEEQ('p','P',"password",type)) {
    W_L(" style=\"-wap-input-format: &quot;*&lt;ja:n&gt;&quot;;\"");
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
    W_L(" checked=\"checked\"");
  }
  W_L(" />");
#endif
  return ixhtml10->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_input_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_center_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc    = ixhtml10->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  W_L("<div");
  W_L(" style=\"");
  W_L("text-align:center;");
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
  W_L(">");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_center_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc    = ixhtml10->doc;
  r      = doc->r;

  W_L("</div>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_li_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_type  = NULL;
  char        *attr_value = NULL;
  char        *attr_style = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_li_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t     *ixhtml10;
  Doc         *doc;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  W_L("</li>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_ol_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_start = NULL;
  char        *attr_type  = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_ol_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t     *ixhtml10;
  Doc         *doc;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</ol>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_p_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align = NULL;
  char        *attr_style = NULL;
  char        *attr_color = NULL;
  char        *attr_blink = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_p_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t  *ixhtml10 = GET_IXHTML10(pdoc);
  Doc       *doc    = ixhtml10->doc;

  W_L("</p>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_pre_tag(void *pdoc, Node *node)
{
  ixhtml10_t  *ixhtml10 = GET_IXHTML10(pdoc);
  Doc       *doc   = ixhtml10->doc;
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

  if (IS_CSS_ON(ixhtml10->entryp)) {
    s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  ixhtml10->pre_flag++;
  W_L("<pre>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_pre_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc     *doc   = ixhtml10->doc;

  W_L("</pre>");
  ixhtml10->pre_flag--;
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_ul_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc    = ixhtml10->doc;
  Attr     *attr;
  char     *attr_type = NULL;
  char     *attr_style = NULL;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_ul_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc     *doc   = ixhtml10->doc;

  W_L("</ul>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_hr_tag(void *pdoc, Node *node)
{
  Attr        *attr;
  ixhtml10_t     *ixhtml10;
  Doc         *doc;
  request_rec *r;
  char        *attr_align   = NULL;
  char        *attr_size    = NULL;
  char        *attr_width   = NULL;
  char        *attr_noshade = NULL;
  char        *attr_style   = NULL;
  char        *attr_color   = NULL;
  char        *attr_bgcolor = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
        attr_color   = value;
        attr_bgcolor = value;
      }
      break;

    default:
      break;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *border_style_prop = chxj_css_get_property_value(doc, style, "border-style");
      css_property_t *height_prop       = chxj_css_get_property_value(doc, style, "height");
      css_property_t *width_prop        = chxj_css_get_property_value(doc, style, "width");
      css_property_t *color_prop        = chxj_css_get_property_value(doc, style, "border-color");
      css_property_t *bgcolor_prop      = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *cur;
      for (cur = border_style_prop->next; cur != border_style_prop; cur = cur->next) {
        if (STRCASEEQ('s','S',"solid",cur->value)) {
          attr_noshade = "noshade";
        }
      }
      for (cur = color_prop->next; cur != color_prop; cur = cur->next) {
        attr_color = apr_pstrdup(doc->pool, cur->value);
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
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
  if (attr_align || attr_size || attr_width || attr_noshade || attr_color) {
    W_L(" style=\"");
    if (attr_align) {
      W_L("float:");
      if (STRCASEEQ('c','C',"center",attr_align)) {
        W_L("none");
      }
      else {
        W_V(attr_align);
      }
      W_L(";");
    }
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
    if (attr_color) {
      W_L("border-color:");
      W_V(attr_color);
      W_L(";");
    }
    if (attr_bgcolor) {
      W_L("background-color:");
      W_V(attr_bgcolor);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(" />");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param ixhtml10  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_hr_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_img_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10 = GET_IXHTML10(pdoc);
  Doc         *doc   = ixhtml10->doc;
  request_rec *r     = doc->r;
  Attr        *attr;
  char        *attr_src    = NULL;
  char        *attr_height = NULL;
  char        *attr_width  = NULL;
  char        *attr_align  = NULL;
  char        *attr_alt    = NULL;
  char        *attr_style  = NULL;
  char        *attr_hspace = NULL;
  char        *attr_vspace = NULL;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = ixhtml10->spec;
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
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      value = chxj_encoding_parameter(r, value);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s",
                             value);
      }
      attr_src = value;
#else
      value = chxj_img_conv(r, spec, value);
      value = chxj_encoding_parameter(r, value);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s",
                             value);
      }
      attr_src = value;
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
          attr_align = value;
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          attr_align = apr_pstrdup(doc->pool, "middle");
        }
      }
    }
    else if (STRCASEEQ('w','W',"width",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_width = value;
    }
    else if (STRCASEEQ('h','H',"height",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_height = value;
    }
    else if (STRCASEEQ('h','H',"hspace",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_hspace = value;
    }
    else if (STRCASEEQ('v','V',"vspace",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_vspace = value;
    }
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_alt = value;
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_style = value;
    }
  }

  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *height_prop = chxj_css_get_property_value(doc, style, "height");
      css_property_t *width_prop  = chxj_css_get_property_value(doc, style, "width");
      css_property_t *valign_prop = chxj_css_get_property_value(doc, style, "vertical-align");
      css_property_t *margin_left_prop   = chxj_css_get_property_value(doc, style, "margin-left");
      css_property_t *margin_right_prop  = chxj_css_get_property_value(doc, style, "margin-right");
      css_property_t *margin_top_prop    = chxj_css_get_property_value(doc, style, "margin-top");
      css_property_t *margin_bottom_prop = chxj_css_get_property_value(doc, style, "margin-bottom");
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
      for (cur = margin_left_prop->next; cur != margin_left_prop; cur = cur->next) {
        attr_hspace = apr_pstrdup(doc->pool, cur->value);
      }
      if (! attr_hspace) {
        for (cur = margin_right_prop->next; cur != margin_right_prop; cur = cur->next) {
          attr_hspace = apr_pstrdup(doc->pool, cur->value);
        }
      }
      for (cur = margin_top_prop->next; cur != margin_top_prop; cur = cur->next) {
        attr_vspace = apr_pstrdup(doc->pool, cur->value);
      }
      if (! attr_vspace) {
        for (cur = margin_bottom_prop->next; cur != margin_bottom_prop; cur = cur->next) {
          attr_vspace = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }

  W_L("<img");
  if (attr_src) {
    W_L(" src=\"");
    W_V(attr_src);
    W_L("\"");
  }
  if (attr_align || attr_hspace || attr_vspace) {
    W_L(" style=\"");
    if (attr_align) {
      if (STRCASEEQ('t','T',"top", attr_align)) {
        W_L("vertical-align:top;");
      }
      else if (STRCASEEQ('m','M',"middle", attr_align) || STRCASEEQ('c','C',"center",attr_align)) {
        W_L("vertical-align:middle;");
      }
      else if (STRCASEEQ('b','B',"bottom", attr_align)) {
        W_L("vertical-align:bottom;");
      }
      else if (STRCASEEQ('l','L',"left", attr_align)) {
        W_L("float:left;");
      }
      else if (STRCASEEQ('r','R',"right", attr_align)) {
        W_L("float:right;");
      }
    }
    if (attr_hspace) {
      W_L("margin-left:");
      W_V(attr_hspace);
      W_L(";");
      W_L("margin-right:");
      W_V(attr_hspace);
      W_L(";");
    }
    if (attr_vspace) {
      W_L("margin-top:");
      W_V(attr_vspace);
      W_L(";");
      W_L("margin-bottom:");
      W_V(attr_vspace);
      W_L(";");
    }
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
  if (attr_alt) {
    W_L(" alt=\"");
    W_V(attr_alt);
    W_L("\"");
  }
  else {
    W_L(" alt=\"\"");
  }
  W_L(" />");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_img_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_select_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10    = GET_IXHTML10(pdoc);
  Doc     *doc      = ixhtml10->doc;
  Attr    *attr;
  char    *size     = NULL;
  char    *name     = NULL;
  char    *multiple = NULL;
  char    *attr_style = NULL;

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
    else if (STRCASEEQ('m','M',"multiple", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      multiple = apr_pstrdup(doc->buf.pool, val);
    }
  }
  W_L("<select");
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

  if (IS_CSS_ON(ixhtml10->entryp)) {
    s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc     *doc   = ixhtml10->doc;

  W_L("</select>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_option_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc     *doc   = ixhtml10->doc;
  Attr    *attr;

  char *selected   = NULL;
  char *value      = NULL;
  char *attr_style = NULL;

  for (attr = qs_get_attr(doc,node);
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
    else if (STRCASEEQ('s','S',"style",nm) && val && *val) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }
  W_L("<option");
  if (value && *value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected=\"selected\"");
  }
  W_L(">");

  if (IS_CSS_ON(ixhtml10->entryp)) {
    s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc = ixhtml10->doc;

  W_L("</option>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_div_tag(void *pdoc, Node *node)
{
  ixhtml10_t  *ixhtml10;
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
  char        *attr_font_size         = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

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

  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_nopush_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t      *ixhtml10;
  Doc          *doc;
  request_rec  *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</div>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


static char *
s_ixhtml10_istyle_to_mode(apr_pool_t *p, const char *s)
{
  if (s) {
    switch (s[0]) {
    case '1': return apr_psprintf(p, "hiragana");
    case '2': return apr_psprintf(p, "hankakukana");
    case '3': return apr_psprintf(p, "alphabet");
    case '4': return apr_psprintf(p, "numeric");
    default:
      return apr_pstrdup(p, "");
    }
  }

  return apr_pstrdup(p,"");
}

static char *
s_ixhtml10_istyle_to_wap_input_format(apr_pool_t *p, const char *s)
{
  if (s) {
    switch (s[0]) {
    case '1': return apr_psprintf(p, "&quot;*&lt;ja:h&gt;&quot;");
    case '2': return apr_psprintf(p, "&quot;*&lt;ja:hk&gt;&quot;");
    case '3': return apr_psprintf(p, "&quot;*&lt;ja:en&gt;&quot;");
    case '4': return apr_psprintf(p, "&quot;*&lt;ja:n&gt;&quot;");
    default:
      return apr_pstrdup(p, "");
    }
  }

  return apr_pstrdup(p,"");
}


static char *
s_ixhtml10_chxjif_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc     *doc;
  Node    *child;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_ixhtml10_chxjif_tag(ixhtml10, child);
  }
  return NULL;
}


/**
 * It is a handler who processes the TEXTARE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_textarea_tag(void *pdoc, Node *node)
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_accesskey = NULL;
  char          *attr_name      = NULL;
  char          *attr_rows      = NULL;
  char          *attr_cols      = NULL;
  char          *attr_istyle    = NULL;
  char          *attr_style     = NULL;


  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  ixhtml10->textarea_flag++;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_nopush_and_get_now_style(pdoc, node, attr_style);
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
    char *vv = s_ixhtml10_istyle_to_wap_input_format(doc->buf.pool,attr_istyle);
    W_L(" style=\"");
    W_L("-wap-input-format:");
    W_V(vv);
    W_L(";");
    W_L("\"");
  }
  W_L(">");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the TEXTAREA tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_textarea_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t       *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("</textarea>");
  ixhtml10->textarea_flag--;

  return ixhtml10->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_ixhtml10_start_b_tag(void* pdoc, Node* UNUSED(node))
{
  ixhtml10_t*      ixhtml10;
  Doc*          doc;
  request_rec*  r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  W_L("<b>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_ixhtml10_end_b_tag(void* pdoc, Node* UNUSED(child))
{
  ixhtml10_t*      ixhtml10 = GET_IXHTML10(pdoc);
  Doc*          doc   = ixhtml10->doc;

  W_L("</b>");
  return ixhtml10->out;
}

static char*
s_ixhtml10_text_tag(void* pdoc, Node* child)
{
  ixhtml10_t*     ixhtml10;
  Doc*         doc;
  char*        textval;
  char*        tmp;
  char*        tdst;
  char         one_byte[2];
  int          ii;
  int          tdst_len;
  request_rec* r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc   = ixhtml10->doc;
  r     = doc->r;

  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return ixhtml10->out;
  }

  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);

  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;

  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_ixhtml10_search_emoji(ixhtml10, &textval[ii], &out);
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
    if (ixhtml10->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else
    if (ixhtml10->textarea_flag) {
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_blockquote_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc      *doc;
  Attr     *attr;
  char     *attr_style = NULL;
  char     *attr_color = NULL;
  char     *attr_size  = NULL;

  ixhtml10  = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc     *doc   = ixhtml10->doc;
  W_L("</blockquote>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_dir_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  W_L("</dir>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_dl_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  W_L("</dl>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_dt_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc    = ixhtml10->doc;
  W_L("</dt>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_dd_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc = ixhtml10->doc;
  W_L("</dd>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h1_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc    = ixhtml10->doc;
  r      = doc->r;

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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(";\"");
  }
  W_L(">");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h1_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t*    ixhtml10;
  Doc*          doc;
  request_rec*  r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h1>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h2_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
    W_L(";\"");
  }
  W_L(">");

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h2_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t*    ixhtml10;
  Doc*          doc;
  request_rec*  r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h2>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h3_tag(void *pdoc, Node *node)
{
  ixhtml10_t  *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h3_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t*    ixhtml10;
  Doc*          doc;
  request_rec*  r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h3>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h4_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h4_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  request_rec   *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h4>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h5_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h5_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t  *ixhtml10;
  Doc         *doc;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h5>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_h6_tag(void *pdoc, Node *node)
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;

  ixhtml10   = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_h6_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t    *ixhtml10;
  Doc         *doc;
  request_rec *r;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  r       = doc->r;

  W_L("</h6>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }

  return ixhtml10->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_menu_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  W_L("</menu>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_plaintext_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc *doc;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  W_L("<plaintext>");
  s_ixhtml10_start_plaintext_tag_inner(pdoc,node);
  return ixhtml10->out;
}

static char *
s_ixhtml10_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc *doc;
  Node *child;
  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_ixhtml10_start_plaintext_tag_inner(pdoc, child);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_blink_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10      = GET_IXHTML10(pdoc);
  Doc       *doc        = ixhtml10->doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style", name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_blink_tag(void *pdoc, Node *UNUSED(child))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc = ixhtml10->doc;
  W_L("</blink>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_marquee_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc       *doc = ixhtml10->doc;
  Attr      *attr;
  char      *attr_direction = NULL;
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
    else if (STRCASEEQ('b','B',"behavior",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('l','L',"loop",name)) {
      /* ignore */
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
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop  = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *bgcolor_prop  = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *direction_prop  = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
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
    }
  }
  W_L("<marquee");
  if (attr_color || attr_size || attr_direction || attr_bgcolor) {
    W_L(" style=\"");
    if (attr_direction) {
      W_L("-wap-marquee-dir:");
      W_V(attr_direction);
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

  return ixhtml10->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_marquee_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc      *doc     = ixhtml10->doc;
  W_L("</marquee>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is handler who processes the New Line Code.
 */
static char *
s_ixhtml10_newline_mark(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  W_NLCODE();
  return ixhtml10->out;
}


/**
 * It is a handler who processes the LINK tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The LINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_link_tag(void *pdoc, Node *node)
{
  ixhtml10_t      *ixhtml10;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc    = ixhtml10->doc;

  if (! IS_CSS_ON(ixhtml10->entryp)) {
    return ixhtml10->out;
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
    ixhtml10->style = chxj_css_parse_from_uri(doc->r, doc->pool, ixhtml10->style, href);
    DBG(doc->r, "end load CSS. url:[%s]", href);
  }

  return ixhtml10->out;
}


static css_prop_list_t *
s_ixhtml10_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(ixhtml10->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, ixhtml10->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(ixhtml10->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(ixhtml10->css_prop_stack);

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
s_ixhtml10_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(ixhtml10->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, ixhtml10->style, node);
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
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_span_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
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

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(ixhtml10->entryp)) {
    css_prop_list_t *style = s_ixhtml10_push_and_get_now_style(pdoc, node, attr_style);
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
  return ixhtml10->out;
}


/**
 * It is a handler who processes the SPAN tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_span_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;

  W_L("</span>");
  if (IS_CSS_ON(ixhtml10->entryp)) {
    chxj_css_pop_prop_list(ixhtml10->css_prop_stack);
  }
  return ixhtml10->out;
}


/**
 * It is a handler who processes the STYLE tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The STYLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_style_tag(void *pdoc, Node *node)
{
  ixhtml10_t     *ixhtml10;
  Doc           *doc;
  Attr          *attr;
  char          *type = NULL;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;

  if (! IS_CSS_ON(ixhtml10->entryp)) {
    return ixhtml10->out;
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
      ixhtml10->style = chxj_css_parse_style_value(doc, ixhtml10->style, value);
      DBG(doc->r, "end load CSS. value:[%s]", value);
    }
  }
  return ixhtml10->out;
}


static char *
s_ixhtml10_create_style_data(apr_pool_t *pool, const char *style_data)
{
  if (! style_data) {
    return "";
  }
  return apr_pstrcat(pool, "<style type=\"text/css\"><![CDATA[",style_data, "]]></style>", NULL);
}


/**
 * It is a handler who processes the NOBR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The NOBR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_start_nobr_tag(void *pdoc, Node *node)
{
  ixhtml10_t *ixhtml10;
  Doc *doc;

  ixhtml10 = GET_IXHTML10(pdoc);
  doc     = ixhtml10->doc;

  W_L("<nobr>");
  return ixhtml10->out;
}


/**
 * It is a handler who processes the NOBR tag.
 *
 * @param pdoc  [i/o] The pointer to the IXHTML10 structure at the output
 *                     destination is specified.
 * @param node   [i]   The NOBR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_ixhtml10_end_nobr_tag(void *pdoc, Node *UNUSED(node))
{
  ixhtml10_t *ixhtml10 = GET_IXHTML10(pdoc);
  Doc *doc = ixhtml10->doc;

  W_L("</nobr>");
  return ixhtml10->out;
}

/*
 * vim:ts=2 et
 */
