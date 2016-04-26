/*************************************************************************
	> File Name: message_shunter.h
	> Author:CaiYaodeng 
	> Mail:512324191@qq.com 
	> Created Time: Wed 13 Apr 2016 07:34:40 AM EDT
 ************************************************************************/

#ifndef _MESSAGE_SHUNTER_H_
#define _MESSAGE_SHUNTER_H_

#include <iostream>
#include "message.h"
#include "id_identifier.h"
#include "transit_center.h"
#include "user_configure_updater.h"

class MessageShunter {
    public:
        MessageShunter();
        ~MessageShunter();
    public:
        int shuntMsg(Message *pMsg);
};
#endif
