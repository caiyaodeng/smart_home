/*************************************************************************
	> File Name: UserConfigureUpdater.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:40:04 AM EDT
 ************************************************************************/

#include "user_configure_updater.h"
#include <iostream>

UserConfigureUpdater::UserConfigureUpdater(Dal *&pDal):
    m_pDal(pDal){

}

UserConfigureUpdater::~UserConfigureUpdater() {

}

bool UserConfigureUpdater::updateConfiguration(Message *pMsg, int iTaskId, std::list <ReadyDevice> readyDeviceList) {
    memcpy(pMsg->peer_addr.data_addr, (const char *)"updateConfiguration", 20);
    *(pMsg->peer_addr.len_addr) = 20;

    cmd::ProtocolContrlSet *pContrlSet = (cmd::ProtocolContrlSet *)malloc(CONTRL_SIZE);    
    memset(pContrlSet, 0, CONTRL_SIZE);
    memcpy (pContrlSet->protocol_contrl_set_data, pMsg->data_ptr, pMsg->data_length);
    *(pContrlSet->protocol_contrl_set_data+10) = '\0';
    *(pContrlSet->protocol_contrl_set_data+30) = '\0';

    /*Conf Add*/
    if (*pContrlSet->protocol_contrl.conf_opt == CONF_ENABLE) {
    //    std::cout << "*!!!!!!Conf ADD " << std::endl;
        char pName[16];
        char pDefaultAction[33];
        memset(pName, 0 ,16);
        memset(pDefaultAction, '\0' ,33);
        memcpy(pName, pContrlSet->protocol_contrl.name, 16);
        memcpy(pDefaultAction, pContrlSet->protocol_contrl.default_action, 32);
        std::cout << "defaultAction: " << pDefaultAction << std::endl;
        if (*pContrlSet->protocol_contrl.conf_way == ROOM_DEVICE) {
            if (!addConf("room", pName, (const char *)pContrlSet->protocol_contrl.device_serial, (const char *)pDefaultAction, pMsg->source_id)) {
                memcpy(pMsg->peer_addr.data_addr, (const char *)"update failed", 14);
                *(pMsg->peer_addr.len_addr) = 14;
                m_pDal->sendToPeer(iTaskId);
                return false;    
            }
        }
        else if (*pContrlSet->protocol_contrl.conf_way == MODEL_DEVICE){
            if (!addConf("model", pName, (const char *)pContrlSet->protocol_contrl.device_serial, (const char *)pDefaultAction, pMsg->source_id)) {
                memcpy(pMsg->peer_addr.data_addr, (const char *)"update failed", 14);
                *(pMsg->peer_addr.len_addr) = 14;
                m_pDal->sendToPeer(iTaskId);
                return false;    
            }
        }
    }

    /*Time ADJ*/
    if (*pContrlSet->protocol_contrl.time_adjust == TIME_ADJ) {
    //    std::cout << "*!!!!!!Time ADJ " << std::endl;
        /*get UTC time*/
        unsigned char strUTC[4];
        unsigned char pCommand[33];
        memset(strUTC, 0, 4);
        memset(pCommand, '-', 32);
        pCommand[32] = '\0';
        if (!tx::get_UTC_E8(strUTC)) {
            return false;
        }
        memcpy(pCommand+14, strUTC, 4);

        /*start check time*/
        char pSQL[100];
        memset(pSQL, 0, 100);
        sprintf(pSQL, "select deviceId from roomdevice where userid = %d;", pMsg->source_id);
        std::cout << "pSql"<< pSQL << std::endl;

        /*quary from DB*/
        char ***pResult = nullptr;
        char *strResult = nullptr;
        int iRow(0), iColum(0);

        if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
            return false;
        }
        int iDeviceId[iRow];
        int iCount = iRow;
        for (int i=0; i<iCount; ++i) { /*cpy device id*/
            iDeviceId[i] = atoi(pResult[i+1][0]);
        }

        std::list <ReadyDevice>::iterator i;
        for (int j=0; j<iCount; ++j) {
            for (i = readyDeviceList.begin(); i != readyDeviceList.end() ; i++) {
                if ((*i).getDeviceId() == iDeviceId[j]) {
                    memset((char *)(*i).getPeerAddr(), '\0', COMMAND_SIZE+18);
                    sprintf((char *)(*i).getPeerAddr(), "dSEQSERV%04d0032%s|", (*i).getDeviceId(), pCommand);
                    std::cout << "peerAddr:" << (char *)(*i).getPeerAddr() << std::endl;
                    *((*i).getPeerLenAddr()) = COMMAND_SIZE+18;
                    m_pDal->sendToPeer((*i).getTaskId());
                }    
            }
        }
        memset(pSQL, 0, 100);
    }

    /*Remote Update*/
    if (*pContrlSet->protocol_contrl.remote_update == UPD) {
        //std::cout << "*!!!!!!Update " << std::endl;
    }

    /*Reset*/
    if (*pContrlSet->protocol_contrl.reset == RESET) {
       // std::cout << "*!!!!!!Reset " << std::endl;
        if(!resetConf(pMsg->source_id)) {
            memcpy(pMsg->peer_addr.data_addr, (const char *)"update failed", 14);
            *(pMsg->peer_addr.len_addr) = 14;
            m_pDal->sendToPeer(iTaskId);
        }
    }

    /*collect*/
    unsigned char pTime[20];
    if (!tx::get_Time_E8(pTime)) {
      return false;
    }  

    if (*pContrlSet->protocol_contrl.collect == COLLECT) {
       // std::cout << "*!!!!!!Collect " << std::endl;
       if (!collectDevice(atoi((char *)pContrlSet->protocol_contrl.collect_device_id), pTime, pContrlSet->protocol_contrl.collect_name)) {
            return false;
        }
    }

    memcpy(pMsg->peer_addr.data_addr, (const char *)"update successed", 16);
    *(pMsg->peer_addr.len_addr) = 16;
    m_pDal->sendToPeer(iTaskId);

    free(pContrlSet);
    pContrlSet = nullptr;
    std::cout << "UserConfigureUpdater::updateConfiguration___ok___" << std::endl;
    return true;
}

bool UserConfigureUpdater::resetConf(int iUserId) {
    char ***pResult = nullptr;
    char *strResult = nullptr;
    char pSQL[100]; 
    memset(pSQL, 0, 100);
    int iRow(0), iColum(0);

    sprintf(pSQL, "delete from roomdevice where userId = %d;", iUserId);
    std::cout << pSQL <<std::endl;
    if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
        return false;
    }
    
    sprintf(pSQL, "delete from modeldevice where userId = %d;", iUserId);
    std::cout << pSQL <<std::endl;
    if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
        return false;
    }
    return true;
}

bool UserConfigureUpdater::addConf(const char *ptable, const char *pName, const char *pDeviceCode, const char *pDefaultAction, int iUserId) {
    char ***pResult = nullptr;
    char *strResult = nullptr;
    char pSQL[100]; 
    memset(pSQL, 0, 100);
    int iRow(0), iColum(0);
    char *strDeviceCode = (char *)malloc (12);
    memset (strDeviceCode, '\0', 12);
    memcpy(strDeviceCode, pDeviceCode, 11);

    sprintf(pSQL, "select deviceId from device where deviceCode = %s;", strDeviceCode);
    std::cout << pSQL << std::endl;
    if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
        return false;
    }

    memset(pSQL, 0, 100);
    std::cout << pSQL << std::endl;
    sprintf(pSQL, "insert into %sdevice (userId, %sName, deviceId, defaultAction) value(%d, '%s', %d, '%s');", ptable, ptable, iUserId, pName ,atoi(pResult[1][0]), pDefaultAction);
    if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
        return false;
    }

    std::cout << pSQL << std::endl;
    return true;
}
bool UserConfigureUpdater::collectDevice(const int iDeviceId, const unsigned char *pTime, const unsigned char *pGroupName) {

    std::cout << "*!!!!!!Collect " << std::endl;
    std::cout << iDeviceId << std::endl;
    std::cout << pTime << std::endl;
    std::cout << pGroupName << std::endl;
    return true;
}
