/*************************************************************************
	> File Name: ready_device.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:48:41 AM EDT
 ************************************************************************/

#ifndef _READY_DEVICE_H_
#define _READY_DEVICE_H_

class ReadyDevice {
    public:
        ReadyDevice();
        ~ReadyDevice();
    public:
        int getDeviceId();
        void setDeviceId(int iDeviceId);

        int getTaskId();
        void setTaskId(int iTask);

        void setPeerAddr(unsigned char *pPeerSendAddr);
        unsigned char *getPeerAddr();

        void setPeerLenAddr(unsigned int *pPeerLenAddr);
        unsigned int*getPeerLenAddr();
    private:
        int m_iDeviceId;
        int m_iTaskId;
        unsigned char *m_pSendAddr;
        unsigned int *m_pSendLenAddr;

};
#endif
