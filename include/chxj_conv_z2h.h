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
#ifndef __CHXJ_CONV_Z2H_H__
#define __CHXJ_CONV_Z2H_H__

#include "mod_chxj.h"

typedef struct {
  apr_size_t byte;
  char *hankaku;
} z2h_table_t;

extern char *chxj_conv_z2h(request_rec *r, const char *src, apr_size_t *len, chxjconvrule_entry *entryp);

#endif
