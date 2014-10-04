/*
 * varlist.cpp
 *  Created on: 2013-5-12
 *      Author: qianqians
 * varlist
 */
#include "varlist.h"
#include "IObj.h"

namespace utopia {

varlist::varlist(){
}

varlist::~varlist(){
}

size_t varlist::count() const{
	return _varlist.size();
}

int varlist::type(int index) const{
	if (index < (int)_varlist.size()){
		return _varlist[index]->type();
	}
	return -1;
}

//Integer
void varlist::setint(int index, int32_t var){
	if (!_varlist[index]->set((int64_t)var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
	}
}

void varlist::setint(int index, int64_t var){
	if (!_varlist[index]->set(var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
	}
}

Integer varlist::valueint(int index){
	Integer ret = 0;

	if (!_varlist[index]->get(ret)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Integer"));
	}

	return ret;
}

//UInteger
void varlist::setuint(int index, uint32_t var){
	if (!_varlist[index]->set((uint64_t)var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
	}
}

void varlist::setuint(int index, uint64_t var){
	if (!_varlist[index]->set(var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
	}
}

UInteger varlist::valueuint(int index){
	UInteger ret = 0;

	if (!_varlist[index]->get(ret)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not UInteger"));
	}

	return ret;
}

//OBJID
void varlist::setobjid(int index, OBJID var){
	if (!_varlist[index]->set(var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not OBJID"));
	}
}

OBJID varlist::valueobjid(int index){
	OBJID ret = 0;

	if (!_varlist[index]->get(ret)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not OBJID"));
	}

	return ret;
}

//Float
void varlist::setfloat(int index, float var){
	if (!_varlist[index]->set((double)var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
	}
}

void varlist::setfloat(int index, double var){
	if (!_varlist[index]->set(var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
	}
}

Float varlist::valuefloat(int index){
	Float ret = 0;

	if (!_varlist[index]->get(ret)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not Float"));
	}

	return ret;
}

//String
void varlist::setstring(int index, String var){
	if (!_varlist[index]->set((std::string)var)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not String"));
	}
}

String varlist::valuestring(int index){
	String ret;

	if (!_varlist[index]->get(ret)){
		BOOST_THROW_EXCEPTION(std::logic_error("type is not String"));
	}

	return ret;
}

//Integer
varlist & varlist::operator << (int32_t var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_INT);
	_varlist.back()->set((Integer)var);

	return *this;
}

varlist & varlist::operator << (int64_t var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_INT);
	_varlist.back()->set(var);

	return *this;
}

//UInteger
varlist & varlist::operator << (uint32_t var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_UINT);
	_varlist.back()->set((UInteger)var);

	return *this;
}

varlist & varlist::operator << (uint64_t var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_UINT);
	_varlist.back()->set(var);

	return *this;
}

//Float
varlist & varlist::operator << (float var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_DOUBLE);
	_varlist.back()->set((double)var);

	return *this;
}

varlist & varlist::operator << (double var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_DOUBLE);
	_varlist.back()->set(var);

	return *this;
}

//String
varlist & varlist::operator << (String var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_STRING);
	_varlist.back()->set(var);
	
	return *this;
}

//Object
varlist & varlist::operator << (Object var){
	_varlist.push_back(boost::shared_ptr<base::value>(new base::value()));
	_varlist.back()->create(utopia::VALUE_TYPE_OBJECT);
	_varlist.back()->set(var);
	
	return *this;
}

//返回总长度,传入缓冲区长度大于参数列表总长度时，将数据写入缓冲区 （待修改，下版本，传递参数使用本地缓冲区）
size_t varlist::make_buff(char * buff, size_t llen){
	size_t _llen = 0;
	_llen += sizeof(uint32_t);//buff长度
	_llen += sizeof(uint32_t);//参数个数
	for(std::vector<boost::shared_ptr<base::value> >::iterator iter = _varlist.begin(); iter != _varlist.end(); iter++){
		_llen += sizeof(uint16_t);//类型
		_llen += sizeof(uint16_t);//长度
		switch(iter->get()->type()){
		case VALUE_TYPE_INT:
			_llen += sizeof(Integer);
			break;

		case VALUE_TYPE_UINT:
			_llen += sizeof(UInteger);
			break;

		case VALUE_TYPE_DOUBLE:
			_llen += sizeof(Float);
			break;

		case VALUE_TYPE_OBJID:
			_llen += sizeof(OBJID);
			break;

		case VALUE_TYPE_OBJECT:
			_llen += sizeof(OBJID);
			break;

		case VALUE_TYPE_STRING:
			String var;
			iter->get()->get(var);
			_llen += var.size();
			break;

		}
	}

	size_t index = 0;
	buff[index++] = _llen & 0x0f;
	buff[index++] = (_llen & 0xf0) >> 8;
	buff[index++] = (_llen & 0xf00) >> 16;
	buff[index++] = (_llen & 0xf000) >> 24;

	buff[index++] = _varlist.size() & 0x0f;
	buff[index++] = (_varlist.size() & 0xf0) >> 8;
	buff[index++] = (_varlist.size() & 0xf00) >> 16;
	buff[index++] = (_varlist.size() & 0xf000) >> 24;

	if (llen > _llen){
		for(std::vector<boost::shared_ptr<base::value> >::iterator iter = _varlist.begin(); iter != _varlist.end(); iter++){
			_llen += sizeof(uint16_t);
			switch(iter->get()->type()){
			case VALUE_TYPE_INT:
				{
					Integer var;
					iter->get()->get(var);
					buff[index++] = var & 0x0f;
					buff[index++] = (var & 0xf0) >> 8;
					buff[index++] = (var & 0xf00) >> 16;
					buff[index++] = (var & 0xf000) >> 24;
					buff[index++] = (var & 0xf0000) >> 32;
					buff[index++] = (var & 0xf00000) >> 40;
					buff[index++] = (var & 0xf000000) >> 48;
					buff[index++] = (var & 0xf0000000) >> 56;
				}
				break;

			case VALUE_TYPE_UINT:
				{
					UInteger var;
					iter->get()->get(var);
					buff[index++] = var & 0x0f;
					buff[index++] = (var & 0xf0) >> 8;
					buff[index++] = (var & 0xf00) >> 16;
					buff[index++] = (var & 0xf000) >> 24;
					buff[index++] = (var & 0xf0000) >> 32;
					buff[index++] = (var & 0xf00000) >> 40;
					buff[index++] = (var & 0xf000000) >> 48;
					buff[index++] = (var & 0xf0000000) >> 56;
				}
				break;

			case VALUE_TYPE_DOUBLE:
				{
					Float var;
					iter->get()->get(var);
					buff[index++] = *((Integer*)&var) & 0x0f;
					buff[index++] = (*((Integer*)&var) & 0xf0) >> 8;
					buff[index++] = (*((Integer*)&var) & 0xf00) >> 16;
					buff[index++] = (*((Integer*)&var) & 0xf000) >> 24;
					buff[index++] = (*((Integer*)&var) & 0xf0000) >> 32;
					buff[index++] = (*((Integer*)&var) & 0xf00000) >> 40;
					buff[index++] = (*((Integer*)&var) & 0xf000000) >> 48;
					buff[index++] = (*((Integer*)&var) & 0xf0000000) >> 56;
				}
				break;

			case VALUE_TYPE_OBJID:
				{
					OBJID var;
					iter->get()->get(var);
					buff[index++] = var & 0x0f;
					buff[index++] = (var & 0xf0) >> 8;
					buff[index++] = (var & 0xf00) >> 16;
					buff[index++] = (var & 0xf000) >> 24;
					buff[index++] = (var & 0xf0000) >> 32;
					buff[index++] = (var & 0xf00000) >> 40;
					buff[index++] = (var & 0xf000000) >> 48;
					buff[index++] = (var & 0xf0000000) >> 56;
				}
				break;

			case VALUE_TYPE_OBJECT:
				{
					base::IObj * var = 0;
					OBJID id = 0;
					iter->get()->get(var);
					if (var != 0){
						id = var->ID();
					}
					buff[index++] = id & 0x0f;
					buff[index++] = (id & 0xf0) >> 8;
					buff[index++] = (id & 0xf00) >> 16;
					buff[index++] = (id & 0xf000) >> 24;
					buff[index++] = (id & 0xf0000) >> 32;
					buff[index++] = (id & 0xf00000) >> 40;
					buff[index++] = (id & 0xf000000) >> 48;
					buff[index++] = (id & 0xf0000000) >> 56;
				}
				break;

			case VALUE_TYPE_STRING:
				{
					String var;
					iter->get()->get(var);
					memcpy(&buff[index], var.c_str(), var.size());
					index += var.size();
				}
				break;

			}
		}
	}

	return _llen;
}

}// utopia