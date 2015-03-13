/*=============================================================================
#
#     FileName: test4.cpp
#         Desc: 测试一下ptr_proxy
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-06-13 15:47:32
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-06-13 15:47:32 | initialization
#
=============================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "ptr_proxy.h"

using namespace std;

class A : public IPtrProxy<A>
{
public:
    A() {}
    virtual ~A() {}
};

class B : public IPtrProxy<B>
{
public:
    B() {}
    virtual ~B() {}

    void print()
    {
        printf("this is print\n");
    }

    void SetAPtr(const ptr_proxy<A>& pptr)
    {
        m_ptr_a = pptr;
    }

    void check()
    {
        if (m_ptr_a.is_null())
        {
            printf("is null\n");
        }
        else
        {
            printf("is not null\n");
        }
    }

    ptr_proxy<A> m_ptr_a;
};

int main(int argc, char **argv)
{
    A* a = new A();
    B* b = new B();
    b->SetAPtr(a->get_ptr_proxy());
    delete a;
    b->check();

    b->get_ptr_proxy().true_ptr()->print();
    delete b;
    return 0;
}
