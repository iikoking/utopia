/*
 * OutMsg.h
 *  Created on: 2013-4-29
 *      Author: qianqians
 * OutMsg
 */
#ifndef _OUTMSG_H
#define _OUTMSG_H

#include "Msg.h"

#include "../base/varlist.h"
#include "../base/u_stdint.h"

namespace utopia{

struct OutMsg : public MsgHead{
	uint32_t connid;//接受玩家连接id,广播包此参数无效 
	uint32_t llen;
	char msg[1];
};

}// utopia

#endif //_OUTMSG_H