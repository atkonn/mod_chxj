#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "mod_chxj.h"
#include "chxj_apache.h"
#include "apr.h"
#include "apr_strings.h"
#include "apr_uri.h"
#include "chxj_serf.h"
#include "chxj_css.h"
#include "chxj_serf.c"
#include "chxj_css.c"
#include "chxj_encoding.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_str_util.c"



void test_chxj_css_parse_from_uri_001();
void test_chxj_css_parse_from_uri_002();
void test_chxj_css_parse_from_uri_003();
void test_chxj_css_parse_from_uri_004();
void test_chxj_css_parse_from_uri_005();
void test_chxj_css_parse_from_uri_006();
void test_chxj_css_parse_from_uri_007();
void test_chxj_css_parse_from_uri_008();
/* pend */

int
main()
{
  CU_pSuite css_suite;
  CU_initialize_registry();
  css_suite = CU_add_suite("test chxj_css", NULL, NULL);

  CU_add_test(css_suite, "test css 001",                                    test_chxj_css_parse_from_uri_001);
  CU_add_test(css_suite, "test css 002",                                    test_chxj_css_parse_from_uri_002);
  CU_add_test(css_suite, "test css 003",                                    test_chxj_css_parse_from_uri_003);
  CU_add_test(css_suite, "test css 004",                                    test_chxj_css_parse_from_uri_004);
  CU_add_test(css_suite, "test css 005",                                    test_chxj_css_parse_from_uri_005);
  CU_add_test(css_suite, "test css 006",                                    test_chxj_css_parse_from_uri_006);
  CU_add_test(css_suite, "test css 007",                                    test_chxj_css_parse_from_uri_007);
  CU_add_test(css_suite, "test css 008",                                    test_chxj_css_parse_from_uri_008);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}

void test_set_content_type(request_rec *r, const char *ct)
{
  fprintf(stderr, "%s:%d set_content_type(%s)\n", __FILE__,__LINE__,ct);
}

char *test_ap_escape_html(apr_pool_t *pool, const char *s)
{
  return (char *)s;
}

char *test_os_escape_path(apr_pool_t *p, const char *path, int partial)
{
  return (char *)path;
}

const char *test_run_http_scheme(request_rec *r)
{
  static char *s = "http";
  return s;
}

void * test_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
  static mod_chxj_config cfg;
  memset(&cfg, 0, sizeof(mod_chxj_config));
  cfg.new_line_type = NLTYPE_NONE;
  return &cfg;
}

chxjconvrule_entry *
chxj_apply_convrule(request_rec* r, apr_array_header_t* convrules)
{
  static chxjconvrule_entry entries;
  memset(&entries, 0, sizeof(chxjconvrule_entry));
  entries.encoding = apr_pstrdup(r->pool, "UTF8");
  return &entries;
}

void test_log_rerror(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}
void test_log_error(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
    r.hostname = apr_pstrdup(p, "localhost"); \
    apr_uri_parse(p, "http://localhost/abc", &r.parsed_uri); \
  } \
  while (0)

#define APR_TERM \
  do { \
    apr_terminate(); \
  } while (0)


/** TEST CASE START */
char *test_chxj_serf_get001(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_001()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "http://localhost/hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      ii++;
    }
    CU_ASSERT(ii == 1);
    CU_ASSERT(jj == 1);
  }


  APR_TERM;
}



char *test_chxj_serf_get002(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html,body { display: none }\nhtml,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_002()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get002;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "http://localhost/hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}



char *test_chxj_serf_get003(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "@import url(\"hoge.css\");\nhtml,body { display: none }\nhtml,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_003()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get003;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "http://localhost/hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}




char *test_chxj_serf_get004(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "@import url(\"hoge.css\");\nhtml,body { display: none }\nhtml,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_004()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get004;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}


char *test_chxj_serf_get005(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_005()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get005;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}



char *test_chxj_serf_get006(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_006()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get006;

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "../hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}



char *test_chxj_serf_get007(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_007()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}



char *test_chxj_serf_get008(request_rec *r, apr_pool_t *ppool, const char *uri_path)
{
  static char *css = "html,body { display: none }";

  return css;
}
void test_chxj_css_parse_from_uri_008()
{
  css_stylesheet_t *ret;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get008;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);
  {
    css_selector_t *cur;
    int ii = 0;
    int jj = 0;
    for (cur = ret->selector_head.next; cur != &ret->selector_head; cur = cur->next) {
      css_property_t *cur_prop;
      switch(ii) {
      case 0: CU_ASSERT(strcmp(cur->name, "html") == 0); break;
      case 1: CU_ASSERT(strcmp(cur->name, "body") == 0); break;
      }
      jj = 0;
      for (cur_prop = cur->property_head.next; cur_prop != &cur->property_head; cur_prop = cur_prop->next) {
        switch (jj) {
        case 0: 
          CU_ASSERT(strcmp(cur_prop->name, "display") == 0);
          CU_ASSERT(strcmp(cur_prop->value, "none") == 0);
          break;
        }
        jj++;
      }
      CU_ASSERT(jj == 1);
      ii++;
    }
    CU_ASSERT(ii == 2);
  }


  APR_TERM;
}
/*
 * vim:ts=2 et
 */
