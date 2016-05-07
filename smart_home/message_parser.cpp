/*************************************************************************
	> File Name: message_parser.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@QQ.com
	> Created Time: Wed 13 Apr 2016 07:33:22 AM EDT
 ************************************************************************/

#include "message_parser.h"
#include <stdio.h>
#include <iostream>

MessageParser::MessageParser() {

}

MessageParser::~MessageParser() {

}

Message *MessageParser::changeToMessage(const unsigned char *pUndoMsg, int iUndoMsgLength, unsigned char *&pPeerSendAddr, int *&pPeerLenAddr) {
    if (pUndoMsg == nullptr) {
        return nullptr;
    }

    /*close event*/
    //if (iUndoMsgLength == 1) {
    //    std::cout << "get close event1" << std::endl;
    //    return nullptr;
    //}

    Message *pMsg = (Message *)malloc(sizeof(Message)*sizeof(char));
    memset(pMsg, 0, sizeof(Message));
    unsigned char *pMsgBuffer = (unsigned char *)malloc(1024);
    memset(pMsgBuffer, 0, 1024);
    char *pInt = (char *)malloc(sizeof(int));
    memset(pInt, 0, sizeof(int));
    memcpy(pMsgBuffer, pUndoMsg, iUndoMsgLength);
    
    int iLeftIndex(-1),iLen(-1);

    iLeftIndex = 0;
    iLen = 1;
    memcpy(pMsg->connection_type, pMsgBuffer+iLeftIndex, iLen);        
     
    iLeftIndex = 1;
    iLen = 3;
    memcpy(pMsg->message_type, pMsgBuffer+iLeftIndex, iLen);        

    iLeftIndex = 4;
    iLen = 4;
    memcpy(pInt, pMsgBuffer+iLeftIndex, iLen);        
    pMsg->source_id = atoi(pInt);
    memset(pInt, 0, sizeof(int));
    
    iLeftIndex = 8;
    iLen = 4;
    memcpy(pInt, pMsgBuffer+iLeftIndex, iLen);        
    if (strncmp(pInt, "MULT", 4) == 0) {
        memcpy((void *)&(pMsg->destination_id), "MULT", 4);
    }
    else {
        pMsg->destination_id = atoi(pInt);
    }
    memset(pInt, 0, sizeof(int));
    //std::cout << "pMsg->destination:" << (char *)&pMsg->destination_id << std::endl;
    //std::cout << "pMsg->destination:" << pMsg->destination_id << std::endl;

    iLeftIndex = 12;
    iLen = 4;
    memcpy(pInt, pMsgBuffer+iLeftIndex, iLen);        
    pMsg->data_length = atoi(pInt);
    memset(pInt, 0, sizeof(int));

    iLeftIndex = 16;
    iLen = pMsg->data_length;
    pMsg->data_ptr = (uint8_t *)malloc(iLen);
    memcpy(pMsg->data_ptr, pMsgBuffer+iLeftIndex, iLen);

    pMsg->peer_addr.len_addr = (uint32_t *)pPeerLenAddr; 
    pMsg->peer_addr.data_addr = pPeerSendAddr;

    free(pInt);
    pInt = nullptr;
    free(pMsgBuffer);
    pMsgBuffer = nullptr;

    return pMsg;
}

int MessageParser::changeToBuf(unsigned char *&pBuf, Message *pMsg) {
    int iTotalLength = pMsg->data_length + 16 + 4 + 1;
    pBuf = (unsigned char *)malloc(iTotalLength * sizeof(char));
    memset(pBuf, 0, iTotalLength);
    char *pInt = (char *)malloc(sizeof(int));
    memset(pInt, 0, sizeof(int));

    int iLeftIndex(-1),iLen(-1);

    iLeftIndex = 0;
    iLen = 1;
    memcpy(pBuf+iLeftIndex, pMsg->connection_type, iLen);

    iLeftIndex = 1;
    iLen = 3;
    memcpy(pBuf+iLeftIndex, pMsg->message_type, iLen);

    iLeftIndex = 4;
    iLen = 4;
    sprintf(pInt, "%d", pMsg->source_id);
    memcpy(pBuf+iLeftIndex, pInt, iLen);

    iLeftIndex = 8;
    iLen = 4;
    sprintf(pInt, "%d", pMsg->destination_id);
    memcpy(pBuf+iLeftIndex, pInt, iLen);

    iLeftIndex = 12;
    iLen = 4;
    sprintf(pInt, "%d", pMsg->data_length);
    memcpy(pBuf+iLeftIndex, pInt, iLen);

    iLeftIndex = 16;
    iLen = pMsg->data_length;
    memcpy(pBuf+iLeftIndex, pMsg->data_ptr, iLen);

    return iTotalLength;
}
