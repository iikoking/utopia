/*
 * attribute.h
 *  Created on: 2013-4-14
 *      Author: qianqians
 * attribute
 */
#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <boost/signal.hpp>
#include "u_stdint.h"
#include "value.h"

namespace utopia {
namespace base {

class attribute{
public:
	attribute(){}
	~attribute(){}

private:
	attribute(const attribute & _a){}
	void operator = (const attribute & _a){}

public:
	bool create(const String & _key, const int _value_type){
		key = _key;
		if (val.create(_value_type)){
			sigCreat(_key, _value_type);
			return true;
		}
		return false;
	}	
	
	//int
	bool setattribute(int32_t var){
		bool bret =  val.set((int64_t)var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool setattribute(int64_t var){
		bool bret = val.set(var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool getattribute(int64_t & var){
		return val.get(var);
	}

	//uint
	bool setattribute(uint32_t var){
		bool bret =  val.set((uint64_t)var);
		sigSetAttribute(key, &val);

		return bret;
	}
	
	bool setattribute(uint64_t var){
		bool bret = val.set(var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool getattribute(uint64_t & var){
		return val.get(var);
	}
	
	////OBJID
	//bool setattribute(OBJID var){
	//	bool bret =  val.set(var);
	//	sigSetAttribute(key, &val);

	//	return bret;
	//}

	//bool getattribute(OBJID & var){
	//	return val.get(var);
	//}

	//double
	bool setattribute(double var){
		bool bret = val.set(var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool getattribute(double & var){
		return val.get(var);
	}

	//string
	bool setattribute(String var){
		bool bret =  val.set(var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool getattribute(String & var){
		return val.get(var);
	}

	//Object
	bool setattribute(Object var){
		bool bret =  val.set(var);
		sigSetAttribute(key, &val);

		return bret;
	}

	bool getattribute(Object & var){
		return val.get(var);
	}

public:
	boost::signal<void(const String & _key, const int _value_type) > sigCreat;
	boost::signal<void(const String & _key, const value * _val) > sigSetAttribute;

private:
	String key;
	value val;

};

}// base
}// utopia

#endif //_ATTRIBUTE_H