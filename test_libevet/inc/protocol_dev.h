#ifndef  	_PROTOCOL_DEV_H_
#define  	_PROTOCOL_DEV_H_
#include 	<iostream>

#include 	<string.h>
#include 	"msg_deal_queue.h"
#include 	"protocol_base.h"


using namespace std;

#define    READ_4BYTE(p)  *(p)+(*(p+1)<<8)+(*(p + 2)<<16)+(*(p + 3) << 24)
#define    READ_2BYTE(p)  *(p)+(*(p+1)<<8)

#define  MAX_SIZE_ID   			20+1
enum
{
	DATA_TYPE = 0,//
	SWITCH_TYPE,//
	
};

#pragma pack(1)
//---------------------------------
typedef struct
{
	char  	did_str[MAX_SIZE_ID];
}msg_dev_login;

typedef struct
{
	int     err_code;// -1 错误 id不存在, 0 ok.
	int     report_period;//
	char    token[64];//返回的token
}msg_dev_login_ack;

typedef struct
{
	char   	 did_str[MAX_SIZE_ID];
//	uint32   data_type;//data type
	int      switch_no;//num
	char     switch_status[30];//value
	uint32   connect_ip;//connect ip
}msg_dev_status_report;


typedef struct
{
	int     switch_no;
	char    set_status;//0 off 1 on
}msg_dev_status_ctrl;

typedef struct
{
	char      username_id[MAX_SIZE_ID];
	char      device_id[MAX_SIZE_ID];
	int       switch_no;
	char      ctrl_on_off;
}Client_Sever_Cmd_Ctrl;


#pragma pack()
//---------------------------------

class Protocol_Msg_dev
{		
	
	uint32      			Message_id;
	
	int 					Msg_buf_len;
	char					Msg_buf[2*1024];
	int					Deal_data(struct bufferevent	*bev,msg_dev_login *pmsg);
	int					Deal_data(struct bufferevent	*bev,msg_dev_status_report *pmsg);
	int					Deal_data(struct bufferevent	*bev,Client_Sever_Cmd_Ctrl *pmsg);
//	int					Deal_data(struct bufferevent	*bev,	Client_Sever_Cmd_Ctrl *pmsg);
	
public:

	struct bufferevent	*bev;//发送socket指针
	string				did_str;
 

	Protocol_Msg_dev()
	{
		did_str = "";
//		dev_cmd = "";
//		dev_type = "";
//		dev_stat = "";
		memset(Msg_buf, 0, sizeof(Msg_buf));
		Msg_buf_len = 0;
	}
	
	int 		send_len;
	char		send_buf[1024];
	
	int         Protocol_Message_pack(uint32  msg_id, char *pmsg, int len);
	int			Protocol_Message_pack(uint32  msg_id, char *pmsg, int len, char *pout);
//	int         Protocol_Message_pack(msg_regist  *pmsg_regist_ack);
//	int         Protocol_Message_pack(msg_regist  *pmsg_login);
//	int         Protocol_Message_pack(msg_regist  *pmsg_cmd);
//	int         Protocol_Message_pack(msg_regist  *pmsg_msgbody);
	int  		Protocol_Message_Parse(struct bufferevent *bev, char *msg, int len);	
	int  		Protocol_Message_Parse_json(struct bufferevent *bev, char *msg, int len);	
//	virtual     int  
};

#endif
