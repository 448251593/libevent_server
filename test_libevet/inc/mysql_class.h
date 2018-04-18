#if 0
#ifndef  _MYSQL_H_
#define  _MYSQL_H_

#include <mysql/mysql.h>
#define MAX_BUF_SIZE 1024 // 缓冲区最大字节数


class   mysql_class
{
private:
    MYSQL *g_conn; // mysql 连接
    MYSQL_RES *g_res; // mysql 记录集
    MYSQL_ROW g_row; // 字符串数组，mysql 记录行


    void print_mysql_error(const char *msg)
    { // 打印最后一次错误
        if (msg)
            printf("%s: %s\n", msg, mysql_error(g_conn));
        else
            puts(mysql_error(g_conn));
    }
	
    int executesql(const char * sql)
    {
        /*query the database according the sql*/
	int rslt = 0;
	rslt = mysql_real_query(g_conn, sql, strlen(sql));
	//cout<<"rslt="<<rslt<<endl;
        //if (rslt) // 如果失败
         //   return -1; // 表示失败

        return rslt; // 成功执行
    }

public:
	char *g_host_name;// = "127.0.0.1";
    char *g_user_name;// = "root";
    char *g_password ;// = "bcg12345";
    char *g_db_name  ;// = "device";
    unsigned int g_db_port;// = 3306;
	mysql_class();
	~mysql_class();
	int  init_mysql();
	int  register_user(char *pusername, char *ppassword);
    int  register_devid(const char *p_devid, const char *pctrl_key);
	int  get_register_devid(const char *p_devid,  char *poutctrl_key);
	
	int  bind_devid_user(const char *p_devid,const char *pusername ,const char *ppassword);
	
	int get_user_devices(const char *pusername ,const char *ppassword); 
    //int  Set_devid_attr(char  *pid, char *puser);
    //int  Get_devid_attr(char  *pid, char *puser);
};


#endif
#endif
