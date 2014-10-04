/*
 * active_server.cpp
 *
 *  Created on: 2012-4-8
 *      Author: qianqians
 */

#include "active_server.h"
#include "mirco_active.h"

#include <Windows.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Hemsleya {
namespace active {

active_server::active_server() {
	// TODO Auto-generated constructor stub

	_current_pool = &_active_pool[0];
	_backstage_pool = &_active_pool[1];
}

active_server::~active_server() {
	// TODO Auto-generated destructor stub
}

void active_server::do_wait(){
	boost::shared_lock<boost::shared_mutex> shared_lock(_swap_mu);

	mirco_active * _active = next_active.get();
	if (_active == 0){
		if (_current_pool->pop(_active)){
			next_active.reset(_active);
		}
	}

	if (_active != 0){
		if (!_active->do_one()){
			next_active.release();
		}
	}
}

void active_server::run(){
	boost::shared_lock<boost::shared_mutex> shared_lock(_swap_mu);

	while(1){
		mirco_active * _active = next_active.get();
		if (_active == 0){
			_current_pool->pop(_active);
		}
		if (_active == 0){
			shared_lock.unlock();
			break;
		}

		_active->run();
	}

	boost::unique_lock<boost::shared_mutex> lock(_swap_mu, boost::try_to_lock);
	if (lock.owns_lock()){
		std::swap(_current_pool, _backstage_pool);
	}
}

void active_server::post_mirco_active(mirco_active * _mirco_active){
	_backstage_pool->push(_mirco_active);
}

} /* namespace active */
} /* namespace Hemsleya */
