#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>


#include <bufferevent.h>
#include <buffer.h>
#include <listener.h>
#include <util.h>
#include <event.h>
#include <semaphore.h>
#include "msg_deal_queue.h"
using namespace  std;

//Msg_Buf_Queue_head				Msg_Cmd_Q_head;	//消息命令队列头
//Msg_Buf_Queue_head				Msg_Send_Q_head;	//消息命令队列头


int  Message_queue_bev::Data_Queue_Push(bev_data_struct *data)
{
	std::unique_lock<std::mutex> lck (this->lock);
	data_queue.push(*data);
	//发送资源数据
	sem_post(&sem_block);
}
int Message_queue_bev::Data_Queue_Pop(bev_data_struct *data)
{
	//等待是否有数据
	sem_wait(&sem_block);
	std::unique_lock<std::mutex> lck (this->lock);

	if(!data_queue.empty())
	{
		*data = data_queue.front();
		//出队列 删除

		data_queue.pop();
		 
		return 1;
	}
	else
	{
		return -1;
	}
}

