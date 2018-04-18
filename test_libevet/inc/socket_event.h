#ifndef _SOCKET_EVENT_H_
#define _SOCKET_EVENT_H_
#include <map>
#include <mutex>
#include "boost/smart_ptr.hpp"

using namespace std;

class Map_Data_Dev_Parse
{

public:
	mutex                                        lock;

	map <struct bufferevent*, boost::shared_ptr<Protocol_Msg_dev>>	Map;//句柄和解析map

};

class Map_Dev_Online_class
{
	
public:
//	map <string , device_client*>					Map;
	map <string , boost::shared_ptr<device_client>>					Map;
	
	mutex										lock;
	
};

class Map_Client_Online_class
{

public:
	map <string , device_client*>					Map;
	mutex											lock;

};
extern Map_Data_Dev_Parse									Map_Data_Dev;//句柄和解析map		
extern Map_Dev_Online_class									Map_Dev_online;//设备在线map
extern Map_Client_Online_class								Map_Client_online;

typedef map <struct bufferevent*, boost::shared_ptr<Protocol_Msg_dev>>::iterator   Map_Parse_Dev_iter;

typedef map <string , boost::shared_ptr<device_client>>::iterator				  Map_Dev_online_iter;
//typedef map <struct bufferevent*, Protocol_Msg_dev*>::iterator   Map_Parse_Dev_iter;




extern Message_queue_bev												Device_Send_queue;//设备数据发送队列
extern void  send_back_thread(int arg);
#endif
