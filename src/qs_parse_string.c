/*
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 * Copyright (C) 2005 Atsushi Konno All rights reserved.
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
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "httpd.h"
#include "http_log.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

#include "mod_chxj.h"

#if defined(HAVE_LIBICONV_HOOK)
#  include "iconv_hook/iconv.h"
#else
#  if defined(HAVE_LIBICONV)
#    include "iconv.h"
#  else
#    error "Please install libiconv or libiconv_hook. and Please set LD_LIBRARY_PATH."
#  endif
#endif

static int s_cut_tag (const char* s, int len) ;
static int s_cut_text(const char* s, int len) ;
static void qs_dump_node(Doc* doc, Node* node, int indent);


Node*
qs_parse_string(Doc* doc, const char* src, int srclen) 
{
  int ii;
  char encoding[256];
  char* osrc = NULL;
  char* ibuf = NULL;
  int olen;
  int ilen;
  iconv_t cd;

  memset(encoding, 0, 256);

  doc->now_parent_node = qs_init_root_node(doc);
  /* 
   * It is the pre reading. 
   * Because I want to specify encoding.
   */
  for (ii=0; ii<srclen; ii++) {
    if (is_white_space(src[ii]))
      continue;

    if ((unsigned char)'<' == src[ii]) {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
      node = qs_parse_tag(doc, &src[ii], endpoint);
      ii += endpoint;

      if (node->name[0] != '?') break; 

      if (strcasecmp(node->name, "?xml") == 0) {
        Attr* parse_attr;
        for(parse_attr = node->attr;
            parse_attr && *encoding == '\0'; 
            parse_attr = parse_attr->next) {
          if ((*parse_attr->name == 'e' || *parse_attr->name == 'E')
          &&   strcasecmp(parse_attr->name, "encoding") == 0) {
            switch (*parse_attr->value) {
            case 'S':
            case 's':
              if ((strcasecmp(parse_attr->value, "Shift_JIS") == 0)
              ||  (strcasecmp(parse_attr->value, "sjis"     ) == 0)
              ||  (strcasecmp(parse_attr->value, "Shift-JIS") == 0)
              ||  (strcasecmp(parse_attr->value, "x-sjis"   ) == 0)) {
                strcpy((char*)encoding, (char*)"NONE");
              }
              break;

            case 'e':
            case 'E':
              if ((strcasecmp(parse_attr->value, "EUC_JP") == 0)
              ||  (strcasecmp(parse_attr->value, "EUC-JP") == 0)
              ||  (strcasecmp(parse_attr->value, "EUCJP" ) == 0)) {
                strcpy((char*)encoding, "EUC-JP");
              }
              break;

            case 'u':
            case 'U':
              if ((strcasecmp(parse_attr->value, "UTF-8") == 0)
              ||  (strcasecmp(parse_attr->value, "UTF8") == 0)) {
                strcpy((char*)encoding, "UTF-8");
              }
              break;

            default:
              strcpy((char*)encoding, "NONE");
              break;
            }
          }
        }
        break;
      }
      break;
    }
  }

  if (strcasecmp(encoding, "NONE") != 0 && strlen(encoding) != 0) {
    char* sv_osrc;
    olen = srclen * 4 + 1;
    sv_osrc = osrc =(char*)apr_palloc(doc->pool, olen);
    memset((char*)osrc, 0, olen);
    if ((cd = iconv_open("CP932", encoding)) != (iconv_t) -1) {
      ilen = srclen;
      ibuf = apr_palloc(doc->pool, ilen+1);
      memset(ibuf, 0, ilen+1);
      memcpy(ibuf, src, ilen);
      while (ilen > 0) {
        size_t result = iconv(cd, &ibuf, (size_t*)&ilen, &osrc, (size_t*)&olen);
        if (result == (size_t)(-1)) {
          break;
        }
      }
      srclen = olen;
      src = sv_osrc;
      iconv_close(cd);
    }
  }

  /*
   * Now, true parsing is done here. 
   */
  for (ii=0; ii<srclen; ii++) {
    if (doc->parse_mode != PARSE_MODE_NO_PARSE 
    && is_white_space(src[ii])) {
      continue;
    }
    if ((unsigned char)'<' == src[ii]) {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
      node = qs_parse_tag(doc, &src[ii], endpoint);

      ii += endpoint;
      if (node->name[0] == '/' ) {

        if ((doc->parse_mode == PARSE_MODE_CHTML && has_child(&(node->name[1])))
        ||  (doc->parse_mode == PARSE_MODE_NO_PARSE 
        &&  (node->name[1] == 'c' || node->name[1] == 'C')
        &&  strcasecmp(&node->name[1], "chxj:if") == 0)) {
          if (doc->now_parent_node->parent != NULL) {
            doc->now_parent_node = doc->now_parent_node->parent;
            doc->parse_mode = PARSE_MODE_CHTML;
          }
        }

        if (doc->parse_mode != PARSE_MODE_NO_PARSE)
          continue;
      }
      if (*node->name == '!' && strncmp(node->name, "!--", 3) == 0) {
        /* comment tag */
        continue;
      }
      qs_add_child_node(doc,node);

      if (doc->parse_mode == PARSE_MODE_NO_PARSE) {
        if (node->name[0] == '/')
          continue;
      }
#ifdef DEBUG
  QX_LOGGER_DEBUG("return from qs_add_child_node()");
#endif
      if (doc->parse_mode == PARSE_MODE_CHTML && 
      (*node->name == 'c' || *node->name == 'C') &&
      strcasecmp(node->name, "chxj:if") == 0) {
        Attr* parse_attr;

        doc->parse_mode = PARSE_MODE_NO_PARSE;
        doc->now_parent_node = node;
        for(parse_attr = node->attr;
            parse_attr; 
            parse_attr = parse_attr->next) {
          if ((*parse_attr->name == 'p' || *parse_attr->name == 'P') 
          &&   strcasecmp(parse_attr->name, "parse") == 0) {
            if ((*parse_attr->value == 't' || *parse_attr->value == 'T')
            &&   strcasecmp(parse_attr->value, "true") == 0) {
              doc->parse_mode = PARSE_MODE_CHTML;
            }
          }
        }

      }
      if (doc->parse_mode == PARSE_MODE_CHTML && has_child(node->name)) {
        doc->now_parent_node = node;
      }
    }
    else {
      /* TEXT */
      int endpoint = s_cut_text(&src[ii], srclen - ii);
      Node* node = qs_new_tag(doc);
      node->value = (char*)apr_palloc(doc->pool,endpoint+1);
      node->name  = (char*)apr_palloc(doc->pool,4+1);
      node->otext = (char*)apr_palloc(doc->pool,endpoint+1);
      node->size  = endpoint;
      memset(node->value, 0, endpoint+1);
      memset(node->otext, 0, endpoint+1);
      memset(node->name,  0, 4+1       );
      memcpy(node->value, &src[ii], endpoint);
      memcpy(node->name,  "text",   4);
      memcpy(node->otext,node->value, endpoint);

      qs_add_child_node(doc,node);
      ii += (endpoint - 1);
    }
  }
#ifdef DEBUG
  QX_LOGGER_DEBUG("parse_string end");
#endif
#ifdef DEBUG
  if (doc->r != NULL) {
    qs_dump_node(doc, doc->root_node, 0);
  }
#endif

  return doc->root_node;
}

static void
qs_dump_node(Doc* doc, Node* node, int indent) 
{
  Node* p = (Node*)qs_get_child_node(doc,node);

  for (;p;p = (Node*)qs_get_next_node(doc,p)) {
    Attr* attr;
    if ((char*)qs_get_node_value(doc,p) != NULL) {
      DBG5(doc->r,"%*.*sNode:[%s][%s]\n", indent,indent," ",
                      (char*)qs_get_node_name(doc,p),
                      (char*)qs_get_node_value(doc,p));
    }
    else {
      DBG4(doc->r,"%*.*sNode:[%s]\n", indent,indent," ", qs_get_node_name(doc,p));
    }
    for (attr = (Attr*)qs_get_attr(doc,p); attr; attr = (Attr*)qs_get_next_attr(doc,attr)) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*s  ATTR:[%s]\n", indent,indent," ", (char *)qs_get_attr_name(doc,attr));
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*s  VAL :[%s]\n", indent,indent," ", (char *)qs_get_attr_value(doc,attr));
    }
    qs_dump_node(doc,p, indent+4);
  }
}



static int
s_cut_tag(const char* s, int len) 
{
  int lv = 0;
  int ii;

  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) 
      continue;

    if (is_white_space(s[ii])) 
      continue;

    if (s[ii] == '<') {
      lv++;
      continue;
    }

    if (s[ii] == '>') {
      lv--;
      if (lv == 0) 
        break;
      continue;
    }
  }
  return ii;
}

static int
s_cut_text(const char* s, int len) 
{
  int ii;

  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) 
      continue;

    if (is_white_space(s[ii])) 
      continue;

    if (s[ii] == '<') 
      break;

  }

  return ii;
}


Node*
qs_init_root_node(Doc* doc) 
{
  doc->root_node = (Node*)apr_palloc(doc->pool,sizeof(struct _node));
  if (doc->root_node == NULL) 
    QX_LOGGER_FATAL("Out Of Memory");

  doc->root_node->next   = NULL;
  doc->root_node->parent = NULL;
  doc->root_node->child  = NULL;
  doc->root_node->attr   = NULL;

  doc->root_node->name   = (char*)apr_palloc(doc->pool,5);
  if (doc->root_node->name == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  memset(doc->root_node->name, 0, 5);
  strcpy(doc->root_node->name, "ROOT");

  return doc->root_node;
}



void
qs_add_child_node(Doc* doc,Node* node) 
{
  node->next       = NULL;
  node->child      = NULL;
  node->child_tail = NULL;
  node->parent = doc->now_parent_node;
  if (doc->now_parent_node->child == NULL) {
    doc->now_parent_node->child      = node;
    doc->now_parent_node->child_tail = node;
  }
  else {
#ifdef DEBUG
    QX_LOGGER_DEBUG("search child free node");
#endif
    doc->now_parent_node->child_tail->next = node;
    doc->now_parent_node->child_tail       = node;
  }
}




Node*
qs_get_root(Doc* doc) {
  return doc->root_node;
}




char* 
qs_get_node_value(Doc* doc, Node* node) {
  return node->value;
}




char*
qs_get_node_name(Doc* doc, Node* node) {
  return node->name;
}



Node*
qs_get_child_node(Doc* doc, Node* node) {
  return node->child;
}




Node*
qs_get_next_node(Doc* doc, Node* node) {
  return node->next;
}



Attr*
qs_get_attr(Doc* doc, Node* node) {
  return node->attr;
}




Attr*
qs_get_next_attr(Doc* doc, Attr* attr) {
  return attr->next;
}



char*
qs_get_attr_name(Doc* doc, Attr* attr) {
  return attr->name;
}



char*
qs_get_attr_value(Doc* doc, Attr* attr) {
  return attr->value;
}

int 
qs_get_node_size(Doc* doc, Node* node) {
  return node->size;
}
/*
 * vim:ts=2 et
 */
