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

/**
 * Dump string to debug log.
 */
void
chxj_dump_string(request_rec *r, const char *filename, int line, const char *title, const char *str, apr_size_t len)
{
  apr_size_t ii;
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] +-------------------------------------------------------------------+", (unsigned int)(apr_size_t)r);
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] |                                                                   |", (unsigned int)(apr_size_t)r);
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] | %-*.*s |", (unsigned int)(apr_size_t)r, 64, 64, title);
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] |                                                                   |", (unsigned int)(apr_size_t)r);
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] +-------------------------------------------------------------------+", (unsigned int)(apr_size_t)r);
  for (ii=0; ii<len/64; ii++) {
    chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] | [%-*.*s] |", (unsigned int)(apr_size_t)r, 64, 64, &str[ii * 64]);
  }
  if (len % 64) {
    chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] | [%-*.*s] |", (unsigned int)(apr_size_t)r, 64, 64, &str[ii * 64]);
  }
  chxj_log_rerror(filename, line, APLOG_DEBUG,0,r, "REQ[%X] +-------------------------------------------------------------------+", (unsigned int)(apr_size_t)r);
}
