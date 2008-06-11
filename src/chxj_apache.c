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
#include "chxj_apache.h"
#if defined(CHXJ_TEST)
#include "pcre.h"
#undef ap_regex_t
#undef ap_regmatch_t
#ifndef POSIX_MALLOC_THRESHOLD
#define POSIX_MALLOC_THRESHOLD (10)
#endif
#endif

const char *
chxj_apache_run_http_scheme(request_rec *r)
{
#if defined(CHXJ_TEST)
  return test_run_http_scheme(r);
#else
#  if AP_SERVER_MAJORVERSION_NUMBER == 2 && AP_SERVER_MINORVERSION_NUMBER == 2
  return ap_run_http_scheme(r);
#  else
  return ap_run_http_method(r);
#  endif
#endif
}

char *chxj_os_escape_path(apr_pool_t *p, const char *path, int partial) 
{
#if defined(CHXJ_TEST)
  return test_os_escape_path(p, path, partial);
#else
  return ap_os_escape_path(p, path, partial);
#endif
}


void
chxj_set_content_type(request_rec *r, const char *ct)
{
#if defined(CHXJ_TEST)
  test_set_content_type(r, ct);
#else
  ap_set_content_type(r, ct);
#endif
}


void *
chxj_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
#if defined(CHXJ_TEST)
  return test_get_module_config(cv,m);
#else
  return ap_get_module_config(cv,m);
#endif
}


char *
chxj_ap_escape_html(apr_pool_t *p, const char *s) 
{
#if defined(CHXJ_TEST)
  return test_ap_escape_html(p, s);
#else
  return ap_escape_html(p, s);
#endif
}


int
chxj_ap_regcomp(ap_regex_t *preg, const char *pattern, int cflags)
{
#if defined(CHXJ_TEST)
  const char *errorptr;
  int erroffset;
  int options = 0;
  
  if ((cflags & AP_REG_ICASE) != 0) options |= PCRE_CASELESS;
  if ((cflags & AP_REG_NEWLINE) != 0) options |= PCRE_MULTILINE;
  
  preg->re_pcre = pcre_compile(pattern, options, &errorptr, &erroffset, NULL);
  preg->re_erroffset = erroffset;
  
  if (preg->re_pcre == NULL) return AP_REG_INVARG;
  
  preg->re_nsub = pcre_info((const pcre *)preg->re_pcre, NULL, NULL);
  return 0;
#else
  return ap_regcomp(preg,pattern,cflags);
#endif
}

ap_regex_t *
chxj_ap_pregcomp(apr_pool_t *p, const char *pattern, int cflags)
{
#if defined(CHXJ_TEST)
  ap_regex_t *preg = apr_palloc(p, sizeof(*preg));
  if (chxj_ap_regcomp(preg, pattern, cflags)) {
    return NULL;
  }
  return preg;
#else
  return ap_pregcomp(p, pattern, cflags);
#endif
}
void 
chxj_ap_pregfree(apr_pool_t *p, ap_regex_t *reg)
{
#if defined(CHXJ_TEST)
#else
  return ap_pregfree(p, reg);
#endif
}

int
chxj_ap_regexec(const ap_regex_t *preg, const char *string, apr_size_t nmatch, ap_regmatch_t *pmatch, int eflags)
{
#if defined(CHXJ_TEST)
  /* copy from apache2.2.8 server/util_pcre.c */
  int rc;
  int options = 0;
  int *ovector = NULL;
  int small_ovector[POSIX_MALLOC_THRESHOLD * 3];
  int allocated_ovector = 0;

  if ((eflags & AP_REG_NOTBOL) != 0) options |= PCRE_NOTBOL;
  if ((eflags & AP_REG_NOTEOL) != 0) options |= PCRE_NOTEOL;

  ((ap_regex_t *)preg)->re_erroffset = (apr_size_t)(-1);  /* Only has meaning after compile */

  if (nmatch > 0) {
    if (nmatch <= POSIX_MALLOC_THRESHOLD) {
      ovector = &(small_ovector[0]);
    }
    else {
      ovector = (int *)malloc(sizeof(int) * nmatch * 3);
      if (ovector == NULL) return AP_REG_ESPACE;
      allocated_ovector = 1;
    }
  }

  rc = pcre_exec((const pcre *)preg->re_pcre, NULL, string, (int)strlen(string), 0, options, ovector, nmatch * 3);
  if (rc == 0) rc = nmatch;    /* All captured slots were filled in */
  if (rc >= 0) {
    apr_size_t i;
    for (i = 0; i < (apr_size_t)rc; i++) {
      pmatch[i].rm_so = ovector[i*2];
      pmatch[i].rm_eo = ovector[i*2+1];
    }
    if (allocated_ovector) free(ovector);
    for (; i < nmatch; i++) pmatch[i].rm_so = pmatch[i].rm_eo = -1;
    return 0;
  } 
  else {
    if (allocated_ovector) free(ovector);
    switch(rc) {
    case PCRE_ERROR_NOMATCH: return AP_REG_NOMATCH;
    case PCRE_ERROR_NULL: return AP_REG_INVARG;
    case PCRE_ERROR_BADOPTION: return AP_REG_INVARG;
    case PCRE_ERROR_BADMAGIC: return AP_REG_INVARG;
    case PCRE_ERROR_UNKNOWN_NODE: return AP_REG_ASSERT;
    case PCRE_ERROR_NOMEMORY: return AP_REG_ESPACE;
#ifdef PCRE_ERROR_MATCHLIMIT
    case PCRE_ERROR_MATCHLIMIT: return AP_REG_ESPACE;
#endif
#ifdef PCRE_ERROR_BADUTF8
    case PCRE_ERROR_BADUTF8: return AP_REG_INVARG;
#endif
#ifdef PCRE_ERROR_BADUTF8_OFFSET
    case PCRE_ERROR_BADUTF8_OFFSET: return AP_REG_INVARG;
#endif
    default: return AP_REG_ASSERT;
    }
  }
#else
  return ap_regexec(preg, string, nmatch, pmatch, eflags);
#endif
}

char *
chxj_ap_pregsub(apr_pool_t *p, const char *input, const char *source, size_t nmatch, ap_regmatch_t pmatch[])
{
#if defined(CHXJ_TEST)
  /* copy from apache2.2.8 server/util.c */
  const char *src = input;
  char *dest, *dst;
  char c;
  size_t no;
  int len;

  if (!source) return NULL;
  if (!nmatch) return apr_pstrdup(p, src);

  len = 0;
  while ((c = *src++) != '\0') {
    if (c == '&') no = 0;
    else if (c == '$' && apr_isdigit(*src))
      no = *src++ - '0';
    else
      no = 10;

    if (no > 9) {                /* Ordinary character. */
      if (c == '\\' && (*src == '$' || *src == '&'))
        c = *src++;
      len++;
    }
    else if (no < nmatch && pmatch[no].rm_so < pmatch[no].rm_eo) {
      len += pmatch[no].rm_eo - pmatch[no].rm_so;
    }
  }
  dest = dst = apr_pcalloc(p, len + 1);
  /* Now actually fill in the string */
  src = input;
  while ((c = *src++) != '\0') {
    if (c == '&')
      no = 0;
    else if (c == '$' && apr_isdigit(*src))
      no = *src++ - '0';
    else
      no = 10;

    if (no > 9) {                /* Ordinary character. */
      if (c == '\\' && (*src == '$' || *src == '&'))
         c = *src++;
      *dst++ = c;
    }
    else if (no < nmatch && pmatch[no].rm_so < pmatch[no].rm_eo) {
      len = pmatch[no].rm_eo - pmatch[no].rm_so;
      memcpy(dst, source + pmatch[no].rm_so, len);
      dst += len;
    }
  }
  *dst = '\0';

  return dest;
#else
  return ap_pregsub(p, input, source, nmatch, pmatch);
#endif
}
/*
 * vim:ts=2 et
 */
