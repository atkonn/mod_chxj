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
#include "chxj_str_util.h"
#include "chxj_add_device_env.h"

static device_table  UNKNOWN_DEVICE      = {
  .next = NULL,
  .provider  = CHXJ_PROVIDER_UNKNOWN,
  .device_id = "",
  .device_name = "UNKNOWN",
  .html_spec_type = CHXJ_SPEC_UNKNOWN,
  .width = 640,
  .heigh = 480,
  /*--------------------------------------------------------------------------*/
  /* Walll Paper Size                                                         */
  /*--------------------------------------------------------------------------*/
  .wp_width = 640,
  .wp_heigh = 480,
  /*--------------------------------------------------------------------------*/
  /* Cache Size                                                               */
  /*--------------------------------------------------------------------------*/
  .cache    = 10000000,
  /*--------------------------------------------------------------------------*/
  /* Correspondence image format                                              */
  /* 1: It is possible to display it.                                         */
  /* 0: It is not possible to display it.                                     */
  /*--------------------------------------------------------------------------*/
  .available_gif = 1,
  .available_jpeg = 1,
  .available_png = 1,
  .available_bmp2 = 1,
  .available_bmp4 = 1,
  .dpi_width = 96,
  .dpi_heigh = 96,
  /*--------------------------------------------------------------------------*/
  /* Color number type                                                        */
  /* 2       : 2        Colors                                                */
  /* 4       : 4        Colors                                                */
  /* 256     : 256      Colors                                                */
  /* 4096    : 4096     Colors                                                */
  /* 65536   : 65536    Colors                                                */
  /* 262144  : 262144   Colors                                                */
  /* 15680000: 15680000 over colors                                           */
  /*--------------------------------------------------------------------------*/
  .color = 15680000,
  .emoji_type = NULL,
  .output_encoding = "Shift_JIS",
};
/* spec cache */
static device_table *s_get_device_data(request_rec *r, const char *device_id, device_table_list *dtl);
static device_table *s_specified_device_from_xml(request_rec *r, mod_chxj_config * conf, const char *user_agent);
static device_table *s_specified_device_from_tsv(request_rec *r,device_table *spec,const char *user_agent);


int
get_boolean_value(request_rec *r,const char *val){
  switch(*val) {
    case '0':
    case 'f':
    case 'F':
    case 'n':
    case 'N':
      return 0;
      break;
    case '1':
    case 't':
    case 'T':
    case 'y':
    case 'Y':
      return 1;
      break;
  }
  return -1;
}

/**
 * The device is specified from UserAgent. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
device_table *
chxj_specified_device(request_rec *r, const char *user_agent) 
{
  device_table         *dt = &UNKNOWN_DEVICE;
  mod_chxj_config      *conf;
  mod_chxj_req_config  *request_conf;
  
  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);

  request_conf = chxj_get_req_config(r);
  if (user_agent) {
    request_conf->user_agent = apr_pstrdup(r->pool, user_agent);
  }
  else {
    request_conf->user_agent = "";
  }
  
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if(! user_agent){
    request_conf->spec = dt;
    DBG(r, "REQ[%X] end %s() %d", TO_ADDR(r), __func__,conf->detect_device_type);
    return dt;
  }
  
  dt = s_specified_device_from_xml(r,conf, user_agent);

  if (conf->detect_device_type == CHXJ_ADD_DETECT_DEVICE_TYPE_TSV ){
    s_specified_device_from_tsv(r,dt,user_agent);
  }
  /* save to spec cache */
  request_conf->spec = dt;
  
  DBG(r, "REQ[%X] end %s() %d",TO_ADDR(r), __func__,conf->detect_device_type);
  return dt;
}

/**
 * The device is specified from UserAgent. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
static device_table *
s_specified_device_from_xml(request_rec *r, mod_chxj_config * conf, const char *user_agent) 
{
  ap_regmatch_t        match[10];
  device_table         *returnType = &UNKNOWN_DEVICE;
  device_table_list    *dtl;
  device_table         *dt;
  char                 *device_id;

  DBG(r, "REQ[%X] start %s()", TO_ADDR(r),__func__);
  if (! user_agent) {
    DBG(r, "REQ[%X] end %s() (user_agent is not set)", TO_ADDR(r),__func__);
    return returnType;
  }
            


  if (! conf->devices) {
    ERR(r, "REQ[%X] device_data.xml load failure", TO_ADDR(r));
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r), __func__);
    return returnType;
  }

  for (dtl = conf->devices; dtl; dtl = dtl->next) {
    if (! dtl->pattern) {
      continue;
    }

    /* DBG(r, "pattern is [%s]", dtl->pattern); */
    if (! dtl->regexp) {
      ERR(r,"REQ[%X] compile failed.", TO_ADDR(r));
      DBG(r, "REQ[%X] %s()", TO_ADDR(r),__func__);
      return returnType;
    }

    if (ap_regexec((const ap_regex_t *)dtl->regexp, user_agent, (apr_size_t)dtl->regexp->re_nsub + 1, match, 0) == 0) {
      device_id = ap_pregsub(r->pool, "$1", user_agent, dtl->regexp->re_nsub + 1, match);
      DBG(r, "REQ[%X] device_id:[%s]", TO_ADDR(r), device_id);

      dt = s_get_device_data(r, device_id, dtl);
      if (! dt) {
        dt = dtl->tail;
        if (dt){
          if (conf->detect_device_type > CHXJ_ADD_DETECT_DEVICE_TYPE_NONE ){
            dt->device_id = device_id;
          }
          DBG(r,"REQ[%X] end %s() (Not Found) use [%s]", TO_ADDR(r), __func__, dt->device_id);
          return dt;
        }
      }
      DBG(r,"REQ[%X] end %s() (Found) use [%s]", TO_ADDR(r), __func__, dt->device_id);
      return dt;
    }
  }

  DBG(r,"REQ[%X] end %s()", TO_ADDR(r), __func__);

  return returnType;
}


#include <stdlib.h>
static int 
s_compar(const void *a, const void *b)
{
  device_table *aa = *(device_table **)a;
  device_table *bb = *(device_table **)b;

  /*
   * Don't use strcasecmp for LOAD!!
   */
  return strcmp(aa->device_id, bb->device_id);
}


static device_table *
s_get_device_data(request_rec *r, const char *device_id, device_table_list *dtl)
{
  device_table dt;
  device_table *_dt;
  dt.device_id = device_id;
  _dt = &dt;
  device_table **ret = bsearch(&_dt, dtl->sort_table, dtl->table_count, sizeof(device_table *), s_compar);
  if (ret && *ret) {
    return *ret;
  }
  return NULL;
}


/**
 * The device is specified from TSV file. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
static device_table *
s_specified_device_from_tsv(request_rec *r,device_table *spec,const char *user_agent)
{
  if(spec->device_id == NULL){
    return spec;
  }
  
  DBG(r, "REQ[%X] start %s() device_id:[%s]", TO_ADDR(r), __func__,spec->device_id);
  mod_chxj_config      *conf;
  
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  
  if(conf->device_hash == NULL){
    DBG(r, "REQ[%X] end %s()", TO_ADDR(r),__func__);
    return spec;
  }
  char *key = apr_psprintf(r->pool,"%d.%s",spec->provider,spec->device_id);
  
  apr_table_t *ht = apr_hash_get(conf->device_hash,key,APR_HASH_KEY_STRING);
  
  if(ht != NULL){
    DBG(r, "REQ[%X] found ! %s() %s", TO_ADDR(r), __func__,key);
    int i;
    for ( i=0; i< conf->device_keys->nelts; i++){
      const char *k = ((const char**)conf->device_keys->elts)[i];
      char *val = (char *)apr_table_get(ht,k);
      if(val == NULL){
        continue;
      }
      DBG(r, "REQ[%X] start chxj_specified_device_from_tsv() [%s] = [%s]:[%s]",TO_ADDR(r),spec->device_id,k,val);
      if (STRCASEEQ('d','D',"device_name",k)){
        spec->device_name = apr_pstrdup(r->pool,val);
      }
      if (STRCASEEQ('w','W',"width",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->width = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('h','H',"heigh",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->heigh = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('h','H',"height",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->heigh = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('w','W',"wp_width",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->wp_width = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('w','W',"wp_heigh",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->wp_heigh = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('w','W',"wp_height",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->wp_heigh = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('c','C',"cache",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->cache = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('g','G',"gif",k)){
        int tmp = get_boolean_value(r,val);
        if(tmp > -1 ){
            spec->available_gif = tmp;
        }
      }
      else if (STRCASEEQ('j','J',"jpeg",k)){
        int tmp = get_boolean_value(r,val);
        if(tmp > -1 ){
            spec->available_jpeg = tmp;
        }
      }
      else if (STRCASEEQ('p','P',"png",k)){
        int tmp = get_boolean_value(r,val);
        if(tmp > -1 ){
          spec->available_png = tmp;
        }
      }
      else if (STRCASEEQ('b','B',"bmp2",k)){
        int tmp = get_boolean_value(r,val);
        if(tmp > -1 ){
          spec->available_bmp2 = tmp;
        }
      }
      else if (STRCASEEQ('b','B',"bmp4",k)){
        int tmp = get_boolean_value(r,val);
        if(tmp > -1 ){
          spec->available_bmp4 = tmp;
        }
      }
      else if (STRCASEEQ('c','C',"color",k)){
        if(chxj_chk_numeric(val) == 0){
          spec->color = chxj_atoi(val);
        }
      }
      else if (STRCASEEQ('e','E',"emoji_type",k)){
        spec->emoji_type = apr_pstrdup(r->pool,val);
      }
      else if (STRCASEEQ('h','H',"html_spec_type",k)){
        if (STRCASEEQ('x','X',"xhtml_mobile_1_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_XHtml_Mobile_1_0;
        }
        else if (STRCASEEQ('c','C',"chtml_1_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_1_0;
        }
        else if (STRCASEEQ('c','C',"chtml_2_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_2_0;
        }
        else if (STRCASEEQ('c','C',"chtml_3_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_3_0;
        }
        else if (STRCASEEQ('c','C',"chtml_4_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_4_0;
        }
        else if (STRCASEEQ('c','C',"chtml_5_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_5_0;
        }
        else if (STRCASEEQ('c','C',"chtml_6_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_6_0;
        }
        else if (STRCASEEQ('c','C',"chtml_7_0",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_7_0;
        }
        else if (STRCASEEQ('h','H',"hdml",val)) {
          spec->html_spec_type = CHXJ_SPEC_Hdml;
        }
        else if (STRCASEEQ('i','I',"ixhtml",val)) {
          spec->html_spec_type = CHXJ_SPEC_Chtml_7_0;
        }
        else if (STRCASEEQ('j','J',"jhtml",val)) {
          spec->html_spec_type = CHXJ_SPEC_Jhtml;
        }
        else if (STRCASEEQ('j','J',"jxhtml",val)) {
          spec->html_spec_type = CHXJ_SPEC_Jxhtml;
        }
      }
      else if (STRCASEEQ('o','O',"output_encoding",k)){
          spec->output_encoding = apr_pstrdup(r->pool,val);
      }
    }
  }
  
  DBG(r, "REQ[%X] end %s() [%d]",TO_ADDR(r),__func__,spec->provider);
  return spec;
}
/*
 * vim:ts=2 et
 */

