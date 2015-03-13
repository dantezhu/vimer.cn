/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fsm.h
#  Description:     
#  Version:         1.0
#  LastChange:      2011-01-19 12:13:02
#  History:         
=============================================================================*/
#ifndef _FSM_H_
#define _FSM_H_
#include <error.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "fsm_interface.h"
using namespace std;
#define foreach(container,it) \
    for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)

class CBaseActor : public IActor
{
public:
    CBaseActor () {
        m_Fsm = NULL;
        m_mapFsmMgr = NULL;
    }
    virtual ~CBaseActor () {}

    int AttachFsmMgr(map<int, IFsm*> * mapFsmMgr)
    {
        m_mapFsmMgr = mapFsmMgr;
        return 0;
    }

    int ChangeState(int destState)
    {
        if (m_mapFsmMgr == NULL)
        {
            return -1;
        }

        if (0 == destState)
        {
            //此次处理结束
            return 0;
        }
        else if (destState < 0)
        {
            //需要关闭整个请求
            return destState;
        }
        IFsm * destFsm = NULL;
        destFsm = (*m_mapFsmMgr)[destState];
        int state = doChangeFsm(destFsm);
        return ChangeState(state);
    }
private:
    int doChangeFsm(IFsm* destFsm)
    {
        if (destFsm == NULL)
        {
            return 0;
        }

        if (m_Fsm != destFsm)
        {
            if (m_Fsm != NULL)
            {
                m_Fsm->Fini(this);
            }
            m_Fsm = destFsm;
            m_Fsm->Init(this);
        }
        return m_Fsm->Process(this);
    }


protected:
    IFsm* m_Fsm;
    map<int, IFsm*> *m_mapFsmMgr;
};
//=============================================================================

class CSocketActor : public CBaseActor
{
public:
    CSocketActor () {}
    virtual ~CSocketActor () {}

    int HandleSend()
    {
        cout<<"sending"<<endl;
        if (rand() % 5 == 0)
        {
            //代表发送完了
            return 1;
        }
        return 0;
    }
    int HandleRecv()
    {
        cout<<"recving"<<endl;
        return 0;
    }
    int HandleError()
    {
        cout<<"error"<<endl;
        return 0;
    }
    int HandleTimeout()
    {
        cout<<"timeout"<<endl;
        return 0;
    }
};

//=============================================================================

//waitsend sending sendover waitrecv recving recvover waitclose closing closeover error timeout

class CWaitSendFsm : public IFsm
{
public:
    CWaitSendFsm () {}
    virtual ~CWaitSendFsm () {}
    virtual int Init(IActor* obj)
    {
        cout<<"Init WaitSend"<<endl;
        return 0;
    }
    virtual int Process(IActor* obj)
    {
        cout<<"Process WaitSend"<<endl;
        return 0;
    }
    virtual int Fini(IActor* obj)
    {
        cout<<"Fini WaitSend"<<endl;
        return 0;
    }
};
class CSendingFsm : public IFsm
{
public:
    CSendingFsm () {}
    virtual ~CSendingFsm () {}
    virtual int Init(IActor* obj)
    {
        cout<<"Init Sending"<<endl;
        return 0;
    }
    virtual int Process(IActor* obj)
    {
        cout<<"Process Sending"<<endl;
        CSocketActor * chirdObj = (CSocketActor*) obj;
        int ret = chirdObj->HandleSend();
        if (ret == 1)
        {
            return 3;
        }
        return 0;
    }
    virtual int Fini(IActor* obj)
    {
        cout<<"Fini Sending"<<endl;
        return 0;
    }
};
class CSendOverFsm : public IFsm
{
public:
    CSendOverFsm () {}
    virtual ~CSendOverFsm () {}
    virtual int Init(IActor* obj)
    {
        cout<<"Init SendOver"<<endl;
        return 0;
    }
    virtual int Process(IActor* obj)
    {
        cout<<"Process SendOver"<<endl;
        return 0;
    }
    virtual int Fini(IActor* obj)
    {
        cout<<"Fini SendOver"<<endl;
        return 0;
    }
};

//=============================================================================

class CWorld
{
public:
    CWorld () {
        srand(time(NULL));
    }
    virtual ~CWorld () {}

    int Init()
    {
        int count = 10;

        m_mapFsmMgr[1]=new CWaitSendFsm();
        m_mapFsmMgr[2]=new CSendingFsm();
        m_mapFsmMgr[3]=new CSendOverFsm();

        for (int i = 0; i < count; i++)
        {
            IActor * actor = new CSocketActor();
            m_vecActors.push_back(actor);

            actor->AttachFsmMgr(&m_mapFsmMgr);
            actor->ChangeState(1);
        }
        return 0;
    }
    int Run()
    {
        while (true)
        {
            int state = 0;
            int val = rand() % 5;
            switch(val)
            {
                case 0:
                case 1:
                    state = 1;
                    break;
                case 2:
                    state = 2;
                    break;
                default:
                    state = 3;
                    break;
            }
            foreach (m_vecActors, it)
            {
                (*it)->ChangeState(state);   
            }
            sleep(1);
        }
        return 0;
    }

private:
    vector<IActor*> m_vecActors;
    map<int, IFsm* > m_mapFsmMgr;
};

//=============================================================================
#endif
