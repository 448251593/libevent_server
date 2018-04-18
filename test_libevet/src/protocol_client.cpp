#if 0
#include <iostream>
#include <map>
#include <string.h>

#include "Protocol_client.h"
#include "client.h"
#include "mysql.h"
#include "dealdata.h"
#include "msg_deal_queue.h"

using namespace  std;
//map<string, device_cl> m_device;



int   Protocol_Msg_client::Protocol_Message_Parse(struct bufferevent *bev, char *msg, int len )
{	
	msg_head   *ptemp;

	if(Msg_buf_len + len < sizeof(Msg_buf))
	{

		memcpy(Msg_buf + Msg_buf_len, msg, len);
		Msg_buf_len += len;
	}
	else
	{

		memcpy(Msg_buf + Msg_buf_len, msg, sizeof(Msg_buf) - Msg_buf_len);
		Msg_buf_len += sizeof(Msg_buf) - Msg_buf_len;
	}

MSG_PARSE:
	if(Msg_buf_len < sizeof(msg_head))
	{
		return -1;
	}

	ptemp = (msg_head*)Msg_buf;
	// Debug_printf((char*)"parse", msg, len);

	//    cout<<"len="<<ptemp->len<< " msg_id="<<ptemp->msg_id<<endl;
	if (ptemp->len < Msg_buf_len )
	{
		return -1;
	}
	//cout<<"--->0x"<<hex<<ptemp->msg_id<<endl;	
	switch(ptemp->msg_id)
	{
	case DEVICE_LOGIN_REPORT:
		{
			msg_dev_login *pmsg;
			pmsg = (msg_dev_login *)(msg + sizeof(msg_head));
			Deal_data(pmsg);			
		}
		break;
	case DEVICE_STATUS_REPORT:
		{
			msg_dev_status_report *pmsg;
			pmsg = (msg_dev_status_report *)(msg + sizeof(msg_head));
			Deal_data(bev, pmsg);
		}
		break;
	default:
		return -1;
		break;
	}

	memmove(Msg_buf, Msg_buf+ptemp->len, Msg_buf_len - ptemp->len);
	Msg_buf_len = Msg_buf_len - ptemp->len;
	if(Msg_buf_len > 0)
	{
		goto MSG_PARSE;
	}	
}


int  Protocol_Msg_client::Deal_data(msg_cmd_ctrl *pmsg)
{
//	string   id_str;
//	id_str = pmsg->did_str;
//	map <string , device_client>::iterator m1_iter;
//	
//	m1_iter = Map_Dev_online.find(id_str);
//	
//	if(m1_iter != Map_Dev_online.end())	
//	{		
//		cout<< "old device"<<endl;	
//		device_client  tmp_client;
//		tmp_client = m1_iter->second;
//		tmp_client.switch_no = pmsg->switch_no;
//		memcpy(tmp_client.switch_status, pmsg->switch_status, pmsg->switch_no);
//		
//		Map_Dev_online.at(id_str) = tmp_client;
//	}	
}

int Protocol_Msg_client::Protocol_Message_pack(uint32  msg_id,char *pmsg, int len, char *pout)
{
	msg_head  msg_h;

	int head_len;
	//char  *pout;
	//pout = send_buf;

	head_len = sizeof(msg_head);

	if ( head_len + len <= sizeof(send_buf) )
	{
		msg_h.len = len + head_len;
		msg_h.msg_id = msg_id;

		memset(pout, 0, sizeof(send_buf));		
		memcpy(pout, (char*)&msg_h, sizeof(msg_head));
		memcpy(pout + sizeof(msg_head), pmsg, len);
		send_len = sizeof(msg_head) + len;

		return send_len;
	}
	send_len = 0;
	return 0;
}

//Protocol_Message::Protocol_Message_GetData(char ** buf, int *len)
//{

//}
//Protocol_Message::Protocol_Message_pack(msg_regist  *pmsg_login)
//{

//}
//Protocol_Message::Protocol_Message_pack(msg_regist  *pmsg_cmd)
//{

//}
//Protocol_Message::Protocol_Message_pack(msg_regist  *pmsg_msgbody)
//{

//}

//int   package_msg(uint32  msg_id, char *pmsg, int len, char *pout, int outlen)
//{
//	msg_head  msg_h;

//    int head_len;
//    head_len = sizeof(msg_head);


//	
//    if ( head_len + len <= outlen )
//    {
//		msg_h.len = len + head_len;
//		msg_h.msg_id = msg_id;
//		
//        memset(pout, 0, outlen);
//		
//        memcpy(pout, (char*)&msg_h, sizeof(msg_head));
//        memcpy(pout + sizeof(msg_head), pmsg, len);
//		
//        return sizeof(msg_head) + len;
//    }
//    return -1;
//}




#endif
