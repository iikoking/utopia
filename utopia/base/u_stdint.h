/*
 * stdint.h
 *  Created on: 2013-4-11
 *      Author: qianqians
 * intdef
 */
#ifndef _U_STDINT_H
#define _U_STDINT_H

#include <string>

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif 

#if (_MSC_VER >= 1600) || (GCC_VERSION > 40602)
#include <stdint.h>
#else
#include <boost/cstdint.hpp>

namespace utopia {

using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;

using boost::int_least8_t;
using boost::int_least16_t;
using boost::int_least32_t;
using boost::int_least64_t;
using boost::uint_least8_t;
using boost::uint_least16_t;
using boost::uint_least32_t;
using boost::uint_least64_t;

using boost::int_fast8_t;
using boost::int_fast16_t;
using boost::int_fast32_t;
using boost::int_fast64_t;
using boost::uint_fast8_t;
using boost::uint_fast16_t;
using boost::uint_fast32_t;
using boost::uint_fast64_t;

}// utopia

#endif 

namespace utopia{
namespace base{	
class IObj;
}// base


typedef int64_t Integer;
typedef uint64_t UInteger;
typedef UInteger OBJID;
typedef double Float;
typedef std::string String;
typedef base::IObj* Object;  

}// utopia

#endif //_U_STDINT_H