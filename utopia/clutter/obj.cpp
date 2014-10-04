/*
 * obj.cpp
 *  Created on: 2013-4-24
 *      Author: qianqians
 * obj
 */
#include "obj.h"
#include "../base/table.h"
#include "../base/attribute.h"

#include <stdarg.h>

namespace utopia {
namespace clutter{

obj::obj(){
}

obj::~obj(){
	for(std::map<String, base::attribute * >::iterator iter = _map_attribute.begin(); iter != _map_attribute.end(); iter++){
		delete iter->second;
	}

	for(std::map<String, base::table * >::iterator iter = _map_table.begin(); iter != _map_table.end(); iter++){
		delete iter->second;
	}
}

bool obj::addattribute(const String & key, const int valuetype){
	base::attribute * p = new base::attribute();
	if (!p->create(key, valuetype)){
		return false;
	}
	_map_attribute.insert(std::make_pair(key, p));

	return true;
}

bool obj::removeattribute(const String & key){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}
	_map_attribute.erase(iter);

	return true;
}
	
bool obj::findattribute(const String & key){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return true;
}
	
bool obj::setattribute(const String & key, const Integer value){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return iter->second->setattribute(value);
}

bool obj::setattribute(const String & key, const UInteger value){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return iter->second->setattribute(value);
}

bool obj::setattribute(const String & key, const Float value){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return iter->second->setattribute(value);
}

bool obj::setattribute(const String & key, const String & value){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return iter->second->setattribute(value);
}
	
bool obj::setattribute(const String & key, const Object value){
	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter == _map_attribute.end()){
		return false;
	}

	return iter->second->setattribute(value);
}
	
Integer obj::getattributeinteger(const std::string & key){
	Integer ret = 0;

	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter != _map_attribute.end()){
		iter->second->getattribute(ret);
	}

	return ret;
}

UInteger obj::getattributeuinteger(const std::string & key){
	UInteger ret = 0;

	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter != _map_attribute.end()){
		iter->second->getattribute(ret);
	}

	return ret;
}

Float obj::getattributefloat(const std::string & key){
	Float ret = 0;

	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter != _map_attribute.end()){
		iter->second->getattribute(ret);
	}

	return ret;
}

String obj::getattributestring(const std::string & key){
	String ret = "";

	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter != _map_attribute.end()){
		iter->second->getattribute(ret);
	}

	return ret;
}
	
Object obj::getattributeobject(const std::string & key){
	Object ret = 0;

	std::map<String, base::attribute * >::iterator iter = _map_attribute.find(key);
	if (iter != _map_attribute.end()){
		iter->second->getattribute(ret);
	}

	return ret;
}

bool obj::addtable(const String & key, const int col, ...){
	base::table * ptable = new base::table();

	va_list p;
	va_start(p, col);
	std::vector<int > vectortype;
	vectortype.resize(col);
	for (int i = 0; i < col; i++){
		vectortype[i] = va_arg(p, int);
	}
	va_end(p);

	ptable->create(key, vectortype);
	
	_map_table.insert(std::make_pair(key, ptable));
	
	return true;
}

bool obj::removetable(const String & key){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	_map_table.erase(iter);

	return true;
}

bool obj::findtable(const String & key){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return true;
}

bool obj::inserttablerow(const String & key, const varlist & _varlist){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->insert(_varlist);

	return true;
}

bool obj::settablevalue(const String & key, const int col, const int row, const Integer value){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->setvalue(col, row, value);

	return true;
}

bool obj::settablevalue(const String & key, const int col, const int row, const UInteger value){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->setvalue(col, row, value);

	return true;
}

bool obj::settablevalue(const String & key, const int col, const int row, const Float value){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->setvalue(col, row, value);

	return true;
}

bool obj::settablevalue(const String & key, const int col, const int row, const String & value){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->setvalue(col, row, value);

	return true;
}

bool obj::settablevalue(const String & key, const int col, const int row, const Object value){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}
	iter->second->setvalue(col, row, value);

	return true;
}

Integer obj::gettablevalueinteger(const String & key, const int col, const int row){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return iter->second->getvalueint(col, row);
}

UInteger obj::gettablevalueuinteger(const String & key, const int col, const int row){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return iter->second->getvalueuint(col, row);
}
	
Float obj::gettablevaluefloat(const String & key, const int col, const int row){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return iter->second->getvaluefloat(col, row);
}
	
String obj::gettablevaluestring(const String & key, const int col, const int row){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return iter->second->getvaluestring(col, row);;
}
	
Object obj::gettablevalueobject(const String & key, const int col, const int row){
	std::map<String, base::table * >::iterator iter = _map_table.find(key);
	if (iter == _map_table.end()){
		return false;
	}

	return iter->second->getvalueobject(col, row);
}

}// clutter
}// utopia