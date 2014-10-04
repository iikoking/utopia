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
	//����ע��·����Ϣ
	REGISTER_ROUTING_INFO_REQ = 1,
	REGISTER_ROUTING_INFO_ACK,

	//��ѯ·����Ϣ
	SEARCH_ROUTING_INFO_REQ,
	SEARCH_ROUTING_INFO_ACK,

	//ɾ��·����Ϣ
	DELTE_ROUTING_INFO_REQ,
	DELTE_ROUTING_INFO_ACK,

	//����·����Ϣ
	SET_ROUTING_INFO_REQ,
	SET_ROUTING_INFO_ACK,

	//gatewayserver������Ϣ
	REGISTER_GATEWAY_INFO_REQ,
	REGISTER_GATEWAY_INFO_ACK,
	REGISTER_GATEWAY_INFO_CONFIRM,

	//logicserver������Ϣ
	REGISTER_LOGIC_INFO_REQ,
	REGISTER_LOGIC_INFO_ACK,

	//�㲥���ط�������Ϣ��logic server
	BROADCAST_GATEWAY_INFO,

	//ע���û�·����Ϣ
	REGISTER_USER_ROUTING_INFO_REQ,
	REGISTER_USER_ROUTING_INFO_ACK,

	//��ѯ�û�·����Ϣ
	SEARCH_USER_ROUTING_INFO_REQ,
	SEARCH_USER_ROUTING_INFO_ACK,

	//ɾ���û�·����Ϣ
	DELTE_USER_ROUTING_INFO_REQ,
	DELTE_USER_ROUTING_INFO_ACK,

	//�����û�·����Ϣ
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
	int32_t ret; //0�ɹ���1ʧ��
};

struct search_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct search_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t addr;
	uint16_t port;
	int32_t ret;//0�ɹ���1ʧ��
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
	int32_t ret; //0�ɹ���1ʧ��
};

struct register_gatewayserver_routinginfo_msg : public MsgHead{
	uint32_t addr;
	uint16_t port;
};

struct register_gatewayserver_routinginfo_msg_ack : public MsgHead{
	uint32_t serverid;
	uint32_t addr;
	uint16_t port;
	int32_t ret; //0�ɹ���1ʧ��
};

struct register_gatewayserver_routinginfo_msg_confirm : public MsgHead{
	uint32_t serverid;
	int32_t ret; //0�ɹ���1ʧ��
};

struct register_logicwayserver_routinginfo_msg : public MsgHead{
	uint32_t addr;
	uint16_t port;
};

struct register_logicwayserver_routinginfo_msg_ack : public MsgHead{
	uint32_t serverid;
	uint32_t addr;
	uint16_t port;
	int32_t ret; //0�ɹ���1ʧ��
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
	int32_t ret; //0�ɹ���1ʧ��
};

struct search_user_routinginfo_msg : public MsgHead{
	OBJID id;
};

struct search_user_routinginfo_msg_ack : public MsgHead{
	OBJID id;
	uint32_t gatewayserverid;
	uint32_t connid;
	int32_t ret;//0�ɹ���1ʧ��
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
	int32_t ret; //0�ɹ���1ʧ��
};

}// utopia

#endif //_ROUTINGMSG_H