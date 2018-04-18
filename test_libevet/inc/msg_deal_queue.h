#ifndef   _MSG_DEAL_QUEUE_H_
#define   _MSG_DEAL_QUEUE_H_
#include <bufferevent.h>
#include <buffer.h>
#include <listener.h>
#include <util.h>
#include <event.h>
#include <pthread.h>

#include <queue>
#include <mutex>

#include <semaphore.h>

using namespace std;


#define    CMD_BUF_SIZE       1024

#define    CMD_BUF_Q_NUM      512

typedef struct msg_buf_s
{
	int 					fd;//接收fd
	struct bufferevent 		*bev;
	int						len;
	char					buf[CMD_BUF_SIZE];
	struct msg_buf_s		*pnext;
}msg_buf_struct;

typedef struct
{
	pthread_mutex_t 		q_lock;
	int						frm_nums;//记录缓冲区使用数量
	msg_buf_struct			*q_p_end;/*指向上一个的指针,头特有的*/
	msg_buf_struct			*q_list;/*指向list的*/
}Msg_Buf_Queue_head;




class    Message_queue
{
	private:
		
		Msg_Buf_Queue_head				Msg_Cmd_Q_head; //消息命令队列头
		int  							Msg_Cmd_Destroy_queue();
	public:
		Message_queue();
		Message_queue( int  q_nums);
		~Message_queue();
		int  Msg_Cmd_queue_Init( int  q_nums);
		int  Msg_Cmd_Add_queue( char *data, int len);
		int  Msg_Cmd_Get_queue(char *pmsg_out);
		int  Msg_Cmd_Del_queue_first();
};

typedef struct
{
	struct bufferevent		*bev;
	int                 	data_len;
	char					data[2048];
}bev_data_struct;

class Message_queue_bev
{
	private:
		sem_t 						sem_block; 
		
	public:
		mutex										lock;
		queue<bev_data_struct>						data_queue;
		Message_queue_bev(){ 
			sem_init (&sem_block, 0, 0);//初始化0
		};
		
		int Data_Queue_Push(bev_data_struct *data);
		int Data_Queue_Pop(bev_data_struct *data);

		
};
#endif
