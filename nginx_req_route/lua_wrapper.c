
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>

// 崩溃啊，这里一定要放到 ngx 自身的头文件后面，否则打印log的时候，就会core掉，震惊了
#include "lua_pub.h"

#define LUA_FILENAME "req_route.lua"

int get_route_id(ngx_http_request_t *r)
{
    const char lua_funcname[] = "get_route_id";

    ngx_http_request_body_t* rb = r->request_body;

    char* body = NULL;
    int body_size = 0;

    if (rb && rb->buf)
    {
        body = (char*)rb->buf->pos;
        body_size = rb->buf->last - rb->buf->pos;
    }

    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0))
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "cannot run configuration file: %s", lua_tostring(L, -1));
        lua_close(L);
        return -1;
    }

    lua_getglobal(L, lua_funcname); /* function to be called */
    lua_pushnumber(L, (int)r->method);
    lua_pushlstring(L, (char*)r->uri.data, r->uri.len);
    lua_pushlstring(L, (char*)r->args.data, r->args.len);
    lua_pushlstring(L, body, body_size);

    /* do the call (1 arguments, 1 result) */
    if (lua_pcall(L, 4, 1, 0) != 0)
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "error running function %s: %s", lua_funcname, lua_tostring(L, -1));
        lua_close(L);
        return -2;
    }

    /* retrieve result */
    if (!lua_isnumber(L, -1))
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "function %s must return a number", lua_funcname);
        lua_close(L);
        return -3;
    }

    int result = (int)lua_tonumber(L, -1);

    lua_pop(L, 1); /* pop returned value */

    lua_close(L);
    return result;

}
