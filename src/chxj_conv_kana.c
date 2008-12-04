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
#include "mod_chxj.h"
#include "chxj_conv_kana.h"
#include "chxj_url_encode.h"
#include "qs_parse_string.h"
#include <errno.h>

typedef struct {
  apr_size_t byte;
  char *hankaku;
} kana_table_t;

static kana_table_t kana_table1[] = {
  { /* 00 */ 1, "\xa7\x00",},
  { /* 01 */ 1, "\xb1\x00",},
  { /* 02 */ 1, "\xa8\x00",},
  { /* 03 */ 1, "\xb2\x00",},
  { /* 04 */ 1, "\xa9\x00",},
  { /* 05 */ 1, "\xb3\x00",},
  { /* 06 */ 1, "\xaa\x00",},
  { /* 07 */ 1, "\xb4\x00",},
  { /* 08 */ 1, "\xab\x00",},
  { /* 09 */ 1, "\xb5\x00",},
  { /* 10 */ 1, "\xb6\x00",},
  { /* 11 */ 2, "\xb6\xde",},
  { /* 12 */ 1, "\xb7\x00",},
  { /* 13 */ 2, "\xb7\xde",},
  { /* 14 */ 1, "\xb8\x00",},
  { /* 15 */ 2, "\xb8\xde",},
  { /* 16 */ 1, "\xb9\x00",},
  { /* 17 */ 2, "\xb9\xde",},
  { /* 18 */ 1, "\xba\x00",},
  { /* 19 */ 2, "\xba\xde",},
  { /* 20 */ 1, "\xbb\x00",},
  { /* 21 */ 2, "\xbb\xde",},
  { /* 22 */ 1, "\xbc\x00",},
  { /* 23 */ 2, "\xbc\xde",},
  { /* 24 */ 1, "\xbd\x00",},
  { /* 25 */ 2, "\xbd\xde",},
  { /* 26 */ 1, "\xbe\x00",},
  { /* 27 */ 2, "\xbe\xde",},
  { /* 28 */ 1, "\xbf\x00",},
  { /* 29 */ 2, "\xbf\xde",},
  { /* 30 */ 1, "\xc0\x00",},
  { /* 31 */ 2, "\xc0\xde",},
  { /* 32 */ 1, "\xc1\x00",},
  { /* 33 */ 2, "\xc1\xde",},
  { /* 34 */ 1, "\xaf\x00",},
  { /* 35 */ 1, "\xc2\x00",},
  { /* 36 */ 2, "\xc2\xde",},
  { /* 37 */ 1, "\xc3\x00",},
  { /* 38 */ 2, "\xc3\xde",},
  { /* 39 */ 1, "\xc4\x00",},
  { /* 40 */ 2, "\xc4\xde",},
  { /* 41 */ 1, "\xc5\x00",},
  { /* 42 */ 1, "\xc6\x00",},
  { /* 43 */ 1, "\xc7\x00",},
  { /* 44 */ 1, "\xc8\x00",},
  { /* 45 */ 1, "\xc9\x00",},
  { /* 46 */ 1, "\xca\x00",},
  { /* 47 */ 2, "\xca\xde",},
  { /* 48 */ 2, "\xca\xdf",},
  { /* 49 */ 1, "\xcb\x00",},
  { /* 50 */ 2, "\xcb\xde",},
  { /* 51 */ 2, "\xcb\xdf",},
  { /* 52 */ 1, "\xcc\x00",},
  { /* 53 */ 2, "\xcc\xde",},
  { /* 54 */ 2, "\xcc\xdf",},
  { /* 55 */ 1, "\xcd\x00",},
  { /* 56 */ 2, "\xcd\xde",},
  { /* 57 */ 2, "\xcd\xdf",},
  { /* 58 */ 1, "\xce\x00",},
  { /* 59 */ 2, "\xce\xde",},
  { /* 60 */ 2, "\xce\xdf",},
  { /* 61 */ 1, "\xcf\x00",},
  { /* 62 */ 1, "\xd0\x00",},
  { /* 63 */ 0, "\x00\x00",},
  { /* 64 */ 1, "\xd1\x00",},
  { /* 65 */ 1, "\xd2\x00",},
  { /* 66 */ 1, "\xd3\x00",},
  { /* 67 */ 1, "\xac\x00",},
  { /* 68 */ 1, "\xd4\x00",},
  { /* 69 */ 1, "\xad\x00",},
  { /* 70 */ 1, "\xd5\x00",},
  { /* 71 */ 1, "\xae\x00",},
  { /* 72 */ 1, "\xd6\x00",},
  { /* 73 */ 1, "\xd7\x00",},
  { /* 74 */ 1, "\xd8\x00",},
  { /* 75 */ 1, "\xd9\x00",},
  { /* 76 */ 1, "\xda\x00",},
  { /* 77 */ 1, "\xdb\x00",},
  { /* 78 */ 2, "\x83\x8e",},
  { /* 79 */ 1, "\xdc\x00",},
  { /* 80 */ 2, "\x83\x90",},
  { /* 81 */ 2, "\x83\x91",},
  { /* 82 */ 1, "\xa6\x00",},
  { /* 83 */ 1, "\xdd\x00",},
  { /* 84 */ 2, "\xb3\xde",},
  { /* 85 */ 2, "\x83\x95",},
  { /* 86 */ 2, "\x83\x96",},
};
static kana_table_t kana_table2[] = {
  { /* 0x8141 、*/ 1, "\xa4",},
  { /* 0x8142 。*/ 1, "\xa1",},
  { /* 0x8143 ，*/ 1, ",",},
  { /* 0x8144 ．*/ 1, ".",},
  { /* 0x8145 ・*/ 1, "\xa5",},
  { /* 0x8146 ：*/ 1, ":",},
  { /* 0x8147 ；*/ 1, ";",},
  { /* 0x8148 ？*/ 1, "?",},
  { /* 0x8149 ！*/ 1, "!",},
  { /* 0x814a ゛*/ 1, "\xde",},
  { /* 0x814b ゜*/ 1, "\xdf",},
};
static kana_table_t kana_table3[] = {
  { /* 0x8175 「*/ 1, "\xa2",},
  { /* 0x8176 」*/ 1, "\xa3",},
};
static kana_table_t kana_table4[] = {
  { /* 0x815b ゛*/ 1, "-",},
};

/**
 */
char *
chxj_conv_z2h_kana(request_rec *r, const char *src, apr_size_t *len, chxjconvrule_entry *entryp)
{
  apr_size_t          ii;
  apr_size_t          ilen;
  apr_pool_t          *pool;
  char                *obuf;
  apr_size_t          olen;

  DBG(r,"REQ[%X] start chxj_conv_z2h_kana()", (unsigned int)(apr_size_t)r);

  if (entryp->action & CONVRULE_Z2H_OFF_BIT) {
    DBG(r,"REQ[%X] Detect Z2hOff", (unsigned int)(apr_size_t)r);
    DBG(r,"REQ[%X] end chxj_conv_z2h_kana()", (unsigned int)(apr_size_t)r);
    return (char *)src;
  }
  if (! (entryp->action & CONVRULE_Z2H_ON_BIT)) {
    DBG(r,"REQ[%X] Detect Z2hOff", (unsigned int)(apr_size_t)r);
    DBG(r,"REQ[%X] end chxj_conv_z2h_kana()", (unsigned int)(apr_size_t)r);
    return (char *)src;
  }

  apr_pool_create(&pool, r->pool);

  olen = 0;
  ilen = *len;

  obuf = apr_palloc(pool, ilen + 1);
  if (! obuf) {
    ERR(r,"%s:%d REQ[%X] memory allocation error", __FILE__,__LINE__,(unsigned int)(apr_size_t)r);
    return (char*)src;
  }

  memset(obuf, 0, ilen + 1);
  for (ii=0; ii<ilen; ii++) {
    /* sjis only */
    if (is_sjis_kana(src[ii])) {
      obuf[olen++] = src[ii];
    }
    else if (is_sjis_kanji(src[ii])) {
      unsigned char firstbyte  = src[ii + 0];
      unsigned char secondbyte = src[ii + 1];
      if (   firstbyte == 0x83
          && (secondbyte >= 0x40 && secondbyte <= 0x96)) {
        unsigned char p = secondbyte - 0x40;
        if (kana_table1[p].byte != 0) {
          /* Detect Zenkakaku Kana */
          strcpy(&obuf[olen], kana_table1[p].hankaku);
          olen += kana_table1[p].byte;
        }
        else {
          obuf[olen++] = src[ii + 0];
          obuf[olen++] = src[ii + 1];
        }
      }
      else
      if (   firstbyte == 0x81
          && (secondbyte >= 0x41 && secondbyte <= 0x4b)) {
        unsigned char p = secondbyte - 0x41;
        if (kana_table2[p].byte != 0) {
          /* Detect Zenkakaku Kana */
          strcpy(&obuf[olen], kana_table2[p].hankaku);
          olen += kana_table2[p].byte;
        }
        else {
          obuf[olen++] = src[ii + 0];
          obuf[olen++] = src[ii + 1];
        }
      }
      else
      if (   firstbyte == 0x81
          && (secondbyte >= 0x75 && secondbyte <= 0x76)) {
        unsigned char p = secondbyte - 0x75;
        if (kana_table3[p].byte != 0) {
          /* Detect Zenkakaku Kana */
          strcpy(&obuf[olen], kana_table3[p].hankaku);
          olen += kana_table3[p].byte;
        }
        else {
          obuf[olen++] = src[ii + 0];
          obuf[olen++] = src[ii + 1];
        }
      }
      else
      if (   firstbyte == 0x81
          && (secondbyte >= 0x5b && secondbyte <= 0x5b)) {
        unsigned char p = secondbyte - 0x5b;
        if (kana_table4[p].byte != 0) {
          /* Detect Zenkakaku Kana */
          strcpy(&obuf[olen], kana_table4[p].hankaku);
          olen += kana_table4[p].byte;
        }
        else {
          obuf[olen++] = src[ii + 0];
          obuf[olen++] = src[ii + 1];
        }
      }
      else {
        obuf[olen++] = src[ii + 0];
        obuf[olen++] = src[ii + 1];
      }
      ii++;
    }
    else {
      obuf[olen++] = src[ii];
    }
  }
  *len = olen;

  DBG(r,"REQ[%X] end chxj_conv_z2h_kana()", (unsigned int)(apr_size_t)r);
  return obuf;
}
/*
 * vim: ts=2 et
 */
