/*
 * obj.h
 *  Created on: 2013-4-23
 *      Author: qianqians
 * obj
 */
#ifndef _OBJ_H
#define _OBJ_H

#include <map>
#include "../base/varlist.h"
#include "../base/IObj.h"

namespace utopia {
namespace base {
class table;
class attribute;
}// base

namespace clutter{

class obj : public base::IObj{
public:
	obj();
	~obj();

	//属性接口
	bool addattribute(const String & key, const int valuetype);
	bool removeattribute(const String & key);
	bool findattribute(const String & key);
	bool setattribute(const String & key, const Integer value);
	bool setattribute(const String & key, const UInteger value);
	bool setattribute(const String & key, const Float value);
	bool setattribute(const String & key, const String & value);
	bool setattribute(const String & key, const Object value);
	Integer getattributeinteger(const String & key);
	UInteger getattributeuinteger(const String & key);
	Float getattributefloat(const String & key);
	String getattributestring(const String & key);
	Object getattributeobject(const String & key);

	//容器接口
	bool addtable(const String & key, const int col, ...);
	bool removetable(const String & key);
	bool findtable(const String & key);
	bool inserttablerow(const String & key, const varlist & _varlist);
	bool settablevalue(const String & key, const int col, const int row, const Integer value);
	bool settablevalue(const String & key, const int col, const int row, const UInteger value);
	bool settablevalue(const String & key, const int col, const int row, const Float value);
	bool settablevalue(const String & key, const int col, const int row, const String & value);
	bool settablevalue(const String & key, const int col, const int row, const Object value);
	Integer gettablevalueinteger(const String & key, const int col, const int row);
	UInteger gettablevalueuinteger(const String & key, const int col, const int row);
	Float gettablevaluefloat(const String & key, const int col, const int row);
	String gettablevaluestring(const String & key, const int col, const int row);
	Object gettablevalueobject(const String & key, const int col, const int row);

private:
	//属性回调


	//容器回调

private:
	std::map<String, base::attribute * > _map_attribute;
	std::map<String, base::table * > _map_table;



};

}// clutter
}// utopia

#endif //_OBJ_H