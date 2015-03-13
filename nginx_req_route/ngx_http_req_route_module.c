#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>

#define DFT_ROUTE_ID 433

extern int get_route_id(ngx_http_request_t *r);

static  char *ngx_http_req_route(ngx_conf_t *cf ,ngx_command_t *cmd,void *conf);
static ngx_command_t ngx_http_req_route_commands[]={
    {
        ngx_string("req_route"),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
        ngx_http_req_route,
        0,
        0,
        NULL
    },
    ngx_null_command
};
static ngx_http_module_t ngx_http_req_route_module_ctx={
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
ngx_module_t ngx_http_req_route_module={
    NGX_MODULE_V1,
    &ngx_http_req_route_module_ctx,
    ngx_http_req_route_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};


void ngx_http_foo_post_handler(ngx_http_request_t *r){
    // 请求全部读完后从这里入口, 可以产生响应
    /*ngx_http_finalize_request(r, NGX_HTTP_OK);*/

    int result = get_route_id(r);
    ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "get_route_id result:%d", result);
    if (result < 0)
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "get_route_id fail, result:%d", result);
        result = DFT_ROUTE_ID;
    }
    ngx_http_finalize_request(r, result);
}

static ngx_int_t ngx_http_req_route_handler(ngx_http_request_t *r)
{
    ngx_http_read_client_request_body(r, ngx_http_foo_post_handler);
    return NGX_DONE; // 主handler结束
}
static char *ngx_http_req_route(ngx_conf_t *cf,ngx_command_t *cmd,void *conf)
{
    ngx_http_core_loc_conf_t *clcf;

    clcf=ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
    clcf->handler=ngx_http_req_route_handler;
    return NGX_CONF_OK;
}
