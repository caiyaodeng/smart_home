#include "type.h"
//int get_10(const unsigned char *p16) {
//        int var = 0,res=0;
//        memcpy(&var,p16,1);
//        res=(int)var*16*16*16+res;
//        memcpy(&var,p16+1,1);
//        res=(int)var*16*16+res;
//        memcpy(&var,p16+2,1);
//        res=(int)var*16+res;
//        memcpy(&var,p16+3,1);
//        res=(int)var+res;
//        return res;
//}
int get_10(const unsigned char *strValue){  
    	int a = 0;
    	for(int i =0; i< 4; i++){
    	      a += (strValue[i]&0xff) << (24-8*i);
    	}
    	return a;
    }  
unsigned char *get_16(const int intValue) {
	unsigned char *str16 = nullptr;  
	str16=(unsigned char*)malloc(4);
        str16[3] = (unsigned char)(intValue & 0xff);
        str16[2] = (unsigned char)(intValue>>8 & 0xff);
        str16[1] = (unsigned char)(intValue>>16 & 0xff);
        str16[0] = (unsigned char)(intValue>>24 & 0xff);
	return str16;
}

