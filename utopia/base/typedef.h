/*
 * typedef.h
 *  Created on: 2013-4-3
 *      Author: qianqians
 * typedef
 */
#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#define __IS__AMD64__ (sizeof(void*)==8)

namespace utopia{

enum type_def{
	VALUE_TYPE_INT = 1,
	VALUE_TYPE_UINT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_STRING,
	VALUE_TYPE_OBJECT,
	VALUE_TYPE_OBJID,
};

} //utopia

#endif //_TYPEDEF_H