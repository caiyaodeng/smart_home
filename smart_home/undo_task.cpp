/*************************************************************************
	> File Name: undo_task.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com
	> Created Time: Wed 13 Apr 2016 08:48:56 AM EDT
 ************************************************************************/

#include "undo_task.h"

UndoTask::UndoTask() {

}

UndoTask::~UndoTask() {

}
void UndoTask::setSourceId(int iSourceId) {
    m_iSourceId = iSourceId;
}
void UndoTask::setSourceId(const char *pSourceId) {
    memcpy(&m_iSourceId, pSourceId, 4);
}
int UndoTask::getSourceId() {
    return m_iSourceId;
}

void UndoTask::setDestinationId(int iDestinationId) {
    m_iDestinationId = iDestinationId;
}

int UndoTask::getDestinationId() {
    return m_iDestinationId;
}
