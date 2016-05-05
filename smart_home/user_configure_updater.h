/*************************************************************************
	> File Name: UserConfigureUpdater.h
	> Author: CaiYaodeng
	> Mail:5123242191@QQ.com 
	> Created Time: Wed 13 Apr 2016 07:39:25 AM EDT
 ************************************************************************/

#ifndef _USER_CONFIGURE_UPDATER_H_
#define _USER_CONFIGURE_UPDATER_H_
 
#include "message.h"   
#include "../base/timex.h"
#include "dal.h"
#include "ready_device.h"

class UserConfigureUpdater {
    public:
        UserConfigureUpdater(Dal *&pDal);
        ~UserConfigureUpdater();
    public:
        bool updateConfiguration(Message *pMsg, int iTaskId, std::list <ReadyDevice> readyDeviceList);
    protected:
        inline bool addConf(const char *ptable, const char *pName, const char *pDeviceId, const char *pDefaultAction, int iUserId);
        inline bool resetConf(int iUserId);
    private:
        Dal *m_pDal;
};
#endif
