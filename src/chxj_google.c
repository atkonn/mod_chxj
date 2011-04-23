/*
 * Copyright (C) 2011 Atsushi Konno All rights reserved.
 * Copyright (C) 2011 QSDN,Inc. All rights reserved.
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
#include "chxj_google.h"
#include "chxj_preg_replace.h"
#include "chxj_serf.h"
#include "chxj_url_encode.h"
#include "chxj_encoding.h"

#include "apr_uuid.h"
#include "apr_md5.h"


char *
chxj_google_analytics_get_image_url(request_rec *r, const char *pagetitle)
{
  char *url;
  char *referer;
  char *query;
  char *path;
  char *ptitle;
  mod_chxj_config *conf;
  int rdm;
  apr_pool_t *pool;
  apr_size_t plen;

  
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (conf->use_google_analytics != 1 
     || conf->google_analytics_account == NULL
     || conf->google_analytics_target == NULL) {
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return "";
  }
  
  srand(time(0));
  rdm = rand();
  apr_pool_create(&pool, r->pool);

  

  plen = 0;
  ptitle = "";
  if (pagetitle) {
    plen = strlen(pagetitle);
    ptitle = chxj_iconv(r,pool,apr_pstrdup(pool, pagetitle),&plen,"CP932","UTF-8");
  }
  else {
    ptitle = "(no name)";
  }
  url = apr_pstrdup(pool, conf->google_analytics_target);
  url = apr_pstrcat(pool, url, "?", NULL);
  url = apr_pstrcat(pool, url, "utmac=", conf->google_analytics_account, NULL);
  url = apr_pstrcat(pool, url, "&utmn=", apr_psprintf(pool, "%d",rdm), NULL);
  url = apr_pstrcat(pool, url, "&utmdt=", chxj_url_encode(pool, ptitle), NULL);

  referer = (char *)apr_table_get(r->headers_in, "Referer");
  DBG(r, "REQ[%X] referer:[%s]", TO_ADDR(r), referer);
  query = r->args         ? apr_pstrdup(pool, r->args)         : "";
  DBG(r, "REQ[%X] query:[%s]", TO_ADDR(r), query);
  path  = r->unparsed_uri ? apr_pstrdup(pool, r->unparsed_uri) : "";
  DBG(r, "REQ[%X] path:[%s]", TO_ADDR(r), path);
  
  if (! referer || *referer == '\0') {
    referer = "-";
  }
  url = apr_pstrcat(pool, url, "&utmr=", chxj_url_encode(pool,referer), NULL);
  DBG(r, "REQ[%X] url:[%s]", TO_ADDR(r), url);

  if (path && *path != '\0') {
    if (query && *query != '\0') {
      path = apr_pstrcat(pool, path, "?", query, NULL);
    }
    url = apr_pstrcat(pool, url, "&utmp=", chxj_url_encode(pool,path), NULL);
  }
  DBG(r, "REQ[%X] url:[%s]", TO_ADDR(r), url);
  url = apr_pstrcat(pool, url, "&guid=ON", NULL);
  DBG(r, "REQ[%X] url:[%s]", TO_ADDR(r), url);

  int len = strlen(url);
  int ii, jj;
  char *ret = apr_palloc(pool, len * 4 + 1);   
  memset(ret, 0, len * 4 + 1);
  for (ii=0,jj=0; ii<len; ii++) {
    if (url[ii] == '&') {
      ret[jj++] = '&';
      ret[jj++] = 'a';
      ret[jj++] = 'm';
      ret[jj++] = 'p';
      ret[jj++] = ';';
    }
    else {
      ret[jj++] = url[ii];
    }
  }

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  return ret;
}


#define DL_VERSION "4.4sh"
#define DL_COOKIE_NAME "__utmmobile"
#define DL_COOKIE_PATH "/"
#define DL_COOKIE_USER_PERSISTENCE (63072000)
#define DL_UTM_GIF_LOCATION "http://www.google-analytics.com/__utm.gif"


// 1x1 transparent GIF
static char v_GIF_DATA[35] = {
  (char)0x47, (char)0x49, (char)0x46, (char)0x38, (char)0x39, (char)0x61,
  (char)0x01, (char)0x00, (char)0x01, (char)0x00, (char)0x80, (char)0xff,
  (char)0x00, (char)0xff, (char)0xff, (char)0xff, (char)0x00, (char)0x00,
  (char)0x00, (char)0x2c, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
  (char)0x01, (char)0x00, (char)0x01, (char)0x00, (char)0x00, (char)0x02,
  (char)0x02, (char)0x44, (char)0x01, (char)0x00, (char)0x3b
};


static char *
s_get_ip(request_rec *r, const char *remote_address)
{
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  if (! remote_address || *remote_address == '\0') {
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return apr_pstrdup(r->pool, "");
  }

  char *ret = apr_pstrdup(r->pool, remote_address);
  
  char *idx= strrchr(ret, '.');
  if (idx == NULL) {
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return apr_pstrdup(r->pool, "");
  }
  *++idx = '0';
  *++idx = 0;
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  return ret;
}


static char *
s_uniqid(request_rec *r)
{
  char                *uuid_string;
  apr_uuid_t          uuid;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
  return uuid_string;

}

static char *
s_get_visitor_id(request_rec *r, const char *guid, const char *account, const char *user_agent, const char *cookie_str)
{
  unsigned char       *md5_value;
  apr_status_t        retval;
  char                *visitor_id;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  if (cookie_str && *cookie_str != 0) {
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return (char *)cookie_str;
  }
  if (! account) {
    ERR(r, "Please set ChxjGoogleAnalyticsAccount");
  }


  char *message = "";
  if (guid && *guid != 0) {
    message = apr_psprintf(r->pool, "%s%s", guid, account);
  }
  else {
    message = apr_psprintf(r->pool, "%s%s", user_agent, s_uniqid(r));
  }

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);

  retval = apr_md5(md5_value,
                   (const char*)message,
                   strlen(message));
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return NULL;
  }

  DBG(r, "REQ[%X] md5:[%s]", TO_ADDR(r), md5_value);

  visitor_id = apr_pstrdup(r->pool, "0x");
  int ii;
  for (ii=0; ii<APR_MD5_DIGESTSIZE; ii++) {
    visitor_id = apr_pstrcat(r->pool, visitor_id, apr_psprintf(r->pool, "%02X", md5_value[ii]), NULL);
  }
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);

  return visitor_id;
}

static void
s_write_gif_data(request_rec *r)
{
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  apr_table_setn(r->headers_out, "Content-Type", "image/gif");
  apr_table_setn(r->headers_out, "Cache-Control","private, no-cache, no-cache=Set-Cookie, proxy-revalidate");
  apr_table_setn(r->headers_out, "Pragma",       "no-cache");
  apr_table_setn(r->headers_out, "Expires",      "Wed, 17 Sep 1975 21:32:10 GMT");
  apr_table_setn(r->headers_out, "Content-Length", "35");
  ap_rwrite(v_GIF_DATA, 35, r);
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


static void
s_send_request_to_google_analytics(request_rec *r, const char *utm_url)
{
  apr_pool_t *ppool;
  apr_size_t response_len;
  char *data;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  apr_pool_create(&ppool, r->pool);
  
  // ichiou data ni totte oku. tsukawanai kedo.
  data = chxj_serf_get(r, ppool, utm_url, 0, &response_len);
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


static void
s_track_page_view(request_rec *r)
{
  apr_time_t timestamp = apr_time_now();
  char *domain_name = NULL;
  char *document_referer = NULL;
  char *document_path = NULL;
  char *args;
  char *pstate;
  char *vstate;
  char *dates;
  char *account;
  char *user_agent;
  char *pair;
  char *name;
  char *value;
  char *guid_header = NULL;
  char *visitor_id = NULL;
  int utmdebug = 0;
  int ii;
  apr_array_header_t *headers = NULL;
  apr_table_entry_t *hentryp = NULL;
  char *cookie_str = NULL;
  char *utmUrl = NULL;


  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  domain_name = (char *)apr_table_get(r->headers_in, "Host");
  if (domain_name == NULL || *domain_name == '\0') {
    domain_name = "";
  }

  args = r->args ? apr_pstrdup(r->pool, r->args) : "";

  for (;;) {
    char *pair_sv;

    pair = apr_strtok(args, "&", &pstate);
    if (pair == NULL)
      break;

    args = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (! name) continue;
    if (STRCASEEQ('u','U',"utmr",name)) {
      if (value && *value && strcmp(value, "0") != 0) {
        document_referer = chxj_url_decode(r->pool, value);
      }
      else {
        document_referer = "-";
      }
    }
    else if (STRCASEEQ('u','U',"utmp",name)) {
      if (value && *value) {
        document_path = chxj_url_decode(r->pool, value);
      }
      else {
        document_path = "";
      }
    }
    else if (STRCASEEQ('u','U',"utmac",name)) {
      if (value && *value) {
        account = value;
      }
      else {
        ERR(r, "why not set account? - [%s]", r->args);
      }
    }
    else if (STRCASEEQ('u','U',"utmdebug",name)) {
      utmdebug = 1;
    }
  }
  user_agent = (char *)apr_table_get(r->headers_in,"User-Agent");
  if (!user_agent || *user_agent == '\0') {
    user_agent = "";
  }

  headers = (apr_array_header_t*)apr_table_elts(r->headers_in);
  hentryp = (apr_table_entry_t*)headers->elts;

  cookie_str = NULL;
  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(hentryp[ii].key, "Cookie") == 0) {
      DBG(r,"REQ[%X] FOUND Cookie:[%s]",TO_ADDR(r),hentryp[ii].val);
      char *val = hentryp[ii].val;
      for (;;) {
        char *pair_sv = NULL;
        pair = apr_strtok(val, ";", &pstate);
        if (pair == NULL) break;
        val = NULL;
        pair_sv = apr_pstrdup(r->pool, pair);
        name = apr_strtok(pair, "=", &vstate);
        value = apr_strtok(NULL, "=", &vstate);
        if (! name) continue;
        if (strcasecmp(DL_COOKIE_NAME, name) == 0) {
          cookie_str = apr_pstrdup(r->pool, value);
          break;
        }
      }
    }
  }
  
  guid_header = (char *)apr_table_get(r->headers_in, "X-DCMGUID");
  if (!guid_header || *guid_header == 0) {
    guid_header = (char *)apr_table_get(r->headers_in, "X-UP-SUBNO");
  }
  if (!guid_header || *guid_header == 0) {
    guid_header = (char *)apr_table_get(r->headers_in, "X-JPHONE-UID");
  }
  if (!guid_header || *guid_header == 0) {
    guid_header = (char *)apr_table_get(r->headers_in, "X-EM-UID");
  }
  visitor_id = s_get_visitor_id(r, guid_header, account, user_agent, cookie_str);

  dates = apr_pcalloc(r->pool, MAX_STRING_LEN);
  apr_rfc822_date(dates, timestamp + DL_COOKIE_USER_PERSISTENCE);
  apr_table_setn(r->headers_out, "Set-Cookie",apr_psprintf(r->pool, "%s=%s; expires=%s; path=%s", DL_COOKIE_NAME,visitor_id, dates, DL_COOKIE_PATH));


  utmUrl = apr_pstrdup(r->pool, DL_UTM_GIF_LOCATION);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "?", NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "utmwv=", DL_VERSION, NULL);
  srand(time(0));
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmn=", apr_psprintf(r->pool, "%d", rand()), NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmhn=", chxj_url_encode(r->pool, domain_name), NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmr=", chxj_url_encode(r->pool, document_referer), NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmp=", chxj_url_encode(r->pool, document_path), NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmac=", account, NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmcc=__utma%3D999.999.999.999.999.1%3B", NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmvid=", visitor_id, NULL);
  utmUrl = apr_pstrcat(r->pool, utmUrl, "&utmip=", s_get_ip(r,r->connection->remote_ip), NULL);

  DBG(r, "REQ[%X] utmUrl:[%s]", TO_ADDR(r), utmUrl);

  s_send_request_to_google_analytics(r, utmUrl);

  if (utmdebug) {
    apr_table_setn(r->headers_out, "X-GA-MOBILE-URL", utmUrl);
  }

  s_write_gif_data(r);
  DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
}


int
chxj_google_analytics_handler(request_rec *r)
{
  mod_chxj_config *conf;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (conf->use_google_analytics != 1
     || conf->google_analytics_account == NULL
     || conf->google_analytics_target == NULL) {
    DBG(r, "REQ[%X] end %s() (ChxjUseGoogleAnalytics Off)", TO_ADDR(r),__func__);
    return DECLINED;
  }
  if (strcmp(r->handler, "chxj-google-analytics-handler")) {
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return DECLINED;
  }
  s_track_page_view(r);

  chxj_remove_filter(r);
  DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
  return OK;
}


/*
 * vim:ts=2 et
 */
