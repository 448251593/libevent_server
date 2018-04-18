#if 0
#ifndef  _SQL_INTERFACE_H_
#define  _SQL_INTERFACE_H_
#include "sqlite3.h"

class  Sql_Interface_class
{
	private:
		int   max_id;
		sqlite3 *db;
	public:
	
		Sql_Interface_class(const char *p_sql_path_name);
		~Sql_Interface_class();
		int  Generate_devid(char *p_devid);
		int  Set_devid_attr(char  *pid, char *puser);
		int  Get_devid_attr(char  *pid, char *puser);
		
	
};




#endif
#endif