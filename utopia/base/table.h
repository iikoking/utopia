/*
 * table.h
 *  Created on: 2013-4-14
 *      Author: qianqians
 * table
 */
#ifndef _TABLE_H
#define _TABLE_H

#include <vector>
#include <stdarg.h>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include "value.h"
#include "varlist.h"

namespace utopia {
namespace base {

class table{
public:
	table(){}
	~table(){}
	
private:
	table(const table &){}
	void operator = (const table &){}

public:
	bool create(const String & _key, const int col, ...){
		key = _key;

		va_list p;
		va_start(p, col);
		table_type.resize(col);
		for (int i = 0; i < col; i++){
			table_type[i] = va_arg(p, int);
		}
		va_end(p);

		sigCreat(_key, table_type);

		return true;
	}

	bool create(const String & _key, const std::vector<int > & vectortype){
		key = _key;

		table_type.resize(vectortype.size());
		for (int i = 0; i < (int)vectortype.size(); i++){
			table_type[i] = vectortype[i];
		}

		sigCreat(_key, table_type);

		return true;
	}

	bool insert(const varlist & _varlist){
		if (_varlist.count() > table_type.size()){
			return false;
		}

		for (int i = 0; i < (int)_varlist.count(); i++){
			if (_varlist.type(i) != table_type[i]){
				return false;
			}
		}

		_table.push_back(std::vector<boost::shared_ptr<base::value> >());
		int i = 0;
		for (; i < (int)_varlist.count(); i++){
			_table.back().push_back(_varlist._varlist[i]);
		}
		for (; i < (int)table_type.size(); i++){
			_table.back().push_back(boost::shared_ptr<base::value>(new base::value()));
			_table.back().back()->create(table_type[i]);
		}

		sigAddRow(key, _table.back());

		return true;
	}

	//int
	void setvalue(const int row, const int col, const int32_t var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set((int64_t)var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	void setvalue(const int row, const int col, const int64_t var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set(var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	Integer getvalueint(const int row, const int col){
		Integer ret;

		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->get(ret)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
		}

		return ret;
	}

	//uint
	void setvalue(const int row, const int col, const uint32_t var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set((uint64_t)var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	void setvalue(const int row, const int col, const uint64_t var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set(var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	UInteger getvalueuint(const int row, const int col){
		UInteger ret;

		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->get(ret)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
		}

		return ret;
	}

	////OBJID
	//void setvalue(const int row, const int col, const OBJID var){
	//	if (row >= (int)_table.size()){
	//		BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
	//	}

	//	if (col >= (int)table_type.size()){
	//		BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
	//	}

	//	if (!_table[row][col]->set((OBJID)var)){
	//		BOOST_THROW_EXCEPTION(std::logic_error("type is not OBJID"));
	//	}

	//	sigSet(key, row, col, _table[row][col].get());
	//}
	//
	//OBJID getvalueobjid(const int row, const int col){
	//	OBJID ret;

	//	if (row >= (int)_table.size()){
	//		BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
	//	}

	//	if (col >= (int)table_type.size()){
	//		BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
	//	}

	//	if (!_table[row][col]->get(ret)){
	//		BOOST_THROW_EXCEPTION(std::logic_error("type is not OBJID"));
	//	}

	//	return ret;
	//}

	//double
	void setvalue(const int row, const int col, const float var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set((double)var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	void setvalue(const int row, const int col, const double var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set(var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	Float getvaluefloat(const int row, const int col){
		Float ret;

		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->get(ret)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
		}

		return ret;
	}

	//string
	void setvalue(const int row, const int col, const String & var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set(var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not String"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	String getvaluestring(const int row, const int col){
		String ret;

		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->get(ret)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not String"));
		}

		return ret;
	}

	//Object
	void setvalue(const int row, const int col, const Object var){
		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->set(var)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Object"));
		}

		sigSet(key, row, col, _table[row][col].get());
	}

	Object getvalueobject(const int row, const int col){
		Object ret;

		if (row >= (int)_table.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent row"));
		}

		if (col >= (int)table_type.size()){
			BOOST_THROW_EXCEPTION(std::logic_error("inexistent col"));
		}

		if (!_table[row][col]->get(ret)){
			BOOST_THROW_EXCEPTION(std::logic_error("type is not Object"));
		}

		return ret;
	}

public:
	boost::signal<void(const String & key, const std::vector<int> & table_type)> sigCreat;
	boost::signal<void(const String & key, const std::vector<boost::shared_ptr<base::value> > & row) > sigAddRow;
	boost::signal<void(const String & key, const int row, const int col, const base::value * value) > sigSet;

private:
	String key;
	std::vector<int> table_type;
	std::vector<std::vector<boost::shared_ptr<base::value> > > _table;

};

}// base
}// utopia

#endif //_TABLE_H