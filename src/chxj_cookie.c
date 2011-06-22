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
#include <time.h>

#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_apply_convrule.h"
#include "chxj_str_util.h"

#include "ap_release.h"

#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"
#include "apr_time.h"
#include "apr_date.h"

#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#if defined(USE_MEMCACHE_COOKIE)
#  include "chxj_memcache.h"
#endif
#include "chxj_dbm.h"

#define DUMP_HEADERS(X) do { \
  int __ii; \
  apr_array_header_t  *__headers = (apr_array_header_t*)apr_table_elts((X)); \
  apr_table_entry_t   *__hentryp = (apr_table_entry_t*)__headers->elts; \
  for (__ii=0; __ii<__headers->nelts; __ii++) { \
    DBG(r, "key:[%s] val:[%s]", __hentryp[__ii].key, __hentryp[__ii].val); \
  } \
} while (0) 



static char* s_get_hostname_from_url(request_rec* r, char* value);
static char* s_cut_until_end_hostname(request_rec*, char* value);
static int valid_domain(request_rec *r, const char *value);
static int valid_path(request_rec *r, const char *value);
static int valid_expires(request_rec *r, const char *value);
static int valid_secure(request_rec *r, const char *value);
static int check_valid_cookie_attribute(request_rec *r, const char *pair);
static int check_valid_cookie_attribute_expires_only(request_rec *r, const char *value);

apr_proc_mutex_t *global_cookie_mutex;

static char *
alloc_cookie_id(request_rec *r)
{
  char                *cookie_id;
  char                *uuid_string;
  unsigned char       *md5_value;
  apr_uuid_t          uuid;
  apr_status_t        retval;

  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);

  retval = apr_md5(md5_value, 
                   (const char*)uuid_string, 
                   APR_UUID_FORMATTED_LENGTH);
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    return NULL;
  }

  cookie_id = apr_palloc(r->pool, apr_base64_encode_len(APR_MD5_DIGESTSIZE)+1);
  memset(cookie_id, 0, APR_MD5_DIGESTSIZE+1);
  apr_base64_encode(cookie_id, (char*)md5_value, APR_MD5_DIGESTSIZE);

  DBG(r,"REQ[%X] cookie_id=[%s]", TO_ADDR(r),cookie_id);

  cookie_id = chxj_url_encode(r->pool,cookie_id);

  DBG(r, "REQ[%X] cookie_id=[%s]", TO_ADDR(r),cookie_id);
  return cookie_id;
}



/*
 *
 */
cookie_t *
chxj_save_cookie(request_rec *r)
{
  int                 ii;
  apr_array_header_t  *headers;
  apr_table_entry_t   *hentryp;
  apr_array_header_t  *err_headers;
  apr_table_entry_t   *err_hentryp;
  char                *old_cookie_id;
  char                *store_string;
  mod_chxj_config     *dconf;
  mod_chxj_req_config *req_conf;
  chxjconvrule_entry  *entryp;
  apr_table_t         *new_cookie_table;
  int                 has_cookie = 0;
  cookie_t            *cookie;
  cookie_t            *old_cookie;
  char                *refer_string;
  apr_uri_t           parsed_uri;
  int                 has_refer;
  apr_pool_t          *pool;


  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  apr_pool_create(&pool, r->pool);

  cookie = (cookie_t*)apr_palloc(pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;

  has_cookie = 0;
  has_refer = 0;

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  req_conf = chxj_get_req_config(r);
  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (req_conf->user_agent) {
    entryp = req_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }
  if (! entryp) {
    DBG(r,"REQ[%X] end %s() no pattern",TO_ADDR(r),__func__);
    return NULL;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT) && ! (entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
    DBG(r,"REQ[%X] end %s() CookieOff",TO_ADDR(r),__func__);
    return NULL;
  }



  headers = (apr_array_header_t*)apr_table_elts(r->headers_out);
  hentryp = (apr_table_entry_t*)headers->elts;
  err_headers = (apr_array_header_t*)apr_table_elts(r->err_headers_out);
  err_hentryp = (apr_table_entry_t*)err_headers->elts;


  new_cookie_table = apr_table_make(pool, 0);

  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "REQ[%X] cookie=[%s:%s]", TO_ADDR(r), hentryp[ii].key, hentryp[ii].val);

      char* key;
      char* val;
      char* buff;

      char *pair = apr_psprintf(pool, "%s:%s", hentryp[ii].key, hentryp[ii].val);
      if (check_valid_cookie_attribute_expires_only(r, pair)) {
        buff = apr_pstrdup(pool, hentryp[ii].val);
        val = strchr(buff, '=');
        if (val) {
          key = buff;
          *val++ = 0;
          apr_table_set(new_cookie_table, apr_pstrdup(pool, key), apr_pstrdup(pool, val));
          if (strcasecmp(REFERER_COOKIE_KEY, key) == 0) has_refer++;
        }
        has_cookie = 1;
      }
    }
  }
  for (ii=0; ii<err_headers->nelts; ii++) {
    if (strcasecmp(err_hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "REQ[%X] cookie=[%s:%s]", TO_ADDR(r), err_hentryp[ii].key, err_hentryp[ii].val);

      char* key;
      char* val;
      char* buff;

      char *pair = apr_psprintf(pool, "%s:%s", err_hentryp[ii].key, err_hentryp[ii].val);
      if (check_valid_cookie_attribute_expires_only(r, pair)) {
        buff = apr_pstrdup(pool, err_hentryp[ii].val);
        val = strchr(buff, '=');
        if (val) {
          key = buff;
          *val++ = 0;
          apr_table_set(new_cookie_table, apr_pstrdup(pool, key), apr_pstrdup(pool, val));
          if (strcasecmp(REFERER_COOKIE_KEY, key) == 0) has_refer++;
        }
        has_cookie = 1;
      }
    }
  }
#if 0
  apr_table_unset(r->headers_out, "Set-Cookie");
  apr_table_unset(r->err_headers_out, "Set-Cookie");
#endif

  if (! has_refer) {
    apr_uri_parse(pool,r->uri, &parsed_uri);
    refer_string = apr_psprintf(pool, 
                                "%s://%s%s", 
                                chxj_run_http_scheme(r),
                                r->hostname,
                                apr_uri_unparse(pool,
                                                &parsed_uri,
                                                APR_URI_UNP_OMITSITEPART));
    if (r->args && strlen(r->args)) {
      refer_string = apr_pstrcat(pool, refer_string, "?", r->args, NULL);
    }
    apr_table_setn(new_cookie_table, REFERER_COOKIE_KEY, refer_string);
    DBG(r, "REQ[%X] ADD REFER[%s]", TO_ADDR(r),refer_string);
    has_cookie++;
  }


  /*
   * check input parameters
   */
  old_cookie_id = (char*)apr_table_get(r->headers_in, "CHXJ_COOKIE_ID");
  if (old_cookie_id) {
    old_cookie = chxj_load_cookie(r, old_cookie_id); 
    if (old_cookie && old_cookie->cookie_headers) {
      hentryp = (apr_table_entry_t*)old_cookie->cookie_headers->elts;
      for (ii=0; ii<old_cookie->cookie_headers->nelts; ii++) {
        if (hentryp && apr_table_get(new_cookie_table, hentryp[ii].key) == NULL) {
          apr_table_add(new_cookie_table, apr_pstrdup(pool, hentryp[ii].key), apr_pstrdup(pool, hentryp[ii].val));
        }
      }
      if (has_cookie) {
        chxj_delete_cookie(r,        old_cookie_id);
        chxj_delete_cookie_expire(r, old_cookie_id);
      }
    }
  }



  if (! has_cookie) {
    DBG(r,"REQ[%X] end %s() (no cookie)",TO_ADDR(r),__func__);
    return NULL;
  }

  /*
   * create val
   */
  cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(new_cookie_table);
  hentryp = (apr_table_entry_t*)cookie->cookie_headers->elts;
  apr_size_t store_string_len = 0;
  for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
    if (ii) store_string_len++;
    store_string_len += (strlen(hentryp[ii].key) + strlen(hentryp[ii].val) + 1);
  }
  store_string = apr_palloc(pool, store_string_len + 1);
  memset(store_string, 0, store_string_len + 1);
  apr_size_t npos = 0;

  for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
    if (ii) store_string[npos++] = '\n';
    memcpy(&store_string[npos], hentryp[ii].key, strlen(hentryp[ii].key));
    npos += strlen(hentryp[ii].key);
    store_string[npos++] = '=';
    memcpy(&store_string[npos], hentryp[ii].val, strlen(hentryp[ii].val));
    npos += strlen(hentryp[ii].val);
  }

  if (old_cookie_id && IS_COOKIE_LAZY(dconf)) {
    DBG(r, "REQ[%X] LAZY COOKIE save",TO_ADDR(r));
    cookie->cookie_id = apr_pstrdup(r->pool, old_cookie_id);
  }
  else if (old_cookie_id && apr_table_get(r->headers_in, "X-Chxj-Cookie-No-Update")) {
    DBG(r, "REQ[%X] NO UPDATE MODE",TO_ADDR(r));
    cookie->cookie_id = apr_pstrdup(r->pool, old_cookie_id);
  }
  else {
    if (old_cookie_id && apr_table_get(r->headers_in, "X-Chxj-Forward")) {
      DBG(r, "REQ[%X] NO LAZY COOKIE(X-Chxj-Forward)  save",TO_ADDR(r));
      cookie->cookie_id = apr_pstrdup(r->pool, old_cookie_id);
    }
    else {
      DBG(r, "REQ[%X] NO LAZY COOKIE save",TO_ADDR(r));
      cookie->cookie_id = alloc_cookie_id(r);
    }
  }

  DBG(r,"REQ[%X] TYPE:[%d]", TO_ADDR(r), dconf->cookie_store_type);
  DBG(r,"REQ[%X] STORE STRING:=======================================================", TO_ADDR(r));
  DBG(r,"REQ[%X] [%s]", TO_ADDR(r), store_string);
  DBG(r,"REQ[%X] =======================================================:STORE STRING", TO_ADDR(r));

  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (! chxj_save_cookie_mysql(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "%s:%d faild: chxj_save_cookie_mysql() cookie_id:[%s]", APLOG_MARK,cookie->cookie_id);
        cookie = NULL;
        goto on_error;
      }
      done_proc = 1;
    }
#endif
#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! chxj_save_cookie_memcache(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "%s:%d failed: chxj_save_cookie_memcache() cookie_id:[%s]", APLOG_MARK, cookie->cookie_id);
        cookie = NULL;
        goto on_error;
      }
      done_proc = 1;
    }
#endif
    if (IS_COOKIE_STORE_DBM(dconf->cookie_store_type) || ! done_proc) {
      if (! chxj_save_cookie_dbm(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "%s:%d failed: chxj_save_cookie_dbm() cookie_id:[%s]", APLOG_MARK, cookie->cookie_id);
        cookie = NULL;
        goto on_error;
      }
    }
  }
#if 0
  apr_table_unset(r->headers_out, "Set-Cookie");
  apr_table_unset(r->err_headers_out, "Set-Cookie");
#endif

  if (cookie) {
    chxj_save_cookie_expire(r, cookie);
  }

on_error:
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return cookie;
}



/*
 *
 */
cookie_t *
chxj_update_cookie(request_rec *r, cookie_t *old_cookie)
{
  int                 ii;
  apr_array_header_t  *headers;
  apr_table_entry_t   *hentryp;
  char                *store_string;
  mod_chxj_config     *dconf;
  mod_chxj_req_config *req_conf;
  chxjconvrule_entry  *entryp;
  cookie_t            *cookie;


  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  if (!old_cookie || ! old_cookie->cookie_headers || ! old_cookie->cookie_id) {
    DBG(r,"REQ[%X] old_cookie is null",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return  NULL;
  }

  cookie = (cookie_t *)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  req_conf = chxj_get_req_config(r);
  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (req_conf->user_agent) {
    entryp = req_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }
  if (! entryp) {
    DBG(r,"REQ[%X] no pattern",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT)) {
    DBG(r,"REQ[%X] CookieOff",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }

  headers = (apr_array_header_t*)apr_table_elts(r->headers_out);
  hentryp = (apr_table_entry_t*)headers->elts;

  chxj_delete_cookie(r,        old_cookie->cookie_id);
  chxj_delete_cookie_expire(r, old_cookie->cookie_id);

  if (IS_COOKIE_LAZY(dconf)) {
    DBG(r,"REQ[%X] LAZY MODE",TO_ADDR(r));
    cookie->cookie_id = apr_pstrdup(r->pool, old_cookie->cookie_id);
  }
  else {
    DBG(r,"REQ[%X] NO LAZY MODE",TO_ADDR(r));
    cookie->cookie_id = alloc_cookie_id(r);
  }

  cookie->cookie_headers = old_cookie->cookie_headers;
  store_string = apr_palloc(r->pool, 1);
  store_string[0] = 0;
  hentryp = (apr_table_entry_t*)cookie->cookie_headers->elts;

  for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
    if (ii) store_string = apr_pstrcat(r->pool,
                               store_string, 
                               "\n",
                               NULL);

    DBG(r,"REQ[%X] OLD COOKIE VALUE=[%s][%s]",TO_ADDR(r), hentryp[ii].key, hentryp[ii].val);
    store_string = apr_pstrcat(r->pool, 
                               store_string, 
                               hentryp[ii].key, 
                               "=",
                               hentryp[ii].val, 
                               NULL);
  }

  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (!chxj_update_cookie_mysql(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r,"REQ[%X] failed: chxj_update_cookie_mysql() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif

#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! chxj_update_cookie_memcache(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "REQ[%X] failed: chxj_update_cookie_memcache() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif
    if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
      if (! chxj_update_cookie_dbm(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "REQ[%X] failed: chxj_update_cookie_dbm() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
        goto on_error;
      }
    }
  }

  chxj_save_cookie_expire(r, cookie);

  apr_table_setn(r->headers_in, "CHXJ_COOKIE_ID", cookie->cookie_id);


on_error:
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return cookie;
}


/*
 *
 * @return loaded data.
 */
cookie_t *
chxj_load_cookie(request_rec *r, char *cookie_id)
{
  mod_chxj_config         *dconf;
  mod_chxj_req_config     *req_conf;
  chxjconvrule_entry      *entryp;
  cookie_t                *cookie;
  apr_table_t             *load_cookie_table;
  char                    *load_string = NULL;
  char                    *pstat;
  char                    *key;
  char                    *val;
  char                    *pair;
  char                    *header_cookie;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] cookie_id=[%s]", TO_ADDR(r), cookie_id);
  chxj_cookie_expire_gc(r);

  cookie = (cookie_t*)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_headers = NULL;
  cookie->cookie_id = chxj_url_decode(r->pool, apr_pstrdup(r->pool, cookie_id));
  cookie->cookie_id = chxj_url_encode(r->pool, cookie->cookie_id);


  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  req_conf = chxj_get_req_config(r);
  /*-------------------------------------------------------------------------*/
  /* already setup entryp if request_conf->user_agent is not null            */
  /*-------------------------------------------------------------------------*/
  if (req_conf->user_agent) {
    entryp = req_conf->entryp;
  }
  else {
    entryp = chxj_apply_convrule(r, dconf->convrules);
  }
  if (! entryp) {
    DBG(r,"REQ[%X] pattern", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    goto on_error0;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT) && ! (entryp->action & CONVRULE_COOKIE_ONLY_BIT)) {
    DBG(r,"REQ[%X] CookieOff", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    goto on_error0;
  }
  load_cookie_table = apr_table_make(r->pool, 0);

  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_mysql(r, dconf, cookie->cookie_id))) {
        ERR(r, "REQ[%X] %s:%d failed: chxj_load_cookie_mysql() cookie_id:[%s]", TO_ADDR(r),APLOG_MARK, cookie_id);
        goto on_error0;
      }
      done_proc = 1;
    }
#endif
#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_memcache(r, dconf, cookie->cookie_id))) {
        ERR(r, "REQ[%X] %s:%d failed: chxj_load_cookie_memcache() cookie_id:[%s]", TO_ADDR(r),APLOG_MARK,cookie_id);
        goto on_error0;
      }
      done_proc = 1;
    }
#endif
    if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_dbm(r, dconf, cookie->cookie_id))) {
        ERR(r, "REQ[%X] %s:%d failed: chxj_load_cookie_dbm() cookie_id:[%s]", TO_ADDR(r),APLOG_MARK,cookie_id);
        goto on_error0;
      }
    }
  }

  if (load_string) {
    DBG(r, "REQ[%X] load_string=[%s]", TO_ADDR(r), load_string);
    header_cookie = apr_palloc(r->pool, 1);
    header_cookie[0] = 0;
    for (;;) {
      char *tmp_sem;
      char *tmp_pair;
      pair = apr_strtok(load_string, "\n", &pstat);  
      load_string = NULL;
      if (!pair) break;

      DBG(r,"REQ[%X] Cookie:[%s]", TO_ADDR(r), pair);

      tmp_pair = apr_pstrdup(r->pool, pair);
      val = strchr(tmp_pair, '=');
      if (val) {
        key = tmp_pair;
        *val++ = 0;
        apr_table_add(load_cookie_table, key, val);
        DBG(r,"REQ[%X] ADD key:[%s] val:[%s]", TO_ADDR(r), key, val);
      }
      tmp_pair = apr_pstrdup(r->pool, pair);
      tmp_sem = strchr(tmp_pair, ';'); 
      if (tmp_sem)
        *tmp_sem = '\0';

      if (check_valid_cookie_attribute(r, pair)) {
        if (strlen(header_cookie)) 
          header_cookie = apr_pstrcat(r->pool, header_cookie, ";", NULL);
  
        header_cookie = apr_pstrcat(r->pool, header_cookie, tmp_pair, NULL);
      }
    }
    if (strlen(header_cookie)) {
      DBG(r,"REQ[%X] ADD COOKIE to REQUEST HEADER:[%s]", TO_ADDR(r), header_cookie);
      apr_table_add(r->headers_in, "Cookie", header_cookie);
    }
  
    cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(load_cookie_table);

    if (apr_table_get(r->headers_in, "referer") == NULL) {
      apr_table_setn(r->headers_in, 
                     "referer",
                     apr_table_get(load_cookie_table, REFERER_COOKIE_KEY));
    }
  
    /*
     * save cookie_id to request header.
     */
    apr_table_setn(r->headers_in, "CHXJ_COOKIE_ID", cookie->cookie_id);
  }

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return cookie;


on_error0:

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return NULL;
}

static int
check_valid_cookie_attribute(request_rec *r, const char *value)
{
  char *pstat;
  char *pair;
  char *first_pair;
  char *domain_pair;
  char *path_pair;
  char *expire_pair;
  char *secure_pair;
  char *p;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r),value);

  domain_pair = path_pair = expire_pair = secure_pair = NULL;
  p = apr_pstrdup(r->pool, value);

  /* pass first pair */
  first_pair = apr_strtok(p, ";", &pstat);  

  for (;;) {
    pair = apr_strtok(NULL, ";", &pstat);
    if (! pair) break;
    pair = qs_trim_string(r->pool, pair);
    if (STRNCASEEQ('d','D',"domain", pair, sizeof("domain")-1)) {
      domain_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('p','P',"path", pair, sizeof("path")-1)) {
      path_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('e','E',"expires", pair, sizeof("expires")-1)) {
      expire_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('s','S',"secure", pair, sizeof("secure")-1)) {
      secure_pair = apr_pstrdup(r->pool, pair);
    }
  }

  if (domain_pair) {
    if (!valid_domain(r, domain_pair)) {
      DBG(r,"REQ[%X] invalid domain. domain_pair:[%s]", TO_ADDR(r),domain_pair);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  if (path_pair) {
    if (!valid_path(r, path_pair)) {
      DBG(r,"REQ[%X] invalid path. path_pair:[%s]", TO_ADDR(r),path_pair);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  if (expire_pair) {
    if (!valid_expires(r, expire_pair)) {
      DBG(r,"REQ[%X] invalid expire. expire_pair:[%s]", TO_ADDR(r),expire_pair);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  if (secure_pair) {
    if (!valid_secure(r, secure_pair)) {
      DBG(r,"REQ[%X] invalid secure. secure_pair:[%s]", TO_ADDR(r),secure_pair);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r),value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


static int
check_valid_cookie_attribute_expires_only(request_rec *r, const char *value)
{
  char *pstat;
  char *pair;
  char *first_pair;
  char *expire_pair = NULL;
  char *p;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r), value);

  expire_pair = NULL;
  p = apr_pstrdup(r->pool, value);

  /* pass first pair */
  first_pair = apr_strtok(p, ";", &pstat);

  for (;;) {
    pair = apr_strtok(NULL, ";", &pstat);
    if (! pair) break;
    pair = qs_trim_string(r->pool, pair);
    if (STRNCASEEQ('e','E',"expires", pair, sizeof("expires")-1)) {
      expire_pair = apr_pstrdup(r->pool, pair);
    }
  }

  if (expire_pair) {
    if (!valid_expires(r, expire_pair)) {
      DBG(r,"REQ[%X] invalid expire. expire_pair:[%s]", TO_ADDR(r), expire_pair);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  DBG(r,"REQ[%X] value:[%s]",TO_ADDR(r), value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


static int
valid_domain(request_rec *r, const char *value)
{
  int  len;
  char *name;
  char *val;
  char *pstat;
  char *p = apr_pstrdup(r->pool, value);
  char *host = (char *)apr_table_get(r->headers_in, HTTP_HOST);

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r), value);
  if (!host) {
    DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r), value);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return CHXJ_TRUE;
  }
  DBG(r, "REQ[%X] host:[%s]", TO_ADDR(r), host);
  host = s_cut_until_end_hostname(r, apr_pstrdup(r->pool, host));
  DBG(r, "REQ[%X] host:[%s](after s_cut_until_end_hostname())", TO_ADDR(r), host);

  name = apr_strtok(p,"=", &pstat);
  name = qs_trim_string(r->pool, name);
  val = apr_strtok(NULL, "=", &pstat);
  val = qs_trim_string(r->pool, val);
  len = strlen(host);
  if (len) {
    if (chxj_strcasenrcmp(r->pool, host, val, strlen(val))) {
      DBG(r,"REQ[%X] not match domain. host domain:[%s] vs value:[%s]", TO_ADDR(r), host, val);
      DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r), value);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return CHXJ_FALSE;
    }
  }
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r), value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


static int
valid_path(request_rec *r, const char *value)
{
  char *p = apr_pstrdup(r->pool, value);
  char *uri;
  char *tmp;
  char *name;
  char *val;
  char *pstat;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] unparsed_uri:[%s] value:[%s]", TO_ADDR(r),r->unparsed_uri, value);
  if (chxj_starts_with(r->unparsed_uri, "http://")) {
    uri = strchr(&r->unparsed_uri[sizeof("http://")], '/');
    if (uri != NULL) {
      uri = apr_pstrdup(r->pool, uri);
    }
  }
  else if (chxj_starts_with(r->unparsed_uri, "https://")) {
    uri = strchr(&r->unparsed_uri[sizeof("https://")], '/');
    if (uri != NULL) {
      uri = apr_pstrdup(r->pool, uri);
    }
  }
  else if (chxj_starts_with(r->unparsed_uri, "/")) {
    uri = apr_pstrdup(r->pool, r->unparsed_uri);
  }
  else {
    uri = apr_pstrdup(r->pool, "/");
  }
  
  if ((tmp = strchr(uri, '?'))) {
    *tmp = '\0';
  }
  DBG(r,"REQ[%X] uri=[%s]", TO_ADDR(r),uri);
  name = apr_strtok(p, "=", &pstat);
  val = apr_strtok(NULL, "=", &pstat);
  name = qs_trim_string(r->pool, name);
  val = qs_trim_string(r->pool, val);
  DBG(r,"REQ[%X] name=[%s] val=[%s]", TO_ADDR(r),name, val);
  
  DBG(r, "REQ[%X] val:[%s] vs uri:[%s]", TO_ADDR(r),val, uri);
  if (! chxj_starts_with(uri, val)) {
    DBG(r,"REQ[%X] unparsed_uri:[%s] value:[%s] (false)",TO_ADDR(r), r->unparsed_uri, value);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return CHXJ_FALSE;
  }
  DBG(r,"REQ[%X] unparsed_uri:[%s] value:[%s] (true)", TO_ADDR(r),r->unparsed_uri, value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


static int
valid_expires(request_rec *r, const char *value)
{
  char       *name;
  char       *val;
  char       *p = apr_pstrdup(r->pool, value);
  char       *pstat;
  apr_time_t expires;
  apr_time_t now;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] value:[%s]",TO_ADDR(r),value);
  name = apr_strtok(p, "=", &pstat);
  val  = apr_strtok(NULL, "=", &pstat);
  DBG(r,"REQ[%X] name=[%s] val=[%s]", TO_ADDR(r),name, val);
  now = apr_time_now();
  expires = chxj_parse_cookie_expires(val);
  if (expires < now) {
    DBG(r,"REQ[%X] value:[%s] (expired)", TO_ADDR(r),value);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return CHXJ_FALSE;
  }
  
  DBG(r,"REQ[%X] value:[%s] (non expired)", TO_ADDR(r),value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


static int
valid_secure(request_rec *r, const char *value)
{
  const char *scheme;
  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] value:[%s]", TO_ADDR(r),value);
  scheme = chxj_apache_run_http_scheme(r);
  if (strcasecmp("https", scheme)) {
    DBG(r,"REQ[%X] value:[%s] (non secure)", TO_ADDR(r),value);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return CHXJ_FALSE;
  }
  DBG(r,"REQ[%X] value:[%s] (secure)", TO_ADDR(r),value);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return CHXJ_TRUE;
}


char *
chxj_add_cookie_parameter(request_rec *r, char *value, cookie_t *cookie)
{
  char *qs;
  char *dst;
  char *name = "";

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] cookie_id=[%s]", TO_ADDR(r), (cookie) ? cookie->cookie_id : NULL);

  dst = apr_pstrdup(r->pool, value);

  if (!cookie)
    goto on_error;

  if (!cookie->cookie_id)
    goto on_error;

  if (chxj_cookie_check_host(r, value) != 0) {
    DBG(r,"REQ[%X] (check host)", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    goto on_error;
  }

  qs = strchr(dst, '#');
  if (qs) {
    name = apr_pstrdup(r->pool, qs);
    *qs = 0;
  }

  qs = strchr(dst, '?');
  if (qs) {
    char *sv_qs = qs;
    qs = chxj_delete_chxj_cc_param(r, ++qs);
    DBG(r, "REQ[%X] qs:[%s]",TO_ADDR(r), qs);
    *sv_qs = 0;
    if (qs && strlen(qs)) {
      dst = apr_psprintf(r->pool, "%s?%s", dst, qs);
    }
  }
  if (qs) {
    dst = apr_psprintf(r->pool, "%s&%s=%s%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id, name);
  }
  else {
    if (dst[0] == '\0') {
      /* No adding cookie parameter if flagment only. 2011.06.22 */
      dst = apr_psprintf(r->pool, "%s", name);
    }
    else {
      dst = apr_psprintf(r->pool, "%s?%s=%s%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id, name);
    }
  }

  DBG(r,"REQ[%X] dst=[%s]", TO_ADDR(r), dst);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);

  return dst;

on_error:
  DBG(r,"REQ[%X] (on_error)", TO_ADDR(r));
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return dst;
}


char *
chxj_add_cookie_no_update_parameter(request_rec *r, char *value, int xmlflag)
{
  char *qs;
  char *dst;
  char *name = "";

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  if (! value || ! *value) {
    DBG(r,"REQ[%X] (void value)", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return apr_pstrdup(r->pool, "");
  }

  dst = apr_pstrdup(r->pool, value);

  if (chxj_cookie_check_host(r, value) != 0) {
    DBG(r,"REQ[%X] (check host)", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    goto on_error;
  }

  qs = strchr(dst, '#');
  if (qs) {
    name = apr_pstrdup(r->pool, qs);
    *qs = 0;
  }
  char *amp = "&";
  if (xmlflag) {
    amp = "&amp;";
  }
  dst = apr_psprintf(r->pool, "%s%s%s=true%s", dst, (strchr(dst,'?')) ? amp : "?",CHXJ_COOKIE_NOUPDATE_PARAM, name);
  DBG(r,"REQ[%X] dst=[%s]", TO_ADDR(r), dst);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return dst;

on_error:
  DBG(r,"REQ[%X] (on_error)", TO_ADDR(r));
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return dst;
}


int
chxj_cookie_check_host(request_rec *r, char *value) 
{
  char *hostnm;
  mod_chxj_config *dconf;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] hostname=[%s] vs Location:[%s]", TO_ADDR(r), r->hostname, value);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  hostnm = s_get_hostname_from_url(r, value);
  if (hostnm) {
    if (dconf->allowed_cookie_domain) {
      DBG(r, "REQ[%X] allowed_domain[%s] vs Location:[%s]", TO_ADDR(r), dconf->allowed_cookie_domain, value);
      if (chxj_strcasenrcmp(r->pool, hostnm, dconf->allowed_cookie_domain, strlen(dconf->allowed_cookie_domain))) {
        DBG(r,"REQ[%X] (false/allowed_domain)", TO_ADDR(r));
        DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
        return 1;
      }
      else {
        DBG(r,"REQ[%X] (true/allowed_domain)", TO_ADDR(r));
        DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
        return 0;
      }
    }
    else {
      if (strcasecmp(hostnm, r->hostname) == 0) {
        DBG(r,"REQ[%X] (true)", TO_ADDR(r));
        DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
        return 0;
      }
      else {
        DBG(r,"REQ[%X] (false)", TO_ADDR(r));
        DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
        return 1;
      }
    }
  }
  DBG(r,"REQ[%X] (true)", TO_ADDR(r));
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return 0;
}


static char *
s_get_hostname_from_url(request_rec *r, char *value)
{
  if (!value) 
    return NULL; 

  if (strncasecmp(value, "http://",  7) == 0 )
    return s_cut_until_end_hostname(r, &value[7]);

  if (strncasecmp(value, "https://", 8) == 0)
    return s_cut_until_end_hostname(r, &value[8]);

  return NULL;
}


static char *
s_cut_until_end_hostname(request_rec *r, char *value)
{
  char *sp;
  char *hostnm;

  hostnm = sp = apr_pstrdup(r->pool, value);
  for (;*sp; sp++) {
    if (*sp == '/'|| *sp == '?' || *sp == ':') {
      *sp = '\0';
      break;
    }
  }
  return hostnm;
}



void
chxj_delete_cookie(request_rec *r, const char *cookie_id)
{
  int done_proc = 0;
  mod_chxj_config *dconf;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_mysql(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_mysql() cookie_id:[%s]", TO_ADDR(r),cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_memcache(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_memcache() cookie_id:[%s]", TO_ADDR(r), cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_dbm(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_dbm() cookie_id:[%s]", TO_ADDR(r),cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
  }

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
}


/*
 *
 */
void
chxj_save_cookie_expire(request_rec *r, cookie_t *cookie)
{
  int done_proc = 0;
  mod_chxj_config         *dconf;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  if (!cookie) {
    DBG(r,"REQ[%X] cookie is NULL",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return;
  }
  if (!cookie->cookie_id) {
    DBG(r,"REQ[%X] cookie->cookie_id is NULL",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return;
  }

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (!dconf) {
    DBG(r,"REQ[%X] dconf is NULL",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return;
  }

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_mysql(r, dconf, cookie->cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_save_cookie_expire_mysql() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_memcache(r, dconf, cookie->cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_save_cookie_expire_memcache() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_dbm(r, dconf, cookie->cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_save_cookie_expire_dbm() cookie_id:[%s]", TO_ADDR(r),cookie->cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
  }

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
}


void
chxj_delete_cookie_expire(request_rec *r, char *cookie_id)
{
  int done_proc = 0;
  mod_chxj_config *dconf;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_expire_mysql(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_expire_mysql() cookie_id:[%s]", TO_ADDR(r),cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (!chxj_delete_cookie_expire_memcache(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_expire_memcache() cookie_id:[%s]", TO_ADDR(r),cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    } 
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (!chxj_delete_cookie_expire_dbm(r, dconf, cookie_id)) {
      ERR(r,"REQ[%X] failed: chxj_delete_cookie_expire_dbm() cookie_id:[%s]", TO_ADDR(r),cookie_id);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    } 
  }

  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
}


void
chxj_cookie_expire_gc(request_rec *r)
{
  mod_chxj_config   *dconf;
  int done_proc = 0;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_cookie_expire_gc_mysql(r, dconf)) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_expire_gc(): failed: chxj_cookie_expire_gc_mysql()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  } 
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_cookie_expire_gc_memcache(r, dconf)) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_expire_gc(): failed: chxj_cookie_expire_gc_memcache()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
    done_proc = 1;
  } 
#endif
  if (!done_proc) {
    if (! chxj_cookie_expire_gc_dbm(r, dconf)) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_expire_gc(): failed: chxj_cookie_expire_gc_dbm()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return;
    }
  }
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
}

apr_time_t
chxj_parse_cookie_expires(const char *s)
{
  if (!s) return (apr_time_t)0;
  return apr_date_parse_rfc(s);
}


cookie_lock_t *
__chxj_cookie_lock(request_rec *r, const char *filename, int line)
{
  mod_chxj_config *dconf;
  apr_status_t rv;
  int done_proc = 0;
  cookie_lock_t *ret = NULL;

  DBG(r,"REQ[%X] start %s() call from %s:%d",TO_ADDR(r),__func__, filename,line);
  if ((rv = apr_proc_mutex_lock(global_cookie_mutex)) != APR_SUCCESS) {
    char errstr[255];
    ERR(r,"REQ[%X] %s:%d apr_proc_mutex_lock failure.(%d:%s)", TO_ADDR(r),APLOG_MARK, rv, apr_strerror(rv, errstr, 255));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_cookie_lock_mysql(r, dconf)) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_lock(): failed: chxj_cookie_lock_mysql()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return NULL;
    }
    done_proc = 1;
    ret = apr_palloc(r->pool, sizeof(*ret));
    memset(ret, 0, sizeof(*ret));
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_cookie_lock_memcache(r, dconf)) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_lock(): failed: chxj_cookie_lock_memcache()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
      return NULL;
    }
    done_proc = 1;
    ret = apr_palloc(r->pool, sizeof(*ret));
    memset(ret, 0, sizeof(*ret));
  }
#endif
  if (!done_proc) {
    if (!(ret = chxj_cookie_lock_dbm(r, dconf))) {
      ERR(r,"REQ[%X] %s:%d end chxj_cookie_lock(): failed: chxj_cookie_lock_dbm()", TO_ADDR(r),APLOG_MARK);
      DBG(r,"REQ[%X] end %s() call from %s:%d",TO_ADDR(r),__func__,filename,line);
      return NULL;
    }
  }
  DBG(r,"REQ[%X] end %s() call from %s:%d",TO_ADDR(r),__func__,filename,line);
  return ret;
}


int
__chxj_cookie_unlock(request_rec *r, cookie_lock_t *lock, const char *filename, int line)
{
  mod_chxj_config *dconf;
  int done_proc = 0;
  apr_status_t rv;
  int rtn = 1;

  DBG(r,"REQ[%X] start %s() call from %s:%d", TO_ADDR(r),__func__,filename, line);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_cookie_unlock_mysql(r, dconf)) {
      ERR(r,"REQ[%X] failed: chxj_cookie_unlock_mysql()",TO_ADDR(r));
      rtn = 0;
      goto end_chxj_cookie_unlock;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_cookie_unlock_memcache(r, dconf)) {
      ERR(r,"REQ[%X] failed: chxj_cookie_unlock_memcache()",TO_ADDR(r));
      rtn = 0;
      goto end_chxj_cookie_unlock;
    }
    done_proc = 1;
  }
#endif
  if (!done_proc) {
    if (! chxj_cookie_unlock_dbm(r, lock, dconf)) {
      ERR(r,"REQ[%X] failed: chxj_cookie_unlock_dbm()",TO_ADDR(r));
      rtn = 0;
      goto end_chxj_cookie_unlock;
    }
  }
end_chxj_cookie_unlock:
  if ((rv = apr_proc_mutex_unlock(global_cookie_mutex)) != APR_SUCCESS) {
    char errstr[255];
    ERR(r,"REQ[%X] %s:%d apr_proc_mutex_unlock failure.(%d:%s)", TO_ADDR(r),APLOG_MARK, rv, apr_strerror(rv, errstr, 255));
    DBG(r,"REQ[%X] end %s() call from %s:%d", TO_ADDR(r),__func__,filename, line);
    return 0;
  }
  DBG(r,"REQ[%X] end %s() call from %s:%d", TO_ADDR(r),__func__,filename, line);

  return rtn;
}


#include "chxj_encoding.h"
char *
s_convert_a_tag(request_rec *r, const char *s, apr_size_t *len, cookie_t *cookie)
{
  apr_pool_t *pool;
  ap_regex_t *regexp = NULL;
  int   nowpos = 0;
  Doc       doc;
  char *dst = NULL;

  apr_pool_create(&pool, r->pool);
  regexp = ap_pregcomp(pool, "(<a[^>]*>)", AP_REG_EXTENDED|AP_REG_ICASE);
  doc.r = r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  while(1) {
    ap_regmatch_t        match[10];
    if (ap_regexec((const ap_regex_t *)regexp, &s[nowpos], (apr_size_t)regexp->re_nsub + 1, match, 0) == 0) {
      apr_size_t plen = match[1].rm_so;
      if (plen > 0) {
        char *tmpstr = apr_palloc(pool, plen + 1);
        memset(tmpstr, 0, plen + 1);
        memcpy(tmpstr, &s[nowpos], plen);
        dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
      }
      else {
        plen = 0;
      }
      char *matchstr = ap_pregsub(pool, "$1", &s[nowpos], regexp->re_nsub + 1, match);
      if (matchstr) {
        Node *node = qs_parse_tag(&doc, matchstr, strlen(matchstr) - 1);
        Attr *attr;

        DBG(r,"REQ[%X] matchstr:[%s]", TO_ADDR(r),matchstr);
        dst = apr_pstrcat(pool, (dst) ? dst : "", "<a ", NULL);
        for (attr = qs_get_attr(&doc,node);
             attr;
             attr = qs_get_next_attr(&doc,attr)) {
          char *name  = qs_get_attr_name(&doc,attr);
          char *value = qs_get_attr_value(&doc,attr);
          DBG(r,"REQ[%X] name:[%s] value=[%s]",TO_ADDR(r), name, value);
          if (STRCASEEQ('h', 'H', "href", name)
              && ! chxj_starts_with(value, "mailto:") 
              && ! chxj_starts_with(value, "tel:")) {
            char *flgp = strchr(value, '#');
            char *flgsv = NULL;
            if (flgp) {
              int flgl = strlen(flgp);
              flgsv = apr_palloc(pool, flgl+1);
              memset(flgsv, 0, flgl + 1);
              memcpy(flgsv, flgp, flgl);
              *flgp = 0;
            }
            if (strchr(value, '?') != 0) {
              value = apr_pstrcat(pool, value, "&_chxj_cc=", cookie->cookie_id, NULL);
            }
            else {
              value = apr_pstrcat(pool, value, "?_chxj_cc=", cookie->cookie_id, NULL);
            }
            if (flgsv) {
              value = apr_pstrcat(pool, value, flgsv, NULL);
            }
            dst = apr_pstrcat(pool, (dst) ? dst : "", "href='", value, "' ", NULL);
          }
          else {
            dst = apr_pstrcat(pool, (dst) ? dst : "", name, "='", value, "' ", NULL);
          }
        }
        dst = apr_pstrcat(pool, (dst) ? dst : "", ">", NULL);
        plen += strlen(matchstr);
      }
      nowpos += plen;
    }
    else {
      break;
    }
  }

  if ((apr_size_t)nowpos < *len) {
    apr_size_t plen = *len - nowpos;
    char *tmpstr = apr_palloc(pool, plen + 1);
    memset(tmpstr, 0, plen+1);
    memcpy(tmpstr, &s[nowpos], plen);
    dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
  }

  *len = strlen(dst);
  return dst;
}
char *
s_convert_img_tag(request_rec *r, const char *s, apr_size_t *len, cookie_t *cookie)
{
  apr_pool_t *pool;
  ap_regex_t *regexp = NULL;
  int   nowpos = 0;
  Doc       doc;
  char *dst = NULL;

  apr_pool_create(&pool, r->pool);
  regexp = ap_pregcomp(pool, "(<img[^>]*>)", AP_REG_EXTENDED|AP_REG_ICASE);
  doc.r = r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  while(1) {
    ap_regmatch_t        match[10];
    if (ap_regexec((const ap_regex_t *)regexp, &s[nowpos], (apr_size_t)regexp->re_nsub + 1, match, 0) == 0) {
      apr_size_t plen = match[1].rm_so;
      if (plen > 0) {
        char *tmpstr = apr_palloc(pool, plen + 1);
        memset(tmpstr, 0, plen + 1);
        memcpy(tmpstr, &s[nowpos], plen);
        dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
      }
      else {
        plen = 0;
      }
      char *matchstr = ap_pregsub(pool, "$1", &s[nowpos], regexp->re_nsub + 1, match);
      if (matchstr) {
        Node *node = qs_parse_tag(&doc, matchstr, strlen(matchstr) - 1);
        Attr *attr;

        DBG(r,"REQ[%X] matchstr:[%s]", TO_ADDR(r),matchstr);
        dst = apr_pstrcat(pool, (dst) ? dst : "", "<img ", NULL);
        for (attr = qs_get_attr(&doc,node);
             attr;
             attr = qs_get_next_attr(&doc,attr)) {
          char *name  = qs_get_attr_name(&doc,attr);
          char *value = qs_get_attr_value(&doc,attr);
          DBG(r,"REQ[%X] name:[%s] value=[%s]", TO_ADDR(r),name, value);
          if (STRCASEEQ('s', 'S', "src", name)) {
            if (strchr(value, '?') != 0) {
              value = apr_pstrcat(pool, value, "&_chxj_cc=", cookie->cookie_id, "&_chxj_nc=true", NULL);
            }
            else {
              value = apr_pstrcat(pool, value, "?_chxj_cc=", cookie->cookie_id, "&_chxj_nc=true", NULL);
            }
            dst = apr_pstrcat(pool, (dst) ? dst : "", "src='", value, "' ", NULL);
          }
          else {
            dst = apr_pstrcat(pool, (dst) ? dst : "", name, "='", value, "' ", NULL);
          }
        }
        dst = apr_pstrcat(pool, (dst) ? dst : "", "/>", NULL);
        plen += strlen(matchstr);
      }
      nowpos += plen;
    }
    else {
      break;
    }
  }

  if ((apr_size_t)nowpos < *len) {
    apr_size_t plen = *len - nowpos;
    char *tmpstr = apr_palloc(pool, plen + 1);
    memset(tmpstr, 0, plen+1);
    memcpy(tmpstr, &s[nowpos], plen);
    dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
  }

  *len = strlen(dst);
  return dst;
}
char *
s_convert_form_tag(request_rec *r, const char *s, apr_size_t *len, cookie_t *cookie)
{
  apr_pool_t *pool;
  ap_regex_t *regexp = NULL;
  int   nowpos = 0;
  Doc       doc;
  char *dst = NULL;
  char *cookie_id;

  apr_pool_create(&pool, r->pool);
  regexp = ap_pregcomp(pool, "(</form>)", AP_REG_EXTENDED|AP_REG_ICASE);
  doc.r = r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  cookie_id = chxj_url_decode(pool, cookie->cookie_id);

  while(1) {
    ap_regmatch_t        match[10];
    if (ap_regexec((const ap_regex_t *)regexp, &s[nowpos], (apr_size_t)regexp->re_nsub + 1, match, 0) == 0) {
      apr_size_t plen = match[1].rm_so;
      if (plen > 0) {
        char *tmpstr = apr_palloc(pool, plen + 1);
        memset(tmpstr, 0, plen + 1);
        memcpy(tmpstr, &s[nowpos], plen);
        dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
      }
      else {
        plen = 0;
      }
      char *matchstr = ap_pregsub(pool, "$1", &s[nowpos], regexp->re_nsub + 1, match);
      if (matchstr) {
        DBG(r,"REQ[%X] matchstr:[%s]",TO_ADDR(r),matchstr);
        dst = apr_pstrcat(pool, (dst) ? dst : "", "<input type='hidden' name='_chxj_cc' value='", cookie_id, "' />", matchstr, NULL);
        plen += strlen(matchstr);
      }
      nowpos += plen;
    }
    else {
      break;
    }
  }

  if ((apr_size_t)nowpos < *len) {
    apr_size_t plen = *len - nowpos;
    char *tmpstr = apr_palloc(pool, plen + 1);
    memset(tmpstr, 0, plen + 1);
    memcpy(tmpstr, &s[nowpos], plen);
    dst = apr_pstrcat(pool, (dst) ? dst : "", tmpstr, NULL);
  }

  *len = strlen(dst);
  return dst;
}

char *
chxj_cookie_only_mode(request_rec *r, const char *src, apr_size_t *len, cookie_t* cookie)
{
  char *s;
  char *result;
  char *dst = NULL;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  s = chxj_encoding(r, src, len);

  dst = s_convert_a_tag(r, s, len, cookie);
  dst = s_convert_img_tag(r, dst, len, cookie);
  dst = s_convert_form_tag(r, dst, len, cookie);

  result = chxj_rencoding(r, dst, len, NULL);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return result;
}
/*
 * vim:ts=2 et
 */
