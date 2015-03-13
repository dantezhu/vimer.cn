#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import logging

from bottle import request, response, redirect

from setting import SECRET_KEY, SESSION_MAX_AGE, ADMIN_EMAIL_LIST

session_key_name = 'session_key'

def get_session_info():
    data = request.get_cookie(session_key_name, secret=SECRET_KEY)

    try:
        session_info = json.loads(data)
    except Exception, e:
        logging.info('exception occur.msg[%s], traceback[%s]' % (str(e), __import__('traceback').format_exc()))
        return None
    else:
        return session_info

def set_session_info(**kwargs):
    session_info = {}

    session_info.update(kwargs)

    data = json.dumps(session_info)

    response.set_cookie(session_key_name, data, secret=SECRET_KEY, max_age=SESSION_MAX_AGE, path='/')

def del_session_info():
    response.delete_cookie(session_key_name, path='/')


def check_admin():
    session_info = get_session_info()
    if session_info is None:
        return -1
    if 'email' not in session_info or session_info['email'] not in ADMIN_EMAIL_LIST:
        return 1

    return 0


def deco_session_check(func):
    def func_wrapper(*args, **kargs):
        session_info = get_session_info()
        if session_info is None:
            return redirect('/auth/login')
        return func(*args, **kargs)
    return func_wrapper

def deco_admin_check(func):
    def func_wrapper(*args, **kargs):
        ret = check_admin()
        if ret == -1:
            return redirect('/auth/login')
        elif ret == 1:
            return '请使用管理员账户登录'
        return func(*args, **kargs)
    return func_wrapper
