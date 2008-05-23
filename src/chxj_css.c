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

#include <libgen.h>

#undef list_insert
#undef list_remove
#define list_insert(node, point) do {           \
    node->ref  = point->ref;                    \
    *node->ref = node;                          \
    node->next = point;                         \
    point->ref = &node->next;                   \
} while (0)

#define list_remove(node) do {                  \
    *node->ref      = node->next;               \
    node->next->ref = node->ref;                \
} while (0)

/*===========================================================================*/
/* PARSER                                                                    */
/*===========================================================================*/
struct css_already_import_stack {
  char *full_url;
  struct css_already_import_stack *next;
  struct css_already_import_stack **ref;
};

struct css_app_data {
  css_stylesheet_t *stylesheet;
  char **selector_list;
  int selector_count;
  apr_pool_t *pool;
  request_rec *r;
  int error_occured;
  css_property_t property_head;
  struct css_already_import_stack imported_stack_head;
};
static char *s_css_parser_get_charset(apr_pool_t *pool, const char *src, apr_size_t *next_pos);
static void s_css_parser_from_uri_start_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_end_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_property(CRDocHandler *a_this, CRString *a_name, CRTerm *a_expression, gboolean a_is_important);
static css_property_t *s_css_parser_copy_property(apr_pool_t *pool, css_property_t *from);
static css_selector_t *s_new_selector(apr_pool_t *pool, css_stylesheet_t *stylesheet, char *name);
static css_selector_t *s_search_selector(css_stylesheet_t *stylesheet, const char *name);
static void s_merge_property(css_selector_t *sel, css_property_t *tgt);
static void s_css_parser_from_uri_import_style(CRDocHandler *a_this, GList *a_media_list, CRString *a_uri, CRString *a_uri_default_ns, CRParsingLocation *a_location);
static char *s_path_to_fullurl(apr_pool_t *pool, const char *base_url, const char *base_path, const char *uri);
static char *s_uri_to_base_url(apr_uri_t *uri, apr_pool_t *pool);
static css_stylesheet_t *s_chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, struct css_already_import_stack *imported_stack, css_stylesheet_t *old_stylesheet, const char *uri);
static int s_is_already_imported(struct css_already_import_stack *imported_stack_head, const char *url);
static css_stylesheet_t *s_merge_stylesheet(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, css_stylesheet_t *new_stylesheet);

css_stylesheet_t *
chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *uri)
{
  return s_chxj_css_parse_from_uri(r, pool, NULL, old_stylesheet, uri);
}

static css_stylesheet_t *
s_chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, struct css_already_import_stack *imported_stack, css_stylesheet_t *old_stylesheet, const char *uri)
{
  CRParser     *parser      = NULL;
  CRDocHandler *sac_handler = NULL;
  enum CRStatus ret;
  char         *css         = NULL;
  char         *charset     = NULL;
  char         *full_url    = NULL;
  apr_size_t  srclen;
  apr_size_t  next_pos;
  css_stylesheet_t *stylesheet = NULL;
  struct css_already_import_stack *new_stack;
  struct css_app_data app_data;
  char         *base_url;
  

  DBG(r, "start chxj_css_parse_from_uri()");

  base_url = s_uri_to_base_url(&r->parsed_uri, pool);
  full_url = s_path_to_fullurl(pool, base_url, r->parsed_uri.path, uri);

  /* check already import */
  if (imported_stack && s_is_already_imported(imported_stack, full_url)) {
    DBG(r, "end chxj_css_parse_from_uri(): already imported:[%s]", full_url); 
    return NULL;
  }

  /* GET request */
  css = chxj_serf_get(r, pool, full_url);
  if (css == NULL) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): serf_get failed: url:[%s]", APLOG_MARK, uri);
    return NULL;
  }
  srclen = strlen(css);
  
  /* convert encoding */
  charset = s_css_parser_get_charset(pool, css, &next_pos);
  if (charset) {
    DBG(r, "charset:[%s]\n", charset);
    css += next_pos;
    srclen = strlen(css);
    css = chxj_iconv(r, pool, css, &srclen, charset, "UTF-8");
  }

  /* create parser */
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
  app_data.stylesheet     = stylesheet;
  app_data.selector_list  = NULL;
  app_data.selector_count = 0;
  app_data.pool           = pool;
  app_data.error_occured  = 0;
  app_data.r              = r;
  if (imported_stack) {
    app_data.imported_stack_head.next = imported_stack->next;
    app_data.imported_stack_head.ref  = imported_stack->ref;
  }
  else {
    app_data.imported_stack_head.next = &app_data.imported_stack_head;
    app_data.imported_stack_head.ref  = &app_data.imported_stack_head.next;
  }
  new_stack = apr_palloc(pool, sizeof(*new_stack));
  memset(new_stack, 0, sizeof(*new_stack));
  new_stack->next = new_stack;
  new_stack->ref  = &new_stack->next;
  new_stack->full_url = full_url;
  list_insert(new_stack, app_data.imported_stack_head.next);
  

  sac_handler->app_data = &app_data;

  sac_handler->start_selector = s_css_parser_from_uri_start_selector;
  sac_handler->end_selector   = s_css_parser_from_uri_end_selector;
  sac_handler->property       = s_css_parser_from_uri_property;
  sac_handler->import_style   = s_css_parser_from_uri_import_style;

  ret = cr_parser_set_sac_handler(parser, sac_handler);
  if (ret != CR_OK) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_parser_set_sac_handler() failed: ret:[%d]", APLOG_MARK, ret);
    cr_parser_destroy(parser);
    return NULL;
  }

  ret = cr_parser_parse(parser);
  cr_parser_destroy(parser);
  DBG(r, "end chxj_css_parse_from_uri()");
  return s_merge_stylesheet(pool, old_stylesheet, app_data.stylesheet);
}


#define ERROR_OCCORED do {          \
    if (app_data->error_occured) {  \
      return;                       \
    }                               \
  }                                 \
  while (0)


#define CB_INIT \
  struct css_app_data *app_data = (struct css_app_data *)a_this->app_data



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


static void
s_css_parser_from_uri_import_style(CRDocHandler *a_this, GList *a_media_list, CRString *a_uri, CRString *a_uri_default_ns, CRParsingLocation *a_location) 
{
  CB_INIT;
  ERROR_OCCORED;
  guint ii = 0;
  guint len = g_list_length(a_media_list);
  int flag = 0;

  for (ii=0; ii<len; ii++) {
    char *str = cr_string_peek_raw_str(g_list_nth_data(a_media_list, ii));
    if (('h' == *str || 'H' == *str) && strcasecmp(str, "handheld") == 0) {
      flag = 1;
      break;
    }
    if (('a' == *str || 'A' == *str) && strcasecmp(str, "all") == 0) {
      flag = 1;
      break;
    }
  }
  if (flag || len == 0) {
    if (a_uri) {
      apr_uri_t uri;
      char      *new_url = NULL;
      char      *import_url = cr_string_peek_raw_str(a_uri);
      char      *base_url = NULL;

fprintf(stderr, "import_url:[%s]\n", import_url);
      base_url = s_uri_to_base_url(&app_data->r->parsed_uri, app_data->pool);
      new_url = s_path_to_fullurl(app_data->pool, base_url, app_data->r->parsed_uri.path, import_url);
      
fprintf(stderr, "base_url:[%s]\n", base_url);
fprintf(stderr, "new_url:[%s]\n", new_url);
      app_data->stylesheet = s_chxj_css_parse_from_uri(app_data->r, app_data->pool, &app_data->imported_stack_head, app_data->stylesheet, new_url);
    }
  }
}


static char *
s_path_to_fullurl(apr_pool_t *pool, const char *base_url, const char *base_path, const char *uri)
{
  char *new_url = NULL;
  if (chxj_starts_with(uri, "http")) {
    return uri;
  }

  if (*uri == '/') {
    return apr_pstrcat(pool, base_url, uri, NULL);
  }

  new_url = apr_pstrcat(pool, base_url, base_path, NULL);
  if (new_url[strlen(new_url)-1] == '/') {
    new_url = apr_pstrcat(pool, new_url, uri, NULL);
  }
  else {
    new_url = apr_pstrcat(pool, new_url, "/", uri, NULL);
  }
  return new_url;
}


static char *
s_uri_to_base_url(apr_uri_t *uri, apr_pool_t *pool)
{
  char *new_url = apr_psprintf(pool, "%s://%s", uri->scheme, uri->hostname);
  if (strcmp(uri->scheme, "http") == 0) {
    if (uri->port != 80 && uri->port != 0) {
      new_url = apr_pstrcat(pool, new_url, apr_psprintf(pool, ":%d", uri->port), NULL);
    }
  }
  else if (strcmp(uri->scheme, "https") == 0) {
    if (uri->port != 443 && uri->port != 0) {
      new_url = apr_pstrcat(pool, new_url, apr_psprintf(pool, ":%d", uri->port), NULL);
    }
  }
  return new_url;
}

static int
s_is_already_imported(struct css_already_import_stack *imported_stack_head, const char *url)
{
  struct css_already_import_stack *cur;
  char l = tolower(*url);
  char u = toupper(*url);
  for (cur = imported_stack_head->next; cur != imported_stack_head; cur = cur->next) {
    if ((l == cur->full_url || u == cur->full_url) && strcasecmp(url, cur->full_url) == 0) {
      return 1;
    }
  }
  return 0;
}

static css_stylesheet_t *
s_merge_stylesheet(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, css_stylesheet_t *new_stylesheet)
{
  if (! old_stylesheet) return new_stylesheet;
  /* このへんから */
  return old_stylesheet;
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
