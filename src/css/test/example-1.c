
#include "scss.h"
#include "apr.h"
#include "apr_pools.h"


int
main(int argc, char *argv[])
{
  SCSSDocPtr_t doc;
  char *s = "@import url(aaa.css);";

  apr_initialize();
  doc = scss_parser(NULL,  s);

  scss_dump_nodes(doc);

  apr_terminate();
  return 0;
}
