#ifndef  	_PROTOCOL_CLIETN_H_
#define  	_PROTOCOL_CLIETN_H_
#include 	<iostream>

#include 	<string.h>

#include 	"protocol_base.h"
#include 	"msg_deal_queue.h"

using namespace std;
#define  USER_SIZE_LEN  			5


//---------------------------------
//---------------------------------
#pragma  pack(1)
typedef struct
{
	char  	userid[USER_SIZE_LEN];
	char		password[USER_SIZE_LEN];
}msg_regist;

typedef struct
{
	int     regist_result;// 0 ok -1Ê§°Ü
}msg_regist_ack;

typedef struct
{
	char  	userid[USER_SIZE_LEN];
	char	password[USER_SIZE_LEN];
}msg_login;

typedef struct
{
	char  	userid[USER_SIZE_LEN];
	char	password[USER_SIZE_LEN];
}msg_login_ack;




#pragma pack()
//extern Message_queue          Send_Queue;
//extern Message_queue          Recv_Queue;

class Protocol_Msg_client
{
	uint32			Message_id;
	char				Msg_buf[8*1024];
	int 				Msg_buf_len;
	//int			switch_cmd(uint32  cmd_id);
	int				Deal_data(msg_regist *pmsg);
	int				Deal_data(msg_login *pmsg);
	
public:
	Protocol_Msg_client()
	{
		memset(Msg_buf, 0, sizeof(Msg_buf));
		Msg_buf_len = 0;
	}

	int 		send_len;
	char		send_buf[2048];
	
	int         Protocol_Message_pack(uint32  msg_id,char *pmsg, int len, char *pout);
	
	int			Protocol_Message_Parse(struct bufferevent *bev, char *msg, int len );

};

#endif
