/*************************************************************************
	> File Name: IdIdentifier.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:36:36 AM EDT
 ************************************************************************/

#ifndef _ID_IDENTIFIER_H_
#define _ID_IDENTIFIER_H_

#include "message.h"
#include "dal.h"
#include "ready_device.h"
#include "ready_user.h"

class  IdIdentifier {
    public:
        IdIdentifier(Dal *&pDal);
        ~IdIdentifier();
        
    public:
        bool identityIdentification(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList, std::list <ReadyUser> &readyUserList);
    protected:
        int identityReadyUser(Message *pMsg, int iTaskId, std::list <ReadyUser> &readyUserList);
        int identityReadyDevice(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList);
    private:
        Dal *m_pDal;
};
#endif
