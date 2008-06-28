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
#include "qs_malloc.c"
#include "qs_parse_string.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include "chxj_tag_util.c"


void test_chxj_css_parse_from_uri_001();
void test_chxj_css_parse_from_uri_002();
void test_chxj_css_parse_from_uri_003();
void test_chxj_css_parse_from_uri_004();
void test_chxj_css_parse_from_uri_005();
void test_chxj_css_parse_from_uri_006();
void test_chxj_css_parse_from_uri_007();
void test_chxj_css_parse_from_uri_008();

/* chxj_css_find_selector */
void test_chxj_css_find_selector_001();
void test_chxj_css_find_selector_002();
void test_chxj_css_find_selector_003();
void test_chxj_css_find_selector_004();
void test_chxj_css_find_selector_005();
void test_chxj_css_find_selector_006();
void test_chxj_css_find_selector_007();
void test_chxj_css_find_selector_unmatch_pattern_001();
void test_chxj_css_find_selector_unmatch_pattern_002();
void test_chxj_css_find_selector_unmatch_pattern_003();
/* E > F */
void test_chxj_css_find_selector_p_001();
void test_chxj_css_find_selector_p_002();
void test_chxj_css_find_selector_p_003();
void test_chxj_css_find_selector_p_004();
void test_chxj_css_find_selector_p_005();
void test_chxj_css_find_selector_p_006();
void test_chxj_css_find_selector_p_007();
void test_chxj_css_find_selector_p_008();
void test_chxj_css_find_selector_p_009();

/* E F */
void test_chxj_css_find_selector_des_001();
void test_chxj_css_find_selector_des_002();
void test_chxj_css_find_selector_des_003();
void test_chxj_css_find_selector_des_004();
void test_chxj_css_find_selector_des_005();
void test_chxj_css_find_selector_des_006();
void test_chxj_css_find_selector_des_007();
void test_chxj_css_find_selector_des_008();
void test_chxj_css_find_selector_des_009();
void test_chxj_css_find_selector_des_010();

/* E + F */
void test_chxj_css_find_selector_shib_001();
void test_chxj_css_find_selector_shib_002();
void test_chxj_css_find_selector_shib_003();
void test_chxj_css_find_selector_shib_004();
void test_chxj_css_find_selector_shib_005();
void test_chxj_css_find_selector_shib_006();
void test_chxj_css_find_selector_shib_007();
void test_chxj_css_find_selector_shib_008();
void test_chxj_css_find_selector_shib_009();
void test_chxj_css_find_selector_shib_010();

/* MIX (p + div > p + hr) */
void test_chxj_css_find_selector_mix_001();
void test_chxj_css_find_selector_mix_002(); /* p + div p + hr */
void test_chxj_css_find_selector_mix_003();
void test_chxj_css_find_selector_mix_004();

/* pseudo */
void test_chxj_css_find_selector_pseudo_001();
void test_chxj_css_find_selector_pseudo_002();
void test_chxj_css_find_selector_pseudo_003();
void test_chxj_css_find_selector_pseudo_004();

/* style attribute */
void test_chxj_css_parse_style_attr_001();
void test_chxj_css_parse_style_attr_002();
void test_chxj_css_parse_style_attr_003();

/* style value */
void test_chxj_css_parse_style_value_001();
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
  CU_add_test(css_suite, "test css find_selector 001",                      test_chxj_css_find_selector_001);
  CU_add_test(css_suite, "test css find_selector 002",                      test_chxj_css_find_selector_002);
  CU_add_test(css_suite, "test css find_selector 003",                      test_chxj_css_find_selector_003);
  CU_add_test(css_suite, "test css find_selector 004",                      test_chxj_css_find_selector_004);
  CU_add_test(css_suite, "test css find_selector 005",                      test_chxj_css_find_selector_005);
  CU_add_test(css_suite, "test css find_selector 006",                      test_chxj_css_find_selector_006);
  CU_add_test(css_suite, "test css find_selector 007",                      test_chxj_css_find_selector_007);
  CU_add_test(css_suite, "test css find_selector unmatch 001",              test_chxj_css_find_selector_unmatch_pattern_001);
  CU_add_test(css_suite, "test css find_selector unmatch 002",              test_chxj_css_find_selector_unmatch_pattern_002);
  CU_add_test(css_suite, "test css find_selector unmatch 003",              test_chxj_css_find_selector_unmatch_pattern_003);
  CU_add_test(css_suite, "test css find_selector with '>' 001",             test_chxj_css_find_selector_p_001);
  CU_add_test(css_suite, "test css find_selector with '>' 002",             test_chxj_css_find_selector_p_002);
  CU_add_test(css_suite, "test css find_selector with '>' 003",             test_chxj_css_find_selector_p_003);
  CU_add_test(css_suite, "test css find_selector with '>' 004",             test_chxj_css_find_selector_p_004);
  CU_add_test(css_suite, "test css find_selector with '>' 005",             test_chxj_css_find_selector_p_005);
  CU_add_test(css_suite, "test css find_selector with '>' 006",             test_chxj_css_find_selector_p_006);
  CU_add_test(css_suite, "test css find_selector with '>' 007",             test_chxj_css_find_selector_p_007);
  CU_add_test(css_suite, "test css find_selector with '>' 008",             test_chxj_css_find_selector_p_008);
  CU_add_test(css_suite, "test css find_selector with '>' 009",             test_chxj_css_find_selector_p_009);

  CU_add_test(css_suite, "test css find_selector with ' ' 001",             test_chxj_css_find_selector_des_001);
  CU_add_test(css_suite, "test css find_selector with ' ' 002",             test_chxj_css_find_selector_des_002);
  CU_add_test(css_suite, "test css find_selector with ' ' 003",             test_chxj_css_find_selector_des_003);
  CU_add_test(css_suite, "test css find_selector with ' ' 004",             test_chxj_css_find_selector_des_004);
  CU_add_test(css_suite, "test css find_selector with ' ' 005",             test_chxj_css_find_selector_des_005);
  CU_add_test(css_suite, "test css find_selector with ' ' 006",             test_chxj_css_find_selector_des_006);
  CU_add_test(css_suite, "test css find_selector with ' ' 007",             test_chxj_css_find_selector_des_007);
  CU_add_test(css_suite, "test css find_selector with ' ' 008",             test_chxj_css_find_selector_des_008);
  CU_add_test(css_suite, "test css find_selector with ' ' 009",             test_chxj_css_find_selector_des_009);
  CU_add_test(css_suite, "test css find_selector with ' ' 010",             test_chxj_css_find_selector_des_010);

  CU_add_test(css_suite, "test css find_selector with '+' 001",             test_chxj_css_find_selector_shib_001);
  CU_add_test(css_suite, "test css find_selector with '+' 002",             test_chxj_css_find_selector_shib_002);
  CU_add_test(css_suite, "test css find_selector with '+' 003",             test_chxj_css_find_selector_shib_003);
  CU_add_test(css_suite, "test css find_selector with '+' 004",             test_chxj_css_find_selector_shib_004);
  CU_add_test(css_suite, "test css find_selector with '+' 005",             test_chxj_css_find_selector_shib_005);
  CU_add_test(css_suite, "test css find_selector with '+' 006",             test_chxj_css_find_selector_shib_006);
  CU_add_test(css_suite, "test css find_selector with '+' 007",             test_chxj_css_find_selector_shib_007);
  CU_add_test(css_suite, "test css find_selector with '+' 008",             test_chxj_css_find_selector_shib_008);
  CU_add_test(css_suite, "test css find_selector with '+' 009",             test_chxj_css_find_selector_shib_009);
  CU_add_test(css_suite, "test css find_selector with '+' 010",             test_chxj_css_find_selector_shib_010);

  CU_add_test(css_suite, "test css find_selector with 'mix' 001",           test_chxj_css_find_selector_mix_001);
  CU_add_test(css_suite, "test css find_selector with 'mix' 002",           test_chxj_css_find_selector_mix_002);
  CU_add_test(css_suite, "test css find_selector with 'mix' 003",           test_chxj_css_find_selector_mix_003);
  CU_add_test(css_suite, "test css find_selector with 'mix' 004",           test_chxj_css_find_selector_mix_004);

  CU_add_test(css_suite, "test css find_pseudo_selector 001",               test_chxj_css_find_selector_pseudo_001);
  CU_add_test(css_suite, "test css find_pseudo_selector 002",               test_chxj_css_find_selector_pseudo_002);
  CU_add_test(css_suite, "test css find_pseudo_selector 003",               test_chxj_css_find_selector_pseudo_003);
  CU_add_test(css_suite, "test css find_pseudo_selector 004",               test_chxj_css_find_selector_pseudo_004);

  CU_add_test(css_suite, "test css parse style attr 001",                   test_chxj_css_parse_style_attr_001);
  CU_add_test(css_suite, "test css parse style attr 002",                   test_chxj_css_parse_style_attr_002);
  CU_add_test(css_suite, "test css parse style attr 003",                   test_chxj_css_parse_style_attr_003);

  CU_add_test(css_suite, "test css parse style value 001",                  test_chxj_css_parse_style_value_001);
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
char *test_chxj_serf_get001(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html { display: none }";
  *sz = strlen(css);
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



char *test_chxj_serf_get002(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body { display: none }\nhtml,body { display: none }";
  *sz = strlen(css);
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



char *test_chxj_serf_get003(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "@import url(\"hoge.css\");\nhtml,body { display: none }\nhtml,body { display: none }";
  *sz = strlen(css);
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




char *test_chxj_serf_get004(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "@import url(\"hoge.css\");\nhtml,body { display: none }\nhtml,body { display: none }";
  *sz = strlen(css);
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


char *test_chxj_serf_get005(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body { display: none }";
  *sz = strlen(css);
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



char *test_chxj_serf_get006(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body { display: none }";
  *sz = strlen(css);
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



char *test_chxj_serf_get007(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body { display: none }";
  *sz = strlen(css);
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



char *test_chxj_serf_get008(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html > body + h2 { display: none }";
  *sz = strlen(css);
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
      case 0: CU_ASSERT(strcmp(cur->name, "html>body+h2") == 0); break;
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
    CU_ASSERT(ii == 1);
  }


  APR_TERM;
}


/*===========================================================================*/
/*                                                                           */
/* chxj_css_find_selector                                                    */
/*                                                                           */
/*===========================================================================*/
char *test_chxj_serf_get009(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr.abc#id1 { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_001()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get009;
  fprintf(stderr, "start %s\n", __func__);

  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
#undef TEST_STRING
  fprintf(stderr, "end %s\n", __func__);
}



char *test_chxj_serf_get010(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .abc#id1 { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_002()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}



char *test_chxj_serf_get011(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, #id1 { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_003()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get011;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}



char *test_chxj_serf_get012(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .abc { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_004()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get012;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}


char *test_chxj_serf_get013(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr.abc { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_005()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get013;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}



char *test_chxj_serf_get014(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr#id1 { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_006()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get014;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}



char *test_chxj_serf_get015(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr { display: none }";

  return css;
}
void test_chxj_css_find_selector_007()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get015;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}


char *test_chxj_serf_get016(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, li { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_unmatch_pattern_001()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get016;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get017(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr.def { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_unmatch_pattern_002()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get017;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get018(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr#id2 { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_unmatch_pattern_003()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get018;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

/*===========================================================================*/
/* E > F                                                                     */
/*===========================================================================*/
char *test_chxj_serf_get019(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, body>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_001()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get019;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get020(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html>body>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_002()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get020;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get021(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html>body>div>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_003()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get021;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get022(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html>body>div>div>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_004()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get023(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html>body>div.def#id2>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_005()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get023;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get024(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html>body>div.abc#id2>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_006()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get024;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get025(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, body>*.def#id2>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_007()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get025;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get026(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, body.def#id2>*.def#id2>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_008()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get026;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get027(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, body.abc#id2>*.def#id2>hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_p_009()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><hr class=\"abc\" id=\"id1\"></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get027;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get028(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, body hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_001()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get028;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get029(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html body hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_002()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get029;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get030(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html body.def#id2 hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_003()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get030;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get031(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html body.abc#id2 hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_004()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get031;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get032(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, html body.def hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_005()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get032;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get033(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .def hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_006()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get033;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get034(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .abc hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_007()
{
#define TEST_STRING "<html><head></head><body class=\"def\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get034;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get035(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .abc hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_008()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id2\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get035;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get036(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, #id2 hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_009()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get036;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get037(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, #id3 hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_des_010()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get037;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get038(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_001()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p>aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get038;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get039(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p.pclass#pid + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_002()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get039;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get040(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .pclass#pid + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_003()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get040;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get041(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, #pid + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_004()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get041;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get042(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, .pclass + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_005()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get043(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p.pclass + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_006()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get043;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get044(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p#pid + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_007()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get044;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get045(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p.aclass#pid + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_008()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get045;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get046(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p.pclass#did + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_009()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get046;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get047(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, hr +  p + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_shib_010()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><div><hr><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get047;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->child;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;
  tmp_node = node->next;
  node = tmp_node;

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

/* MIX */
char *test_chxj_serf_get048(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p + div >  p + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_mix_001()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><p></p><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get048;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child; /* root -> html */
  node = tmp_node;
  tmp_node = node->child; /* html -> head */
  node = tmp_node;
  tmp_node = node->next;  /* head -> body */
  node = tmp_node;
  tmp_node = node->child; /* body -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> hr*/
  node = tmp_node;
  qs_dump_node(&doc, node_sv, 0);

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get049(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p + div  p + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_mix_002()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><p></p><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get049;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child; /* root -> html */
  node = tmp_node;
  tmp_node = node->child; /* html -> head */
  node = tmp_node;
  tmp_node = node->next;  /* head -> body */
  node = tmp_node;
  tmp_node = node->child; /* body -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> hr*/
  node = tmp_node;
  qs_dump_node(&doc, node_sv, 0);

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get050(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p + div  p > hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_mix_003()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><p></p><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></div></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get050;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child; /* root -> html */
  node = tmp_node;
  tmp_node = node->child; /* html -> head */
  node = tmp_node;
  tmp_node = node->next;  /* head -> body */
  node = tmp_node;
  tmp_node = node->child; /* body -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p -> hr*/
  node = tmp_node;
  qs_dump_node(&doc, node_sv, 0);

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel == NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get051(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "html,body, p > div  p + hr { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_mix_004()
{
#define TEST_STRING "<html><head></head><body class=\"abc\" id=\"id3\"><div class=\"def\" id=\"id2\"><p><div><p class=\"pclass\" id=\"pid\">aaa</p><hr class=\"abc\" id=\"id1\"></div></p></body></html>"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get051;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node_sv = node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  tmp_node = node->child; /* root -> html */
  node = tmp_node;
  tmp_node = node->child; /* html -> head */
  node = tmp_node;
  tmp_node = node->next;  /* head -> body */
  node = tmp_node;
  tmp_node = node->child; /* body -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->child; /* p -> div */
  node = tmp_node;
  tmp_node = node->child; /* div -> p */
  node = tmp_node;
  tmp_node = node->next; /* p ->  hr */
  node = tmp_node;
  qs_dump_node(&doc, node_sv, 0);

  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  sel = chxj_css_find_selector(&doc, ret, node);
  CU_ASSERT(sel != NULL);
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}


/* pseudo selector */
char *test_chxj_serf_get052(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:link { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_pseudo_001()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  int ii;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get052;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  ret = chxj_find_pseudo_selectors(&doc, ret);
  CU_ASSERT(ret != NULL);
  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%s\n", sel->name);
    switch(ii) {
    case 0:
      CU_ASSERT(strcasecmp(sel->name, "a:link") == 0);
      break;
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
char *test_chxj_serf_get053(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:visited { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_pseudo_002()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  int ii;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get053;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  ret = chxj_find_pseudo_selectors(&doc, ret);
  CU_ASSERT(ret != NULL);
  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%s\n", sel->name);
    switch(ii) {
    case 0:
      CU_ASSERT(strcasecmp(sel->name, "a:visited") == 0);
      break;
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get054(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:focus { display: none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_pseudo_003()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  int ii;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get054;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  ret = chxj_find_pseudo_selectors(&doc, ret);
  CU_ASSERT(ret != NULL);
  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%s\n", sel->name);
    switch(ii) {
    case 0:
      CU_ASSERT(strcasecmp(sel->name, "a:focus") == 0);
      break;
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get055(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_find_selector_pseudo_004()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  int ii;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get055;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);

  ret = chxj_find_pseudo_selectors(&doc, ret);
  CU_ASSERT(ret != NULL);
  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%s\n", sel->name);
    switch(ii) {
    case 0:
      CU_ASSERT(strcasecmp(sel->name, "a:focus") == 0);
      break;
    case 1:
      CU_ASSERT(strcasecmp(sel->name, "a:link") == 0);
      break;
    case 2:
      CU_ASSERT(strcasecmp(sel->name, "a:visited") == 0);
      break;
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}


/* parse style attr */
char *test_chxj_serf_get056(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_parse_style_attr_001()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  css_property_t *cur_prop;
  int ii;
  int jj;
  APR_INIT;

  chxj_serf_get = test_chxj_serf_get056;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);
  ret = chxj_css_parse_style_attr(&doc, ret, "br", NULL, NULL, "clear:both");
  CU_ASSERT(ret != NULL);

  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%d %s\n", ii, sel->name);
    switch (ii) {
    case 5:
      jj = 0;
      for (cur_prop = sel->property_head.next; cur_prop != &sel->property_head; cur_prop = cur_prop->next) {
        fprintf(stderr, "%d %s\n", jj, cur_prop->name);
        switch (jj) {
        case 0: 
          CU_ASSERT(strcasecmp(cur_prop->name,  "clear") == 0);
          CU_ASSERT(strcasecmp(cur_prop->value, "both") == 0);
          break;
        }
        jj++;
      }
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}


char *test_chxj_serf_get057(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_parse_style_attr_002()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  css_property_t *cur_prop;
  int ii;
  int jj;
  APR_INIT;

  chxj_serf_get = test_chxj_serf_get057;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);
  ret = chxj_css_parse_style_attr(&doc, ret, "br", "abc", NULL, "clear:both");
  CU_ASSERT(ret != NULL);

  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%d %s\n", ii, sel->name);
    switch (ii) {
    case 5:
      jj = 0;
      for (cur_prop = sel->property_head.next; cur_prop != &sel->property_head; cur_prop = cur_prop->next) {
        fprintf(stderr, "%d %s\n", jj, cur_prop->name);
        switch (jj) {
        case 0: 
          CU_ASSERT(strcasecmp(cur_prop->name,  "clear") == 0);
          CU_ASSERT(strcasecmp(cur_prop->value, "both") == 0);
          break;
        }
        jj++;
      }
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

char *test_chxj_serf_get058(request_rec *r, apr_pool_t *ppool, const char *uri_path, int set_headers_flag, apr_size_t *sz)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }";
  *sz = strlen(css);
  return css;
}
void test_chxj_css_parse_style_attr_003()
{
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  css_property_t *cur_prop;
  int ii;
  int jj;
  APR_INIT;

  chxj_serf_get = test_chxj_serf_get058;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_from_uri(&r, r.pool, NULL, "/hoge.css");
  CU_ASSERT(ret != NULL);
  ret = chxj_css_parse_style_attr(&doc, ret, "br", "abc", "id1", "clear:both");
  CU_ASSERT(ret != NULL);

  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%d %s\n", ii, sel->name);
    switch (ii) {
    case 5:
      jj = 0;
      for (cur_prop = sel->property_head.next; cur_prop != &sel->property_head; cur_prop = cur_prop->next) {
        fprintf(stderr, "%d %s\n", jj, cur_prop->name);
        switch (jj) {
        case 0: 
          CU_ASSERT(strcasecmp(cur_prop->name,  "clear") == 0);
          CU_ASSERT(strcasecmp(cur_prop->value, "both") == 0);
          break;
        }
        jj++;
      }
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}

/* style tag */
void test_chxj_css_parse_style_value_001()
{
#define TEST_STRING "a { color: rgb(1,2,3); }"
  Doc doc;
  Node *node;
  Node *tmp_node;
  Node *node_sv;
  css_stylesheet_t *ret;
  css_selector_t *sel;
  css_property_t *cur_prop;
  int ii;
  int jj;
  APR_INIT;

  fprintf(stderr, "start %s\n", __func__);
  doc.r = &r;
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;


  apr_uri_parse(p, "http://localhost:888/abc", &r.parsed_uri); \

  ret = chxj_css_parse_style_value(&doc, NULL, TEST_STRING);
  CU_ASSERT(ret != NULL);
  ii = 0;
  for (sel = ret->selector_head.next;ret && sel != &ret->selector_head; sel = sel->next) {
    fprintf(stderr, "%d %s\n", ii, sel->name);
    jj = 0;
    for (cur_prop = sel->property_head.next; cur_prop != &sel->property_head; cur_prop = cur_prop->next) {
      fprintf(stderr, "%d %s:%s\n", jj, cur_prop->name, cur_prop->value);
      switch (jj) {
      case 0:
        CU_ASSERT(strcasecmp(cur_prop->name, "color") == 0);
        CU_ASSERT(strcasecmp(cur_prop->value, "rgb(1, 2, 3)") == 0);
      }
      jj++;
    }
    ii++;
  }
  APR_TERM;
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
