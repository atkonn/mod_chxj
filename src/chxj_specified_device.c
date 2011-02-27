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
#include "chxj_add_device_env.h"

static device_table  UNKNOWN_DEVICE      = {
  .next = NULL,
  .device_id = "",
  .device_name = "UNKNOWN",
  .html_spec_type = CHXJ_SPEC_UNKNOWN,
  .width = 640,
  .heigh = 480,
  /*--------------------------------------------------------------------------*/
  /* Walll Paper Size                                                         */
  /*--------------------------------------------------------------------------*/
  .wp_width = 640,
  .wp_heigh = 480,
  /*--------------------------------------------------------------------------*/
  /* Cache Size                                                               */
  /*--------------------------------------------------------------------------*/
  .cache    = 10000000,
  /*--------------------------------------------------------------------------*/
  /* Correspondence image format                                              */
  /* 1: It is possible to display it.                                         */
  /* 0: It is not possible to display it.                                     */
  /*--------------------------------------------------------------------------*/
  .available_gif = 1,
  .available_jpeg = 1,
  .available_png = 1,
  .available_bmp2 = 1,
  .available_bmp4 = 1,
  .dpi_width = 96,
  .dpi_heigh = 96,
  /*--------------------------------------------------------------------------*/
  /* Color number type                                                        */
  /* 2       : 2        Colors                                                */
  /* 4       : 4        Colors                                                */
  /* 256     : 256      Colors                                                */
  /* 4096    : 4096     Colors                                                */
  /* 65536   : 65536    Colors                                                */
  /* 262144  : 262144   Colors                                                */
  /* 15680000: 15680000 over colors                                           */
  /*--------------------------------------------------------------------------*/
  .color = 15680000,
  .emoji_type = NULL,
};
static __thread device_table *v_spec = NULL;
static device_table * s_get_device_data(request_rec *r, const char *device_id, device_table_list *dtl);

/**
 * The device is specified from UserAgent. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
device_table *
chxj_specified_device(request_rec *r, const char *user_agent) 
{
  ap_regmatch_t        match[10];
  device_table         *returnType = &UNKNOWN_DEVICE;
  device_table_list    *dtl;
  mod_chxj_config      *conf; 
  char                 *device_id;
  char                 *spec_check = NULL;

  if (! user_agent) 
    return returnType;

  DBG(r, "REQ[%x] start chxj_specified_device()", (unsigned int)(apr_size_t)r);


  spec_check = (char *)apr_table_get(r->headers_in, "X-Chxj-Spec-Check");
  if (spec_check && STRCASEEQ('d','D',"done",spec_check)) {
#if 0
    returnType = chxj_get_device_env(r);
#else 
    returnType = v_spec;
#endif
    DBG(r, "REQ[%x] end chxj_specified_device() (Spec-Check-Done)", (unsigned int)(apr_size_t)r);
    return returnType;
  }


  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (! conf->devices) {
    DBG(r, "device_data.xml load failure");
    return returnType;
  }

  for (dtl = conf->devices; dtl; dtl = dtl->next) {
    if (! dtl->pattern) {
      DBG(r, "pattern is null");
      continue;
    }

    /* DBG(r, "pattern is [%s]", dtl->pattern); */
    if (! dtl->regexp) {
      DBG(r,"compile failed.");
      return returnType;
    }

    if (ap_regexec((const ap_regex_t *)dtl->regexp, user_agent, (apr_size_t)dtl->regexp->re_nsub + 1, match, 0) == 0) {
      device_id = ap_pregsub(r->pool, "$1", user_agent, dtl->regexp->re_nsub + 1, match);
      DBG(r, "device_id:[%s]", device_id);
      returnType = s_get_device_data(r, device_id, dtl);
      DBG(r, "device_id:[%s] is NOT FOUND", device_id);

      if (! returnType) {
        returnType = dtl->tail;
      }
    }
    v_spec = returnType;

    if (returnType && returnType != &UNKNOWN_DEVICE) {
      apr_table_setn(r->headers_in, "X-Chxj-Spec-Check", "done");
      DBG(r,"end chxj_specified_device()");
      return returnType;
    }
  }

  apr_table_setn(r->headers_in, "X-Chxj-Spec-Check", "done");
  DBG(r,"end chxj_specified_device()");

  v_spec = &UNKNOWN_DEVICE;
  return &UNKNOWN_DEVICE;
}


#include <stdlib.h>
static int 
s_compar(const void *a, const void *b)
{
  device_table *aa = *(device_table **)a;
  device_table *bb = *(device_table **)b;
{FILE *fp=fopen("/tmp/erer.log","a");fprintf(fp, "aa[%s] vs bb[%s]\n", aa->device_id, bb->device_id); fclose(fp);}
  return strcasecmp(aa->device_id, bb->device_id);
}
static device_table *
s_get_device_data(request_rec *r, const char *device_id, device_table_list *dtl)
{
#if 0
  device_table *dt;
  for (dt = dtl->table; dt; dt = dt->next) {
    if (strcasecmp(device_id, dt->device_id) == 0) {
      DBG(r, "device_name:[%s]", dt->device_name);
      return dt;
    }
  }
  return NULL;
#else
  device_table dt;
  device_table *_dt;
  dt.device_id = device_id;
  _dt = &dt;
  device_table **ret = bsearch(&_dt, dtl->sort_table, dtl->table_count, sizeof(device_table *), s_compar);
  if (ret && *ret) {
    return *ret;
  }
  return NULL;
#endif
}
/*
 * vim:ts=2 et
 */

