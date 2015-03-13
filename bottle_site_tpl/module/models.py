#!/usr/bin/python
# -*- coding: utf-8 -*-

import datetime

from autumn.model import Model

from connect_db import connect_db

#初始化DB连接
connect_db()

class Frame(Model):
    '''帧'''
    class Meta:
        defaults = {
            'create_time': datetime.datetime.now,
            'up_num':0,
        }
