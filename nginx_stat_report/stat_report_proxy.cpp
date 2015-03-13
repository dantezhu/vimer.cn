/*=============================================================================
#
#     FileName: stat_report_proxy.cpp
#         Desc: 
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2012-05-09 04:33:45
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2012-05-09 04:33:45 | initialization
#
=============================================================================*/

extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>
}

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "OpenAPIMonitorClient.h"
#include "stat_report_proxy.h"
using namespace std;
using namespace OPENAPI_MONITOR_CLIENT_V1;

#ifndef foreach
#define foreach(container,it) \
    for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif

template <typename T> 
int TransType(const string &src, T& dst)
{
    if (src.size() <= 0)
    {
        dst = 0;
        return 0;
    }

    stringstream ss;
    ss << src;
    ss >> dst;
    return 0;
}

int TransType(const string &src, string& dst)
{
    dst = src;
    return 0;
}

template <typename T> 
int SplitString(const string &src,const string &seq,vector<T> &dst, uint32_t max_size=-1)
{
    if(src.size()==0)
    {
        return 0;
    }
    size_t old_pos=0,new_pos=0;
    string tmp_str;
    T tmp_val;
    while(1)
    {
        if (max_size>0 && dst.size()>max_size)
        {
            break;
        }

        new_pos=src.find(seq,old_pos);
        if(new_pos!=string::npos)
        {
            tmp_str = src.substr(old_pos,new_pos-old_pos);
            TransType(tmp_str, tmp_val);
            dst.push_back(tmp_val);
            old_pos=new_pos+seq.size();
        }
        else if(old_pos<=src.size())
        {
            tmp_str= src.substr(old_pos);
            TransType(tmp_str, tmp_val);
            dst.push_back(tmp_val);
            break;
        }
        else
        {
            break;
        }
    }
    return 0;
}

//获取经过时间
static ngx_msec_int_t get_pass_time_ms(ngx_http_request_t *r)
{
    ngx_time_t      *tp;
    ngx_msec_int_t   ms;

    tp = ngx_timeofday();

    ms = (ngx_msec_int_t)
             ((tp->sec - r->start_sec) * 1000 + (tp->msec - r->start_msec));
    ms = ngx_max(ms, 0);

    return ms;
}

//=============================================================================
//http解析相关

//将http字符串解析成map
int Trans2MapParams(const string& str_data, map<string, string>& params)
{
    vector<string> tmp_vec;
    SplitString(str_data, "&", tmp_vec);

    foreach(tmp_vec, it)
    {
        vector<string> t;
        SplitString(*it, "=", t);
        if (t.size() != 2)
        {
            continue;
        }

        params[t[0]] = t[1];
    }

    return 0;
}

//解析http protocol
//原来是 HTTP/1.1 Host
string ParseProtocol(const string& str_org_protocol)
{
    return str_org_protocol.substr(0, str_org_protocol.find('/'));
}
//=============================================================================

// 检查配置是否齐全
int CheckStatReportConf(ngx_http_request_t* r, ngx_http_stat_report_conf_t* conf)
{
    if (!conf ||
        conf->host.len == 0 ||
        conf->port == 0 ||
        conf->collect_point.len == 0)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "not conf stat_report");
        return 1;
    }

    return 0;
}

//转化成OpenAPIMoinitor使用的格式
int Trans2OpenAPIMonitorStruct(ngx_http_request_t* r, ngx_http_stat_report_conf_t* conf, SPackage& pkg)
{
    ngx_http_request_body_t* rb = r->request_body;

    char* body = NULL;
    int body_size = 0;

    if (rb && rb->buf)
    {
        body = (char*)rb->buf->pos;
        body_size = rb->buf->last - rb->buf->pos;
    }

    string str_uri = r->uri.data ? string((char*)r->uri.data,r->uri.len) : "";

    string protocol = r->http_protocol.data ? string((char*)r->http_protocol.data, r->http_protocol.len) : "";

    string str_data;

    map<string,string> params;

    if (r->method == 2) // get
    {
        pkg.method = "GET";
        str_data = r->args.data ? string((char*)r->args.data, r->args.len) : "";
    }
    else if (r->method == 8)
    {
        pkg.method = "POST";
        str_data = (body && body_size>0) ? string(body, body_size) : "";
    }
    else
    {
        return -1;
    }
    //ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "args:%s uri:%s protocol:%s end", str_data.c_str(), str_uri.c_str(), protocol.c_str());

    Trans2MapParams(str_data, params);

    ngx_msec_int_t ms = get_pass_time_ms(r);
    double time_sec = ((double)ms)/1000;

    pkg.appid = strtoul(params["appid"].c_str(), NULL, 10);
    pkg.rc = 0;
    if (r->headers_out.status >= 400) // 就代表有问题
    {
        pkg.rc = r->headers_out.status;
    }
    pkg.timestamp = time(NULL);
    pkg.time = time_sec;
    pkg.pf = params["pf"];
    //转发的IP和port
    pkg.svr_name = "";
    if (r->upstream && r->upstream->peer.name && r->upstream->peer.name->data)
    {
        pkg.svr_name = string((char*)r->upstream->peer.name->data, r->upstream->peer.name->len);
    }
    pkg.interface = str_uri;
    pkg.protocol = ParseProtocol(protocol);
    pkg.collect_point = conf->collect_point.data ? string((char*)conf->collect_point.data, conf->collect_point.len) : "";

    /*ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "appid:%d, rc:%d, timestamp:%d, pass_time_ms:%d, pf:%s, svr_name:%s, interface:%s, protocol:%s, collect_point:%s",
                  pkg.appid,
                  pkg.rc,
                  pkg.timestamp,
                  ms,
                  pkg.pf.c_str(),
                  pkg.svr_name.c_str(),
                  pkg.interface.c_str(),
                  pkg.protocol.c_str(),
                  pkg.collect_point.c_str()
                  );*/

    return 0;
}

// 发送往OpenAPIMoinitor
int Send2OpenAPIMonitor(ngx_http_request_t* r, ngx_http_stat_report_conf_t* conf, SPackage& pkg)
{
    //ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "host:%s, port:%d, collect_point:%s", conf->host.data, conf->port, conf->collect_point.data);

    static COpenApiMonitorClient client;

    SServerInfo server_info;
    server_info.strIP = (char*)conf->host.data;
    server_info.usPort = (uint16_t)conf->port;

    int ret = client.Report(pkg, server_info);

    if (ret)
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "COpenApiMonitorClient report fail.ret:%d, msg:%s", ret, client.GetErrMsg().c_str());
    }

    return ret;
}

// 对外提供的函数
int SendStatReport(ngx_http_request_t *r, ngx_http_stat_report_conf_t* conf)
{
    int ret;
    
    ret = CheckStatReportConf(r, conf);
    if (ret)
    {
        return 1;
    }

    SPackage pkg;

    ret = Trans2OpenAPIMonitorStruct(r, conf, pkg);
    if (ret)
    {
        return 2;
    }

    ret = Send2OpenAPIMonitor(r, conf, pkg);
    if (ret)
    {
        return 3;
    }

    return 0;
}
