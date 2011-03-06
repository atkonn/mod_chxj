/*
 * Copyright (C) 2005-2009 Atsushi Konno All rights reserved.
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

/**
 */
char *
chxj_conv_z2h_num(request_rec *r, const char *src, apr_size_t *len, chxjconvrule_entry *entryp)
{
  apr_size_t          ii;
  apr_size_t          ilen;
  apr_pool_t          *pool;
  char                *obuf;
  apr_size_t          olen;

  DBG(r,"REQ[%X] start %s()", TO_ADDR(r),__func__);

  if (entryp->action & CONVRULE_Z2H_NUM_OFF_BIT) {
    DBG(r,"REQ[%X] Detect Z2hNumOff", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
    return (char *)src;
  }
  if (! (entryp->action & CONVRULE_Z2H_NUM_ON_BIT)) {
    DBG(r,"REQ[%X] Detect Z2hNumOff", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
    return (char *)src;
  }

  apr_pool_create(&pool, r->pool);

  olen = 0;
  ilen = *len;

  obuf = apr_palloc(pool, ilen + 1);
  if (! obuf) {
    ERR(r,"REQ[%X] %s:%d memory allocation error", TO_ADDR(r),__FILE__,__LINE__);
    DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
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
      if (   firstbyte == 0x82
          && (secondbyte >= 0x4F && secondbyte <= 0x58)) {
        unsigned char p = secondbyte - 0x4F;
        /* Detect Zenkaku Number */
        obuf[olen] = '0' + p;
        olen++;
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

  DBG(r,"REQ[%X] end %s()", TO_ADDR(r),__func__);
  return obuf;
}
/*
 * vim: ts=2 et
 */
