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
bool PollingMachine::pollingTask(std::list <ReadyDevice> &readyDeviceList, std::list <UndoTask> &undoTasks) {
    std::list <ReadyDevice>::iterator i;
    
	for (i = readyDeviceList.begin(); i != readyDeviceList.end() ; i++) {
		UndoTask *pUndoTask = new UndoTask();
		pUndoTask->setDestinationId((i)->getDeviceId());
		pUndoTask->setSourceId((const char *)"SERV");
		undoTasks.push_back(*pUndoTask);
	}

    return true;
}
