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
    	bool pollingTask(std::list <ReadyDevice> &readyDeviceList, Dal *&pDal);
};
#endif
