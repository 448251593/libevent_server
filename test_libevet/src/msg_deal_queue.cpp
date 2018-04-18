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

//Msg_Buf_Queue_head				Msg_Cmd_Q_head;	//��Ϣ�������ͷ
//Msg_Buf_Queue_head				Msg_Send_Q_head;	//��Ϣ�������ͷ

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

    pthread_mutex_init(&pmsg_q_head->q_lock, NULL);	 //������

    ptemp = (msg_buf_struct *)malloc(sizeof(msg_buf_struct));
    if(!ptemp)
    {
        cout<<"pmalloc err1-------->>>\n";
        return -1;
    }
    
    memset(ptemp, 0, sizeof(msg_buf_struct));//����
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
        memset(ptemp1, 0, sizeof(msg_buf_struct));//����
        //------------------------------

        ptemp->pnext = ptemp1;//����
        ptemp = ptemp->pnext;//����
    }

    pmsg_q_head->q_p_end = ptemp;//ָ�����һ���˴�ָ����ptem1һ���ǿ�ʵ��
    pmsg_q_head->q_p_end->pnext = pmsg_q_head->q_list;/*ָ�� ͷ����*/
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

    //��ⳤ��
    if(len > CMD_BUF_SIZE)
    {
        return -1;//����̫�������.��Ϊ��crcУ��.����洢û��������
    }
    pthread_mutex_lock(&pmsg_q_head->q_lock);

    ptemp = pmsg_q_head->q_list;
    if ( pmsg_q_head->frm_nums < CMD_BUF_Q_NUM)
    {
        /*�ƶ�����ָ�뵽ĩβ*/
        for(i = 0; i < pmsg_q_head->frm_nums; i++)
        {
            ptemp = ptemp->pnext;
        }
//		ptemp->bev = bev;
//        ptemp->fd = fd;		
        ptemp->len = len;

        memcpy(ptemp->buf, data, len);/*��ֵ*/

        pmsg_q_head->frm_nums++;
        pthread_mutex_unlock(&pmsg_q_head->q_lock);
        return 0;
    }

    /*ȫ��������,�Ͱѵ�һ������(ɾ��),Ȼ����ڶ���β,��ʵ���ǰ�ͷ����һ��*/
    ptemp = pmsg_q_head->q_list;
    //    ptemp->resend_ct = CHBUF_NOSND;//��־�����
    //    ptemp->pnext ��ֵ��Ҫ�޸���,�������.
    ptemp->len = len;
    memcpy(ptemp->buf, data, len);/*��ֵ*/
    //------------------------------------------------------------------------
    ptemp1 = pmsg_q_head->q_list;
    pmsg_q_head->q_list = pmsg_q_head->q_list->pnext;/*ͷ����*/
    pmsg_q_head->q_p_end = ptemp1;/*����p_end*/
    //    DEBUG(" msg queue overflow..frameid=%d\n", ptemp->len);
    //------------��������ʹ������=Chnl_buf_nums----------
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
* Description	 : Msg_cmd_Del_queue_first ɾ����һ��
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
        pmsg_q_head->frm_nums--;//֡����������

        ptemp1 = pmsg_q_head->q_list;
        pmsg_q_head->q_list = pmsg_q_head->q_list->pnext;/*ͷ����*/
        pmsg_q_head->q_p_end = ptemp1;/*����p_end*/
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
	
    pmsg_q_head->frm_nums = 0;//����
    sleep(1);
    ptemp = pmsg_q_head->q_list;

    pmsg_q_head->q_p_end->pnext = NULL; //�Ͽ�����

    while(ptemp)
    {
        ptemp1 = ptemp->pnext;//����next��ַ
        free(ptemp);//�ͷ�ptemp����
        ptemp = ptemp1;//��ַǰ��
    }

    return 0;
}

int  Message_queue_bev::Data_Queue_Push(bev_data_struct *data)
{
	std::unique_lock<std::mutex> lck (this->lock);
	data_queue.push(*data);
	//������Դ����
	sem_post(&sem_block);
}
int Message_queue_bev::Data_Queue_Pop(bev_data_struct *data)
{
	//�ȴ��Ƿ�������
	sem_wait(&sem_block);
	std::unique_lock<std::mutex> lck (this->lock);

	if(!data_queue.empty())
	{
		*data = data_queue.front();
		//������ ɾ��

		data_queue.pop();
		 
		return 1;
	}
	else
	{
		return -1;
	}
}

