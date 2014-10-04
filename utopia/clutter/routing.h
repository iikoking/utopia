/*
 * routing.h
 *  Created on: 2013-4-23
 *      Author: qianqians
 * routing
 */
#ifndef _ROUTING_H 
#define _ROUTING_H

#include <angelica/container/concurrent_interval_table.h>

#include "../base/u_stdint.h"

namespace utopia{
namespace clutter{

template<typename connection_info>
class routing{
public:
	routing();
	~routing();

	bool addobj(OBJID objid, connection_info info){
		return routing_table.insert(objid, info);
	}

	bool deleteobj(OBJID objid){
		return routing_table.erase(objid);
	}

	bool search(OBJID objid, connection_info & info){
		return routing_table.search(objid, info);
	}

	bool setobj(OBJID objid, connection_info info){
		return routing_table.set(objid, info);
	}

	void for_each(boost::function<void(connection_info & var) > handle){
		routing_table.for_each(handle);
	}

private:
	angelica::container::concurrent_interval_table<OBJID, connection_info> routing_table;

};

}// clutter
}// utopia

#endif //_ROUTING_H