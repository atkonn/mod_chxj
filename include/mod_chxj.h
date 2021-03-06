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
#ifndef __MOD_CHXJ_H__
#define __MOD_CHXJ_H__

#define CHXJ_TRUE    (1)
#define CHXJ_FALSE   (0)

/* #define DEBUG_FD(X,args...) {FILE*fp=fopen("/tmp/error.log", "ab");fprintf(fp, X, ##args);fflush(fp);fclose(fp);} */

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


#include <string.h>

#include "chxj_apache.h"

#include "apr_strings.h"
#include "apr_buckets.h"
#include "apr_lib.h"
#include "apr_tables.h"
#include "apr_dso.h"
#include "apr_general.h"
#include "apr_pools.h"
#include "apr_hash.h"

#if defined(AP_NEED_SET_MUTEX_PERMS)
#  include "unixd.h"
#endif
#if !defined(AP_NEED_SET_MUTEX_PERMS)
#  if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#    define AP_NEED_SET_MUTEX_PERMS
#  endif
#endif

#define STRCASEEQ(a,b,c,d) \
  ((((a) == *(d))|| ((b) == *(d))) && strcasecmp((c),(d)) == 0)
#define STRNCASEEQ(a,b,c,d,e) \
  ((((a) == *(d))|| ((b) == *(d))) && strncasecmp((c),(d),(e)) == 0)

#define CHXJ_QUERY_STRING_PARAM_PREFIX "_chxj_qs_"
#define CHXJ_QUERY_STRING_PARAM_PREFIX_ENC "%5Fchxj%5Fqs%5F"


#include "qs_ignore_sp.h"
#include "qs_log.h"
#include "qs_malloc.h"
#include "qs_parse_attr.h"
#include "qs_parse_file.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"

#if defined(HAVE_AP_REGEX_H) && HAVE_AP_REGEX_H == 1
#  include "ap_regex.h"
#else
#  include "pcreposix.h"
#  include "pcre.h"
#  if !defined(AP_REG_ASSERT)
#    define AP_REG_ASSERT   REG_ASSERT
#  endif
#  if !defined(AP_REG_BADBR)
#    define AP_REG_BADBR    REG_BADBR
#  endif
#  if !defined(AP_REG_BADPAT)
#    define AP_REG_BADPAT   REG_BADPAT
#  endif
#  if !defined(AP_REG_BADRPT)
#    define AP_REG_BADRPT   REG_BADRPT
#  endif
#  if !defined(AP_REG_EBRACE)
#    define AP_REG_EBRACE   REG_EBRACE
#  endif
#  if !defined(AP_REG_EBRACK)
#    define AP_REG_EBRACK   REG_EBRACK
#  endif
#  if !defined(AP_REG_ECOLLATE)
#    define AP_REG_ECOLLATE REG_ECOLLATE
#  endif
#  if !defined(AP_REG_ECTYPE)
#    define AP_REG_ECTYPE   REG_ECTYPE
#  endif
#  if !defined(AP_REG_EESCAPE)
#    define AP_REG_EESCAPE  REG_EESCAPE
#  endif
#  if !defined(AP_REG_EMPTY)
#    define AP_REG_EMPTY    REG_EMPTY
#  endif
#  if !defined(AP_REG_EPAREN)
#    define AP_REG_EPAREN   REG_EPAREN
#  endif
#  if !defined(AP_REG_ERANGE)
#    define AP_REG_ERANGE   REG_ERANGE
#  endif
#  if !defined(AP_REG_ESIZE)
#    define AP_REG_ESIZE    REG_ESIZE
#  endif
#  if !defined(AP_REG_ESPACE)
#    define AP_REG_ESPACE   REG_ESPACE
#  endif
#  if !defined(AP_REG_ESUBREG)
#    define AP_REG_ESUBREG  REG_ESUBREG
#  endif
#  if !defined(AP_REG_INVARG)
#    define AP_REG_INVARG   REG_INVARG
#  endif
#  if !defined(AP_REG_NOMATCH)
#    define AP_REG_NOMATCH  REG_NOMATCH
#  endif

#  if !defined(AP_REG_ICASE)
#    define AP_REG_ICASE    REG_ICASE
#  endif
#  if !defined(AP_REG_NEWLINE)
#    define AP_REG_NEWLINE  REG_NEWLINE
#  endif
#  if !defined(AP_REG_NOTBOL)
#    define AP_REG_NOTBOL   REG_NOTBOL
#  endif
#  if !defined(AP_REG_NOTEOL)
#    define AP_REG_NOTEOL   REG_NOTEOL
#  endif

#  if !defined(AP_REG_EXTENDED)
#    define AP_REG_EXTENDED REG_EXTENDED
#  endif
#  if !defined(AP_REG_NOSUB)
#    define AP_REG_NOSUB    REG_NOSUB
#  endif

#  define ap_regex_t      regex_t
#  define ap_regmatch_t   regmatch_t
#endif




#ifdef HAVE_APR_GLOBAL_MUTEX_H
#  include "apr_global_mutex.h"
#endif

#ifdef HAVE_APR_SHM_H
#  include "apr_shm.h"
#endif


typedef struct imode_emoji_t imode_emoji_t;

struct imode_emoji_t {
  char  hex1byte;
  char  hex2byte;
  char* string;
  char* color;
  char *description;
};

typedef struct ezweb_emoji_t ezweb_emoji_t;

struct ezweb_emoji_t {
  char* typeA;
  char* typeB;
  char* typeC;
  char* typeD;
};

typedef struct jphone_emoji_t jphone_emoji_t;

struct jphone_emoji_t {
  char* string;
};

typedef struct iphone_emoji_t iphone_emoji_t;

struct iphone_emoji_t {
  char* string;
};

typedef struct android_emoji_t android_emoji_t;

struct android_emoji_t {
  char* string;
};

typedef struct emoji_t emoji_t;

struct emoji_t {
  struct emoji_t*  next;
  int              no;
  imode_emoji_t*   imode;
  ezweb_emoji_t*   ezweb;
  jphone_emoji_t*  jphone;
  iphone_emoji_t*  iphone;
  android_emoji_t*  android;
};

typedef struct chxjconvrule_entry chxjconvrule_entry;

struct chxjconvrule_entry {
  char         *pattern;
  ap_regex_t   *regexp;
  int          flags;
  int          action;
  char         *encoding;
  int          pc_flag;
  char         *user_agent;
};

typedef struct tag_handler tag_handler;

struct tag_handler {
  char* (*start_tag_handler)(void* doc, Node* node); 
  char* (*end_tag_handler)(void* doc, Node* node); 
};

#include "chxj_specified_device.h"

typedef struct {
  spec_type    type; 
  tag_handler* handler;
} tag_handlers;

typedef enum {
  tagHTML = 0,
  tagMETA,  
  tagTEXTAREA,
  tagP,
  tagPRE,
  tagUL,
  tagLI,
  tagOL,
  tagH1,
  tagH2,
  tagH3,
  tagH4,
  tagH5,
  tagH6,
  tagHEAD,
  tagTITLE,
  tagBASE,
  tagBODY,
  tagA,
  tagBR,
  tagTABLE,
  tagTR,
  tagTD,
  tagTBODY,
  tagFONT,
  tagFORM,
  tagINPUT,
  tagCENTER,
  tagHR,
  tagIMG,
  tagSELECT,
  tagOPTION,
  tagDIV,
  tagCHXJIF,  
  tagCHXJRAW,  
  tagNOBR,  
  tagSMALL,  
  tagSTYLE,  
  tagSPAN,  
  tagTEXT,  
  tagTH,  
  tagB,
  tagFIELDSET,
  tagDT,
  tagLEGEND,
  tagLABEL,
  tagBLOCKQUOTE,
  tagDIR,
  tagDL,
  tagDD,
  tagMENU,
  tagPLAINTEXT,
  tagBLINK,
  tagMARQUEE,
  tagLINK,
  tagNLMARK,      /* New Line Code */
  tagOBJECT,
  tagPARAM,
  tagCAPTION,
} tag_type;

typedef struct mod_chxj_config mod_chxj_config;
typedef struct mod_chxj_req_config_t mod_chxj_req_config;

#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#if defined(USE_MEMCACHE_COOKIE)
#  include "chxj_memcache.h"
#endif

/* cookie store type */
#define CHXJ_COOKIE_STORE_TYPE_DBM      "dbm"
#define CHXJ_COOKIE_STORE_TYPE_MYSQL    "mysql"
#define CHXJ_COOKIE_STORE_TYPE_MEMCACHE "memcache"
typedef enum {
  COOKIE_STORE_TYPE_NONE     = 0,
  COOKIE_STORE_TYPE_DBM      = 1,
  COOKIE_STORE_TYPE_MYSQL    = 2,
  COOKIE_STORE_TYPE_MEMCACHE = 3, 
} cookie_store_type_t;

/* new line type */
#define CHXJ_NEW_LINE_TYPE_CRLF         "crlf"
#define CHXJ_NEW_LINE_TYPE_LF           "lf"
#define CHXJ_NEW_LINE_TYPE_CR           "cr"
#define CHXJ_NEW_LINE_TYPE_NONE         "none"
typedef enum {
  NLTYPE_NIL  = 0,
  NLTYPE_CRLF = 1,
  NLTYPE_LF   = 2,
  NLTYPE_CR   = 3,
  NLTYPE_NONE = 4,
} chxj_new_line_type_t;
#define IS_NLTYPE_CRLF(X)   ((X)->new_line_type == NLTYPE_CRLF)
#define IS_NLTYPE_LF(X)     ((X)->new_line_type == NLTYPE_LF)
#define IS_NLTYPE_CR(X)     ((X)->new_line_type == NLTYPE_CR)
#define IS_NLTYPE_NONE(X)   ((X)->new_line_type == NLTYPE_NONE)
#define TO_NLCODE(X)        (IS_NLTYPE_CRLF(X) ? "\r\n" : \
                             IS_NLTYPE_LF(X)   ? "\n"   : \
                             IS_NLTYPE_CR(X)   ? "\r"   : \
                             IS_NLTYPE_NONE(X) ? ""     : "\r\n")

struct mod_chxj_config {
  int                   image;

  char                  *device_data_file;
  char                  *emoji_data_file;

  char                  *image_cache_dir;
  char                  *image_copyright;
  unsigned long         image_cache_limit;
  device_table_list     *devices;
  emoji_t               *emoji;
  emoji_t               *emoji_tail;
  int                   imode_emoji_color;
  char                  *server_side_encoding;

  char                  *dir; /* for LOG */

  apr_array_header_t    *convrules;

  char                  *cookie_db_dir;
  long                  cookie_timeout;
  cookie_store_type_t   cookie_store_type;
  int                   cookie_lazy_mode;
  char                  *cookie_dbm_type;
  
  int                   detect_device_type; /* XML|TSV */

#if defined(USE_MYSQL_COOKIE)
  mysql_t               mysql;
#endif
#if defined(USE_MEMCACHE_COOKIE)
  memcache_t            memcache;
#endif
  char                  *forward_url_base;  /* use input filter */
  char                  *forward_server_ip; /* use input filter */

  char                  *allowed_cookie_domain; /* default is null */

  chxj_new_line_type_t  new_line_type;

  char                  *post_log;              /* post log environment name. */
  
  apr_array_header_t    *device_keys;           /* TSV header array */
  apr_hash_t            *device_hash;           /* TSV device data hash table */

  int                   image_rewrite;
  char                  *image_rewrite_url;
  int                   image_rewrite_mode;

  int                   use_emoji_image;
  char                  *emoji_image_url;


  int                   use_google_analytics;
  char                  *google_analytics_account;
  char                  *google_analytics_target;   /* i.e. /ga.pl */
  char                  *google_analytics_debug;    /* debug filename */
};

struct mod_chxj_req_config_t {
  char               *user_agent;
  device_table       *spec;
  ap_filter_t        *f;
  chxjconvrule_entry *entryp;
};

#define IS_COOKIE_STORE_DBM(X)      ((X) == COOKIE_STORE_TYPE_DBM)
#define IS_COOKIE_STORE_MYSQL(X)    ((X) == COOKIE_STORE_TYPE_MYSQL)
#define IS_COOKIE_STORE_MEMCACHE(X) ((X) == COOKIE_STORE_TYPE_MEMCACHE)
#define IS_COOKIE_STORE_NONE(X)     ((X) == COOKIE_STORE_TYPE_NONE)

#define COOKIE_LAZY_ON                (2)
#define COOKIE_LAZY_OFF               (1)
#define IS_COOKIE_LAZY(X)             ((X)->cookie_lazy_mode == COOKIE_LAZY_ON)


#define CONVRULE_ENGINE_ON_BIT        (0x00000001)
#define CONVRULE_ENGINE_OFF_BIT       (0x00000002)
#define CONVRULE_COOKIE_ON_BIT        (0x00000004)
#define CONVRULE_CSS_ON_BIT           (0x10000000)
#define CONVRULE_Z2H_ON_BIT           (0x00000008)
#define CONVRULE_Z2H_OFF_BIT          (0x00000010)
#define CONVRULE_Z2H_ALPHA_ON_BIT     (0x00000020)
#define CONVRULE_Z2H_ALPHA_OFF_BIT    (0x00000040)
#define CONVRULE_Z2H_NUM_ON_BIT       (0x00000080)
#define CONVRULE_Z2H_NUM_OFF_BIT      (0x00000100)
#define CONVRULE_QSCONV_OFF_BIT       (0x00000200)
#define CONVRULE_JRCONV_OFF_BIT       (0x00000400)
#define CONVRULE_NOCACHE_ON_BIT       (0x00000800)
#define CONVRULE_EMOJI_ONLY_BIT       (0x00001000)
#define CONVRULE_ENVINFO_ONLY_BIT     (0x00002000)
#define CONVRULE_COOKIE_ONLY_BIT      (0x00004000)
#define CONVRULE_OVERWRITE_X_CLIENT_TYPE_BIT (0x00008000)

#define CONVRULE_ENGINE_ON_CMD        "EngineOn"
#define CONVRULE_ENGINE_OFF_CMD       "EngineOff"
#define CONVRULE_COOKIE_ON_CMD        "CookieOn"
#define CONVRULE_COOKIE_OFF_CMD       "CookieOff"
#define CONVRULE_CSS_ON_CMD           "CssOn"
#define CONVRULE_CSS_OFF_CMD          "CssOff"
#define CONVRULE_Z2H_ON_CMD           "Z2hOn"
#define CONVRULE_Z2H_OFF_CMD          "Z2hOff"
#define CONVRULE_Z2H_ALPHA_ON_CMD     "Z2hAlphaOn"
#define CONVRULE_Z2H_ALPHA_OFF_CMD    "Z2hAlphaOff"
#define CONVRULE_Z2H_NUM_ON_CMD       "Z2hNumOn"
#define CONVRULE_Z2H_NUM_OFF_CMD      "Z2hNumOff"
#define CONVRULE_Z2H_ALL_ON_CMD       "Z2hAllOn"
#define CONVRULE_Z2H_ALL_OFF_CMD      "Z2hAllOff"
#define CONVRULE_QSCONV_OFF_CMD       "QSConvOff"
#define CONVRULE_JRCONV_OFF_CMD       "JRConvOff"
#define CONVRULE_NOCACHE_ON_CMD       "NoCacheOn"
#define CONVRULE_EMOJI_ONLY_CMD       "EmojiOnly"
#define CONVRULE_ENVINFO_ONLY_CMD     "EnvInfoOnly"
#define CONVRULE_COOKIE_ONLY_CMD      "CookieOnly"
#define CONVRULE_OVERWRITE_X_CLIENT_TYPE_CMD "OverwriteXClientType"


#define CONVRULE_FLAG_NOTMATCH        (0x00000001)

#define CONVRULE_PC_FLAG_ON_CMD       "PC"

#define CONVRULE_PC_FLAG_ON_BIT       (0x00000001)
#define CONVRULE_PC_FLAG_OFF_BIT      (0x00000002)


#define IS_CSS_ON(X)                  ((X)->action & CONVRULE_CSS_ON_BIT)

typedef struct {
  apr_global_mutex_t* cookie_db_lock;
} mod_chxj_global_config;

typedef struct {
  apr_size_t len;
  chxjconvrule_entry  *entryp;
  device_table        *spec;

  apr_bucket_brigade *bb;
  apr_pool_t *pool;

  char* buffer;
} mod_chxj_ctx;

#include "chxj_tag_util.h"

#define CHXJ_MOD_CONFIG_KEY   "chxj_module_key"

#define HTTP_USER_AGENT       "User-Agent"
#define HTTP_HOST             "Host"
#define CHXJ_HTTP_USER_AGENT  "CHXJ_HTTP_USER_AGENT"
#define CHXJ_HEADER_ORIG_CLIENT_IP "X-Chxj-Orig-Client-Ip"
#define CHXJ_HEADER_ORIG_CLIENT_TYPE "X-Chxj-Orig-Client-Type"

module AP_MODULE_DECLARE_DATA chxj_module;

#define CHXJ_IMG_ON     (2)
#define CHXJ_IMG_OFF    (1)
#define CHXJ_IMG_NONE   (0)

#define CHXJ_IMODE_EMOJI_COLOR_ON   (3)
#define CHXJ_IMODE_EMOJI_COLOR_AUTO (2)
#define CHXJ_IMODE_EMOJI_COLOR_OFF  (1)
#define CHXJ_IMODE_EMOJI_COLOR_NONE (0)

#define CHXJ_ADD_DETECT_DEVICE_TYPE_TSV     (1)
#define CHXJ_ADD_DETECT_DEVICE_TYPE_NONE    (0)

#define CHXJ_PROVIDER_UNKNOWN  (0)
#define CHXJ_PROVIDER_DOCOMO   (1)
#define CHXJ_PROVIDER_AU       (2)
#define CHXJ_PROVIDER_SOFTBANK (3)

#define CHXJ_IMG_REWRITE_ON     (2)
#define CHXJ_IMG_REWRITE_OFF    (1)
#define CHXJ_IMG_REWRITE_NONE   (0)

#define CHXJ_IMG_REWRITE_MODE_ALL  (3)
#define CHXJ_IMG_REWRITE_MODE_USER (2)
#define CHXJ_IMG_REWRITE_MODE_TAG  (1)
#define CHXJ_IMG_REWRITE_MODE_NONE (0)

#define CHXJ_IMG_REWRITE_URL_STRING             "_x-chxj_imgrewrite=on"
#define CHXJ_IMG_X_HTTP_IMAGE_FILENAME          "X-Chxj-Image-Filename"
#define CHXJ_IMG_X_HTTP_IMAGE_TYPE              "X-Chxj-Image-Type"

#define DBG(X,args...)  chxj_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(request_rec*)(X),##args)
#define SDBG(X,Y)       chxj_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y))
#define PDBG(X,args...) chxj_log_perror(APLOG_MARK,APLOG_DEBUG,0,(apr_pool_t *)(X),##args)
#define ERR(X,args...)  chxj_log_rerror(APLOG_MARK,APLOG_ERR,0,(X), ##args)
#define SERR(X,args...) chxj_log_error(APLOG_MARK,APLOG_ERR,0,(X),##args)
#define PERR(X,args...) chxj_log_perror(APLOG_MARK,APLOG_ERR,0,(apr_pool_t *)(X),##args)
#define WRN(rec,format,args...)  chxj_log_rerror(APLOG_MARK,APLOG_WARNING,0,(rec),(format), ##args)

extern tag_handlers chxj_tag_handlers[];
extern tag_handler  chtml10_handler[];
extern tag_handler  chtml20_handler[];
extern tag_handler  chtml30_handler[];
extern tag_handler  chtml40_handler[];
extern tag_handler  chtml50_handler[];
extern tag_handler  ixhtml10_handler[];
extern tag_handler  xhtml_handler[];
extern tag_handler  hdml_handler[];
extern tag_handler  jhtml_handler[];
extern tag_handler  jxhtml_handler[];
extern tag_handler  iphone_handler[];
extern tag_handler  android_handler[];

extern char* chxj_node_convert( 
  device_table *spec,
  request_rec  *r,
  void         *pdoc,
  Doc          *doc,
  Node         *node,
  int          indent
);
extern char *chxj_node_convert_chxjif_only(
  request_rec  *r,
  device_table *spec,
  const char   *src,
  apr_size_t   *len
);
extern void chxj_remove_filter(request_rec *r);
extern mod_chxj_req_config *chxj_get_req_config(request_rec *r);

#define IMAGE_CACHE_LIMIT_FMT_LEN  (20)


#if HAVE_MALLOC == 0
extern void *rpl_malloc(size_t n);
#endif


#define TO_ADDR(X) ((unsigned int)(apr_size_t)(X))
#define ADJUST_WIDTH_FOR_ANDROID (40)

#if APR_MAJOR_VERSION < 1
  #ifndef APR_FOPEN_READ
    #define APR_FOPEN_READ APR_READ
  #endif
  #ifndef APR_FOPEN_BUFFERED
    #define APR_FOPEN_BUFFERED APR_BUFFERED
  #endif
  #ifndef APR_FOPEN_BINARY
    #define APR_FOPEN_BINARY APR_BINARY
  #endif
  #ifndef APR_FOPEN_CREATE
    #define APR_FOPEN_CREATE APR_CREATE
  #endif
  #ifndef APR_FOPEN_WRITE
    #define APR_FOPEN_WRITE APR_WRITE
  #endif
#endif

#endif
/*
 * vim:ts=2 et
 */
