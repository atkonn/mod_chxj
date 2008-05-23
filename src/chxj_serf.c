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
#include "chxj_serf.h"
#include "apr_pools.h"

typedef struct __app_ctx_t     app_ctx_t;
typedef struct __handler_ctx_t handler_ctx_t;

struct __app_ctx_t {
  int                 ssl_flag;
  serf_ssl_context_t  *ssl_ctx;
  serf_bucket_alloc_t *bkt_alloc;
};

struct __handler_ctx_t {
#if APR_MAJOR_VERSION > 0
  apr_uint32_t requests_outstanding;
#else
  apr_atomic_t requests_outstanding;
#endif

  serf_response_acceptor_t acceptor;
  app_ctx_t                *acceptor_ctx;

  serf_response_handler_t  handler;

  const char *host;
  const char *method;
  const char *path;

  apr_status_t rv;
  const char *reason;

  char *response;
};

char *default_chxj_serf_get(request_rec *r, apr_pool_t *ppool, const char *url_path);
char *(*chxj_serf_get)(request_rec *r, apr_pool_t *ppool, const char *url_path) = default_chxj_serf_get;


void
s_init(apr_pool_t *ppool, apr_pool_t **pool)
{
  apr_pool_create(pool, ppool);
  apr_atomic_init(*pool);
}


void
s_term(apr_pool_t *pool)
{
  apr_pool_destroy(pool);
}


static serf_bucket_t *
s_connection_setup(apr_socket_t *skt, void *setup_ctx, apr_pool_t *pool)
{
  serf_bucket_t  *c;
  app_ctx_t      *ctx = (app_ctx_t *)setup_ctx;

  if (ctx->ssl_flag) {
    c = serf_bucket_ssl_decrypt_create(c, ctx->ssl_ctx, ctx->bkt_alloc);
    if (!ctx->ssl_ctx) {
      ctx->ssl_ctx = serf_bucket_ssl_decrypt_context_get(c);
    }
    return c;
  }
  return serf_bucket_socket_create(skt, ctx->bkt_alloc);
}


static void 
s_connection_closed(serf_connection_t *conn, void *closed_baton, apr_status_t why, apr_pool_t *pool)
{
  /* nothing */
}


static serf_bucket_t *
s_accept_response(serf_request_t *request, serf_bucket_t *stream, void *acceptor_baton, apr_pool_t *pool)
{
    serf_bucket_alloc_t *bkt_alloc;
    serf_bucket_t       *c;

    bkt_alloc = serf_request_get_alloc(request);
    c = serf_bucket_barrier_create(stream, bkt_alloc);
    return serf_bucket_response_create(c, bkt_alloc);
}


static apr_status_t 
s_handle_response(serf_request_t *request, serf_bucket_t *response, void *handler_ctx, apr_pool_t *pool)
{
  const char      *data;
  apr_size_t      len;
  serf_status_line sl;
  apr_status_t     rv;
  handler_ctx_t  *ctx = handler_ctx;

  rv = serf_bucket_response_status(response, &sl);
  if (rv != APR_SUCCESS) {
    if (APR_STATUS_IS_EAGAIN(rv)) {
      return rv;
    }
    ctx->rv = rv;
    apr_atomic_dec32(&ctx->requests_outstanding); 
    return rv;
  }
  ctx->reason = sl.reason;

  while (1) {
    char *buf;
    rv = serf_bucket_read(response, 2048, &data, &len);
    if (SERF_BUCKET_READ_ERROR(rv)) {
      ctx->rv;
      apr_atomic_dec32(&ctx->requests_outstanding);
      return rv;
    }

    if (! ctx->response) {
      ctx->response = apr_palloc(pool, 1);
      ctx->response[0] = 0;
    }
    
    buf = apr_palloc(pool, len + 1);
    memset(buf, 0, len + 1);
    memcpy(buf, data, len);
    ctx->response = apr_pstrcat(pool, ctx->response, buf, NULL);

    if (APR_STATUS_IS_EOF(rv)) {
      apr_atomic_dec32(&ctx->requests_outstanding);
      return APR_EOF;
    }

    if (APR_STATUS_IS_EAGAIN(rv))
      return rv;
  }
}

static apr_status_t 
s_setup_request(serf_request_t           *request,
                void                     *setup_ctx,
                serf_bucket_t            **req_bkt,
                serf_response_acceptor_t *acceptor,
                void                     **acceptor_ctx,
                serf_response_handler_t  *handler,
                void                     **handler_ctx,
                apr_pool_t               *pool)
{
    handler_ctx_t *ctx = setup_ctx;
    serf_bucket_t *hdrs_bkt;
    serf_bucket_t *body_bkt;

    *req_bkt = serf_bucket_request_create(ctx->method, ctx->path, NULL, serf_request_get_alloc(request));

    hdrs_bkt = serf_bucket_request_get_headers(*req_bkt);

    serf_bucket_headers_setn(hdrs_bkt, "Host", ctx->host);
    serf_bucket_headers_setn(hdrs_bkt, "User-Agent", "Wget2/1.0");

    apr_atomic_inc32(&(ctx->requests_outstanding));

    if (ctx->acceptor_ctx->ssl_flag) {
        serf_bucket_alloc_t *req_alloc;
        app_ctx_t *app_ctx = ctx->acceptor_ctx;

        req_alloc = serf_request_get_alloc(request);

        if (app_ctx->ssl_ctx == NULL) {
            *req_bkt = serf_bucket_ssl_encrypt_create(*req_bkt, NULL, app_ctx->bkt_alloc);
            app_ctx->ssl_ctx = serf_bucket_ssl_encrypt_context_get(*req_bkt);
        }
        else {
            *req_bkt = serf_bucket_ssl_encrypt_create(*req_bkt, app_ctx->ssl_ctx, app_ctx->bkt_alloc);
        }
    }

    *acceptor       = ctx->acceptor;
    *acceptor_ctx   = ctx->acceptor_ctx;
    *handler        = ctx->handler;
    *handler_ctx    = ctx;

    return APR_SUCCESS;
}

char *
default_chxj_serf_get(request_rec *r, apr_pool_t *ppool, const char *url_path)
{
  apr_pool_t *pool;
  apr_uri_t url;
  apr_status_t rv;
  apr_sockaddr_t *address = NULL;

  serf_context_t *context;
  serf_connection_t *connection;

  app_ctx_t app_ctx;
  handler_ctx_t handler_ctx;
  char *ret;


  s_init(ppool, &pool);

  apr_uri_parse(pool, url_path, &url);
  if (!url.port) {
    url.port = apr_uri_port_of_scheme(url.scheme);
  }
  if (!url.port) {
    url.port = 80;
  }
  if (!url.path) {
    url.path = "/";
  }
  if (!url.hostname) {
    url.hostname = "localhost";
  }

  rv = apr_sockaddr_info_get(&address, url.hostname, APR_UNSPEC, url.port, 0, pool);
  if (rv != APR_SUCCESS) {
    char buf[256];
    ERR(r, "apr_sockaddr_info_get() failed: rv:[%d|%s]", rv, apr_strerror(rv, buf, 256));
    s_term(pool);
    return NULL;
  }
  memset(&app_ctx, 0, sizeof(app_ctx_t));

  app_ctx.bkt_alloc = serf_bucket_allocator_create(pool, NULL, NULL);
  if (strcasecmp(url.scheme, "https") == 0) {
    app_ctx.ssl_flag = 1;
  }

  context = serf_context_create(pool);
  connection = serf_connection_create(context, address, s_connection_setup, &app_ctx, s_connection_closed, &app_ctx, pool);

  memset(&handler_ctx, 0, sizeof(handler_ctx_t));
  handler_ctx.requests_outstanding = 0;
  handler_ctx.host = url.hostinfo;
  handler_ctx.method = "GET";
  handler_ctx.path = url.path;

  handler_ctx.acceptor     = s_accept_response;
  handler_ctx.acceptor_ctx = &app_ctx;
  handler_ctx.handler      = s_handle_response;

  serf_connection_request_create(connection, s_setup_request, &handler_ctx);

  while (1) {
    rv = serf_context_run(context, SERF_DURATION_FOREVER, pool);
    if (APR_STATUS_IS_TIMEUP(rv))
      continue;
    if (rv) {
      char buf[200];
      ERR(r, "Error running context: (%d) %s\n", rv, apr_strerror(rv, buf, sizeof(buf)));
      break;
    }
    if (!apr_atomic_read32(&handler_ctx.requests_outstanding)) {
      if (handler_ctx.rv != APR_SUCCESS) {
        char buf[200];
        ERR(r, "Error running context: (%d) %s\n", handler_ctx.rv, apr_strerror(handler_ctx.rv, buf, sizeof(buf)));
      }
      break;
    }
  }


  serf_connection_close(connection);
  ret = apr_pstrdup(ppool, handler_ctx.response);
  s_term(pool);
  return ret;
}
/*
 * vim:ts=2 et
 */
