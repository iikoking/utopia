/*
 * varlist.h
 *  Created on: 2013-4-14
 *      Author: qianqians
 * varlist
 */
#ifndef _VARLIST_H
#define _VARLIST_H

#include <vector>
#include <boost/throw_exception.hpp>
#include <boost/shared_ptr.hpp>

#include "value.h"

namespace utopia {
namespace base {
class table;
}// base

class varlist{
public:
	varlist();
	~varlist();

	size_t count() const;

	int type(int index) const;

	//Integer
	void setint(int index, int32_t var);

	void setint(int index, int64_t var);

	Integer valueint(int index);

	//UInteger
	void setuint(int index, uint32_t var);

	void setuint(int index, uint64_t var);

	UInteger valueuint(int index);

	//OBJID
	void setobjid(int index, OBJID var);

	OBJID valueobjid(int index);

	//Float
	void setfloat(int index, float var);

	void setfloat(int index, double var);

	Float valuefloat(int index);

	//String
	void setstring(int index, String var);

	String valuestring(int index);

	//Integer
	varlist & operator << (int32_t var);

	varlist & operator << (int64_t var);

	//UInteger
	varlist & operator << (uint32_t var);

	varlist & operator << (uint64_t var);

	//Float
	varlist & operator << (float var);

	varlist & operator << (double var);

	//String
	varlist & operator << (String var);

	//Object
	varlist & operator << (Object var);

public:
	//返回总长度,传入缓冲区长度大于参数列表总长度时，将数据写入缓冲区 （待修改，下版本，传递参数使用本地缓冲区）
	size_t make_buff(char * buff, size_t llen);

private:
	std::vector<boost::shared_ptr<base::value> > _varlist;

	friend class base::table;

};

}// utopia

#endif //_VARLIST_H