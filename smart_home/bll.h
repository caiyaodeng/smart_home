/*************************************************************************
	> File Name: bll.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:58:14 AM EDT
 ************************************************************************/

#ifndef _BLL_H_
#define _BLL_H_

#include "../net/net_work.h"
#include "id_identifier.h"
#include "message_shunter.h"
#include "polling_machine.h"
#include "ready_device.h"
#include "ready_user.h"
#include "transit_center.h"
#include "undo_task.h"
#include "user_configure_updater.h"
#include "message.h"
#include "dal.h"

class Bll {
    public:
        Bll(mynamespace::NetWork *&pNetGetter);
        ~Bll();

        bool init();
        bool pushMessage(int iTaskId, Message *pUndoTask);
    protected:
        int idenfity(int iTaskId, Message *pUndoTask);
        int transmit(int iTaskId, Message *pUndoTask,std::list <ReadyDevice> &readyDeviceList);
        int update(int iTaskId, Message *pUndoTask);
        int polling(std::list <ReadyDevice> &readyDeviceList, std::list <UndoTask> &undoTasks);
    protected:
        void addReadyUser(int iUserId, int iTaskId, unsigned char *pPeerSendAddr);
        int getUserTask(int iUserId, unsigned char *&pPeerSendAddr); //return for taskId
        void removeReadyUser(int iUserId);   

        void addReadyDevice(int iDeviceId, int iTaskId, unsigned char *pPeerSendAddr);
        int getDeviceTask(int iDeviceId, unsigned char *&pPeerSendAddr); //return for taskId
        void removeReadyDevice(int iDeviceId);

        void addTask(int iSourceId, int iDestinnationId); 
        UndoTask *popTask();
    private:
        std::list <ReadyUser> m_readyUserList;
        std::list <ReadyDevice> m_readyDeviceList;
        std::list <UndoTask> m_undoTasks;
        MessageShunter *m_pRecvMessageShunter;
        IdIdentifier *m_pIdDistributor;
        TransitCenter *m_pCommandTransmitter;
        UserConfigureUpdater *m_pUserConfigUpdater;
        PollingMachine *m_pStatusPoller;
        Dal *m_pDal;
        mynamespace::NetWork *m_pNetGetter;
};
#endif
