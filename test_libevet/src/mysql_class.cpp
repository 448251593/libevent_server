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
	mysql_close(g_conn); // 关闭链接
}

int mysql_class::init_mysql() 
{ 
	// 初始化连接
    // init the database connection
    g_conn = mysql_init(NULL);

    /* connect the database */
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0)) // 如果失败
	{
		printf("connect err\n");
		return -1;
	}
    // 是否连接已经可用
    if (executesql("set names utf8")) // 如果失败
	{
		printf("set name err\n");
		mysql_close(g_conn);//断开连接
		return -1;
	}
	
	
	
	printf("init sql ok\n");
	
    return 0; // 返回成功
}

int mysql_class::get_user_devices(const char *pusername ,const char *ppassword) 
{	
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	int iNum_rows = 0;
	
	//-----------------------------------以username查询-----------------------------------

	sprintf(sql, "select * from `device`.`table_user_devid_bind` where username = '%s';", pusername);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集
    
    iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id 不存在
	}
	//while ((g_row=mysql_fetch_row(g_res))) // 打印结果集
	//{
	//	printf("%s\t%s\n", g_row[0], g_row[1]); // 第一，第二字段
	//}
	
    mysql_free_result(g_res); // 释放结果集
	
	return 0;
}

int mysql_class::bind_devid_user(const char *p_devid,const char *pusername ,const char *ppassword) {
	
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	int iNum_rows = 0;

	//
	//1  查询是否存在
	//2  匹配密码
	//3  绑定设备
	//-------------------------------------------------------
	sprintf(sql, "select deviceid from `device`.`table_register_devid` where deviceid = '%s';", p_devid);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	
	g_res = mysql_store_result(g_conn); // 
    
    iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id 不存在
	}
	mysql_free_result(g_res); // 释放结果集
	//-------------------------------------------------------
	sprintf(sql, "select username from `device`.`table_register_user` where username = '%s';", pusername);
	if (executesql(sql))
	{
        print_mysql_error(NULL);
		return -1;
	}
	mysql_free_result(g_res); // 释放结果集
	
	g_res = mysql_store_result(g_conn); //
    
    iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
	if(iNum_rows != 1)
	{
		//failed
		return -3;//user name 不存在
	}
	mysql_free_result(g_res); // 释放结果集
	
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
    iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
	if(iNum_rows != 1)
	{
		//failed
		return -2;//device id 不存在
	}
	g_row=mysql_fetch_row(g_res);
	if(g_row)
	{
		cout<<"->ctrl_key: "<<g_row[0]<<endl;
		strcpy(poutctrl_key,g_row[0]);
	}
	mysql_free_result(g_res); // 释放结果集
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