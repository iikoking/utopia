/*
 * Msg.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * Msg
 */
#ifndef _MSG_H
#define _MSG_H

#include "../base/u_stdint.h"

namespace utopia{

#define FLAG 1359786240

enum MSG_TYPE{
	ROUTING_MSG = 1,
	CLUSTER_MSG,
	OBJ_MSG,
	OUT_MSG,
	BROADCAST_MSG,
	CLIENT_MSG,
};

struct MsgHead{
	uint32_t flag;
	uint32_t msgtype;
	uint32_t msgid;
	uint32_t msglen;
	uint32_t check;
};

inline void make_msg_check(void * msg, uint32_t llen){
	((MsgHead*)msg)->flag = FLAG;

	uint32_t nCheck = 0;
	for(uint32_t i = 0; i < sizeof(MsgHead)-sizeof(uint32_t); i++){
		nCheck += ((char*)msg)[i];
	}
	for(uint32_t i = sizeof(MsgHead); i < llen; i++){
		nCheck += ((char*)msg)[i];
	}
	((MsgHead*)msg)->check = nCheck/257;
}

inline bool check_msg(void * msg, uint32_t llen){
	if (((MsgHead*)msg)->flag != FLAG){
		return false;
	}

	uint32_t nCheck = 0;
	for(uint32_t i = 0; i < sizeof(MsgHead)-sizeof(uint32_t); i++){
		nCheck += ((char*)msg)[i];
	}
	for(uint32_t i = (int)sizeof(MsgHead); i < llen; i++){
		nCheck += ((char*)msg)[i];
	}

	return ((MsgHead*)msg)->check == nCheck;
}

}// utopia

#endif //_MSG_H