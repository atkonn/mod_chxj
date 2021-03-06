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
#include "mod_chxj.h"
#include "chxj_url_encode.h"

#define IS_ALPHA_UPPER(X) (((X) >='A') && ((X) <= 'Z'))
#define IS_ALPHA_LOWER(X) (((X) >='a') && ((X) <= 'z'))
#define IS_DIGIT(X)       (((X) >='0') && ((X) <= '9'))
#define TO_HEXSTRING(X)   (((X) < 10) ? (X) + '0' : (X) + ('A' - 10)) 
#define IS_HEXCHAR(X)     (                             \
                             ((X) >= 'A' && (X) <= 'F') \
                          || ((X) >= 'a' && (X) <= 'f') \
                          || ((X) >= '0' && (X) <= '9') \
                          )

static inline char 
s_hex_value(char c)
{
  if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
  if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
  if (c >= '0' && c <= '9') return (c - '0' +  0);
  return 0;
}

char *
chxj_url_encode(apr_pool_t *pool, const char *src)
{
  char *dst;
  register char *sp = (char *)src;
  int len;
  register int pos;

  if (! src) return apr_pstrdup(pool, "\0");

  len = strlen(src) * 3 + 1;
  dst = apr_palloc(pool, len);
  memset(dst, 0, len);
  pos = 0;

  while(*sp) {
    if (IS_ALPHA_UPPER(*sp) || IS_ALPHA_LOWER(*sp) || IS_DIGIT(*sp)) {
      dst[pos++] = *sp++;
      continue;
    }
    if (*sp == ' ') {
      dst[pos++] = '+';
      sp++;
      continue;
    }

    dst[pos++] = '%';
    dst[pos++] = TO_HEXSTRING((*sp >> 4) & 0x0f);
    dst[pos++] = TO_HEXSTRING((*sp & 0x0f));
    sp++;
  }
  return dst;
}


char *
chxj_url_decode(apr_pool_t *pool, const char *src)
{
  char  *dst;
  int   len; 
  int   ii;
  int   jj;


  if (!src) return apr_pstrdup(pool, "\0");

  len = strlen(src);
  dst = apr_palloc(pool, len+1);
  memset(dst, 0, len+1);

  for (jj=0,ii=0; src[ii] != '\0' && ii < len; ii++) {
    if (src[ii] == '%') {
      if (ii + 2 <= len && IS_HEXCHAR(src[ii+1]) && IS_HEXCHAR(src[ii+2])) {       
        dst[jj++] = s_hex_value(src[ii+1]) * 16 + s_hex_value(src[ii+2]);
        ii+=2;
      }
    }
    else if (src[ii] == '+') {
      dst[jj++] = ' ';
    }
    else {
      dst[jj++] = src[ii];
    }
  }

  return dst;
}
/*
 * vim:ts=2 et
 */
