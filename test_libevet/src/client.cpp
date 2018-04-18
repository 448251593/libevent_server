#include<stdio.h>
#include"client.h"
#include"cJSON.h"
#include <netinet/in.h>
#include <arpa/inet.h>

using  namespace std;
device_client::device_client()
{    
    pRoot = NULL;
}

device_client::device_client(int  switch_no_t)
{
	switch_no = switch_no_t;
	pRoot = NULL;	
}
device_client::~device_client()
{
  //  if ( pRoot )
  //  {
  //      cJSON_Delete(pRoot);
		//cout<<"delete proot\n";
  //  }
}

string  device_client::get_json_info(void)
{
	string  tmp;
	if ( pRoot == NULL )
	{
		if (time(0) - last_time > 60)
		{
			return "";
		}
		else
		{
			pRoot = cJSON_CreateObject();	
			cJSON_AddStringToObject(pRoot, "ver",(char*)ver_json.c_str());
			cJSON_AddStringToObject(pRoot, "dev_id",(char*)dev_id.c_str());
			cJSON_AddNumberToObject(pRoot, "expire",time(0) - last_time);
			cJSON_AddStringToObject(pRoot, "cmdtype",(char*)cmdtype.c_str());
			cJSON_AddStringToObject(pRoot, "dev_data",(char*)dev_data.c_str());

			
			//cJSON_AddStringToObject(pRoot, "origin_str",(char*)json_str_origin.c_str());
			//struct sockaddr_in		   addr;
			//addr.sin_addr.s_addr = ctrl_ip;
			//cJSON_AddStringToObject(pRoot, "ctrl_ip", (char*)inet_ntoa(addr.sin_addr));
			//cJSON_AddNumberToObject(pRoot, "data_type", data_type);
			//cJSON_AddNumberToObject(pRoot, "switch_no", switch_no);

			//cJSON    *array = cJSON_CreateArray();
			//cJSON_AddItemToObject(pRoot, "status", array);
			//int  	i;
			//for ( i = 0; i < switch_no  ; i++ )
			//{
			//	cJSON *parray_item = cJSON_CreateObject();
			//	cJSON_AddNumberToObject(parray_item, "on/off", switch_status[i]);		
			//	cJSON_AddItemToArray(array,parray_item);
			//}
		}
		
	}

   	char *cjsn = cJSON_Print(pRoot); 
	if(cjsn ==  NULL)
	{
		
		cjsn =(char*) "";	
	}
	tmp = cjsn;
		


	cJSON_Delete(pRoot);
	pRoot = NULL;
	return   tmp;
}

uint32  device_client::get_timeout(void)
{
	return time(0) - last_time;
}



int 	device_client::set_switch_num(int	 num, char  *status)
{
	switch_no = num;
	int maxsize = sizeof(switch_status);
	switch_no = switch_no < maxsize ? switch_no : maxsize;

	memcpy(switch_status, status, switch_no);
	
	return 0;
}
int 	device_client::get_switch_num()
{
	return switch_no;
}


Ctrl_client_Class::Ctrl_client_Class()
{
	self_bev = NULL;
}






















