/*
 * Copyright (C) 2005-2008 Atsushi Konno All rights reserved.
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
  /* "X-Chxj-Provider" => { 0 => 'unknown', 1 => 'docomo', 2 => 'au', 3 => 'SoftBank' */
  /* "X-Chxj-DeviceId" => { docomo => 'D503i' and so on, au => 'CA31' and so on, SoftBank => '706P' and so on */
  /* "X-Chxj-DeviceName" => { 'SoftBank 706P' and so on. } <device_name> */
  /* "X-Chxj-HtmlSpecType" => { JXHTML, JHTML, CHTML1.0 CHTML2.0 CHTML3.0 CHTML4.0 CHTML5.0 IXHTML1.0 } */
  /* "X-Chxj-Width"        => { <width> } */
  /* "X-Chxj-Height"       => { <height> } */
  /* "X-Chxj-Gif"          => { true/false } */
  /* "X-Chxj-Jpeg"         => { true/false } */
  /* "X-Chxj-Png"          => { true/false } */
  /* "X-Chxj-Bmp2"         => { true/false } */
  /* "X-Chxj-Bmp4"         => { true/false } */
  /* "X-Chxj-Color"        => { <color> } */
  /* "X-Chxj-WP-Width"     => { <wp_width> } */
  /* "X-Chxj-WP-Height"    => { <wp_height> } */
  /* "X-Chxj-Cache"        => { <cache> } */

  DBG(r, "REQ[%X] end chxj_add_device_env()", (unsigned int)(apr_size_t)r);
}
