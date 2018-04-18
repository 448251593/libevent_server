#ifndef  	_PROTOCOL_BASE_H_
#define  	_PROTOCOL_BASE_H_
#include 	<iostream>
#include 	"msg_deal_queue.h"
#include 	<string.h>

using namespace std;

typedef char  				int8;
typedef short 				int16;
typedef int					int32;

typedef unsigned char 		uint8;
typedef unsigned short 		uint16;

typedef unsigned int  		uint32;
typedef unsigned long long 	uint64;

#define   DEVICE_DEF   			0x01
#define   SERVER_DEF   			0x02
#define   CLIENT_DEF   			0x03

#define   MSG_ID(P,P1,ID)   	((P<<16)+(P1<<8)+ID)

enum
{
	DEVICE_LOGIN_REPORT = MSG_ID(DEVICE_DEF,SERVER_DEF, 0x01),
	DEVICE_LOGIN_RESPONSE = MSG_ID(SERVER_DEF,DEVICE_DEF, 0x01),

	DEVICE_STATUS_REPORT = MSG_ID(DEVICE_DEF,SERVER_DEF, 0x02),
	DEVICE_STATUS_RESPONSE = MSG_ID(SERVER_DEF,DEVICE_DEF, 0x02),

	CLIENT_SERVER_MSG=MSG_ID(CLIENT_DEF, SERVER_DEF, 0x01),
	SERVER_CLIENT_MSG=MSG_ID(SERVER_DEF, CLIENT_DEF, 0x01),
	
};

typedef struct
{
	uint32  len;
	uint32  msg_id;
	
}msg_head;


#endif
