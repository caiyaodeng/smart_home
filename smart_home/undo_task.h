/*************************************************************************
	> File Name: undo_task.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 08:45:46 AM EDT
 ************************************************************************/

#ifndef _UNDO_TASK_H_
#define _UNDO_TASK_H_

#include <string.h>

class UndoTask {
    public:
        UndoTask();
        ~UndoTask();
    public:
        void setSourceId(int iSourceId);
        void setSourceId(const char *pSourceId);
        int getSourceId();

        void setDestinationId(int iDestinationId);
        int getDestinationId();
    private:
        int m_iSourceId;
        int m_iDestinationId;
};
#endif
