/*************************************************************************
	> File Name: dal.h
	> Author:  CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 10:09:07 AM EDT
 ************************************************************************/

#ifndef _DAL_H_
#define _DAL_H_

#include "../net/net_work.h"
#include "../db/mysql/db_mysql.h"

class Dal {
    public:
        Dal(mynamespace::NetWork *&pNetGetter);
        ~Dal();
    public:
        bool init();

        int execute(const char *pSql, char ***&pResult, int *pRow, int *pColum, char *&strResult);
        bool sendToPeer(int iTaskId);
    private:
        mynamespace::NetWork *m_pNet;
        mynamespace::MyDB *m_pDB;
};

#endif
