/*=============================================================================
#
#     FileName: ptr_proxy.h
#         Desc: 这个类的作用，就是为了解决互指指针，不知道对方已经析构的问题
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-06-13 15:24:12
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-06-13 15:24:12 | initialization
#
=============================================================================*/

#ifndef __PTR_PROXY_H__
#define __PTR_PROXY_H__
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
using namespace std;

template <typename T>
class ptr_proxy
{
public:
    ptr_proxy(const T* pobj=NULL) : m_ppobj(NULL), m_pcount(NULL)
    {
        init(pobj);
    }

    ptr_proxy(const ptr_proxy& rhs) // 拷贝构造函数
    {
        m_ppobj = rhs.m_ppobj; // 指向同一块内存
        m_pcount = rhs.m_pcount; // 使用同一个计数值
        add_count();
    }

    virtual ~ptr_proxy()
    {
        dec_count();
    }

    /**
     * @brief   如果指向的对象被释放了，一定要调用这个函数让他知道
     */
    void set2null()
    {
        if (m_ppobj)
        {
            (*m_ppobj) = NULL;
        }
    }

    /**
     * @brief   copy构造函数
     *
     * @param   rhs         被拷贝对象
     *
     * @return  自己的引用
     */
    ptr_proxy& operator=(const ptr_proxy& rhs)
    {
        if( m_ppobj == rhs.m_ppobj ) // 首先判断是否本来就指向同一内存块
            return *this; // 是则直接返回

        dec_count();

        m_ppobj = rhs.m_ppobj; // 指向同一块内存
        m_pcount = rhs.m_pcount; // 使用同一个计数值
        add_count();

        return *this; // 是则直接返回
    }

    ptr_proxy& operator=(const T* pobj)
    {
        if(m_ppobj && *m_ppobj == pobj) // 首先判断是否本来就指向同一内存块
            return *this; // 是则直接返回
        dec_count();

        init(pobj);

        return *this;
    }

    /**
     * @brief   获取内部关联的obj的指针
     *
     * @return
     */
    T* true_ptr()
    {
        if (m_ppobj)
        {
            return *m_ppobj;
        }
        else
        {
            return NULL;
        }
    }

    /**
     * @brief   获取内部关联的obj的指针
     *
     * @return
     */
    T* operator*()
    {
        return true_ptr();
    }

    /**
     * @brief   获取内部关联的obj的个数
     *
     * @return  个数
     */
    int count()
    {
        if (m_pcount != NULL)
        {
            return *m_pcount;
        }
        return 0;
    }

    /**
     * @brief   判断智能指针是否为空
     *
     * @return
     */
    bool is_null()
    {
        if (m_ppobj == NULL || (*m_ppobj) == NULL)
        {
            return true;
        }
        return false;
    }

protected:
    void init(const T* pobj)
    {
        if (pobj == NULL)
        {
            m_ppobj = NULL;
            m_pcount = NULL;
            return;
        }
        m_ppobj = new (T*)();
        *m_ppobj = (T*)pobj;
        m_pcount = new int(); // 初始化计数值为 1
        *m_pcount = 1;
    }

    void add_count()
    {
        if (m_pcount == NULL)
        {
            return;
        }
        (*m_pcount) ++;
    }

    /**
     * @brief   计数减1
     */
    void dec_count()
    {
        if (m_pcount == NULL || m_ppobj == NULL)
        {
            return;
        }

        (*m_pcount)--; // 计数值减 1 ，因为该指针不再指向原来内存块了
        if( *m_pcount <= 0 ) // 已经没有别的指针指向原来内存块了
        {
            //我们不去主动析构对象
            //free_sptr(*m_ppobj);//把对象析构
            if (m_ppobj != NULL)
            {
                delete m_ppobj;
                m_ppobj = NULL;
            }

            if (m_pcount != NULL)
            {
                delete m_pcount;
                m_pcount = NULL;
            }
        }
    }

protected:
    T** m_ppobj;
    int* m_pcount;
};

template <typename T>
class IPtrProxy
{
public:
    IPtrProxy() {
        m_ptr_proxy = (T*)this;
    }
    virtual ~IPtrProxy() {
        m_ptr_proxy.set2null();
    }

    ptr_proxy<T>& get_ptr_proxy()
    {
        return m_ptr_proxy;
    }

protected:
    ptr_proxy<T> m_ptr_proxy;
};

#endif
