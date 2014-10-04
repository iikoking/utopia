/*
 * IDB.h
 * Created on: 2013-6-10
 *	   Author: qianqians
 *  数据库基础接口
 */
#ifndef _IDB_H
#define _IDB_H

#include <utopia/base/u_stdint.h>
#include <utopia/base/IObj.h>

namespace utopia{
namespace DB{

struct db_handle;

struct db_group{
	db_handle * db_handle;
	String group_key;
};

struct db_obj{
	db_group * db_group;
	String obj_key;
	OBJID id;


};

class IDB{
public:
	virtual db_handle * create_db(const String, const String, const String, const String, const UInteger) = 0;
	virtual void close_db_handle(db_handle * db_handle) = 0;
	virtual db_group * create_db_group(const db_handle * , const String) = 0;
	virtual void close_db_group(db_group * db_group) = 0;
	virtual db_obj * create_db_obj(const db_group *, const String) = 0;
	virtual void close_db_obj(db_obj * db_obj) = 0;
	virtual bool set_db_obj_item(const db_obj *, const String, const String) = 0;
	virtual String get_db_obj_item(const db_obj *, const String) = 0;
	virtual bool set_db_obj_container_item(const db_obj *, const String, const String, const String) = 0;
	virtual String get_db_obj_container_item(const db_obj *, const String, const String) = 0;
};

}// DB
}// utopia

#endif //_IDB_H