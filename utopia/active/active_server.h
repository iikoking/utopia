/*
 * active_server.h
 *
 *  Created on: 2012-4-8
 *      Author: qianqians
 */

#ifndef ACTIVE_SERVER_H_
#define ACTIVE_SERVER_H_

#include <vector>

#include <boost/thread.hpp>
#include <boost/atomic.hpp>

#include <Hemsleya/base/concurrent/container/msque.h>

namespace Hemsleya {
namespace active {

class active;
class mirco_active;

class active_server {
public:
	active_server();
	~active_server();

	void do_wait();

	void run();

private:
	void post_mirco_active(mirco_active *);

private:
	friend class mirco_active;
	friend class active;

	boost::thread_specific_ptr<mirco_active> next_active;

	boost::shared_mutex _swap_mu;
	Hemsleya::container::msque<mirco_active*> *_current_pool, *_backstage_pool;
	Hemsleya::container::msque<mirco_active*> _active_pool[2];

};

} /* namespace active */
} /* namespace Hemsleya */
#endif /* ACTIVESERVER_H_ */
