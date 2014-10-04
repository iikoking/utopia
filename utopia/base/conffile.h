/*
 * conffile.h
 *  Created on: 2013-4-30
 *      Author: qianqians
 * conffile
 */
#ifndef _CONFFILE_H
#define _CONFFILE_H

#include "u_stdint.h"
#include <fstream>
#include <vector>
#include <map>

#include <boost/pool/pool_alloc.hpp>

namespace utopia{
namespace base{

class conffile{
private:
	typedef std::map<std::string, std::string, std::less<std::string>, boost::pool_allocator<std::pair<std::string, std::string> > > assembly;
	typedef std::map<std::string, assembly, std::less<std::string>, boost::pool_allocator<std::pair<std::string, assembly> > > assembly_map;
	typedef std::vector<std::string> vector_string;

public:
	conffile();
	~conffile();

	bool loadfile(std::string strPath);

	bool findvalue(std::string key1, std::string key2);

	Integer getvalueInteger(std::string key1, std::string key2, Integer def);

	String getvalueString(std::string key1, std::string key2, String def);

	Float getvalueFloat(std::string key1, std::string key2, Float def);

private:
	void analyze(std::string strline);

private:
	std::ifstream fini;
	assembly_map map_value;
	std::string strassemblykey;
	vector_string vector_annotate;
};

}// base
}// utopia

#endif //_CONFFILE_H