#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include "scss.h"



void test_import_001();
void test_import_002();
void test_import_003();
/* pend */

int
main()
{
  CU_pSuite atkeyword_suite;
  CU_initialize_registry();
  atkeyword_suite = CU_add_suite("test Simple CSS(is based CSS2 but not CSS2)  Parser", NULL, NULL);
  CU_add_test(atkeyword_suite, "@import 001",  test_import_001);
  CU_add_test(atkeyword_suite, "@import 002",  test_import_002);
  CU_add_test(atkeyword_suite, "@import 003",  test_import_003);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}


void test_import_001()
{
#define TEST_STRING "@import"
  SCSSDocPtr_t doc;
  apr_pool_t *pool;

  fprintf(stderr, "start %s\n", __func__);
  apr_initialize();
  apr_pool_create(&pool, NULL);

  doc = scss_parser(pool,  TEST_STRING);

  CU_ASSERT(doc != NULL);
  CU_ASSERT(doc->rootNode != NULL);
  CU_ASSERT(doc->rootNode->type == SCSSTYPE_STYLESHEET);
  CU_ASSERT(doc->rootNode->child != NULL);
  CU_ASSERT(doc->rootNode->child->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->name, "<sentinel>") == 0);
  CU_ASSERT(doc->rootNode->child->next != NULL);
  CU_ASSERT(doc->rootNode->child->next->type == SCSSTYPE_ATKEYWORD);
  CU_ASSERT(doc->rootNode->child->next->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->name, "@import") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value1, "") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value2, "all") == 0);
  CU_ASSERT(doc->rootNode->child->next->next == doc->rootNode->child);
  CU_ASSERT(doc->rootNode->child->next->ref == &doc->rootNode->child->next);
  CU_ASSERT(doc->rootNode->child->next->child == NULL);


  apr_terminate();
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
void test_import_002()
{
#define TEST_STRING "@import@import"
  SCSSDocPtr_t doc;
  apr_pool_t *pool;

  fprintf(stderr, "start %s\n", __func__);
  apr_initialize();
  apr_pool_create(&pool, NULL);

  doc = scss_parser(pool,  TEST_STRING);

  CU_ASSERT(doc != NULL);
  CU_ASSERT(doc->rootNode != NULL);
  CU_ASSERT(doc->rootNode->type == SCSSTYPE_STYLESHEET);
  CU_ASSERT(doc->rootNode->child != NULL);
  CU_ASSERT(doc->rootNode->child->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->name, "<sentinel>") == 0);
  CU_ASSERT(doc->rootNode->child->next != NULL);
  CU_ASSERT(doc->rootNode->child->next->type == SCSSTYPE_ATKEYWORD);
  CU_ASSERT(doc->rootNode->child->next->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->name, "@import@import") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value1, "") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value2, "") == 0);
  CU_ASSERT(doc->rootNode->child->next->next == doc->rootNode->child);
  CU_ASSERT(doc->rootNode->child->next->ref == &doc->rootNode->child->next);
  CU_ASSERT(doc->rootNode->child->next->child == NULL);


  apr_terminate();
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
void test_import_003()
{
#define TEST_STRING "@import;@import"
  SCSSDocPtr_t doc;
  apr_pool_t *pool;

  fprintf(stderr, "start %s\n", __func__);
  apr_initialize();
  apr_pool_create(&pool, NULL);

  doc = scss_parser(pool,  TEST_STRING);

  CU_ASSERT(doc != NULL);
  CU_ASSERT(doc->rootNode != NULL);
  CU_ASSERT(doc->rootNode->type == SCSSTYPE_STYLESHEET);
  CU_ASSERT(doc->rootNode->child != NULL);
  CU_ASSERT(doc->rootNode->child->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->name, "<sentinel>") == 0);
  CU_ASSERT(doc->rootNode->child->next != NULL);
  CU_ASSERT(doc->rootNode->child->next->type == SCSSTYPE_ATKEYWORD);
  CU_ASSERT(doc->rootNode->child->next->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->name, "@import") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value1, "") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->value2, "all") == 0);
  CU_ASSERT(doc->rootNode->child->next->next != NULL);
  CU_ASSERT(doc->rootNode->child->next->next->type == SCSSTYPE_ATKEYWORD);
  CU_ASSERT(doc->rootNode->child->next->next->name != NULL);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->next->name, "@import") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->next->value1, "") == 0);
  CU_ASSERT(strcasecmp(doc->rootNode->child->next->next->value2, "all") == 0);


  apr_terminate();
  fprintf(stderr, "end %s\n", __func__);
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
