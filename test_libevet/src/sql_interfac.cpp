#if 0
#include"sqlite3.h"
#include <stdlib.h>
#include<stdio.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include"sql_interface.h"

using namespace std;


Sql_Interface_class::Sql_Interface_class(const char *p_sql_path_name)
{
    int  rc;
    rc = sqlite3_open(p_sql_path_name, &db);
    if(rc)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
		//sqlite3_close(db);	
	}	
	else 	
	{
		printf("db open successfully!\n");	
		const char *sql_str= "select max(id) from product_id;";
		sqlite3_stmt *stat;
	    const void * data;
	    int     rslt = 0;

	    sqlite3_prepare(db, sql_str, -1, &stat, 0);/*??*/
	    rslt = sqlite3_step(stat);
	    if (SQLITE_ROW == rslt)//
	    {
	        max_id = sqlite3_column_int(stat, 0);	       
	        printf("max_id=%d\n  ", max_id);		
	    }
	    else
	    {
	        printf("max_id no item\n");
			sqlite3_close(db); // close sqlite db
			db = NULL;
	    }
		
    }
}

Sql_Interface_class::~Sql_Interface_class()
{
	if(db)
	{
		sqlite3_close(db); // close sqlite db
	}
}

int Sql_Interface_class::Generate_devid(char *p_devid)
{
	if(db)
	{
		max_id++;//最大id++
		char   sql_str_tmp[1024];
		memset(sql_str_tmp, 0, sizeof(sql_str_tmp));
		
		sprintf(p_devid, "%d%d", (int)time(0), max_id);//id create

		sprintf(sql_str_tmp, "insert into dev_id_attr values(\"%s\",0,\"\"); ", 
			p_devid);	
		
		char * zErrMsg = 0;
		sqlite3_exec(db, sql_str_tmp, 0, 0, &zErrMsg);//??sql??
		cout<<"1"<<sql_str_tmp<<endl;
		
		
		sprintf(sql_str_tmp, "insert into product_id values(%d,\"\");", 
				max_id);	
		sqlite3_exec(db, sql_str_tmp, 0, 0, &zErrMsg);//??sql??
		cout<<"2"<<sql_str_tmp<<endl;
		
		return 0;
	}
	return -1;
}

int Sql_Interface_class::Set_devid_attr(char  *pid, char *puser)
{
	if(db)
	{
		max_id++;//最大id++
		char   sql_str_tmp[1024];
		memset(sql_str_tmp, 0, sizeof(sql_str_tmp));
		
	
		if(pid)
		{
			cout<<"pid id null\n";
			return -2;
		}
		if(puser)
		{
			sprintf(sql_str_tmp, "update dev_id_attr set struser=\"\" set bingflag=1 where strid=\"%s\" ;", pid);
		}
		else
		{
			sprintf(sql_str_tmp, "update dev_id_attr set struser=\"%s\" set bingflag=1 where strid=\"%s\";", 
			puser,pid);			
		}
					
		char * zErrMsg = 0;
		sqlite3_exec(db, sql_str_tmp, 0, 0, &zErrMsg);//??sql??
		cout<<"set attr->"<<sql_str_tmp<<endl;
		
		//update dev_id_attr set struser="bcg" where strid=14444703712

		
		return 0;
	}
	return -1;
}
int Sql_Interface_class::Get_devid_attr(char  *pid, char *puser)
{
	if(db)
	{		
		char   sql_str_tmp[1024];
		memset(sql_str_tmp, 0, sizeof(sql_str_tmp));
				
		if(pid == NULL)
		{
			cout<<"pid id null\n";
			return -2;
		}
		sprintf(sql_str_tmp, "select *from dev_id_attr where strid=\"%s\";", pid);
		
		sqlite3_stmt *stat;
	    const void * data;
	    int     rslt = 0;

	    sqlite3_prepare(db, sql_str_tmp, -1, &stat, 0);/**/
	    rslt = sqlite3_step(stat);
	    if (SQLITE_ROW == rslt)//
	    {
			int bindflag = sqlite3_column_int(stat, 1);	       
	        printf("bindflag=%d\n  ", bindflag);
			
	        data = sqlite3_column_text(stat, 2);	       
	        strcpy((char*)puser, (const char*)data); 
			cout<<"user name:"<<puser<<endl;
	    }
	    else
	    {
	        printf(" no item\n");

	    }
		
		//update dev_id_attr set struser="bcg" where strid=14444703712

		
		return 0;
	}
	return -1;
}

//int main()
//{
//	Sql_Interface_class   sqlite3_ctrl("./test.db");
//	char  tmp_buf[1024];
//	
//	if(sqlite3_ctrl.Get_devid_attr((char*)"14444703712", tmp_buf) == 0)
//	{
//		cout<<"------>:"<<tmp_buf<<endl;
//	}
//	else
//	{
//		cout<<"no generate \n";
//	}
//	return 0;
//}

#endif 