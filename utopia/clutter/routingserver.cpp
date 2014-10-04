/*
 * routingserver.cpp
 *  Created on: 2013-4-29
 *      Author: qianqians
 * routingserver
 */
#include "routingserver.h"

#include <Windows.h>

#include <boost/pool/singleton_pool.hpp>

namespace utopia{
namespace clutter{

routingserver::routingserver() : _socket(_async_service) {
#ifdef _WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	nconcurrent = info.dwNumberOfProcessors;
#endif //_WIN32
}

routingserver::~routingserver(){
}

bool routingserver::config(String strPath){
	if (!conf.loadfile(strPath)){
		return false;
	}

	return true;
}

bool routingserver::start(){
	brun.store(true);

	for(uint32_t i = 0; i < nconcurrent; i++){
		th_group.create_thread(boost::bind(&angelica::async_net::async_service::run, &_async_service));
	}

	_socket.register_accpet_handle(boost::bind(&routingserver::OnAccept, this, _1, _2, _3));
	
	if (!conf.findvalue("net", "addr") && !conf.findvalue("net", "port")){
		std::cout << "load conf failed" << std::endl;
		return false;
	}
	String addr = conf.getvalueString("net", "addr", "127.0.0.1");
	Integer port = conf.getvalueInteger("net", "port", 0);
	_socket.bind(angelica::async_net::sock_addr(addr.c_str(), (unsigned short)port));
	_socket.async_accpet(0, true);

	return true;
}

void routingserver::stop(){
	_socket.async_accpet(false);

	boost::shared_lock<boost::shared_mutex> lock(_vector_gateway_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_gateway.begin(); iter != vector_gateway.end(); iter++){
		iter->s.closesocket();
	}
	vector_gateway.clear();

	boost::shared_lock<boost::shared_mutex> lock1(_vector_logic_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_logic.begin(); iter != vector_logic.end(); iter++){
		iter->s.closesocket();
	}
	vector_logic.clear();
	
	brun.store(false);

	th_group.join_all();
}

void routingserver::run(){
	while(brun.load()){
		_async_service.run();
	}
}

void routingserver::sendmsg(angelica::async_net::socket & s, char * buff, unsigned int lenbuff){
	if (s.async_send((char*)&buff, lenbuff) != angelica::async_net::socket_succeed){
		//log error
		disconnectconn(s);
	}
}

void routingserver::broadcast2gate(char * buff, unsigned int lenbuff){
	boost::shared_lock<boost::shared_mutex> lock(_vector_gateway_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_gateway.begin(); iter != vector_gateway.end(); iter++){
		sendmsg(iter->s, buff, lenbuff);
	}
}

void routingserver::broadcast2logic(char * buff, unsigned int lenbuff){
	boost::shared_lock<boost::shared_mutex> lock(_vector_logic_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_logic.begin(); iter != vector_logic.end(); iter++){
		sendmsg(iter->s, buff, lenbuff);
	}
}

void routingserver::processmsg(angelica::async_net::socket & s, MsgHead * msg){
	switch(msg->msgid){
	case REGISTER_ROUTING_INFO_REQ:
		OnRegisterRoutingInfo(s, (register_routinginfo_msg *)msg);
		break;

	case SEARCH_ROUTING_INFO_REQ:
		OnSearchRoutingInfo(s, (search_routinginfo_msg *)msg);
		break;

	case DELTE_ROUTING_INFO_REQ:
		OnDeleteRoutingInfo(s, (delete_routinginfo_msg *)msg);
		break;
	
	case SET_ROUTING_INFO_REQ:
		OnSetRoutingInfo(s, (set_routinginfo_msg *)msg);
		break;

	case REGISTER_GATEWAY_INFO_REQ:
		OnRegisterGateWayRoutingInfo(s, (register_gatewayserver_routinginfo_msg *)msg);
		break;

	case REGISTER_GATEWAY_INFO_CONFIRM:
		OnRegisterGateWayRoutingInfoConfirm(s, (register_gatewayserver_routinginfo_msg_confirm *)msg);
		break;

	case REGISTER_LOGIC_INFO_REQ:
		OnRegisterLogicRoutingInfo(s, (register_logicwayserver_routinginfo_msg *)msg);
		break;

	case REGISTER_USER_ROUTING_INFO_REQ:
		OnRegisterUserRoutingInfo(s, (register_user_routinginfo_msg *)msg);
		break;

	case SEARCH_USER_ROUTING_INFO_REQ:
		OnSearchUserRoutingInfo(s, (search_user_routinginfo_msg *)msg);
		break;

	case DELTE_USER_ROUTING_INFO_REQ:
		OnDeleteUserRoutingInfo(s, (delete_user_routinginfo_msg *)msg);
		break;

	case SET_USER_ROUTING_INFO_REQ:
		OnSetUserRoutingInfo(s, (set_user_routinginfo_msg *)msg);
		break;

	

	}
}

void routingserver::OnRegisterUserRoutingInfo(angelica::async_net::socket & s, register_user_routinginfo_msg * msg){
	register_user_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = REGISTER_USER_ROUTING_INFO_ACK;
	ack.msglen = sizeof(register_user_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.gatewayserverid = msg->gatewayserverid;
	ack.connid = msg->connid;
	ack.ret = 0;

	userconninfo info;
	info.gatewayserverid = msg->gatewayserverid;
	info.connid = msg->connid;
	info.id = msg->id;
	if (!userroutinginfo.addobj(info.id, info)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnSearchUserRoutingInfo(angelica::async_net::socket & s, search_user_routinginfo_msg * msg){
	search_user_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = SEARCH_USER_ROUTING_INFO_ACK;
	ack.msglen = sizeof(search_user_routinginfo_msg_ack);
	ack.id = msg->id;

	userconninfo info;
	if (userroutinginfo.search(msg->id, info) && info.gatewayserverid < vector_logic.size()){
		ack.gatewayserverid = info.gatewayserverid;
		ack.connid = info.connid;
		ack.ret = 0;
	}else{
		ack.gatewayserverid = 0;
		ack.connid = 0;
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnDeleteUserRoutingInfo(angelica::async_net::socket & s, delete_user_routinginfo_msg * msg){
	delete_user_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = DELTE_USER_ROUTING_INFO_ACK;
	ack.msglen = sizeof(delete_user_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.ret = 0;

	if (!userroutinginfo.deleteobj(msg->id)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnSetUserRoutingInfo(angelica::async_net::socket & s, set_user_routinginfo_msg * msg){
	set_user_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = SET_USER_ROUTING_INFO_ACK;
	ack.msglen = sizeof(set_user_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.gatewayserverid = msg->gatewayserverid;
	ack.connid = msg->connid;
	ack.ret = 0;

	userconninfo info;
	info.id = msg->id;
	info.gatewayserverid = msg->gatewayserverid;
	info.connid = msg->connid;
	if (!userroutinginfo.setobj(msg->id, info)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnRegisterRoutingInfo(angelica::async_net::socket & s, register_routinginfo_msg * msg){
	register_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = REGISTER_ROUTING_INFO_ACK;
	ack.msglen = sizeof(register_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.serverid = msg->serverid;
	ack.ret = 0;

	objinfo info;
	info.id = msg->id;
	info.logicserverid = msg->serverid;
	if (!objroutinginfo.addobj(msg->id, info)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnSearchRoutingInfo(angelica::async_net::socket & s, search_routinginfo_msg * msg){
	search_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = SEARCH_ROUTING_INFO_ACK;
	ack.msglen = sizeof(search_routinginfo_msg_ack);
	ack.id = msg->id;

	objinfo info;
	if (objroutinginfo.search(msg->id, info) && info.logicserverid < vector_logic.size()){
		ack.addr = vector_logic[info.logicserverid].addr;
		ack.port = vector_logic[info.logicserverid].port;
		ack.ret = 0;
	}else{
		ack.addr = 0;
		ack.port = 0;
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnDeleteRoutingInfo(angelica::async_net::socket & s, delete_routinginfo_msg * msg){
	delete_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = DELTE_ROUTING_INFO_ACK;
	ack.msglen = sizeof(delete_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.ret = 0;

	if (!objroutinginfo.deleteobj(msg->id)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnSetRoutingInfo(angelica::async_net::socket & s, set_routinginfo_msg * msg){
	set_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = SET_ROUTING_INFO_ACK;
	ack.msglen = sizeof(set_routinginfo_msg_ack);
	ack.id = msg->id;
	ack.serverid = msg->serverid;
	ack.ret = 0;

	objinfo info;
	info.id = msg->id;
	info.logicserverid = msg->serverid;
	if (!objroutinginfo.setobj(msg->id, info)){
		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnRegisterGateWayRoutingInfo(angelica::async_net::socket & s, register_gatewayserver_routinginfo_msg * msg){
	register_gatewayserver_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = REGISTER_GATEWAY_INFO_ACK;
	ack.msglen = sizeof(register_gatewayserver_routinginfo_msg_ack);
	ack.ret = 0;

	boost::upgrade_lock<boost::shared_mutex> lock(_vector_gateway_mu);
	VECTOR_SERVER_INFO::iterator iter = vector_gateway.begin();
	for( ; iter != vector_gateway.end(); iter++){
		if (iter->s == s){
			break;
		}
	}
	if (iter == vector_gateway.end()){
		serverinfo info;
		if (!que_free_gateway_id.pop(info.id)){
			info.id = gateway_id++;
		}
		info.addr = msg->addr;
		info.port = msg->port;
		info.s = s;
		
		boost::unique_lock<boost::shared_mutex> unique_lock(boost::move(lock));
		vector_gateway.push_back(info);

		ack.serverid = info.id;
		ack.addr = info.addr;
		ack.port = info.port;
	}else{
		ack.serverid = 0;
		ack.addr = 0;
		ack.port = 0;

		ack.ret = 1;
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);
}

void routingserver::OnRegisterGateWayRoutingInfoConfirm(angelica::async_net::socket & s, register_gatewayserver_routinginfo_msg_confirm * msg){
	boost::shared_lock<boost::shared_mutex> lock(_vector_gateway_mu);
	if (vector_gateway[msg->serverid].s != s){
		return;
	}

	gateway_info_msg info;
	info.msgtype = ROUTING_MSG;
	info.msgid = BROADCAST_GATEWAY_INFO;
	info.msglen = sizeof(gateway_info_msg);
	info.count = 1;
	info.info->gatewayserverid = msg->serverid;
	info.info->addr = vector_gateway[msg->serverid].addr;
	info.info->port = vector_gateway[msg->serverid].port;
	make_msg_check(&info, info.msglen);

	broadcast2logic((char*)&info, info.msglen);
}

void routingserver::OnRegisterLogicRoutingInfo(angelica::async_net::socket & s, register_logicwayserver_routinginfo_msg * msg){
	register_logicwayserver_routinginfo_msg_ack ack;
	ack.msgtype = ROUTING_MSG;
	ack.msgid = REGISTER_LOGIC_INFO_ACK;
	ack.msglen = sizeof(register_logicwayserver_routinginfo_msg_ack);
	ack.ret = 0;

	boost::upgrade_lock<boost::shared_mutex> lock(_vector_logic_mu);
	VECTOR_SERVER_INFO::iterator iter = vector_logic.begin();
	if (iter == vector_logic.end()){
		serverinfo info;
		if (!que_free_gateway_id.pop(info.id)){
			info.id = gateway_id++;
		}
		info.addr = msg->addr;
		info.port = msg->port;
		info.s = s;

		boost::unique_lock<boost::shared_mutex> unique_lock(boost::move(lock));
		vector_logic.push_back(info);

		ack.serverid = info.id;
		ack.addr = info.addr;
		ack.port = info.port;
	}else{
		ack.serverid = 0;
		ack.addr = 0;
		ack.port = 0;

		ack.ret = 1;
		
	}
	make_msg_check(&ack, ack.msglen);
	sendmsg(s, (char*)&ack, ack.msglen);

	//向此服务器发送网关服务器信息
	boost::shared_lock<boost::shared_mutex> _lock(_vector_gateway_mu);
	uint32_t count = vector_gateway.size();
	uint32_t llen = sizeof(gateway_info_msg) + (count-1)*sizeof(info_msg);
	gateway_info_msg * info = (gateway_info_msg*)boost::singleton_pool<boost::fast_pool_allocator_tag, 1>::ordered_malloc(llen);
	info->msgtype = ROUTING_MSG;
	info->msgid = BROADCAST_GATEWAY_INFO;
	info->msglen = llen;
	info->count = count;
	for(VECTOR_SERVER_INFO::iterator iter = vector_gateway.begin(); iter != vector_gateway.end(); iter++){
		info->info->gatewayserverid = iter->id;
		info->info->addr = iter->addr;
		info->info->port = iter->port;
	}
	sendmsg(s, (char*)info, llen);
	boost::singleton_pool<boost::fast_pool_allocator_tag, 1>::ordered_free(info);
}

void routingserver::OnAccept(angelica::async_net::socket & s, angelica::async_net::sock_addr & addr, angelica::async_net::_error_code err){
	if (err == 0){
		s.register_recv_handle(boost::bind(&routingserver::OnRecv, this, s, _1, _2, _3));
		s.async_recv(true);
	}
}

void routingserver::OnRecv(angelica::async_net::socket & s, char * buff, unsigned int lenbuff, angelica::async_net::_error_code err){
	if (err == 0){
		uint32_t index = 0;
		while(lenbuff > index){
			MsgHead * msg = (MsgHead*)&buff[index];
			if (check_msg(msg, msg->msglen)){
				if (msg->msgtype != ROUTING_MSG){
					//不是路由请求，记入log
				}
			
				processmsg(s, msg);
				index += msg->msglen;
			}else{
				//消息错误，断开连接
				disconnectconn(s);
			}
		}
	}else{
		//接受错误，断开连接
		disconnectconn(s);
	}
}

void routingserver::disconnectconn(angelica::async_net::socket & s){
	s.closesocket();
		
	boost::upgrade_lock<boost::shared_mutex> lock(_vector_gateway_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_gateway.begin(); iter != vector_gateway.end(); iter++){
		if (iter->s == s){
			boost::unique_lock<boost::shared_mutex> unique_lock(boost::move(lock));
			vector_gateway.erase(iter);
			que_free_gateway_id.push(iter->id);
			break;
		}
	}

	boost::upgrade_lock<boost::shared_mutex> lock1(_vector_logic_mu);
	for(VECTOR_SERVER_INFO::iterator iter = vector_logic.begin(); iter != vector_logic.end(); iter++){
		if (iter->s == s){
			boost::unique_lock<boost::shared_mutex> unique_lock1(boost::move(lock));
			vector_gateway.erase(iter);
			que_free_logic_id.push(iter->id);
			break;
		}
	}
}

}// clutter
}// utopia