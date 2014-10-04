/*
 * conffile.h
 *  Created on: 2013-4-30
 *      Author: qianqians
 * conffile
 */
#include "conffile.h"
#include "strtools.h"

namespace utopia{
namespace base{

conffile::conffile(){
	vector_annotate.push_back("#");
	vector_annotate.push_back(";");
	vector_annotate.push_back("//");

	strassemblykey = "default";
	map_value.insert(std::make_pair(strassemblykey, assembly()));
}
	
conffile::~conffile(){
}

bool conffile::loadfile(std::string strPath){
	fini.open(strPath);
	if (!fini.is_open()){
		return false;
	}

	std::string strline;
	while(!getline(fini, strline).eof()){
		analyze(strline);
	}

	fini.close();
	strassemblykey = "default";

	return true;
}

void conffile::analyze(std::string strline){
	if(strline.front() == '['){
		int pos = strline.find(']');
		strassemblykey = strline.substr(1, pos);
	}

	for(vector_string::iterator iter = vector_annotate.begin(); iter != vector_annotate.end(); iter++){
		int pos = strline.find(*iter);
		if (pos != std::string::npos){
			return;
		}
	}

	std::vector<std::string> vectorStr;
	strtools::spitle('=', strline, vectorStr, 1);

	for(vector_string::iterator iter = vector_annotate.begin(); iter != vector_annotate.end(); iter++){
		int pos = vectorStr[1].find(*iter);
		if (pos != std::string::npos){
			vectorStr[1] = vectorStr[1].substr(0, pos);
			break;
		}
	}
	
	assembly_map::iterator iterfind = map_value.find(strassemblykey);
	if (iterfind == map_value.end()){
		map_value.insert(std::make_pair(strassemblykey, assembly()));
		iterfind = map_value.find(strassemblykey);
	}
	iterfind->second.insert(std::make_pair(vectorStr[0], vectorStr[1]));
}

bool conffile::findvalue(std::string key1, std::string key2){
	assembly_map::iterator iterfind = map_value.find(key1);
	if (iterfind == map_value.end()){
		return false;
	}

	assembly::iterator iterfind1 = iterfind->second.find(key2);
	if (iterfind1 == iterfind->second.end()){
		return false;
	}

	return true;
}

Integer conffile::getvalueInteger(std::string key1, std::string key2, Integer def){
	assembly_map::iterator iterfind = map_value.find(key1);
	if (iterfind == map_value.end()){
		return def;
	}

	assembly::iterator iterfind1 = iterfind->second.find(key2);
	if (iterfind1 == iterfind->second.end()){
		return def;
	}

	return strtools::str2int(iterfind1->second);
}

String conffile::getvalueString(std::string key1, std::string key2, String def){
	assembly_map::iterator iterfind = map_value.find(key1);
	if (iterfind == map_value.end()){
		return def;
	}

	assembly::iterator iterfind1 = iterfind->second.find(key2);
	if (iterfind1 == iterfind->second.end()){
		return def;
	}

	return iterfind1->second;
}

Float conffile::getvalueFloat(std::string key1, std::string key2, Float def){
	assembly_map::iterator iterfind = map_value.find(key1);
	if (iterfind == map_value.end()){
		return def;
	}

	assembly::iterator iterfind1 = iterfind->second.find(key2);
	if (iterfind1 == iterfind->second.end()){
		return def;
	}

	return strtools::str2float(iterfind1->second);
}

}// base
}// utopia
