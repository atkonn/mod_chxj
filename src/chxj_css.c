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
#include "scss.h"

#include <libgen.h>

#if defined(CHXJ_TEST)
#undef ap_regex_t
#undef ap_regmatch_t
#endif

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

/*===========================================================================*/
/*                                                                           */
/* PROTOTYPE                                                                 */
/*                                                                           */
/*===========================================================================*/
static void s_css_parser_from_uri_start_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
static void s_css_parser_from_uri_end_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
static void s_css_parser_from_uri_property(SCSSParserPtr_t parser, const char *propertyName, const char *value, int impotant);
static css_property_t *s_css_parser_copy_property(apr_pool_t *pool, css_property_t *from);
static css_selector_t *s_new_selector(apr_pool_t *pool, css_stylesheet_t *stylesheet, char *name);
static css_selector_t *s_search_selector(css_stylesheet_t *stylesheet, const char *name);
static void s_merge_property(css_selector_t *sel, css_property_t *tgt);
static void s_css_parser_from_uri_import_style(SCSSParserPtr_t parser, const char *uri, const char **media, const char *defaultNamespaceURI);
static char *s_path_to_fullurl(apr_pool_t *pool, const char *base_url, const char *base_path, const char *uri);
static char *s_uri_to_base_url(apr_uri_t *uri, apr_pool_t *pool);
static css_stylesheet_t *s_chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, struct css_already_import_stack *imported_stack, css_stylesheet_t *old_stylesheet, const char *uri);
static int s_is_already_imported(struct css_already_import_stack *imported_stack_head, const char *url);
static css_stylesheet_t *s_merge_stylesheet(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, css_stylesheet_t *new_stylesheet);
static void s_copy_already_import_stack(apr_pool_t *pool, struct css_already_import_stack *base, struct css_already_import_stack *imported_stack);
static css_selector_t *s_search_selector_regexp(Doc *doc, request_rec *r, apr_pool_t *pool, css_stylesheet_t *stylesheet, const char *pattern_str1, const char *pattern_str2, Node *node);
static void s_get_tag_and_class_and_id(Doc *doc, Node *node, char **tag_name, char **class_name, char **id);
static char *s_cmp_now_node_vs_current_style(Doc *doc, request_rec *r, apr_pool_t *pool, char *src, ap_regex_t *pattern4, Node *node);
static css_stylesheet_t *s_dup_stylesheet(Doc *doc, css_stylesheet_t *org);


/**
 * Data is acquired from url specified by using libserf. 
 * The acquired data is analyzed in the syntax and css_stylesheet_t is made. 
 *
 * @param r request_rec
 * @param pool pool
 * @param old_stylesheet old stylesheet
 * @param uri url
 *
 * @return css_stylesheet_t
 */
css_stylesheet_t *
chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *uri)
{
  return s_chxj_css_parse_from_uri(r, pool, NULL, old_stylesheet, uri);
}

/**
 * find selector engine.
 * @param doc        Doc structure.
 * @param stylesheet Retrieval object.
 * @param node       this node.
 * @return css_selector_t if any. null if not found.
 *
 */
css_selector_t *
chxj_css_find_selector(Doc *doc, css_stylesheet_t *stylesheet, Node *node)
{
  request_rec *r    = doc->r;
  apr_pool_t  *pool = doc->pool;
  css_selector_t *sel = NULL;
  char *tag_name   = NULL;
  char *class_name = NULL;
  char *id         = NULL;
  DBG(r, "start chxj_css_find_selector()");

  if (! stylesheet) {
    /* NOT FOUND */
    return NULL;
  }
  s_get_tag_and_class_and_id(doc, node, &tag_name, &class_name, &id);
  
  if (! tag_name || strcasecmp("ROOT", tag_name) == 0) {
    ERR(r, "%s:%d tag_name is null", APLOG_MARK);
    return NULL;
  }
  char *pattern_str1 = NULL;
  char *pattern_str2 = NULL;
  if (class_name && id) {
    pattern_str1 = apr_psprintf(pool, 
                                "^((%s|\\*)(\\.%s)?(#%s)?|(\\*|)\\.%s(#%s)?|(\\*|)(\\.%s)?#%s)$",
                                tag_name,
                                class_name,
                                id,
                                class_name,
                                id,
                                class_name,
                                id);
    pattern_str2 = apr_psprintf(pool,
                                ".*([ >+])((%s|\\*)(\\.%s)?(#%s)?|(\\*|)\\.%s(#%s)?|(\\*|)(\\.%s)?#%s)$",
                                tag_name,
                                class_name,
                                id,
                                class_name,
                                id,
                                class_name,
                                id);
    sel = s_search_selector_regexp(doc, r, pool, stylesheet, pattern_str1, pattern_str2, node);
    if (sel) {
      DBG(r, "end chxj_css_find_selector()");
      return sel;
    }
  }
  else if (id) {
    pattern_str1 = apr_psprintf(pool, 
                                "^((%s|\\*)(#%s)?|(\\*|)(#%s)?|(\\*|)#%s)$",
                                tag_name,
                                id,
                                id,
                                id);
    pattern_str2 = apr_psprintf(pool,
                                ".*([ >+])((%s|\\*)(#%s)?|(\\*|)(#%s)?|(\\*|)#%s)$",
                                tag_name,
                                id,
                                id,
                                id);
    sel = s_search_selector_regexp(doc, r, pool, stylesheet, pattern_str1, pattern_str2, node);
    if (sel) {
      DBG(r, "end chxj_css_find_selector()");
      return sel;
    }
  }
  else if (class_name) {
    pattern_str1 = apr_psprintf(pool, 
                                "^((%s|\\*)(\\.%s)?|(\\*|)\\.%s|(\\*|)(\\.%s))$",
                                tag_name,
                                class_name,
                                class_name,
                                class_name);
    pattern_str2 = apr_psprintf(pool,
                                ".*([ >+])((%s|\\*)(\\.%s)?|(\\*|)\\.%s|(\\*|)(\\.%s)?)$",
                                tag_name,
                                class_name,
                                class_name,
                                class_name);
    sel = s_search_selector_regexp(doc, r, pool, stylesheet, pattern_str1, pattern_str2, node);
    if (sel) {
      DBG(r, "end chxj_css_find_selector()");
      return sel;
    }
  }
  else {
    DBG(r, " ");
    pattern_str1 = apr_psprintf(pool, 
                                "^(%s|\\*)$",
                                tag_name);
    pattern_str2 = apr_psprintf(pool,
                                ".*([ >+])(%s|\\*)$",
                                tag_name);
    DBG(r, " ");
    sel = s_search_selector_regexp(doc, r, pool, stylesheet, pattern_str1, pattern_str2, node);
    if (sel) {
      DBG(r, "end chxj_css_find_selector() (FOUND)");
      return sel;
    }
  }

  DBG(r, "end chxj_css_find_selector() (Not FOUND)");
  return sel;
}

/*===========================================================================*/
/*                                                                           */
/* STATIC                                                                    */
/*                                                                           */
/*===========================================================================*/
static css_selector_t *
s_search_selector_regexp(Doc *doc, request_rec *r, apr_pool_t *pool, css_stylesheet_t *stylesheet, const char *pattern_str1, const char *pattern_str2, Node *node)
{
  Node *node_sv = node;
  css_selector_t *ret_sel = NULL;
  css_selector_t *tail;
  css_selector_t *cur;
  DBG(r, "pattern1:[%s]", pattern_str1);
  DBG(r, "pattern2:[%s]", pattern_str2);
  ap_regex_t *pattern1 = chxj_ap_pregcomp(pool, pattern_str1, AP_REG_EXTENDED|AP_REG_ICASE);
  ap_regex_t *pattern2 = chxj_ap_pregcomp(pool, pattern_str2, AP_REG_EXTENDED|AP_REG_ICASE);
  ap_regex_t *pattern3 = chxj_ap_pregcomp(pool, "^.*([>+ ])([^>+ ]+?)$", AP_REG_EXTENDED|AP_REG_ICASE);
  ap_regex_t *pattern4 = chxj_ap_pregcomp(pool, "^([^.#]+?)?(\\.[^#]+?)?(#.+?)?$", AP_REG_EXTENDED|AP_REG_ICASE);
                              
  tail = (css_selector_t *)((apr_size_t)stylesheet->selector_head.ref - (apr_size_t)APR_OFFSETOF(css_selector_t, next));
  for (cur = tail; 
       cur != &stylesheet->selector_head; 
       cur = (css_selector_t *)((apr_size_t)cur->ref - (apr_size_t)APR_OFFSETOF(css_selector_t, next))) {
    ap_regmatch_t match[256];
    if (chxj_ap_regexec(pattern1, cur->name, pattern1->re_nsub + 1, match, 0) == 0) {
      DBG(r, "match(independent of)");
      ret_sel = cur;
      goto end_of_search;
    }
    else 
    if (chxj_ap_regexec(pattern2, cur->name, pattern2->re_nsub + 1, match, 0) == 0) {
      char *src = apr_pstrdup(pool, cur->name);
      char *one = chxj_ap_pregsub(pool, "$1",src, pattern2->re_nsub + 1, match);
      int loop = 0;
      do {
        *strrchr(src, *one) = 0;
        switch (*one) {
        case '>': /* Child selectors */
          if (chxj_ap_regexec(pattern3, src, pattern3->re_nsub + 1, match, 0) == 0) {
            one = chxj_ap_pregsub(pool, "$1",src, pattern3->re_nsub + 1, match);

            char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, strrchr(src, *one)+1, pattern4, node->parent);
            if (ret) {
              DBG(r, "continue do while");
              node = node->parent;
              DBG(r, "new node:[%x]", node);
              DBG(r, "new node->prev:[%x]", node->prev);
              loop = 1;
            }
          }
          else {
            char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, src, pattern4, node->parent);
            if (ret) {
              ret_sel = cur;
              goto end_of_search;
            }
            loop = 0;
          }
          break;


        case '+': /* Adjacent sibling selectors */
          if (chxj_ap_regexec(pattern3, src, pattern3->re_nsub + 1, match, 0) == 0) {
            one = chxj_ap_pregsub(pool, "$1",src, pattern3->re_nsub + 1, match);
            char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, strrchr(src, *one)+1, pattern4, node->prev);
            if (ret) {
              loop = 1;
              node = node->prev;
              break;
            }
          }
          else {
            if (! node->prev) {
              ret_sel = NULL;
              goto end_of_search;
            }
            char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, src, pattern4, node->prev);
            if (ret) {
              ret_sel = cur;
              goto end_of_search;
            }
          }
          break;


        case ' ': /* Descendant selectors */
          if (chxj_ap_regexec(pattern3, src, pattern3->re_nsub + 1, match, 0) == 0) {
            one = chxj_ap_pregsub(pool, "$1",src, pattern3->re_nsub + 1, match);
            for (; node && node->parent; node = node->parent) {
              if (strcasecmp(node->name, "ROOT") == 0 || strcasecmp(node->parent->name, "ROOT") == 0) {
                loop = 0;
                break;
              }
              char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, strrchr(src, *one)+1, pattern4, node->parent);
              if (ret) {
                loop = 1;
                node = node->parent;
                break;
              }
            }
          }
          else {
            for (; node && node->parent; node = node->parent) {
              if (strcasecmp(node->name, "ROOT") == 0 || strcasecmp(node->parent->name, "ROOT") == 0) {
                loop = 0;
                break;
              }
              char *ret = s_cmp_now_node_vs_current_style(doc, r, pool, src, pattern4, node->parent);
              if (ret) {
                ret_sel = cur;
                goto end_of_search;
              }
            }
          }
          break;
        default:
          loop = 0;
        }
      } while(loop);
      node = node_sv;
    }
  }

end_of_search:
  chxj_ap_pregfree(pool, pattern1);
  chxj_ap_pregfree(pool, pattern2);
  chxj_ap_pregfree(pool, pattern3);
  chxj_ap_pregfree(pool, pattern4);
  return ret_sel;
}
static css_stylesheet_t *
s_chxj_css_parse_from_uri(request_rec *r, apr_pool_t *pool, struct css_already_import_stack *imported_stack, css_stylesheet_t *old_stylesheet, const char *uri)
{
  SCSSParserPtr_t     parser  = NULL;
  SCSSSACHandlerPtr_t handler = NULL;
  char         *css         = NULL;
  char         *full_url    = NULL;
  apr_size_t  srclen;
  css_stylesheet_t *stylesheet = NULL;
  struct css_already_import_stack *new_stack;
  struct css_app_data app_data;
  char         *base_url;
  apr_size_t   css_len;
  

  DBG(r, "start chxj_css_parse_from_uri() uri:[%s]", uri);

  base_url = s_uri_to_base_url(&r->parsed_uri, pool);
  full_url = s_path_to_fullurl(pool, base_url, r->parsed_uri.path, uri);

  /* check already import */
  if (imported_stack && s_is_already_imported(imported_stack, full_url)) {
    DBG(r, "end chxj_css_parse_from_uri(): already imported:[%s]", full_url); 
    return NULL;
  }

  /* GET request */
  css = chxj_serf_get(r, pool, full_url, 0, &css_len);
  if (css == NULL) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): serf_get failed: url:[%s]", APLOG_MARK, uri);
    return NULL;
  }
  srclen = strlen(css);
  
  /* create parser */
  parser = scss_parser_new_from_buf(pool, css, "");
  if (!parser) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_parser_new_from_buf() failed", APLOG_MARK);
    return NULL;
  }

  /* create handler */
  handler = scss_doc_handler_new(parser);
  if (!handler) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): cr_doc_handler_new() failed", APLOG_MARK);
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
    s_copy_already_import_stack(pool, &app_data.imported_stack_head, imported_stack);
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
  list_insert(new_stack, (&app_data.imported_stack_head));

  scss_doc_set_user_data(parser->doc, &app_data);

  handler->startSelector = s_css_parser_from_uri_start_selector;
  handler->endSelector   = s_css_parser_from_uri_end_selector;
  handler->property      = s_css_parser_from_uri_property;
  handler->import        = s_css_parser_from_uri_import_style;

  scss_parse_stylesheet(parser);
  DBG(r, "end chxj_css_parse_from_uri() url:[%s]", uri);

  return s_merge_stylesheet(pool, old_stylesheet, app_data.stylesheet);
}

static css_stylesheet_t *
s_chxj_css_parse_from_buf(request_rec *r, apr_pool_t *pool, struct css_already_import_stack *imported_stack, css_stylesheet_t *old_stylesheet, const char *css)
{
  apr_size_t  srclen;
  SCSSParserPtr_t     parser  = NULL;
  SCSSSACHandlerPtr_t handler = NULL;
  css_stylesheet_t *stylesheet = NULL;
  struct css_app_data app_data;
  struct css_already_import_stack *new_stack;

  DBG(r, "end chxj_css_parse_from_buf() css:[%s]", css);
  srclen = strlen(css);
  
  /* create parser */
  parser = scss_parser_new_from_buf(pool, css, "");
  if (!parser) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): scss_parser_new_from_buf() failed", APLOG_MARK);
    return NULL;
  }
  handler = scss_doc_handler_new(parser);
  if (!handler) {
    ERR(r, "%s:%d end chxj_css_parse_from_uri(): scss_doc_handler_new() failed", APLOG_MARK);
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
    s_copy_already_import_stack(pool, &app_data.imported_stack_head, imported_stack);
  }
  else {
    app_data.imported_stack_head.next = &app_data.imported_stack_head;
    app_data.imported_stack_head.ref  = &app_data.imported_stack_head.next;
  }

  scss_doc_set_user_data(parser->doc, &app_data);

  new_stack = apr_palloc(pool, sizeof(*new_stack));
  memset(new_stack, 0, sizeof(*new_stack));
  new_stack->next = new_stack;
  new_stack->ref  = &new_stack->next;
  new_stack->full_url = "";
  list_insert(new_stack, (&app_data.imported_stack_head));

  handler->startSelector = s_css_parser_from_uri_start_selector;
  handler->endSelector   = s_css_parser_from_uri_end_selector;
  handler->property      = s_css_parser_from_uri_property;
  handler->import        = s_css_parser_from_uri_import_style;

  scss_parse_stylesheet(parser);
  DBG(r, "end chxj_css_parse_from_buf() css:[%s]", css);
  return s_merge_stylesheet(pool, old_stylesheet, app_data.stylesheet);
}


#define ERROR_OCCORED do {          \
    if (app_data->error_occured) {  \
      return;                       \
    }                               \
  }                                 \
  while (0)


#define CB_INIT \
  struct css_app_data *app_data = (struct css_app_data *)scss_doc_get_user_data(parser->doc)



static void 
s_css_parser_from_uri_start_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors)
{
  int ii;
  SCSSNodePtr_t cur = NULL;
  CB_INIT;
  ERROR_OCCORED;

  app_data->selector_count = 0;
  for (cur = selectors->next; cur != selectors; cur = cur->next)
    app_data->selector_count++;

  app_data->selector_list = apr_palloc(app_data->pool, sizeof(char *) * app_data->selector_count);
  if (! app_data->selector_list) {
    ERR(app_data->r, "%s:%d Out of memory", APLOG_MARK);
    app_data->error_occured = 1;
    return;
  }
  ii = 0;
  for (cur = selectors->next; cur != selectors; cur = cur->next) {
    app_data->selector_list[ii++] = apr_pstrdup(app_data->pool, (char *)cur->name);
  }
  app_data->property_head.next = &app_data->property_head;
  app_data->property_head.ref  = &app_data->property_head.next;
}


static void 
s_css_parser_from_uri_end_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors)
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
  if (! stylesheet) return NULL;
  for (cur = stylesheet->selector_head.next; cur != &stylesheet->selector_head; cur = cur->next) {
    if (cur->name && (l == *cur->name || u == *cur->name) && strcasecmp(cur->name, name) == 0) {
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
s_css_parser_from_uri_property(SCSSParserPtr_t parser, const char *propertyName, const char *value, int impotant)
{
  CB_INIT;
  ERROR_OCCORED;
  css_property_t *property;

  if (propertyName && value) {
    property = apr_palloc(app_data->pool, sizeof(*property));
    memset(property, 0, sizeof(*property));
    property->name = apr_pstrdup(app_data->pool, propertyName);
    property->value = apr_pstrdup(app_data->pool, value);

    css_property_t *point_property = &app_data->property_head;
    list_insert(property, point_property);
  }
}




static void
s_css_parser_from_uri_import_style(SCSSParserPtr_t parser, const char *uri, const char **media, const char *defaultNamespaceURI)
{
  CB_INIT;
  ERROR_OCCORED;
  int flag = 0;
  int ii;
  css_stylesheet_t *new_stylesheet = NULL;

  for (ii=0; ii<SCSS_MEDIA_TYPE_NUM; ii++) {
    if (('h' == *media[ii] || 'H' == *media[ii]) && strcasecmp(media[ii], "handheld") == 0) {
      flag = 1;
      break;
    }
    if (('a' == *media[ii] || 'A' == *media[ii]) && strcasecmp(media[ii], "all") == 0) {
      flag = 1;
      break;
    }
  }
  if (flag) {
    if (uri) {
      char      *new_url = NULL;
      char      *base_url = NULL;

      base_url = s_uri_to_base_url(&app_data->r->parsed_uri, app_data->pool);
      new_url = s_path_to_fullurl(app_data->pool, base_url, app_data->r->parsed_uri.path, uri);
      
      new_stylesheet = s_chxj_css_parse_from_uri(app_data->r, app_data->pool, &app_data->imported_stack_head, app_data->stylesheet, new_url);
      if (new_stylesheet) {
        app_data->stylesheet = new_stylesheet;
      }
    }
  }
}


static char *
s_path_to_fullurl(apr_pool_t *pool, const char *base_url, const char *base_path, const char *uri)
{
  char *new_url = NULL;
  if (chxj_starts_with(uri, "http")) {
    return apr_pstrdup(pool, uri);
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
    if ((l == *cur->full_url || u == *cur->full_url) && strcasecmp(url, cur->full_url) == 0) {
      return 1;
    }
  }
  return 0;
}

static css_stylesheet_t *
s_merge_stylesheet(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, css_stylesheet_t *new_stylesheet)
{
  css_selector_t *cur;
  if (! old_stylesheet) {
    return new_stylesheet;
  }

  for (cur = new_stylesheet->selector_head.next; cur != &new_stylesheet->selector_head; cur = cur->next) {
    char *name = cur->name;
    char l = tolower(*name);
    char u = toupper(*name);
    css_selector_t *cur_old;
    int found;

    found = 0;
    for (cur_old = old_stylesheet->selector_head.next; cur_old != &old_stylesheet->selector_head; cur_old = cur_old->next) {
      char *oldname = cur_old->name;
      if ((l == *oldname || u == *oldname) && strcasecmp(name, oldname) == 0) {
        css_property_t *cur_prop;
        for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
          css_property_t *target = s_css_parser_copy_property(pool, cur_prop);
          s_merge_property(cur_old, target);
        }
        found = 1;
        break;
      }
    }
    if (! found) {
      /* add new selector */
      css_property_t *cur_prop;
      css_selector_t *new_selector =  apr_palloc(pool, sizeof(*new_selector));
      memset(new_selector, 0, sizeof(*new_selector));
      new_selector->next = new_selector;
      new_selector->ref  = &new_selector->next;
      new_selector->property_head.next = &new_selector->property_head;
      new_selector->property_head.ref  = &new_selector->property_head.next;
      new_selector->name = apr_pstrdup(pool, name);
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        css_property_t *target = s_css_parser_copy_property(pool, cur_prop);
        list_insert(target, (&new_selector->property_head));
      }
      list_insert(new_selector, (&old_stylesheet->selector_head));
    }
  }

  return old_stylesheet;
}


static void
s_copy_already_import_stack(apr_pool_t *pool, struct css_already_import_stack *base, struct css_already_import_stack *imported_stack)
{
  struct css_already_import_stack *cur;

  base->next = base;
  base->ref  = &base->next;
  
  for (cur = imported_stack->next; cur != imported_stack; cur = cur->next) {
    struct css_already_import_stack *new_stack;
    new_stack = apr_palloc(pool, sizeof(*new_stack));
    memset(new_stack, 0, sizeof(*new_stack));
    new_stack->full_url = apr_pstrdup(pool, cur->full_url);
    list_insert(new_stack, base);
  }
}

/* For DEBUG */
void
chxj_css_stylesheet_dump(css_stylesheet_t *stylesheet)
{
  css_selector_t *cur_sel; 
  css_property_t *cur_prop;

  for (cur_sel = stylesheet->selector_head.next; cur_sel != &stylesheet->selector_head; cur_sel = cur_sel->next) {
    fprintf(stderr, "selector:[%s]\n", cur_sel->name);
    for (cur_prop = cur_sel->property_head.next; cur_prop != &cur_sel->property_head; cur_prop = cur_prop->next) {
      fprintf(stderr, "\tproperty:\n");
      fprintf(stderr, "\t\t- name:%s\n", cur_prop->name);
      fprintf(stderr, "\t\t- value:%s\n", cur_prop->value);
    }
  }
}

static void
s_get_tag_and_class_and_id(Doc *doc, Node *node, char **tag_name, char **class_name, char **id)
{
  Attr *attr;
  *tag_name = node->name;
  for (attr = qs_get_attr(doc, node); attr; attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C', "class", name)) {
      if (*value != 0) {
        *class_name = value;
      }
    }
    else 
    if (STRCASEEQ('i','I', "id", name)) {
      if (*value != 0) {
        *id = value;
      }
    }
    if (*id && *class_name) break;
  }
}

static char *
s_cmp_now_node_vs_current_style(Doc *doc, request_rec *r, apr_pool_t *pool, char *src, ap_regex_t *pattern4, Node *node)
{
  ap_regmatch_t match[256];
  DBG(r, "src:[%s]", src);
  if (chxj_ap_regexec(pattern4, src, pattern4->re_nsub + 1, match, 0) == 0) {
    char *tag_name   = chxj_ap_pregsub(pool, "$1", src, pattern4->re_nsub + 1, match);
    char *class_name = chxj_ap_pregsub(pool, "$2", src, pattern4->re_nsub + 1, match);
    char *id_name    = chxj_ap_pregsub(pool, "$3", src, pattern4->re_nsub + 1, match);
    DBG(r, "tag:[%s] class:[%s] id:[%s]", tag_name, class_name, id_name);
    if (!node) {
      DBG(r, "unmatch(parent is null)");
      return NULL;
    }
    char *node_tag_name   = NULL;
    char *node_class_name = NULL;
    char *node_id_name    = NULL;
    s_get_tag_and_class_and_id(doc, node, &node_tag_name, &node_class_name, &node_id_name);
    if (*tag_name == 0 || strcasecmp(node_tag_name, tag_name) == 0 || strcmp("*", tag_name) == 0) {
      if (class_name && *class_name != 0) {
        if (!node_class_name) {
          DBG(r, "unmatch (class) node:[NULL]");
          return NULL;
        }
        if (strcasecmp(node_class_name, &class_name[1]) != 0) {
          DBG(r, "unmatch (class) node:[%s] style:[%s]", node_class_name, &class_name[1]);
          return NULL;
        }
      }
      if (id_name && *id_name != 0) {
        if (!node_id_name) {
          DBG(r, "unmatch (id) node:[NULL]");
          return NULL;
        }
        if (strcasecmp(node_id_name, &id_name[1]) != 0) {
          DBG(r, "unmatch (id)");
          return NULL;
        }
      }
      DBG(r, "match");
      return src;
    }
    DBG(r, "unmatch(tag) tag:[%s] vs [%s]", tag_name, node_tag_name);
  }
  return NULL;
}


/**
 * find selector with pseudo class.
 * @return new css_stylesheet_t
 */
css_stylesheet_t *
chxj_find_pseudo_selectors(Doc *doc, css_stylesheet_t *stylesheet)
{
  css_selector_t *cur_sel; 
  css_stylesheet_t *result;
  char *pattern_str = "^a:(link|focus|visited)$";
  ap_regex_t *pattern1 = chxj_ap_pregcomp(doc->pool, pattern_str, AP_REG_EXTENDED|AP_REG_ICASE);

  result = apr_palloc(doc->pool, sizeof(*result));
  if (! result) {
    ERR(doc->r, "%s:%d Out of Memory", APLOG_MARK);
    return NULL;
  }
  memset(result, 0, sizeof(*result));
  result->selector_head.next = &result->selector_head;
  result->selector_head.ref  = &result->selector_head.next;

  for (cur_sel = stylesheet->selector_head.next; cur_sel != &stylesheet->selector_head; cur_sel = cur_sel->next) {
    ap_regmatch_t match[256];
    if (chxj_ap_regexec(pattern1, cur_sel->name, pattern1->re_nsub + 1, match, 0) == 0) {
      css_selector_t *new_sel = s_new_selector(doc->pool, result, cur_sel->name);
      css_property_t *cur_prop;
      for (cur_prop = cur_sel->property_head.next; cur_prop != &cur_sel->property_head; cur_prop = cur_prop->next) {
        css_property_t *to_prop = s_css_parser_copy_property(doc->pool, cur_prop);
        list_insert(to_prop, (&new_sel->property_head));
      }
      list_insert(new_sel, (&result->selector_head));
    }
  }

  chxj_ap_pregfree(doc->pool, pattern1);

  return result;
}

/**
 * style attribute parser.
 * @return merged new css_stylesheet_t object.
 */
css_stylesheet_t *
chxj_css_parse_style_attr(Doc *doc, css_stylesheet_t *old_stylesheet, char *tag_name, char *class_name, char *id_name, char *style_attr_value)
{
  css_stylesheet_t *new_stylesheet;
  css_stylesheet_t *dup_stylesheet = NULL;
  char *attr_value;
  char *class_name_sel = NULL;
  char *id_name_sel    = NULL;
  DBG(doc->r, "start chxj_css_parse_style_attr()");

  if (class_name) {
    class_name_sel = apr_psprintf(doc->pool, ".%s", class_name);
  }
  if (id_name) {
    id_name_sel = apr_psprintf(doc->pool, "#%s", id_name);
  }

  attr_value = apr_psprintf(doc->pool, 
                            "%s%s%s { %s; }",
                            tag_name, 
                            (class_name) ?  class_name_sel : "",
                            (id_name)    ?  id_name_sel    : "",
                            style_attr_value);

  if (old_stylesheet) {
    dup_stylesheet = s_dup_stylesheet(doc, old_stylesheet);
  }
  new_stylesheet = s_chxj_css_parse_from_buf(doc->r, doc->pool, NULL, dup_stylesheet, attr_value);

  DBG(doc->r, "end   chxj_css_parse_style_attr()");
  return new_stylesheet;
}


static css_stylesheet_t *
s_dup_stylesheet(Doc *doc, css_stylesheet_t *stylesheet)
{
  css_selector_t   *cur_sel; 
  css_stylesheet_t *result;

  result = apr_palloc(doc->pool, sizeof(*result));
  if (! result) {
    ERR(doc->r, "%s:%d Out of Memory", APLOG_MARK);
    return NULL;
  }
  memset(result, 0, sizeof(*result));
  result->selector_head.next = &result->selector_head;
  result->selector_head.ref  = &result->selector_head.next;

  for (cur_sel = stylesheet->selector_head.next; cur_sel != &stylesheet->selector_head; cur_sel = cur_sel->next) {
    css_selector_t *new_sel = s_new_selector(doc->pool, result, cur_sel->name);
    css_property_t *cur_prop;
    for (cur_prop = cur_sel->property_head.next; cur_prop != &cur_sel->property_head; cur_prop = cur_prop->next) {
      css_property_t *to_prop = s_css_parser_copy_property(doc->pool, cur_prop);
      list_insert(to_prop, (&new_sel->property_head));
    }
    list_insert(new_sel, (&result->selector_head));
  }

  return result;
}

/**
 * style value parser.
 * <style>css</style>
 * @return merged new css_stylesheet_t object.
 */
css_stylesheet_t *
chxj_css_parse_style_value(Doc *doc, css_stylesheet_t *old_stylesheet, char *style_attr_value)
{
  css_stylesheet_t *new_stylesheet;
  css_stylesheet_t *dup_stylesheet = NULL;

  DBG(doc->r, "start chxj_css_parse_style_value()");

  if (old_stylesheet) {
    dup_stylesheet = s_dup_stylesheet(doc, old_stylesheet);
  }
  new_stylesheet = s_chxj_css_parse_from_buf(doc->r, doc->pool, NULL, dup_stylesheet, style_attr_value);

  DBG(doc->r, "end   chxj_css_parse_style_value()");
  return new_stylesheet;
}


/**
 * allocate prop_list_stack.
 */
css_prop_list_stack_t *
chxj_new_prop_list_stack(Doc *doc)
{
  css_prop_list_stack_t *new_stack = apr_palloc(doc->pool, sizeof(css_prop_list_stack_t));
  if (! new_stack) {
    ERR(doc->r, "%s:%d Out of memory.", APLOG_MARK);
    return NULL;
  }
  new_stack->head.next = &new_stack->head;
  new_stack->head.ref  = &new_stack->head.next;
  return new_stack;
}

css_prop_list_t *
chxj_css_create_prop_list(Doc *doc, css_selector_t *sel)
{
  css_prop_list_t *prop_list;
  prop_list = apr_palloc(doc->pool, sizeof(*prop_list));
  if (! prop_list) {
    ERR(doc->r, "%s:%d Out of memory.", APLOG_MARK);
    return NULL;
  }
  prop_list->head.next = &prop_list->head;
  prop_list->head.ref  = &prop_list->head.next;
  if (sel)  {
    css_property_t *cur_prop;
    for (cur_prop = sel->property_head.next; 
         cur_prop != &sel->property_head; 
         cur_prop = cur_prop->next) {
      css_property_t *cp_prop = s_css_parser_copy_property(doc->pool, cur_prop);
      list_insert(cp_prop, (&prop_list->head));
    }
  }
  return prop_list;
}

/**
 * PUSH
 */
void
chxj_css_push_prop_list(css_prop_list_stack_t *stack, css_prop_list_t *prop_list)
{
  list_insert(prop_list, (&stack->head));
}

/**
 * POP
 */
css_prop_list_t *
chxj_css_pop_prop_list(css_prop_list_stack_t *stack)
{
  css_prop_list_t *tail = chxj_css_get_last_prop_list(stack);
  if (! tail) {
    return NULL;
  }

  list_remove(tail);
  return tail;
}


/**
 * POP but delete.
 */
css_prop_list_t *
chxj_css_get_last_prop_list(css_prop_list_stack_t *stack)
{
  css_prop_list_t *tail = (css_prop_list_t *)((apr_size_t)stack->head.ref - (apr_size_t)APR_OFFSETOF(css_prop_list_t, next));
  if (tail == &stack->head) {
    return NULL;
  }
  return tail;
}

css_prop_list_t *
chxj_dup_css_prop_list(Doc *doc, css_prop_list_t *old)
{
  css_property_t *cur_prop;
  css_prop_list_t *new_prop_list;

  new_prop_list = chxj_css_create_prop_list(doc, NULL);
  if (! new_prop_list) {
    ERR(doc->r, "%s:%d Out of memory.", APLOG_MARK);
    return NULL;
  }
  if (old) {
    for (cur_prop = old->head.next; cur_prop != &old->head; cur_prop = cur_prop->next) {
      css_property_t *cp_prop = s_css_parser_copy_property(doc->pool, cur_prop);
      list_insert(cp_prop, (&new_prop_list->head));
    }
  }
  return new_prop_list;
}
void
chxj_css_prop_list_merge_property(Doc *doc, css_prop_list_t *base, css_selector_t *sel)
{
  css_property_t *cur_prop;
  css_property_t *b_prop;
  for (cur_prop = sel->property_head.next; cur_prop != &sel->property_head; cur_prop = cur_prop->next) {
    int found = 0;
    for (b_prop = base->head.next; b_prop != &base->head; b_prop = b_prop->next) {
      if (cur_prop->name && b_prop->name && strcasecmp(cur_prop->name, b_prop->name) == 0) {
        found = 1;
        b_prop->value = apr_pstrdup(doc->pool, cur_prop->value);
      }
    }
    if (! found) {
      css_property_t *cp_prop = s_css_parser_copy_property(doc->pool, cur_prop);
      list_insert(cp_prop, (&base->head));
    }
  }
  
}

css_property_t *
chxj_css_get_property_value(Doc *doc, css_prop_list_t *base, const char *name)
{
  css_property_t *b_prop;
  css_property_t *result;

  result = apr_palloc(doc->pool, sizeof(*result));
  memset(result, 0, sizeof(*result));
  result->next = result;
  result->ref  = &result->next;

  for (b_prop = base->head.next; b_prop != &base->head; b_prop = b_prop->next) {
    if (b_prop->name && strcasecmp(name, b_prop->name) == 0) {
      css_property_t *tmp;

      tmp = apr_palloc(doc->pool, sizeof(*tmp));
      memset(tmp, 0, sizeof(*tmp));
      tmp->next = tmp;
      tmp->ref  = &tmp->next;

      tmp->value = apr_pstrdup(doc->pool, b_prop->value);
      list_insert(tmp, result);
    }
  }
  return result;
}


char *
chxj_css_rgb_func_to_value(apr_pool_t *pool, const char *rgb_func_string)
{
  char *s = apr_pstrdup(pool, rgb_func_string);
  if (chxj_starts_with(s, "rgb")) {
    s += 3;
    s = qs_trim_string(pool, s);
    if (*s == '(') s++;
    int len = strlen(s);
    if (s[len - 1] == ')') s[len-1] = 0;
    char *pstat;
    char *red   = qs_trim_string(pool, apr_strtok(s, ",", &pstat));
    char *green = qs_trim_string(pool, apr_strtok(NULL, ",", &pstat));
    char *blue  = qs_trim_string(pool, apr_strtok(NULL, ",", &pstat));
    if (red && (pstat = strchr(red, '%'))) {
      *pstat = 0;
    }
    else {
      red = "0";
    }
    if (green && (pstat = strchr(green, '%'))) {
      *pstat = 0;
    }
    else {
      green = "0";
    }
    if (blue && (pstat = strchr(blue, '%'))) {
      *pstat = 0;
    }
    else {
      blue = "0";
    }
    double d_red = ((double)chxj_atoi(red) / 100.0);
    double d_green = ((double)chxj_atoi(green) / 100.0);
    double d_blue = ((double)chxj_atoi(blue) / 100.0);
    d_red   *= 255;
    d_green *= 255;
    d_blue  *= 255;
    char *ret = apr_psprintf(pool, "#%02x%02x%02x", (int)d_red, (int)d_green, (int)d_blue);
    return ret;
  }
  return s;
}

/*
 * vim:ts=2 et
 */
