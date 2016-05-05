/*************************************************************************
  > File Name: time.cpp
  > Author: CaiYaodeng
  > Mail:512324291@qq.com 
  > Created Time: Wed 04 May 2016 09:52:00 AM EDT
 ************************************************************************/
#include "timex.h"

namespace tx {
	bool get_UTC_E8(unsigned char *strUTC){
		type_int *pType = (type_int *)malloc (sizeof (type_int));  
		memset (pType, 0, sizeof (type_int));  
		time_t time_UTC;
		if (time(&time_UTC) == -1){ /*获得time_t结构的时间，UTC时间*/
            return false;
        }
		time_UTC+=28800;
		/*要转成字符型的整型变量*/  
		pType->i_t = time_UTC;  

		memcpy(strUTC, pType->str_t, 4);

		return true;  
	}  
}

