/*
 * Copyright (C) 2005-2009 Atsushi Konno All rights reserved.
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
#include "chxj_jxhtml.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"
#include "chxj_header_inf.h"
#include "chxj_jreserved_tag.h"
#include "chxj_conv_z2h.h"


#define GET_JXHTML(X) ((jxhtml_t *)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { jxhtml->out = BUFFERED_WRITE_LITERAL(jxhtml->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { jxhtml->out = (X) ? BUFFERED_WRITE_VALUE(jxhtml->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(jxhtml->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(jxhtml->conf); W_V(nlcode); } while (0)

static char *s_jxhtml_start_html_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_html_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_meta_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_meta_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_head_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_head_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_title_tag    (void *pdoc, Node *node);
static char *s_jxhtml_end_title_tag      (void *pdoc, Node *node);
static char *s_jxhtml_start_base_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_base_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_body_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_body_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_a_tag        (void *pdoc, Node *node);
static char *s_jxhtml_end_a_tag          (void *pdoc, Node *node);
static char *s_jxhtml_start_pre_tag      (void *pdoc, Node *node);
static char *s_jxhtml_end_pre_tag        (void *pdoc, Node *node);
static char *s_jxhtml_start_p_tag        (void *pdoc, Node *node);
static char *s_jxhtml_end_p_tag          (void *pdoc, Node *node);
static char *s_jxhtml_start_ul_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_ul_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_ol_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_ol_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_li_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_li_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_br_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_br_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_table_tag    (void *pdoc, Node *node);
static char *s_jxhtml_end_table_tag      (void *pdoc, Node *node);
static char *s_jxhtml_start_tr_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_tr_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_td_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_td_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_font_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_font_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_form_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_form_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_input_tag    (void *pdoc, Node *node);
static char *s_jxhtml_end_input_tag      (void *pdoc, Node *node);
static char *s_jxhtml_start_center_tag   (void *pdoc, Node *node);
static char *s_jxhtml_end_center_tag     (void *pdoc, Node *node);
static char *s_jxhtml_start_hr_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_hr_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_img_tag      (void *pdoc, Node *node);
static char *s_jxhtml_end_img_tag        (void *pdoc, Node *node);
static char *s_jxhtml_start_select_tag   (void *pdoc, Node *node);
static char *s_jxhtml_end_select_tag     (void *pdoc, Node *node);
static char *s_jxhtml_start_option_tag   (void *pdoc, Node *node);
static char *s_jxhtml_end_option_tag     (void *pdoc, Node *node);
static char *s_jxhtml_start_div_tag      (void *pdoc, Node *node);
static char *s_jxhtml_end_div_tag        (void *pdoc, Node *node);
static char *s_jxhtml_start_textarea_tag (void *pdoc, Node *node);
static char *s_jxhtml_end_textarea_tag   (void *pdoc, Node *node);
static char *s_jxhtml_start_b_tag        (void *pdoc, Node *node);
static char *s_jxhtml_end_b_tag          (void *pdoc, Node *node);
static char *s_jxhtml_chxjif_tag         (void *pdoc, Node *node); 
static char *s_jxhtml_text_tag           (void *pdoc, Node *node);
static char *s_jxhtml_start_blockquote_tag (void *pdoc, Node *node);
static char *s_jxhtml_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_jxhtml_start_dir_tag      (void *pdoc, Node *node);
static char *s_jxhtml_end_dir_tag        (void *pdoc, Node *node);
static char *s_jxhtml_start_dl_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_dl_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_dt_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_dt_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_dd_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_dd_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h1_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h1_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h2_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h2_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h3_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h3_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h4_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h4_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h5_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h5_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_h6_tag       (void *pdoc, Node *node);
static char *s_jxhtml_end_h6_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_menu_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_menu_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_jxhtml_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_jxhtml_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_jxhtml_start_blink_tag  (void *pdoc, Node *node);
static char *s_jxhtml_end_blink_tag    (void *pdoc, Node *node);
static char *s_jxhtml_start_marquee_tag (void *pdoc, Node *node);
static char *s_jxhtml_end_marquee_tag  (void *pdoc, Node *node);
static char *s_jxhtml_newline_mark       (void *pdoc, Node *node);
static char *s_jxhtml_link_tag           (void *pdoc, Node *node);
static char *s_jxhtml_start_span_tag     (void *pdoc, Node *node);
static char *s_jxhtml_end_span_tag       (void *pdoc, Node *node);
static char *s_jxhtml_style_tag       (void *pdoc, Node *node);

static void  s_init_jxhtml(jxhtml_t *jxhtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_jxhtml_search_emoji(jxhtml_t *jxhtml, char *txt, char **rslt);

static css_prop_list_t *s_jxhtml_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);
static css_prop_list_t *s_jxhtml_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value);



tag_handler jxhtml_handler[] = {
  /* tagHTML */
  {
    s_jxhtml_start_html_tag,
    s_jxhtml_end_html_tag,
  },
  /* tagMETA */
  {
    s_jxhtml_start_meta_tag,
    s_jxhtml_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_jxhtml_start_textarea_tag,
    s_jxhtml_end_textarea_tag,
  },
  /* tagP */
  {
    s_jxhtml_start_p_tag,
    s_jxhtml_end_p_tag,
  },
  /* tagPRE */
  {
    s_jxhtml_start_pre_tag,
    s_jxhtml_end_pre_tag,
  },
  /* tagUL */
  {
    s_jxhtml_start_ul_tag,
    s_jxhtml_end_ul_tag,
  },
  /* tagLI */
  {
    s_jxhtml_start_li_tag,
    s_jxhtml_end_li_tag,
  },
  /* tagOL */
  {
    s_jxhtml_start_ol_tag,
    s_jxhtml_end_ol_tag,
  },
  /* tagH1 */
  {
    s_jxhtml_start_h1_tag,
    s_jxhtml_end_h1_tag,
  },
  /* tagH2 */
  {
    s_jxhtml_start_h2_tag,
    s_jxhtml_end_h2_tag,
  },
  /* tagH3 */
  {
    s_jxhtml_start_h3_tag,
    s_jxhtml_end_h3_tag,
  },
  /* tagH4 */
  {
    s_jxhtml_start_h4_tag,
    s_jxhtml_end_h4_tag,
  },
  /* tagH5 */
  {
    s_jxhtml_start_h5_tag,
    s_jxhtml_end_h5_tag,
  },
  /* tagH6 */
  {
    s_jxhtml_start_h6_tag,
    s_jxhtml_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_jxhtml_start_head_tag,
    s_jxhtml_end_head_tag,
  },
  /* tagTITLE */
  {
    s_jxhtml_start_title_tag,
    s_jxhtml_end_title_tag,
  },
  /* tagBASE */
  {
    s_jxhtml_start_base_tag,
    s_jxhtml_end_base_tag,
  },
  /* tagBODY */
  {
    s_jxhtml_start_body_tag,
    s_jxhtml_end_body_tag,
  },
  /* tagA */
  {
    s_jxhtml_start_a_tag,
    s_jxhtml_end_a_tag,
  },
  /* tagBR */
  {
    s_jxhtml_start_br_tag,
    s_jxhtml_end_br_tag,
  },
  /* tagTABLE */
  {
    s_jxhtml_start_table_tag,
    s_jxhtml_end_table_tag,
  },
  /* tagTR */
  {
    s_jxhtml_start_tr_tag,
    s_jxhtml_end_tr_tag,
  },
  /* tagTD */
  {
    s_jxhtml_start_td_tag,
    s_jxhtml_end_td_tag,
  },
  /* tagTBODY */
  {
    NULL,
    NULL,
  },
  /* tagFONT */
  {
    s_jxhtml_start_font_tag,
    s_jxhtml_end_font_tag,
  },
  /* tagFORM */
  {
    s_jxhtml_start_form_tag,
    s_jxhtml_end_form_tag,
  },
  /* tagINPUT */
  {
    s_jxhtml_start_input_tag,
    s_jxhtml_end_input_tag,
  },
  /* tagCENTER */
  {
    s_jxhtml_start_center_tag,
    s_jxhtml_end_center_tag,
  },
  /* tagHR */
  {
    s_jxhtml_start_hr_tag,
    s_jxhtml_end_hr_tag,
  },
  /* tagIMG */
  {
    s_jxhtml_start_img_tag,
    s_jxhtml_end_img_tag,
  },
  /* tagSELECT */
  {
    s_jxhtml_start_select_tag,
    s_jxhtml_end_select_tag,
  },
  /* tagOPTION */
  {
    s_jxhtml_start_option_tag,
    s_jxhtml_end_option_tag,
  },
  /* tagDIV */
  {
    s_jxhtml_start_div_tag,
    s_jxhtml_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_jxhtml_chxjif_tag,
    NULL,
  },
  /* tagCHXJRAW */
  {
    s_jxhtml_chxjif_tag,
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
    s_jxhtml_style_tag,
    NULL,
  },
  /* tagSPAN */
  {
    s_jxhtml_start_span_tag,
    s_jxhtml_end_span_tag,
  },
  /* tagTEXT */
  {
    s_jxhtml_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_jxhtml_start_b_tag,
    s_jxhtml_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_jxhtml_start_dt_tag,
    s_jxhtml_end_dt_tag,
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
    s_jxhtml_start_blockquote_tag,
    s_jxhtml_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_jxhtml_start_dir_tag,
    s_jxhtml_end_dir_tag,
  },
  /* tagDL */
  {
    s_jxhtml_start_dl_tag,
    s_jxhtml_end_dl_tag,
  },
  /* tagDD */
  {
    s_jxhtml_start_dd_tag,
    s_jxhtml_end_dd_tag,
  },
  /* tagMENU */
  {
    s_jxhtml_start_menu_tag,
    s_jxhtml_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_jxhtml_start_plaintext_tag,
    s_jxhtml_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_jxhtml_start_blink_tag,
    s_jxhtml_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_jxhtml_start_marquee_tag,
    s_jxhtml_end_marquee_tag,
  },
  /* tagLINK */
  {
    s_jxhtml_link_tag,
    NULL,
  },
  /* tagNLMARK */
  {
    s_jxhtml_newline_mark,
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
};


/**
 * converts from CHTML5.0 to JXHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_convert_jxhtml(
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
  jxhtml_t   jxhtml;
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
  s_init_jxhtml(&jxhtml, &doc, r, spec);

  jxhtml.entryp = entryp;
  jxhtml.cookie = cookie;

  chxj_set_content_type(r, chxj_header_inf_set_content_type(r, "application/xhtml+xml; charset=Windows-31J"));

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);

  memset(ss,   0, srclen + 1);
  memcpy(ss, src, srclen);

  if (IS_CSS_ON(jxhtml.entryp)) {
    /* current property list */
    jxhtml.css_prop_stack = chxj_new_prop_list_stack(&doc);
  }
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> JXHTML", ss, srclen);
#endif

  qs_parse_string(&doc,ss,strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to JXHTML.                                     */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void*)&jxhtml, &doc, qs_get_root(&doc), 0);
  jxhtml.out = chxj_buffered_write_flush(jxhtml.out, &doc.buf);
  dst = apr_pstrdup(r->pool, jxhtml.out);
  chxj_buffered_write_terminate(&doc.buf);


  qs_all_free(&doc,QX_LOGMARK);

  if (! dst) 
    return apr_pstrdup(r->pool,ss);

  if (! strlen(dst)) 
    dst = apr_psprintf(r->pool, "\n");

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> JXHTML", dst, *dstlen);
#endif

  return dst;
}


/**
 * The JXHTML structure is initialized.
 *
 * @param jxhtml [i/o] The pointer to the JXHTML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   JXHTML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_jxhtml(jxhtml_t *jxhtml, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(jxhtml, 0, sizeof(jxhtml_t));

  doc->r      = r;
  jxhtml->doc  = doc;
  jxhtml->spec = spec;
  jxhtml->out  = qs_alloc_zero_byte_string(r->pool);
  jxhtml->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  jxhtml->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param jxhtml   [i]   The pointer to the JXHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_jxhtml_search_emoji(jxhtml_t *jxhtml, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = jxhtml->spec;

  len = strlen(txt);
  r = jxhtml->doc->r;

  if (! spec) DBG(r,"spec is NULL");

  for (ee = jxhtml->conf->emoji;
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


char *
chxj_jxhtml_emoji_only_converter(request_rec *r, device_table *spec, const char *src, apr_size_t len)
{
  apr_size_t ii;
  Doc __doc;
  Doc *doc;
  jxhtml_t __jxhtml;
  jxhtml_t *jxhtml;
  char one_byte[2];
  char two_byte[3];
  apr_pool_t *pool;

  jxhtml = &__jxhtml;
  doc    = &__doc;

  DBG(r, "REQ[%X] start chxj_jxhtml_emoji_eonly_converter()", (unsigned int)(apr_size_t)r);
  memset(doc,    0, sizeof(Doc));
  memset(jxhtml, 0, sizeof(jxhtml_t));

  doc->r       = r;
  jxhtml->doc  = doc;
  jxhtml->spec = spec;
  jxhtml->out  = qs_alloc_zero_byte_string(r->pool);
  jxhtml->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  jxhtml->doc->parse_mode = PARSE_MODE_CHTML;

  apr_pool_create(&pool, r->pool);

  chxj_buffered_write_init(pool, &doc->buf);

  for (ii=0; ii<len; ii++) {
    char *out;
    int   rtn;

    rtn = s_jxhtml_search_emoji(jxhtml, (char *)&src[ii], &out);
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
  jxhtml->out = chxj_buffered_write_flush(jxhtml->out, &doc->buf);

  DBG(r, "REQ[%X] end chxj_jxhtml_emoji_eonly_converter()", (unsigned int)(apr_size_t)r);
  return jxhtml->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  jxhtml_t       *jxhtml;
  Doc           *doc;
  request_rec   *r;


  jxhtml  = GET_JXHTML(pdoc);
  doc    = jxhtml->doc;
  r      = doc->r;
  DBG(r, "REQ[%X] start s_jxhtml_start_html_tag()", TO_ADDR(r));

  W_L("<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>");
  W_NLCODE();
  W_L("<!DOCTYPE html PUBLIC \"-//J-PHONE//DTD XHTML Basic 1.0 Plus//EN\" \"xhtml-basic10-plus.dtd\">");
  W_NLCODE();

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"ja\" xml:lang=\"ja\">");

  jxhtml->start_html_flag = 1;

  DBG(r, "REQ[%X] end s_jxhtml_start_html_tag()", TO_ADDR(r));

  return jxhtml->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml = GET_JXHTML(pdoc);
  Doc           *doc = jxhtml->doc;

  W_L("</html>");

  return jxhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_meta_tag(void *pdoc, Node *node) 
{
  jxhtml_t     *jxhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  int          content_type_flag;
  int          refresh_flag;

  jxhtml             = GET_JXHTML(pdoc);
  doc               = jxhtml->doc;
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
          W_V(chxj_header_inf_set_content_type(r, "application/xhtml+xml; charset=SHIFT_JIS"));
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
            url = chxj_encoding_parameter(r, url, 1);
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
    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0 && value && *value) {
        W_L(" name=\"");
        W_V(value);
        W_L("\"");
      }
      break;
    default:
      break;
    }
  }
  W_L(" />");
  return jxhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);

  return jxhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("<head>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t       *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</head>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  jxhtml_t      *jxhtml;
  Doc          *doc;
  request_rec  *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("<title>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</title>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_base_tag(void *pdoc, Node *node) 
{
  jxhtml_t      *jxhtml;
  Attr          *attr;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_body_tag(void *pdoc, Node *node) 
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_bgcolor = NULL;
  char        *attr_text    = NULL;
  char        *attr_link    = NULL;
  char        *attr_style   = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }

  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
    if (jxhtml->style) {
      css_stylesheet_t *pseudos = chxj_find_pseudo_selectors(doc, jxhtml->style);
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
  W_L("><div>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t       *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</div></body>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_a_tag(void *pdoc, Node *node) 
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
      W_V(chxj_jreserved_to_safe_tag(r, value, jxhtml->entryp));
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"href",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value, 1);
      if (! chxj_starts_with(value, "mailto:") && ! chxj_starts_with(value, "tel:")) {
        value = chxj_jreserved_tag_to_safe_for_query_string(r, value, jxhtml->entryp, 1);
      }
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml;
  Doc          *doc;
  request_rec  *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</a>");

  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_br_tag(void *pdoc, Node *node)
{
  jxhtml_t     *jxhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;
  
  char         *attr_style = NULL;
  char         *attr_clear = NULL;

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
      attr_style = apr_pstrdup(doc->buf.pool, value);
    }
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  if(attr_clear){
    W_L(" clear=\"");
    W_V(attr_clear);
    W_L("\"");
  }
  W_L(" />");
  return jxhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}

/**
 * It is a handler who processes the TABLE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_table_tag(void *pdoc, Node *node) 
{
  jxhtml_t     *jxhtml;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;
  
  char         *attr_style  = NULL;
  char         *attr_align  = NULL;
  char         *attr_width  = NULL;
  char         *attr_height = NULL;
  char         *attr_bgcolor = NULL;
  char         *attr_border_width  = NULL;
  char         *attr_border_color  = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;
  
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *val   = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        attr_align = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('h','H',"height",name) && val && *val) {
      attr_height = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('w','W',"width",name) && val && *val) {
      attr_width = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('s','S',"style",name) && val && *val) {
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('b','B',"bgcolor",name) && val && *val) {
      attr_bgcolor = apr_pstrdup(doc->buf.pool, val);
      attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
    }
    else if (STRCASEEQ('b','B',"border",name) && val && *val) {
      attr_border_width = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('b','B',"bordercolor",name) && val && *val) {
      attr_border_color = apr_pstrdup(doc->buf.pool, val);
      attr_border_color = chxj_css_rgb_func_to_value(doc->pool, attr_border_color);
    }
  }
  
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *width_prop             = chxj_css_get_property_value(doc, style, "width");
      css_property_t *height_prop            = chxj_css_get_property_value(doc, style, "height");
      css_property_t *align_prop             = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *bgcolor_prop           = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *border_width_prop      = chxj_css_get_property_value(doc, style, "border-width");
      css_property_t *border_color_prop      = chxj_css_get_property_value(doc, style, "border-color");
      
      css_property_t *cur;
      for (cur = width_prop->next; cur != width_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) {
          size_t len = strlen(tmp) - strlen(tmpp);
          attr_width = apr_pstrndup(doc->pool, tmp,len);
        }
        else{
          attr_width = apr_pstrdup(doc->pool, tmp);
        }
      }
      for (cur = height_prop->next; cur != height_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) {
          size_t len = strlen(tmp) - strlen(tmpp);
          attr_height = apr_pstrndup(doc->pool, tmp,len);
        }
        else{
          attr_height = apr_pstrdup(doc->pool, tmp);
        }
      }
      for (cur = align_prop->next; cur != align_prop; cur = cur->next) {
        if (cur->value && (STRCASEEQ('l','L',"left",cur->value) || STRCASEEQ('r','R',"right",cur->value) || STRCASEEQ('c','C',"center",cur->value))) {
          attr_align = apr_pstrdup(doc->buf.pool, cur->value);
        }
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      }
      for (cur = border_width_prop->next; cur != border_width_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "px");
        if (tmpp) {
          size_t len = strlen(tmp) - strlen(tmpp);
          attr_border_width = apr_pstrndup(doc->pool, tmp,len);
        }
        else{
          attr_border_width = apr_pstrdup(doc->pool, tmp);
        }
      }
      for (cur = border_color_prop->next; cur != border_color_prop; cur = cur->next) {
        attr_border_color = apr_pstrdup(doc->pool, cur->value);
        attr_border_color = chxj_css_rgb_func_to_value(doc->pool, attr_border_color);
      }
    }
  }

  W_L("<table");
  if (attr_align){
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  if (attr_height){
    W_L(" height=\"");
    W_L(attr_height);
    W_L("\"");
  }
  if (attr_width){
    W_L(" width=\"");
    W_V(attr_width);
    W_L("\"");
  }
  if (attr_bgcolor && *attr_bgcolor){
    W_L(" bgcolor=\"");
    W_V(attr_bgcolor);
    W_L("\"");
  }
  if (attr_border_width || attr_border_color ){
    W_L(" style=\"border:");
    if (attr_border_width){
      W_V(attr_border_width);
    }
    else{
      W_L("1");
    }
    W_L("px solid");
    
    if (attr_border_color && *attr_border_color){
      W_L(" ");
      W_V(attr_border_color);
    }
    W_L(";\"");
  }
  W_L(">");
  
  return jxhtml->out;
}

/**
 * It is a handler who processes the TABLE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_table_tag(void *pdoc, Node *UNUSED(node)) 
{
  jxhtml_t      *jxhtml;
  request_rec  *r;
  Doc          *doc;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = jxhtml->doc->r;
  
  W_L("</table>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_tr_tag(void *pdoc, Node *node) 
{
  jxhtml_t      *jxhtml;
  Doc          *doc;
  request_rec  *r;
  
  Attr         *attr;
  
  char         *attr_style  = NULL;
  char         *attr_align  = NULL;
  char         *attr_valign = NULL;
  char         *attr_bgcolor = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;
  
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *val   = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        attr_align = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('v','V',"valign",name) && val && *val) {
      if (val && (STRCASEEQ('t','T',"top",val) || STRCASEEQ('m','M',"middle",val) || STRCASEEQ('b','B',"bottom",val))) {
        attr_valign = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && val && *val) {
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('b','B',"bgcolor",name) && val && *val) {
      attr_bgcolor = apr_pstrdup(doc->buf.pool, val);
      attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
    }
  }
  
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *align_prop             = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *valign_prop            = chxj_css_get_property_value(doc, style, "vertical-align");
      css_property_t *bgcolor_prop           = chxj_css_get_property_value(doc, style, "background-color");
      
      css_property_t *cur;
      for (cur = align_prop->next; cur != align_prop; cur = cur->next) {
        if (cur->value && (STRCASEEQ('l','L',"left",cur->value) || STRCASEEQ('r','R',"right",cur->value) || STRCASEEQ('c','C',"center",cur->value))) {
          attr_align = apr_pstrdup(doc->buf.pool, cur->value);
        }
      }
      for (cur = valign_prop->next; cur != valign_prop; cur = cur->next) {
        if (cur->value && (STRCASEEQ('t','T',"top",cur->value) || STRCASEEQ('m','M',"middle",cur->value) || STRCASEEQ('b','B',"bottom",cur->value))) {
          attr_valign = apr_pstrdup(doc->buf.pool, cur->value);
        }
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      }
    }
  }

  W_L("<tr");
  if (attr_align){
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  if (attr_valign){
    W_L(" valign=\"");
    W_V(attr_valign);
    W_L("\"");
  }
  if (attr_bgcolor && *attr_bgcolor){
    W_L(" bgcolor=\"");
    W_V(attr_bgcolor);
    W_L("\"");
  }
  W_L(">");
  return jxhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml;
  request_rec  *r;
  Doc          *doc;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = jxhtml->doc->r;
  
  W_L("</tr>");
  return jxhtml->out;
}

/**
 * It is a handler who processes the TD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_td_tag(void *pdoc, Node *node) 
{
  jxhtml_t      *jxhtml;
  Doc          *doc;
  request_rec  *r;

  Attr         *attr;
  
  char         *attr_style  = NULL;
  char         *attr_align  = NULL;
  char         *attr_valign = NULL;
  char         *attr_bgcolor = NULL;
  char         *attr_colspan = NULL;
  char         *attr_rowspan = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;
  
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *val   = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        attr_align = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('v','V',"valign",name) && val && *val) {
      if (val && (STRCASEEQ('t','T',"top",val) || STRCASEEQ('m','M',"middle",val) || STRCASEEQ('b','B',"bottom",val))) {
        attr_valign = apr_pstrdup(doc->buf.pool, val);
      }
    }
    else if (STRCASEEQ('s','S',"style",name) && val && *val) {
      attr_style = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('b','B',"bgcolor",name) && val && *val) {
      attr_bgcolor = apr_pstrdup(doc->buf.pool, val);
      attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
    }
    else if (STRCASEEQ('c','C',"colspan",name) && val && *val) {
      attr_colspan = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('r','R',"rowspan",name) && val && *val) {
      attr_rowspan = apr_pstrdup(doc->buf.pool, val);
    }
  }
  
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *align_prop             = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *valign_prop            = chxj_css_get_property_value(doc, style, "vertical-align");
      css_property_t *bgcolor_prop           = chxj_css_get_property_value(doc, style, "background-color");
      
      css_property_t *cur;
      for (cur = align_prop->next; cur != align_prop; cur = cur->next) {
        if (cur->value && (STRCASEEQ('l','L',"left",cur->value) || STRCASEEQ('r','R',"right",cur->value) || STRCASEEQ('c','C',"center",cur->value))) {
          attr_align = apr_pstrdup(doc->buf.pool, cur->value);
        }
      }
      for (cur = valign_prop->next; cur != valign_prop; cur = cur->next) {
        if (cur->value && (STRCASEEQ('t','T',"top",cur->value) || STRCASEEQ('m','M',"middle",cur->value) || STRCASEEQ('b','B',"bottom",cur->value))) {
          attr_valign = apr_pstrdup(doc->buf.pool, cur->value);
        }
      }
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      }
    }
  }

  W_L("<td");
  if (attr_align){
    W_L(" align=\"");
    W_V(attr_align);
    W_L("\"");
  }
  if (attr_valign){
    W_L(" valign=\"");
    W_V(attr_valign);
    W_L("\"");
  }
  if (attr_colspan){
    W_L(" colspan=\"");
    W_V(attr_colspan);
    W_L("\"");
  }
  if (attr_rowspan){
    W_L(" rowspan=\"");
    W_V(attr_rowspan);
    W_L("\"");
  }
  if (attr_bgcolor && *attr_bgcolor){
    W_L(" bgcolor=\"");
    W_V(attr_bgcolor);
    W_L("\"");
  }
  W_L(">");
  return jxhtml->out;
}


/**
 * It is a handler who processes the TD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_td_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml;
  request_rec  *r;
  Doc          *doc;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = jxhtml->doc->r;
  
  W_L("</td>");
  return jxhtml->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_font_tag(void *pdoc, Node *node) 
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_color = NULL;
  char          *attr_size  = NULL;
  char          *attr_style = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
      attr_size = apr_pstrdup(doc->buf.pool, value);
    }
    else if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = apr_pstrdup(doc->buf.pool, value);
    }
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  jxhtml_flags_t *flg = (jxhtml_flags_t *)apr_palloc(doc->pool, sizeof(*flg));
  memset(flg, 0, sizeof(*flg));
  if (attr_color) {
    attr_color = chxj_css_rgb_func_to_value(doc->pool, attr_color);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_font_tag(void *pdoc, Node *node)
{
  jxhtml_t      *jxhtml;
  request_rec  *r;
  Doc          *doc;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = jxhtml->doc->r;

  jxhtml_flags_t *flg = (jxhtml_flags_t *)node->userData;
  if (flg && flg->font_size_flag) {
    W_L("</span>");
  }
  if (flg && flg->font_color_flag) {
    W_L("</font>");
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_form_tag(void *pdoc, Node *node) 
{
  jxhtml_t    *jxhtml;
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

  jxhtml  = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
    attr_action = chxj_encoding_parameter(r, attr_action, 1);
    attr_action = chxj_add_cookie_parameter(r, attr_action, jxhtml->cookie);
    char *q;
    char *old_qs = NULL;
    q = strchr(attr_action, '?');
    if (q) {
      new_hidden_tag = chxj_form_action_to_hidden_tag(r, doc->pool, attr_action, 1, post_flag, &old_qs, CHXJ_FALSE, CHXJ_TRUE, jxhtml->entryp);
      if (new_hidden_tag || old_qs) {
        *q = 0;
      }
    }
    W_L(" action=\"");
    W_V(attr_action);
    if (old_qs) {
      W_L("?");
      W_V(old_qs);
    }
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

  jxhtml_flags_t *flg = (jxhtml_flags_t *)apr_palloc(doc->pool, sizeof(jxhtml_flags_t));
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_form_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc    = jxhtml->doc;

  jxhtml_flags_t *flg = (jxhtml_flags_t *)node->userData;
  if (flg && flg->with_div_flag) {
    W_L("</div>");
  }
  if (flg && flg->with_font_flag) {
    W_L("</font>");
  }
  W_L("</form>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_input_tag(void *pdoc, Node *node) 
{
  jxhtml_t    *jxhtml;
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

  jxhtml  = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
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
    W_V(chxj_jreserved_to_safe_tag(r, attr_name, jxhtml->entryp));
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
    W_L(" istyle=\"");
    W_V(attr_istyle);
    W_L("\"");
  }
  else if(attr_type && STRCASEEQ('p','P',"password",attr_type)) {
    W_L(" istyle=\"4\"");
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
  jxhtml_t       *jxhtml;
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

  jxhtml       = GET_JXHTML(pdoc);
  doc         = jxhtml->doc;
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
    W_V(chxj_jreserved_to_safe_tag(r, name, jxhtml->entryp));
    W_L("\"");
  }
  if (value && *value) {
    if (type && (STRCASEEQ('s','S',"submit",type) || STRCASEEQ('r','R',"reset",type))) {
      apr_size_t value_len = strlen(value);
      value = chxj_conv_z2h(r, value, &value_len, jxhtml->entryp);
    }

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
    if (type && STRCASEEQ('p','P',"password", type) && ! jxhtml->entryp->pc_flag ) {
      W_L(" style=\"-wap-input-format: &quot;*&lt;ja:n&gt;&quot;;\"");
    }
    else {
      char *vv = qs_conv_istyle_to_format(doc->buf.pool, istyle);
      W_L(" style=\"");
      W_L("-wap-input-format:'*");
      W_V(vv);
      W_L("';");
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_center_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
  Doc       *doc;
  Attr      *attr;
  char      *attr_style = NULL;
  char      *attr_color = NULL;
  char      *attr_size  = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc    = jxhtml->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"style",name) && value && *value) {
      attr_style = value;
    }
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  
  return jxhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_center_tag(void *pdoc, Node *UNUSED(node))
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc    = jxhtml->doc;
  r      = doc->r;

  W_L("</center>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_li_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_type  = NULL;
  char        *attr_value = NULL;
  char        *attr_style = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t     *jxhtml;
  Doc         *doc;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  W_L("</li>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_ol_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_start = NULL;
  char        *attr_type  = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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

  return jxhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t     *jxhtml;
  Doc         *doc;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</ol>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_p_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_align = NULL;
  char        *attr_style = NULL;
  char        *attr_color = NULL;
  char        *attr_blink = NULL;
  char        *css_clear  = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *text_align_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *color_prop      = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_deco_prop  = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *clear_prop      = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        css_clear = apr_pstrdup(doc->pool, cur->value);
      }
    }
  }
  W_L("<p");
  if ((attr_align && *attr_align) || (attr_color && *attr_color) || (attr_blink && *attr_blink) || css_clear) {
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
    if (css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return jxhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t  *jxhtml = GET_JXHTML(pdoc);
  Doc       *doc    = jxhtml->doc;

  W_L("</p>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_pre_tag(void *pdoc, Node *node)
{
  jxhtml_t  *jxhtml = GET_JXHTML(pdoc);
  Doc       *doc   = jxhtml->doc;
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  jxhtml->pre_flag++;
  W_L("<pre>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc     *doc   = jxhtml->doc;

  W_L("</pre>");
  jxhtml->pre_flag--;
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_ul_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc    = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc     *doc   = jxhtml->doc;

  W_L("</ul>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_hr_tag(void *pdoc, Node *node) 
{
  Attr        *attr;
  jxhtml_t     *jxhtml;
  Doc         *doc;
  request_rec *r;
  char        *attr_align   = NULL;
  char        *attr_size    = NULL;
  char        *attr_width   = NULL;
  char        *attr_noshade = NULL;
  char        *attr_style   = NULL;
  char        *attr_color   = NULL;
  
  char        *style_float  = NULL;
  char        *style_border_color = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *border_style_prop = chxj_css_get_property_value(doc, style, "border-style");
      css_property_t *height_prop       = chxj_css_get_property_value(doc, style, "height");
      css_property_t *width_prop        = chxj_css_get_property_value(doc, style, "width");
      
      css_property_t *bgcolor_prop      = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *float_prop        = chxj_css_get_property_value(doc, style, "float");
      css_property_t *border_color_prop = chxj_css_get_property_value(doc, style, "border-color");
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
      if(!attr_color){
          for(cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next){
          char *tmp   = apr_pstrdup(doc->pool,cur->value);
          attr_color  = apr_pstrdup(doc->pool,tmp);
        }
      }
      for (cur = float_prop->next; cur != float_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp,"none");
        if(tmpp){
          style_float = "center";
        }
        else{
          style_float = apr_pstrdup(doc->pool,tmp);
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
      
      for (cur = border_color_prop->next; cur != border_color_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        if(tmp){
          style_border_color = apr_pstrdup(doc->pool, tmp);
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
  else{
    if(style_float){
      W_L(" align=\"");
      W_V(style_float);
      W_L("\"");
    }
  }
  if (attr_size || attr_width || attr_noshade || style_border_color) {
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
    if(style_border_color){
      W_L("border-color:");
      W_V(style_border_color);
      W_V(";");
    }
    W_L("\"");
  }
  if (attr_color) {
    W_L(" color=\"");
    W_V(attr_color);
    W_L("\"");
  }
  
  W_L(" />");

  return jxhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param jxhtml  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_img_tag(void *pdoc, Node *node) 
{
  jxhtml_t    *jxhtml = GET_JXHTML(pdoc);
  Doc         *doc   = jxhtml->doc;
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
  
  char        *css_float          = NULL;
  char        *css_margin_left    = NULL;
  char        *css_margin_right   = NULL;
  char        *css_margin_top     = NULL;
  char        *css_margin_bottom  = NULL;
  char        *css_display        = NULL;
  char        *css_valign         = NULL;
  
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = jxhtml->spec;
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
      value = chxj_encoding_parameter(r, value, 1);
      value = chxj_jreserved_tag_to_safe_for_query_string(r, value, jxhtml->entryp, 1);
      value = chxj_add_cookie_no_update_parameter(r, value);
      attr_src = value;
#else
      value = chxj_img_conv(r, spec, value);
      value = chxj_encoding_parameter(r, value, 1);
      value = chxj_jreserved_tag_to_safe_for_query_string(r, value, jxhtml->entryp, 1);
      value = chxj_add_cookie_no_update_parameter(r, value);
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
            STRCASEEQ('b','B',"bottom",value)){
          css_valign = value;
        }else if (STRCASEEQ('l','L',"left",  value) || STRCASEEQ('r','R',"right", value)) {
          css_float = value;
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          css_valign = apr_pstrdup(doc->pool, "middle");
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
    else if (STRCASEEQ('h','H',"hspace",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      attr_hspace = value;
    }
    else if (STRCASEEQ('v','V',"vspace",name)) {
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
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
      if(!css_valign){
        for (cur = valign_prop->next; cur != valign_prop; cur = cur->next) {
          css_valign = apr_pstrdup(doc->pool, cur->value);
        }
      }
      if (! attr_hspace) {
        for (cur = margin_left_prop->next; cur != margin_left_prop; cur = cur->next) {
          css_margin_left   = apr_pstrdup(doc->pool, cur->value);
        }
        for (cur = margin_right_prop->next; cur != margin_right_prop; cur = cur->next) {
          css_margin_right  = apr_pstrdup(doc->pool, cur->value);
        }
      }
      if (! attr_vspace) {
        for (cur = margin_top_prop->next; cur != margin_top_prop; cur = cur->next) {
          css_margin_top = apr_pstrdup(doc->pool, cur->value);
        }
        for (cur = margin_bottom_prop->next; cur != margin_bottom_prop; cur = cur->next) {
          css_margin_bottom = apr_pstrdup(doc->pool, cur->value);
        }
      }
      if(!css_float){
        css_property_t *float_prop = chxj_css_get_property_value(doc, style, "float");
        for (cur = float_prop->next; cur != float_prop; cur = cur->next) {
          css_float = apr_pstrdup(doc->pool, cur->value);
        }
      }
      
      css_property_t *display_prop       = chxj_css_get_property_value(doc, style, "display");
      for (cur = display_prop->next; cur != display_prop; cur = cur->next) {
        char *tmp = apr_pstrdup(doc->pool, cur->value);
        char *tmpp = strstr(tmp, "none");
        if(tmpp){
          css_display = apr_pstrdup(doc->pool, tmp);
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
  if (attr_hspace || attr_vspace || css_float || css_margin_left || css_margin_right || css_margin_top || css_margin_bottom || css_valign || css_display) {
    W_L(" style=\"");
    if(css_float){
      W_L("float:");
      W_V(css_float);
      W_L(";");
    }
    if(css_valign){
      W_L("vertical-align:");
      W_V(css_valign);
      W_L(";");
    }
    if (attr_hspace) {
      W_L("margin-left:");
      W_V(attr_hspace);
      W_L(";");
      W_L("margin-right:");
      W_V(attr_hspace);
      W_L(";");
    }
    else{
      if(css_margin_left){
        W_L("margin-left:");
        W_V(css_margin_left);
        W_L(";");
      }
      if(css_margin_right){
        W_L("margin-right:");
        W_V(css_margin_right);
        W_L(";");
      }
    }
    if (attr_vspace) {
      W_L("margin-top:");
      W_V(attr_vspace);
      W_L(";");
      W_L("margin-bottom:");
      W_V(attr_vspace);
      W_L(";");
    }
    else{
      if(css_margin_top){
        W_L("margin-top:");
        W_V(css_margin_top);
        W_L(";");
      }
      if(css_margin_bottom){
        W_L("margin-bottom:");
        W_V(css_margin_bottom);
        W_L(";");
      }
    }
    if(css_display){
      W_L("display:none;");
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_select_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml    = GET_JXHTML(pdoc);
  Doc     *doc      = jxhtml->doc;
  Attr    *attr;
  char    *size     = NULL;
  char    *name     = NULL;
  char    *multiple = NULL;
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc     *doc   = jxhtml->doc;

  W_L("</select>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_option_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc     *doc   = jxhtml->doc;
  Attr    *attr;

  char *selected   = NULL;
  char *value      = NULL;
  char *attr_style = NULL;

  W_L("<option");
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
  if (value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected");
  }
  W_L(">");

  if (IS_CSS_ON(jxhtml->entryp)) {
    s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc = jxhtml->doc;

  W_L("</option>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_div_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
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
  char        *attr_font_size         = NULL;
  char        *css_clear              = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
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

  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *display_prop           = chxj_css_get_property_value(doc, style, "display");
      css_property_t *text_decoration_prop   = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *color_prop             = chxj_css_get_property_value(doc, style, "color");
      css_property_t *text_align_prop        = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *font_size_prop         = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *background_color_prop  = chxj_css_get_property_value(doc, style, "background-color");
      css_property_t *background_prop        = chxj_css_get_property_value(doc, style, "background");
      css_property_t *clear_prop             = chxj_css_get_property_value(doc, style, "clear");

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
      for (cur = background_color_prop->next; cur != background_color_prop; cur = cur->next) {
        attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
        attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
      }
      for (cur = background_prop->next; cur != background_prop; cur = cur->next) {
        char *ss = strchr(cur->value, '#');
        if (!ss || !*ss) {
          ss = strstr(cur->value, "rgb");
        }
        if (ss && *ss) {
          attr_bgcolor = apr_pstrdup(doc->pool, cur->value);
          attr_bgcolor = chxj_css_rgb_func_to_value(doc->pool, attr_bgcolor);
        }
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
          if(strcmp(cur->value,"0") == 0 || strcmp(cur->value,"-1") == 0){
            attr_wap_marquee_loop = "infinite";
          }
          else{
            attr_wap_marquee_loop = apr_pstrdup(doc->pool, cur->value);
          }
        }
      }
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        css_clear = apr_pstrdup(doc->pool, cur->value);
      }
    }
  }  
  W_L("<div");
  if (attr_align
      || attr_display
      || attr_decoration
      || attr_wap_marquee_style
      || attr_wap_marquee_dir
      || attr_wap_marquee_loop
      || attr_color
      || attr_bgcolor
      || attr_font_size
      || css_clear ) {
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
    if (attr_bgcolor) {
      W_L("background-color:");
      W_V(attr_bgcolor);
      W_L(";");
    }
    if (attr_font_size) {
      W_L("font-size:");
      W_V(attr_font_size);
      W_L(";");
    }
    if (css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return jxhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t      *jxhtml;
  Doc          *doc;
  request_rec  *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</div>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


static char *
s_jxhtml_chxjif_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
  Doc     *doc;
  Node    *child;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_jxhtml_chxjif_tag(jxhtml, child);
  }
  return NULL;
}


/**
 * It is a handler who processes the TEXTARE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_textarea_tag(void *pdoc, Node *node) 
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *attr_accesskey = NULL;
  char          *attr_name      = NULL;
  char          *attr_rows      = NULL;
  char          *attr_cols      = NULL;
  char          *attr_istyle    = NULL;
  char          *attr_style     = NULL;


  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  jxhtml->textarea_flag++;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_nopush_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the TEXTAREA tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t       *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("</textarea>");
  jxhtml->textarea_flag--;

  return jxhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jxhtml_start_b_tag(void* pdoc, Node* UNUSED(node)) 
{
  jxhtml_t*      jxhtml;
  Doc*          doc;
  request_rec*  r;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  W_L("<b>");
  return jxhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jxhtml_end_b_tag(void* pdoc, Node* UNUSED(child)) 
{
  jxhtml_t*      jxhtml = GET_JXHTML(pdoc);
  Doc*          doc   = jxhtml->doc;

  W_L("</b>");
  return jxhtml->out;
}

static char*
s_jxhtml_text_tag(void* pdoc, Node* child)
{
  jxhtml_t*     jxhtml;
  Doc*         doc;
  char*        textval;
  char*        tmp;
  char*        tdst;
  char         one_byte[2];
  int          ii;
  int          tdst_len;
  request_rec* r;
  apr_size_t   z2h_input_len;

  jxhtml = GET_JXHTML(pdoc);
  doc   = jxhtml->doc;
  r     = doc->r;

  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return jxhtml->out;
  }

  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);

  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;

  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_jxhtml_search_emoji(jxhtml, &textval[ii], &out);
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
    if (jxhtml->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else
    if (jxhtml->textarea_flag) {
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
  z2h_input_len = strlen(tdst);
  tdst = chxj_conv_z2h(r, tdst, &z2h_input_len, jxhtml->entryp);

  W_V(tdst);
  return jxhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_blockquote_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
  Doc      *doc;
  Attr     *attr;
  char     *attr_style = NULL;
  char     *attr_color = NULL;
  char     *attr_size  = NULL;

  jxhtml  = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc     *doc   = jxhtml->doc;
  W_L("</blockquote>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_dir_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;
  W_L("</dir>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_dl_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;
  W_L("</dl>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_dt_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc    = jxhtml->doc;
  W_L("</dt>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_dd_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc = jxhtml->doc;
  W_L("</dd>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h1_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc    = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h1");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t*    jxhtml;
  Doc*          doc;
  request_rec*  r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;
  
  W_L("</h1>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h2_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h2");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t*    jxhtml;
  Doc*          doc;
  request_rec*  r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;
  
  W_L("</h2>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h3_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h3");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t*    jxhtml;
  Doc*          doc;
  request_rec*  r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;

  W_L("</h3>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h4_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h4");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  request_rec   *r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;
  
  W_L("</h4>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h5_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h5");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;
  
  W_L("</h5>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_h6_tag(void *pdoc, Node *node)
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  char        *attr_style = NULL;
  char        *attr_align = NULL;
  char        *css_clear  = NULL;

  jxhtml   = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *list_style_type_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *clear_prop           = chxj_css_get_property_value(doc, style, "clear");
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
      for (cur = clear_prop->next; cur != clear_prop; cur = cur->next) {
        if (STRCASEEQ('b','B',"both", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "both");
        }
        else if (STRCASEEQ('r','R',"right", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "right");
        }
        else if (STRCASEEQ('l','L',"left", cur->value)) {
          css_clear = apr_pstrdup(doc->pool, "left");
        }
      }
    }
  }
  W_L("<h6");
  if (attr_align || css_clear ) {
    W_L(" style=\"");
    if(attr_align){
      W_L("text-align:");
      W_V(attr_align);
      W_L(";");
    }
    if(css_clear){
      W_L("clear:");
      W_V(css_clear);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");

  return jxhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  jxhtml_t    *jxhtml;
  Doc         *doc;
  request_rec *r;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  r       = doc->r;
  
  W_L("</h6>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }

  return jxhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_menu_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;
  W_L("</menu>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_plaintext_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
  Doc *doc;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  W_L("<plaintext>");
  s_jxhtml_start_plaintext_tag_inner(pdoc,node);
  return jxhtml->out;
}

static char *
s_jxhtml_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
  Doc *doc;
  Node *child;
  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_jxhtml_start_plaintext_tag_inner(pdoc, child);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  return jxhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_blink_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml      = GET_JXHTML(pdoc);
  Doc       *doc        = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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
  return jxhtml->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_blink_tag(void *pdoc, Node *UNUSED(child))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc = jxhtml->doc;
  W_L("</blink>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_marquee_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc       *doc = jxhtml->doc;
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
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
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

  return jxhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_marquee_tag(void *pdoc, Node *UNUSED(node))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc      *doc     = jxhtml->doc;
  W_L("</marquee>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is handler who processes the New Line Code.
 */
static char *
s_jxhtml_newline_mark(void *pdoc, Node *UNUSED(node))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  if (jxhtml->start_html_flag) {
    Doc *doc = jxhtml->doc;
    W_NLCODE();
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the LINK tag.
 *
 * @param pdoc  [i/o] The pointer to the JXHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_link_tag(void *pdoc, Node *node)
{
  jxhtml_t      *jxhtml;
  Doc           *doc;
  Attr          *attr;
  char          *rel  = NULL;
  char          *href = NULL;
  char          *type = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc    = jxhtml->doc;

  if (! IS_CSS_ON(jxhtml->entryp)) {
    return jxhtml->out;
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
    jxhtml->style = chxj_css_parse_from_uri(doc->r, doc->pool, jxhtml->style, href);
    DBG(doc->r, "end load CSS. url:[%s]", href);
  }

  return jxhtml->out;
}


static css_prop_list_t *
s_jxhtml_push_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(jxhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, jxhtml->style, node);
    if (selector) {
      chxj_css_prop_list_merge_property(doc, dup_css, selector);
    }
    chxj_css_push_prop_list(jxhtml->css_prop_stack, dup_css);
    last_css = chxj_css_get_last_prop_list(jxhtml->css_prop_stack);

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
s_jxhtml_nopush_and_get_now_style(void *pdoc, Node *node, const char *style_attr_value)
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;
  css_prop_list_t *last_css = NULL;
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *dup_css;
    css_selector_t  *selector;

    last_css = chxj_css_get_last_prop_list(jxhtml->css_prop_stack);
    dup_css  = chxj_dup_css_prop_list(doc, last_css);
    selector = chxj_css_find_selector(doc, jxhtml->style, node);
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
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_start_span_tag(void *pdoc, Node *node)
{
  jxhtml_t *jxhtml;
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
  char *css_bgcolor        = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (val && STRCASEEQ('s','S',"style", nm)) {
      attr_style = val;
    }
  }
  if (IS_CSS_ON(jxhtml->entryp)) {
    css_prop_list_t *style = s_jxhtml_push_and_get_now_style(pdoc, node, attr_style);
    if (style) {
      css_property_t *color_prop = chxj_css_get_property_value(doc, style, "color");
      css_property_t *size_prop = chxj_css_get_property_value(doc, style, "font-size");
      css_property_t *text_align_prop = chxj_css_get_property_value(doc, style, "text-align");
      css_property_t *decoration_prop = chxj_css_get_property_value(doc, style, "text-decoration");
      css_property_t *display_prop = chxj_css_get_property_value(doc, style, "display");
      css_property_t *marquee_dir_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-dir");
      css_property_t *marquee_style_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-style");
      css_property_t *marquee_loop_prop = chxj_css_get_property_value(doc, style, "-wap-marquee-loop");
      css_property_t *bgcolor_prop = chxj_css_get_property_value(doc, style, "background-color");
      
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
          if(strcmp(cur->value,"0") == 0 || strcmp(cur->value,"-1") == 0){
            attr_marquee_loop = "infinite";
          }
          else{
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
      for (cur = bgcolor_prop->next; cur != bgcolor_prop; cur = cur->next) {
        if (cur->value && *cur->value) {
          css_bgcolor = apr_pstrdup(doc->pool, cur->value);
        }
      }
    }
  }

  W_L("<span");
  if (attr_color || attr_size || attr_align || attr_blink || attr_marquee || css_bgcolor) {
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
    if(css_bgcolor){
      W_L("background-color:");
      W_V(css_bgcolor);
      W_L(";");
    }
    W_L("\"");
  }
  W_L(">");
  return jxhtml->out;
}


/**
 * It is a handler who processes the SPAN tag.
 *
 * @param pdoc  [i/o] The pointer to the JHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SPAN tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_end_span_tag(void *pdoc, Node *UNUSED(node))
{
  jxhtml_t *jxhtml = GET_JXHTML(pdoc);
  Doc *doc = jxhtml->doc;

  W_L("</span>");
  if (IS_CSS_ON(jxhtml->entryp)) {
    chxj_css_pop_prop_list(jxhtml->css_prop_stack);
  }
  return jxhtml->out;
}


/**
 * It is a handler who processes the STYLE tag.
 *
 * @param pdoc  [i/o] The pointer to the SoftBank XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The STYLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_jxhtml_style_tag(void *pdoc, Node *node)
{
  jxhtml_t     *jxhtml;
  Doc           *doc;
  Attr          *attr;
  char          *type = NULL;

  jxhtml = GET_JXHTML(pdoc);
  doc     = jxhtml->doc;

  if (! IS_CSS_ON(jxhtml->entryp)) {
    return jxhtml->out;
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
      jxhtml->style = chxj_css_parse_style_value(doc, jxhtml->style, value);
      DBG(doc->r, "end load CSS. value:[%s]", value);
    }
  }
  return jxhtml->out;
}
/*
 * vim:ts=2 et
 */
