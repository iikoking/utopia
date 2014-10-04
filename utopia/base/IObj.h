/*
 * IObj.h
 *  Created on: 2013-4-24
 *      Author: qianqians
 * obj 接口 
 */
#ifndef _IOBJ_H
#define _IOBJ_H

#include "u_stdint.h"
#include "varlist.h"

namespace utopia {
namespace base{

class IObj{
public:
	//属性接口
	virtual bool addattribute(const String & key, const int valuetype) = 0;
	virtual bool removeattribute(const String & key) = 0;
	virtual bool findattribute(const String & key) = 0;
	virtual bool setattribute(const String & key, const Integer value) = 0;
	virtual bool setattribute(const String & key, const UInteger value) = 0;
	virtual bool setattribute(const String & key, const Float value) = 0;
	virtual bool setattribute(const String & key, const String & value) = 0;
	virtual bool setattribute(const String & key, const Object value) = 0;
	virtual Integer getattributeinteger(const String & key) = 0;
	virtual UInteger getattributeuinteger(const String & key) = 0;
	virtual Float getattributefloat(const String & key) = 0;
	virtual String getattributestring(const String & key) = 0;
	virtual Object getattributeobject(const String & key) = 0;

	//容器接口
	virtual bool addtable(const String & key, const int col, ...) = 0;
	virtual bool removetable(const String & key) = 0;
	virtual bool findtable(const String & key) = 0;
	virtual bool inserttablerow(const String & key, const varlist & _varlist) = 0;
	virtual bool settablevalue(const String & key, const int col, const int row, const Integer value) = 0;
	virtual bool settablevalue(const String & key, const int col, const int row, const UInteger value) = 0;
	virtual bool settablevalue(const String & key, const int col, const int row, const Float value) = 0;
	virtual bool settablevalue(const String & key, const int col, const int row, const String & value) = 0;
	virtual bool settablevalue(const String & key, const int col, const int row, const Object value) = 0;
	virtual Integer gettablevalueinteger(const String & key, const int col, const int row) = 0;
	virtual UInteger gettablevalueuinteger(const String & key, const int col, const int row) = 0;
	virtual Float gettablevaluefloat(const String & key, const int col, const int row) = 0;
	virtual String gettablevaluestring(const String & key, const int col, const int row) = 0;
	virtual Object gettablevalueobject(const String & key, const int col, const int row) = 0;

	//远程调用接口
	virtual void createremoteobj(Integer serverid) = 0;

	OBJID ID(){return id;}

private:
	OBJID id;

};

}// clutter
}// utopia

#endif //_IOBJ_H