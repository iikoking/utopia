/*
 * value.h
 *  Created on: 2013-4-3
 *      Author: qianqians
 * value
 */
#ifndef _VALUE_H
#define _VALUE_H

#include <boost/exception/all.hpp>

#include "u_stdint.h"
#include "typedef.h"

namespace utopia {
namespace base {

class value{
public:
	value(){
		_value = 0; 
		_value_type = 0;
	}
	~value(){
		if (_value != 0){
			delete _value;
		}
	}

private:
	value(const value & _v){}
	void operator = (const value & _v){}

public:
	bool create(const int type){
		bool ret = true;

		_value_type = type;
		switch(_value_type){
		case utopia::VALUE_TYPE_INT:
			_value = (void*)(new int64_t);
			break;
		
		case utopia::VALUE_TYPE_UINT:
			_value = (void*)(new uint64_t);
			break;

		case utopia::VALUE_TYPE_DOUBLE:
			_value = (void*)(new double);
			break;

		case utopia::VALUE_TYPE_STRING:
			_value = (void*)(new std::string);
			break;

		case utopia::VALUE_TYPE_OBJECT:
			_value = (void*)(new Object);
			break;

		case utopia::VALUE_TYPE_OBJID:
			_value = (void*)(new OBJID);
			break;

		default:
			ret = false;
			break;
		}

		return true;
	}

	//int
	bool set(int64_t var){
		if (_value_type == utopia::VALUE_TYPE_INT){
			*((int64_t*)_value) = var;

			return true;
		}
		return false;
	}
	
	bool get(int64_t & var){
		if (_value_type == utopia::VALUE_TYPE_INT){
			var = *((int64_t*)_value);

			return true;
		}
		return false;
	}

	//uint
	bool set(uint64_t var){
		if (_value_type == utopia::VALUE_TYPE_UINT){
			*((uint64_t*)_value) = var;

			return true;
		}
		return false;
	}
	
	bool get(uint64_t & var){
		if (_value_type == utopia::VALUE_TYPE_UINT){
			var = *((uint64_t*)_value);

			return true;
		}
		return false;
	}

	////objid
	//bool set(OBJID var){
	//	if (_value_type == utopia::VALUE_TYPE_OBJID){
	//		*((OBJID*)_value) = var;

	//		return true;
	//	}
	//	return false;
	//}
	//
	//bool get(OBJID & var){
	//	if (_value_type == utopia::VALUE_TYPE_OBJID){
	//		var = *((OBJID*)_value);

	//		return true;
	//	}
	//	return false;
	//}

	//double
	bool set(double var){
		if (_value_type == utopia::VALUE_TYPE_DOUBLE){
			*((double*)_value) = var;

			return true;
		}
		return false;
	}
	
	bool get(double & var){
		if (_value_type == utopia::VALUE_TYPE_DOUBLE){
			var = *((double*)_value);

			return true;
		}
		return false;
	}

	//string
	bool set(std::string var){
		if (_value_type == utopia::VALUE_TYPE_STRING){
			*((std::string*)_value) = var;

			return true;
		}
		return false;
	}
	
	bool get(std::string & var){
		if (_value_type == utopia::VALUE_TYPE_STRING){
			var = *((std::string*)_value);

			return true;
		}
		return false;
	}

	//Object
	bool set(Object var){
		if (_value_type == utopia::VALUE_TYPE_OBJECT){
			*((Object*)_value) = var;

			return true;
		}
		return false;
	}
	
	bool get(Object & var){
		if (_value_type == utopia::VALUE_TYPE_OBJECT){
			var = *((Object*)_value);

			return true;
		}
		return false;
	}

	//返回数据类型
	int type(){
		return _value_type;
	}

private:
	void * _value;
	int _value_type;

};

} //base
} //utopia

#endif //_VALUE_H