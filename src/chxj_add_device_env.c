/*
 * Copyright (C) 2005-2009 Atsushi Konno All rights reserved.
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
#include "chxj_add_device_env.h"
#include "chxj_str_util.h"

void
chxj_add_device_env(request_rec *r, device_table *spec)
{
  DBG(r, "REQ[%X] start chxj_add_device_env()", (unsigned int)(apr_size_t)r);
  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER, "1");
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER_STRING, "docomo");
    break;
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER, "2");
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER_STRING, "au");
    break;
  case CHXJ_SPEC_Jhtml:
  case CHXJ_SPEC_Jxhtml:
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER, "3");
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER_STRING, "SoftBank");
    break;
  default:
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER, "0");
    apr_table_setn(r->headers_in, HTTP_X_CHXJ_PROVIDER_STRING, "unknown");
    break;
  }

  apr_table_setn(r->headers_in, HTTP_X_CHXJ_DEVICEID, apr_pstrdup(r->pool, spec->device_id));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_DEVICENAME, apr_pstrdup(r->pool, spec->device_name));

  switch(spec->html_spec_type) {
  case CHXJ_SPEC_Chtml_1_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML1.0"); break;
  case CHXJ_SPEC_Chtml_2_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML2.0"); break;
  case CHXJ_SPEC_Chtml_3_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML3.0"); break;
  case CHXJ_SPEC_Chtml_4_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML4.0"); break;
  case CHXJ_SPEC_Chtml_5_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML5.0"); break;
  case CHXJ_SPEC_Chtml_6_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML6.0"); break;
  case CHXJ_SPEC_Chtml_7_0:        apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "CHTML7.0"); break;
  case CHXJ_SPEC_XHtml_Mobile_1_0: apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "XHTML");    break;
  case CHXJ_SPEC_Hdml:             apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "HDML");     break;
  case CHXJ_SPEC_Jhtml:            apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "JHTML");    break;
  case CHXJ_SPEC_Jxhtml:           apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "JXHTML");   break;
  default:                         apr_table_setn(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE, "UNKNOWN");  break;
  }

  apr_table_setn(r->headers_in, HTTP_X_CHXJ_WIDTH,    apr_psprintf(r->pool, "%d", spec->width));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_HEIGHT,   apr_psprintf(r->pool, "%d", spec->heigh));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_GIF,      (spec->available_gif  == 0) ? "false" : "true");
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_JPEG,     (spec->available_jpeg == 0) ? "false" : "true");
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_PNG,      (spec->available_png  == 0) ? "false" : "true");
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_BMP2,     (spec->available_bmp2 == 0) ? "false" : "true");
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_BMP4,     (spec->available_bmp4 == 0) ? "false" : "true");
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_COLOR,    apr_psprintf(r->pool, "%d", spec->color));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_WP_WIDTH, apr_psprintf(r->pool, "%d", spec->wp_width));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_WP_HEIGHT,apr_psprintf(r->pool, "%d", spec->wp_heigh));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_CACHE,    apr_psprintf(r->pool, "%d", spec->cache));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_DPI_WIDTH,    apr_psprintf(r->pool, "%d", spec->dpi_width));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_DPI_HEIGHT,   apr_psprintf(r->pool, "%d", spec->dpi_heigh));
  apr_table_setn(r->headers_in, HTTP_X_CHXJ_EMOJI_TYPE,   (spec->emoji_type) ? spec->emoji_type : "");

  apr_table_setn(r->headers_in, HTTP_X_CHXJ_VERSION,  apr_pstrdup(r->pool, PACKAGE_VERSION));
  
  mod_chxj_config*    dconf;
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (dconf->detect_device_type == CHXJ_ADD_DETECT_DEVICE_TYPE_TSV ){
    if (spec->device_id     != NULL &&
        dconf->device_hash  != NULL){
      char *key = apr_psprintf(r->pool,"%d.%s",spec->provider,spec->device_id);
      apr_table_t *ht = apr_hash_get(dconf->device_hash,key,APR_HASH_KEY_STRING);
  
      if(ht != NULL){
        int i;
        for ( i=0; i< dconf->device_keys->nelts; i++){
          const char *k = ((const char**)dconf->device_keys->elts)[i];
          char *val = (char *)apr_table_get(ht,k);
          if(val != NULL){
            char *info_key = apr_psprintf(r->pool,"%s-%s",HTTP_X_CHXJ_INFO,k);
            apr_table_setn(r->headers_in, info_key, val);
          }
        }
      }
    }
  }

  DBG(r, "REQ[%X] end chxj_add_device_env()", (unsigned int)(apr_size_t)r);
}

device_table *
chxj_get_device_env(request_rec *r)
{
  device_table *spec;
  char *tmp;
  DBG(r, "REQ[%X] start chxj_get_device_env()", (unsigned int)(apr_size_t)r);

  spec = apr_palloc(r->pool, sizeof(device_table));
  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_HTMLSPECTYPE);
  if (! tmp) {
    DBG(r, "REQ[%X] end chxj_get_device_env()", (unsigned int)(apr_size_t)r);
    return NULL;
  }
  if (STRCASEEQ('c','C', "CHTML1.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_1_0;
  }
  else if (STRCASEEQ('c','C', "CHTML2.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_2_0;
  }
  else if (STRCASEEQ('c','C', "CHTML3.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_3_0;
  }
  else if (STRCASEEQ('c','C', "CHTML4.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_4_0;
  }
  else if (STRCASEEQ('c','C', "CHTML5.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_5_0;
  }
  else if (STRCASEEQ('c','C', "CHTML6.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_6_0;
  }
  else if (STRCASEEQ('c','C', "CHTML7.0", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Chtml_7_0;
  }
  else if (STRCASEEQ('x','X', "XHTML", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_XHtml_Mobile_1_0;
  }
  else if (STRCASEEQ('h','H', "HDML", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Hdml;
  }
  else if (STRCASEEQ('j','j', "JHTML", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Jhtml;
  }
  else if (STRCASEEQ('j','j', "JXHTML", tmp)) {
    spec->html_spec_type = CHXJ_SPEC_Jxhtml;
  }
  else {
    spec->html_spec_type = CHXJ_SPEC_UNKNOWN;
  }
  
  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_DEVICEID);
  if (tmp) {
    spec->device_id = apr_pstrdup(r->pool, tmp);
  }
  else {
    spec->device_id = apr_pstrdup(r->pool, "");
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_DEVICENAME);
  if (tmp) {
    spec->device_name = apr_pstrdup(r->pool, tmp);
  }
  else {
    spec->device_name = apr_pstrdup(r->pool, "UNKNOWN");
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_WIDTH);
  if (tmp) {
    spec->width = chxj_atoi(tmp); 
  }
  else {
    spec->width = 640;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_HEIGHT);
  if (tmp) {
    spec->heigh = chxj_atoi(tmp); 
  }
  else {
    spec->heigh = 480; 
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_GIF);
  if (STRCASEEQ('t','T',"true",tmp)) {
    spec->available_gif = 1;
  }
  else {
    spec->available_gif = 0;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_JPEG);
  if (STRCASEEQ('t','T',"true",tmp)) {
    spec->available_jpeg = 1;
  }
  else {
    spec->available_jpeg = 0;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_PNG);
  if (STRCASEEQ('t','T',"true",tmp)) {
    spec->available_png = 1;
  }
  else {
    spec->available_png = 0;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_BMP2);
  if (STRCASEEQ('t','T',"true",tmp)) {
    spec->available_bmp2 = 1;
  }
  else {
    spec->available_bmp2 = 0;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_BMP4);
  if (STRCASEEQ('t','T',"true",tmp)) {
    spec->available_bmp4 = 1;
  }
  else {
    spec->available_bmp4 = 0;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_COLOR);
  if (tmp) {
    spec->color = chxj_atoi(tmp); 
  }
  else {
    spec->color = 15680000; 
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_WP_WIDTH);
  if (tmp) {
    spec->wp_width = chxj_atoi(tmp); 
  }
  else {
    spec->wp_width = 640; 
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_WP_HEIGHT);
  if (tmp) {
    spec->wp_heigh = chxj_atoi(tmp); 
  }
  else {
    spec->wp_heigh = 480; 
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_CACHE);
  if (tmp) {
    spec->cache = chxj_atoi(tmp); 
  }
  else {
    spec->cache = 10000000; 
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_DPI_WIDTH);
  if (tmp) {
    spec->dpi_width = chxj_atoi(tmp); 
  }
  else {
    spec->dpi_width = 96;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_DPI_HEIGHT);
  if (tmp) {
    spec->dpi_heigh = chxj_atoi(tmp); 
  }
  else {
    spec->dpi_heigh = 96;
  }

  tmp = (char *)apr_table_get(r->headers_in, HTTP_X_CHXJ_EMOJI_TYPE);
  if (tmp) {
    spec->emoji_type = apr_pstrdup(r->pool, tmp);
  }
  else {
    spec->emoji_type = apr_pstrdup(r->pool, "");
  }

  DBG(r, "REQ[%X] end chxj_get_device_env()", (unsigned int)(apr_size_t)r);
  return spec;
}
