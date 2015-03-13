import setting


def init_db():
    from autumn.db.connection import autumn_db

    from module.connect_db import connect_db

    connect_db()

    db = autumn_db.conn.connection

    c = db.cursor()

    data = file('conf/schema.sql').read()
    
    if hasattr(c, 'executescript') and callable(c.executescript):
        c.executescript(data)
        db.commit()
    else:
        cmds = data.split(';')
        for it in cmds:
            cmd = it.strip()
            if cmd and not cmd.startswith('--'):
                c.execute(cmd)
        db.commit()


def main():
    init_db()
    print 'init_db over'

if __name__ == '__main__':
    main()
