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
#ifndef __CHXJ_JRESERVED_TAG_H__
#define __CHXJ_JRESERVED_TAG_H__

#define CHXJ_SOFTBANK_RESERVED_TAG_PREFIX "_chxj_sf_"

extern char *chxj_jreserved_to_safe_tag(request_rec *r, const char *src, chxjconvrule_entry *entryp);
extern char *chxj_safe_to_jreserved_tag(request_rec *r, const char *src);

extern char *chxj_jreserved_tag_to_safe_for_query_string(request_rec *r, const char *query_string, chxjconvrule_entry *entryp, int xmlflag);

extern int chxj_is_jreserved_tag(const char *src);
#endif
