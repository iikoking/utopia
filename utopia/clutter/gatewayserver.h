/*
 * gatewayserver.h
 *  Created on: 2013-4-23
 *      Author: qianqians
 * gatewayserver
 */
#ifndef _GATEWAYSERVER_H
#define _GATEWAYSERVER_H

#include <time.h>

#include <vector>

#include <boost/atomic.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/thread.hpp>

#include <angelica/container/msque.h>
#include <angelica/async_net/async_net.h>
#include <angelica/timer/timer.h>

#include "routing.h"
#include "OutMsg.h"
#include "InMsg.h"

#include "../base/u_stdint.h"
#include "../base/conffile.h"

namespace utopia{
namespace clutter{

class gatewayserver{
private:
	struct serverinfo{
		uint32_t serverid;
		uint32_t objcount;
		time_t timestamp;
		angelica::async_net::socket _socket;
	};

	struct userinfo{
		uint32_t connid;
		OBJID useid;
		uint32_t serverid;
		time_t timestamp;
		angelica::async_net::socket _socket;
	};

	enum conn_type{
		conn_type_logic = 1,
		conn_type_user = 2,
	};

public:
	gatewayserver();
	~gatewayserver();

	bool config(String strPath);

	bool start();

	void stop();

private:
	void run();

	void runtime();

private:
	void sendmsg2client(uint32_t connid, char * buff, unsigned int lenbuff);
	
	void sendmsg2server(uint32_t connid, char * buff, unsigned int lenbuff);

	void sendmsg(angelica::async_net::socket & s, char * buff, unsigned int lenbuff);
	
	void disconnectconn(angelica::async_net::socket & s);

	void disconnectconn(uint32_t connid, conn_type type);

private:
	void OnConnectRouting(angelica::async_net::_error_code err);

	void OnRecvRouting(char * buff, unsigned int lenbuff, angelica::async_net::_error_code err);

	void processroutingmsg(MsgHead * msg);

	void OnRegisterGatewayInfoAck(register_gatewayserver_routinginfo_msg_ack * msg);

	bool RegisterGatewayRouting();

	bool ConfirmRegisterGatewayRouting();

private:
	void OnAcceptCluster(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err);
	
	void OnRecvClusterUnRegister(angelica::async_net::socket & s, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err);

	void OnRecvClusterRegister(uint32_t connid, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err);

	void procesclustersmsg(angelica::async_net::socket & s, MsgHead * msg);

	void OnRegisterGatewayLogicserver(angelica::async_net::socket & s, register_gateway_logicserver_info * msg);

	void procesoutmsg(OutMsg * msg);

	void procesbroadcastmsg(OutMsg * msg);

private:
	void OnAcceptClient(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err);

	void OnRecvClient(uint32_t connid, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err);

	void procesusermsg(uint32_t serverid, OBJID userid, InMsg * msg);

private:
	base::conffile conf;

	uint32_t nconcurrent;
	boost::atomic_bool brun;

	angelica::async_net::sock_addr addrcluster;
	angelica::async_net::sock_addr addrclient;

	uint32_t serverid;

	routing<serverinfo > routing_logic_server;

	//玩家连接 
	boost::atomic_uint32_t connid;
	routing<userinfo > routing_user_conn;
	angelica::container::msque<uint32_t> que_free_connid;

	//线程
	boost::thread_group th_group;

	//定时器
	angelica::timer::timer _timer;

	//网络
	angelica::async_net::async_service _async_service;
	angelica::async_net::socket _socket_routing;
	angelica::async_net::socket _socket_cluster;
	angelica::async_net::socket _socket_client;

};

}// clutter
}// utopia

#endif //_GATEWAYSERVER_H

