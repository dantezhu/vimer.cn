/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        interfaces.h
#  Description:     公共接口
#  Version:         1.0
#  LastChange:      2011-01-19 23:24:33
#  History:         
=============================================================================*/
#ifndef _INTERFACES_H_
#define _INTERFACES_H_
#include <iostream>
#include <map>
using namespace std;

class IFsm;

class IActor
{
public:
    virtual ~IActor() {}
    virtual int AttachFsmMgr(map<int, IFsm*> *mapFsmMgr)=0;
    virtual int ChangeState(int destState)=0;
};


class IFsm
{
public:
    virtual ~IFsm() {}
    /**
     * @brief   在进入这个状态的时候，obj需要做的事情
     *
     * @param   obj
     *
     * @return  0           succ
     *          else        fail
     */
    virtual int Init(IActor* obj)=0;

    /**
     * @brief   执行这个状态该做的事情
     *
     * @param   obj
     *
     * @return  应该进入的另一个状态
     *          0           结束本次Process执行，不进入其他状态
     *          <0          结束整个请求（obj需要被后续删除）
     *          else        其他状态（可以返回自己，但是会造成循环，有点危险）
     *          
     */
    virtual int Process(IActor* obj)=0;

    /**
     * @brief   退出这个状态时需要做的事情
     *
     * @param   obj
     *
     * @return  0           succ
     *          else        fail
     */
    virtual int Fini(IActor* obj)=0;
};

#endif
