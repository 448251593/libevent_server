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
#include "protocol_dev.h"
#include "client.h"
#include "http.h"
#include "keyvalq_struct.h"
#include "util.h"
#include "http_server.h"
#include <execinfo.h>

#include "socket_event.h"
#include "threadpool.h"

#include "boost/smart_ptr.hpp"

using namespace std;


void    Debug_printf(char *funNameStr, const char *buf, uint16 buflen);

//static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 7000;


static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *bev, void *ctx);

static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);
//-----------------------------------------------------------------------

void 		print_trace (int  args);



static 		pthread_t    thread_id1;
static 		pthread_t    thread_id2;

//static void* Msg_Deal_thread(void *args);
void 		*Msg_Deal_thread(void *arg);
void  		*send_back_thread(void *arg);

struct 		event_base *base;
struct 		evconnlistener *listener;
struct 		event *signal_event;


Map_Data_Dev_Parse										Map_Data_Dev;//句柄和解析map		
Map_Dev_Online_class									Map_Dev_online;

Map_Client_Online_class									Map_Client_online;//控制端在线map

Message_queue_bev											Device_Recv_queue;//设备数据接收队列
Message_queue_bev											Device_Send_queue;//设备数据发送队列

int main(int argc, char **argv)
{

	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	//线程池初始化	
	//pool_init(5);

	sigset(SIGSEGV,print_trace);
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	if (!listener) {
		fprintf(stderr, "Could not create a listener!\n");
		return 1;
	}

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}
	cout<<"TCP listen port"<<PORT<<endl;
	//--------------------------------http---------------------------------------------
	http_server_init(1212);	
	//---------------------------------------------------------------------------------

	
	int iRet;
	iRet = pthread_create(&thread_id1, NULL, &Msg_Deal_thread, NULL);
	if (iRet == -1)
	{
		printf("can't creat timer_1s thread\n");
	    return 1;
	}
	iRet = pthread_create(&thread_id2, NULL, &send_back_thread, NULL);
	if (iRet == -1)
	{
		printf("can't creat timer_1s thread\n");
		return 1;
	}

	


	event_base_dispatch(base);
	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}

void *Msg_Deal_thread(void *arg)
{
	int					len, i;
	char				tmp_buf[CMD_BUF_SIZE];
	struct bufferevent 	*bev;
	Message_queue			*msg_q_tmp;
//	Protocol_Msg_dev		*parse_dev_msg;

	boost::shared_ptr<Protocol_Msg_dev>  parse_dev_msg;
	bev_data_struct       				 tmp_bev_data;

	while(1)
	{
		{
			//阻塞的方式获取队列数据
		   if(Device_Recv_queue.Data_Queue_Pop(&tmp_bev_data))
		   	{
		
				Map_Parse_Dev_iter		m1_iter;

				if (tmp_bev_data.data_len > 0)
				{
					//Debug_printf((char*)"parse data ", tmp_bev_data.data, tmp_bev_data.data_len);	
						std::unique_lock<std::mutex> lck (Map_Data_Dev.lock);
						m1_iter = Map_Data_Dev.Map.find(tmp_bev_data.bev);
						if(m1_iter != Map_Data_Dev.Map.end())	
						{	
							parse_dev_msg = m1_iter->second;
							//解析数据
							int r = parse_dev_msg->Protocol_Message_Parse_json(tmp_bev_data.bev,tmp_bev_data.data, tmp_bev_data.data_len);
							if(r== -2)
							{											
								Map_Data_Dev.Map.erase(m1_iter);
								bufferevent_free(tmp_bev_data.bev);//关闭连接	
							}
						}
						else
						{
							parse_dev_msg == NULL;
							cout<<"not find bev,it may break connect\n"<<endl;
						}
				
				}
		   	}
		}
		
			
		//std::chrono::milliseconds	dura(1000);
		//std::this_thread::sleep_for(dura);
		//usleep(1000);
	}
	
}
void  *send_back_thread(void *arg)

{
	//-----------------------------------------------------------------------------------
		bev_data_struct tmpdata;
	//阻塞的方式获取队列数据
	while(1)
	{
		if (Device_Send_queue.Data_Queue_Pop(&tmpdata))
		{
			bufferevent_write(tmpdata.bev, tmpdata.data, tmpdata.data_len);
			//Debug_printf((char*)"senddata",tmpdata.data,tmpdata.data_len);
		}
	}	
}



static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	//	timeout_add;
//			timeout_set(bev, );
	//		event_set(bev->ev_read, );
	bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, NULL);
	
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);

	Map_Parse_Dev_iter		m1_iter;
	{
		std::unique_lock<std::mutex> lck (Map_Data_Dev.lock);
		m1_iter = Map_Data_Dev.Map.find(bev);
		if(m1_iter == Map_Data_Dev.Map.end())	
		{				
			cout<< "new socket:"<<bev<<endl;
//			Protocol_Msg_dev  *msg_parse = new Protocol_Msg_dev();
//			Map_Data_Dev.Map.insert(pair<struct bufferevent*, Protocol_Msg_dev*> (bev, msg_parse));

			boost::shared_ptr<Protocol_Msg_dev> msg_parse1( new Protocol_Msg_dev());//创建一个解析器			
			Map_Data_Dev.Map.insert(
				pair<struct bufferevent*, boost::shared_ptr<Protocol_Msg_dev>> 
				(bev, msg_parse1));
		}	
	}
	
	

//	bufferevent_disable(bev, EV_READ);
//	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

void conn_readcb(struct bufferevent *bev, void *ctx)
{
	char  	tmpbuf[4096];
	int    len;
	struct evbuffer *input = bufferevent_get_input(bev);
	if (evbuffer_get_length(input) > 0) {
		
		//std::unique_lock<std::mutex> lck (Device_Recv_queue.lock);
		bev_data_struct  tmpdata;
		tmpdata.bev = bev;
		memset(tmpdata.data,0,sizeof(tmpdata.data));
		len = bufferevent_read(bev, tmpdata.data, sizeof(tmpdata.data));
		
		//cout<<"read len="<<len<<endl;		
//		Debug_printf(har * funNameStr,const char * buf,uint16 buflen)
		tmpdata.data_len = len;
//		Device_Recv_queue.data_queue.push(tmpdata);		
		Device_Recv_queue.Data_Queue_Push(&tmpdata);
		//Debug_printf((char *)"recvdata", tmpdata.data, tmpdata.data_len);
		
	//	pool_add_worker(Msg_Deal_thread,0);

	}
}

//static void
//conn_writecb(struct bufferevent *bev, void *user_data)
//{
//	struct evbuffer *output = bufferevent_get_output(bev);
//	if (evbuffer_get_length(output) == 0) {
//		//printf("flushed answer\n");
//	//		bufferevent_free(bev);
//	}
//}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}
	//cout<<"conn_eventcb-------->"<<bev<<endl;
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	string did_str;
	{
		std::unique_lock<std::mutex> lck (Map_Data_Dev.lock);
		Map_Parse_Dev_iter m1_iter;
		m1_iter = Map_Data_Dev.Map.find(bev);
		if(m1_iter != Map_Data_Dev.Map.end())	
		{		
			if(m1_iter->second != NULL)
			{
				did_str = m1_iter->second->did_str;
				cout<<"delete socket msg parse\n";

				cout<<"dev_id="<<did_str.c_str()<<endl;				
			}

			Map_Data_Dev.Map.erase(m1_iter);
		}
		bufferevent_free(bev);
	}
	
	{
		std::unique_lock<std::mutex> lck (Map_Dev_online.lock);
		Map_Dev_online_iter m1_iter1 = Map_Dev_online.Map.find(did_str);

		if(m1_iter1 != Map_Dev_online.Map.end())	
		{		
			cout<<"delete dev_client\n";
			Map_Dev_online.Map.erase(m1_iter1);
		}
	}
	
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}
//--------------------------------------
void print_trace (int  args)
{
	void *buffer[30] = {0};
	 size_t size;
	 char **strings = NULL;
	 size_t i = 0;

	 size = backtrace(buffer, 30);
	 fprintf(stdout, "Obtained %zd stack frames.nm\n", size);
	 strings = backtrace_symbols(buffer, size);
	 if (strings == NULL)
	 {
	  perror("backtrace_symbols.");
	  exit(EXIT_FAILURE);
	 }
	 
	 for (i = 0; i < size; i++)
	 {
	  fprintf(stdout, "%s\n", strings[i]);
	 }
	 free(strings);
	 strings = NULL;
	 exit(0);
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


