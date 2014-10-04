/*
 * routingmsg.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * routingmsg
 */
#ifndef _ROUTINGMSG_H
#define _ROUTINGMSG_H

#include "Msg.h"
#include "../base/u_stdint.h"

namespace utopia{

enum ROUTING_MSG{
	//对象注册路由信息
	REGISTER_ROUTING_INFO_REQ = 1,
	REGISTER_ROUTING_INFO_ACK,

	//查询路由信息
	SEARCH_ROUTING_INFO_REQ,
	SEARCH_ROUTING_INFO_ACK,

	//删除路由信息
	DELTE_ROUTING_INFO_REQ,
	DELTE_ROUTING_INFO_ACK,

	//设置路由信息
	SET_ROUTING_INFO_REQ,
	SET_ROUTING_INFO_ACK,

	//gatewayserver接入信息
	REGISTER_GATEWAY_INFO_REQ,
	REGISTER_GATEWAY_INFO_ACK,
	REGISTER_GATEWAY_INFO_CONFIRM,

	//logicserver接入信息
	REGISTER_LOGIC_INFO_REQ,
	REGISTER_LOGIC_INFO_ACK,

	//广播网关服务器信息到logic server
	BROADCAST_GATEWAY_INFO,

	//注册用户路由信息
	REGISTER_USER_ROUTING_INFO_REQ,
	REGISTER_USER_ROUTING_INFO_ACK,

	//查询用户路由信息
	SEARCH_USER_ROUTING_INFO_REQ,
	SEARCH_USER_ROUTING_INFO_ACK,

	//删除用户路由信息
	DELTE_USER_ROUTING_INFO_REQ,
	DELTE_USER_ROUTING_INFO_ACK,

	//设置用户路由信息
	SET_USER_ROUTING_INFO_REQ,
	SET_USER_ROUTING_INFO_ACK,

};

struct register_routinginfo_msg : public MsgHead{
	OBJID id;
	uint32_t serverid;
};

struct register_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t serverid;
	int32_t ret; //0成功，1失败
};

struct search_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct search_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t addr;
	uint16_t port;
	int32_t ret;//0成功，1失败
};

struct delete_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct delete_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	int32_t ret;
};

struct set_routinginfo_msg : public MsgHead{
	OBJID id;
	uint32_t serverid;
};

struct set_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t serverid;
	int32_t ret; //0成功，1失败
};

struct register_gatewayserver_routinginfo_msg : public MsgHead{
	uint32_t addr;
	uint16_t port;
};

struct register_gatewayserver_routinginfo_msg_ack : public MsgHead{
	uint32_t serverid;
	uint32_t addr;
	uint16_t port;
	int32_t ret; //0成功，1失败
};

struct register_gatewayserver_routinginfo_msg_confirm : public MsgHead{
	uint32_t serverid;
	int32_t ret; //0成功，1失败
};

struct register_logicwayserver_routinginfo_msg : public MsgHead{
	uint32_t addr;
	uint16_t port;
};

struct register_logicwayserver_routinginfo_msg_ack : public MsgHead{
	uint32_t serverid;
	uint32_t addr;
	uint16_t port;
	int32_t ret; //0成功，1失败
};

struct info_msg {
	uint32_t gatewayserverid;
	uint32_t addr;
	uint16_t port;
};

struct gateway_info_msg : public MsgHead{
	uint32_t count;
	//loop
	info_msg info[1];
	//...
};

struct register_user_routinginfo_msg : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
};

struct register_user_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
	int32_t ret; //0成功，1失败
};

struct search_user_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct search_user_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
	int32_t ret;//0成功，1失败
};

struct delete_user_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct delete_user_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	int32_t ret;
};

struct set_user_routinginfo_msg : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
};

struct set_user_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
	int32_t ret; //0成功，1失败
};

}// utopia

#endif //_ROUTINGMSG_H