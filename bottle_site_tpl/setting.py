#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from os.path import abspath, dirname, join
import logging

sys.path.insert(0, abspath(dirname(__file__)))
sys.path.insert(0, abspath(join(dirname(__file__), "depend/")))
sys.path.insert(0, abspath(join(dirname(__file__), "module/")))
sys.path.insert(0, abspath(join(dirname(__file__), "web/")))

#logging.basicConfig(level=logging.ERROR,
logging.basicConfig(level=logging.INFO,
                    format='[%(levelname)s][%(asctime)s][%(process)d][%(filename)s:%(lineno)d][%(funcName)s] %(message)s',
                    filename=abspath(join(dirname(__file__), "log/site.log")),
                    filemode='a+')


DATABASE = {
    'engine' : 'mysql',
    'db' : 'test_b',
    'user' : 'dantezhu',
    'passwd' : '',
    'host' : 'localhost',
    'port' : 3306,
}

#网站根域名
ROOT_DOMAIN = 'test.com'

#session相关
SECRET_KEY = 'secret_key_for_test'
SESSION_MAX_AGE = 7200

#管理员邮箱列表
ADMIN_EMAIL_LIST = ['zny2008@gmail.com']

#指定的模板路径
CUSTOM_TPL_PATH = abspath(join(dirname(__file__), "views/"))

#静态文件
STATIC_FILE_PATH = abspath(join(dirname(__file__), "static/"))
