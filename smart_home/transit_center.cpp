/*************************************************************************
	> File Name: TransitCenter.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:38:29 AM EDT
 ************************************************************************/

#include "transit_center.h"
#include <iostream>

TransitCenter::TransitCenter(Dal *&pDal):
    m_pDal(pDal){

}

TransitCenter::~TransitCenter() {

}

bool TransitCenter::forwardCommand(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList) {
    std::cout << "forwardCommand" << std::endl;
    std::list <ReadyDevice>::iterator i;
    int iPeerDeviceId = pMsg->destination_id;
    unsigned char *pPeerSendAddr = nullptr;
    unsigned int *pPeerLenAddr = nullptr;
    int iPeerTaskId = 0;

    /*mult CMD*/
    if (strncmp((char *)&pMsg->destination_id, "MULT", 4) == 0) {
        if (!multCommand(pMsg, readyDeviceList)) {
            return false;
        }
    }    
    else {
        for (i = readyDeviceList.begin(); i != readyDeviceList.end(); ++i) {
            if ((*i).getDeviceId() == iPeerDeviceId) {
                pPeerSendAddr = (*i).getPeerAddr();
                pPeerLenAddr = (*i).getPeerLenAddr();
                iPeerTaskId = (*i).getTaskId();
                break;
            }
        }
        if (i == readyDeviceList.end()) {
            return false;
        }

        /*combine header*/
        unsigned char *pMsgData = nullptr;
        int iMsgDataLen = pMsg->data_length; 
        int iMsgLen = 17+iMsgDataLen+1;                                 
        unsigned char *pMsgBuf = (unsigned char *)malloc(iMsgLen);         
        pMsgData = (unsigned char *)malloc(iMsgDataLen+2);                 
        memset(pMsgData, '\0', iMsgDataLen+2);                             
        memcpy(pMsgData, (const char *)pMsg->data_ptr, iMsgDataLen);            
        *(pMsgData+iMsgDataLen) = '|';                                     
        memset(pMsgBuf, '\0', iMsgLen);                                       

        sprintf((char *)pMsgBuf, "dSEQ%04d%04d%04d%s", pMsg->source_id, pMsg->destination_id, iMsgDataLen, pMsgData);                                        
        std::cout << "pMsgbuf:" << std::endl;                              
        std::cout << pMsgBuf << std::endl;                                 
        memcpy(pPeerSendAddr, pMsgBuf, iMsgLen);               
        *pPeerLenAddr = iMsgLen;                               
        std::cout << "iMsgLen:" << *pPeerLenAddr << std::endl;
        std::cout << "iPeerTaskId" << iPeerTaskId << std::endl;
        m_pDal->sendToPeer(iPeerTaskId);
    }

    std::cout << "TransitCenter::forwardCommand ____ok____" << std::endl;
    return true;
}

bool TransitCenter::getResponse(Message *pMsg, int iTaskId, std::list <ReadyUser> &readyUserList) {

    std::cout << "transit ok" << std::endl;
    if (pMsg->destination_id == 0) {
        if (!pushServResponse()) {
            return false;
        }
        
        std::cout << "serv!" << std::endl;
    } 
    else {
        if (!pushUserResponse(pMsg->destination_id, pMsg->source_id, readyUserList)) {
            return false;
        }
        std::cout << "peer!" << std::endl;
    }
    return true; 
}

bool TransitCenter::multCommand(Message *pMsg, std::list <ReadyDevice> &readyDeviceList) {
std::cout << "pMsg->data_length :" << pMsg->data_length << std::endl;
std::cout << "pMsg->data_ptr :" << pMsg->data_ptr << std::endl;
    char *pData = (char *)malloc(pMsg->data_length);
    char pTableName[6];
    char pSQL[100];
    memset(pSQL, 0, 100);
    memset(pTableName, 0, 6);
    memset(pData, 0, pMsg->data_length);
    memcpy(pData, pMsg->data_ptr, pMsg->data_length);
    int iId = atoi(pData+1);
std::cout << "pData :" << pData << std::endl;

    /*select right table*/
    if (*pData == ROOM_DEVICE) {
       memcpy(pTableName, "room", 5); 
    }
    if (*pData == MODEL_DEVICE) {
       memcpy(pTableName, "model", 6); 
    }
    sprintf(pSQL, "select deviceId from %sdevice where %sId = %d and userid = %d;", pTableName, pTableName, iId, pMsg->source_id);
    std::cout << "pSql"<< pSQL << std::endl;

    /*quary from DB*/
    char ***pResult = nullptr;
    char *strResult = nullptr;
    int iRow(0), iColum(0);

    if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
        return false;
    }
    int iDeviceId[iRow];
    char **pDefaultActions = (char **)malloc(iRow*sizeof(char *));   
    int iCount = iRow;
    for (int i=0; i<iCount; ++i) { /*cpy device id*/
        iDeviceId[i] = atoi(pResult[i+1][0]);
    }
    for (int i=0; i<iCount; ++i) { /*malloc memory*/
        pDefaultActions[i] = (char *)malloc(COMMAND_SIZE);
        memset(pDefaultActions[i], 0, COMMAND_SIZE);
    }
    for (int i=0; i<iCount; ++i) {
        sprintf(pSQL, "select defaultAction from %sdevice where deviceId = %d and userid = %d and %sid = %d;", pTableName, iDeviceId[i], pMsg->source_id, pTableName, iId);
        std::cout << "pSql"<< pSQL << std::endl;
        if (m_pDal->execute(pSQL, pResult, &iRow, &iColum, strResult) == -1) {
            return false;
        } 
        memcpy(pDefaultActions[i], pResult[1][0], COMMAND_SIZE);
    }

    for (int i=0; i<iCount; ++i) {
        std::cout << iDeviceId[i] << std::endl;
        std::cout << pDefaultActions[i] << std::endl;
    }
     
    std::list <ReadyDevice>::iterator i;
    for (int j=0; j<iCount; ++j) {
        for (i = readyDeviceList.begin(); i != readyDeviceList.end() ; i++) {
            if ((*i).getDeviceId() == iDeviceId[j]) {
                memcpy((*i).getPeerAddr(), pDefaultActions[j], COMMAND_SIZE);
                std::cout << "peerAddr:" << pDefaultActions[j] << std::endl;
                *((*i).getPeerLenAddr()) = COMMAND_SIZE;
                m_pDal->sendToPeer((*i).getTaskId());
            }    
        }
    }
    memset(pSQL, 0, 100);

    free(pData);
    pData = nullptr;
    for (int i=0; i<iCount; ++i) { /*free memory*/
        free(pDefaultActions[i]);
        pDefaultActions[i] = nullptr;
    }
    free(pDefaultActions);
    pDefaultActions = nullptr;
    return true;
}

bool TransitCenter::pushServResponse() {
    //
    return true;
}
bool TransitCenter::pushUserResponse(int iUserId, int iDeviceId, std::list <ReadyUser> &readyUserList) { 
    std::list <ReadyUser>::iterator i; 
    for (i=readyUserList.begin(); i != readyUserList.end(); i++) {
        if ((*i).getUserId() == iUserId) {
            sprintf((char *)(*i).getPeerAddr(), "dACK%04d%04d0000--------------------------------|", iDeviceId, iUserId);
            *((*i).getPeerLenAddr()) = COMMAND_SIZE+18;
            m_pDal->sendToPeer((*i).getTaskId());
            break;
        }
	}

    if (i == readyUserList.end()) {
        return false;
    }

    return true;
}
