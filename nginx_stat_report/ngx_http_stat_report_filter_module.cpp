extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>
}

#include "stat_report_def.h"
#include "stat_report_proxy.h"

static ngx_int_t
ngx_http_stat_report_filter_init(ngx_conf_t *cf);

static void *
ngx_http_stat_report_create_loc_conf(ngx_conf_t *cf);
static char *
ngx_http_stat_report_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);

static ngx_command_t  ngx_http_stat_report_filter_commands[] = {
    { ngx_string("stat_report_host"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_stat_report_conf_t, host),
        NULL },

    { ngx_string("stat_report_port"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_stat_report_conf_t, port),
        NULL },

    { ngx_string("stat_report_collect_point"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_stat_report_conf_t, collect_point),
        NULL },

    ngx_null_command
};

static ngx_http_module_t  ngx_http_stat_report_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_stat_report_filter_init,          /* postconfiguration */
    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */
    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */
    ngx_http_stat_report_create_loc_conf,                                    /* create location configuration */
    ngx_http_stat_report_merge_loc_conf                                   /* merge location configuration */
};

ngx_module_t  ngx_http_stat_report_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_stat_report_filter_module_ctx,   /* module context */
    ngx_http_stat_report_filter_commands,      /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt ngx_http_next_body_filter;


static ngx_int_t
ngx_http_stat_report_header_filter(ngx_http_request_t *r)
{
    ngx_http_stat_report_conf_t  *conf;

    conf = (ngx_http_stat_report_conf_t  *)ngx_http_get_module_loc_conf(r, ngx_http_stat_report_filter_module);

    SendStatReport(r, conf);

    return ngx_http_next_header_filter(r);
}

static ngx_int_t  
ngx_http_stat_report_body_filter(ngx_http_request_t *r, ngx_chain_t *in)  
{  
    return ngx_http_next_body_filter(r, in);  
}

static ngx_int_t
ngx_http_stat_report_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_stat_report_header_filter;
    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_stat_report_body_filter;
    return NGX_OK;
}

static void *
ngx_http_stat_report_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_stat_report_conf_t  *conf;

    conf = (ngx_http_stat_report_conf_t  *)ngx_pcalloc(cf->pool, sizeof(ngx_http_stat_report_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    conf->host.len = 0;
    conf->host.data = NULL;

    conf->port = NGX_CONF_UNSET;

    conf->collect_point.len = 0;
    conf->collect_point.data = NULL;

    return conf;
}

static char *
ngx_http_stat_report_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_stat_report_conf_t *prev = (ngx_http_stat_report_conf_t *)parent;
    ngx_http_stat_report_conf_t *conf = (ngx_http_stat_report_conf_t *)child;

    ngx_conf_merge_str_value(conf->host, prev->host, "");
    ngx_conf_merge_value(conf->port, prev->port, 0);
    ngx_conf_merge_str_value(conf->collect_point, prev->collect_point, "");

    return NGX_CONF_OK;
}


//=============================================================================
