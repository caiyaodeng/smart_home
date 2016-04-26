/*************************************************************************
	> File Name: message_parser.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:32:20 AM EDT
 ************************************************************************/

#ifndef _MESSAGE_PARSER_H_
#define _MESSAGE_PARSER_H_

#include "message.h"

class MessageParser {
    public:
        MessageParser();
        ~MessageParser();
        Message *changeToMessage(const unsigned char *pUndoMsg, int iUndoMsgLength, unsigned char *&pPeerSendAddr, int *&pPeerLenAddr);
        int changeToBuf(unsigned char *&pBuf, Message *pMsg);
};
#endif
