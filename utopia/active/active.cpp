/*
 * active.cpp
 *
 *  Created on: 2013-1-21
 *      Author: qianqians
 */

#include "active.h"
#include "active_server.h"

namespace Hemsleya {
namespace active {

active::active(active_server & _active_server_) : _active_server(&_active_server_), _is_cancel(false){
	_current_active = &_mirco_active[0];
	_backstage_active = &_mirco_active[1];

	_active_server->post_mirco_active(_current_active);
}

active::~active(){
}

void active::post(event_handle _handle){
	while(!_current_active->post(_handle)){
		do{
			boost::unique_lock<boost::shared_mutex> lock(_mu, boost::try_to_lock);
			if (lock.owns_lock()){
				std::swap(_current_active, _backstage_active);
				_active_server->post_mirco_active(_current_active);
				lock.unlock();
				break;
			}
		}while(0);
	
		boost::shared_lock<boost::shared_mutex> shared_lock(_mu);
	}
}

void active::cancel(){
	_is_cancel.store(true);
}

bool active::is_cancel(){
	return _is_cancel.load();
}

} /* namespace active */
} /* namespace Hemsleya */