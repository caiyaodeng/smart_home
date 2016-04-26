/*************************************************************************
	> File Name: message_shunter.cpp
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 07:35:26 AM EDT
 ************************************************************************/

#include "message_shunter.h"

MessageShunter::MessageShunter(){

}

MessageShunter::~MessageShunter() {

}

int MessageShunter::shuntMsg(Message *pMsg) {
    
    /*std::cout << "TaskId:" << iTaskId << std::endl;
    std::cout << "connection_type:" << pMsg->connection_type << std::endl;
    std::cout << "message_type:" << pMsg->message_type << std::endl;
    std::cout << "source_id:" << pMsg->source_id << std::endl;
    std::cout << "destination_id:" << pMsg->destination_id << std::endl;
    std::cout << "data_length:" << pMsg->data_length << std::endl;
    std::cout << "data_ptr:" << pMsg->data_ptr << std::endl;
    std::cout << "peer_addr.len_addr:" << pMsg->peer_addr.len_addr << std::endl;
    std::cout << "peer_addr.data_addr:" << pMsg->peer_addr.data_addr << std::endl;
    std::cout << "TaskId:" << iTaskId << std::endl;
    std::cout << "Bll::getMessage : ok" << std::endl; */

    if (strncmp("SYN", (const char *)pMsg->message_type, 3) == 0) {
        return 0;
    }

    else if (strncmp("SEQ", (const char *)pMsg->message_type, 3)  == 0){
        return 1;
    }

    else if (strncmp("UPD", (const char *)pMsg->message_type, 3)  == 0){
        return 2;
    }

    else if (strncmp("ACK", (const char *)pMsg->message_type, 3)  == 0){
        return 3;
    }

    std::cout << "MessageShunter::shuntMsg : ok" << std::endl;
    return -1;
}
