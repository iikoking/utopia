/*
 * gatewayserver.cpp
 *  Created on: 2013-5-4
 *      Author: qianqians
 * gatewayserver
 */
#include "routingmsg.h"
#include "gatewaymsg.h"
#include "gatewayserver.h"

#include "../base/wait_any.h"

#include <Windows.h>

#include <boost/exception/all.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace utopia{
namespace clutter{

gatewayserver::gatewayserver() :_socket_routing(_async_service), _socket_cluster(_async_service), _socket_client(_async_service){
#ifdef _WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	nconcurrent = info.dwNumberOfProcessors;
#endif //_WIN32
}

gatewayserver::~gatewayserver(){
}

bool gatewayserver::config(String strPath){
	if (!conf.loadfile(strPath)){
		return false;
	}

	return true;
}

void gatewayserver::run(){
	while(brun.load()){
		_async_service.run();
	}
}

void gatewayserver::runtime(){
	while(brun.load()){
		_timer.do_timer();
	}
}

bool gatewayserver::start(){
	brun.store(true);

	for (uint32_t i = 0; i < nconcurrent; i++){
		th_group.create_thread(boost::bind(&gatewayserver::run, this));
	}
	th_group.create_thread(boost::bind(&gatewayserver::runtime, this));

	//init socket 
	if (!conf.findvalue("net", "clusteraddr") || !conf.findvalue("net", "clusterport")){
		std::cout << "load conf failed" << std::endl;
		return false;
	}
	String addr = conf.getvalueString("net", "clusteraddr", "127.0.0.1");
	Integer port = conf.getvalueInteger("net", "clusterport", 0);
	addrcluster = angelica::async_net::sock_addr(addr.c_str(), (unsigned short)port);
	_socket_cluster.bind(addrcluster);
	_socket_routing.bind(angelica::async_net::sock_addr(addr.c_str(), 0));

	if (!conf.findvalue("net", "outsideaddr") || !conf.findvalue("net", "outsideport")){
		std::cout << "load conf failed" << std::endl;
		return false;
	}
	addr = conf.getvalueString("net", "outsideaddr", "127.0.0.1");
	port = conf.getvalueInteger("net", "outsideport", 0);
	addrclient = angelica::async_net::sock_addr(addr.c_str(), (unsigned short)port);
	_socket_client.bind(addrclient);

	//囧 才能形容我的心情
	struct Condition{
		Condition(){}
		~Condition(){}

		bool operator()(boost::atomic_int * _pCondition) {return 0 != _pCondition->load(); }

	};

	//连接routing服务器
	if (!conf.findvalue("net", "routingaddr") || !conf.findvalue("net", "routingport")){
		std::cout << "load conf failed" << std::endl;
		return false;
	}

	addr = conf.getvalueString("net", "routingaddr", "127.0.0.1");
	port = conf.getvalueInteger("net", "routingport", 0);

	boost::atomic_int isConnectRouting = 0;//0未连接,1连接成功,-1连接失败
	base::addwait("isConnectRouting", &isConnectRouting);

	_socket_routing.register_connect_handle(boost::bind(&gatewayserver::OnConnectRouting, this, _1));
	if (_socket_routing.async_connect(angelica::async_net::sock_addr(addr.c_str(), (unsigned short)port)) != angelica::async_net::socket_succeed){
		std::cout << "_socket_routing async_connect error " << std::endl;
		return false;
	}
	
	//等待连接routing
	boost::function<bool(boost::atomic_int * _pCondition) > fn_Condition = Condition();
	base::wait_any("isConnectRouting", boost::bind(fn_Condition, &isConnectRouting), boost::bind(&base::wait));

	//连接routing失败
	if (isConnectRouting.load() == -1){
		std::cout << "connect routing  error " << std::endl;
		return false;
	}

	std::cout << "connect routing success " << std::endl;

	//连接routing成功 注册接受消息回调
	_socket_routing.register_recv_handle(boost::bind(&gatewayserver::OnRecvRouting, this, _1, _2, _3));

	//register routing
	boost::atomic_int isRegisterRouting = 0;//0未注册,1注册成功,-1注册失败
	base::addwait("isRegisterRouting", &isRegisterRouting);

	if (!RegisterGatewayRouting()){
		std::cout << "register routing error" << std::endl;
		return false;
	}

	//等待注册routing完成
	base::wait_any("isRegisterRouting", boost::bind(fn_Condition, &isRegisterRouting), boost::bind(&base::wait));

	if (isRegisterRouting.load() == -1){
		std::cout << "routing ack register success" << std::endl;
		return false;
	}

	std::cout << "routing ack register error" << std::endl;

	//accept cluster 
	_socket_cluster.register_accpet_handle(boost::bind(&gatewayserver::OnAcceptCluster, this, _1, _2, _3));

	if (_socket_cluster.async_accpet(0, true) != angelica::async_net::socket_succeed){
		std::cout << "create accept cluster error" << std::endl;
		return false;
	}

	std::cout << "start accept cluster" << std::endl;

	//accept client
	_socket_client.register_accpet_handle(boost::bind(&gatewayserver::OnAcceptClient, this, _1, _2, _3));
	if (_socket_client.async_accpet(0, true) != angelica::async_net::socket_succeed){
		std::cout << "create accept client error" << std::endl;
		return false;
	}

	std::cout << "start accept client" << std::endl;

	if (!ConfirmRegisterGatewayRouting()){
		std::cout << "confirm register routing error" << std::endl;
		return false;
	}

	//注册成功
	std::cout << "gateway server start" << std::endl;

	return true;
}

void gatewayserver::stop(){
	brun.store(false);

	struct disconnectserver{
		void operator()(serverinfo & info){
			info._socket.closesocket();
		}
	};
	routing_logic_server.for_each(disconnectserver());

	struct disconnectuser{
		void operator()(userinfo & info){
			info._socket.closesocket();
		}
	};
	routing_user_conn.for_each(disconnectuser());

	_socket_routing.async_accpet(false);
	_socket_routing.async_recv(false);

	_socket_cluster.async_accpet(false);

	_socket_client.async_accpet(false);

	th_group.join_all();
}

bool gatewayserver::RegisterGatewayRouting(){
	if (!conf.findvalue("net", "clusteraddr") || !conf.findvalue("net", "clusterport")){
		std::cout << "config cluster addr error" << std::endl;
		return false;
	}

	String addr = conf.getvalueString("net", "clusteraddr", "127.0.0.1");
	Integer port = conf.getvalueInteger("net", "clusterport", 0);

	register_gatewayserver_routinginfo_msg msg;
	msg.msgtype = ROUTING_MSG;
	msg.msgid = REGISTER_GATEWAY_INFO_REQ;
	msg.msglen = sizeof(register_gatewayserver_routinginfo_msg);
	msg.addr = angelica::async_net::sock_addr(addr.c_str(), (uint16_t)port).int_address();
	msg.port = angelica::async_net::sock_addr(addr.c_str(), (uint16_t)port).port();
	make_msg_check(&msg, msg.msglen);
	if (_socket_routing.async_send((char*)&msg, msg.msglen) != angelica::async_net::socket_succeed){
		std::cout << "_socket_routing async_send error" << std::endl;
		return false;
	}

	return true;
}

bool gatewayserver::ConfirmRegisterGatewayRouting(){
	//确认注册routing成功
	register_gatewayserver_routinginfo_msg_confirm confirm;
	confirm.msgtype = ROUTING_MSG;
	confirm.msgid = REGISTER_GATEWAY_INFO_CONFIRM;
	confirm.msglen = sizeof(register_gatewayserver_routinginfo_msg_confirm);
	confirm.serverid = serverid;
	confirm.ret = 0;
	make_msg_check(&confirm, confirm.msglen);
	if (_socket_routing.async_send((char*)&confirm, confirm.msglen != angelica::async_net::socket_succeed)){
		std::cout << "_socket_routing async_send error" << std::endl;
		return false;
	}

	return true;
}

void gatewayserver::sendmsg(angelica::async_net::socket & s, char * buff, unsigned int lenbuff){
	if (s.async_send((char*)&buff, lenbuff) != angelica::async_net::socket_succeed){
		//log error
		disconnectconn(s);
	}
}

void gatewayserver::sendmsg2client(uint32_t connid, char * buff, unsigned int lenbuff){
	userinfo info;
	routing_user_conn.search(connid, info);
	if (info._socket.async_send((char*)&buff, lenbuff) != angelica::async_net::socket_succeed){
		//log error
		disconnectconn(connid, conn_type_user);
	}
}
	
void gatewayserver::sendmsg2server(uint32_t connid, char * buff, unsigned int lenbuff){
	serverinfo info;
	routing_logic_server.search(connid, info);
	if (info._socket.async_send((char*)&buff, lenbuff) != angelica::async_net::socket_succeed){
		//log error
		disconnectconn(connid, conn_type_logic);
	}
}

void gatewayserver::OnConnectRouting(angelica::async_net::_error_code err){
	boost::atomic_int * isConnectRouting = (boost::atomic_int *)base::getwait("isConnectRouting");
	if (err == 0){
		isConnectRouting->store(1);
	}else{
		isConnectRouting->store(-1);
	}
}

void gatewayserver::OnRecvRouting(char * buff, unsigned int lenbuff, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t index = 0;
		while(lenbuff > index){
			MsgHead * msg = (MsgHead*)&buff[index];
			if (check_msg(msg, msg->msglen)){
				if (msg->msgtype != ROUTING_MSG){
					//不是路由请求，记入log
				}
				
				processroutingmsg(msg);
				index += msg->msglen;
			}else{
				//消息错误，断开连接
				disconnectconn(_socket_routing);
			}
		}
	}else{
		//接受错误，断开连接
		disconnectconn(_socket_routing);
	}
}

void gatewayserver::processroutingmsg(MsgHead * msg){
	switch(msg->msgid){
	case REGISTER_GATEWAY_INFO_ACK:
		OnRegisterGatewayInfoAck((register_gatewayserver_routinginfo_msg_ack *)msg);
		break;
	}
}

void gatewayserver::OnRegisterGatewayInfoAck(register_gatewayserver_routinginfo_msg_ack * msg){
	boost::atomic_int * isRegisterRouting = (boost::atomic_int *)base::getwait("isRegisterRouting");
	if (msg->ret == 0){
		serverid = msg->serverid;
		isRegisterRouting->store(1);
	}else{
		isRegisterRouting->store(-1);
	}
}

void gatewayserver::OnAcceptCluster(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err){
	if (err == 0){
		s.register_recv_handle(boost::bind(&gatewayserver::OnRecvClusterUnRegister, this, s, _1, _2, _3));
		s.async_recv(true);
	}
}
	
void gatewayserver::OnRecvClusterUnRegister(angelica::async_net::socket & s, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t index = 0;
		while(index < lenbuff){
			MsgHead * msg = (MsgHead *)&buff[index];
			index += msg->msglen;
			if (check_msg(msg, msg->msglen)){
				if (msg->msgtype == CLUSTER_MSG){
					procesclustersmsg(s, msg);
				}else{
					//error log
					disconnectconn(s);
				}
			}else{
				//error log
				disconnectconn(s);
			}
		}
	}else{
		//error log
		disconnectconn(s);
	}
}

void gatewayserver::procesclustersmsg(angelica::async_net::socket & s, MsgHead * msg){
	switch(msg->msgid){
	case REGISTER_GATEWAY_LOGICSERVER_INFO_REQ:
		OnRegisterGatewayLogicserver(s, (register_gateway_logicserver_info *)msg);
		break;

	}
}

void gatewayserver::OnRegisterGatewayLogicserver(angelica::async_net::socket & s, register_gateway_logicserver_info * msg){
	serverinfo info;
	info.serverid = msg->serverid;
	info.objcount = msg->objcount;
	time(&info.timestamp);
	info._socket = s;

	routing_logic_server.addobj(info.serverid, info);
}

void gatewayserver::OnRecvClusterRegister(uint32_t connid, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t index = 0;
		while(index < lenbuff){
			MsgHead * msg = (MsgHead *)&buff[index];
			index += msg->msglen;
			if (check_msg(msg, msg->msglen)){
				if (msg->msgtype == OUT_MSG){
					procesoutmsg((OutMsg*)msg);
				}else if (msg->msgtype == BROADCAST_MSG){
					procesbroadcastmsg((OutMsg*)msg);
				}else{
					//error log
					disconnectconn(connid, conn_type_logic);
				}
			}else{
				//error log
				disconnectconn(connid, conn_type_logic);
			}
		}
	}else{
		//error log
		disconnectconn(connid, conn_type_logic);
	}
}

//暂定，消息直接转发
void gatewayserver::procesoutmsg(OutMsg * msg){
	sendmsg2client(msg->connid, msg->msg, msg->llen);
}

void gatewayserver::procesbroadcastmsg(OutMsg * msg){
	struct send{
		void operator()(boost::function<void(uint32_t) > sendhandle, userinfo & info){ sendhandle(info.connid);}
	};

	boost::function<void(uint32_t) > sendhandle = boost::bind(&gatewayserver::sendmsg2client, this, _1, msg->msg, msg->llen);
	boost::function<void(boost::function<void(uint32_t) > sendhandle, userinfo & info) > _send = send();
	routing_user_conn.for_each(boost::bind(_send, sendhandle, _1));
}

void gatewayserver::OnAcceptClient(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t _connid;
		if (!que_free_connid.pop(_connid)){
			_connid = connid++;
		}

		userinfo info;
		info.connid = _connid;
		info.serverid = -1;//连接，未注册到服务器
		time(&info.timestamp);
		info._socket = s;
		routing_user_conn.addobj(_connid, info);

		s.register_recv_handle(boost::bind(&gatewayserver::OnRecvClient, this, _connid, _1, _2, _3));
		s.async_recv(true);
	}
}

void gatewayserver::OnRecvClient(uint32_t connid, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t index = 0;
		while(index < lenbuff){
			MsgHead * msg = (MsgHead *)&buff[index];
			index += msg->msglen;
			if (check_msg(msg, msg->msglen)){
				if (msg->msgtype == CLIENT_MSG){
					userinfo info;
					routing_user_conn.search(connid, info);
					procesusermsg(info.serverid, info.useid, (InMsg*)msg);
				}else{
					//error log
					disconnectconn(connid, conn_type_logic);
				}
			}else{
				//error log
				disconnectconn(connid, conn_type_logic);
			}
		}
	}else{
		//error log
		disconnectconn(connid, conn_type_logic);
	}
}

void gatewayserver::procesusermsg(uint32_t serverid, OBJID userid, InMsg * msg){
	//EventMsg eventmsg;

}

void gatewayserver::disconnectconn(angelica::async_net::socket & s){
	s.closesocket();
}

void gatewayserver::disconnectconn(uint32_t connid, conn_type type){
	if (type == conn_type_logic){
		serverinfo info;
		routing_logic_server.search(connid, info);
		info._socket.closesocket();
		routing_logic_server.deleteobj(connid);
	}else if (type == conn_type_user){
		userinfo info;
		routing_user_conn.search(connid, info);
		info._socket.closesocket();
		routing_logic_server.deleteobj(connid);
	}
}

}// clutter
}// utopia