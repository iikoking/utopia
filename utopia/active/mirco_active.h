/*
 * mirco_active.h
 *
 *  Created on: 2012-3-27
 *      Author: qianqians
 */

#ifndef MIRCO_ACTIVE_H_
#define MIRCO_ACTIVE_H_

#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <Hemsleya/base/concurrent/abstract_factory/abstract_factory.h>

namespace Hemsleya {
namespace active {

typedef boost::function<void(void) > event_handle;

class active;

class mirco_active {
public:
	mirco_active();
	~mirco_active();

	bool post(event_handle _handle);

	bool do_one();
	
	void run();

private:
	friend class active_server;

	active * owner;

	struct node{
		node(event_handle _event_handle_) : _event_handle(_event_handle_), _next(0){}

		event_handle _event_handle;
		node * _next;
	};

	abstract_factory::abstract_factory<node> _abstract_factory_node;

	boost::shared_mutex _mu;
	boost::atomic<node *> _list;

};

} /* namespace active */
} /* namespace Hemsleya */
#endif /* MIRCO_ACTIVE_H_ */
