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
#include "chxj_str_util.h"
#if !defined (__CHXJ_STR_UTIL_C__)
#define __CHXJ_STR_UTIL_C__

int
chxj_chk_numeric(const char *s)
{
  int len;
  int ii;

  if (! s) return -1; /* NG(not numeric) */

  len = strlen(s);
  if (len == 0) return -1; /* NG(not numeric) */

  for (ii=0; ii<len; ii++) {
    if (ii == 0 && (s[ii] < '0' || s[ii] > '9') && s[ii] != '-') {
      return -1; /* NG(not numeric) */
    }
    if (ii != 0 && (s[ii] < '0' || s[ii] > '9')) {
      return -1; /* NG(not numeric) */
    }
  }

  return 0; /* OK */
}


int
_chxj_atoi(const char *s, int base)
{
  int len;
  int ii;
  int result;
  int mflag = 0;
  int break_flag = 0;

  if (! s) return 0;

  result = 0;

  len = strlen(s);

  for (ii=0; ii < len && break_flag == 0; ii++) {
    result *= base;
    switch(s[ii]) {
    case '0': result += 0; break; 
    case '1': result += 1; break;
    case '2': result += 2; break;
    case '3': result += 3; break;
    case '4': result += 4; break;
    case '5': result += 5; break;
    case '6': result += 6; break;
    case '7': result += 7; break;
    case '8': result += 8; break;
    case '9': result += 9; break;
    case 'a': case 'A': result += 10; break;
    case 'b': case 'B': result += 11; break;
    case 'c': case 'C': result += 12; break;
    case 'd': case 'D': result += 13; break;
    case 'e': case 'E': result += 14; break;
    case 'f': case 'F': result += 15; break;
    case '-': 
      result /= base;
      if (ii != 0) {
        break_flag = 1;
        break;
      }
      mflag = 1;
      break;

    case '+':
      result /= base;
      if (ii != 0) {
        break_flag = 1;
        break;
      }
      break;

    default: 
      break_flag = 1;
      result /= base;
      break;
    }
  }

  if (mflag)
    result *= -1;

  return result;
}

int
chxj_atoi(const char *s)
{
  return _chxj_atoi(s, 10);
}
int
chxj_axtoi(const char *s)
{
  return _chxj_atoi(s, 16);
}

int
chxj_strcasenrcmp(apr_pool_t *p, const char *s1, const char *s2, int n)
{
    register int s1_len;
    register int s2_len;
    char *ss1;
    char *ss2;
    register char *ss1p;
    register char *ss2p;

    if (! s1) {
      s1_len = 0;
    }
    else {
      s1_len = strlen(s1) - 1;
    }
    if (! s2) {
      s2_len = 0;
    }
    else {
      s2_len = strlen(s2) - 1;
    }

    ss1 = (char *)apr_palloc(p, s1_len + 2);
    if (!ss1) {
      return -1;
    }
    ss2 = (char *)apr_palloc(p, s2_len + 2);
    if (!ss2) {
      return -1;
    }

    ss1[1] = ss1[0] = 0;
    ss2[1] = ss2[0] = 0;
    if (s1) {
      strcpy(&ss1[1], s1);
    }
    if (s2) {
      strcpy(&ss2[1], s2);
    }
    ss1p = &ss1[s1_len+1];
    ss2p = &ss2[s2_len+1];

    for (;*ss1p && *ss2p && *ss1p == *ss2p && n - 1 > 0; ss1p--, ss2p--, n--);
    return (int)(*ss1p - *ss2p);
}


int
chxj_starts_with(const char *str, const char *word)
{
  int len;
  char *w = (char *)word;
  char *s = (char *)str;
  if (! w) w = "";
  if (! s) s = "";
  len = strlen(w);
  if (len == 0) len = 1;
  return strncasecmp(s, w, len) == 0;
}


char *
chxj_add_slash_to_doublequote(apr_pool_t *pool, const char *str)
{
  char *ret;
  int  len;
  int  tlen;
  int  ii;
  int  pos;
  int  cnt;

  len = strlen(str);
  cnt = 0;
  for (ii=0; ii<len; ii++) {
    if (str[ii] == '"') {
      cnt++;
    }
  }
  tlen = (len - cnt)  + (cnt * (sizeof("&quot;")-1)) + 1;
  ret = apr_palloc(pool, tlen);
  memset(ret, 0, tlen);
  pos = 0;
  for (ii=0; ii<len; ii++) {
    if (str[ii] == '"') {
      strcpy(&ret[pos], "&quot;");
      pos += sizeof("&quot;")-1;
    }
    else {
      ret[pos++] = str[ii];
    }
  }
  return ret;
}


int
chxj_strcount(const char *s, const char *str)
{
  register char *p = (char *)s;
  register char i;
  int len = strlen(str);
  int count = 0;
  while (*p) {
    for (i=0; i<len; i++) {
      if (*p == str[(unsigned int)i]) {
        count++;
        break;
      }
    }
    p++;
  }
  return count;
}
#endif
/*
 * vim:ts=2 et
 */
