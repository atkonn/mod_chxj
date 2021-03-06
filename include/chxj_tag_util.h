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
#ifndef __CHXJ_TAG_UTIL_H__
#define __CHXJ_TAG_UTIL_H__

#include "mod_chxj.h"

/*----------------------------------------------------------------------------*/
/* prototype declare                                                          */
/*----------------------------------------------------------------------------*/
extern char *qs_alloc_zero_byte_string(apr_pool_t *pool);
extern char *qs_trim_string(apr_pool_t *p, char *s);
extern char *qs_get_value_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_checked_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_type_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_selected_value_text(Doc *doc, Node *node, apr_pool_t *pool);
extern char *qs_get_selected_value(Doc *doc, Node *node, apr_pool_t *pool);
extern char *qs_get_name_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_size_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_accesskey_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_istyle_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_maxlength_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern int   qs_is_checked_checkbox_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern int   chxj_chxjif_is_mine(device_table *spec, Doc *doc, Node *tag);
extern char *qs_get_destlang_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *qs_get_parse_attr(Doc *doc, Node *tag, apr_pool_t *pool);
extern char *chxj_form_action_to_hidden_tag(
  request_rec *r, 
  apr_pool_t *pool,
  const char *str,
  int xmlFlag,
  int post, 
  char **new_query_string, 
  int docomo,
  int softbank,
  chxjconvrule_entry *entryp
  );
extern char *chxj_delete_chxj_cc_param(request_rec *r, const char *str);

#endif
/*
 * vim:ts=2 et
 */
