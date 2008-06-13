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
#include <libcroco/libcroco.h>


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
  enum Combinator combinator;
  css_property_t property_head;
} css_selector_t;


/**
 * CSS stylesheet.
 * Manager of css_selector_t.
 */
typedef struct __css_stylesheet_t {
  css_selector_t selector_head;
} css_stylesheet_t;


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
#endif
/*
 * vim:ts=2 et
 */
