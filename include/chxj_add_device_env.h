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
#ifndef __CHXJ_ADD_DEVICE_ENV_H__
#define __CHXJ_ADD_DEVICE_ENV_H__

#include "mod_chxj.h"
#include "chxj_specified_device.h"

/* "X-Chxj-Provider" => { 0 => 'unknown', 1 => 'docomo', 2 => 'au', 3 => 'SoftBank' */
#define HTTP_X_CHXJ_PROVIDER        "X-Chxj-Provider"
#define HTTP_X_CHXJ_PROVIDER_STRING "X-Chxj-Provider-String"

/* "X-Chxj-DeviceId" => { docomo => 'D503i' and so on, au => 'CA31' and so on, SoftBank => '706P' and so on */
#define HTTP_X_CHXJ_DEVICEID "X-Chxj-DeviceId"

/* "X-Chxj-DeviceName" => { 'SoftBank 706P' and so on. } <device_name> */
#define HTTP_X_CHXJ_DEVICENAME "X-Chxj-DeviceName"

/* "X-Chxj-HtmlSpecType" => { JXHTML, JHTML, CHTML1.0 CHTML2.0 CHTML3.0 CHTML4.0 CHTML5.0 IXHTML1.0 } */
#define HTTP_X_CHXJ_HTMLSPECTYPE "X-Chxj-HtmlSpecType"

/* "X-Chxj-Width"        => { <width> } */
#define HTTP_X_CHXJ_WIDTH "X-Chxj-Width"

/* "X-Chxj-Height"       => { <height> } */
#define HTTP_X_CHXJ_HEIGHT "X-Chxj-Height"

/* "X-Chxj-Gif"          => { true/false } */
#define HTTP_X_CHXJ_GIF    "X-Chxj-Gif"

/* "X-Chxj-Jpeg"         => { true/false } */
#define HTTP_X_CHXJ_JPEG   "X-Chxj-Jpeg"

/* "X-Chxj-Png"          => { true/false } */
#define HTTP_X_CHXJ_PNG    "X-Chxj-Png"

/* "X-Chxj-Bmp2"         => { true/false } */
#define HTTP_X_CHXJ_BMP2   "X-Chxj-Bmp2"

/* "X-Chxj-Bmp4"         => { true/false } */
#define HTTP_X_CHXJ_BMP4   "X-Chxj-Bmp4"

/* "X-Chxj-Color"        => { <color> } */
#define HTTP_X_CHXJ_COLOR  "X-Chxj-Color"

/* "X-Chxj-WP-Width"     => { <wp_width> } */
#define HTTP_X_CHXJ_WP_WIDTH "X-Chxj-WP-Width"

/* "X-Chxj-WP-Height"    => { <wp_height> } */
#define HTTP_X_CHXJ_WP_HEIGHT "X-Chxj-WP-Height"

/* "X-Chxj-Cache"        => { <cache> } */
#define HTTP_X_CHXJ_CACHE    "X-Chxj-Cache"

#define HTTP_X_CHXJ_VERSION  "X-Chxj-Version"

extern void chxj_add_device_env(request_rec *r, device_table *spec);

#endif
/*
 * vim:ts=2 et
 */
