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
#include <libgen.h>
#include <limits.h>
#include "mod_chxj.h"
#include "chxj_img_conv_format.h"
#include "chxj_specified_device.h"
#include "chxj_str_util.h"
#include "chxj_qr_code.h"
#include "chxj_apply_convrule.h"
#include "chxj_url_encode.h"
#include "qs_parse_string.h"
#include "chxj_preg_replace.h"

#include "http_core.h"

#include <wand/magick_wand.h>

/* @see htcacheclean */
#define CACHEDATA_EXT "data"

#if !defined(LONG_LONG_MAX) && defined(LLONG_MAX)
#  define LONG_LONG_MAX LLONG_MAX
#endif


#define EXIT_MAGICK_ERROR() \
  do { \
    char *description; ExceptionType severity; \
    description=MagickGetException(magick_wand,&severity); \
    ap_log_rerror(APLOG_MARK,APLOG_ERR, 0, r,"%s %s %d %s\n",__FILE__,(__func__),__LINE__,description); \
    description=(char *) MagickRelinquishMemory(description); \
    DestroyMagickWand(magick_wand); \
  } while(0) 

typedef enum img_conv_mode_t {
  IMG_CONV_MODE_NORMAL = 0,
  IMG_CONV_MODE_THUMBNAIL,
  IMG_CONV_MODE_WALLPAPER,
  IMG_CONV_MODE_EZGET,
} img_conv_mode_t;

/*----------------------------------------------------------------------------*/
/* User-Agent use flag                                                        */
/*----------------------------------------------------------------------------*/
typedef enum _ua_use_flag_t {
  UA_USE=0,               /* User-Agent is used.                              */
  UA_IGN,                 /* User-Agent is disregarded.                       */
} ua_use_flag_t;

/*----------------------------------------------------------------------------*/
/* Request parameter maintenance structure                                    */
/*----------------------------------------------------------------------------*/
typedef struct query_string_param_t query_string_param_t;

struct query_string_param_t {
  img_conv_mode_t   mode;
  char              *user_agent;
  ua_use_flag_t     ua_flag;

  char              *name;      /* for EZGET */
  long              offset;    /* for EZGET */
  long              count;     /* for EZGET */
  int               width;
  int               height;
};

/*----------------------------------------------------------------------------*/
/* Device_spec when User-Agent is disregarded                                 */
/*----------------------------------------------------------------------------*/
static device_table v_ignore_spec = {
  NULL,
  0,
  "IGN",
  "IGN",
  CHXJ_SPEC_HTML,
  640,
  480,
  640,
  480,
  1024*1024,
  1,
  1,
  1,
  0,
  0,
  96,
  96,
  65536,
  NULL,
  "Shift_JIS"
};

/*----------------------------------------------------------------------------*/
/* CRC calculation table for AU                                               */
/*----------------------------------------------------------------------------*/
static unsigned short  AU_CRC_TBL[256] = {
  0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
  0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
  0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
  0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
  0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
  0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
  0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
  0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
  0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
  0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
  0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
  0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
  0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
  0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
  0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
  0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
  0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
  0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
  0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
  0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
  0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
  0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
  0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
  0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
  0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
  0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
  0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
  0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
  0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
  0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
  0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
  0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0 
};

/*----------------------------------------------------------------------------*/
/* Download page for AU                                                       */
/*----------------------------------------------------------------------------*/
static const char *HDML_FIRST_PAGE = 
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <NODISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=GOSUB DEST=\"device:data/dnld?url=%s&name=%s%s&size=%ld&disposition=%s&title=%s\">\r\n"
  "  </NODISPLAY>\r\n"
  "</HDML>\r\n";

static const char *HDML_SUCCESS_PAGE =
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <DISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=RETURN>\r\n"
  "    \x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc9\x90\xac\x8c\xf7\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n"
  "  </DISPLAY>\r\n"
  "<HDML>\r\n";

static const char *HDML_FAIL_PAGE =
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <DISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=RETURN>\r\n"
  "    \x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n"
  "  </DISPLAY>\r\n"
  "<HDML>\r\n";

static ap_regex_t *v_docomo_serial_pattern1 = NULL;
static ap_regex_t *v_docomo_serial_pattern2 = NULL;
static ap_regex_t *v_docomo_serial_pattern3 = NULL;
static ap_regex_t *v_softbank_serial_pattern1 = NULL;

/*----------------------------------------------------------------------------*/
/* Prototype declaration                                                      */
/*----------------------------------------------------------------------------*/
static char *s_create_workfile_name(request_rec *, 
                                    mod_chxj_config *, 
                                    const char *,
                                    query_string_param_t *);

static apr_status_t s_create_cache_file(request_rec          *r, 
                                        const char           *tmpfile, 
                                        device_table         *spec,
                                        apr_finfo_t          *st,
                                        query_string_param_t *qsp,
                                        mod_chxj_config      *conf);

static apr_status_t s_send_cache_file(mod_chxj_config       *conf,
                                      device_table          *spec,
                                      query_string_param_t  *query_string,
                                      request_rec           *r,
                                      const char            *tmpfile);

static apr_status_t s_send_original_file(request_rec *r, 
                                         const char  *originalfile);

static apr_status_t s_header_only_cache_file(device_table         *spec, 
                                             query_string_param_t *query_string, 
                                             request_rec          *r, 
                                             const char           *tmpfile);

static query_string_param_t *s_get_query_string_param(request_rec *r);

static unsigned short s_add_crc(const char *writedata, apr_size_t witebyte);

static MagickWand *s_fixup_size(MagickWand   *, 
                                request_rec  *r, 
                                device_table *spec, 
                                query_string_param_t *qsp);

static MagickWand *s_fixup_color(MagickWand *magick_wand, 
                                 request_rec *r, 
                                 device_table *spec, 
                                 img_conv_mode_t mode);
static MagickWand *s_fixup_depth(MagickWand* magick_wand, 
                                 request_rec* r, device_table* spec);
static MagickWand *s_img_down_sizing(MagickWand *magick_wand, 
                                request_rec *r, device_table *spec);

static MagickWand *s_add_copyright(MagickWand *magick_wand,
                                   request_rec *r,
                                   device_table *spec);

static char *s_create_blob_data(request_rec *r,
                                device_table *spec,
                                query_string_param_t *qsp,
                                char *indata,
                                apr_size_t *len);

static int s_img_conv_format_from_file(request_rec          *r, 
                                       mod_chxj_config      *conf, 
                                       const char           *user_agent,
                                       query_string_param_t *qsp,
                                       device_table         *spec);
static int s_convert_to_jpeg(MagickWand *magick_wand, request_rec *r, device_table *spec);
static int s_convert_to_png(MagickWand *maigck_wand, request_rec *r, device_table *spec);
static int s_convert_to_gif(MagickWand *magick_wand, request_rec *r, device_table *spec);
static int s_convert_to_bmp(MagickWand *magick_wand, request_rec *r, device_table *spec);
static int s_chxj_trans_name2(request_rec *r);
static char *s_add_comment_to_png(request_rec *r, char *data, apr_size_t *len);



int 
chxj_img_conv_format_handler(request_rec *r)
{
  mod_chxj_config       *conf;
  mod_chxj_req_config   *req_conf;
  query_string_param_t  *qsp;
  char                  *user_agent;
  device_table          *spec;
  chxjconvrule_entry    *entryp;
  int                   rtn;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  s_chxj_trans_name2(r);
  
  if (! r->handler || 
      (r->handler && !STRCASEEQ('c','C',"chxj-picture",r->handler) && !STRCASEEQ('c','C',"chxj-qrcode",r->handler))) {
    DBG(r,"REQ[%X] Response Code:[%d]", TO_ADDR(r), r->status);
    DBG(r,"REQ[%X] r->handler is[%s]", TO_ADDR(r), r->handler);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }

  qsp = s_get_query_string_param(r);
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  req_conf = chxj_get_req_config(r);
  if (conf == NULL) {
    DBG(r,"REQ[%X] conf is null",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }

  if (STRCASEEQ('c','C',"chxj-qrcode",r->handler) && conf->image == CHXJ_IMG_OFF) {
    DBG(r,"REQ[%X] chxj-qrcode and ImageEngineOff", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }

  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* User-Agent to spec                                                       */
  /*--------------------------------------------------------------------------*/
  if (qsp->user_agent) {
    user_agent = apr_pstrdup(r->pool, qsp->user_agent);
  }
  else {
    /*-------------------------------------------------------------------------*/
    /* already setup entryp if request_conf->user_agent is not null            */
    /*-------------------------------------------------------------------------*/
    if (req_conf->user_agent) {
      entryp = req_conf->entryp;
    }
    else {
      entryp = chxj_apply_convrule(r, conf->convrules);
    }
    if (entryp && entryp->user_agent) {
      user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
    }
    else {
      user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
    }
  }



  if (qsp->ua_flag == UA_IGN)
    spec = &v_ignore_spec;
  else {
    if (user_agent) {
      if (!req_conf->spec || !req_conf->user_agent) {
        spec = chxj_specified_device(r, user_agent);
      }
      else if (req_conf->user_agent && strcmp(req_conf->user_agent, user_agent) != 0) {
        spec = chxj_specified_device(r, user_agent);
      }
      else {
        spec = req_conf->spec;
      }
    }
  }

  DBG(r,"REQ[%X] found device_name=[%s]", TO_ADDR(r), spec->device_name);
  DBG(r,"REQ[%X] User-Agent=[%s]", TO_ADDR(r), user_agent);


  /*-------------------------------------------------------------------------*/
  /* Do not process output_filter                                            */
  /*-------------------------------------------------------------------------*/
  chxj_remove_filter(r);
  rtn = s_img_conv_format_from_file(r, conf, user_agent, qsp, spec);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return rtn;
}



/**
 * It converts it from ImageData corresponding to each model.
 *
 * @param r   [i]
 * @param src [i]   It is former image binary data.
 * @param len [i/o] It is length of former image binary data.
 */
char *
chxj_convert_image(request_rec *r, const char **src, apr_size_t *len)
{
  mod_chxj_config       *conf;
  query_string_param_t  *qsp;
  char                  *user_agent;
  device_table          *spec;
  char                  *dst;
  char                  *conv_check;
  chxjconvrule_entry    *entryp;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  conv_check = (char*)apr_table_get(r->headers_in, "CHXJ_IMG_CONV");
  if (conv_check) {
    DBG(r,"REQ[%X] already convert.", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }


  qsp = s_get_query_string_param(r);
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (conf == NULL) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }

  mod_chxj_req_config *req_conf = chxj_get_req_config(r);
  /*--------------------------------------------------------------------------*/
  /* User-Agent to spec                                                       */
  /*--------------------------------------------------------------------------*/
  if (qsp->user_agent) {
    user_agent = apr_pstrdup(r->pool, qsp->user_agent);
  }
  else {
    /*-------------------------------------------------------------------------*/
    /* already setup entryp if request_conf->user_agent is not null            */
    /*-------------------------------------------------------------------------*/
    if (req_conf->user_agent) {
      entryp = req_conf->entryp;
    }
    else {
      entryp = chxj_apply_convrule(r, conf->convrules);
    }
    if (entryp && entryp->user_agent) {
      user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
    }
    else {
      user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
    }
  }

  if (qsp->ua_flag == UA_IGN)
    spec = &v_ignore_spec;
  else {
    if (user_agent) {
      if (!req_conf->spec || !req_conf->user_agent) {
        spec = chxj_specified_device(r, user_agent);
      }
      else if (req_conf->user_agent && strcmp(req_conf->user_agent, user_agent) != 0) {
        spec = chxj_specified_device(r, user_agent);
      }
      else {
        spec = req_conf->spec;
      }
    }
  }

  DBG(r,"REQ[%X] found device_name=[%s]",TO_ADDR(r),spec->device_name);
  DBG(r,"REQ[%X] User-Agent=[%s]",TO_ADDR(r),user_agent);

  if (spec->width == 0 || spec->heigh == 0) {
    DBG(r,"REQ[%X] not convert (width or height is 0)",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }

  dst = s_create_blob_data(r, spec, qsp, (char*)*src, len);
  if (dst == NULL) 
    *len = 0;

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);

  return dst;
}


static int
s_img_conv_format_from_file(
                request_rec          *r, 
                mod_chxj_config      *conf, 
                const char           *user_agent,
                query_string_param_t *qsp,
                device_table         *spec)
{
  apr_status_t   rv;
  apr_finfo_t    st;
  apr_finfo_t    cache_st;
  char           *tmpfile;
  int            try_count;

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    /* 
     * If ``ua'' parameter is specified, it must be CHXJ_SPEC_HTML. 
     */
    return s_send_original_file(r, r->filename);
  }

  /*--------------------------------------------------------------------------*/
  /* Create Workfile Name                                                     */
  /*--------------------------------------------------------------------------*/
  tmpfile = s_create_workfile_name(r, conf, user_agent, qsp);
  DBG(r,"REQ[%X] workfile=[%s]", TO_ADDR(r), tmpfile);

  rv = apr_stat(&st, r->filename, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  apr_table_setn(r->headers_in, "CHXJ_IMG_CONV", "done");
  try_count = CACHE_RETRY_COUNT;
  do {
    rv = apr_stat(&cache_st, tmpfile, APR_FINFO_MIN, r->pool);
  
    if (rv != APR_SUCCESS || cache_st.ctime < st.mtime) {
      /*------------------------------------------------------------------------*/
      /* It tries to make the cash file when it doesn't exist or there is       */
      /* change time later since the making time of the cash file.              */
      /*------------------------------------------------------------------------*/
      rv = s_create_cache_file(r,tmpfile, spec, &st, qsp, conf);
      if (rv != OK)
        return rv;
    }
  
    DBG(r,"REQ[%X] color=[%d]", TO_ADDR(r), spec->color);
    if (! r->header_only)  {
      rv = s_send_cache_file(conf, spec, qsp,r, tmpfile);
    }
    else {
      rv = s_header_only_cache_file(spec, qsp, r, tmpfile);
    }
    if (rv == OK) break;
    if (rv == HTTP_NOT_FOUND) {
      DBG(r,"REQ[%X] recheck wait... try_count[%d]", TO_ADDR(r),try_count);
      apr_sleep(CACHE_RECHECK_WAIT);
    }
  } while (try_count--); 
  if (try_count <= 0) {
    WRN(r, "cache retry failure....");
    WRN(r, "cache file was deleted...");
  }

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);

  return rv;
}


static apr_status_t
s_create_cache_file(request_rec          *r, 
                    const char           *tmpfile, 
                    device_table         *spec, 
                    apr_finfo_t          *st, 
                    query_string_param_t *qsp,
                    mod_chxj_config      *conf)
{
  apr_status_t       rv;
  apr_size_t         readbyte;
  apr_size_t         writebyte;
  unsigned short     crc;
  img_conv_mode_t    mode = qsp->mode;

  char *writedata = NULL;
  char *readdata  = NULL;

  apr_file_t  *fout;
  apr_file_t  *fin;
  apr_finfo_t cache_dir_st;

  MagickWand *magick_wand;
  int        img_count;

  if (STRCASEEQ('c','C',"chxj-qrcode",r->handler)) {
    /*------------------------------------------------------------------------*/
    /* QRCODE用のファイルの場合                                               */
    /*------------------------------------------------------------------------*/
    Doc       doc;
    Node      *root;
    qr_code_t qrcode;
    int       sts;

    memset(&doc,    0, sizeof(Doc));
    memset(&qrcode, 0, sizeof(qr_code_t));
    doc.r = r;
    doc.parse_mode  = PARSE_MODE_CHTML;
    qrcode.doc      = &doc;
    qrcode.r        = r;

    qs_init_malloc(&doc);

    root = qs_parse_file(&doc, r->filename);

    chxj_qrcode_node_to_qrcode(&qrcode, root);

    qs_all_free(&doc,QX_LOGMARK);

    sts = chxj_qrcode_create_image_data(&qrcode, &readdata, &readbyte);
    if (sts != OK) {
      ERR(r, "qrcode create failed.");
      return sts;
    }
  }
  else {
    /*------------------------------------------------------------------------*/
    /* 通常のイメージファイルの場合                                           */
    /*------------------------------------------------------------------------*/
#if APR_HAS_MMAP
    DBG(r, "REQ[%X] Use mmap to read original image.", TO_ADDR(r));
    {
      apr_finfo_t finfo;
      apr_mmap_t *mmap = NULL;
      rv = apr_file_open(&fin, 
                         r->filename, 
                         APR_FOPEN_READ | APR_FOPEN_SHARELOCK, 
                         APR_OS_DEFAULT, 
                         r->pool);
      if (rv != APR_SUCCESS) {
        char buf[256];
        ERR(r,"REQ[%X] %s:%d apr_file_open failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        ERR(r,"REQ[%X] %s:%d file open failed.[%s]", TO_ADDR(r),__FILE__,__LINE__,r->filename);
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      rv = apr_file_info_get(&finfo, APR_FINFO_SIZE, fin);
      if (rv != APR_SUCCESS) {
        char buf[256];
        apr_file_close(fin);
        ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      rv = apr_mmap_create(&mmap, fin, 0, finfo.size, APR_MMAP_READ, r->pool);
      if (rv != APR_SUCCESS) {
        char buf[256];
        apr_file_close(fin);
        ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      readdata = apr_palloc(r->pool, st->size);
      readbyte = st->size;
      memcpy(readdata, mmap->mm, st->size);
      //apr_mmap_delete(mmap);
      apr_file_close(fin);
    }
#else
    rv = apr_file_open(&fin, 
                    r->filename, 
                    APR_FOPEN_READ | APR_FOPEN_BINARY | APR_FOPEN_BUFFERED | APR_FOPEN_SHARELOCK,
                    APR_OS_DEFAULT, 
                    r->pool);
    if (rv != APR_SUCCESS) {
      DBG(r,"REQ[%X] file open failed.[%s]", TO_ADDR(r),r->filename);
      return HTTP_NOT_FOUND;
    }
  
    readdata = apr_palloc(r->pool, st->size);
    rv = apr_file_read_full(fin, (void*)readdata, st->size, &readbyte);
    apr_file_close(fin);
    if (rv != APR_SUCCESS || readbyte != st->size) {
      DBG(r,"REQ[%X] file read failed.[%s]", TO_ADDR(r), r->filename);
      apr_file_close(fin);
  
      return HTTP_NOT_FOUND;
    }
#endif
  }
  DBG(r,"REQ[%X] start img convert", TO_ADDR(r));


  magick_wand = NewMagickWand();
  if (MagickReadImageBlob(magick_wand,readdata, readbyte) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return HTTP_NOT_FOUND;
  }

  /*------------------*/
  /* for AnimationGIF */
  /*------------------*/
  img_count = MagickGetNumberImages(magick_wand);
  DBG(r, "REQ[%X] img_count is [%d]", TO_ADDR(r), img_count);
  if (img_count > 1) {
    MagickSetImageIndex(magick_wand, 0);
    MagickWand *magick_wand2 = MagickGetImage(magick_wand);
    DestroyMagickWand(magick_wand);
    magick_wand = magick_wand2;
  }

  if (MagickStripImage(magick_wand) == MagickFalse) {
    ERR(r, "mod_chxj: strip image failure.");
    EXIT_MAGICK_ERROR();
    return HTTP_NOT_FOUND;
  }
  {
    MagickWand *magick_wand2;
    if ((magick_wand2 = MagickCoalesceImages(magick_wand)) == NULL) {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    DestroyMagickWand(magick_wand);
    magick_wand = magick_wand2;
  }

  if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
    int oldw = MagickGetImageWidth(magick_wand);
    int oldh = MagickGetImageHeight(magick_wand);
    int done_fixup_size = 0;
    int ww = spec->width;
    int hh = spec->heigh;
    if (IS_IPHONE(spec) || IS_ANDROID(spec)) {
      ww = (int)((double)ww * (double)1.5);
      hh = (int)((double)hh * (double)1.5);
      if (IS_ANDROID(spec)) {
        ww = ww - ADJUST_WIDTH_FOR_ANDROID;
      }
    }
    if ((qsp->mode == IMG_CONV_MODE_WALLPAPER && spec->wp_width < oldw && spec->wp_heigh < oldh)
      || (qsp->mode != IMG_CONV_MODE_WALLPAPER && ww < oldw && hh < oldh)) {
      /*
       * The size of the image is changed.
       */
      DBG(r,"REQ[%X] call s_fixup_size()", TO_ADDR(r));
  
      if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL) 
        return HTTP_NOT_FOUND;

      done_fixup_size = 1;
    }
    /*
     * The colors of the image is changed.
     */
    DBG(r,"REQ[%X] call s_fixup_color()", TO_ADDR(r));
  
    if ((magick_wand = s_fixup_color(magick_wand, r,spec, mode)) == NULL) 
      return HTTP_NOT_FOUND;
  
    /*
     * DEPTH of the image is changed.
     */
    DBG(r,"REQ[%X] call s_fixup_depth()", TO_ADDR(r));
  
    if ((magick_wand = s_fixup_depth(magick_wand, r, spec)) == NULL) 
      return HTTP_NOT_FOUND;
  
  
    if (! done_fixup_size) {
      /*
       * The size of the image is changed.
       */
      DBG(r,"REQ[%X] call s_fixup_size()", TO_ADDR(r));
      if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL) 
        return HTTP_NOT_FOUND;
    }

    char *nowFormat = MagickGetImageFormat(magick_wand);
    int fixFormatFlag = 0;
    if (nowFormat) {
      if (STRCASEEQ('g','G',"gif",nowFormat)) {
        if (spec->available_gif) {
          if (s_convert_to_gif(magick_wand, r, spec)) {
            return HTTP_NOT_FOUND;
          }
          fixFormatFlag = 1;
        }
      }
      else if (STRCASEEQ('j','J',"jpg",nowFormat)||STRCASEEQ('j','J',"jpeg",nowFormat)) {
        if (spec->available_jpeg) {
          if (s_convert_to_jpeg(magick_wand, r, spec)) {
            return HTTP_NOT_FOUND;
          }
          fixFormatFlag = 1;
        }
      }
      else if (STRCASEEQ('p','P',"png",nowFormat)) {
        if (spec->available_png) {
          if (s_convert_to_png(magick_wand, r, spec)) {
            return HTTP_NOT_FOUND;
          }
          fixFormatFlag = 1;
        }
      }
      else if (STRCASEEQ('b','B',"bmp",nowFormat)) {
        if (spec->available_bmp4 || spec->available_bmp2) {
          if (s_convert_to_bmp(magick_wand, r, spec)) {
            return HTTP_NOT_FOUND;
          }
          fixFormatFlag = 1;
        }
      }
    }

    DBG(r,"REQ[%X] start convert and compression", TO_ADDR(r));
  
    if (! fixFormatFlag) {
      if (spec->available_jpeg) {
        if (s_convert_to_jpeg(magick_wand, r, spec)) {
          return HTTP_NOT_FOUND;
        }
      }
      else if (spec->available_gif) {
        if (s_convert_to_gif(magick_wand, r, spec)) {
          return HTTP_NOT_FOUND;
        }  
      }
      else if (spec->available_png) {
        if (s_convert_to_png(magick_wand, r, spec)) {
          return HTTP_NOT_FOUND;
        }  
  
      }
      else
      if (spec->available_bmp2 || spec->available_bmp4) { 
        if (s_convert_to_bmp(magick_wand, r, spec)) {
          return HTTP_NOT_FOUND;
        }
      }
    }
  
    /*
     * Add Comment (Copyright and so on.)
     */
    DBG(r,"REQ[%X] call s_add_copyright()",TO_ADDR(r));
  
    if ((magick_wand = s_add_copyright(magick_wand, r, spec)) == NULL) 
      return HTTP_NOT_FOUND;
  }
  else {
    char* fmt;
    fmt = MagickGetImageFormat(magick_wand);
    if (fmt == NULL) {
      if (MagickSetImageFormat(magick_wand, "jpg") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      r->content_type = apr_psprintf(r->pool, "image/jpeg");
    }
    else {
      if (strcasecmp(fmt, "jpg") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/jpeg");
      }
      else
      if (strcasecmp(fmt, "jpeg") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/jpeg");
      }
      else
      if (strcasecmp(fmt, "gif") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/gif");
      }
      else
      if (strcasecmp(fmt, "png") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/png");
      }
    }
  }

  char *sv_writedata;
  sv_writedata = writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

  if (! writebyte) {
    DestroyMagickWand(magick_wand);
    ERR(r,"convert failure to Jpeg [%s]", tmpfile);

    return HTTP_INTERNAL_SERVER_ERROR;
  }
  writedata = apr_palloc(r->pool, writebyte);
  memcpy(writedata, sv_writedata, writebyte);

  DBG(r,"REQ[%X] end convert and compression",TO_ADDR(r));

  /* Added PNG Comment if type is image/png. */
  if (r->content_type && strcmp(r->content_type, "image/png") == 0) {
    if ((writedata = s_add_comment_to_png(r, writedata, &writebyte)) == NULL) {
      DBG(r,"REQ[%X] Add comment to PNG failure.",TO_ADDR(r));
      DestroyMagickWand(magick_wand);
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
  }
  
  /* check limit */
  rv = apr_stat(&cache_dir_st, conf->image_cache_dir, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    ERR(r,"dir stat error.[%s]", conf->image_cache_dir);
    if (sv_writedata) free(sv_writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  
  for (;;) {
    /* delete candidate */
    apr_finfo_t dcf;   
    /* get dir files size */
    apr_dir_t *dir;
    unsigned long total_size = 0;
    int found_file = 0;
    unsigned long max_size = (! conf->image_cache_limit) ? DEFAULT_IMAGE_CACHE_LIMIT : conf->image_cache_limit;
    char *delete_file_name;

    rv = apr_dir_open(&dir, conf->image_cache_dir, r->pool);
    if (rv != APR_SUCCESS) { 
      DestroyMagickWand(magick_wand);
      ERR(r,"dir open error.[%s]", conf->image_cache_dir);
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    memset(&dcf, 0, sizeof(apr_finfo_t));
    dcf.atime = (apr_time_t)LONG_LONG_MAX;
    for (;;) {
      apr_finfo_t dirf;
      rv = apr_dir_read(&dirf, APR_FINFO_SIZE|APR_FINFO_NAME|APR_FINFO_DIRENT|APR_FINFO_ATIME , dir);
      if (rv != APR_SUCCESS) {
        break;
      }
      if (dirf.name && strcmp(dirf.name, ".") != 0 && strcmp(dirf.name, "..") != 0) {
        total_size += (unsigned long)dirf.size;
        DBG(r,"REQ[%x] dirf.name=[%s] dirf.size=[%ld] dirf.atime=[%lld]", TO_ADDR(r),dirf.name, (long)dirf.size, (long long int)dirf.atime);
        if (dcf.atime >= dirf.atime) {
          memcpy(&dcf, &dirf, sizeof(apr_finfo_t));
        }
        found_file++;
      }
    }
    apr_dir_close(dir);
    if (total_size + writebyte < max_size) {
      DBG(r,"REQ[%X] There is an enough size in cache. total_size:[%lu] max_size:[%lu] found_file=[%d] max_size=[%lu]", TO_ADDR(r),total_size, max_size, found_file, max_size);
      break;
    }
    if (found_file == 0 && writebyte >= max_size) {
      ERR(r,"REQ[%X] ==========================================",TO_ADDR(r));
      ERR(r,"REQ[%X] cache space is too small...",TO_ADDR(r));
      ERR(r,"REQ[%X] At least the same size as %luByte is necessary for me.", TO_ADDR(r),(unsigned long)writebyte); 
      ERR(r,"REQ[%X] Please specify the ChxjImageCacheLimit that is larger than now value. ",TO_ADDR(r));
      ERR(r,"REQ[%X] ==========================================",TO_ADDR(r));
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    DBG(r,"REQ[%X] Image Cache dir is full. total_size:[%lu] max_size:[%lu]",TO_ADDR(r),total_size + writebyte, max_size);
    /* search delete candidate */
    delete_file_name = apr_psprintf(r->pool, "%s/%s", conf->image_cache_dir, dcf.name);
    DBG(r,"REQ[%X] delete image cache target:[%s] atime:[%lld]", TO_ADDR(r),delete_file_name, (long long int)dcf.atime);
    rv = apr_file_remove(delete_file_name, r->pool);
    if (rv != APR_SUCCESS) {
      ERR(r,"REQ[%X] cache file delete failure.[%s]", TO_ADDR(r),delete_file_name);
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    DBG(r,"REQ[%X] deleted image cache target:[%s]", TO_ADDR(r),delete_file_name);
    if (total_size + writebyte - dcf.size < max_size) {
      DBG(r,"REQ[%X] OK, there is an enough size in cache.",TO_ADDR(r));
      break;
    }
  }

  /* to cache */
  rv = apr_file_open(&fout, tmpfile,
                  APR_FOPEN_WRITE| APR_FOPEN_CREATE | APR_FOPEN_BINARY | APR_SHARELOCK ,
                  APR_OS_DEFAULT,
                  r->pool);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    ERR(r,"REQ[%X] file open error.[%s]", TO_ADDR(r), tmpfile);
    if (sv_writedata) free(sv_writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  rv = apr_file_write(fout, (void*)writedata, &writebyte);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    apr_file_close(fout);
    if (sv_writedata) free(sv_writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  /*
   * CRC is added for AU for EzGET.
   */
  if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
  ||  spec->html_spec_type == CHXJ_SPEC_Hdml            ) {

    crc = s_add_crc(writedata, writebyte);

    rv = apr_file_putc((crc >> 8)  & 0xff, fout);
    if (rv != APR_SUCCESS) {
      DestroyMagickWand(magick_wand);
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }

    rv = apr_file_putc( crc        & 0xff, fout);
    if (rv != APR_SUCCESS) {
      DestroyMagickWand(magick_wand);
      if (sv_writedata) free(sv_writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
  }

  DestroyMagickWand(magick_wand);
  if (sv_writedata) free(sv_writedata);

  rv = apr_file_close(fout);
  if (rv != APR_SUCCESS) {
    DBG(r,"REQ[%X] file write error.[%s]",TO_ADDR(r),tmpfile);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  return OK;
}


static int
s_convert_to_jpeg(MagickWand *magick_wand, request_rec *r, device_table *spec) 
{
  if (MagickSetImageCompression(magick_wand,JPEGCompression) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickSetImageFormat(magick_wand, "jpg") == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickStripImage(magick_wand) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
    return -1;
  
  r->content_type = apr_psprintf(r->pool, "image/jpeg");
  ap_set_content_type(r, "image/jpeg");
  DBG(r,"REQ[%X] convert to jpg",TO_ADDR(r));
  return 0;
}


static int
s_convert_to_png(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  if (MagickSetImageCompression(magick_wand,ZipCompression) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickSetImageFormat(magick_wand, "png") == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickStripImage(magick_wand) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
    return -1;
  
  r->content_type = apr_psprintf(r->pool, "image/png");
  ap_set_content_type(r, "image/png");
  DBG(r,"REQ[%X] convert to png",TO_ADDR(r));
  return 0;
}


static int
s_convert_to_gif(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  if (MagickSetImageCompression(magick_wand,LZWCompression) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickSetImageFormat(magick_wand, "gif") == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickStripImage(magick_wand) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
    return -1;
  
  r->content_type = apr_psprintf(r->pool, "image/gif");
  ap_set_content_type(r, "image/gif");
  
  DBG(r,"REQ[%X] convert to gif",TO_ADDR(r));
  return 0;
}


static int
s_convert_to_bmp(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  if (MagickSetImageCompression(magick_wand,NoCompression) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickSetImageFormat(magick_wand, "bmp") == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if (MagickStripImage(magick_wand) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return -1;
  }
  
  if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
    return -1;
  
  r->content_type = apr_psprintf(r->pool, "image/bmp");
  ap_set_content_type(r, "image/bmp");
  
  DBG(r,"REQ[%X] convert to bmp(unsupported)",TO_ADDR(r));
  return 0;
}


static char *
s_create_blob_data(request_rec          *r, 
                   device_table         *spec, 
                   query_string_param_t *qsp,
                   char                 *indata,
                   apr_size_t           *len)
{
  apr_size_t         writebyte;
  unsigned short     crc;
  img_conv_mode_t    mode = qsp->mode;

  char *writedata = NULL;
  char *dst       = NULL;
  MagickWand *magick_wand;

  magick_wand = NewMagickWand();

  if (MagickReadImageBlob(magick_wand,indata, *len) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return NULL;
  }

  if (MagickStripImage(magick_wand) == MagickFalse) {
    ERR(r, "mod_chxj: strip image failure.");
    EXIT_MAGICK_ERROR();
    return NULL;
  }

  {
    int oldw = MagickGetImageWidth(magick_wand);
    int oldh = MagickGetImageHeight(magick_wand);
    int done_fixup_size = 0;
    int ww = spec->width;
    int hh = spec->heigh;
    if (IS_IPHONE(spec) || IS_ANDROID(spec)) {
      ww = (int)((double)ww * (double)1.5);
      hh = (int)((double)hh * (double)1.5);
      if (IS_ANDROID(spec)) {
        ww = ww - ADJUST_WIDTH_FOR_ANDROID;
      }
    }
    if ((qsp->mode == IMG_CONV_MODE_WALLPAPER && spec->wp_width < oldw && spec->wp_heigh < oldh)
      || (qsp->mode != IMG_CONV_MODE_WALLPAPER && ww < oldw && hh < oldh)) {
      /*
       * The size of the image is changed.
       */
      DBG(r,"REQ[%X] call s_fixup_size()",TO_ADDR(r));

      if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }

      done_fixup_size = 1;
    }
    /*
     * The colors of the image is changed.
     */
    DBG(r,"REQ[%X] call s_fixup_color()",TO_ADDR(r));

    if ((magick_wand = s_fixup_color(magick_wand, r,spec, mode)) == NULL) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    /*
     * DEPTH of the image is changed.
     */
    DBG(r,"REQ[%X] call s_fixup_depth()",TO_ADDR(r));

    if ((magick_wand = s_fixup_depth(magick_wand, r, spec)) == NULL) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }


    if (! done_fixup_size) {
      /*
       * The size of the image is changed.
       */
      DBG(r,"REQ[%X] call s_fixup_size()",TO_ADDR(r));
      if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
  }

  char *nowFormat = MagickGetImageFormat(magick_wand);
  int fixFormatFlag = 0;
  if (nowFormat) {
    if (STRCASEEQ('g','G',"gif",nowFormat)) {
      if (spec->available_gif) {
        if (s_convert_to_gif(magick_wand, r, spec)) {
          return NULL;
        }
        fixFormatFlag = 1;
      }
    }
    else if (STRCASEEQ('j','J',"jpg",nowFormat)||STRCASEEQ('j','J',"jpeg",nowFormat)) {
      if (spec->available_jpeg) {
        if (s_convert_to_jpeg(magick_wand, r, spec)) {
          return NULL;
        }
        fixFormatFlag = 1;
      }
    }
    else if (STRCASEEQ('p','P',"png",nowFormat)) {
      if (spec->available_png) {
        if (s_convert_to_png(magick_wand, r, spec)) {
          return NULL;
        }
        fixFormatFlag = 1;
      }
    }
  }

  DBG(r,"REQ[%X] start convert and compression",TO_ADDR(r));

  if (!fixFormatFlag) {
    if (spec->available_jpeg) {
      if (s_convert_to_jpeg(magick_wand, r, spec)) {
        return NULL;
      }
    }
    else if (spec->available_png) {
      if (s_convert_to_png(magick_wand, r, spec)) {
        return NULL;
      }
    }
    else if (spec->available_gif) {
      if (s_convert_to_gif(magick_wand, r, spec)) {
        return NULL;
      }
    }
    else if (spec->available_bmp2 || spec->available_bmp4) {
      if (s_convert_to_bmp(magick_wand, r, spec)) {
        return NULL;
      }
    }
  }
  /*--------------------------------------------------------------------------*/
  /* Add Comment (Copyright and so on.)                                       */
  /*--------------------------------------------------------------------------*/
  DBG(r,"REQ[%X] call s_add_copyright()",TO_ADDR(r));

  if ((magick_wand = s_add_copyright(magick_wand, r, spec)) == NULL)
    return NULL;

  writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

  if (! writebyte) {
    DestroyMagickWand(magick_wand);
    DBG(r,"REQ[%X] convert failure to Jpeg ",TO_ADDR(r));
    return NULL;
  }

  DBG(r,"REQ[%X] end convert and compression",TO_ADDR(r));


  
  dst = apr_palloc(r->pool, writebyte+2);

  memcpy(dst, writedata, writebyte);
  /*--------------------------------------------------------------------------*/
  /* CRC is added for AU for EzGET.                                           */
  /*--------------------------------------------------------------------------*/
  if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
  ||  spec->html_spec_type == CHXJ_SPEC_Hdml) {
    crc = s_add_crc(writedata, writebyte);
    dst[writebyte + 0] = (crc >> 8) & 0xff;
    dst[writebyte + 1] = (crc     ) & 0xff;
    writebyte += 2;
  }

  DestroyMagickWand(magick_wand);

  *len = writebyte;
  return dst;
}


static MagickWand *
s_fixup_size(MagickWand           *magick_wand, 
             request_rec          *r, 
             device_table         *spec, 
             query_string_param_t *qsp)
{
  img_conv_mode_t mode = qsp->mode;
  int oldw;
  int oldh;
  int neww;
  int newh;
  int c_width;
  int c_heigh;

  oldw = MagickGetImageWidth(magick_wand);
  oldh = MagickGetImageHeight(magick_wand);

  DBG(r,"REQ[%X] detect width=[%d]", TO_ADDR(r),oldw);
  DBG(r,"REQ[%X] detect heigh=[%d]", TO_ADDR(r),oldh);

  neww = oldw;
  newh = oldh;

  DBG(r,"REQ[%X] detect spec width=[%d]", TO_ADDR(r),spec->width);
  DBG(r,"REQ[%X] detect spec heigh=[%d]", TO_ADDR(r),spec->heigh);

  c_width = spec->width;
  c_heigh = spec->heigh;

  if (IS_IPHONE(spec) || IS_ANDROID(spec)) {
    c_width = (int)((double)c_width * (double)1.5);
    c_heigh = (int)((double)c_heigh * (double)1.5);
    if (IS_ANDROID(spec)) {
      c_width = c_width - ADJUST_WIDTH_FOR_ANDROID;
    }
    DBG(r,"REQ[%X] detect iphone/android width=[%d]", TO_ADDR(r),c_width);
    DBG(r,"REQ[%X] detect iphone/android heigh=[%d]", TO_ADDR(r),c_heigh);
  }

  switch(mode) {
  case IMG_CONV_MODE_THUMBNAIL:

    DBG(r,"REQ[%X] **** detect thumbnail mode ****",TO_ADDR(r));

    if (neww > c_width) {
      newh = (int)((double)newh * (double)((double)c_width / (double)neww));
      neww = (int)((double)neww * (double)((double)c_width / (double)neww));
    }
    if (newh > c_heigh) {
      neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
      newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
    }

    neww = (int)((double)(neww / 3) * 0.8);
    newh = (int)((double)(newh / 3) * 0.8);
    break;

  case IMG_CONV_MODE_WALLPAPER:
  case IMG_CONV_MODE_EZGET:

    DBG(r,"REQ[%X] **** detect wallpaper mode ****",TO_ADDR(r));

    if (spec->wp_width && spec->wp_heigh) {
      c_width = spec->wp_width;
      c_heigh = spec->wp_heigh;
    }

    DBG(r,"REQ[%X] calc new width and height",TO_ADDR(r));

    neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
    newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));

    DBG(r,"REQ[%X] newh = [%d] neww = [%d]", TO_ADDR(r),newh, neww);
    break;

  default:

    DBG(r,"REQ[%X] **** detect normal mode ****",TO_ADDR(r));

    if (qsp->ua_flag != UA_IGN && spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
      if (neww > c_width) {
        newh = (int)((double)newh * (double)((double)c_width / (double)neww));
        neww = (int)((double)neww * (double)((double)c_width / (double)neww));
      }

      if (newh > c_heigh) {
        neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
        newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
      }
    }
    break;
  }

  if (neww == 0) neww = 1;
  if (newh == 0) newh = 1;

  if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
    DBG(r,"REQ[%X] convert width=[%d --> %d]", TO_ADDR(r),oldw, neww);
    DBG(r,"REQ[%X] convert heigh=[%d --> %d]", TO_ADDR(r),oldh, newh);
  
    MagickResetIterator(magick_wand);
  
    while (MagickNextImage(magick_wand) != MagickFalse) {
      switch (mode) {
      case IMG_CONV_MODE_WALLPAPER:
      case IMG_CONV_MODE_EZGET:
  
        if (MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
  
        if (MagickCropImage(magick_wand, 
                        (unsigned long)c_width, 
                        (unsigned long)c_heigh,
                        (long)((neww - c_width) / 2),
                        (long)((newh - c_heigh) / 2)) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
  
      case IMG_CONV_MODE_NORMAL:
        if (qsp->width) {
          DBG(r,"REQ[%X] convert width=[%d --> %d]", TO_ADDR(r),neww, qsp->width);
          neww = qsp->width;
        }
        if (qsp->height) {
          DBG(r,"REQ[%X] convert heigh=[%d --> %d]", TO_ADDR(r),newh, qsp->height);
          newh = qsp->height;
        }
  
      default:
        if (MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
      }
  
      if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
        if (MagickSetImageUnits(magick_wand, PixelsPerInchResolution) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
    
        if (MagickSetImageResolution(magick_wand,
                                     (double)spec->dpi_width,
                                     (double)spec->dpi_heigh) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
    
        if (MagickSetImageDispose(magick_wand, BackgroundDispose) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
      }
    }
  }
  return magick_wand;
}


static MagickWand *
s_fixup_color(MagickWand *magick_wand, request_rec *r, device_table *spec, img_conv_mode_t UNUSED(mode))
{
  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r,"REQ[%X] Pass s_fixup_color proc",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return magick_wand;
  }

  unsigned long colors = MagickGetImageColors(magick_wand);
  DBG(r,"REQ[%X] now color:[%ld] spec->color:[%ld]", TO_ADDR(r),colors, (unsigned long)spec->color);
  if (colors < (unsigned long)spec->color) {
    DBG(r,"REQ[%X] Pass s_fixup_color proc. color:[%ld] spec->color:[%d]", TO_ADDR(r),colors, spec->color);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return magick_wand;
  }

  if (spec->color >= 256) {

    DBG(r,"REQ[%X] call MagickQuantizeImage() spec->color=[%d]",TO_ADDR(r),spec->color);

    if (MagickQuantizeImage(magick_wand,
                           spec->color,
                           RGBColorspace,
                           4,   /* tree depth of 4 */
                           1,
                           0) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    DBG(r,"REQ[%X] call end MagickQuantizeImage() spec->color=[%d]",TO_ADDR(r),spec->color);

  }
  else {
    DBG(r,"REQ[%X] call MagickQuantizeImage() spec->color=[%d]",TO_ADDR(r),spec->color);

    if (MagickQuantizeImage(magick_wand,
                           spec->color,
                           GRAYColorspace,
                           4,   /* tree depth of 4 */
                           1,
                           0) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    DBG(r,"REQ[%X] call end MagickQuantizeImage() spec->color=[%d]",TO_ADDR(r),spec->color);
  }

  {
    if (r->server != NULL && r->server->loglevel == APLOG_DEBUG) {
      colors = MagickGetImageColors(magick_wand);
      DBG(r, "REQ[%X] fixup_color result colors:[%u]",TO_ADDR(r), colors);
    }
  }


  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);

  return magick_wand;
}



static MagickWand *
s_fixup_depth(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r,"REQ[%X] Pass s_fixup_depth proc",TO_ADDR(r));
    return magick_wand;
  }

  if (spec->color == 15680000) {
    if (MagickSetImageDepth(magick_wand, 24) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else 
  if (spec->color == 262144) {
    if (MagickSetImageDepth(magick_wand, 18) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 65536) {
    if (MagickSetImageDepth(magick_wand, 16) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4096) {
    if (MagickSetImageDepth(magick_wand, 12) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 256) {
    if (MagickSetImageDepth(magick_wand, 8) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4) {
    if (MagickSetImageDepth(magick_wand, 2) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 2) {
    if (MagickSetImageDepth(magick_wand, 1) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }

  return magick_wand;
}


static MagickWand *
s_add_copyright(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  mod_chxj_config *conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r,"REQ[%X] Pass add_copiright proc",TO_ADDR(r));
    return magick_wand;
  }

  if (conf->image_copyright) {

    DBG(r,"REQ[%X] Add COPYRIGHT [%s]", TO_ADDR(r),conf->image_copyright);

    if (spec->html_spec_type == CHXJ_SPEC_Jhtml
    ||  spec->html_spec_type == CHXJ_SPEC_Jxhtml) {
      apr_table_setn(r->headers_out, "x-jphone-copyright", "no-transfer");
      if (MagickCommentImage(magick_wand, 
                             apr_psprintf(r->pool, 
                                          "Copyright(C) %s", 
                                          conf->image_copyright)) == MagickFalse) 
        goto on_error;
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
    ||  spec->html_spec_type == CHXJ_SPEC_Hdml) {
      if (MagickCommentImage(magick_wand, 
                             apr_psprintf(r->pool, 
                                         "kddi_copyright=on,%s", 
                                          conf->image_copyright)) == MagickFalse) 
        goto on_error;
    }
    else {
      if (MagickCommentImage(magick_wand, 
                            apr_psprintf(r->pool, 
                                         "copy=\"NO\",%s",
                                         conf->image_copyright)) == MagickFalse)
        goto on_error;
    }
  }
  else {
    if (MagickCommentImage(magick_wand, "mod_chxj") == MagickFalse)
      goto on_error;
  }
  return magick_wand;

on_error:
  EXIT_MAGICK_ERROR();
  return NULL;
}

static MagickWand *
s_img_down_sizing(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  MagickBooleanType  status;
  unsigned long      quality = 70;
  apr_size_t         writebyte = 0;
  char               *writedata;
  apr_size_t         prev_size = 0;
  char               *fmt;
  int                fmt_type = 0;
  
  writedata = (char *)MagickGetImageBlob(magick_wand, &writebyte);
  prev_size = writebyte;

  
  fmt = MagickGetImageFormat(magick_wand);
  if (fmt) {
    if (STRCASEEQ('j','J',"jpg",fmt)) fmt_type = 1;
    if (STRCASEEQ('p','P',"png",fmt)) fmt_type = 2;
    if (STRCASEEQ('g','G',"gif",fmt)) fmt_type = 3;
    if (STRCASEEQ('b','B',"bmp",fmt)) fmt_type = 4;
  }
  if (fmt_type == 1) {
    do {
      if (MagickSetImageCompressionQuality(magick_wand, quality) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
  
      writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);
      if (writebyte >= prev_size) {
        DBG(r,"REQ[%X] quality=[%ld] size=[%d]", TO_ADDR(r),(long)quality, (int)writebyte);
        quality += 5;
        if (quality > 100) {
          if (MagickSetImageCompression(magick_wand,NoCompression) == MagickFalse) {
            EXIT_MAGICK_ERROR();
            return NULL;
          }
          break;
        }
        if (MagickSetImageCompressionQuality(magick_wand, quality) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
      }
  
      DBG(r,"REQ[%X] quality=[%ld] size=[%d]", TO_ADDR(r),(long)quality, (int)writebyte);
  
      if (spec->cache == 0)
        break;
  
      if (writebyte <= (unsigned int)spec->cache)
        break;
  
      quality -= 5;
  
      if (quality == 0 || quality > 100)
        break;
  
    }
    while (1);
  }


  if (spec->cache > 0 
  &&  writebyte   > (unsigned int)spec->cache) {
    unsigned long now_color = spec->color;
    unsigned long depth     = 0;
    do {
      switch(now_color) {
      case 2:      depth = 1; break;
      case 4:      now_color = 2;        depth = 1;  break;
      case 8:      now_color = 4;        depth = 2;  break;
      case 16:     now_color = 8;        depth = 3;  break;
      case 256:    now_color = 16;       depth = 4;  break;
      case 4096:   now_color = 256;      depth = 8;  break;
      case 65536:  now_color = 4096;     depth = 12; break;
      case 262144: now_color = 65536;    depth = 16; break;
      case 15680000: now_color = 262144; depth = 18; break;
      default:
        now_color = 2;
        break;
      }

      if (now_color <= 2) break;


      if (now_color >= 8) {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             RGBColorspace,
                             0,
                             1,
                             0);
      }
      else {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             GRAYColorspace,
                             0,
                             1,
                             0);
        MagickSetImageType(magick_wand, GrayscaleType);
      }
      if (status == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
      if (fmt_type != 2) {
        if (MagickSetImageDepth(magick_wand, depth) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
      }
      writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

      DBG(r,"REQ[%X] now_color=[%ld] size=[%d]",TO_ADDR(r),(long)now_color, (int)writebyte);

      /* Once per request */
      break;
    }
    while(now_color > 2);
  }

  return magick_wand;
}


static apr_status_t 
s_send_cache_file(
  mod_chxj_config      *conf,
  device_table         *spec, 
  query_string_param_t *query_string, 
  request_rec          *r, 
  const char           *tmpfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  apr_file_t   *fout;
  apr_size_t   sendbyte;
  char         *contentLength;
  char         *readdata;

  rv = apr_stat(&st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  DBG(r, "REQ[%X] mode:[%d]",    TO_ADDR(r), query_string->mode);
  DBG(r, "REQ[%X] name:[%s]",    TO_ADDR(r), query_string->name);
  DBG(r, "REQ[%X] offset:[%ld]", TO_ADDR(r), query_string->offset);
  DBG(r, "REQ[%X] count:[%ld]",  TO_ADDR(r), query_string->count);

  /* for mod_cache */
  {
    apr_table_setn(r->headers_out, "Vary", "User-Agent");
    apr_table_setn(r->err_headers_out, "Vary", "User-Agent");
    ap_update_mtime(r, st.mtime);
    ap_set_last_modified(r);
  }

  if (query_string->mode != IMG_CONV_MODE_EZGET && query_string->name == NULL) {
    contentLength = apr_psprintf(r->pool, "%d", (int)st.size);
    apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
    DBG(r,"REQ[%X] Content-Length:[%d]", TO_ADDR(r),(int)st.size);
    readdata = apr_palloc(r->pool, st.size);
#if APR_HAS_MMAP
    DBG(r, "REQ[%X] Use mmap to read cache file", TO_ADDR(r));
    {
      apr_finfo_t finfo;
      apr_mmap_t *mmap = NULL;
      rv = apr_file_open(&fout, tmpfile, 
        APR_FOPEN_READ, 
        APR_OS_DEFAULT, r->pool);
      if (rv != APR_SUCCESS) {
        char buf[256];
        ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        ERR(r,"REQ[%X] %s:%d tmpfile open failed[%s]", TO_ADDR(r),__FILE__,__LINE__,tmpfile);
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      rv = apr_file_info_get(&finfo, APR_FINFO_SIZE, fout);
      if (rv != APR_SUCCESS) {
        char buf[256];
        apr_file_close(fout);
        ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      rv = apr_mmap_create(&mmap, fout, 0, finfo.size, APR_MMAP_READ, r->pool);
      if (rv != APR_SUCCESS) {
        char buf[256];
        apr_file_close(fout);
        ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
        return HTTP_NOT_FOUND;
      }
      memcpy(readdata, mmap->mm, st.size);
      //apr_mmap_delete(mmap);
      apr_file_close(fout);
    }
#else
    {
      apr_size_t   readbyte;
      rv = apr_file_open(&fout, tmpfile, 
        APR_FOPEN_READ | APR_FOPEN_BINARY | APR_FOPEN_BUFFERED | APR_FOPEN_SHARELOCK, 
        APR_OS_DEFAULT, r->pool);
      if (rv != APR_SUCCESS) {
        ERR(r, "REQ[%X] %s:%d cache file open failed[%s]", TO_ADDR(r),__FILE__,__LINE__,tmpfile);
        return HTTP_NOT_FOUND;
      }
      rv = apr_file_read_full(fout, (void*)readdata, st.size, &readbyte);
      apr_file_close(fout);
      if (rv != APR_SUCCESS) {
        char buf[256];
        ERR(r,"REQ[%X] %s:%d apr_file_read_full failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
        ERR(r,"REQ[%X] %s:%d tmpfile open failed[%s]", TO_ADDR(r),__FILE__,__LINE__,tmpfile);
        return HTTP_NOT_FOUND;
      }
    }
#endif

    MagickWand *magick_wand = NewMagickWand();
    if (MagickReadImageBlob(magick_wand,readdata, st.size) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return HTTP_NOT_FOUND;
    }
    if (MagickStripImage(magick_wand) == MagickFalse) {
      ERR(r, "mod_chxj: strip image failure.");
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    char *nowFormat = MagickGetImageFormat(magick_wand);
    DestroyMagickWand(magick_wand);
    char *fname = apr_pstrdup(r->pool, r->filename);
    char *ext = strrchr(fname, '.');
    if (nowFormat) {
      if (STRCASEEQ('j','J',"jpeg",nowFormat) || STRCASEEQ('j','J',"jpg",nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => jpg.",TO_ADDR(r));
        ap_set_content_type(r, "image/jpeg");

        if (!ext) {
          fname = apr_pstrcat(r->pool, fname, ".jpg");
        }
        else if (ext && strcasecmp(".jpeg", ext) != 0 && strcasecmp(".jpg", ext) != 0) {
          DBG(r, "REQ[%X]", TO_ADDR(r));
          *ext = 0;
          fname = apr_pstrcat(r->pool, fname, ".jpg");
        }
      }
      else if (STRCASEEQ('p','P',"png", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => png.",TO_ADDR(r));
        ap_set_content_type(r, "image/png");

        if (!ext) {
          fname = apr_pstrcat(r->pool, fname, ".png");
        }
        else if (ext && strcasecmp(".png", ext) != 0) {
          DBG(r, "REQ[%X]", TO_ADDR(r));
          *ext = 0;
          fname = apr_pstrcat(r->pool, fname, ".png");
        }

      }
      else if (STRCASEEQ('g','G',"gif", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => gif.",TO_ADDR(r));
        ap_set_content_type(r, "image/gif");

        if (!ext) {
          fname = apr_pstrcat(r->pool, fname, ".gif");
        }
        else if (ext && strcasecmp(".gif", ext) != 0) {
          DBG(r, "REQ[%X]", TO_ADDR(r));
          *ext = 0;
          fname = apr_pstrcat(r->pool, fname, ".gif");
        }
      }
      else if (STRCASEEQ('b','B',"bmp", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => bmp.",TO_ADDR(r));
        ap_set_content_type(r, "image/bmp");

        if (!ext) {
          fname = apr_pstrcat(r->pool, fname, ".bmp");
        }
        else if (ext && strcasecmp(".bmp", ext) != 0) {
          DBG(r, "REQ[%X]", TO_ADDR(r));
          *ext = 0;
          fname = apr_pstrcat(r->pool, fname, ".bmp");
        }
      }
      else {
        ERR(r,"REQ[%X] detect unknown file",TO_ADDR(r));
        return HTTP_NOT_FOUND;
      }
    }
    if (query_string->mode == IMG_CONV_MODE_WALLPAPER && (IS_IPHONE(spec)||IS_ANDROID(spec))) {
      ap_set_content_type(r, "application/octet-stream");
      apr_table_setn(r->headers_out, "Content-Disposition", apr_psprintf(r->pool, "attachment; filename=%s", fname));
      DBG(r,"REQ[%X] detect cache file => iphone/android",TO_ADDR(r));
    }
    if (conf->image_copyright) {
      DBG(r,"REQ[%X] Add COPYRIGHT Header for SoftBank [%s]", TO_ADDR(r), conf->image_copyright);
      if (spec->html_spec_type == CHXJ_SPEC_Jhtml ||  spec->html_spec_type == CHXJ_SPEC_Jxhtml) {
        apr_table_setn(r->headers_out, "x-jphone-copyright", "no-transfer");
      }
    }
    {
      conn_rec *c = r->connection;
      apr_bucket_brigade *bb = NULL;
      apr_bucket *b;
  
      bb = apr_brigade_create(r->pool, c->bucket_alloc);
      b = apr_bucket_heap_create(readdata, st.size, NULL, c->bucket_alloc);
      APR_BRIGADE_INSERT_TAIL(bb, b);
      ap_pass_brigade(r->output_filters, bb);
      sendbyte = st.size;
    }
    DBG(r, "REQ[%X] send file data[%d]byte", TO_ADDR(r), (int)sendbyte);
  }
  else
  if (query_string->mode == IMG_CONV_MODE_EZGET) {
    char *name = apr_pstrdup(r->pool, basename(r->filename));
    name[strlen(name)-4] = 0;
    if (strcasecmp(r->content_type, "image/jpeg") == 0) {

      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".jpg", (long)st.size, "devjaww", name);
    }
    else
    if (strcasecmp(r->content_type, "image/bmp") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".bmp", (long)st.size, "devabm", name);
    }
    else
    if (strcasecmp(r->content_type, "image/png") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".png", (long)st.size, "dev8aww", name);
    }
    else
    if (strcasecmp(r->content_type, "image/gif") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".gif", (long)st.size, "devgi0z", name);
    }
  }
  else
  if (query_string->mode == IMG_CONV_MODE_WALLPAPER && query_string->name != NULL) {
    if (query_string->count == -1 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_SUCCESS_PAGE);
      ap_rflush(r);
    }
    else
    if (query_string->count == -2 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FAIL_PAGE);
      ap_rflush(r);
    }
    else { 
      chxj_set_content_type(r, "application/x-up-download");
      contentLength = apr_psprintf(r->pool, "%ld", query_string->count);
      apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
      DBG(r,"REQ[%X] Content-Length:[%d]", TO_ADDR(r),(int)st.size);
#if APR_HAS_MMAP
      DBG(r, "REQ[%X] Use ap_send_mmap to send cache file", TO_ADDR(r));
      {
        apr_finfo_t finfo;
        apr_mmap_t *mmap;
        rv = apr_file_open(&fout, tmpfile, 
          APR_FOPEN_READ | APR_FOPEN_BINARY, 
          APR_OS_DEFAULT, r->pool);
        if (rv != APR_SUCCESS) {
          ERR(r,"REQ[%X] %s:%d tmpfile open failed[%s]", TO_ADDR(r),__FILE__,__LINE__,tmpfile);
          DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
          return HTTP_NOT_FOUND;
        }
        rv = apr_file_info_get(&finfo, APR_FINFO_SIZE, fout);
        if (rv != APR_SUCCESS) {
          char buf[256];
          apr_file_close(fout);
          ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
          DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
          return HTTP_NOT_FOUND;
        }
        rv = apr_mmap_create(&mmap, fout, 0, finfo.size, APR_MMAP_READ, r->pool);
        if (rv != APR_SUCCESS) {
          char buf[256];
          apr_file_close(fout);
          ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
          DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
          return HTTP_NOT_FOUND;
        }
        sendbyte = ap_send_mmap(mmap, r, query_string->offset, query_string->count);
        // apr_mmap_delete(mmap);
        apr_file_close(fout);
      }
      ap_rflush(r);
#else
      rv = apr_file_open(&fout, tmpfile, 
        APR_FOPEN_READ | APR_FOPEN_BINARY | APR_FOPEN_BUFFERED | APR_FOPEN_SHARELOCK, 
        APR_OS_DEFAULT, r->pool);

      if (rv != APR_SUCCESS) {
        DBG(r,"REQ[%X] tmpfile open failed[%s]", TO_ADDR(r),tmpfile);
        return HTTP_NOT_FOUND;
      }

      ap_send_fd(fout, r, query_string->offset, query_string->count, &sendbyte);
      apr_file_close(fout);
      ap_rflush(r);
#endif
      DBG(r,"REQ[%X] send file data[%d]byte", TO_ADDR(r),(int)sendbyte);
    }
  }
  
  return OK;
}


static apr_status_t 
s_send_original_file(request_rec *r, const char *originalfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  apr_file_t   *fout;
  apr_size_t   sendbyte = 0;

  rv = apr_stat(&st, originalfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  /* for mod_cache */
  {
    apr_table_setn(r->headers_out, "Vary", "User-Agent");
    apr_table_setn(r->err_headers_out, "Vary", "User-Agent");
    ap_update_mtime(r, st.mtime);
    ap_set_last_modified(r);
  }

#if APR_HAS_MMAP
  DBG(r, "REQ[%X] Use ap_send_mmap to send original image file", TO_ADDR(r));
  {
    apr_finfo_t finfo;
    apr_mmap_t *mmap = NULL;
    rv = apr_file_open(&fout, 
                       originalfile, 
                       APR_FOPEN_READ | APR_FOPEN_SHARELOCK, 
                       APR_OS_DEFAULT, r->pool);
    if (rv != APR_SUCCESS) {
      char buf[256];
      ERR(r,"REQ[%X] %s:%d apr_file_open failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
      ERR(r,"REQ[%X] %s:%d original file open failed[%s]", TO_ADDR(r),__FILE__,__LINE__,originalfile);
      DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
      return HTTP_NOT_FOUND;
    }
    rv = apr_file_info_get(&finfo, APR_FINFO_SIZE, fout);
    if (rv != APR_SUCCESS) {
      char buf[256];
      apr_file_close(fout);
      ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
      DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
      return HTTP_NOT_FOUND;
    }
    rv = apr_mmap_create(&mmap, fout, 0, finfo.size, APR_MMAP_READ, r->pool);
    if (rv != APR_SUCCESS) {
      char buf[256];
      apr_file_close(fout);
      ERR(r,"REQ[%X] %s:%d apr_file_info_get failed. [%s]", TO_ADDR(r),__FILE__,__LINE__,apr_strerror(rv,buf,256));
      DBG(r,"REQ[%X] end %s",TO_ADDR(r), __func__);
      return HTTP_NOT_FOUND;
    }
    sendbyte = ap_send_mmap(mmap, r, 0, st.size);
    //apr_mmap_delete(mmap);
    apr_file_close(fout);
  }
  ap_rflush(r);
#else
  rv = apr_file_open(&fout, originalfile, 
    APR_FOPEN_READ | APR_FOPEN_BINARY | APR_FOPEN_BUFFERED | APR_FOPEN_SHARELOCK,
    APR_OS_DEFAULT, r->pool);
  if (rv != APR_SUCCESS) {
    DBG(r,"REQ[%X] originalfile open failed[%s]", TO_ADDR(r),originalfile);
    return HTTP_NOT_FOUND;
  }

  ap_send_fd(fout, r, 0, st.size, &sendbyte);
  apr_file_close(fout);
  ap_rflush(r);
#endif
  DBG(r,"REQ[%x] send file data[%d]byte", TO_ADDR(r),(int)sendbyte);
  
  return OK;
}


static apr_status_t 
s_header_only_cache_file(device_table *spec, query_string_param_t *query_string, request_rec *r, const char *tmpfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  char         *contentLength;
  mod_chxj_config *conf = ap_get_module_config(r->per_dir_config, &chxj_module);

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  rv = apr_stat(&st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return HTTP_NOT_FOUND;
  }

  DBG(r, "REQ[%X] mode:[%d]",    TO_ADDR(r), query_string->mode);
  DBG(r, "REQ[%X] name:[%s]",    TO_ADDR(r), query_string->name);
  DBG(r, "REQ[%X] offset:[%ld]", TO_ADDR(r), query_string->offset);
  DBG(r, "REQ[%X] count:[%ld]",  TO_ADDR(r), query_string->count);

  if (query_string->mode != IMG_CONV_MODE_EZGET && query_string->name == NULL) {
    contentLength = apr_psprintf(r->pool, "%d", (int)st.size);
    apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);

    MagickWand *magick_wand = NewMagickWand();
    if (MagickReadImage(magick_wand,tmpfile) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    if (MagickStripImage(magick_wand) == MagickFalse) {
      ERR(r, "mod_chxj: strip image failure.");
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    char *nowFormat = MagickGetImageFormat(magick_wand);
    DestroyMagickWand(magick_wand);
    if (nowFormat) {
      if (STRCASEEQ('j','J',"jpeg",nowFormat) || STRCASEEQ('j','J',"jpg",nowFormat)) {
        DBG(r,"REQ[%x] detect cache file => jpg.",TO_ADDR(r));
        ap_set_content_type(r, "image/jpeg");
      }
      else if (STRCASEEQ('p','P',"png", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => png.",TO_ADDR(r));
        ap_set_content_type(r, "image/png");
      }
      else if (STRCASEEQ('g','G',"gif", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => gif.",TO_ADDR(r));
        ap_set_content_type(r, "image/gif");
      }
      else if (STRCASEEQ('b','B',"bmp", nowFormat)) {
        DBG(r,"REQ[%X] detect cache file => bmp.",TO_ADDR(r));
        ap_set_content_type(r, "image/bmp");
      }
      else {
        ERR(r, "REQ[%X] detect unknown file", TO_ADDR(r));
        return HTTP_NOT_FOUND;
      }
    }
  
    DBG(r,"REQ[%X] Content-Length:[%d]", TO_ADDR(r),(int)st.size);
  }
  else
  if (query_string->mode == IMG_CONV_MODE_EZGET) {
    char *name = apr_pstrdup(r->pool, basename(r->filename));
    name[strlen(name)-4] = 0;
    if (strcasecmp(r->content_type, "image/jpeg") == 0) {

      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/bmp") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/png") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/gif") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
  }
  else
  if (query_string->mode == IMG_CONV_MODE_WALLPAPER && query_string->name != NULL) {
    if (query_string->count == -1 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (query_string->count == -2 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else { 
      chxj_set_content_type(r, "application/x-up-download");
      contentLength = apr_psprintf(r->pool, "%ld", query_string->count);
      apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
      DBG(r, "REQ[%X] Content-Length:[%d]", TO_ADDR(r), (int)st.size);
    }
  }
  if (conf->image_copyright) {
    DBG(r, "REQ[%X] Add COPYRIGHT Header for SoftBank [%s]", TO_ADDR(r), conf->image_copyright);
    if (spec->html_spec_type == CHXJ_SPEC_Jhtml ||  spec->html_spec_type == CHXJ_SPEC_Jxhtml) {
      apr_table_setn(r->headers_out, "x-jphone-copyright", "no-transfer");
    }
  }
  
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return OK;
}


static void 
s_init_serial_pattern(apr_pool_t *p)
{
  if (!v_docomo_serial_pattern1) {
    v_docomo_serial_pattern1 = chxj_compile_for_preg_replace(p, "/ser[^;\\)]+");
  }  
  if (!v_docomo_serial_pattern2) {
    v_docomo_serial_pattern2 = chxj_compile_for_preg_replace(p, ";ser[^;\\)]+");
  }  
  if (!v_docomo_serial_pattern3) {
    v_docomo_serial_pattern3 = chxj_compile_for_preg_replace(p, ";icc[^;\\)]+");
  }  
  if (!v_softbank_serial_pattern1) {
    v_softbank_serial_pattern1 = chxj_compile_for_preg_replace(p, "/SN[0-9a-zA-Z]+ ");
  }  
}


static char *
s_create_workfile_name(
                request_rec          *r, 
                mod_chxj_config      *conf, 
                const char           *user_agent, 
                query_string_param_t *qsp)
{
  int  ii;
  int  jj;
  int  len;
  char *w = apr_palloc(r->pool, 256);
  char *fname;
  char *new_user_agent;

  s_init_serial_pattern(r->server->process->pool);

  /* for DoCoMo */
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern1, "", user_agent);
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern2, "", new_user_agent);
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern3, "", new_user_agent);

  /* for SoftBank */
  new_user_agent = chxj_preg_replace(r->pool, v_softbank_serial_pattern1, " ", new_user_agent);

  DBG(r,"REQ[%X] old user_agent:[%s] ==> new user_agent:[%s]", TO_ADDR(r),user_agent, new_user_agent);


  memset(w, 0, 256);
  switch (qsp->mode) {
  case IMG_CONV_MODE_THUMBNAIL:
    fname = apr_psprintf(r->pool, "%s.%s.thumbnail", r->filename, new_user_agent);
    DBG(r,"REQ[%X] mode=thumbnail [%s]", TO_ADDR(r),fname);
    break;

  case IMG_CONV_MODE_WALLPAPER:
  case IMG_CONV_MODE_EZGET:
    fname = apr_psprintf(r->pool, "%s.%s.wallpaper", r->filename, new_user_agent);
    DBG(r,"REQ[%X] mode=WallPaper [%s]", TO_ADDR(r),fname);
    break;

  case IMG_CONV_MODE_NORMAL:
  default:

    fname = apr_psprintf(r->pool, "%s.%s", r->filename, new_user_agent);

    if (qsp->width)
      fname = apr_psprintf(r->pool, "%s.w%d", fname, qsp->width);

    if (qsp->height)
      fname = apr_psprintf(r->pool, "%s.h%d", fname, qsp->height);

    DBG(r,"REQ[%X] mode=normal [%s]", TO_ADDR(r),fname);
    break;
  }
  if (qsp->ua_flag == UA_IGN) {
    fname = apr_psprintf(r->pool, "%s.IGN", fname);
  }

  len = strlen(fname);
  jj=0;
  for  (ii=0; ii<len; ii++) {
    if (fname[ii] == '/' 
    ||  fname[ii] == ' ' 
    ||  fname[ii] == '-' 
    ||  fname[ii] == '.' 
    ||  fname[ii] == '(' 
    ||  fname[ii] == ')') {
      w[jj++] = '_';
    }
    else {
      w[jj++] = fname[ii];
    }
  }

  return apr_psprintf(r->pool, "%s/%s.%s", conf->image_cache_dir,w,CACHEDATA_EXT);
}


static unsigned short
s_add_crc(const char *writedata, apr_size_t writebyte)
{
  unsigned short crc = 0xffff;
  apr_size_t     ii;
  unsigned char  ch;

  for(ii=0;ii<writebyte;ii++) {
    ch  = writedata[ii];
    crc = AU_CRC_TBL[(crc>>8^ch)&0xff]^(crc<<8);
  }
  return crc;
}


int
chxj_trans_name(request_rec *r)
{
  const char      *ccp;
  char            *docroot;
  int             len;
  apr_finfo_t     st;
  apr_status_t    rv;
  mod_chxj_config *conf;
  int             ii;
  char            *ext[] = {
          "jpg",
          "JPG",
          "jpeg",
          "JPEG",
          "png",
          "PNG",
          "bmp",
          "BMP",
          "gif",
          "GIF",
          "qrc",    /* QRCode出力用ファイルの拡張子 */
          "",
  };
  char     *fname = NULL;
  char     *idx;
  char     *filename_sv;
  int      do_ext_check = TRUE;
  int      next_ok      = FALSE;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (conf == NULL) {
    DBG(r,"REQ[%X] conf is null[%s]", TO_ADDR(r),r->uri);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }

  if (conf->image != CHXJ_IMG_ON) {
    DBG(r,"REQ[%X] conf not found",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }


  DBG(r,"REQ[%X] Match URI[%s]", TO_ADDR(r),r->uri);


  if (r->filename == NULL) 
    r->filename = apr_pstrdup(r->pool, r->uri);

  filename_sv = NULL;
  if ((idx = strchr(r->filename, ':')) != NULL) 
    filename_sv = idx+1;
  else 
    filename_sv = r->filename;

  DBG(r,"REQ[%X] r->filename[%s]", TO_ADDR(r),filename_sv);

  ccp = ap_document_root(r);
  if (ccp == NULL)
    return HTTP_INTERNAL_SERVER_ERROR;

  docroot = apr_pstrdup(r->pool, ccp);
  len = strlen(docroot);

  if (docroot[len-1] == '/') 
    docroot[len-1] = '\0';


  if (r->server->path 
  &&  *filename_sv == *r->server->path 
  &&  strncmp(filename_sv, r->server->path, r->server->pathlen) == 0)
    filename_sv = apr_pstrcat(r->pool, docroot, (filename_sv + r->server->pathlen), NULL);
  else
    filename_sv = apr_pstrcat(r->pool, docroot, filename_sv, NULL);

  DBG(r,"REQ[%X] URI[%s]", TO_ADDR(r),filename_sv);

  do_ext_check = TRUE;
  for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
    char* pos = strrchr(filename_sv, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        do_ext_check = FALSE;
        fname = apr_psprintf(r->pool, "%s", filename_sv);
        break;
      }
    }
  }

  if (do_ext_check) {
    for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
      if (strlen(ext[ii]) == 0) {
        fname = apr_psprintf(r->pool, "%s", filename_sv);
      }
      else 
        fname = apr_psprintf(r->pool, "%s.%s", filename_sv, ext[ii]);
  
      DBG(r,"REQ[%X] search [%s]", TO_ADDR(r),fname);
  
      rv = apr_stat(&st, fname, APR_FINFO_MIN, r->pool);
      if (rv == APR_SUCCESS) {
        if (st.filetype != APR_DIR)
          break;
      }
  
      fname = NULL;
    }
  }
  if (fname == NULL) {
    DBG(r,"REQ[%X] NotFound [%s]", TO_ADDR(r),r->filename);
    return DECLINED;
  }
  for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
    char* pos = strrchr(fname, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        next_ok = TRUE;
        break;
      }
    }
  }

  if (! next_ok)  {
    DBG(r,"REQ[%X] NotFound [%s]", TO_ADDR(r),r->filename);
    return DECLINED;
  }

  if (r->handler == NULL || strcasecmp(r->handler, "chxj-qrcode") != 0) {
    DBG(r,"REQ[%X] Found [%s]", TO_ADDR(r),fname);

    r->filename = apr_psprintf(r->pool, "%s", fname);
  
    if (strcasecmp("qrc", ext[ii]) == 0)
      r->handler = apr_psprintf(r->pool, "chxj-qrcode");
    else
      r->handler = apr_psprintf(r->pool, "chxj-picture");
  }
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return OK;
}



static int
s_chxj_trans_name2(request_rec *r)
{
  apr_finfo_t     st;
  apr_status_t    rv;
  mod_chxj_config *conf;
  int             ii;
  char            *ext[] = {
          "jpg",
          "JPG",
          "jpeg",
          "JPEG",
          "png",
          "PNG",
          "bmp",
          "BMP",
          "gif",
          "GIF",
          "qrc",    /* QRCode出力用ファイルの拡張子 */
          "",
  };
  char     *fname = NULL;
  char     *filename_sv;
  int      do_ext_check = TRUE;
  int      next_ok      = FALSE;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (conf == NULL) {
    DBG(r,"REQ[%X] conf is null[%s]", TO_ADDR(r), r->uri);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }

  if (conf->image != CHXJ_IMG_ON) {
    DBG(r,"REQ[%X] ImageEngineOff", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }


  DBG(r,"REQ[%X] Match URI[%s]", TO_ADDR(r),r->uri);

  if (r->filename == NULL) {
    DBG(r,"REQ[%X] r->filename is null", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return DECLINED;
  }
     
  filename_sv = r->filename;

  DBG(r,"REQ[%x] r->filename[%s]", TO_ADDR(r), filename_sv);

  do_ext_check = TRUE;
  for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
    char* pos = strrchr(filename_sv, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        do_ext_check = FALSE;
        fname = apr_psprintf(r->pool, "%s", filename_sv);
        break;
      }
    }
  }

  if (do_ext_check) {
    for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
      if (strlen(ext[ii]) == 0) {
        fname = apr_psprintf(r->pool, "%s", filename_sv);
      }
      else 
        fname = apr_psprintf(r->pool, "%s.%s", filename_sv, ext[ii]);
  
      DBG(r,"REQ[%X] search [%s]", TO_ADDR(r),fname);
  
      rv = apr_stat(&st, fname, APR_FINFO_MIN, r->pool);
      if (rv == APR_SUCCESS) {
        if (st.filetype != APR_DIR)
          break;
      }
  
      fname = NULL;
    }
  }
  if (fname == NULL) {
    DBG(r,"REQ[%X] NotFound [%s]", TO_ADDR(r),r->filename);
    return DECLINED;
  }
  for (ii=0; ii<(int)(sizeof(ext)/sizeof(ext[0])); ii++) {
    char* pos = strrchr(fname, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        next_ok = TRUE;
        break;
      }
    }
  }

  if (! next_ok)  {
    DBG(r,"REQ[%X] NotFound [%s]", TO_ADDR(r),r->filename);
    return DECLINED;
  }

  if (r->handler == NULL || strcasecmp(r->handler, "chxj-qrcode") != 0) {
    DBG(r,"REQ[%X] Found [%s]", TO_ADDR(r),fname);

    r->filename = apr_psprintf(r->pool, "%s", fname);
  
    if (strcasecmp("qrc", ext[ii]) == 0)
      r->handler = apr_psprintf(r->pool, "chxj-qrcode");
    else
      r->handler = apr_psprintf(r->pool, "chxj-picture");
  }
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return OK;
}



/**
 * It converts it from QUERYSTRING.
 *
 * @param r   [i]
 */
static query_string_param_t *
s_get_query_string_param(request_rec *r)
{
  char *pair;
  char *name;
  char *value;
  char *pstate;
  char *vstate;
  char *s;
  query_string_param_t *param;

  s = apr_pstrdup(r->pool, r->parsed_uri.query);
  param = apr_palloc(r->pool, sizeof(query_string_param_t));
  param->mode       = IMG_CONV_MODE_NORMAL;
  param->user_agent = NULL;
  param->ua_flag    = UA_USE;
  param->name       = NULL;
  param->offset     = 0;
  param->count      = 0;
  param->width      = 0;
  param->height     = 0;

  if (s == NULL) return param;

  for (;;) {
    if ((pair = apr_strtok(s, "&", &pstate)) == NULL) break;
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);

    switch (*name) {
    case 'm':
    case 'M':
      if (value && (strcasecmp(name, "mode") == 0 || strcasecmp(name, "m") == 0)) {

        switch (*value) {
        case 't':
        case 'T':
          if (strcasecmp(value, "thumbnail") == 0 || strcasecmp(value, "tb") == 0)
            param->mode = IMG_CONV_MODE_THUMBNAIL;
          break;
  
        case 'w':
        case 'W':
          if (strcasecmp(value, "WP") == 0 || strcasecmp(value, "WallPaper") == 0)
            param->mode = IMG_CONV_MODE_WALLPAPER;
          break;
  
        case 'e':
        case 'E':
          if (strcasecmp(value, "EZGET") == 0)
            param->mode = IMG_CONV_MODE_EZGET;
          break;
        default:
          break;
        }
      }
      break;

    case 'u':
    case 'U':
      if (value && (strcasecmp(name, "ua") == 0 || strcasecmp(name, "user-agent") == 0)) {
        ap_unescape_url(value);

        if ((*value == 'i' || *value == 'I') && strcasecmp(value, "IGN") == 0)
          param->ua_flag = UA_IGN;

        param->user_agent = apr_pstrdup(r->pool, value);
      }
      break;

    case 'n':
    case 'N':
      if (value && strcasecmp(name, "name") == 0)
        param->name = apr_pstrdup(r->pool, value);
      break;

    case 'o':
    case 'O':
      if (value && strcasecmp(name, "offset") == 0 && (! chxj_chk_numeric(value)))
        param->offset = chxj_atoi(value);

      break;

    case 'c':
    case 'C':
      if (value && strcasecmp(name, "count") == 0 && (! chxj_chk_numeric(value)))
        param->count = chxj_atoi(value);
      break;

    case 'w':
    case 'W':
      if (value && strcasecmp(name, "w") == 0 && (! chxj_chk_numeric(value)))
        param->width = chxj_atoi(value);
      break;

    case 'h':
    case 'H':
      if (value && strcasecmp(name, "h") == 0 && (! chxj_chk_numeric(value)))
        param->height = chxj_atoi(value);
      break;

    default:
      break;
    }
  }

  if (param->mode == IMG_CONV_MODE_NORMAL && param->name)
    param->mode = IMG_CONV_MODE_WALLPAPER;

  return param;
}


static char *
s_add_comment_to_png(request_rec *r, char *data, apr_size_t *len)
{
  char *result = NULL;
#define PNG_COPYRIGHT_KEY "Copyright"
#define PNG_COPYRIGHT_VAL "kddi_copyright=on,copy=NO"
#define PNG_SIG_AND_IHDR_SZ (33)
  char *key = PNG_COPYRIGHT_KEY;
  apr_size_t klen = sizeof(PNG_COPYRIGHT_KEY)-1;
  char *val = PNG_COPYRIGHT_VAL;
  apr_size_t vlen = sizeof(PNG_COPYRIGHT_VAL)-1;
  apr_pool_t *pool;
  apr_size_t total_tEXt_size;
  apr_size_t tEXt_data_size;
  apr_size_t pos;
  apr_size_t ii;
  char *buf;
  char *valbuf;
  uint32_t crc;
  mod_chxj_config *conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  if (conf->image_copyright) {
    apr_pool_create(&pool, r->pool);

    valbuf = apr_psprintf(pool, "%s,%s", val, conf->image_copyright);
    vlen = strlen(valbuf);
  
    /* total_size = length + "tEXt" + "Copyright" + '\0' + val + crc */
    total_tEXt_size = 4 + 4 + klen + vlen + 1 + 4;
    result = apr_palloc(pool, total_tEXt_size + *len);
    if (!result) {
      ERR(r, "REQ[%X] memory allocation error.", TO_ADDR(r));
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return NULL;
    }
    pos = PNG_SIG_AND_IHDR_SZ;
    memcpy(result, data, pos); /* 33 = SIGNATURE + IHDR */
    tEXt_data_size = klen + 1 + vlen;
    result[pos + 0] = tEXt_data_size >> 24;
    result[pos + 1] = tEXt_data_size >> 16;
    result[pos + 2] = tEXt_data_size >> 8;
    result[pos + 3] = tEXt_data_size;
    pos += 4;
    buf = apr_palloc(pool, 4 + klen + 1 + vlen);
    memcpy(&buf[0], "tEXt", 4); 
    memcpy(&buf[4], key, klen);
    buf[4+klen] = 0;
    memcpy(&buf[4+klen+1], valbuf, vlen);
    
    
    DBG(r, "REQ[%X] buf:[%s]", (unsigned int)(apr_size_t)r, buf);
  
    crc = 0xffffffff;
    for (ii = 0; ii < 4 + tEXt_data_size; ii++) {
      crc = AU_CRC_TBL[(crc ^ buf[ii]) & 0xff] ^ (crc >> 8);
    }
    crc ^= 0xffffffff;
    memcpy(&result[pos], buf, 4 + klen + 1 + vlen);
    pos += (4 + klen + 1 + vlen);
    result[pos + 0] = crc >> 24;
    result[pos + 1] = crc >> 16;
    result[pos + 2] = crc >> 8;
    result[pos + 3] = crc;
    pos += 4;
    memcpy(&result[pos], &data[PNG_SIG_AND_IHDR_SZ] , *len - PNG_SIG_AND_IHDR_SZ);
    *len = *len + total_tEXt_size;
    DBG(r,"REQ[%X] writebyte:[%d]", TO_ADDR(r), (unsigned int)*len);
  }
  else {
    result = data;
  }
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
#undef PNG_SIG_AND_IHDR_SZ
#undef PNG_COPYRIGHT_KEY
#undef PNG_COPYRIGHT_VAL
  return result;
}
/*
 * vim:ts=2 et
 */
