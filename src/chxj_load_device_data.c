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
#include "mod_chxj.h"
#include <qs_ignore_sp.h>
#include <qs_log.h>
#include <qs_malloc.h>
#include <qs_parse_attr.h>
#include <qs_parse_file.h>
#include <qs_parse_string.h>
#include <qs_parse_tag.h>
#include "chxj_load_device_data.h"
#include "chxj_str_util.h"


static void s_set_devices_data(
  Doc              *doc, 
  apr_pool_t       *p, 
  mod_chxj_config  *conf, 
  Node             *node) ;

static void s_set_user_agent_data(
  Doc              *doc, 
  apr_pool_t       *p, 
  mod_chxj_config  *conf, 
  Node             *node);

static void s_set_device_data(
  Doc               *doc, 
  apr_pool_t        *p, 
  device_table_list *dtl, 
  Node              *node) ;

static void s_set_sort_table(Doc *doc, apr_pool_t *p, device_table_list *dtl);
static int s_sort_table_compare(const void *a, const void *b);


/**
 * load device_data.xml
 */
void
chxj_load_device_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf) 
{
  conf->devices = NULL;
  s_set_devices_data(doc, p, conf,qs_get_root(doc));
}


/**
 * <devices>
 */
static void
s_set_devices_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf, Node *node) 
{
  Node *child;

  for (child = qs_get_child_node(doc,node); 
       child ; 
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    if (STRCASEEQ('d','D',"devices",name)) {
      s_set_user_agent_data(doc, p, conf, child);
    }
  }
}

/**
 * <user_agent>
 */
static void
s_set_user_agent_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf, Node *node) 
{
  Node              *child;
  device_table_list *t;

  for (child = qs_get_child_node(doc,node);
       child ;
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    if (STRCASEEQ('u','U',"user_agent",name)) {
      Attr *attr;
      device_table_list *dtl;

      if (! conf->devices) {
        conf->devices = apr_pcalloc(p, sizeof(device_table_list));
        conf->devices->next    = NULL;
        conf->devices->pattern = NULL;
        conf->devices->table   = NULL;
        conf->devices->tail    = NULL;
        dtl = conf->devices;
      }
      else {
        for (t = conf->devices; t ; t = t->next) {
          if (! t->next)
            break;
        }
        t->next = apr_pcalloc(p, sizeof(device_table_list));
        t->next->next    = NULL;
        t->next->pattern = NULL;
        t->next->table   = NULL;
        t->next->tail    = NULL;
        dtl = t->next;
      }

      for (attr = qs_get_attr(doc,child); 
           attr ; 
           attr = qs_get_next_attr(doc,attr)) {
        char *attr_name = qs_get_attr_name(doc,attr);

        if (STRCASEEQ('p','P',"pattern",attr_name)) {
            dtl->pattern = apr_pstrdup(p, qs_get_attr_value(doc,attr));
            dtl->regexp = ap_pregcomp(p, (const char *)dtl->pattern, AP_REG_EXTENDED|AP_REG_ICASE);
        }
      }

      dtl->table_count = 0;
      s_set_device_data(doc, p, dtl, child);
      s_set_sort_table(doc, p, dtl);
    }
  }
}


static void
s_set_device_data(Doc *doc, apr_pool_t *p, device_table_list *dtl, Node *node) 
{
  Node         *child;
  device_table *dt;

  dt = apr_pcalloc(p, sizeof(device_table));
  dt->next           = NULL;
  dt->provider       = CHXJ_PROVIDER_UNKNOWN;
  dt->device_id      = NULL;
  dt->device_name    = NULL;
  dt->html_spec_type = CHXJ_SPEC_Chtml_3_0;
  dt->width          = 0;
  dt->heigh          = 0;
  dt->wp_width       = 0;
  dt->wp_heigh       = 0;
  dt->cache          = 5;
  dt->emoji_type     = NULL;
  dt->color          = 256;
  dt->dpi_width      = 96;
  dt->dpi_heigh      = 96;
  dt->output_encoding = "Shift_JIS";

  for (child = qs_get_child_node(doc,node); 
       child ;
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    switch (*name) {
    case 'd':
    case 'D':
      if (strcasecmp(name, "device") == 0) {
        s_set_device_data(doc,p, dtl, child);
      }
      else
      if (strcasecmp(name, "device_id") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          dt->device_id = apr_pstrdup(p, qs_get_node_value(doc, ch));
        }
      }
      else
      if (strcasecmp(name, "device_name") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          dt->device_name = apr_pstrdup(p, qs_get_node_value(doc, ch));
        }
      }
      else
      if (strcasecmp(name, "dpi_width") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
          if (ii == strlen(vv)) 
            dt->dpi_width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->dpi_width = 0;
        }
      }
      else
      if (strcasecmp(name, "dpi_heigh") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->dpi_heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->dpi_heigh = 0;
        }
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "html_spec_type") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc, ch);
          if (STRCASEEQ('x','X',"xhtml_mobile_1_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_XHtml_Mobile_1_0;
            dt->provider       = CHXJ_PROVIDER_AU;
          }
          else if (STRCASEEQ('c','C',"chtml_1_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_1_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_2_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_2_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_3_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_3_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_4_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_4_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_5_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_5_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_6_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_6_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('c','C',"chtml_7_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_7_0;
            dt->provider       = CHXJ_PROVIDER_DOCOMO;
          }
          else if (STRCASEEQ('h','H',"hdml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Hdml;
            dt->provider       = CHXJ_PROVIDER_AU;
          }
          else if (STRCASEEQ('j','J',"jhtml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Jhtml;
            dt->provider       = CHXJ_PROVIDER_SOFTBANK;
          }
          else if (STRCASEEQ('j','J',"jxhtml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Jxhtml;
            dt->provider       = CHXJ_PROVIDER_SOFTBANK;
          }
          else if (STRCASEEQ('i','I',"iphone2",vv)) {
            dt->html_spec_type = CHXJ_SPEC_iPhone2;
            dt->provider       = CHXJ_PROVIDER_SOFTBANK;
          }
          else if (STRCASEEQ('i','I',"iphone3",vv)) {
            dt->html_spec_type = CHXJ_SPEC_iPhone3;
            dt->provider       = CHXJ_PROVIDER_SOFTBANK;
          }
          else if (STRCASEEQ('i','I',"iphone4",vv)) {
            dt->html_spec_type = CHXJ_SPEC_iPhone4;
            dt->provider       = CHXJ_PROVIDER_SOFTBANK;
          }
        }
      }
      else 
      if (strcasecmp(name, "heigh") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->heigh = 0;
        }
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0')
              continue;
            break;
          }
          if (ii == strlen(vv))
            dt->width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->width = 0;
        }
      }
      else
      if (strcasecmp(name, "wp_width") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->wp_width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->wp_width = 0;
        }
      }
      else
      if (strcasecmp(name, "wp_heigh") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->wp_heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->wp_heigh = 0;
        }
      }
      break;

    case 'g':
    case 'G':
      if (strcasecmp(name, "gif") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0)
            dt->available_gif = 1;
          else
            dt->available_gif = 0;
        }
      }
      break;

    case 'j':
    case 'J':
      if (strcasecmp(name, "jpeg") == 0 || strcasecmp(name, "jpg") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch != NULL && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_jpeg = 1;
          else 
            dt->available_jpeg = 0;
        }
      }
      break;
  
    case 'p':
    case 'P':
      if (strcasecmp(name, "png") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_png = 1;
          else
            dt->available_png = 0;
        }
      }
      break;

    case 'b':
    case 'B':
      if (strcasecmp(name, "bmp2") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_bmp2 = 1;
          else
            dt->available_bmp2 = 0;
        }
      }
      else
      if (strcasecmp(name, "bmp4") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_bmp4 = 1;
          else
            dt->available_bmp4 = 0;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (chxj_chk_numeric(vv) != 0)
            dt->color = 0;
          else 
            dt->color = chxj_atoi(vv);
        }
      }
      else
      if (strcasecmp(name, "cache") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->cache = atoi(qs_get_node_value(doc,ch));
          else 
            dt->cache = 0;
        }
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "emoji_type") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) 
          dt->emoji_type = apr_pstrdup(p, qs_get_node_value(doc, ch));
      }
      break;
    case 'o':
    case 'O':
      if (strcasecmp(name, "output_encoding") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0){
          dt->output_encoding = apr_pstrdup(p, qs_get_node_value(doc, ch));
        }
      }
      break;
    default:
      break;
    }
  }

  if (dt->device_id) {
    if (! dtl->table) {
      dtl->table = dt;
      dtl->tail = dt;
    }
    else {
      dtl->tail->next = dt;
      dtl->tail = dt;
    }
    dtl->table_count++;
  }
}


static void
s_set_sort_table(Doc *doc, apr_pool_t *p, device_table_list *dtl)
{
  device_table **sort_table;
  device_table *dt;
  size_t ii=0;
  sort_table = apr_palloc(p, sizeof(device_table) * dtl->table_count);
  for (dt = dtl->table; dt; dt = dt->next) {
    sort_table[ii++] = dt;
  }
  dtl->sort_table = sort_table;
  qsort((void *)sort_table, (size_t)dtl->table_count, sizeof(*sort_table), s_sort_table_compare);
}

static int
s_sort_table_compare(const void *a, const void *b)
{
  device_table *aa = *(device_table **)a;
  device_table *bb = *(device_table **)b;

  /* 
   * do not use strcasecmp for LOAD!!
   */
  return strcmp(aa->device_id, bb->device_id);
}


/**
 * load device_data.xml
 */
void
chxj_load_device_tsv_data(apr_file_t *fp,apr_pool_t *p, mod_chxj_config *conf) 
{
  apr_status_t st;
  
  char *pstat;
  char *pair;
  
  char *line = apr_palloc(p,256);
  
  int is_header = 1;
  int keynum = 0;
  int valnum = 0;
  
  conf->device_keys = apr_array_make(p,2, sizeof(const char*));
  conf->device_hash = apr_hash_make(p);
  
  while(APR_EOF != (st=apr_file_eof(fp))){
    st = apr_file_gets(line,1024,fp);
    if(st == APR_SUCCESS){
      if(is_header){
        keynum=0;
        
        for(pair = apr_strtok(line,"\t",&pstat); pair != NULL; pair = apr_strtok(NULL,"\t",&pstat)){
          apr_collapse_spaces(pair,pair);
          if(keynum < 128){
            *(const char**)apr_array_push(conf->device_keys) = apr_pstrdup(p,pair);
          }
          
          //ap_log_error(APLOG_MARK, APLOG_CRIT, 0, NULL, "V[[%s]]", pair);
          keynum++;
        }
        is_header = 0;
      }
      else{
        apr_table_t *tsv_table = apr_table_make(p,keynum);
        valnum = 0;
        char *uid = NULL;
        char *provider = NULL;
        for(pair = apr_strtok(line,"\t",&pstat); pair != NULL; pair = apr_strtok(NULL,"\t",&pstat)){
          apr_collapse_spaces(pair,pair);
          if(valnum < keynum){
            const char *kn = ((const char**)conf->device_keys->elts)[valnum];
            if(strcasecmp("-",pair) != 0){
              apr_table_set(tsv_table,kn,pair);
              if(strcasecmp(kn,"device_id") == 0){
                uid = apr_pstrdup(p,pair);
              }
              else if(strcasecmp(kn,"provider") == 0){
                if(strcasecmp(pair,"docomo") == 0){
                  provider = "1";
                }
                else if(strcasecmp(pair,"au") == 0){
                  provider = "2";
                }
                else if(strcasecmp(pair,"softbank") == 0){
                  provider = "3";
                }
                else{
                  provider = apr_pstrdup(p,pair);
                }
              }
              //ap_log_error(APLOG_MARK, APLOG_CRIT, 0, NULL, "[%s] %d:V[%s] = [%s]", uid,valnum,kn,pair);
            }
          }
          valnum++;
        }
        if(uid != NULL && *uid && provider != NULL && *provider){
          char *key = apr_psprintf(p,"%s.%s",provider,uid);
          //ap_log_error(APLOG_MARK, APLOG_CRIT, 0, NULL, "save hash [%s]",key );
          apr_hash_set(conf->device_hash,key,APR_HASH_KEY_STRING,tsv_table);
        }
      }
    }
  }
}

/*
 * vim:ts=2 et
 */
