#include "scss.h"
#include "scss_string.h"
#include "apr_pools.h"

#include <stdio.h>
/*
 * stylesheet
 *   +--------> @import
 *   +--------> @media 
 *   |            +--> {
 *   |                   selector 
 *   |                      +-----> property
 *   |                   selector
 *   |                      +-----> property
 *   |                 }
 *   +---------> selector
 *   |              +-----> property
 *   +---------> selector
 *                  +-----> property
 */
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

#define PASS_COMMENT(S)                     \
    if (*(S) == '/' && *((S) + 1) == '*') { \
      (S) = s_pass_comment((S));            \
      continue;                             \
    }

#define JUDGE_STRINGS(S)  \
    if (*(S) == '\\') {   \
      (S) +=2;            \
      continue;           \
    }                     \
    if (*(S) == '"') {    \
      if (! sq) {         \
        if (! dq) dq = 1; \
        else      dq = 0; \
      }                   \
    }                     \
    if (*(S) == '\'') {   \
      if (! dq) {         \
        if (! sq) sq = 1; \
        else      sq = 0; \
      }                   \
    }                     

static SCSSDocPtr_t s_create_doc(apr_pool_t *ppool);
static SCSSNodePtr_t s_create_node(apr_pool_t *pool);
static char *s_cut_ident(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static const char *s_pass_comment(const char *s);
static char *s_cut_before_next_semicoron_or_block(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static char *s_cut_before_white_space_or_semicoron(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static char *s_cut_before_block_closer(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static char *s_cut_before_semicoron(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static char *s_trim(SCSSDocPtr_t doc, const char *s);
static void s_add_child_node(SCSSDocPtr_t doc, SCSSNodePtr_t nowNode, SCSSNodePtr_t node);
static char *s_get_one_selector(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);
static char *s_replace_refstring(SCSSDocPtr_t doc, const char *s);
static void s_get_property_list(SCSSDocPtr_t doc, SCSSNodePtr_t nowNode, const char *s);
static char *s_cut_url_function(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len);


SCSSDocPtr_t
scss_parser(apr_pool_t *ppool,  const char *src)
{
  register const char *s;
  int len;
  SCSSDocPtr_t doc;
  apr_size_t pass_len;

  doc = s_create_doc(ppool);

  len = strlen(src);
  if (len == 0) return doc;

  s = (char *)src;

  
  while (*s) {
    int pass = scss_ignore_space(src, len);
    if (pass) {
      s += pass;
      continue;
    }
    if (*s == '@') {
      SCSSNodePtr_t atnode = s_create_node(doc->pool);
      char *name;
      char *value1 = "";
      char *value2 = "";
      s++;
      atnode->type = SCSSTYPE_ATKEYWORD;
      name  = apr_psprintf(doc->pool, "@%s", s_cut_ident(doc, s, &pass_len));
      s += pass_len;
      if (strcasecmp(name, "@import") == 0) {
        value1 = s_trim(doc, s_cut_before_white_space_or_semicoron(doc, s, &pass_len));
        if (scss_starts_with(value1, "url")) {
          value1 = s_trim(doc, s_cut_url_function(doc, s, &pass_len));
          s += pass_len;
        }
        else {
          s += pass_len + 1;
        }
        if (*s != ';') {
          value2 = s_trim(doc, s_cut_before_semicoron(doc, s, &pass_len));
          s += pass_len + 1;
        }
        else {
          value2 = apr_pstrdup(doc->pool, "all");
        }
      }
      else if (strcasecmp(name, "@media") == 0) {
        value2 = s_trim(doc, s_cut_before_next_semicoron_or_block(doc, s, &pass_len));
        s += pass_len + 1;
        if (*s == '{') {
          value1 = s_trim(doc, s_cut_before_block_closer(doc, ++s, &pass_len));
          s += pass_len + 1;
          SCSSNodePtr_t selector_node = s_create_node(doc->pool);
          selector_node->type = SCSSTYPE_SELECTOR;
          selector_node->name = s_get_one_selector(doc, value1, &pass_len);
          value1 += pass_len;
          if (*value1 == '{') value1++;
          selector_node->value1 = s_trim(doc, s_cut_before_block_closer(doc, value1, &pass_len));
          s_get_property_list(doc, selector_node, selector_node->value1);
          s_add_child_node(doc, atnode, selector_node);
        }
        else {
          /* ERROR */
          value2 = "";
          value1 = "";
          name   = "";
        }
      }
      else if (strcasecmp(name, "@charset") == 0) {
        value1 = s_trim(doc, s_cut_before_semicoron(doc, s, &pass_len));
        s += pass_len + 1;
      }
      else if (strcasecmp(name, "@page") == 0) {
        value2 = s_trim(doc, s_cut_before_next_semicoron_or_block(doc, s, &pass_len));
        s += pass_len + 1;
        if (*s == '{') {
          value1 = s_trim(doc, s_cut_before_block_closer(doc, ++s, &pass_len));
          s += pass_len + 1;
        }
        else {
          /* ERROR */
          value2 = "";
          value1 = "";
          name   = "";
        }
      }
      else if (strcasecmp(name, "@font-face") == 0) {
        s_cut_before_next_semicoron_or_block(doc, s, &pass_len);
        s += pass_len + 1;
        if (*s == '{') {
          value1 = s_trim(doc, s_cut_before_block_closer(doc, ++s, &pass_len));
          s += pass_len + 1;
        }
        else {
          /* ERROR */
          value2 = "";
          value1 = "";
          name   = "";
        }
      }
      else {
        value1 = s_trim(doc, s_cut_before_next_semicoron_or_block(doc, s, &pass_len));
        s += pass_len + 1;
      }
      atnode->name = apr_pstrdup(doc->pool, name);
      atnode->value1 = apr_pstrdup(doc->pool, value1);
      atnode->value2 = apr_pstrdup(doc->pool, value2);
      s_add_child_node(doc, doc->rootNode, atnode);
    }
    else if (! is_white_space(*s)) {
      SCSSNodePtr_t selector_node = s_create_node(doc->pool);
      /* selector */
      selector_node->type = SCSSTYPE_SELECTOR;
      selector_node->name = s_get_one_selector(doc, s, &pass_len);
      s += pass_len;
      if (*s == '{') s++;
      selector_node->value1 = s_trim(doc, s_cut_before_block_closer(doc, s, &pass_len));
      s_get_property_list(doc, selector_node, selector_node->value1);
      s_add_child_node(doc, doc->rootNode, selector_node);
      s += pass_len + 1;
    }
    s++;
  }
  return doc;
}


static SCSSDocPtr_t
s_create_doc(apr_pool_t *ppool)
{
  apr_pool_t *pool;
  SCSSDocPtr_t doc;

  apr_pool_create(&pool, ppool);
  doc = apr_palloc(pool, sizeof(*doc));
  memset(doc , 0, sizeof(*doc));

  doc->pool = pool;

  doc->rootNode = apr_palloc(pool, sizeof(SCSSNode_t));

  doc->rootNode->type      = SCSSTYPE_STYLESHEET; 
  doc->rootNode->next      = doc->rootNode;
  doc->rootNode->ref       = &doc->rootNode->next;
  doc->rootNode->child     = NULL;
  doc->rootNode->name      = apr_pstrdup(pool, ROOT_NODE_SIGNATURE);
  doc->rootNode->value1    = NULL;
  doc->rootNode->value2    = NULL;

  doc->nowNode = doc->rootNode;

  return doc;
}


static SCSSNodePtr_t
s_create_node(apr_pool_t *pool)
{
  SCSSNodePtr_t node;
  node = apr_palloc(pool, sizeof(SCSSNode_t));

  node->type      = SCSSTYPE_STYLESHEET; 
  node->next      = node;
  node->ref       = &node->next;
  node->child     = NULL;
  node->name      = NULL;
  node->value1    = NULL;
  node->value2    = NULL;

  return node;
}


static char *
s_cut_ident(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *ret;
  int npos;
  while (*s) {
    PASS_COMMENT(s);
    if (is_white_space(*s)) {
      break;
    }
    s++;
  }
  
  *pass_len = s - spos;
  ret = apr_palloc(doc->pool, *pass_len + 1);
  s = spos;
  npos = 0;
  while (*s) {
    PASS_COMMENT(s);
    if (is_white_space(*s)) {
      break;
    }
    ret[npos++] = *s;
    s++;
  }
  ret[npos] = 0;
  return ret;
}


static const char *
s_pass_comment(const char *s)
{
  while(*s) {
    if (*s == '*' && *(s+1) == '/') {
      ++s;
      return ++s;
    }
    s++;
  }
  return s;
}

  

static char *
s_cut_before_next_semicoron_or_block(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *ret;
  int npos;
  int sq, dq;

  sq = dq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!sq && !dq && (*s == '{' || *s == ';')) {
      break;
    }
    s++;
  }

  *pass_len = s - spos - 1;

  ret = apr_palloc(doc->pool, *pass_len + 1);
  s = spos; 

  npos = 0;
  sq = dq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!sq && !dq && (*s == '{' || *s == ';')) {
      break;
    }
    ret[npos++] = *s++;
  }
  ret[npos] = 0;
  return ret;
}

static char *
s_cut_before_block_closer(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *ret;
  int npos;
  int nest = 0;
  int dq, sq;

  dq = sq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!sq && !dq && *s == '{') {
      nest++;
    }
    if (!sq && !dq && *s == '}') {
      if (! nest) {
        break;
      }
      nest--;
    }
    s++;
  }

  *pass_len = s - spos - 1;

  ret = apr_palloc(doc->pool, *pass_len + 1);
  s = spos; 

  npos = 0;
  dq = sq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!sq && !dq && *s == '{') {
      nest++;
    }
    if (!sq && !dq && *s == '}') {
      if (! nest) {
        break;
      }
      nest--;
    }
    ret[npos++] = *s++;
  }
  ret[npos] = 0;
  return ret;
}


static char *
s_cut_before_white_space_or_semicoron(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *ret;
  int npos;
  int sq, dq;
  while (*s) {
    PASS_COMMENT(s);
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }

  dq = sq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!sq && !dq && is_white_space(*s)) {
      break;
    }
    if (!sq && !dq && *s == ';') {
      break;
    }
    s++;
  }

  *pass_len = s - spos - 1;

  ret = apr_palloc(doc->pool, *pass_len + 1);
  s = spos; 

  npos = 0;
  dq = sq = 0;
  while (*s) {
    PASS_COMMENT(s);
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (!dq && !sq && is_white_space(*s)) {
      break;
    }
    if (!dq && !sq && *s == ';') {
      break;
    }
    ret[npos++] = *s++;
  }
  ret[npos] = 0;
  return ret;
}


static char *
s_cut_before_semicoron(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *ret;
  int npos;
  int sq = 0;
  int dq = 0;
  while (*s) {
    PASS_COMMENT(s);
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }

  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (*s == ';' && ! dq && ! sq) {
      break;
    }
    s++;
  }

  *pass_len = s - spos - 1;

  ret = apr_palloc(doc->pool, *pass_len + 1);
  s = spos; 

  npos = 0;
  while (*s) {
    PASS_COMMENT(s);
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }
  sq = dq = 0;
  while (*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (! dq && ! sq && *s == ';') {
      break;
    }
    ret[npos++] = *s++;
  }
  ret[npos] = 0;
  return ret;
}

static void 
s_add_child_node(SCSSDocPtr_t doc, SCSSNodePtr_t nowNode, SCSSNodePtr_t node)
{
  if (nowNode->child != NULL) {
    list_insert(node, nowNode->child);
  }
  else {
    SCSSNodePtr_t sentinelNode = s_create_node(doc->pool);
    nowNode->child = sentinelNode;
    list_insert(node, sentinelNode);
  }
}


static char *
s_trim(SCSSDocPtr_t doc, const char *s)
{
  char *ss = apr_pstrdup(doc->pool, s);
  int len = strlen(s);
  int ii;

  ii = 0;
  for (ii = 0;is_white_space(*ss) && ii < len; ss++, ii++);

  ii = strlen(ss);
  for(;is_white_space(ss[ii-1]) && ii > 0; ii--);

  ss[ii] = '\0';

  return ss;
}

static char *
s_get_one_selector(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  char *cand;
  char *ret;
  int npos;
  int dq = 0;
  int sq = 0;
  while(*s) {
    PASS_COMMENT(s);
    if (*s == '\\') {
      s += 2;
      continue;
    }
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }

  while(*s) {
    PASS_COMMENT(s);
    JUDGE_STRINGS(s);
    if (! sq && !dq && *s == '{') {
      break;
    }
    s++;
  }
  *pass_len = s - spos;
  cand = apr_palloc(doc->pool, *pass_len + 1);
  memcpy(cand, spos, *pass_len);
  cand[*pass_len] = 0;
  cand = s_trim(doc, cand);
  ret = apr_palloc(doc->pool, *pass_len + 1);
  spos = cand;
  npos = 0;
  while(*cand) {
    while(*cand) {
      if (*cand == ' ' || *cand == '+' || *cand == '>' || *cand == ',') {
        break;
      }
      ret[npos++] = *cand++;
    }
    int sib = 0; /* + */
    int cld = 0; /* > */
    int cnm = 0; /* , */
    while(*cand) {
      if (*cand != ' ' && *cand != '+' && *cand != '>' && *cand != ',') {
        break;
      }
      if (!cnm && !cld && *cand == '+') {
        sib++;
      }
      if (!cnm && !sib && *cand == '>') {
        cld++;
      }
      if (!cld && !sib && *cand == ',') {
        cnm++;
      }
      cand++;
    }
    if (*cand) {
      if (sib) {
        ret[npos++] = '+';
      }
      else if (cld) {
        ret[npos++] = '>';
      }
      else if (cnm) {
        ret[npos++] = ',';
      }
      else {
        ret[npos++] = ' ';
      }
    }
  }
  ret[npos] = 0;
  
  return ret;
}


static void
s_get_property_list(SCSSDocPtr_t doc, SCSSNodePtr_t nowNode, const char *s)
{
  char *block = (char *)s;
  char *pstat;
  block = s_replace_refstring(doc, block);
  while(1) {
    char *pair = apr_strtok(block, ";", &pstat);
    if (!pair) {
      break;
    }
    char *pstat2;
    char *key = apr_strtok(pair, ":", &pstat2);
    char *val = apr_strtok(NULL, ":", &pstat2);
    key = s_trim(doc, key);
    val = s_trim(doc, val);

    SCSSNodePtr_t node = s_create_node(doc->pool);
    node->name   = key;
    node->value1 = val;
    node->value2 = NULL;
    node->type   = SCSSTYPE_PROPERTY;
    s_add_child_node(doc, nowNode, node);
    block = NULL;
  }
}


static char *
s_replace_refstring(SCSSDocPtr_t doc, const char *s)
{
  char *ss;
  char *ret;
  int npos;
  apr_pool_t *pool = doc->pool;

  ss = (char *)s_trim(doc, s);
  if (! ss) return apr_pstrdup(pool, "");
  if (! *ss) return apr_pstrdup(pool, "");

  ret = apr_palloc(pool, strlen(ss) + 1);
  memset(ret, 0, strlen(ss) + 1);
  npos = 0;
  while(*ss) {
    if (*ss == '&') {
      if (strncasecmp(&ss[1], "&quot;", sizeof("&quot;")-1) == 0) {
        ret[npos++] = '"';
        ss += sizeof("&quot;")-1;
        
      }
      else if (strncasecmp(&ss[1], "&amp;", sizeof("&amp;")-1) == 0) {
        ret[npos++] = '&';
        ss += sizeof("&amp;")-1;
      }
      else if (strncasecmp(&ss[1], "&lt;", sizeof("&lt;")-1) == 0) {
        ret[npos++] = '<';
        ss += sizeof("&lt;")-1;
      }
      else if (strncasecmp(&ss[1], "&gt;", sizeof("&gt;")-1) == 0) {
        ret[npos++] = '>';
        ss += sizeof("&gt;")-1;
      }
      else {
        ret[npos++] = *ss;
        ss++;
      }
    }
    else {
      ret[npos++] = *ss;
      ss++;
    }
  }
  ret[npos] = 0;
  return ret;
}


static char *
s_cut_url_function(SCSSDocPtr_t doc, const char *s, apr_size_t *pass_len)
{
  const char *spos = s;
  int pcnt = 0;

  while(*s) {
    PASS_COMMENT(s);
    if (*s == '\\') {
      s += 2;
      continue;
    }
    if (! is_white_space(*s)) {
      break;
    }
    s++;
  }

  while(*s) {
    PASS_COMMENT(s);
    if (*s == '\\') {
      s += 2;
      continue;
    }
    if (pcnt && *s == ')') {
      pcnt--;
      if (! pcnt) {
        s++;
        break;
      }
    }
    if (*s == '(') {
      pcnt++;
    }
    s++;
  }

  *pass_len = s - spos;
  char *ret = apr_palloc(doc->pool, *pass_len + 1);
  memcpy(ret, spos, *pass_len);
  ret[*pass_len] = 0;
  return ret;
}
