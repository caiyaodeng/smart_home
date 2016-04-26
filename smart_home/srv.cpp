/*************************************************************************
	> File Name: main.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Tue 05 Apr 2016 08:26:48 PM EDT
 ************************************************************************/

#include <iostream>
#include "../net/net_work.h"
#include "message_parser.h"
#include "bll.h"
using namespace std;

int main () {

    mynamespace::NetWork *objNet = new mynamespace::NetWork ();
    Bll *objBll = new Bll(objNet);
    MessageParser *objParser = new MessageParser ();
    int iTaskId = -1;
    unsigned char *pUndoMsg = (unsigned char *)malloc(128);
    memset (pUndoMsg, 0, 128);
    int iUndoMsgLength = -1;
    unsigned char *pDoneMsg = nullptr;
    int *pDoneMsgLength = nullptr;
    Message *pMessage = nullptr; 
    objNet->init();
    objBll->init();

    while(true) {
        iTaskId = objNet->pull_msg(pUndoMsg, &iUndoMsgLength, pDoneMsg, pDoneMsgLength);
        if (iTaskId == -1) {
            continue;
            return -1;
        }

        pMessage = objParser->changeToMessage(pUndoMsg, iUndoMsgLength, pDoneMsg, pDoneMsgLength);

        objBll->pushMessage(iTaskId, pMessage);
        memset (pUndoMsg, 0, 128);


    }

    cout << "-----ok----" << endl;
    return 0;
}
