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
#include "chxj_header_inf.h"
#include "chxj_apache.h"
#include "qs_parse_string.h"
#include <errno.h>

/**
 */
char *
chxj_header_inf_set_content_type(request_rec *r, char *default_string)
{
  char *h_out     = NULL;
  char *err_h_out = NULL;

  DBG(r,"REQ[%X] start chxj_header_inf_set_content_type()", (unsigned int)(apr_size_t)r);

  h_out     = (char *)apr_table_get(r->headers_out, HTTP_X_CHXJ_SET_CONTENT_TYPE);
  err_h_out = (char *)apr_table_get(r->err_headers_out, HTTP_X_CHXJ_SET_CONTENT_TYPE);

  if (err_h_out) {
    DBG(r,"REQ[%X] end chxj_header_inf_set_content_type()", (unsigned int)(apr_size_t)r);
    return err_h_out;
  }
  if (h_out) {
    DBG(r,"REQ[%X] end chxj_header_inf_set_content_type()", (unsigned int)(apr_size_t)r);
    return h_out;
  }

  DBG(r,"REQ[%X] end chxj_header_inf_set_content_type()", (unsigned int)(apr_size_t)r);
  return default_string;
}

/*
 * vim: ts=2 et
 */
