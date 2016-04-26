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

bool UserConfigureUpdater::updateConfiguration(Message *pMsg, int iTaskId) {
    memcpy(pMsg->peer_addr.data_addr, (const char *)"updateConfiguration", 20);
    *(pMsg->peer_addr.len_addr) = 20;

    cmd::ProtocolContrlSet *pContrlSet = (cmd::ProtocolContrlSet *)malloc(CONTRL_SIZE);    
    memset(pContrlSet, 0, CONTRL_SIZE);
    memcpy (pContrlSet->protocol_contrl_set_data, pMsg->data_ptr, pMsg->data_length);
    *(pContrlSet->protocol_contrl_set_data+10) = '\0';
    *(pContrlSet->protocol_contrl_set_data+30) = '\0';

    /*Conf Add*/
    if (*pContrlSet->protocol_contrl.conf_opt == CONF_ENABLE) {
        char pName[16];
        char pDefaultAction[33];
        memset(pName, 0 ,16);
        memset(pDefaultAction, '\0' ,33);
        memcpy(pName, pContrlSet->protocol_contrl.name, 16);
        memcpy(pDefaultAction, pContrlSet->protocol_contrl.default_action, 32);
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

    }

    /*Remote Update*/
    if (*pContrlSet->protocol_contrl.remote_update == UPD) {
        
    }

    /*Reset*/
    if (*pContrlSet->protocol_contrl.reset == RESET) {
        if(!resetConf(pMsg->source_id)) {
            memcpy(pMsg->peer_addr.data_addr, (const char *)"update failed", 14);
            *(pMsg->peer_addr.len_addr) = 14;
            m_pDal->sendToPeer(iTaskId);
        }
    }

    /*update database*/

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

    sprintf(pSQL, "select deviceId from device where deviceCode = %s;", pDeviceCode);
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
