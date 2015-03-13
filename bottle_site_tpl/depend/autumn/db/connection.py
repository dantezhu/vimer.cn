#ADD-BEGIN by dantezhu in 2012-02-10 02:30:57
from threading import local

local_data = local()
#ADD-END
        
class Database(object):
    placeholder = '?'
    
    def connect(self, dbtype, *args, **kwargs):
#Add-Begin by dantezhu in 2011-12-16 01:56:34
        self.dbtype = dbtype
#Add-End
        if dbtype == 'sqlite3':
            import sqlite3
            self.connection = sqlite3.connect(*args)
        elif dbtype == 'mysql':
            import MySQLdb
            self.connection = MySQLdb.connect(**kwargs)
            self.placeholder = '%s'

#ADD-BEGIN by dantezhu in 2012-02-10 02:30:40
    def __getattr__(self, key):
        if key == 'connection':
            if not hasattr(local_data, key):
                raise AttributeError, 'the object has no attr: ' + key
                return None

            return getattr(local_data, key)
        else:
            return object.__getattribute__(self, key)

    def __setattr__(self, key, value):
        if key == 'connection':
            setattr(local_data, key, value)
        else:
            object.__setattr__(self, key, value)
#ADD-END

class DBConn(object):
    def __init__(self):
        self.b_debug = False
        self.b_commit = True
        self.conn = None


autumn_db = DBConn()
autumn_db.conn = Database()
