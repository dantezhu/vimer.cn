drop table if exists frame;
create table frame (
    id integer primary key autoincrement,
    content text,
    entry_id integer not null,
    order_index float default 0,
    create_time datetime not null,
    up_num integer not null default 0
);
