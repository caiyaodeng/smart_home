/*************************************************************************
	> File Name: dal.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 09:33:44 PM EDT
 ************************************************************************/

#include "dal.h"

Dal::Dal(mynamespace::NetWork *&pNetGetter) :
    m_pNet (pNetGetter),
    m_pDB(nullptr){ 
}

Dal::~Dal() {
    if (m_pDB != nullptr) {
        delete m_pDB;
        m_pDB = nullptr;
    }
}
bool Dal::init() {
    m_pDB = new mynamespace::MyDB();
    m_pDB->init_db();
    if (m_pDB->conn_db("localhost", "root", "ss","SmartHome") != 0) {
        return false;
    }
    return true;
}
int Dal::execute(const char *pSql, char ***&pResult, int *pRow, int *pColum, char *&strResult) {
    if (m_pDB->sql_db(pSql, pResult, pRow, pColum) != 0) {
        return -1; 
    }

    return m_pDB->get_string(strResult); 
}
bool Dal::sendToPeer(int iTaskId) {
    std::cout << "dal:" << iTaskId << std::endl;
    std::cout << "m_pNet" << m_pNet << std::endl;
    if (m_pNet->push_msg(iTaskId) != 0) {
        return false;
    }
    return true;
}
