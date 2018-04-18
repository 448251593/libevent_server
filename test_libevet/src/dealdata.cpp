#include <iostream>
#include <pthread.h>

#include "stdio.h"

using namespace std;
pthread_t 	dealdata_thread;

void *Deal_thread(void *arg)
{
	
	while(1)
	{
//		list_get_data();
	}
}
void Creat_Deal_thread(void)
{
	if (pthread_create(&dealdata_thread, NULL,Deal_thread,NULL))
	{
		//echo("create p2p thread eror! %s", strerror(errno));
		//return errno;
	}
}

/*******************************************************************************
 * Description : Debug_printf
 * Arguments   :
 * Returns     :
 * Caller      : APP layer.
 * Notes       : none.
 *******************************************************************************/
void    Debug_printf(char *funNameStr,  char *buf, unsigned int buflen)
{
    int i;
    printf("%s->len=%d ", funNameStr, buflen);
    for( i = 0; i < buflen ; i++ )
    {
        printf("%-2x ", (unsigned char)(buf[i]));//加上强制转换很有必要
    }

    printf("\n");//加上强制转换很有必要

}

