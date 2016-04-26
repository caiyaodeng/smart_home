/*************************************************************************
	> File Name: message.h
	> Author:CaiYaodeng 
	> Mail:512324291@qq.com 
	> Created Time: Wed 13 Apr 2016 10:32:20 PM EDT
 ************************************************************************/

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "protocol_command.h" /*get command type*/

typedef struct _peer_send_addr {
    uint32_t *len_addr;
    uint8_t *data_addr;
}peer_send_addr;

typedef struct _message {
    peer_send_addr peer_addr;
    uint8_t connection_type[1];
    uint8_t message_type[3];
    uint32_t source_id;
    uint32_t destination_id;
    uint32_t data_length;
    uint8_t *data_ptr;
}Message;

#endif
