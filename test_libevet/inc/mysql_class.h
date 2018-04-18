#if 0
#ifndef  _MYSQL_H_
#define  _MYSQL_H_

#include <mysql/mysql.h>
#define MAX_BUF_SIZE 1024 // ����������ֽ���


class   mysql_class
{
private:
    MYSQL *g_conn; // mysql ����
    MYSQL_RES *g_res; // mysql ��¼��
    MYSQL_ROW g_row; // �ַ������飬mysql ��¼��


    void print_mysql_error(const char *msg)
    { // ��ӡ���һ�δ���
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
        //if (rslt) // ���ʧ��
         //   return -1; // ��ʾʧ��

        return rslt; // �ɹ�ִ��
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
