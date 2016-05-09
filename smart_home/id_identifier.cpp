/*************************************************************************
	> File Name: IdIdentifier.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:37:11 AM EDT
 ************************************************************************/

#include "id_identifier.h"
#include "message_parser.h"
#include <iostream>
IdIdentifier::IdIdentifier(Dal *&pDal):
    m_pDal(pDal){

}

IdIdentifier::~IdIdentifier() {

}

bool IdIdentifier::identityIdentification(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList, std::list <ReadyUser> &readyUserList) {
    if (*pMsg->connection_type == 'u') {
        if (identityReadyUser(pMsg,iTaskId, readyUserList) == 0) {
            return true;
        }
    }
    else if (*pMsg->connection_type == 'd') {
        if (identityReadyDevice(pMsg,iTaskId, readyDeviceList) == 0) {
            return true;
        }
    }
    return false;
}
int IdIdentifier::identityReadyDevice(Message *pMsg, int iTaskId, std::list <ReadyDevice> &readyDeviceList) {
 
    int iDataLen = pMsg->data_length;
    //std::cout << iDataLen << std::endl;
    char *pData = (char *)malloc(iDataLen*sizeof(char)+1);
    memset(pData, 0, iDataLen+1);
    memcpy(pData, pMsg->data_ptr, iDataLen);
    //std::cout << pData << std::endl;
    char *pSql = (char *)malloc(200);
    memset(pSql, 0, 200);
    char ***pResult = nullptr;
    int iRow = 0;
    int iCoulmn = 0;
    char *strResult = nullptr;

    sprintf(pSql, "select deviceId from device where deviceCode='%s';", pData);
    printf ("%s\n",pSql); 
    if (m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult) == -1) {
        return -1;
    }

    /*get from 'user', 'device'*/
    int iMsgDataLen = 0;
    int iMsgId = -1;
    iMsgId = atoi(pResult[1][0]);
    std::cout << iMsgId << std::endl;
    unsigned char *pMsgData = nullptr;
    /*sprintf(pSql, "select *from device where deviceId in (select deviceId from modeldevice where userId='%d');", iMsgId);
    if ((iMsgDataLen = m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult)) == -1) {
        std::cout << "ifalse" << std::endl;
        return -1;
    }
    std::cout << strResult << std::endl;
    */

    /*combine header*/
    int iMsgLen = 17+iMsgDataLen+1;
    unsigned char *pMsgBuf = (unsigned char *)malloc(iMsgLen);
    pMsgData = (unsigned char *)malloc(iMsgDataLen+1);
    memset(pMsgData, '|', iMsgDataLen+1);
    memcpy(pMsgData, (const char *)strResult, iMsgDataLen);
    memset(pMsgBuf, 0, iMsgLen);

    sprintf((char *)pMsgBuf, "dACKSERV%04d%04d|", iMsgId, iMsgDataLen+1);
    std::cout << pMsgBuf << std::endl;
    memcpy(pMsg->peer_addr.data_addr, pMsgBuf, iMsgLen);
    *pMsg->peer_addr.len_addr = iMsgLen;

    std::cout << "ok" << std::endl;
    free(pData);
    pData = nullptr;
    free(pSql);
    pSql = nullptr;
    free(pMsgBuf);
    pMsgBuf = nullptr;
    free(pMsgData);
    pMsgData = nullptr;

    /*add readyDevices*/
    if (false) {
    END:
        ReadyDevice *pNewDevice = new ReadyDevice();
        pNewDevice->setDeviceId(iMsgId);
        pNewDevice->setTaskId(iTaskId);
        pNewDevice->setPeerAddr(pMsg->peer_addr.data_addr);
        pNewDevice->setPeerLenAddr(pMsg->peer_addr.len_addr);
        readyDeviceList.push_back(*pNewDevice);
        std::cout << "IdIdentifier::identitydevice  add__device__ok__" << std::endl;
        return 0;
    }
    std::list <ReadyDevice>::iterator i;
    for (i=readyDeviceList.begin(); i!=readyDeviceList.end(); ++i) {
        if ((*i).getDeviceId() == iMsgId) {
            std::cout << "IdIdentifier::identityDeice  not_add_device___ok__" << std::endl;
            return 0;
        }
    }
    goto END;
}
int IdIdentifier::identityReadyUser(Message *pMsg, int iTaskId, std::list <ReadyUser> &readyUserList) {

    int iDataLen = pMsg->data_length;
    char *pData = (char *)malloc(iDataLen*sizeof(char));
    memset(pData, 0, iDataLen);
    memcpy(pData, pMsg->data_ptr, iDataLen);
    const char *pSplit = ","; 
    char *pSub, *pName, *pPassword; 
    bool isName = true;
    bool isPassword = true;
    char *pSql = (char *)malloc(200);
    memset(pSql, 0, 200);
    char ***pResult = nullptr;
    int iRow = 0;
    int iCoulmn = 0;
    char *strResult = nullptr;

    pSub = strtok (pData, pSplit); 
    while(pSub!=NULL) { 
        if (isName) {
            pName = pSub; 
            isName = false;
        }
        else if (isPassword) {
            pPassword = pSub; 
            isPassword = false;
        }
        pSub = strtok(NULL, pSplit); 
    } 
    sprintf(pSql, "select userId from user where userName='%s' and password='%s';", pName, pPassword);
    printf ("%s\n",pSql); 
    if (m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult) == -1) {
        return -1;
    }

    /*get from 'user', 'device'*/
    int iMsgDataLen = 0;
    int iDeviceRet = 0;
    int iMsgId = -1;
    iMsgId = atoi(pResult[1][0]);
    std::cout << iMsgId << std::endl;
    unsigned char *strUser =nullptr;
    unsigned char *pMsgData = nullptr;
    
    sprintf(pSql, "SELECT * FROM device WHERE deviceId IN (SELECT deviceId FROM roomdevice WHERE userId=%d UNION SELECT deviceId FROM modeldevice WHERE userId=%d);", iMsgId, iMsgId);
    if ((iDeviceRet = m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult)) == -1) {
        std::cout << "false" << std::endl;
        return -1;
    }
    std::cout << iDeviceRet << std::endl;
    std::cout<< strResult << std::endl;
    iMsgDataLen += iDeviceRet;
    strUser=(unsigned char *)malloc(iDeviceRet*sizeof(char));
    memcpy(strUser,strResult,iDeviceRet);

    /*get from roomdevice*/
    unsigned char *strRoomDevice = nullptr;
    int iRoomRet = 0;
    sprintf(pSql, "select * from roomdevice where userId='%d';", iMsgId);
    if ((iRoomRet = m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult)) == -1) {
        std::cout << "false" << std::endl;
        return -1;
    }
    std::cout << iRoomRet << std::endl;
    iMsgDataLen += iRoomRet;
    strRoomDevice=(unsigned char *)malloc(iRoomRet*sizeof(char));
    memcpy(strRoomDevice,strResult,iRoomRet);

    /*get from modeldevice*/
    unsigned  char *strModelDevice = nullptr;
    int iModelDeviceRet = 0;
    sprintf(pSql, "select * from modeldevice where userId='%d';", iMsgId);
    if ((iModelDeviceRet = m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult)) == -1) {
        std::cout << "false" << std::endl;
        return -1;
    }
    iMsgDataLen += iModelDeviceRet;
    strModelDevice=(unsigned char *)malloc(iModelDeviceRet*sizeof(char));
    memcpy(strModelDevice,strResult,iModelDeviceRet);

    /*get from collect*/
    unsigned  char *strCollect = nullptr;
    int iCollectRet = 0;
    sprintf(pSql, "select * from collect where userId='%d';", iMsgId);
    if ((iCollectRet = m_pDal->execute(pSql, pResult, &iRow, &iCoulmn, strResult)) == -1) {
        std::cout << "false" << std::endl;
        return -1;
    }
    iMsgDataLen += iCollectRet;
    strCollect=(unsigned char *)malloc(iCollectRet*sizeof(char));
    memcpy(strCollect,strResult,iCollectRet);
    
    /*combine header*/
    int iMsgLen = 16+iMsgDataLen+2+2+2;
    unsigned char *pMsgBuf = (unsigned char *)malloc(iMsgLen);
    pMsgData = (unsigned char *)malloc(iMsgDataLen+2+2+2);
    memset(pMsgData, '\0', iMsgDataLen+2+2+2);
    
    memcpy(pMsgData, "[",1);
    memcpy(pMsgData+1, (const char *)strUser ,iDeviceRet);
    memcpy(pMsgData+iDeviceRet+1, ",",1);
    memcpy(pMsgData+iDeviceRet+1+1, (const char *)strRoomDevice ,iRoomRet);
    memcpy(pMsgData+iDeviceRet+1+1+iRoomRet, ",",1);
    memcpy(pMsgData+iRoomRet+iDeviceRet+1+1+1, (const char *)strModelDevice ,iModelDeviceRet);
    memcpy(pMsgData+iRoomRet+iDeviceRet+iModelDeviceRet+1+1+1, ",",1);
    memcpy(pMsgData+iRoomRet+iDeviceRet+iModelDeviceRet+1+1+1+1, (const char *)strCollect ,iCollectRet);
    memcpy(pMsgData+iRoomRet+iDeviceRet+iModelDeviceRet+1+1+1+1+iCollectRet, "]",1);
   // memcpy(pMsgData+iModelDeviceRet+iDeviceRet+iRoomRet, (const char *)strFixedTime ,iFixedTimeRet);
   
    *(pMsgData+iMsgDataLen+4) = '|';
std::cout<<pMsgData<<std::endl;
    memset(pMsgBuf, 0, iMsgLen);
    sprintf((char *)pMsgBuf, "uACKSERV%04d%04d%s", iMsgId, iMsgDataLen+1, pMsgData);
    std::cout << "pMsgbuf:" << std::endl;
    std::cout << pMsgBuf << std::endl;
    memcpy(pMsg->peer_addr.data_addr, pMsgBuf, iMsgLen);
    *pMsg->peer_addr.len_addr = iMsgLen;
    std::cout << "iMsgLen:" << *pMsg->peer_addr.len_addr << std::endl;

    free(pData);
    pData = nullptr;
    free(pSql);
    pSql = nullptr;
    free(pMsgBuf);
    pMsgBuf = nullptr;
    free(pMsgData);
    pMsgData = nullptr;

    /*add readyUser*/
    if (false) {
    END:
        ReadyUser *pNewUser = new ReadyUser();
        pNewUser->setUserId(iMsgId);
        pNewUser->setTaskId(iTaskId);
        pNewUser->setPeerAddr(pMsg->peer_addr.data_addr);
        pNewUser->setPeerLenAddr(pMsg->peer_addr.len_addr);
        readyUserList.push_back(*pNewUser);
        std::cout << "IdIdentifier::identityUser  add_user___ok__" << std::endl;
        return 0;
    }
    std::list <ReadyUser>::iterator i;
    for (i=readyUserList.begin(); i!=readyUserList.end(); ++i) {
        if ((*i).getUserId() == iMsgId) {
            std::cout << "IdIdentifier::identityUser  not_add_uSER___ok__" << std::endl;
            return 0;
        }
    }
    goto END;
}
