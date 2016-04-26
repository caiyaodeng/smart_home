/*************************************************************************
	> File Name: ready_user.cpp
	> Author: CaiYodeng
	> Mail:5123242191@qq.com 
	> Created Time: Wed 13 Apr 2016 08:14:10 AM EDT
 ************************************************************************/

#include "ready_user.h"

ReadyUser::ReadyUser() {

}

ReadyUser::~ReadyUser() {

}

int ReadyUser::getUserId() {
    return m_iUserId;
}

void ReadyUser::setUserId(int iUserId) {
    m_iUserId = iUserId; 
}

int ReadyUser::getTaskId() {
   return m_iTaskId; 
}

void ReadyUser::setTaskId(int iTaskId) {
   m_iTaskId = iTaskId; 
}

void ReadyUser::setPeerAddr(unsigned char *pPeerAddr) {
   m_pSendAddr = pPeerAddr; 
}

unsigned char *ReadyUser::getPeerAddr() {
   return m_pSendAddr; 
}

void ReadyUser::setPeerLenAddr(unsigned int *pPeerLenAddr) {
    m_pSendLenAddr = pPeerLenAddr;
}

unsigned int *ReadyUser::getPeerLenAddr() {
    return m_pSendLenAddr;
}
