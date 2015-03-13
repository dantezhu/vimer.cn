/*=============================================================================
#
#     FileName: stat_report_proxy.h
#         Desc: 上报数据
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2012-05-09 04:22:46
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2012-05-09 04:22:46 | initialization
#
=============================================================================*/
#ifndef _STAT_REPORT_PROXY_H_20120509042414_
#define _STAT_REPORT_PROXY_H_20120509042414_


extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>
}

#include "stat_report_def.h"

// method: 2: get, 8: post
int SendStatReport(ngx_log_t *log, ngx_http_stat_report_conf_t* conf, 
                   int method, char* uri, char* args, char* body, int body_size, 
                   unsigned pass_time_ms);

int SendStatReport(ngx_http_request_t *r, ngx_http_stat_report_conf_t* conf);

#endif
