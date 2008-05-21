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
#include "chxj_css.h"
#include "chxj_serf.h"
#include "chxj_encoding.h"
#include "qs_parse_string.h"
#include "apr_pools.h"


/*===========================================================================*/
/* PARSER                                                                    */
/*===========================================================================*/
static char *s_css_parser_get_charset(apr_pool_t *pool, const char *src, apr_size_t *next_pos);
static void s_css_parser_from_uri_start_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_end_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_property(CRDocHandler *a_this, CRString *a_name, CRTerm *a_expression, gboolean a_is_important);
static css_property_t *s_css_parser_copy_property(apr_pool_t *pool, css_property_t *from);
static css_selector_t *s_new_selector(apr_pool_t *pool, css_stylesheet_t *stylesheet, char *name);
static css_selector_t *s_search_selector(css_stylesheet_t *stylesheet, const char *name);
static void s_merge_property(css_selector_t *sel, css_property_t *tgt);

struct css_app_data {
  css_stylesheet_t *stylesheet;
  char **selector_list;
  int selector_count;
  apr_pool_t *pool;
  request_rec *r;
  int error_occured;
  css_property_t property_head;
};


css_stylesheet_t *
chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *uri)
{
  CRParser     *parser      = NULL;
  CRDocHandler *sac_handler = NULL;
  enum CRStatus ret;
  char         *css         = NULL;
  char         *charset     = NULL;
  apr_size_t  srclen;
  apr_size_t  next_pos;
  css_stylesheet_t *stylesheet = NULL;
  struct css_app_data app_data;
  

  DBG(r, "start chxj_css_parse_from_uri()");

  css = chxj_serf_get(r, pool, uri);
  if (css == NULL) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): serf_get failed: url:[%s]", APLOG_MARK, uri);
    return NULL;
  }
  srclen = strlen(css);
  

  charset = s_css_parser_get_charset(pool, css, &next_pos);
  if (charset) {
    DBG(r, "charset:[%s]\n", charset);
    css += next_pos;
    srclen = strlen(css);
    css = chxj_iconv(r, pool, css, &srclen, charset, "UTF-8");
  }

  parser = cr_parser_new_from_buf((guchar *)css, srclen, CR_UTF_8, FALSE);
  if (!parser) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_parser_new_from_buf() failed", APLOG_MARK);
    return NULL;
  }
  sac_handler = cr_doc_handler_new();
  if (!sac_handler) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_doc_handler_new() failed", APLOG_MARK);
    cr_parser_destroy(parser);
    return NULL;
  }

  stylesheet = apr_palloc(pool, sizeof(*stylesheet));
  memset(stylesheet, 0, sizeof(*stylesheet));
  stylesheet->selector_head.next = &stylesheet->selector_head;
  stylesheet->selector_head.ref  = &stylesheet->selector_head.next;

  memset(&app_data, 0, sizeof(struct css_app_data));
  app_data.stylesheet = stylesheet;
  app_data.selector_list = NULL;
  app_data.selector_count = 0;
  app_data.pool = pool;
  app_data.error_occured = 0;
  app_data.r = r;

  sac_handler->app_data = &app_data;

  sac_handler->start_selector = s_css_parser_from_uri_start_selector;
  sac_handler->end_selector   = s_css_parser_from_uri_end_selector;
  sac_handler->property       = s_css_parser_from_uri_property;

  ret = cr_parser_set_sac_handler(parser, sac_handler);
  if (ret != CR_OK) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_parser_set_sac_handler() failed: ret:[%d]", APLOG_MARK, ret);
    cr_parser_destroy(parser);
    return NULL;
  }

  ret = cr_parser_parse(parser);
  cr_parser_destroy(parser);
  DBG(r, "end chxj_css_parse_from_uri()");
  return app_data.stylesheet;
}


#define ERROR_OCCORED do {          \
    if (app_data->error_occured) {  \
      return;                       \
    }                               \
  }                                 \
  while (0)


#define CB_INIT \
  struct css_app_data *app_data = (struct css_app_data *)a_this->app_data

#undef list_insert
#undef list_remove
#define list_insert(node, point) do {           \
    node->ref = point->ref;                     \
    *node->ref = node;                          \
    node->next = point;                         \
    point->ref = &node->next;                   \
} while (0)

#define list_remove(node) do {                  \
    *node->ref = node->next;                    \
    node->next->ref = node->ref;                \
} while (0)


static void 
s_css_parser_from_uri_start_selector(CRDocHandler * a_this, CRSelector *a_selector_list)
{
  int ii;
  CRSelector *cur = NULL;
  CB_INIT;
  ERROR_OCCORED;

  app_data->selector_count = 0;
  for (cur = a_selector_list; cur; cur = cur->next)
    app_data->selector_count++;

  app_data->selector_list = apr_palloc(app_data->pool, sizeof(char *) * app_data->selector_count);
  if (! app_data->selector_list) {
    ERR(app_data->r, "%s:%d Out of memory", APLOG_MARK);
    app_data->error_occured = 1;
    return;
  }

  ii = 0;
  for (cur = a_selector_list; cur; cur = cur->next) {
    if (cur->simple_sel) {
      guchar *tmp_str = cr_simple_sel_to_string(cur->simple_sel);
      if (tmp_str) {
        app_data->selector_list[ii++] = apr_pstrdup(app_data->pool, (char *)tmp_str);
        g_free (tmp_str);
        tmp_str = NULL;
      }
    }
  }
  app_data->property_head.next = &app_data->property_head;
  app_data->property_head.ref  = &app_data->property_head.next;
}


static void 
s_css_parser_from_uri_end_selector(CRDocHandler * a_this, CRSelector *a_selector_list)
{
  int ii;
  css_property_t *cur = NULL;
  CB_INIT;
  ERROR_OCCORED;

  if (app_data->property_head.next)  {
    for (ii=0; ii<app_data->selector_count; ii++) {
      css_selector_t *sel = s_new_selector(app_data->pool, app_data->stylesheet, app_data->selector_list[ii]); 

      for (cur = app_data->property_head.next; cur && cur != &app_data->property_head; cur = cur->next) {
        css_property_t *tgt = s_css_parser_copy_property(app_data->pool, cur);
        css_property_t *pnt = &sel->property_head;
        s_merge_property(sel, tgt);
      }
      css_selector_t *point_selector = &app_data->stylesheet->selector_head;
      list_insert(sel, point_selector);
    }
  }
  app_data->property_head.next = &app_data->property_head;
  app_data->property_head.ref  = &app_data->property_head.next;
}

static void
s_merge_property(css_selector_t *sel, css_property_t *tgt)
{
  css_property_t *cur;
  css_property_t *pnt = &sel->property_head;
  char l = tolower(*tgt->name);
  char u = toupper(*tgt->name);
  for (cur = pnt->next; cur != pnt;cur = cur->next) {
    if ((l == *cur->name || u == *cur->name) && strcasecmp(cur->name, tgt->name) == 0) {
      cur->value = tgt->value;
      return;
    }
  }
  list_insert(tgt,  pnt);
}

static css_selector_t *
s_new_selector(apr_pool_t *pool, css_stylesheet_t *stylesheet, char *name)
{
  css_selector_t *sel = NULL;
  sel = s_search_selector(stylesheet, name);
  if (sel) {
    list_remove(sel);
    sel->next = sel;
    sel->ref = &sel->next;
  }
  else {
    sel = apr_palloc(pool, sizeof(css_selector_t));
    memset(sel, 0, sizeof(css_selector_t));
    sel->name = name;
    sel->next = sel;
    sel->ref = &sel->next;
    sel->property_head.next = &sel->property_head;
    sel->property_head.ref = &sel->property_head.next;
  }
  return sel;
}

static css_selector_t *
s_search_selector(css_stylesheet_t *stylesheet, const char *name)
{
  css_selector_t *cur;
  char l = tolower(*name);
  char u = toupper(*name);
  for (cur = stylesheet->selector_head.next; cur != &stylesheet->selector_head; cur = cur->next) {
    if ((l == *cur->name || u == *cur->name) && strcasecmp(cur->name, name) == 0) {
      return cur;
    }
  }
  return NULL;
}

static css_property_t *
s_css_parser_copy_property(apr_pool_t *pool, css_property_t *from)
{
  css_property_t *prop = apr_palloc(pool, sizeof(css_property_t));
  prop->name  = apr_pstrdup(pool, from->name);
  prop->value = apr_pstrdup(pool, from->value);
  prop->next  = prop;
  prop->ref   = &prop->next;
  return prop;
}




static void
s_css_parser_from_uri_property(CRDocHandler *a_this, CRString *a_name, CRTerm *a_expression, gboolean a_is_important)
{
  CB_INIT;
  ERROR_OCCORED;
  css_property_t *property;

  if (a_name && a_expression) {
    guchar *tmp_str;
    property = apr_palloc(app_data->pool, sizeof(*property));
    memset(property, 0, sizeof(*property));
    property->name = apr_pstrdup(app_data->pool, cr_string_peek_raw_str(a_name));
    tmp_str = cr_term_one_to_string(a_expression);
    property->value = apr_pstrdup(app_data->pool, (char *)tmp_str);
    g_free(tmp_str);
    tmp_str = NULL;

    css_property_t *point_property = &app_data->property_head;
    list_insert(property, point_property);
  }
}


static char *
s_css_parser_get_charset(apr_pool_t *pool, const char *src, apr_size_t *next_pos)
{
  register char *p = (char *)src;
  char *sv;
  char *ret = NULL;
   
  if (! p) {
    return NULL;
  }

  for (; *p && is_white_space(*p); p++)
    ;

#define CUT_TOKEN(X) \
        do { \
          sv = ++p; \
          for (;*p && (X); p++) \
            ; \
          ret = apr_palloc(pool, p - sv + 1); \
          memset(ret, 0, p - sv + 1); \
          memcpy(ret, sv, p - sv);  \
        } \
        while (0)

  if (*p == '@') {
    if (strncasecmp(p, "@charset", sizeof("@charset")-1) == 0) {
      p += sizeof("@charset");
      for (; *p && is_white_space(*p); p++)
        ;
      if (*p == '"') {
        CUT_TOKEN(*p != '"');
        if (! *p) return NULL;
      }
      else if (*p == '\'') {
        CUT_TOKEN(*p != '\'');
        if (! *p) return NULL;
      }
      else {
        CUT_TOKEN(! is_white_space(*p));
        if (! *p) return NULL;
      }
    }
  }
  if (ret) {
    *next_pos = p - src + 1;
  }
  else {
    *next_pos = 0;
  }
#undef CUT_TOKEN
  return ret;
}


#if 0
css_stylesheet_t *
chxj_css_parse_from_style_tag(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *style_value)
{
}


css_stylesheet_t *
chxj_css_parse_from_style_attribute(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *style_attribute_value)
{

/*===========================================================================*/
/* find selector                                                             */
/*===========================================================================*/
css_selector_t *
chxj_css_find_selector(apr_pool_t *pool, css_stylesheet_t *stylesheet, const char *tag_name, const char *class_name, const char *id)
{
}

/*===========================================================================*/
/* push/pop current_stylesheet_stack                                         */
/*===========================================================================*/
#endif

/*
 * vim:ts=2 et
 */
