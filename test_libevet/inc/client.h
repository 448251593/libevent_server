#ifndef  _CLIENT_H_
#define  _CLIENT_H_

#include "protocol_dev.h"
#include "cJSON.h"


class device_client
{

	private:
		int 						switch_no;
		char 					switch_status[30];

	public:
	
		uint32                      data_type;
		struct bufferevent		*bev;//自身bev

		struct bufferevent		*bev_client;//控制端的bev socket

		uint32					last_time;//记录上一次的时间点
		cJSON* 					pRoot;	
		string					dev_id;//device id
	
		string 					ver_json ;   		
		string 					cmdtype;   		
		string 					dev_data; 
		
		char 					ctrl_key[31];
		uint32 					ctrl_ip;
		

		~device_client();
		device_client();
		device_client(int  switch_no_t);	
		string  get_json_info(void);
		uint32  get_timeout(void);
		int     set_switch_num(int	 num, char  *status);
		int     get_switch_num();
};
class Ctrl_client_Class
{

private:
	
public:

	struct bufferevent		*self_bev;//自身bev
	
	uint32					client_type;
	string                   username_ID;

	~Ctrl_client_Class();
	Ctrl_client_Class();
	
};

#endif





