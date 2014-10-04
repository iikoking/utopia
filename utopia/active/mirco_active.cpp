/*
 * mirco_active.cpp
 *
 *  Created on: 2012-3-27
 *      Author: qianqians
 */
#include "mirco_active.h"
#include "active_server.h"
#include "active.h"

namespace Hemsleya {
namespace active {

mirco_active::mirco_active() : _list(0){
}

mirco_active::~mirco_active(){
}

bool mirco_active::post(event_handle _handle){
	boost::shared_lock<boost::shared_mutex> lock(_mu, boost::try_to_lock);
	if (!lock.owns_lock()){
		return false;
	}
		
	node * _node = _abstract_factory_node.create_product(_handle);
	_node->_next = _list.exchange(_node);

	return true;
}

bool mirco_active::do_one(){
	if (owner->is_cancel()){
		for(node * _node = _list.load(boost::memory_order_relaxed); _node != 0; ){
			_list.store(_node->_next, boost::memory_order_relaxed);
			_abstract_factory_node.release_product(_node, 1);
		}
		return false;
	}

	boost::unique_lock<boost::shared_mutex> lock(_mu);

	node * _node = _list.load(boost::memory_order_relaxed);
	if (_node == 0){
		return false;
	}
	_list.store(_node->_next, boost::memory_order_relaxed);

	event_handle _handle = _node->_event_handle;
	_abstract_factory_node.release_product(_node, 1);
		
	_handle();

	return true;
}

void mirco_active::run(){
	if (owner->is_cancel()){
		for(node * _node = _list.load(boost::memory_order_relaxed); _node != 0; ){
			_list.store(_node->_next, boost::memory_order_relaxed);
			_abstract_factory_node.release_product(_node, 1);
		}
		return;
	}

	boost::unique_lock<boost::shared_mutex> lock(_mu);
		
	node * _node = 0;
	event_handle _handle;
	while(1){
		_node = _list.load(boost::memory_order_relaxed);
		if (_node != 0){
			_list.store(_node->_next, boost::memory_order_relaxed);
			_handle = _node->_event_handle;
			_abstract_factory_node.release_product(_node, 1);

			_handle();
		}else{
			break;
		}
	}
}

} /* namespace active */
} /* namespace Hemsleya */
