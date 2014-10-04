/*
 * routingserver.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * routingserver
 */
#ifndef _ROUTINGSERVER_H
#define _ROUTINGSERVER_H

#include <time.h>

#include <vector>

#include <boost/thread.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <angelica/async_net/async_net.h>
#include <angelica/active/active_server.h>
#include <angelica/container/msque.h>

#include "routing.h"
#include "routingmsg.h"

#include "../base/conffile.h"

namespace utopia{
namespace clutter{

class routingserver{
private:
	struct objinfo{
		OBJID id;
		uint32_t logicserverid; //所在logicserver
	};

	struct userconninfo{
		OBJID id;
		uint32_t gatewayserverid; 
		uint32_t connid;
	};

	struct serverinfo{
		uint32_t id;
		uint32_t addr;
		uint16_t port;
		angelica::async_net::socket s;
	};

	typedef std::vector<serverinfo, boost::pool_allocator<serverinfo> > VECTOR_SERVER_INFO;
	typedef std::vector<angelica::async_net::socket, boost::pool_allocator<angelica::async_net::socket> > VECTOR_CONN;

public:
	routingserver();
	~routingserver();

	bool config(String strPath);

	bool start();

	void stop();

private:
	void run();

private:
	void sendmsg(angelica::async_net::socket & s, char * buff, unsigned int lenbuff);
	
	void broadcast2gate(char * buff, unsigned int lenbuff);

	void broadcast2logic(char * buff, unsigned int lenbuff);

	void disconnectconn(angelica::async_net::socket & s);

private:
	void processmsg(angelica::async_net::socket & s, MsgHead * msg);

	void OnRegisterRoutingInfo(angelica::async_net::socket & s, register_routinginfo_msg * msg);

	void OnSearchRoutingInfo(angelica::async_net::socket & s, search_routinginfo_msg * msg);

	void OnDeleteRoutingInfo(angelica::async_net::socket & s, delete_routinginfo_msg * msg);

	void OnSetRoutingInfo(angelica::async_net::socket & s, set_routinginfo_msg * msg);

	void OnRegisterGateWayRoutingInfo(angelica::async_net::socket & s, register_gatewayserver_routinginfo_msg * msg);

	void OnRegisterGateWayRoutingInfoConfirm(angelica::async_net::socket & s, register_gatewayserver_routinginfo_msg_confirm * msg);

	void OnRegisterLogicRoutingInfo(angelica::async_net::socket & s, register_logicwayserver_routinginfo_msg * msg);

	void OnRegisterUserRoutingInfo(angelica::async_net::socket & s, register_user_routinginfo_msg * msg);

	void OnSearchUserRoutingInfo(angelica::async_net::socket & s, search_user_routinginfo_msg * msg);

	void OnDeleteUserRoutingInfo(angelica::async_net::socket & s, delete_user_routinginfo_msg * msg);

	void OnSetUserRoutingInfo(angelica::async_net::socket & s, set_user_routinginfo_msg * msg);

private:
	void OnAccept(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err);

	void OnRecv(angelica::async_net::socket & s, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err);

private:
	base::conffile conf;

	uint32_t nconcurrent;
	boost::atomic_bool brun;

	boost::thread_group th_group;

	angelica::async_net::async_service _async_service;
	angelica::async_net::socket _socket;

	//对象路由信息 对象所在服务器
	routing<objinfo > objroutinginfo;

	//对象路由信息 用户连接信息
	routing<userconninfo > userroutinginfo;

	//网关服务器连接
	boost::atomic_uint32_t gateway_id;
	boost::shared_mutex _vector_gateway_mu;
	VECTOR_SERVER_INFO vector_gateway;
	angelica::container::msque<uint32_t> que_free_gateway_id;

	//逻辑服务器连接 
	boost::atomic_uint32_t logic_id;
	boost::shared_mutex _vector_logic_mu;
	VECTOR_SERVER_INFO vector_logic;
	angelica::container::msque<uint32_t> que_free_logic_id;

};

}// clutter
}// utopia

#endif //_ROUTINGSERVER_H