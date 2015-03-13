/*=============================================================================
#
#     FileName: test.cpp
#         Desc: 
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-03-08 20:17:09
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-03-08 20:17:09 | initialization
#
=============================================================================*/
/*
CREATE TABLE `tb_test` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(32) default "",
  `sex` int(11) default 0,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`)
);
*/

#include "soci.h"
#include "soci-mysql.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>

#include <gtest/gtest.h>

using namespace soci;
using namespace std;

#define foreach(container,it) \
    for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)

//=============================================================================
struct Person
{
    int id;
    std::string name;
    int sex;
};

namespace soci
{
    template<> struct type_conversion<Person>
    {
        typedef values base_type;
        static void from_base(values const & v, indicator /* ind */, Person & p)
        {
            p.id = v.get<int>("id");
            p.name = v.get<std::string>("name");
            p.sex = v.get<int>("sex");
        }
        static void to_base(const Person & p, values & v, indicator & ind)
        {
            v.set("id", p.id);
            v.set("name", p.name);
            v.set("sex", p.sex);
            ind = i_ok;
        }
    };
}
//=============================================================================

TEST(soci,select_one)
{
    try
    {
        session sql(mysql, "host=localhost db=soci user=dantezhu");
        indicator ind;

        string name = "dandan";
        int sex;
        sql << "select sex from tb_test where name = :name",
            into(sex, ind), use(name);

        ASSERT_EQ(ind, i_ok) << name;
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}

TEST(soci,select_multi)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");
        indicator ind;

        int count;
        sql << "select count(*) from tb_test", into(count, ind);
        ASSERT_EQ(ind, i_ok) << count;

        if (count == 0)
        {
            SUCCEED();
            return;
        }

        int sex = 1;
        vector<string> vec_name(count);
        vector<indicator> vec_ind;
        sql << "select name from tb_test where sex= :sex",
            into(vec_name, vec_ind), use(sex);

        foreach(vec_ind, it)
        {
            ASSERT_EQ(*it, i_ok) << sex << "," << it-vec_ind.begin();
        }
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}

TEST(soci,select_multi2)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");
        indicator ind;

        int count;
        sql << "select count(*) from tb_test", into(count, ind);
        ASSERT_EQ(ind, i_ok) << count;

        if (count == 0)
        {
            SUCCEED();
            return;
        }

        int sex = 1;
        vector<string> vec_name(count);
        vector<int> vec_sex(count);
        sql << "select name,sex from tb_test where sex = :sex",
            into(vec_name), into(vec_sex), use(sex);
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}

TEST(soci,insert_noexist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        string name = "dandan";
        int sex = 1;

        sql << "delete from tb_test where name = :name",use(name);

        sql << "insert into tb_test(name, sex) values(:name, :sex)",
            use(name), use(sex);
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}

TEST(soci,insert_exist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        string name = "dandan";
        int sex = 1;

        sql << "insert into tb_test(name, sex) values(:name, :sex)",
            use(name), use(sex);
    }
    catch (exception const &e)
    {
        SUCCEED()<<e.what();
    }
}

TEST(soci,update_exist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        string name = "dandan";
        int sex = 2;

        sql << "update tb_test set sex=:sex where name=:name",
            use(sex), use(name);

    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}

TEST(soci,update_noexist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        string name = "niuniu";
        int sex = 2;

        sql << "update tb_test set sex=:sex where name=:name",
            use(sex), use(name);
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}
/*TEST(soci,delete)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        string name = "dandan";

        sql << "delete from tb_test where name = :name",use(name);
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}*/
//=============================================================================
TEST(soci,select_obj_one)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");
        indicator ind;

        int count;
        sql << "select count(*) from tb_test", into(count, ind);
        ASSERT_EQ(ind, i_ok) << count;

        string name = "dandan";
        Person p;
        sql << "select id,name,sex from tb_test where name = :name",
            into(p, ind), use(name);

        ASSERT_EQ(ind, i_ok) << name;

        if (sql.got_data())
        {
            cout<< p.id 
                << "," 
                << p.name 
                << "," 
                << p.sex 
                << endl;
        }

    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}
TEST(soci,insert_obj_noexist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        Person p = {
            0,
            "niuniu",
            2
        };

        sql << "delete from tb_test where name = :name",use(p.name);

        sql << "insert into tb_test(name, sex) values(:name, :sex)",
            use(p);
    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}
TEST(soci,update_obj_exist)
{
    try
    {
        session sql(mysql, "db=soci user=dantezhu");

        Person p = {
            0,
            "niuniu",
            3
        };

        sql << "update tb_test set sex=:sex where name=:name",
            use(p);

    }
    catch (exception const &e)
    {
        FAIL()<<e.what();
    }
}
