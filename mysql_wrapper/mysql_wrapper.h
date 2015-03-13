/*=============================================================================
#
#     FileName: mysql_wrapper.h
#         Desc: mysql��cpp��װ�汾
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-03-09 19:10:45
#      Version: 0.0.2
#      History:
#               0.0.2 | dantezhu | 2011-03-15 20:16:05 | �ж��ַ����ĳ���,��ֹ
#                                                      | ת��Ϊ���ֵ
#               0.0.1 | dantezhu | 2011-03-09 19:10:45 | initialization
#
=============================================================================*/
#ifndef __MYSQL_WRAPPER_H__
#define __MYSQL_WRAPPER_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "mysql.h"

using namespace std;

#define MYSQL_WRAPPER_DFT_PORT      3306

//ͳһ���󷵻���
typedef enum {
    //�ɹ�
    EMYSQLSuc = 0,

    //ϵͳ����
    EMYSQLErrSys = -1000,           //�Ƿ�����
    EMYSQLErrSysMem,                //�ڴ����ʧ��
    EMYSQLErrSysPtr,                //ָ�����

    //DB����
    EMYSQLErrDBInit = -2000,        //DB��ʼ��ʧ��
    EMYSQLErrDBConn,                //DB����ʧ��
    EMYSQLErrDBExe,                 //DBִ��ʧ��
    EMYSQLErrDBRes,                 //DBResultΪ��
} EMYSQLRetcode;


//������resultʵ���Զ�����
//1.���������������Ӱ��mysql������
//2.��ʹmysql�رգ����bufҲ�����ֶ�����
typedef struct _StMYSQLRes
{
public:
    _StMYSQLRes(MYSQL_RES* result)
    {
        m_result = result;
    }
    ~_StMYSQLRes()
    {
        if(NULL != m_result)
        {
            mysql_free_result(m_result);
            m_result = NULL;
        }
    }

private:
    MYSQL_RES *m_result;
}StMYSQLRes;

class MYSQLValue
{
public:
    MYSQLValue() {
        m_bNull = true;
    }
    virtual ~MYSQLValue() {}

    //��������
    int SetData(const char* pData, int iLen)
    {
        if (!pData)
        {
            m_bNull = true;
            return 0;
        }

        m_bNull = false;

        m_strData.resize(iLen+1);
        memcpy((char*)m_strData.data(), pData, iLen);
        ((char*)m_strData.data())[iLen] = '\0';

        return 0;
    }

    int size()
    {
        if (m_bNull)
        {
            return -1;
        }
        return m_strData.size() - 1;
    }

    const char* data()
    {
        if (m_bNull)
        {
            return NULL;
        }
        return m_strData.data();
    }

    template <typename T> 
    T as()
    {
        T asVal;
        pri_as(asVal);

        return asVal;
    }

private:
    int pri_as(char *& val)
    {
        val = (char*)data();
        return 0;
    }
    int pri_as(const char *& val)
    {
        val = data();
        return 0;
    }
    int pri_as(char& val)
    {
        int32_t tmp_val;
        int ret = pri_as(tmp_val);
        val = tmp_val;
        return ret;
    }
    int pri_as(unsigned char& val)
    {
        uint32_t tmp_val;
        int ret = pri_as(tmp_val);
        val = tmp_val;
        return ret;
    }
    int pri_as(string& val)
    {
        if (m_bNull)
        {
            val = "";
        }
        val = m_strData.data();
        return 0;
    }
    template <typename T> 
    int pri_as(T& val)
    {
//Add-Begin by dantezhu in 2011-03-15 20:15:19
//Ҫ��Ȼ����ǿմ��Ļ���ת�ɵ�int�����ֵ
        if (this->size() <= 0)
        {
            val = 0;
            return 0;
        }
//Add-End
        stringstream ss;
        ss << this->data();
        ss >> val;
        return 0;
    }

private:
    string m_strData;

    bool m_bNull;
};

class CMYSQLWrapper
{
public:
    /**
     * @brief   ��̬�������õ��������±��ӳ��
     *
     * @param   result          ȡ����result���
     * @param   mapName2Index   ӳ������
     *
     * @return  0               succ
     *          else            fail
     */
    static int MapName2Index(MYSQL_RES* result, map<string, int>& mapName2Index);

public:
    CMYSQLWrapper();
    virtual ~CMYSQLWrapper();

    /**
     * @brief   ��ȡ������Ϣ
     *
     * @return  ������Ϣ
     */
    const char* GetErrMsg();

    /**
     * @brief   ��ʼ��MYSQL������������������
     *
     * @param   ip          IP
     * @param   user        �û���
     * @param   pwd         ����(û����NULL)
     * @param   db          ��(û����NULL)
     * @param   port        �˿�
     * @param   charset     ����(NULL,utf8,gb2312)
     *
     * @return  0           succ
     *          else        fail
     */
    int Init(const char* ip, const char* user, const char* pwd, const char* db, uint32_t port=MYSQL_WRAPPER_DFT_PORT, const char* charset=NULL);

    /**
     * @brief   ����MYSQL���Ѿ�֧�����Զ�����ģʽ����mysql server�ر����ӻ��Զ�����
     *
     * @return  0           succ
     *          else        fail
     */
    int Open();

    /**
     * @brief   �ر����Ӳ��ͷ�result
     */
    void Close();

    /**
     * @brief   ִ��SQL���
     *
     * @param   strSql      ִ�����
     * @param   result      ִ�н��
     *
     * @return  0           succ
     *          else        fail
     */
    int Query(const char* strSql);
    int Query(const string& strSql);

    /**
     * @brief   ���Read(select)��صĵ�Query������֧��blob��
     *
     * @param   strSql          sql���
     * @param   vecData         rows
     *
     * @return  0               succ
     *          else            fail
     */
    int Query(const char* strSql, vector<map<string, MYSQLValue> > &vecData);
    int Query(const string& strSql, vector<map<string, MYSQLValue> > &vecData);

    /**
     * @brief   ���Write(insert,update,delete)��ص�Query
     *
     * @param   strSql          sql���
     * @param   affectedRows    Ӱ����еĸ���
     *
     * @return  0               succ
     *          else            fail
     */
    int Query(const char* strSql, int& affectedRows);
    int Query(const string& strSql, int& affectedRows);


    /**
     * @brief   Selectʱ��ȡ���ݣ��ǵ��ֹ�������������StMYSQLRes
     *
     * @param   result      ִ�н��
     *
     * @return  0           succ
     *          else        fail
     */
    int Result(MYSQL_RES *&result);

    /**
     * @brief   ����Ӱ������
     *
     * @return  >0          succ
     *          0           û�и���
     *          <0          fail
     */
    int AffectedRows();

    /**
     * @brief   �ж��Ƿ�������
     *
     * @return  true        ������
     *          false       δ����
     */
    bool IsOpen();

    /**
     * @brief   ��Ҫ�ǽ�blobת���ַ���
     *
     * @param   src         blobԴ
     * @param   len         ����
     *
     * @return  ת������ַ���
     */
    string EscStr(const char* src,uint32_t len);

    /**
     * @brief   ���ַ����е�ĳЩ�ַ�ת��(��')
     *
     * @param   src         �ַ���
     *
     * @return  ת������ַ���
     */
    string EscStr(const char* src);
    string EscStr(const string& src);

    /**
     * @brief   ��ȡԭʼ��mysqlָ�룬һ������²�Ҫ����
     *
     * @return  
     */
    MYSQL* GetMYSQLPtr();

protected:
    /**
     * @brief   �ر�mysql
     */
    void _CloseMySQL();
    /**
     * @brief   �ͷŵ������init��ص��ڴ�
     */
    void _FreeInitData();

private:
    char m_szErrMsg[1024];

    uint32_t m_port;
    
    char* m_ip;
    char* m_user;
    char* m_pwd;
    char* m_db;
    char* m_charset;

    MYSQL*      m_Database;

    //�Ƿ�����
    bool m_bOpen;
};
#endif
