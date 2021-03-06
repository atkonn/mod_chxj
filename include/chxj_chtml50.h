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
#ifndef __CHXJ_CHTML50_H__
#define __CHXJ_CHTML50_H__

/* #define DUMP_CHTML50 "" */

#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_css.h"

/*----------------------------------------------------------------------------*/
/* Structure for CHTML4.0                                                     */
/*----------------------------------------------------------------------------*/
typedef struct chtml50_t chtml50_t;

struct chtml50_t {
  Doc                 *doc;
  char                *out;
  int                 out_len;
  int                 pre_flag;
  int                 textarea_flag;
  int                 font_flag;

  device_table        *spec;
  mod_chxj_config     *conf;
  chxjconvrule_entry  *entryp;
  cookie_t            *cookie;
  css_stylesheet_t    *style;
  css_prop_list_stack_t *css_prop_stack;

  char                *pagetitle;
};

typedef struct _chtml50_flags_t chtml50_flags_t;
struct _chtml50_flags_t {
  int with_font_flag;
  int with_blink_flag;
  int with_div_flag;
  int with_marquee_flag;
};

/*----------------------------------------------------------------------------*/
/* Prototype                                                                  */
/*----------------------------------------------------------------------------*/
extern char *chxj_convert_chtml50(
  request_rec           *r,
  device_table          *spec,
  const char            *src,
  apr_size_t            srclen,
  apr_size_t            *dstlen,
  chxjconvrule_entry    *entryp,
  cookie_t              *cookie);


extern char *chxj_chtml50_emoji_only_converter(request_rec *r, device_table *spec, const char *src, apr_size_t len);

#endif
