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

  DBG(r, "REQ[%X] end chxj_add_device_env()", (unsigned int)(apr_size_t)r);
}
