/*
 * Copyright (C) 2008 Atsushi Konno All rights reserved.
 * Copyright (C) 2008 QSDN,Inc. All rights reserved.
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
#if !defined(__SCSS_H__)
#define __SCSS_H__

#include "apr_pools.h"
#include "apr_strings.h"
#include "scss_node.h"

#define ROOT_NODE_SIGNATURE "<rootNode>"

typedef struct _SCSSDoc_t SCSSDoc_t;
typedef SCSSDoc_t *SCSSDocPtr_t;

struct _SCSSDoc_t {
  apr_pool_t *pool;
  SCSSNodePtr_t rootNode;
  SCSSNodePtr_t nowNode;
  void *userData;
};

#define IS_STYLESHEET(X) ((X)->type == SCSSTYPE_STYLESHEET)
#define IS_ATKEYWORD(X)  ((X)->type == SCSSTYPE_ATKEYWORD )
#define IS_SELECTOR(X)   ((X)->type == SCSSTYPE_SELECTOR  )
#define IS_PROPERTY(X)   ((X)->type == SCSSTYPE_PROPERTY  )


#include "scss_sac.h"

typedef void (*SCSSParserError_fn)(void *userData, const char *func, const char *fname, int line, int srcline, char *fmt, ...);



/**
 * for DEBUG. dump Node tree.
 *
 * @param doc     for dump doc object.
 * @param level   please set 0.
 */
extern void scss_dump_nodes(SCSSDocPtr_t doc);

/**
 * CSS Parser.
 *
 * @param ppool - parrent pool.
 * @param src   - CSS source.
 * @return SCSSDocPtr_t
 */
extern SCSSDocPtr_t scss_parser(apr_pool_t *ppool,  const char *src);

/**
 * CSS Parser Error log.
 */
extern SCSSParserError_fn scss_parser_error;
#endif
