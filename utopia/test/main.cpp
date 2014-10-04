#include "../base/attribute.h"
#include "../base/varlist.h"
#include "../base/table.h"
#include <iostream>
#include <boost/thread.hpp>
#include <Windows.h>

int main(){
	int a= 9;

	//utopia::base::attribute var("int", utopia::VALUE_TYPE_INT);
	//var.setattribute(a);

	//int64_t v;
	//var.getattribute(v);
	//printf("%d", v);

	utopia::varlist vlist;
	vlist << "var"<< 0.1 << 1 << "type" << 1;
	
	std::cout << vlist.valuestring(0) << vlist.valueint(2) << vlist.valuestring(3) << vlist.valueint(4);

	for(;;){
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}

	return 1;
}