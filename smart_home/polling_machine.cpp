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
bool PollingMachine::pollingTask(std::list <ReadyDevice> &readyDeviceList,Dal *&pDal) {
    std::list <ReadyDevice>::iterator i;
    
	for (i = readyDeviceList.begin(); i != readyDeviceList.end() ; i++) {
		sprintf((char *)(*i).getPeerAddr(), "dSEQSERV%04d0000--------------------------------|", (*i).getDeviceId());
        *((*i).getPeerLenAddr()) = COMMAND_SIZE+18;
        pDal->sendToPeer((*i).getTaskId());
	}

    return true;
}
