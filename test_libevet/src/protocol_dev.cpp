#include <iostream>
#include <map>
#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "protocol_dev.h"
#include "client.h"
#include "mysql_class.h"
#include "dealdata.h"
#include "msg_deal_queue.h"

#include "socket_event.h"
#include "http_server.h"
using namespace  std;
using namespace boost::property_tree; 
//map<string, device_cl> m_device;

int   Protocol_Msg_dev::Protocol_Message_Parse(struct bufferevent *bev, char *msg, int len )
{
	int   rslt = -1;
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

    cout<<"len="<<ptemp->len<< " msg_id="<<ptemp->msg_id<<endl;
    if (ptemp->len < Msg_buf_len )
    {
        return -1;
    }
	cout<<"--->0x"<<hex<<ptemp->msg_id<<endl;	
    switch(ptemp->msg_id)
    {
    case DEVICE_LOGIN_REPORT:
		{
			msg_dev_login *pmsg;
			pmsg = (msg_dev_login *)(msg + sizeof(msg_head));
			rslt = Deal_data(bev,pmsg);			
    	}
		break;
	case DEVICE_STATUS_REPORT:
		{
			msg_dev_status_report *pmsg;
			pmsg = (msg_dev_status_report *)(msg + sizeof(msg_head));
			rslt = Deal_data(bev, pmsg);
		}
		break;	
	case CLIENT_SERVER_MSG:
		{
			Client_Sever_Cmd_Ctrl *pmsg;
			pmsg = (Client_Sever_Cmd_Ctrl *)(msg + sizeof(msg_head));
			Deal_data(bev, pmsg);
		}
		break;
    default:
		return -1;
        break;
    }
	
	memmove(Msg_buf, Msg_buf+ptemp->len, Msg_buf_len - ptemp->len);
	Msg_buf_len = Msg_buf_len - ptemp->len;
//	if(Msg_buf_len > 0)
//	{
//		goto MSG_PARSE;
//	}
	return rslt ;
}


int  Protocol_Msg_dev::Deal_data(struct bufferevent	*bev,msg_dev_login *pmsg)
{
	string   id_str;
	did_str = pmsg->did_str;
	id_str = pmsg->did_str;
	
	cout<<"string id:"<<id_str<<endl;
	//-------compare id
	//if(id_str == "abcd" || id_str == "bcg123")
	if(1)
	{
			cout<<"id is ok\n";			
		
			//-----------------------------------------------------------------------
			{
				std::unique_lock<std::mutex>   lck(Map_Dev_online.lock);
				
				Map_Dev_online_iter m1_iter;
				
				m1_iter = Map_Dev_online.Map.find(id_str);
				
				if(m1_iter != Map_Dev_online.Map.end())	
				{		
//					device_client *tmp_client;
					boost::shared_ptr<device_client> tmp_client;
					tmp_client = m1_iter->second;
					
					cout<<"relogin\n";
						
					//------------??bev ?????map ??? ????
					Map_Parse_Dev_iter  m1_iter;
					m1_iter = Map_Data_Dev.Map.find(tmp_client->bev);
					if(m1_iter != Map_Data_Dev.Map.end())	//??
					{
						Map_Data_Dev.Map.erase(m1_iter);						
					}
					//------------break connect----------------
					bufferevent_free(tmp_client->bev);

					//online map ?dev_id key????.

					tmp_client->bev = bev;			
					tmp_client->last_time = time(0);
					tmp_client->dev_id = id_str;				
					tmp_client->ctrl_ip = 0;		
					Map_Dev_online.Map.insert(
						pair<string ,boost::shared_ptr<device_client>>
						(did_str, tmp_client));
										
				}	
				else
				{
						cout<<"new device "<<id_str <<" login ok\n";
						//??????map
						boost::shared_ptr<device_client> tmp_client(new device_client());
//						device_client  *tmp_client = new device_client();
						if (tmp_client)
						{
							tmp_client->dev_id = id_str;
		//					tmp_client->token = msg.token;
							tmp_client->bev = bev;			
							tmp_client->last_time = time(0);
							Map_Dev_online.Map.insert(
								pair<string ,boost::shared_ptr<device_client>>
								(id_str, tmp_client));
														
							//return  login infomation
							bev_data_struct		data;
							Protocol_Msg_dev	parse_pack_msg;					
							msg_dev_login_ack   msg;	

							msg.err_code = 0;
							msg.report_period = 3;
							strcpy(msg.token, "123456789");

							data.bev = bev;
							data.data_len = Protocol_Message_pack(DEVICE_LOGIN_RESPONSE, (char*)&msg, sizeof(msg_dev_login_ack), data.data);

							std::unique_lock<std::mutex>  lck(Device_Send_queue.lock);
							Device_Send_queue.data_queue.push(data);							
							
						}
				}
				return  0;
			}
		}
		else
		{
			cout<<"is failed\n";//
			return -2;
		}
	
}

int  Protocol_Msg_dev::Deal_data(struct bufferevent	*bev, msg_dev_status_report *pmsg)
{
	did_str = pmsg->did_str;
	Map_Dev_online_iter  m1_iter;

		//std::unique_lock<std::mutex> lck (Map_Dev_online.lock);
		m1_iter = Map_Dev_online.Map.find(did_str);

		if(m1_iter != Map_Dev_online.Map.end())	//找到
		{		
			//cout<< "old device\n";			
			boost::shared_ptr<device_client >tmp_client;
			tmp_client = m1_iter->second;
			tmp_client->bev = bev;
			//tmp_client->data_type = pmsg->data_type;
			tmp_client->set_switch_num(pmsg->switch_no, pmsg->switch_status);			
			tmp_client->last_time = time(0);
			tmp_client->ctrl_ip = pmsg->connect_ip ;

				
			Map_Dev_online.Map.at(did_str) = tmp_client;		
		}
		else
		{
			//no find
			cout<<"new device\n";
			cout<<"pmsg->did_str="<<did_str.c_str()<<endl;
			cout<<"pmsg->switch_no="<<pmsg->switch_no<<endl;
			//查询数据库
			char  ctrl_key_tmp[31]={0};
			//改为 http方式获取
			/*
			mysql_class  sqlobj;
			sqlobj.init_mysql();
			int rslt = sqlobj.get_register_devid((const char*)did_str.c_str(),
				ctrl_key_tmp);
			if(rslt == 0)
			{
				cout<<"get key ok\n";
			}
			else
			{
				cout<<"get key failed\n";
				return -2;
			}*/

			device_client  *tmp_client = new device_client();
			if ( tmp_client )
			{
				tmp_client->dev_id = did_str;
				tmp_client->bev = bev;
				tmp_client->ctrl_ip = pmsg->connect_ip ;			
				tmp_client->set_switch_num(pmsg->switch_no, pmsg->switch_status);			
				tmp_client->last_time = time(0);
				strcpy(tmp_client->ctrl_key,ctrl_key_tmp);
				Map_Dev_online.Map.insert(pair<string ,device_client*>(did_str, tmp_client));
			}
			//??????????
			return -2;//????
		}
	
}

int  Protocol_Msg_dev::Deal_data(struct bufferevent	*bev, Client_Sever_Cmd_Ctrl *pmsg)
{
	string did_str = pmsg->device_id;
	Map_Dev_online_iter  m1_iter;
	

	std::unique_lock<std::mutex>  lck(Map_Dev_online.lock);
		m1_iter = Map_Dev_online.Map.find(did_str);

	if(m1_iter != Map_Dev_online.Map.end())	//找到
	{		
		cout<< "phone msg control device\n";		
		boost::shared_ptr<device_client> tmp_client;
		tmp_client = m1_iter->second;
		//send_data_to_dev(tmp_client->bev, pmsg->switch_no, pmsg->ctrl_on_off);
	}
	else
	{
		cout<<"client msg not find device ->"<<did_str<<endl;
	}	
	return 0;
}

int Protocol_Msg_dev::Protocol_Message_pack(uint32  msg_id, char *pmsg, int len)
{
	msg_head  msg_h;
	
	int head_len;
	char  *pout;
	pout = send_buf;
	
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
int Protocol_Msg_dev::Protocol_Message_pack(uint32  msg_id, char *pmsg, int len, char *pout)
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

int   Protocol_Msg_dev::Protocol_Message_Parse_json(struct bufferevent *bev, char *msg, int len )
{
	int   rslt = -1;
    msg_head   *ptemp;
	
	std::stringstream ss(msg);  
	ptree pt;
	if(len >= 1000)
	{
		cout<<"parse json err"<<len<<endl;
		return -1;
	}
	
	 try
	 {      
		read_json(ss, pt);  
		
	  }  
	  catch(ptree_error & e) {  
		cout<<"rd jsn err\n";
		return -1;   
	  }
	  
	try{  
		string ver = pt.get<string>("ver");
		this->did_str = pt.get<string>("dev_id"); 
		string cmdtype = pt.get<string>("cmdtype");   		
		 		
		string dev_data = pt.get<string>("data");  
		
	
		Map_Dev_online_iter  m1_iter;

		std::unique_lock<std::mutex> lck (Map_Dev_online.lock);
		m1_iter = Map_Dev_online.Map.find(this->did_str);
		if(m1_iter != Map_Dev_online.Map.end())	//找到
		{		
			//cout<< "old device\n";			
			boost::shared_ptr<device_client >tmp_client;
			//取出设备
			tmp_client = m1_iter->second;
			//更新
			tmp_client->bev = bev;
		
			tmp_client->last_time = time(0);
			tmp_client->ver_json = ver;
			tmp_client->cmdtype = cmdtype;
			tmp_client->dev_data = dev_data;

			//更新map
			Map_Dev_online.Map.at(this->did_str) = tmp_client;		
		}
		else
		{
			//no find
			cout<<"new device\n";
			cout<<"did_str="<<this->did_str.c_str()<<endl;
			
			//查询数据库
			char  ctrl_key_tmp[31]={0};
			//改为 http方式获取 检查数据库
			//todo
			//
			device_client  *tmp_client = new device_client();
			if ( tmp_client )
			{
				
				tmp_client->bev = bev;							
				tmp_client->last_time = time(0);
				tmp_client->dev_id = this->did_str;
				tmp_client->ver_json = ver;
				tmp_client->cmdtype = cmdtype;
				tmp_client->dev_data = dev_data;

				Map_Dev_online.Map.insert(pair<string ,device_client*>(this->did_str, tmp_client));
			}			
		}
		
		
	//    ptree image_array = pt.get_child("images");  /*get_child得到数组对象   */
		  
		/*遍历数组  
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)  
		{  
		  std::stringstream s;  
		  write_json(s, v.second);  
		  std::string image_item = s.str();  
		}   */
	  }  
	  catch (ptree_error & e)  
	  {  
		cout<<"parse json err"<<endl;
		return 2;  
	  } 

	
	return 1;
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





