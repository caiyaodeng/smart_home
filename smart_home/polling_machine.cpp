/*************************************************************************
	> File Name: PollingMachine.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:41:40 AM EDT
 ************************************************************************/

#include "polling_machine.h"

PollingMachine::PollingMachine() {

}

PollingMachine::~PollingMachine() {

}

void PollingMachine::setPollingObj(std::list <ReadyDevice> *readyDeviceList, Dal *&pDal, Locker *&pReadyListLocker) {
    m_pDal = pDal;
    m_readyDeviceList = readyDeviceList;
    m_readylist_lock = pReadyListLocker;
}

void *PollingMachine::startPolling(void *arg) {
    while (true) {
        sleep(5);
        ((PollingMachine *)arg)->pollingTask();
    }
    return nullptr;
}

void PollingMachine::pollingTask() {
    std::list <ReadyDevice>::iterator i;

    std::cout << "poling once !" << std::endl;
    m_readylist_lock->lock();
    for (i= m_readyDeviceList->begin(); i!= m_readyDeviceList->end(); i++) {
        sprintf((char *)(*i).getPeerAddr(), "dSEQSERV%04d0000--------------------------------|", (*i).getDeviceId());
        *((*i).getPeerLenAddr()) = COMMAND_SIZE+18;
        m_pDal->sendToPeer((*i).getTaskId());
        std::cout << "2polling !" << std::endl;
    }
    m_readylist_lock->unlock();
}
