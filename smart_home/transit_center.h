/*************************************************************************
	> File Name: TransitCenter.h
	> Author: CaiYaodeng
	> Mail:512324219@qq.com 
	> Created Time: Wed 13 Apr 2016 07:37:53 AM EDT
 ************************************************************************/

#ifndef _TRANSIT_CENTER_H_
#define _TRANSIT_CENTER_H_

#include "message.h"
#include "dal.h"
#include "ready_device.h"
#include "ready_user.h"
#include "../base/timex.h"

class TransitCenter {
    public:
        TransitCenter(Dal *&pDal);
        ~TransitCenter();
    public:
        bool forwardCommand(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList);
        bool getResponse(Message *pMsg, int iTaskId, std::list <ReadyUser> &readyUserList);
    protected:
        bool multCommand(Message *pMsg, std::list <ReadyDevice> &readyDeviceList);
        bool pushServResponse(int iDeviceId, const char *pData); 
        bool pushUserResponse(int iUserId, int iDeviceId, std::list <ReadyUser> &readyUserList, const char *pData); 
    private:
        Dal *m_pDal;
};
#endif
