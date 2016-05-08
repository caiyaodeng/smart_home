/*************************************************************************
	> File Name: protocol_cammand.h
	> Author:CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 20 Apr 2016 03:46:38 AM EDT
 ************************************************************************/

#ifndef _PROTOCOL_CAMMAND_H_
#define _PROTOCOL_CAMMAND_H_

    /*Contrl CMD*/
#define COMMAND_SIZE 32
#define TIMER_ENABLE  '1'  /*Timer*/
#define TIMER_DISABLE  '0'
#define DAILY  '0'
#define MOM '1' 
#define TUE  '2'
#define WED  '3'
#define THU  '4'
#define FRI  '5'
#define SAT  '6'
#define SUN  '7'
#define ON '1'  /*Power*/
#define OFF '0'

    /*Conf Contrl CMD*/
#define CONTRL_SIZE 128
#define CONF_ENABLE  '1'     /*Conf*/
#define CONF_DISABLE  '0'
#define ROOM_DEVICE  '0'
#define MODEL_DEVICE '1'
#define TIME_ADJ  '1'         /*Time Adjusting*/
#define NO_TIME_ADJ  '0'
#define UPD  '1'             /*Remote Update*/
#define NO_UPD  '0'
#define RESET  '1'           /*Reset Conf*/
#define NO_RESET  '0'
#define COLLECT '1'         /*Collect*/
#define NO_COLLECT '0'

namespace cmd {
    /*data of Contrl CMD*/
    typedef union __protocol_command {
        struct _protocol_command {
            uint8_t timer[1]; 
            uint8_t long_term[1];
            uint8_t time[8];
            uint8_t on_off[1];
            uint8_t intensity[3];
            uint8_t reserve[18];
        };
        uint8_t protocol_command_set_data[COMMAND_SIZE];
    }ProtocolCommandSet;
    
   /*data of Conf Contrl CMD*/
    typedef struct _protocol_contrl_command {
        uint8_t conf_opt[1]; 
        uint8_t conf_way[1];
        uint8_t name[16];
        uint8_t device_serial[16];
        ProtocolCommandSet default_action[1];
        uint8_t time_adjust[1];
        uint8_t remote_update[1];
        uint8_t reset[1];
        uint8_t reserve[123];
    }ProtocolContrl;

    typedef union __protocol_contrl_command {
        ProtocolContrl protocol_contrl;
        uint8_t protocol_contrl_set_data[CONTRL_SIZE];
    }ProtocolContrlSet;
}
#endif
