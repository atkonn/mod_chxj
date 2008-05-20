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
#include "apr_pools.h"

/*===========================================================================*/
/* PARSER                                                                    */
/*===========================================================================*/
static void s_css_parser_from_uri_start_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_end_selector(CRDocHandler * a_this, CRSelector *a_selector_list);
static void s_css_parser_from_uri_property(CRDocHandler *a_this, CRString *a_name, CRTerm *a_expression, gboolean a_is_important);

css_stylesheet_t *
chxj_css_parse_from_uri(apr_pool_t *pool, css_stylesheet_t *old_stylesheet, const char *uri)
{
  CRParser     *parser      = NULL ;
  CRDocHandler *sac_handler = NULL ;
  enum CRStatus ret;

  parser = cr_parser_new_from_file(uri, CR_AUTO);
  if (!parser) {
    fprintf(stderr, "parserオブジェクト失敗\n");
    return NULL;
  }

  sac_handler = cr_doc_handler_new();
  if (!sac_handler) {
    fprintf(stderr, "sac_handlerオブジェクト失敗\n");
    cr_parser_destroy(parser);
    return NULL;
  }

  sac_handler->app_data = "こんちは";

  sac_handler->start_selector = s_css_parser_from_uri_start_selector;
  sac_handler->end_selector   = s_css_parser_from_uri_end_selector;
  sac_handler->property       = s_css_parser_from_uri_property;

  ret = cr_parser_set_sac_handler(parser, sac_handler);
  if (ret != CR_OK) {
    fprintf(stderr, "なんかエラーが起きた:[%d]\n", ret);
    cr_parser_destroy(parser);
    return NULL;
  }

  ret = cr_parser_parse(parser);
  return NULL;
}


static void 
s_css_parser_from_uri_start_selector(CRDocHandler * a_this, CRSelector *a_selector_list)
{
  if (a_selector_list) {
    CRSelector *cur = NULL;
    for (cur = a_selector_list; cur; cur = cur->next) {
      if (cur->simple_sel) {
        guchar *tmp_str = cr_simple_sel_to_string(cur->simple_sel);
        if (tmp_str) {
          printf("start selector:[%s]\n", tmp_str);
          g_free (tmp_str);
          tmp_str = NULL;
        }
      }
    }
  }
}

static void 
s_css_parser_from_uri_end_selector(CRDocHandler * a_this, CRSelector *a_selector_list)
{
  printf("%s\n", a_this->app_data);
  if (a_selector_list) {
    CRSelector *cur = NULL;
    for (cur = a_selector_list; cur; cur = cur->next) {
      if (cur->simple_sel) {
        guchar *tmp_str = cr_simple_sel_to_string(cur->simple_sel);
        if (tmp_str) {
          printf("end selector:[%s]\n", tmp_str);
          g_free (tmp_str);
          tmp_str = NULL;
        }
      }
    }
  }
}

static void
s_css_parser_from_uri_property(CRDocHandler *a_this, CRString *a_name, CRTerm *a_expression, gboolean a_is_important)
{
  printf("%s\n", a_this->app_data);
  if (a_name) {
    printf("property: name:[%s]\n", cr_string_peek_raw_str(a_name));
  }  
  if (a_expression) {
    CRTerm *cur = NULL;
    for (cur = a_expression; cur; cur = cur->next) {
      guchar *tmp = cr_term_one_to_string(cur);
      printf("property: value:[%s]\n", tmp);
      g_free(tmp);
      tmp = NULL;
    }
  }
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
