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
#include "chxj_conv_z2h.h"
#include "chxj_url_encode.h"
#include "qs_parse_string.h"
#include <errno.h>

static z2h_table_t alpha_table_uc[] = {
  { 1, "A",},
  { 1, "B",},
  { 1, "C",},
  { 1, "D",},
  { 1, "E",},
  { 1, "F",},
  { 1, "G",},
  { 1, "H",},
  { 1, "I",},
  { 1, "J",},
  { 1, "K",},
  { 1, "L",},
  { 1, "M",},
  { 1, "N",},
  { 1, "O",},
  { 1, "P",},
  { 1, "Q",},
  { 1, "R",},
  { 1, "S",},
  { 1, "T",},
  { 1, "U",},
  { 1, "V",},
  { 1, "W",},
  { 1, "X",},
  { 1, "Y",},
  { 1, "Z",},
};
static z2h_table_t alpha_table_lc[] = {
  { 1, "a",},
  { 1, "b",},
  { 1, "c",},
  { 1, "d",},
  { 1, "e",},
  { 1, "f",},
  { 1, "g",},
  { 1, "h",},
  { 1, "i",},
  { 1, "j",},
  { 1, "k",},
  { 1, "l",},
  { 1, "m",},
  { 1, "n",},
  { 1, "o",},
  { 1, "p",},
  { 1, "q",},
  { 1, "r",},
  { 1, "s",},
  { 1, "t",},
  { 1, "u",},
  { 1, "v",},
  { 1, "w",},
  { 1, "x",},
  { 1, "y",},
  { 1, "z",},
};

/**
 * 0x8260 -> 0x8279
 * 0x8281 -> 0x829a
 */
char *
chxj_conv_z2h_alpha(request_rec *r, const char *src, apr_size_t *len, chxjconvrule_entry *entryp)
{
  apr_size_t          ii;
  apr_size_t          ilen;
  apr_pool_t          *pool;
  char                *obuf;
  apr_size_t          olen;

  DBG(r,"REQ[%X] start chxj_conv_z2h_alpha()", (unsigned int)(apr_size_t)r);

  if (entryp->action & CONVRULE_Z2H_ALPHA_OFF_BIT) {
    DBG(r,"REQ[%X] Detect Z2hAlphaOff", (unsigned int)(apr_size_t)r);
    DBG(r,"REQ[%X] end chxj_conv_z2h_alpha()", (unsigned int)(apr_size_t)r);
    return (char *)src;
  }
  if (! (entryp->action & CONVRULE_Z2H_ALPHA_ON_BIT)) {
    DBG(r,"REQ[%X] Detect Z2hAlphaOff", (unsigned int)(apr_size_t)r);
    DBG(r,"REQ[%X] end chxj_conv_z2h_alpha()", (unsigned int)(apr_size_t)r);
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
      /* Upper case */
      if (   firstbyte == 0x82
          && (secondbyte >= 0x60 && secondbyte <= 0x79)) {
        unsigned char p = secondbyte - 0x60;
        if (alpha_table_uc[p].byte != 0) {
          /* Detect Zenkaku Alphabet */
          strcpy(&obuf[olen], alpha_table_uc[p].hankaku);
          olen += alpha_table_uc[p].byte;
        }
        else {
          obuf[olen++] = src[ii + 0];
          obuf[olen++] = src[ii + 1];
        }
      }
      else /* Lower case */
      if (   firstbyte == 0x82
          && (secondbyte >= 0x81 && secondbyte <= 0x9a)) {
        unsigned char p = secondbyte - 0x81;
        if (alpha_table_lc[p].byte != 0) {
          /* Detect Zenkaku Alphabet */
          strcpy(&obuf[olen], alpha_table_lc[p].hankaku);
          olen += alpha_table_lc[p].byte;
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

  DBG(r,"REQ[%X] end chxj_conv_z2h_alpha()", (unsigned int)(apr_size_t)r);
  return obuf;
}
/*
 * vim: ts=2 et
 */
