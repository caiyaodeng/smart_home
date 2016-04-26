/*************************************************************************
	> File Name: ready_device.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:49:32 AM EDT
 ************************************************************************/

#include "ready_device.h"

ReadyDevice::ReadyDevice() {

}

ReadyDevice::~ReadyDevice() {

}
int ReadyDevice::getDeviceId() {
    return m_iDeviceId;
}

void ReadyDevice::setDeviceId(int iDeviceId) {
    m_iDeviceId = iDeviceId;
}

int ReadyDevice::getTaskId() {
    return m_iTaskId;
}

void ReadyDevice::setTaskId(int iTask) {
    m_iTaskId = iTask;
}

void ReadyDevice::setPeerAddr(unsigned char *pPeerSendAddr) {
    m_pSendAddr = pPeerSendAddr;
}

unsigned char *ReadyDevice::getPeerAddr() {
    return m_pSendAddr;
}
void ReadyDevice::setPeerLenAddr(unsigned int *pPeerLenAddr) {
    m_pSendLenAddr = pPeerLenAddr;
}

unsigned int *ReadyDevice::getPeerLenAddr() {
    return m_pSendLenAddr;
}
