#ifndef _STAT_REPORT_DEF_H_20120509044946_
#define _STAT_REPORT_DEF_H_20120509044946_

/* Module config */
typedef struct {
    ngx_str_t  host;
    ngx_int_t  port;
    ngx_str_t  collect_point;
} ngx_http_stat_report_conf_t;


#endif
