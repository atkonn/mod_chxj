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
#include <apr_time.h>
#include "chxj_chtml30.h"
#include "chxj_hdml.h"
#include "chxj_str_util.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"

#define GET_CHTML30(X) ((chtml30_t*)(X))
#define W30_L(X)          do { chtml30->out = BUFFERED_WRITE_LITERAL(chtml30->out, &doc->buf, (X)); } while(0)
#define W30_V(X)          do { chtml30->out = (X) ? BUFFERED_WRITE_VALUE(chtml30->out, &doc->buf, (X))  \
                                                  : BUFFERED_WRITE_LITERAL(chtml30->out, &doc->buf, ""); } while(0)

static char* s_chtml30_start_html_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_html_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_meta_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_meta_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_textarea_tag (void* pdoc, Node* node);
static char* s_chtml30_end_textarea_tag   (void* pdoc, Node* node);
static char* s_chtml30_start_p_tag        (void* pdoc, Node* node);
static char* s_chtml30_end_p_tag          (void* pdoc, Node* node);
static char* s_chtml30_start_pre_tag      (void* pdoc, Node* node);
static char* s_chtml30_end_pre_tag        (void* pdoc, Node* node);
static char* s_chtml30_start_h1_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h1_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_h2_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h2_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_h3_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h3_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_h4_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h4_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_h5_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h5_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_h6_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_h6_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_ul_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_ul_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_ol_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_ol_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_li_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_li_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_head_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_head_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_title_tag    (void* pdoc, Node* node);
static char* s_chtml30_end_title_tag      (void* pdoc, Node* node);
static char* s_chtml30_start_base_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_base_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_body_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_body_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_a_tag        (void* pdoc, Node* node);
static char* s_chtml30_end_a_tag          (void* pdoc, Node* node);
static char* s_chtml30_start_br_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_br_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_tr_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_tr_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_font_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_font_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_form_tag     (void* pdoc, Node* node);
static char* s_chtml30_end_form_tag       (void* pdoc, Node* node);
static char* s_chtml30_start_input_tag    (void* pdoc, Node* node);
static char* s_chtml30_end_input_tag      (void* pdoc, Node* node);
static char* s_chtml30_start_center_tag   (void* pdoc, Node* node);
static char* s_chtml30_end_center_tag     (void* pdoc, Node* node);
static char* s_chtml30_start_hr_tag       (void* pdoc, Node* node);
static char* s_chtml30_end_hr_tag         (void* pdoc, Node* node);
static char* s_chtml30_start_img_tag      (void* pdoc, Node* node);
static char* s_chtml30_end_img_tag        (void* pdoc, Node* node);
static char* s_chtml30_start_select_tag   (void* pdoc, Node* node);
static char* s_chtml30_end_select_tag     (void* pdoc, Node* node);
static char* s_chtml30_start_option_tag   (void* pdoc, Node* node);
static char* s_chtml30_end_option_tag     (void* pdoc, Node* node);
static char* s_chtml30_start_div_tag      (void* pdoc, Node* node);
static char* s_chtml30_end_div_tag        (void* pdoc, Node* node);
static char* s_chtml30_chxjif_tag         (void* pdoc, Node* node); 
static char* s_chtml30_text_tag           (void* pdoc, Node* node);

static void  s_init_chtml30(chtml30_t* chtml, Doc* doc, request_rec* r, device_table* spec);

static int   s_chtml30_search_emoji(chtml30_t* chtml, char* txt, char** rslt);


tag_handler chtml30_handler[] = {
  /* tagHTML */
  {
    s_chtml30_start_html_tag,
    s_chtml30_end_html_tag,
  },
  /* tagMETA */
  {
    s_chtml30_start_meta_tag,
    s_chtml30_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_chtml30_start_textarea_tag,
    s_chtml30_end_textarea_tag,
  },
  /* tagP */
  {
    s_chtml30_start_p_tag,
    s_chtml30_end_p_tag,
  },
  /* tagPRE */
  {
    s_chtml30_start_pre_tag,
    s_chtml30_end_pre_tag,
  },
  /* tagUL */
  {
    s_chtml30_start_ul_tag,
    s_chtml30_end_ul_tag,
  },
  /* tagLI */
  {
    s_chtml30_start_li_tag,
    s_chtml30_end_li_tag,
  },
  /* tagOL */
  {
    s_chtml30_start_ol_tag,
    s_chtml30_end_ol_tag,
  },
  /* tagH1 */
  {
    s_chtml30_start_h1_tag,
    s_chtml30_end_h1_tag,
  },
  /* tagH2 */
  {
    s_chtml30_start_h2_tag,
    s_chtml30_end_h2_tag,
  },
  /* tagH3 */
  {
    s_chtml30_start_h3_tag,
    s_chtml30_end_h3_tag,
  },
  /* tagH4 */
  {
    s_chtml30_start_h4_tag,
    s_chtml30_end_h4_tag,
  },
  /* tagH5 */
  {
    s_chtml30_start_h5_tag,
    s_chtml30_end_h5_tag,
  },
  /* tagH6 */
  {
    s_chtml30_start_h6_tag,
    s_chtml30_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_chtml30_start_head_tag,
    s_chtml30_end_head_tag,
  },
  /* tagTITLE */
  {
    s_chtml30_start_title_tag,
    s_chtml30_end_title_tag,
  },
  /* tagBASE */
  {
    s_chtml30_start_base_tag,
    s_chtml30_end_base_tag,
  },
  /* tagBODY */
  {
    s_chtml30_start_body_tag,
    s_chtml30_end_body_tag,
  },
  /* tagA */
  {
    s_chtml30_start_a_tag,
    s_chtml30_end_a_tag,
  },
  /* tagBR */
  {
    s_chtml30_start_br_tag,
    s_chtml30_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_chtml30_start_tr_tag,
    s_chtml30_end_tr_tag,
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
    s_chtml30_start_font_tag,
    s_chtml30_end_font_tag,
  },
  /* tagFORM */
  {
    s_chtml30_start_form_tag,
    s_chtml30_end_form_tag,
  },
  /* tagINPUT */
  {
    s_chtml30_start_input_tag,
    s_chtml30_end_input_tag,
  },
  /* tagCENTER */
  {
    s_chtml30_start_center_tag,
    s_chtml30_end_center_tag,
  },
  /* tagHR */
  {
    s_chtml30_start_hr_tag,
    s_chtml30_end_hr_tag,
  },
  /* tagIMG */
  {
    s_chtml30_start_img_tag,
    s_chtml30_end_img_tag,
  },
  /* tagSELECT */
  {
    s_chtml30_start_select_tag,
    s_chtml30_end_select_tag,
  },
  /* tagOPTION */
  {
    s_chtml30_start_option_tag,
    s_chtml30_end_option_tag,
  },
  /* tagDIV */
  {
    s_chtml30_start_div_tag,
    s_chtml30_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_chtml30_chxjif_tag,
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
    s_chtml30_text_tag,
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
    NULL,
    NULL,
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
};


/**
 * converts from CHTML5.0 to CHTML3.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_chtml30(
  request_rec*        r,
  device_table*       spec,
  const char*         src,
  apr_size_t          srclen,
  apr_size_t*         dstlen,
  chxjconvrule_entry* entryp,
  cookie_t*           cookie
)
{
  char*     dst = NULL;
  char*     ss;
  chtml30_t chtml30;
  Doc       doc;

  DBG(r, "start chxj_exchange_chtml30()");

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"i found qrcode xml");
    DBG(r, "end chxj_exchange_chtml30()");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml30(&chtml30, &doc, r, spec);

  chtml30.entryp = entryp;
  chtml30.cookie = cookie;

  ap_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0, srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML3.0", ss, srclen);
#endif
  chxj_buffered_write_init(r->pool, &doc.buf);

  qs_parse_string(&doc,ss, strlen(ss));

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_exchange(spec,r,(void*)&chtml30, &doc, qs_get_root(&doc), 0);
  chtml30.out = chxj_buffered_write_flush(chtml30.out, &doc.buf);
  dst = apr_pstrdup(r->pool, chtml30.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL) {
    dst = apr_pstrdup(r->pool,ss);
  }
  if (strlen(dst) == 0) {
    dst = apr_psprintf(r->pool, "\n");
  }

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML3.0", dst, *dstlen);
#endif

  DBG(r, "end chxj_exchange_chtml30()");
  return dst;
}


/**
 * The CHTML structure is initialized.
 *
 * @param chtml30 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_chtml30(chtml30_t* chtml30, Doc* doc, request_rec* r, device_table* spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml30, 0, sizeof(chtml30_t));

  doc->r        = r;
  chtml30->doc  = doc;
  chtml30->spec = spec;
  chtml30->out  = qs_alloc_zero_byte_string(r);
  chtml30->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  chtml30->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml30   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_chtml30_search_emoji(chtml30_t* chtml30, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
  int           len;

  spec = chtml30->spec;

  len = strlen(txt);
  r   = chtml30->doc->r;

  if (!spec)
    DBG(r,"spec is NULL");

  for (ee = chtml30->conf->emoji;
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
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_html_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
#if 0
  chtml30->out = apr_pstrcat(r->pool, chtml30->out, "<html>\n", NULL);
#else
  W30_L("<html>\n");
#endif

  return chtml30->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_html_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

#if 0
  chtml30->out = apr_pstrcat(r->pool, chtml30->out, "</html>\n", NULL);
#else
  W30_L("</html>\n");
#endif

  return chtml30->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_meta_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;
  int           content_type_flag;
  int           refresh_flag;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  content_type_flag = 0;
  refresh_flag      = 0;

  W30_L("<meta");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name   = qs_get_attr_name(doc,attr);
    value  = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'h':
    case 'H':
      if (strcasecmp(name, "http-equiv") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W30_L(" http-equiv=\"");
        W30_V(value);
        W30_L("\"");
        if (STRCASEEQ('c','C',"content-type", value))
          content_type_flag = 1;
  
        if (STRCASEEQ('r','R',"refresh", value))
          refresh_flag = 1;
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0) {
        if (content_type_flag) {
          W30_L(" ");
          W30_V(name);
          W30_L("=\"text/html; charset=Windows-31J\"");
        }
        else
        if (refresh_flag) {
          char* buf;
          char* sec;
          char* url;
  
          buf = apr_pstrdup(r->pool, value);
  
          url = strchr(buf, ';');
          if (url) {
            sec = apr_pstrdup(r->pool, buf);
            sec[url-buf] = 0;
            url++;
            url = chxj_encoding_parameter(r, url);
            url = chxj_add_cookie_parameter(r, url, chtml30->cookie);
            W30_L(" ");
            W30_V(name);
            W30_L("=\"");
            W30_V(sec);
            W30_L(";");
            W30_V(url);
            W30_L("\"");
          }
        }
        else {
          W30_L(" ");
          W30_V(name);
          W30_L("=\"");
          W30_V(value);
          W30_L("\"");
        }
      }
      break;

    default:
      break;
    }
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_meta_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_head_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<head>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_head_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</head>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_title_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<title>");

  return chtml30->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_title_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</title>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_base_tag(void* pdoc, Node* node) 
{
  Attr*         attr;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<base");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('h','H',"href", name)) {
      W30_L(" href=\"");
      W30_V(value);
      W30_L("\"");
    }
  }

  W30_L(" >\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_base_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_body_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;


  W30_L("<body");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;

    name   = qs_get_attr_name(doc,attr);
    value  = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('b','B', "bgcolor", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" bgcolor=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('t','T', "text", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" text=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('l','L',"link", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" link=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('a','A',"alink", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  W30_L(">\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_body_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</body>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_a_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<a");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W30_L(" name=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('h','H',"href", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml30->cookie);
      W30_L(" href=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W30_L(" accesskey=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('c','C',"cti", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" cti=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('i','I',"ijam", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      W30_L(" utn ");
    }
    else if (STRCASEEQ('t','T',"telbook", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('k','K',"kana", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('e','E',"email", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('i','I',"ista", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_a_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</a>");

  return chtml30->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_br_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<br>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_br_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param chtml30  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_tr_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_tr_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<br>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_font_tag(void* pdoc, Node* node) 
{
  Attr*         attr;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<font");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('c','C',"color", name)) {
      W30_L(" color=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('s','S',"size", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_font_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</font>");

  return chtml30->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_form_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<form");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A',"action", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml30->cookie);
      W30_L(" action=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('m','M',"method", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" method=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      W30_L(" utn ");
    }
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_form_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</form>");

  return chtml30->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_input_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  chtml30   = GET_CHTML30(pdoc);
  doc       = chtml30->doc;
  r         = doc->r;


  W30_L("<input");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/

  type       = qs_get_type_attr(doc, node, r);
  name       = qs_get_name_attr(doc, node, r);
  value      = qs_get_value_attr(doc,node,r);
  istyle     = qs_get_istyle_attr(doc,node,r);
  max_length = qs_get_maxlength_attr(doc,node,r);
  checked    = qs_get_checked_attr(doc,node,r);
  accesskey  = qs_get_accesskey_attr(doc, node, r);
  size       = qs_get_size_attr(doc, node, r);

  if (type) {
    W30_L(" type=\"");
    W30_V(type);
    W30_L("\"");
  }

  if (size) {
    W30_L(" size=\"");
    W30_V(size);
    W30_L("\" ");
  }

  if (name) {
    W30_L(" name=\"");
    W30_V(name);
    W30_L("\" ");
  }

  if (value) {
    W30_L(" value=\"");
    W30_V(value);
    W30_L("\" ");
  }

  if (accesskey) {
    W30_L(" accesskey=\"");
    W30_V(accesskey);
    W30_L("\" ");
  }

  if (istyle != NULL) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    W30_L(" istyle=\"");
    W30_V(istyle);
    W30_L("\" ");
  }

  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length) {
    if (chxj_chk_numeric(max_length) != 0)
      max_length = apr_psprintf(r->pool, "0");

    if (istyle != NULL && strcasecmp(istyle, "1") == 0) {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length) * 2);
      W30_V(vv);
    }
    else  {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length));
      W30_V(vv);
    }
  }

  if (checked) {
    W30_L(" checked ");
  }

  W30_L(" >");

  return chtml30->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_input_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_center_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<center>");

  return chtml30->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_center_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</center>");

  return chtml30->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_hr_tag(void* pdoc, Node* node) 
{
  Attr*         attr;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<hr ");
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A',"align", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" align=\"");
      W30_V(value);
      W30_L("\" ");
    }
    else if (STRCASEEQ('s','S',"size", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" size=\"");
      W30_V(value);
      W30_L("\" ");
    }
    else if (STRCASEEQ('w','W',"width", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" width=\"");
      W30_V(value);
      W30_L("\" ");
    }
    else if (STRCASEEQ('n','N',"noshade", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" noshade ");
    }
    else if (STRCASEEQ('c','C',"color", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  W30_L(" >");

  return chtml30->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_hr_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_img_tag(void* pdoc, Node* node) 
{
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table *spec;
#endif
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;
#ifndef IMG_NOT_CONVERT_FILENAME
  spec    = chtml30->spec;
#endif

  W30_L("<img");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"src", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml30->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
        W30_L(" src=\"");
        W30_V(value);
        W30_L("\"");
      }

#else
      value = chxj_img_conv(r,spec,value);
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml30->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
      W30_L(" src=\"");
      W30_V(value);
      W30_L("\"");
#endif
    }
    else if (STRCASEEQ('a','A',"align", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" align=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('w','W',"width", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" width=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('h','H', "height", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" height=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('h','H',"hspace", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" hspace=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('v','V',"vspace", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" vspace=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('a','A',"alt",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W30_L(" alt=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('a','A',"align", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_img_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  return chtml30->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_select_tag(void* pdoc, Node* child)
{

  char*         size;
  char*         name;
  Attr*         attr;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;
  size    = NULL;
  name    = NULL;

  W30_L("<select");

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"size", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
  }

  if (size) {
    W30_L(" size=\"");
    W30_V(size);
    W30_L("\"");
  }

  if (name) {
    W30_L(" name=\"");
    W30_V(name);
    W30_L("\"");
  }

  W30_L(">\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_select_tag(void* pdoc, Node* UNUSED(child))
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc   = chtml30->doc;
  r     = doc->r;

  W30_L("</select>\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_option_tag(void* pdoc, Node* child)
{
  Attr*         attr;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  char*         selected;
  char*         value;

  chtml30    = GET_CHTML30(pdoc);
  doc        = chtml30->doc;
  r          = doc->r;
  selected   = NULL;
  value      = NULL;

  W30_L("<option");

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"selected", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (value) {
    W30_L(" value=\"");
    W30_V(value);
    W30_L("\"");
  }
  else {
    W30_L(" value=\"\"");
  }

  if (selected) {
    W30_L(" selected");
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_option_tag(void* pdoc, Node* UNUSED(child))
{
  chtml30_t*    chtml30;

  chtml30 = GET_CHTML30(pdoc);

  /* Don't close */

  return chtml30->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_div_tag(void *pdoc, Node *child)
{
  Attr          *attr;
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;
  char          *align;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;
  align   = NULL;

  W30_L("<div");

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A',"align", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (align) {
    W30_L(" align=\"");
    W30_V(align);
    W30_L("\"");
  }

  W30_L(">");

  return chtml30->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_div_tag(void* pdoc, Node* UNUSED(node))
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</div>\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_ul_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<ul>");

  return chtml30->out;
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
s_chtml30_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</ul>");

  return chtml30->out;
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
s_chtml30_start_pre_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  chtml30->pre_flag++;
  W30_L("<pre>");

  return chtml30->out;
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
s_chtml30_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</pre>");
  chtml30->pre_flag--;

  return chtml30->out;
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
s_chtml30_start_p_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<p>");

  return chtml30->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_p_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</p>");

  return chtml30->out;
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
s_chtml30_start_ol_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<ol>");

  return chtml30->out;
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
s_chtml30_end_ol_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</ol>");

  return chtml30->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_li_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<li>");

  return chtml30->out;
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
s_chtml30_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</li>");

  return chtml30->out;
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
s_chtml30_start_h1_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml30_t     *chtml30;
  Doc           *doc;
  request_rec   *r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h1>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h1_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</h1>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_h2_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h2>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h2_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</h2>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_h3_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h3>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h3_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc   = chtml30->doc;
  r     = doc->r;

  W30_L("</h3>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_h4_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h4>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h4_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</h4>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_h5_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h5>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h5_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</h5>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_h6_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("<h6>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_h6_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</h6>\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the TEXTARE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_start_textarea_tag(void* pdoc, Node* node) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  chtml30->textarea_flag++;

  W30_L("<textarea ");

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name", name)) {
      W30_L(" name=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('r','R',"rows", name)) {
      W30_L(" rows=\"");
      W30_V(value);
      W30_L("\"");
    }
    else if (STRCASEEQ('c','C',"cols", name)) {
      W30_L(" cols=\"");
      W30_V(value);
      W30_L("\"");
    }
  }

  W30_L(">\r\n");

  return chtml30->out;
}


/**
 * It is a handler who processes the TEXTAREA tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml30_end_textarea_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  W30_L("</textarea>\r\n");
  chtml30->textarea_flag--;

  return chtml30->out;
}


static char*
s_chtml30_chxjif_tag(void* pdoc, Node* node)
{
  Node*         child;
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W30_V(child->otext);
    s_chtml30_chxjif_tag(chtml30, child);
  }

  return NULL;
}

static char*
s_chtml30_text_tag(void* pdoc, Node* child)
{
  chtml30_t*    chtml30;
  Doc*          doc;
  request_rec*  r;

  char*   textval;
  char*   tmp;
  char*   tdst;
  char    one_byte[2];
  int     ii;
  int     tdst_len;

  chtml30 = GET_CHTML30(pdoc);
  doc     = chtml30->doc;
  r       = doc->r;
  
  textval = qs_get_node_value(doc,child);
  textval = qs_trim_string(chtml30->doc->r, textval);
  if (strlen(textval) == 0)
    return chtml30->out;
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(r);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_chtml30_search_emoji(chtml30, &textval[ii], &out);
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
    else if (chtml30->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (chtml30->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  W30_V(tdst);

  return chtml30->out;
}

/*
 * vim:ts=2 et
 */
