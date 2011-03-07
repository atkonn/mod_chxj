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
#include "chxj_encoding.h"
#include "chxj_apply_convrule.h"
#include "chxj_url_encode.h"
#include <errno.h>
#include <iconv.h>


char *
chxj_encoding(request_rec *r, const char *src, apr_size_t *len)
{
  char                *obuf;
  char                *ibuf;
  char                *spos;
  
  iconv_t             cd;
  size_t              result;
  apr_size_t          ilen;
  apr_size_t          olen;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;
  apr_pool_t          *pool;


  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (dconf == NULL) {
    DBG(r,"REQ[%X] none encoding.",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }
  if ((int)*len < 0) {
    ERR(r, "REQ[%X] runtime exception: chxj_encoding(): invalid string size.[%d]", TO_ADDR(r),(int)*len);
    DBG(r, "REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char *)apr_pstrdup(r->pool, "");
  }

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (entryp->encoding == NULL) {
    DBG(r,"REQ[%X] none encoding.",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char *)src;
  }

  if (STRCASEEQ('n','N',"none", entryp->encoding)) {
    DBG(r,"REQ[%X] none encoding.",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }

  apr_pool_create(&pool, r->pool);
  ilen = *len;
  ibuf = apr_palloc(pool, ilen+1);
  if (ibuf == NULL) {
    ERR(r, "runtime exception: chxj_encoding(): Out of memory.");
    return (char *)src;
  }
  memset(ibuf, 0, ilen+1);
  memcpy(ibuf, src, ilen);

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(pool, olen);
  if (obuf == NULL) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return ibuf;
  }
  DBG(r,"REQ[%X] encode convert [%s] -> [%s]", TO_ADDR(r),entryp->encoding, "CP932");

  memset(obuf, 0, olen);
  cd = iconv_open("CP932", entryp->encoding);
  if (cd == (iconv_t)-1) {
    if (EINVAL == errno) {
      ERR(r, "REQ[%X] The conversion from %s to %s is not supported by the implementation.", TO_ADDR(r),entryp->encoding, "CP932");
    }
    else {
      ERR(r, "REQ[%X] iconv open failed. from:[%s] to:[%s] errno:[%d]", TO_ADDR(r),entryp->encoding, "CP932", errno);
    }
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return ibuf;
  }
  while (ilen > 0) {
    result = iconv(cd, &ibuf, &ilen, &obuf, &olen);
    if (result == (size_t)(-1)) {
      if (E2BIG == errno) {
        ERR(r, "REQ[%X] There is not sufficient room at *outbuf.",TO_ADDR(r));
        break;
      }
      else if (EILSEQ == errno) {
        ERR(r, "REQ[%X] %s:%d An invalid multibyte sequence has been encountered in the input. input:[%s]", TO_ADDR(r),__FILE__,__LINE__,ibuf);
        chxj_convert_illegal_charactor_sequence(r, entryp, &ibuf, &ilen, &obuf, &olen);
      }
      else if (EINVAL == errno) {
        ERR(r, "REQ[%X] An incomplete multibyte sequence has been encountered in the input. input:[%s]", TO_ADDR(r),ibuf);
        break;
      }
    }
  }
  *len = strlen(spos);
  iconv_close(cd);

  DBG(r,"REQ[%X] len=[%d] obuf=[%.*s]", TO_ADDR(r),(int)*len, (int)*len, spos);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return spos;
}


void
chxj_convert_illegal_charactor_sequence(request_rec *r, chxjconvrule_entry  *entryp, char **ibuf, apr_size_t *ilen, char **obuf, apr_size_t *olen)
{
  if (STRCASEEQ('u','U',"UTF-8", entryp->encoding) || STRCASEEQ('u','U',"UTF8", entryp->encoding)) {
    if ((0xe0 & **ibuf) == 0xc0) {
      /* 2byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 2;
      DBG(r, "REQ[%X] passed 2byte.",TO_ADDR(r));
    }
    else if ((0xf0 & **ibuf) == 0xe0) {
      /* 3byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf +=3;
      DBG(r, "REQ[%X] passed 3byte.",TO_ADDR(r));
    }
    else if ((0xf8 & **ibuf) == 0xf0) {
      /* 4byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf +=4;
      DBG(r, "REQ[%X] passed 4byte.",TO_ADDR(r));
    }
    else if ((0xc0 & **ibuf) == 0x80) {
      /* 1byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 1;
      DBG(r, "REQ[%X] passed 1byte.",TO_ADDR(r));
    }
    else {
      /* unknown charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 1;
      DBG(r, "REQ[%X] passed 1byte.",TO_ADDR(r));
    }
  }
  else if (STRCASEEQ('e','E', "EUCJP",               entryp->encoding)
      ||   STRCASEEQ('c','C', "CSEUCPKDFMTJAPANESE", entryp->encoding)
      ||   STRCASEEQ('e','E', "EUC-JISX0213",        entryp->encoding)
      ||   STRCASEEQ('e','E', "EUC-JP-MS",           entryp->encoding)
      ||   STRCASEEQ('e','E', "EUC-JP",              entryp->encoding)
      ||   STRCASEEQ('e','E', "EUCJP-MS",            entryp->encoding)
      ||   STRCASEEQ('e','E', "EUCJP-OPEN",          entryp->encoding)
      ||   STRCASEEQ('e','E', "EUCJP-WIN",           entryp->encoding)
      ||   STRCASEEQ('e','E', "EUCJP",               entryp->encoding)) {
    if ((unsigned char)**ibuf == 0x8F) {
      /* 3byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf +=3;
      DBG(r, "REQ[%X] passed 3byte.",TO_ADDR(r));
    }
    else {
      /* 2byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 2;
      DBG(r, "REQ[%X] passed 2byte.",TO_ADDR(r));
    }
  }
  else if (STRCASEEQ('c', 'C', "CP932",     entryp->encoding)
      ||   STRCASEEQ('c', 'C', "CSIBM932",  entryp->encoding)
      ||   STRCASEEQ('i', 'I', "IBM-932",   entryp->encoding)
      ||   STRCASEEQ('i', 'I', "IBM932",    entryp->encoding)
      ||   STRCASEEQ('m', 'M', "MS932",     entryp->encoding)
      ||   STRCASEEQ('m', 'M', "MS_KANJI",  entryp->encoding)
      ||   STRCASEEQ('s', 'S', "SJIS-OPEN", entryp->encoding)
      ||   STRCASEEQ('s', 'S', "SJIS-WIN",  entryp->encoding)
      ||   STRCASEEQ('s', 'S', "SJIS",      entryp->encoding)) {
    if ( ( ((0x81 <= (unsigned char)**ibuf) && (0x9f >= (unsigned char)**ibuf))
        || ((0xe0 <= (unsigned char)**ibuf) && (0xfc >= (unsigned char)**ibuf)))
       &&
       (  ((0x40 <= (unsigned char)*((*ibuf)+1)) && (0x7e >= (unsigned char)*((*ibuf)+1)))
        ||((0x80 <= (unsigned char)*((*ibuf)+1)) && (0xfc >= (unsigned char)*((*ibuf)+1))))) {
      /* 2byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 2;
      DBG(r, "REQ[%X] passed 2byte.", TO_ADDR(r));
    }
    else {
      /* 1byte charactor */
      **obuf = '?';
      *obuf += 1;
      *olen -= 1;
      *ibuf += 1;
      DBG(r, "REQ[%X] passed 1byte.",TO_ADDR(r));
    }
  }
  else {
    /* unknown 1byte charactor */
    **obuf = '?';
    *obuf += 1;
    *olen -= 1;
    *ibuf += 1;
    DBG(r, "REQ[%X] passed 1byte.", TO_ADDR(r));
  }
  if (ibuf && *ibuf) {
    *ilen = strlen(*ibuf);
    DBG(r, "REQ[%X] new len = [%" APR_SIZE_T_FMT "].", TO_ADDR(r),(apr_size_t)*ilen);
  }
}


char *
chxj_rencoding(request_rec *r, const char *src, apr_size_t *len)
{
  char                *obuf;
  char                *ibuf;
  char                *spos;
  
  iconv_t             cd;
  size_t              result;
  apr_size_t          ilen;
  apr_size_t          olen;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  if ((int)*len < 0) {
    ERR(r, "REQ[%X] runtime exception: chxj_rencoding(): invalid string size.[%d]", TO_ADDR(r),(int)*len);
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char *)apr_pstrdup(r->pool, "");
  }

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (! dconf) {
    DBG(r,"REQ[%X] none encoding.",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp->encoding) {
    DBG(r,"REQ[%X] none encoding.",TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }

  if (STRCASEEQ('n','N',"none", entryp->encoding)) {
    DBG(r,"REQ[%X] none encoding.", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }

  ilen = *len;
  ibuf = apr_palloc(r->pool, ilen+1);
  if (! ibuf) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return (char*)src;
  }

  memset(ibuf, 0,   ilen+1);
  memcpy(ibuf, src, ilen+0);

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(r->pool, olen);
  if (! obuf) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return ibuf;
  }
  DBG(r,"REQ[%X] encode convert [%s] -> [%s]", TO_ADDR(r),"CP932", entryp->encoding);

  memset(obuf, 0, olen);

  cd = iconv_open(entryp->encoding, "CP932");
  if (cd == (iconv_t)-1) {
    if (EINVAL == errno) {
      ERR(r, "REQ[%X] The conversion from %s to %s is not supported by the implementation.", TO_ADDR(r),"CP932", entryp->encoding);
    }
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return ibuf;
  }

  while (ilen > 0) {
    result = iconv(cd, &ibuf, &ilen, &obuf, &olen);
    if (result == (size_t)(-1)) {
      if (E2BIG == errno) {
        ERR(r, "REQ[%X] There is not sufficient room at *outbuf",TO_ADDR(r));
        break;
      }
      else if (EILSEQ == errno) {
        ERR(r, "REQ[%X] An invalid multibyte sequence has been encountered in the input. input:[%s]", TO_ADDR(r),ibuf);
        chxj_convert_illegal_charactor_sequence(r, entryp, &ibuf, &ilen, &obuf, &olen);
      }
      else if (EINVAL == errno) {
        ERR(r, "REQ[%X] An incomplete multibyte sequence has been encountered in the input. input:[%s]", TO_ADDR(r),ibuf);
        break;
      }
    }
  }

  *len = strlen(spos);
  iconv_close(cd);

  DBG(r,"REQ[%X] len=[%d] obuf=[%.*s]", TO_ADDR(r),(int)*len, (int)*len, spos);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return spos;
}


char *
chxj_encoding_parameter(request_rec *r, const char *value)
{
  char *src;
  char *src_sv;
  char *pstat;
  char *spos;
  char *pair;
  char *key;
  char *val;
  char *vstat;
  char *param;
  char *anchor_pos;
  char *anchor = NULL;

  int   use_amp_flag;
  
  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);

  src = apr_pstrdup(r->pool, value);

  anchor_pos = strchr(src, '#');
  if (anchor_pos) {
    anchor_pos++;
    anchor = apr_pstrdup(r->pool, anchor_pos);
    anchor_pos--;
    *anchor_pos = 0;
  }

  spos = strchr(src, '?');
  if (!spos) {
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    if (anchor_pos) {
      return apr_pstrcat(r->pool, src, "#", anchor, NULL);
    } else {
      return src;
    }
  }
  *spos++ = 0;

  src_sv = apr_pstrdup(r->pool, src);
  param  = apr_palloc(r->pool, 1);
  param[0] = 0;

  for (;;) {
    apr_size_t len;
    char *sep_pos;

    use_amp_flag = 0;

    pair = apr_strtok(spos, "&", &pstat);
    spos = NULL;
    if (!pair) break;
    if (strncasecmp(pair, "amp;", 4) == 0) {
      pair += 4;
      use_amp_flag = 1;
    }
    sep_pos = strchr(pair, '=');
    if (pair == sep_pos) {
      key = apr_pstrdup(r->pool, "");
    }
    else {
      key = apr_strtok(pair, "=", &vstat);
      pair = NULL;
    }
    if (key) {
      apr_size_t klen = (apr_size_t)strlen(key);
      key = chxj_url_decode(r->pool, key);
      len = (apr_size_t)strlen(key);
      if (klen != len) {
        key = chxj_encoding(r, key, &len);
        key = chxj_url_encode(r->pool, key);
      }
#if 0  /* XXX:2009/4/10 */
      key = chxj_url_encode(r->pool, key);
#endif
    }
    val = apr_strtok(pair, "=", &vstat);
    if (! val && sep_pos) {
      val = apr_pstrdup(r->pool, "");
    }
    if (val) {
      apr_size_t vlen = (apr_size_t)strlen(val);
      val = chxj_url_decode(r->pool, val);
      len = (apr_size_t)strlen(val);
      if (vlen != len) {
        val = chxj_encoding(r, val, &len);
        val = chxj_url_encode(r->pool, val);
      }
#if 0  /* XXX:2009/4/10 */
      val = chxj_url_encode(r->pool, val);
#endif
      if (strlen(param) == 0) {
        param = apr_pstrcat(r->pool, param, key, "=", val, NULL);
      }
      else {
        if (use_amp_flag) {
          param = apr_pstrcat(r->pool, param, "&amp;", key, "=", val, NULL);
        }
        else {
          param = apr_pstrcat(r->pool, param, "&", key, "=", val, NULL);
        }
      }
    }
    else {
      if (strlen(param) == 0) {
        param = apr_pstrcat(r->pool, param, key,  NULL);
      }
      else {
        if (use_amp_flag) {
          param = apr_pstrcat(r->pool, param, "&amp;", key, NULL);
        }
        else {
          param = apr_pstrcat(r->pool, param, "&", key, NULL);
        }
      }
    }
  }
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);

  if (anchor_pos) {
    return apr_pstrcat(r->pool, src_sv, "?", param, "#", anchor, NULL);
  } else {
    return apr_pstrcat(r->pool, src_sv, "?", param, NULL);
  }
}
/*
 * vim:ts=2 et
 */
