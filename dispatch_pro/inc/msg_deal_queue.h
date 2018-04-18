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
			sem_init (&sem_block, 0, 0);//≥ı ºªØ0
		};
		
		int Data_Queue_Push(bev_data_struct *data);
		int Data_Queue_Pop(bev_data_struct *data);

		
};
#endif
