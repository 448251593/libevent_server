#if 0
#include <stdio.h>
#include <string.h>

#include <iostream>
#include "protocol_dev.h"
#include "mysql_class.h"
#include <mysql.h>

using namespace std;

mysql_class::mysql_class() 
{
	g_host_name = (char *)"127.0.0.1";
	g_user_name =(char *) "bcg";
	g_password  =(char *) "bcg12345";
	g_db_name   =(char *) "device";
	g_db_port   = 3306;
}

mysql_class::~mysql_class() 
{
	mysql_close(g_conn); // �ر�����
}

int mysql_class::init_mysql() 
{ 
	// ��ʼ������
    // init the database connection
    g_conn = mysql_init(NULL);

    /* connect the database */
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0)) // ���ʧ��
	{
		printf("connect err\n");
		return -1;
	}
    // �Ƿ������Ѿ�����
    if (executesql("set names utf8")) // ���ʧ��
	{
		printf("set name err\n");
		mysql_close(g_conn);//�Ͽ�����
		return -1;
	}
	
	
	
	printf("init sql ok\n");
	
    return 0; // ���سɹ�
}

int mysql_class::get_user_devices(const char *pusername ,const char *ppassword) 
{	
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	int iNum_rows = 0;
	
	//-----------------------------------��username��ѯ-----------------------------------

	sprintf(sql, "select * from `device`.`table_user_devid_bind` where username = '%s';", pusername);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	g_res = mysql_store_result(g_conn); // �ӷ��������ͽ���������أ�mysql_use_resultֱ��ʹ�÷������ϵļ�¼��
    
    iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id ������
	}
	//while ((g_row=mysql_fetch_row(g_res))) // ��ӡ�����
	//{
	//	printf("%s\t%s\n", g_row[0], g_row[1]); // ��һ���ڶ��ֶ�
	//}
	
    mysql_free_result(g_res); // �ͷŽ����
	
	return 0;
}

int mysql_class::bind_devid_user(const char *p_devid,const char *pusername ,const char *ppassword) {
	
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	int iNum_rows = 0;

	//
	//1  ��ѯ�Ƿ����
	//2  ƥ������
	//3  ���豸
	//-------------------------------------------------------
	sprintf(sql, "select deviceid from `device`.`table_register_devid` where deviceid = '%s';", p_devid);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	g_res = mysql_store_result(g_conn); // 
    
    iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id ������
	}
	mysql_free_result(g_res); // �ͷŽ����
	//-------------------------------------------------------
	sprintf(sql, "select username from `device`.`table_register_user` where username = '%s';", pusername);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	mysql_free_result(g_res); // �ͷŽ����
	
	g_res = mysql_store_result(g_conn); //
    
    iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	if(iNum_rows != 1)
	{
		//failed
		return -3;//user name ������
	}
	mysql_free_result(g_res); // �ͷŽ����
	
	//-------------------------------------------------------
	sprintf(sql, "INSERT INTO `device`.`table_user_devid_bind`  VALUES ('%s','%s');",pusername, p_devid);
	
	printf("sql string = %s\n", sql);
	
    if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	return 0;	
}
int  mysql_class::get_register_devid(const char *p_devid,  char *poutctrl_key)
{
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	int iNum_rows = 0;

	//-------------------------------------------------------
	sprintf(sql, "select "
				 "ctrl_key "
				 "from `device`.`table_register_devid` "
				 "where deviceid = '%s';", p_devid);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	g_res = mysql_store_result(g_conn); // 
    iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id ������
	}
	g_row=mysql_fetch_row(g_res);
	if(g_row)
	{
		cout<<"->ctrl_key: "<<g_row[0]<<endl;
		strcpy(poutctrl_key,g_row[0]);
	}
	mysql_free_result(g_res); // �ͷŽ����
	return 0;
	
}

int mysql_class::register_devid(const char *p_devid, const char *pctrl_key) {
	
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	
	
    //sprintf(sql, "insert into table_register_devid values (\"%s\")", p_devid);
	sprintf(sql, "INSERT INTO `device`.`table_register_devid`"
				" (`deviceid`, `ctrl_key`) VALUES ('%s', '%s');", 
			p_devid,pctrl_key);
	
	printf("sql string = %s\n", sql);
	int  rslt = executesql(sql);
    /*	if (rslt == 0)
	{
       		// print_mysql_error(NULL);
		return 0;
	}
	else if (rslt == 1)
	{
		
	}
	else
	{

	}*/
	return rslt;	
}

int mysql_class::register_user(char *pusername, char * ppassword) 
{
    char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	
    sprintf(sql, "insert into  `device`.`table_register_user` VALUES (\"%s\", \"%s\");", pusername, ppassword);
	printf("sql string = %s\n", sql);
    if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	return 0;		
}


#endif