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
#ifndef __CHXJ_CSS_H__
#define __CHXJ_CSS_H__

#include "mod_chxj.h"


/**
 * CSS property.
 */
typedef struct __css_property_t {
  struct __css_property_t *next;
  struct __css_property_t **ref;
  char *name;
  char *value;
} css_property_t;


/**
 * CSS selector.
 */
typedef struct __css_selector_t {
  struct __css_selector_t *next;
  struct __css_selector_t **ref;
  /* has tag or/and class or/and id */
  char *name;
  css_property_t property_head;
} css_selector_t;

typedef struct __css_prop_list_t {
  struct __css_prop_list_t *next;
  struct __css_prop_list_t **ref;
  css_property_t head;
} css_prop_list_t;

typedef struct __css_prop_list_stack_t {
  css_prop_list_t head;
} css_prop_list_stack_t;

/**
 * CSS stylesheet.
 * Manager of css_selector_t.
 */
typedef struct __css_stylesheet_t {
  css_selector_t selector_head;
} css_stylesheet_t;

#define STYLESHEETS_SELECTOR_HEAD(X)  (X)->selector_head
#define STYLESHEETS_FIRST_SELECTOR(X) STYLESHEETS_SELECTOR_HEAD((X))##.next
#define STYLESHEETS_LAST_SELECTOR(X)  ((apr_size_t)(STYLESHEET_SELECTOR_HEAD((X))##.ref) - (apr_size_t)APR_OFFSETOF(css_selector_t, next))
#define STYLESHEETS_SENTINEL(X)       &(STYLESHEETS_SELECTOR_HEAD((X)))
  

/**
 * CSS current_stylesheet.
 */
typedef struct __css_current_stylesheet_t {
  struct __css_property_t property_head;
  struct __css_current_stylesheet_t *next;
  struct __css_current_stylesheet_t **ref;
} css_current_stylesheet_t;


/**
 * CSS current_stylesheet_stack_t.
 */
typedef struct __css_current_stylesheet_stack_t {
  css_current_stylesheet_t *stylesheet_head;
  css_current_stylesheet_t *stylesheet_tail;
} css_current_stylesheet_stack_t;

#include "chxj_apache.h"
extern css_stylesheet_t *chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *uri);
extern void chxj_css_stylesheet_dump(css_stylesheet_t *stylesheet);
extern css_selector_t *chxj_css_find_selector(Doc *doc, css_stylesheet_t *stylesheet, Node *node);
extern css_stylesheet_t *chxj_find_pseudo_selectors(Doc *doc, css_stylesheet_t *stylesheet);
extern css_stylesheet_t *chxj_css_parse_style_attr(Doc *doc, css_stylesheet_t *old_stylesheet, char *tag_name, char *class_name, char *id_name, char *style_attr_value);
extern css_stylesheet_t *chxj_css_parse_style_value(Doc *doc, css_stylesheet_t *old_stylesheet, char *style_attr_value);

extern css_prop_list_stack_t *chxj_new_prop_list_stack(Doc *doc);
extern css_prop_list_t *chxj_css_create_prop_list(Doc *doc, css_selector_t *sel);
extern void chxj_css_push_prop_list(css_prop_list_stack_t *stack, css_prop_list_t *prop_list);
extern css_prop_list_t *chxj_css_pop_prop_list(css_prop_list_stack_t *stack);
extern css_prop_list_t *chxj_dup_css_prop_list(Doc *doc, css_prop_list_t *old);
extern void chxj_css_prop_list_merge_property(Doc *doc, css_prop_list_t *base, css_selector_t *sel);
extern css_prop_list_t *chxj_css_get_last_prop_list(css_prop_list_stack_t *stack);
extern char *chxj_css_get_property_value(Doc *doc, css_prop_list_t *list, const char *name);
#endif
/*
 * vim:ts=2 et
 */
