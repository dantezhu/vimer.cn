#!/usr/bin/python
# -*- coding: utf-8 -*-

from autumn.db.connection import autumn_db

from setting import DATABASE

def connect_db():
    #import thread
    #print thread.get_ident(), 'try connect'
    if hasattr(autumn_db.conn, 'connection') and autumn_db.conn.connection:
        #print thread.get_ident(), 'has init connection'
        return
        
    if DATABASE['engine'] == 'mysql':
        autumn_db.conn.connect(
            DATABASE['engine'],
            db=DATABASE['db'],
            user=DATABASE['user'],
            passwd=DATABASE['passwd'],
            host=DATABASE['host'],
            port=DATABASE['port'],
            use_unicode=True
            )
    elif DATABASE['engine'] == 'sqlite3':
        autumn_db.conn.connect(
            DATABASE['engine'],
            DATABASE['db']
            )
    else:
        raise TypeError, "DATABASE['engine'] is invalid"
