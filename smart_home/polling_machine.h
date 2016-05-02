/*************************************************************************
	> File Name: PollingMachine.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:40:57 AM EDT
 ************************************************************************/

#ifndef _POLLINGMACHINE_H_
#define _POLLINGMACHINE_H_

#include "ready_device.h"
#include <iostream>
#include <list>
#include "dal.h"
#include "protocol_command.h"

class PollingMachine {
    public:
        PollingMachine();
        ~PollingMachine();

        void setPollingObj(std::list <ReadyDevice> *readyDeviceList, Dal *&pDal, Locker *&pReadyListLocker);
        static void *startPolling(void *arg);
        void pollingTask();
    private:
        std::list <ReadyDevice> *m_readyDeviceList;
        Dal *m_pDal;
        Locker *m_readylist_lock;
};
#endif
