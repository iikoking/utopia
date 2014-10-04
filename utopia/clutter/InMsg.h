/*
 * InMsg.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * InMsg
 */
#ifndef _INMSG_H
#define _INMSG_H

#include "Msg.h"

#include "../base/varlist.h"
#include "../base/u_stdint.h"

namespace utopia{

struct InMsg : public MsgHead{
	uint32_t llen;
	char msg[1];
};

struct EventMsg : public MsgHead{
	OBJID userid;
	uint32_t llen;
	char msg[1];
};

}// utopia

#endif //_INMSG_H