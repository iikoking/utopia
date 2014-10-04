/*
 * DB_Mysql.h
 * Created on: 2013-6-11
 *	   Author: qianqians
 *  Mysql数据库接口
 */
#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include <mysql.h>
#include <boost/pool/pool_alloc.hpp>
#include <angelica/exception/exception.h>

#include <utopia/base/strtools.h>

#include "IDB.h"

namespace utopia{
namespace DB{

struct db_handle{
	MYSQL mysql;
	String db;
};

struct db_group{
	db_handle * handle;
	String table;
};

struct db_obj{
	db_group * group;
	String name;
	String type;
	Integer id;
};

template <class _alloc = boost::pool_allocator<db_handle> >
class DB_Mysql : public IDB{
public:
	DB_Mysql(){}
	~DB_Mysql(){}

	virtual db_handle * create_db(const String & db, const String & user, const String & passw, const String & host, const UInteger port){
		db_handle * _handle = _alloc_handle.allocate(1);
		_alloc_handle.construct(_handle);

		if (mysql_init(&_handle->mysql) == 0){
			close_db_handle(_handle);
			throw angelica::exception::exception(String("init mysql") + mysql_error(&db_handle->mysql));
		}else{
			if (mysql_real_connect(&_handle->mysql, host.c_str(), user.c_str(), passw.c_str(), db.c_str(), port, 0, 0) == 0){
				close_db_handle(_handle);
				throw angelica::exception::exception(String("real connect mysql") + mysql_error(&db_handle->mysql));
			}
		}

		return _handle;
	}

	virtual void close_db_handle(db_handle * db_handle){
		mysql_close(&db_handle->mysql);

		_alloc_handle.destory(db_handle);
		_alloc_handle.deallocate(db_handle, 1);
	}

	virtual db_group * create_db_group(const db_handle * db_handle, const String table){
		db_group * _group = 0;

		String sql = String("create table if not exists ") + table + 
			String("id_key int(11) not null, str_name varchar(32) not null, str_type varchar(32) not null, property text");

		if (mysql_query(&db_handle->mysql, sql.c_str()) == 0){//succeeded
			 _group = _alloc_group.allocate(1);
			_alloc_group.construct(_group);

			_group->handle = db_handle;
			_group->table = table;
		}else{
			throw angelica::exception::exception(sql + mysql_error(&db_handle->mysql));
		}

		return _group;
	}

	virtual void close_db_group(db_group * db_group){
		_alloc_group.destory(db_group);
		_alloc_group.deallocate(db_group, 1);
	}

	virtual db_obj * create_db_obj(const db_group * db_group, const Integer id, const String & name, const String & type){
		db_obj * _obj = 0;

		String sql = String("insert into ") + db_group->table + String(" values(") + strtools::int2str(id) + String(",") + name + String(",") + type + String(")");

		if (mysql_query(&db_group->handle->mysql, sql.c_str()) == 0){//succeeded
			 _obj = _alloc_obj.allocate(1);
			_alloc_obj.construct(_obj);

			_obj->group = db_group;
			_obj->key = name;
			_obj->id = id;
		}else{
			throw angelica::exception::exception(sql + mysql_error(&db_group->handle->mysql));
		}

		return _obj;
	}

	virtual void close_db_obj(db_obj * db_obj){
		_alloc_obj.destory(db_obj);
		_alloc_obj.deallocate(db_obj, 1);
	}

	virtual void set_db_obj_item(const db_obj * db_obj, const String key, const String value){
		String sql = String("UPDATE ") + db_obj->group->table + String(" SET(") + key + String("=") + 
			String("'") + value + String("'") + String("where id_key = '") + db_obj->id + String("'");

		if (mysql_query(&db_group->handle->mysql, sql.c_str()) == 0){//succeeded
			 _obj = _alloc_obj.allocate(1);
			_alloc_obj.construct(_obj);

			_obj->group = db_group;
			_obj->key = name;
			_obj->id = id;
		}else{
			throw angelica::exception::exception(sql + mysql_error(&db_group->handle->mysql));
		}
	}

	virtual String get_db_obj_item(const db_obj *, const String){
		;
	}

	virtual bool set_db_obj_container_item(const db_obj *, const String, const String, const String);
	virtual String get_db_obj_container_item(const db_obj *, const String, const String);

private:
	_alloc _alloc_handle;
	typename _alloc::tempalte rebind<db_group>::other _alloc_group;
	typename _alloc::tempalte rebind<db_obj>::other _alloc_obj;

};

}// DB
}// utopia

#endif //_DB_MYSQL_H