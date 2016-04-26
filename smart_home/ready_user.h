/*************************************************************************
	> File Name: ready_user.h
	> Author:CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:42:53 AM EDT
 ************************************************************************/

#ifndef _READY_USER_H_
#define _READY_USER_H_

class ReadyUser {
    public:
        ReadyUser();
        ~ReadyUser();
    public:
        int getUserId();
        void setUserId(int iUserId);

        int getTaskId();
        void setTaskId(int iTaskId);

        void setPeerAddr(unsigned char *pPeerAddr);
        unsigned char *getPeerAddr();

        void setPeerLenAddr(unsigned int *pPeerLenAddr);
        unsigned int*getPeerLenAddr();
    private:
        int m_iUserId;
        int m_iTaskId;
        unsigned char *m_pSendAddr;
        unsigned int *m_pSendLenAddr;
};
#endif
