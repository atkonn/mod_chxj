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
#include "scss.h"
#include "apr_pools.h"

SCSSDocPtr_t
scss_create_doc(apr_pool_t *ppool)
{
  apr_pool_t *pool;
  SCSSDocPtr_t doc;

  apr_pool_create(&pool, ppool);
  doc = apr_palloc(pool, sizeof(*doc));
  memset(doc , 0, sizeof(*doc));

  doc->pool = pool;

  doc->rootNode = apr_palloc(pool, sizeof(SCSSNode_t));

  doc->rootNode->type      = SCSSTYPE_STYLESHEET; 
  doc->rootNode->next      = doc->rootNode;
  doc->rootNode->ref       = &doc->rootNode->next;
  doc->rootNode->child     = NULL;
  doc->rootNode->name      = apr_pstrdup(pool, ROOT_NODE_SIGNATURE);
  doc->rootNode->value1    = NULL;
  doc->rootNode->value2    = NULL;

  doc->nowNode = doc->rootNode;
  doc->userData = NULL;

  return doc;
}
