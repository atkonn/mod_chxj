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
#ifndef __CHXJ_HDML_H__
#define __CHXJ_HDML_H__

/*============================================================================*/
/* The maximum number of form is up to ten.                                   */
/*============================================================================*/
#define MAX_FORM_COUNT  (10)
#define MAX_RADIO_COUNT (10)
#define MAX_RADIO_VALUE_COUNT (100)
#define MAX_SUBMIT_BUTTON_COUNT (10)
#define END_LINE        "\0"

#include "mod_chxj.h"

/*============================================================================*/
/* Structure for HDML                                                         */
/*============================================================================*/
typedef struct _hdml_t {
  Doc*    doc;
  char*   out;
  int     center;
  int     in_center;
  char*   card;
  int     card_cnt;
  apr_time_t   form_cnt;

  /*--------------------------------------------------------------------------*/
  /* If the title tag is found, it becomes ``1''.                             */
  /*--------------------------------------------------------------------------*/
  int     found_title;
  /*--------------------------------------------------------------------------*/
  /* Counter of pure form tag                                                 */
  /*--------------------------------------------------------------------------*/
  int     pure_form_cnt;

  int     var_cnt[MAX_FORM_COUNT];
  char*   postdata[MAX_FORM_COUNT];
  /*------------------------------*/
  /* max <input type="radio"> tag */
  /*------------------------------*/
  char*   radio_value_list[MAX_RADIO_COUNT][MAX_RADIO_VALUE_COUNT];
  char*   radio_name_list[MAX_RADIO_COUNT];
  int     radio_out_cnt[MAX_RADIO_COUNT];
  char*   radio_checked_value[MAX_RADIO_COUNT];
  /*---------------------------------*/
  /* for <input type="submit"> tag   */
  /*---------------------------------*/
  char*   submit_button[MAX_SUBMIT_BUTTON_COUNT];

  /*---------------------------------*/
  /* for <input type="checkbox"> tag */
  /*---------------------------------*/
  int     has_checkbox;

  char*   form_tmp;
  char*   init_vars;
  int     out_len;
  int     card_len;

  /*------------------------------*/
  /* Counter for variable of HDML */
  /*------------------------------*/
  int     qs_cnt;
  int     option_flag;
  int     div_right_flag;
  int     div_center_flag;
  int     div_in_center;
  int     hdml_a_flag;
  int     hdml_br_flag;

  int     hdml_end_flag;

  device_table* spec;

  mod_chxj_config*  conf;

  chxjconvrule_entry* entryp;
} hdml_t;
/*============================================================================*/
/* Prototype declaration.                                                     */
/*============================================================================*/
char* chxj_exchange_hdml(
  request_rec*        r, 
  device_table*       spec,
  const char*         src,
  apr_size_t          srclen,
  apr_size_t*         dstlen,
  chxjconvrule_entry* entryp) ;
char* qs_out_apr_pstrcat(request_rec* r, char* o, char* s, int* len);
char* qs_conv_istyle_to_format(request_rec* r, char* is);

#endif
