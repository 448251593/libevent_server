/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#endif


#include <bufferevent.h>
#include <buffer.h>
#include <listener.h>
#include <util.h>
#include <event.h>
#include <map>
#include	<thread>
#include	<cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include "http.h"
#include "keyvalq_struct.h"
#include "util.h"
#include "http_server.h"
#include <execinfo.h>
#include <iostream>
#include "socket_event.h"
#include<vector>

#include "boost/smart_ptr.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <boost/exception/all.hpp> 
using namespace std;
using namespace boost::property_tree; 

typedef unsigned short int uint16 ;
typedef unsigned char uint8 ;


void    Debug_printf(char *funNameStr, const char *buf, uint16 buflen);
int  parse_json(string msg);
int  outputjson();
typedef struct{
	string ip;
	string maxnums;
	string running;
}server_list_info;

class Map_server
{

public:
	map <string , boost::shared_ptr<server_list_info>>				Map;
	mutex											lock;
};
typedef map <string , boost::shared_ptr<server_list_info>>::iterator  Map_server_iter;
Map_server         server_map_info;
//-----------------------------------------------------------------------
void 		print_trace (int  args);
int   file_timestamp = 0;
char       confbuf[1024*5];
int main(int argc, char **argv)
{

	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif


	//--------------------------------http---------------------------------------------
	http_server_init(7001);	
	//---------------------------------------------------------------------------------
	struct stat buf;
	while(1)
	{		
		int rslt = stat("./serverlist.conf", &buf);
		if(rslt == 0 && buf.st_ctime != file_timestamp)
		{
			file_timestamp = buf.st_ctime;
			//cout<<"st_ctime"<<buf.st_ctime<<endl;
			
			FILE  *fp;
            fp = fopen("./serverlist.conf","r");
            if (fp != NULL)
            {
				memset(confbuf,0,sizeof(confbuf));
				int  len= fread(confbuf, 1, sizeof(confbuf),fp);
                fclose(fp);
				string tmsg = confbuf;
				parse_json(tmsg);
				outputjson();
            }
          
		}
		sleep(2);
	}
	return 0;
}
int  parse_json(string msg)
{
	//stringstream stream;
	string test = "{\"msg\":\"123\"}";
	std::stringstream ss(test);  
	cout<<ss<<endl;
	cout<<msg<<endl;
	ptree pt;
	ptree pt1;
	string  msg_str = msg;
	//stream << msg_str;

	 try
	 {      
		read_json<ptree>(ss, pt);  
	 }  
	 catch(ptree_error & e) 
	 {  
		cout<<"rd jsn err:"<<msg<<"\n";
		cout<<"err;"<<e.what()<<endl;
		return -1;   
	 }
	  return 0;
	try{  
		//this->did_str = pt.get<string>("dev_id");  
		//string dev_cmd = pt.get<string>("cmd");   		
		//string dev_type = pt.get<string>("type");   		
		//string dev_stat = pt.get<string>("stat");  
		
		pt1 =  pt.get_child("serverlist");
		/*遍历数组  */
		 for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it)
		{
			string tmp_ip;
			tmp_ip = it->second.get<string>("ip");
			
			Map_server_iter m1_iter1 = server_map_info.Map.find(tmp_ip);
			if(m1_iter1 == server_map_info.Map.end())	
			{		
				cout<<"delete dev_client\n";
				//server_map_info.Map.erase(m1_iter1);
				server_list_info  *tmp = new server_list_info();
				tmp->ip      = it->second.get<string>("ip");
				tmp->maxnums = it->second.get<string>("maxnums");
				tmp->running = it->second.get<string>("running");
			
				server_map_info.Map.insert(
				pair<string, server_list_info*> 
				(tmp->ip, tmp));
			}
			else
			{
				boost::shared_ptr<server_list_info>tmp;
				//取出
				tmp = m1_iter1->second;
				//更新
				tmp->ip      = it->second.get<string>("ip");
				tmp->maxnums = it->second.get<string>("maxnums");
				tmp->running = it->second.get<string>("running");
				server_map_info.Map.at(tmp->ip) = tmp;
			}
		}
		
	  }  
	  catch (ptree_error & e)  
	  {  
		cout<<"parse json err"<<endl;
		return 2;  
	  } 

}
int  outputjson()
{
	
	Map_server_iter it;
    for(it = server_map_info.Map.begin(); it != server_map_info.Map.end();++it)
	{
		cout<<it->second->ip<<endl;
		cout<<it->second->maxnums<<endl;
		cout<<it->second->running<<endl;
		cout<<"---------------------------\n";
		
	}
       
}
/*******************************************************************************
 * Description : Debug_printf
 * Arguments   :
 * Returns     :
 * Caller      : APP layer.
 * Notes       : none.
 *******************************************************************************/
void    Debug_printf(char *funNameStr, const char *buf, uint16 buflen)
{
    int i;
    printf("%s->len=%d  ", funNameStr, buflen);
    for( i = 0; i < buflen ; i++ )
    {
    	printf("%-2x ", (uint8)(buf[i]));//加上强制转换很有必要
    }

    printf("\n");//加上强制转换很有必要

}


