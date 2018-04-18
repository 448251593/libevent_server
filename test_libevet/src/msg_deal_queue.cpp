#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>

#include "protocol_dev.h"
#include "msg_deal_queue.h"

#include <bufferevent.h>
#include <buffer.h>
#include <listener.h>
#include <util.h>
#include <event.h>
#include <semaphore.h>

using namespace  std;

//Msg_Buf_Queue_head				Msg_Cmd_Q_head;	//消息命令队列头
//Msg_Buf_Queue_head				Msg_Send_Q_head;	//消息命令队列头

Message_queue::~Message_queue()
{
	Msg_Cmd_Destroy_queue();
}
Message_queue::Message_queue()
{
}

Message_queue::Message_queue(int q_nums)
{
	Msg_Cmd_queue_Init(q_nums);
}



/******************************************************************************
* Description	 : Msg_cmd_queue_Init
* Arguments	 :
* Returns		 :
* Caller		 : APP layer.
* Notes		 : none.
*******************************************************************************/
int  Message_queue::Msg_Cmd_queue_Init( int  q_nums)

{
    msg_buf_struct   *ptemp, *ptemp1;
    int 			 i;
	
	Msg_Buf_Queue_head	*pmsg_q_head = &Msg_Cmd_Q_head;

    memset((char *)pmsg_q_head, 0, sizeof(Msg_Buf_Queue_head));

    pthread_mutex_init(&pmsg_q_head->q_lock, NULL);	 //互斥锁

    ptemp = (msg_buf_struct *)malloc(sizeof(msg_buf_struct));
    if(!ptemp)
    {
        cout<<"pmalloc err1-------->>>\n";
        return -1;
    }
    
    memset(ptemp, 0, sizeof(msg_buf_struct));//清零
    //----------------------------------------------
    pmsg_q_head->q_list = ptemp;
    for ( i = 1 ; i < q_nums ; i++)
    {
        ptemp1 = (msg_buf_struct *)malloc(sizeof(msg_buf_struct));
        if(!ptemp1)
        {
            cout<<"pmalloc err2------------------------->>>\n";
            return -1;
        }
        memset(ptemp1, 0, sizeof(msg_buf_struct));//清零
        //------------------------------

        ptemp->pnext = ptemp1;//链接
        ptemp = ptemp->pnext;//后移
    }

    pmsg_q_head->q_p_end = ptemp;//指向最后一个此处指向是ptem1一个非空实体
    pmsg_q_head->q_p_end->pnext = pmsg_q_head->q_list;/*指向 头环形*/
    return 0;
}

/******************************************************************************
* Description	 : add_send_buf
* Arguments	 :
* Returns		 :
* Caller		 : APP layer.
* Notes		 : none.
*******************************************************************************/
int  Message_queue::Msg_Cmd_Add_queue( char *data, int len)
{
    int i;
    msg_buf_struct   *ptemp,*ptemp1;
	Msg_Buf_Queue_head	*pmsg_q_head = &Msg_Cmd_Q_head;

    //检测长度
    if(len > CMD_BUF_SIZE)
    {
        return -1;//数据太长算出错.因为有crc校验.错误存储没有意义了
    }
    pthread_mutex_lock(&pmsg_q_head->q_lock);

    ptemp = pmsg_q_head->q_list;
    if ( pmsg_q_head->frm_nums < CMD_BUF_Q_NUM)
    {
        /*移动队列指针到末尾*/
        for(i = 0; i < pmsg_q_head->frm_nums; i++)
        {
            ptemp = ptemp->pnext;
        }
//		ptemp->bev = bev;
//        ptemp->fd = fd;		
        ptemp->len = len;

        memcpy(ptemp->buf, data, len);/*赋值*/

        pmsg_q_head->frm_nums++;
        pthread_mutex_unlock(&pmsg_q_head->q_lock);
        return 0;
    }

    /*全部都满了,就把第一个覆盖(删除),然后放在队列尾,其实就是把头后移一次*/
    ptemp = pmsg_q_head->q_list;
    //    ptemp->resend_ct = CHBUF_NOSND;//标志新添加
    //    ptemp->pnext 的值不要修改了,链表断裂.
    ptemp->len = len;
    memcpy(ptemp->buf, data, len);/*赋值*/
    //------------------------------------------------------------------------
    ptemp1 = pmsg_q_head->q_list;
    pmsg_q_head->q_list = pmsg_q_head->q_list->pnext;/*头后移*/
    pmsg_q_head->q_p_end = ptemp1;/*更新p_end*/
    //    DEBUG(" msg queue overflow..frameid=%d\n", ptemp->len);
    //------------缓冲区中使用数量=Chnl_buf_nums----------
    pmsg_q_head->frm_nums = CMD_BUF_Q_NUM;
    pthread_mutex_unlock(&pmsg_q_head->q_lock);
    return 0;
}
/******************************************************************************
* Description	 : Msg_Cmd_Get_queue
* Arguments	 :
* Returns		 :
* Caller		 : APP layer.
* Notes		 : none.
*******************************************************************************/
int  Message_queue::Msg_Cmd_Get_queue(char *pmsg_out)
{
    msg_buf_struct   *ptemp;
		Msg_Buf_Queue_head	*pmsg_q_head = &Msg_Cmd_Q_head;
    //    uint32           tmpflag;
    if(pthread_mutex_trylock(&pmsg_q_head->q_lock) == 0)
    {

        if ( pmsg_q_head->frm_nums > 0)
        {
            ptemp = pmsg_q_head->q_list;

            memcpy((char*)pmsg_out, ptemp->buf, ptemp->len);
            pthread_mutex_unlock(&pmsg_q_head->q_lock);
//			*pfd = ptemp->fd;
//			*bev = ptemp->bev;
            return ptemp->len;
        }
		 pthread_mutex_unlock(&pmsg_q_head->q_lock);
    }
   
    return -1;
}

/******************************************************************************
* Description	 : Msg_cmd_Del_queue_first 删除第一个
* Arguments	 :
* Returns		 :
* Caller		 : APP layer.
* Notes		 : none.
*******************************************************************************/
int  Message_queue::Msg_Cmd_Del_queue_first()
{
    msg_buf_struct   *ptemp1;//*ptemp
    	Msg_Buf_Queue_head	*pmsg_q_head = &Msg_Cmd_Q_head;
	pthread_mutex_lock(&pmsg_q_head->q_lock);

    if ( pmsg_q_head->frm_nums > 0)
    {
        //ptemp = Msg_Cmd_Q_head.q_list;
        pmsg_q_head->frm_nums--;//帧数量计数器

        ptemp1 = pmsg_q_head->q_list;
        pmsg_q_head->q_list = pmsg_q_head->q_list->pnext;/*头后移*/
        pmsg_q_head->q_p_end = ptemp1;/*更新p_end*/
    }
	pthread_mutex_unlock(&pmsg_q_head->q_lock);

    return 0;
}
/******************************************************************************
* Description	 : Msg_Cmd_Destroy_queue 
* Arguments	 :
* Returns		 :
* Caller		 : APP layer.
* Notes		 : none.
*******************************************************************************/
int  Message_queue::Msg_Cmd_Destroy_queue()
{
    msg_buf_struct	 *ptemp, *ptemp1;
	Msg_Buf_Queue_head	*pmsg_q_head = &Msg_Cmd_Q_head;
	
    pmsg_q_head->frm_nums = 0;//清零
    sleep(1);
    ptemp = pmsg_q_head->q_list;

    pmsg_q_head->q_p_end->pnext = NULL; //断开链表

    while(ptemp)
    {
        ptemp1 = ptemp->pnext;//保存next地址
        free(ptemp);//释放ptemp本身
        ptemp = ptemp1;//地址前进
    }

    return 0;
}

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

