/*
 * active.h
 *
 *  Created on: 2013-1-21
 *      Author: qianqians
 */
#ifndef _ACTIVE_H
#define _ACTIVE_H

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include "mirco_active.h"

namespace Hemsleya {
namespace active {

class active_server;

class active {
public:
	active(active_server & _active_server_);
	~active();

	void post(event_handle _handle);

	void cancel();

	bool is_cancel();

private:
	active_server * _active_server;

	boost::atomic_bool _is_cancel;

	boost::shared_mutex _mu;
	mirco_active *_current_active, *_backstage_active;
	mirco_active _mirco_active[2];

};

} /* namespace mirco_active */
} /* namespace Hemsleya */

#endif //_ACTIVE_H