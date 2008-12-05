#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "apr_uri.h"
#include "chxj_apache.h"
#include "apr.h"
#include "qs_malloc.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include "qs_parse_file.c"
#include "chxj_buffered_write.c"
#include "chxj_conv_kana.c"
#include "chxj_node_convert.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_str_util.c"
#include <iconv.h>



void test_conv_kana_001();
/* pend */

int
main()
{
  CU_pSuite conv_kana_suite;
  CU_initialize_registry();
  conv_kana_suite = CU_add_suite("test chxj_convert_conv_kana()", NULL, NULL);
  CU_add_test(conv_kana_suite, "test void src1",                                    test_conv_kana_001);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
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

char *chxj_qr_code_blob_handler(request_rec* r, const char* indata, size_t* len)
{
  return NULL;
}

char *test_os_escape_path(apr_pool_t *p, const char *path, int partial)
{
  return (char *)path;
}

void test_set_content_type(request_rec *r, const char *ct)
{
  fprintf(stderr, "%s:%d set_content_type(%s)\n", __FILE__,__LINE__,ct);
}

const char *test_run_http_scheme(request_rec *r)
{
  static char *s = "http";
  return s;
}

tag_handler chtml10_handler[] = {};
tag_handler chtml20_handler[] = {};
tag_handler chtml30_handler[] = {};
tag_handler chtml40_handler[] = {};
tag_handler chtml50_handler[] = {};
tag_handler hdml_handler[] = {};
tag_handler jhtml_handler[] = {};
tag_handler jxhtml_handler[] = {};
tag_handler xhtml_handler[] = {};
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
  entries.action |= CONVRULE_Z2H_ON_BIT;
  return &entries;
}


char *test_ap_escape_html(apr_pool_t *pool, const char *s)
{
  return (char *)s;
}

#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
    r.hostname = apr_pstrdup(p, "localhost"); \
  } \
  while (0)

#define APR_TERM \
  do { \
    apr_terminate(); \
  } while (0)

#define COOKIE_INIT(X) \
  do { \
    X.cookie_id = "test_cookie_id"; \
  } while (0) \

#define SPEC_INIT(X) \
  do { \
    X.html_spec_type = CHXJ_SPEC_Chtml_1_0; \
  } while (0)



void test_conv_kana_001()
{
#define  TEST_STRING "ァアィイゥウェエォオカガキギクグケゲコゴ" \
                     "サザシジスズセゼソゾタダチヂッツヅテデトド" \
                     "ナニヌネノハバパヒビピフブプヘベペホボポ" \
                     "マミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ" \
                     "、。，．・：；？！゛゜" \
                     "゛゜「」ー"
#define  RESULT_STRING "ｧｱｨｲｩｳｪｴｫｵｶｶﾞｷｷﾞｸｸﾞｹｹﾞｺｺﾞ" \
                       "ｻｻﾞｼｼﾞｽｽﾞｾｾﾞｿｿﾞﾀﾀﾞﾁﾁﾞｯﾂﾂﾞﾃﾃﾞﾄﾄﾞ" \
                       "ﾅﾆﾇﾈﾉﾊﾊﾞﾊﾟﾋﾋﾞﾋﾟﾌﾌﾞﾌﾟﾍﾍﾞﾍﾟﾎﾎﾞﾎﾟ" \
                       "ﾏﾐﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛヮﾜヰヱｦﾝｳﾞヵヶ" \
                       "､｡,.･:;?!ﾞﾟﾞﾟ｢｣ｰ"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;


  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  memset(&entry, 0, sizeof(chxjconvrule_entry));
  entry.encoding = apr_pstrdup(r.pool, "UTF8");
  entry.action |= CONVRULE_Z2H_ON_BIT;

  destlen = sizeof(TEST_STRING)-1;
  char *s = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_conv_z2h_kana(&r, s, &destlen,&entry);
  destlen = strlen(ret);
  ret = chxj_rencoding(&r, ret, &destlen);

  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);

  APR_TERM;
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
