/*
 * Copyright (C) 2005-2011 Atsushi Konno All rights reserved.
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
#include "chxj_tag_util.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"
#include "chxj_jreserved_tag.h"
#include "mod_chxj.h"


/**
 * The value of the VALUE attribute that the object tag node maintains is
 * acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param node [i] The tag node to be scanned is specified.
 * @param pool [i] To use POOL.
 * @return The value of the VALUE attribute that the object tag node maintains
 *         is returned. NULL is returned when not found.
 */
char *
qs_get_value_attr(Doc *doc, Node *node, apr_pool_t *pool)
{
  Attr *attr;

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('v','V',"value",name)) {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(pool, value);
    }
  }
  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}


/**
 * The value of the checked tag is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param pool [i] To use POOL(unused).
 * @return The value of the checked tag is returned. NULL is returned when
 *         not found.
 */
char *
qs_get_checked_attr(Doc *doc, Node *tag, apr_pool_t *UNUSED(pool))
{
  Attr *attr;
  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr != NULL;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    if (STRCASEEQ('c','C',"checked",name)) {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return name;
    }
  }
  /*------------------------------------------------------------------------*/
  /* not found                                                              */
  /*------------------------------------------------------------------------*/
  return NULL;
}


/**
 * The value of the type attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the type attribute is returned. NULL is returned when
 *         not found.
 */
char *
qs_get_type_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr != NULL;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(pool, value);
    }
  }
  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}


/**
 * The character string area in 0 bytes is allocated.
 *
 * @param pool    [i]   To use POOL.
 * @return The allocated 0 byte character string is returned.
 */
char *
qs_alloc_zero_byte_string(apr_pool_t *pool)
{
  char *tgt;

  if (! pool) {
    return NULL;
  }
  tgt = apr_palloc(pool, 1);
  tgt[0] = '\0';

  return tgt;
}


/**
 * A consecutive head and the last WHITESPACE are removed.
 *
 * @param p    [i]   To use POOL
 * @param s    [i]   The character string that should be removed is specified.
 * @return The character string that has been removed is returned.
 */
char *
qs_trim_string(apr_pool_t *p, char *s)
{
  char *ss;
  int len;
  int ii;

  if (! s) return apr_pstrdup(p, "");

  ss = apr_pstrdup(p, s);
  len = strlen(s);
  ii = 0;
  for (ii = 0;is_white_space(*ss) && ii < len; ss++, ii++);

  ii = strlen(ss);
  for(;is_white_space(ss[ii-1]) && ii > 0; ii--);

  ss[ii] = '\0';

  return ss;
}


/**
 * The value of child TEXT of tag that maintains the SELECTED attribute is 
 * returned. 
 *
 * @param Doc  [i] The pointer to the Doc structure to be scanned is 
 *                 specified. 
 * @param node [i] The tag node to be scanned is specified.
 * @param pool [i] To use POOL.
 * @reutrn  The value of child TEXT of tag that maintains the SELECTED 
 *          attribute is returned. NULL is returned when not found. 
 */
char *
qs_get_selected_value_text(Doc *doc, Node *node, apr_pool_t *pool)
{
  Node *child;
  Node *selchild;
  char *result   = NULL;

  for (child = qs_get_child_node(doc,node);
       child != NULL; 
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    /*------------------------------------------------------------------------*/
    /* <OPTION> tag                                                           */
    /*------------------------------------------------------------------------*/
    if (STRCASEEQ('o','O',"option",name)) {
      Attr *attr;
      for (attr =  qs_get_attr(doc,child); 
           attr != NULL; 
           attr = qs_get_next_attr(doc,attr)) {
        char *name2  = qs_get_attr_name(doc,attr);
        if (STRCASEEQ('s','S',"selected",name2)) {
          /*------------------------------------------------------------------*/
          /* SELECTED Value Found                                             */
          /*------------------------------------------------------------------*/
          selchild = qs_get_child_node(doc, child);
          if (! selchild) {
            /* void value */
            return apr_pstrdup(pool, "");
          }
          return qs_get_node_value(doc, selchild);
        }
      }
    }

    if ((result = qs_get_selected_value_text(doc, child, pool)) != NULL) {
      return result;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}


/**
 * The value of tag that maintains the SELECTED attribute is acquired. 
 *
 * @param doc    [i]  The pointer to the Doc structure to be scanned is 
 *                    specified. 
 * @param node   [i]  The SELECT tag node is specified.
 * @param pool   [i] To use POOL.
 * @return The value of tag that maintains the SELECTED attribute is 
 *         returned. NULL is returned when not found. 
 */
char *
qs_get_selected_value(Doc *doc, Node *node, apr_pool_t *pool)
{
  Node *child;
  char *result    = NULL;

  for (child = qs_get_child_node(doc,node); 
       child != NULL; 
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    /*------------------------------------------------------------------------*/
    /* <OPTION> tag                                                           */
    /*------------------------------------------------------------------------*/
    if (STRCASEEQ('o','O',"option",name)) {
      Attr *attr;
      for (attr = qs_get_attr(doc,child); 
           attr; 
           attr = qs_get_next_attr(doc,attr)) {
        char *name2  = qs_get_attr_name(doc,attr);
        if (STRCASEEQ('s','S',"selected",name2)) {
          /*------------------------------------------------------------------*/
          /* SELECTED Value Found                                             */
          /*------------------------------------------------------------------*/
          return qs_get_value_attr(doc, child, doc->buf.pool);
        }
      }
    }

    if ((result = qs_get_selected_value(doc, child, pool)) != NULL) {
      return result;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}


/**
 * The value of the NAME attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the SIZE attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the NAME attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_name_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name)) {
      return apr_pstrdup(pool, (value ? value : ""));
    }
  }
  return NULL;
}


/**
 * The value of the SIZE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the SIZE attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the SIZE attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_size_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"size",name)) {
      return apr_pstrdup(pool, (value ? value : ""));
    }
  }
  return NULL;
}


/**
 * The value of the ACCESSKEY attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the ACCESSKEY attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the ACCESSKEY attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_accesskey_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"accesskey",name)) {
      return apr_pstrdup(pool, value);
    }
  }
  return NULL;
}


/**
 * The value of the ISTYLE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the ISTYLE attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the ISTYLE attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_istyle_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag); 
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('i','I',"istyle",name)) {
      return apr_pstrdup(pool, value);
    }
  }
  return NULL;
}


/**
 * The value of the MAXLENGTH attribute is acquired from the tag node of the
 * object.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the MAXLENGTH attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the MAXLENGTH attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_maxlength_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('m','M',"maxlength",name)) {
      return apr_pstrdup(pool, value);
    }
  }
  return NULL;
}


/**
 * It is scanned whether the CHECKBOX tag of the object is CHECKED. 
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the CHECKBOX attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return 1 is returned when it is CHECKED and, additionally, 0 is returned. 
 */
int
qs_is_checked_checkbox_attr(Doc *doc, Node *tag, apr_pool_t *UNUSED(pool))
{
  Attr *attr;
  for (attr = qs_get_attr(doc,tag);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    if (STRCASEEQ('c','C',"checked",name)) {
      return 1;
    }
  }
  return 0;
}


int
chxj_chxjif_is_mine(device_table *spec, Doc *doc, Node *tag)
{
  request_rec *r = doc->r;
  Attr        *attr;

  for (attr = qs_get_attr(doc,tag);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if ((*name == 'l' || *name == 'L') && strcasecmp(name, "lang") == 0) {

      DBG(r,"REQ[%X] lang found [%s] spec [%d]", TO_ADDR(r),value, spec->html_spec_type);
      if (STRCASEEQ('a','A',"all",value)) {
        return 1;
      }
      else if (STRCASEEQ('x','X',"xhtml",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('h','H',"hdml",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_Hdml) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('j','J',"jhtml",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_Jhtml) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('j','J',"jxhtml",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_Jxhtml) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('i','I',"iphone2",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_iPhone2) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('i','I',"iphone3",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_iPhone3) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('i','I',"iphone4",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_iPhone4) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('i','I',"iphone",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_iPhone2
            || spec->html_spec_type == CHXJ_SPEC_iPhone3
            || spec->html_spec_type == CHXJ_SPEC_iPhone4) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('c','C',"chtml",value)) {
        switch (spec->html_spec_type) {
        case CHXJ_SPEC_Chtml_1_0:
        case CHXJ_SPEC_Chtml_2_0:
        case CHXJ_SPEC_Chtml_3_0:
        case CHXJ_SPEC_Chtml_4_0:
        case CHXJ_SPEC_Chtml_5_0:
          return 1;
        default:
          break;
        }
      }
      else if (STRCASEEQ('i','I',"ixhtml",value)) {
        switch (spec->html_spec_type) {
        case CHXJ_SPEC_Chtml_6_0:
        case CHXJ_SPEC_Chtml_7_0:
          return 1;
        default:
          break;
        }
      }
      else if (STRCASEEQ('a','A',"android_s",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_softbank_android) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('a','A',"android_a",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_au_android) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('a','A',"android_d",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_docomo_android) {
          /* Yes , it is mine */
          return 1;
        }
      }
      else if (STRCASEEQ('a','A',"android",value)) {
        if (spec->html_spec_type == CHXJ_SPEC_softbank_android
            || spec->html_spec_type == CHXJ_SPEC_au_android
            || spec->html_spec_type == CHXJ_SPEC_docomo_android
            || spec->html_spec_type == CHXJ_SPEC_android) {
          /* Yes , it is mine */
          return 1;
        }
      }
    }
  }

  /* No, it is not mine. */
  return 0;
}


/**
 * The value of the DESTLANG attribute is acquired from the tag node of the
 * object.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the DESTLANG attribute
 *                 is specified.
 * @param pool [i] To use POOL.
 * @return The value of the DESTLANG attribute is returned. NULL is
 *         returned when not is.
 */
char *
qs_get_destlang_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr  *attr;
  for (attr = qs_get_attr(doc,tag);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('d','D',"destlang",name)) {
      return apr_pstrdup(pool, value);
    }
  }

  return NULL;
}


/**
 * The value of the PARSE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param pool [i] To use POOL.
 * @return The value of the PARSE attribute is returned. NULL is returned when
 *         not found.
 */
char *
qs_get_parse_attr(Doc *doc, Node *tag, apr_pool_t *pool)
{
  Attr *attr;
  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('p','P',"parse",name)) {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(pool, value);
    }
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}


char *
chxj_form_action_to_hidden_tag(
  request_rec         *r,
  apr_pool_t          *pool,
  const char          *str,
  int                 xmlFlag,
  int                 post,
  char                **new_query_string,
  int                 docomo,
  int                 softbank,
  chxjconvrule_entry  *entryp)
{
  char *s = apr_pstrdup(pool, str);
  *new_query_string = NULL;
  int no_qsconv_flag = (((unsigned int)entryp->action & CONVRULE_QSCONV_OFF_BIT) != 0);
  if (!s) return NULL;
  if (chxj_starts_with(s, "http://") || chxj_starts_with(s, "https://")) {
    apr_uri_t url;
    apr_uri_parse(pool, s, &url);
    if (url.hostname && strcasecmp(url.hostname, r->hostname) != 0) {
      return NULL;
    }
  }
  s = strchr(s, '?');
  if (!s) return NULL;
  s++;
  char *result = NULL;

  char *pstat;
  char *pstat2;
  for (;;) {
    char *pair = apr_strtok(s, "&", &pstat);
    if (! pair) break;
    if (strncasecmp(pair, "amp;", 4) == 0) {
      pair += 4;
    }
    s = NULL;
    char *key = apr_strtok(pair, "=",  &pstat2);
    char *val = "";
    if (key) {
      val = apr_strtok(NULL, "=", &pstat2);
      if (!val) val = "";
    }
    char *tmp = NULL;
    
    if (no_qsconv_flag) {
      if (*new_query_string) {
        *new_query_string = apr_psprintf(pool, "%s&%s=%s", *new_query_string, chxj_jreserved_to_safe_tag(r, key, entryp), val);
      }
      else {
        *new_query_string = apr_psprintf(pool, "%s=%s", chxj_jreserved_to_safe_tag(r, key, entryp), val);
      }
    }
    else {
      if (post && strcasecmp(key, "guid") == 0 && docomo) {
        *new_query_string = apr_psprintf(pool, "%s=%s", key, val);
      }
      else {
        if (! post || strcasecmp(key, "_chxj_cc") == 0 || strcasecmp(key, "_chxj_nc") == 0) {
          if (softbank) {
            tmp = apr_psprintf(pool, 
                               "<input type=\"hidden\" name=\"%s\" value=\"%s\"%s>", 
                               chxj_jreserved_to_safe_tag(r, 
                                                          chxj_url_decode(pool, key), 
                                                          entryp), 
                               chxj_url_decode(pool, val), 
                               (xmlFlag == 1) ? " /" : "");
          }
          else {
            tmp = apr_psprintf(pool, 
                               "<input type=\"hidden\" name=\"%s\" value=\"%s\"%s>", 
                               chxj_url_decode(pool, key), 
                               chxj_url_decode(pool, val), 
                               (xmlFlag == 1) ? " /" : "");
          }
        }
        else {
          if (entryp->action & CONVRULE_JRCONV_OFF_BIT) {
            if (chxj_is_jreserved_tag(key)) {
              tmp = apr_psprintf(pool, 
                                 "<input type=\"hidden\" name=\"%s\" value=\"%s\"%s>", 
                                 chxj_url_decode(pool, key), 
                                 chxj_url_decode(pool, val), 
                                 (xmlFlag == 1) ? " /" : "");
            }
            else {
              tmp = apr_psprintf(pool, 
                                 "<input type=\"hidden\" name=\"_chxj_qs_%s\" value=\"%s\"%s>", 
                                 chxj_url_decode(pool, key), 
                                 chxj_url_decode(pool, val), 
                                 (xmlFlag == 1) ? " /" : "");
            }
          }
          else {
            tmp = apr_psprintf(pool, 
                               "<input type=\"hidden\" name=\"_chxj_qs_%s\" value=\"%s\"%s>", 
                               chxj_url_decode(pool, key), 
                               chxj_url_decode(pool, val), 
                               (xmlFlag == 1) ? " /" : "");
          }
        }
        if (result) {
          result = apr_pstrcat(pool, result, tmp, NULL);
        }
        else {
          result = tmp;
        }
      }
    }
  }
  return result;
}


char *
chxj_delete_chxj_cc_param(
  request_rec         *r,
  const char          *str)
{
  apr_pool_t  *pool;
  char        *s;
  int         xmlFlag = 0;

  DBG(r,"REQ[%X] start %s()",TO_ADDR(r),__func__);
  DBG(r,"REQ[%X] str:[%s]", TO_ADDR(r),str);

  apr_pool_create(&pool, r->pool);
  s = apr_pstrdup(pool, str);
  if (!s) {
    ERR(r,"REQ[%X] Memory Allocation Error", TO_ADDR(r));
    DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
    return NULL;
  }
  char *result = NULL;

  char *pstat;
  char *pstat2;
  for (;;) {
    char *pair = apr_strtok(s, "&", &pstat);
    if (! pair) break;
    if (strncasecmp(pair, "amp;", 4) == 0) {
      pair += 4;
      xmlFlag = 1;
    }
    s = NULL;
    char *key = apr_strtok(pair, "=",  &pstat2);
    char *val = "";
    if (key) {
      val = apr_strtok(NULL, "=", &pstat2);
      if (!val) val = "";
    }
    if (strcasecmp(key, "_chxj_cc")) {
      if (result) {
        result = apr_pstrcat(pool, result, ((xmlFlag) ? "&amp;" : "&"), key, "=", val, NULL);
      }
      else {
        result = apr_pstrcat(pool, key, "=", val, NULL);
      }
    }
  }
  DBG(r, "REQ[%X] result:[%s]", TO_ADDR(r), result);
  DBG(r,"REQ[%X] end %s()",TO_ADDR(r),__func__);
  return result;
}
/*
 * vim:ts=2 et
 */
