/*
 * gatewaymsg.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * routingmsg
 */
#ifndef _GATEWAYMSG_H
#define _GATEWAYMSG_H

#include "Msg.h"
#include "../base/u_stdint.h"

namespace utopia{

enum GATEWAY_MSG{
	//逻辑服务器连接网关服务器
	REGISTER_GATEWAY_LOGICSERVER_INFO_REQ = 1,
	REGISTER_GATEWAY_LOGICSERVER_INFO_ACK,

};

struct register_gateway_logicserver_info : public MsgHead{
	uint32_t serverid;
	uint32_t objcount;
};

}// utopia

#endif //_GATEWAYMSG_H