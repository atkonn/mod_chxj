
#include "scss.h"
#include "apr.h"
#include "apr_pools.h"
#include <stdio.h>

static void s_import(SCSSParserPtr_t parser, const char *uri, const char **media, const char *defaultNamespaceURI);

static void s_start_document(SCSSParserPtr_t parser);
static void s_end_document(SCSSParserPtr_t parser);
static void s_start_media(SCSSParserPtr_t parser, const char **media);
static void s_end_media(SCSSParserPtr_t parser, const char **media);
static void s_start_page(SCSSParserPtr_t parser, const char *name, const char *pseudoPage);
static void s_end_page(SCSSParserPtr_t parser, const char *name, const char *pseudoPage);
static void s_start_font_face(SCSSParserPtr_t parser);
static void s_end_font_face(SCSSParserPtr_t parser);
static void s_start_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
static void s_end_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
static void s_property(SCSSParserPtr_t parser, const char *propertyName, const char *value, int important);

static int level = 0;
int
main(int argc, char *argv[])
{
  SCSSParserPtr_t parser;
  SCSSSACHandlerPtr_t handler;
  char *s = "@import url(aaa.css) handheld, tty, all;\n"
            "@media handheld, print {\n"
            "}\n"
            "@media all {\n"
            " br+a { color: #ff0000; }\n"
            " br,a { color: #ff0000; }\n"
            "}\n"
            "@page { margin-top: 2ch; }\n"
            "@page abc { margin-top: 2ch; }\n"
            "@font-face { margin-top: 2ch; }\n"
            "a,br { margin-top: 2ch; }\n"
            "br { margin-top: 2ch; }\n"
            "hr > center { margin-top: 2ch; }\n";

  /* 1) APR initialize */
  apr_initialize();

  /* 2) create parser object */
  parser = scss_parser_new_from_buf(NULL, s, "");

  /* 3) create handler object */
  handler = scss_doc_handler_new(parser);

  /* 4) register handler */
  handler->startDocument = s_start_document;
  handler->endDocument   = s_end_document;
  handler->import        = s_import;
  handler->startMedia    = s_start_media;
  handler->endMedia      = s_end_media;
  handler->startPage     = s_start_page;
  handler->endPage       = s_end_page;
  handler->startFontFace = s_start_font_face;
  handler->endFontFace   = s_end_font_face;
  handler->startSelector = s_start_selector;
  handler->endSelector   = s_end_selector;
  handler->property      = s_property;

  /* 5) do parse. */
  scss_parse_stylesheet(parser);

  /* 6) APR terminate */
  apr_terminate();
  return 0;
}

static void print_level()
{
  int ii;
  for (ii=0; ii<level;ii++) fprintf(stderr, "+");
}

static void 
s_import(SCSSParserPtr_t parser, const char *uri, const char **media, const char *defaultNamespaceURI) 
{
  int ii;
  print_level(); fprintf(stderr, "IMPORT\n");
  print_level(); fprintf(stderr, "\turi:[%s]\n", uri);
  for (ii=0; ii < MEDIA_TYPE_NUM; ii++) {
    if (media[ii]) {
      print_level(); fprintf(stderr, "\tmedia:[%s]\n", media[ii]);
    }
  }
}


static void
s_start_document(SCSSParserPtr_t parser)
{
  fprintf(stderr, "START DOCUMENT\n");
  level++;
}


static void
s_end_document(SCSSParserPtr_t parser)
{
  level--;
  fprintf(stderr, "END DOCUMENT\n");
}



static void
s_start_media(SCSSParserPtr_t parser, const char **media)
{
  int ii;
  print_level(); fprintf(stderr, "START MEDIA\n");
  for (ii=0; ii < MEDIA_TYPE_NUM; ii++) {
    if (media[ii]) {
      print_level(); fprintf(stderr, "\tmedia:[%s]\n", media[ii]);
    }
  }
  level++;
}

static void
s_end_media(SCSSParserPtr_t parser, const char **media)
{
  int ii;
  level--;
  print_level(); fprintf(stderr, "END MEDIA\n");
  for (ii=0; ii < MEDIA_TYPE_NUM; ii++) {
    if (media[ii]) {
      print_level(); fprintf(stderr, "\tmedia:[%s]\n", media[ii]);
    }
  }
}



static void
s_start_page(SCSSParserPtr_t parser, const char *name, const char *pseudoPage)
{
  print_level(); fprintf(stderr, "START PAGE\n");
  print_level(); fprintf(stderr, "\tname:[%s]\n", name);
  print_level(); fprintf(stderr, "\tpseudo:[%s]\n", pseudoPage);
  level++;
}

static void
s_end_page(SCSSParserPtr_t parser, const char *name, const char *pseudoPage)
{
  level--;
  print_level(); fprintf(stderr, "END PAGE\n");
  print_level(); fprintf(stderr, "\tname:[%s]\n", name);
  print_level(); fprintf(stderr, "\tpseudo:[%s]\n", pseudoPage);
}

static void
s_start_font_face(SCSSParserPtr_t parser)
{
  print_level(); fprintf(stderr, "START FONT-PAGE\n");
  ++level;
}
static void
s_end_font_face(SCSSParserPtr_t parser)
{
  --level;
  print_level(); fprintf(stderr, "START FONT-PAGE\n");
}

static void
s_start_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors)
{
  SCSSNodePtr_t curNode;
  print_level(); fprintf(stderr, "START SELECTOR\n");
  for (curNode = selectors->next; curNode != selectors; curNode = curNode->next) {
    print_level(); fprintf(stderr, "\tname:[%s]\n", curNode->name);
  }
  level++; 
}
static void
s_end_selector(SCSSParserPtr_t parser, SCSSNodePtr_t selectors)
{
  SCSSNodePtr_t curNode;
  level--;
  print_level(); fprintf(stderr, "END SELECTOR\n");
  for (curNode = selectors->next; curNode != selectors; curNode = curNode->next) {
    print_level(); fprintf(stderr, "\tname:[%s]\n", curNode->name);
  }
}

static void
s_property(SCSSParserPtr_t parser, const char *propertyName, const char *value, int important)
{
  print_level(); fprintf(stderr, "PROPERTY\n");
  print_level(); fprintf(stderr, "\tname:[%s]\n", propertyName);
  print_level(); fprintf(stderr, "\tvalue:[%s]\n", value);
  print_level(); fprintf(stderr, "\timportant:[%d]\n", important);
}
#if 0
typedef void (*SCSSSAC_startSelector_fn)(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
typedef void (*SCSSSAC_endSelector_fn)(SCSSParserPtr_t parser, SCSSNodePtr_t selectors);
typedef void (*SCSSSAC_property_fn)(SCSSParserPtr_t parser, const char *propertyName, const char *value, int impotant);
#endif
