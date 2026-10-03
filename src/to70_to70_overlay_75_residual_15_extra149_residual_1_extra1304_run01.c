#pragma force_active on
#include "global.h"
typedef struct {u8 pad[16];int error;u8 p2[16];void*a;u8 p3[12];void*b;} Work;
void ov75_022494CC(Work*,void*,int,int,int);
void ov75_02247854(Work*,int,int);
int ov75_02249278(Work*);
int ov75_02249278(Work*w){int alt=0,msg;switch(w->error){
case 1:msg=0x36;break;case -1:case 2:msg=0x37;break;case -14:case -2:msg=0x39;break;
case -15:case -12:case -5:case -3:default:msg=0x38;break;
case -5001:msg=0xb5;alt++;break;case -5002:msg=0xb6;alt++;break;case -5003:msg=0xb6;alt++;break;case -5004:msg=0xb7;alt++;break;case -5005:msg=0xb8;alt++;break;case -5006:msg=0xb6;alt++;break;case -5007:msg=0xb6;alt++;break;case -5010:msg=0xb7;alt++;break;case -5011:msg=0xb8;alt++;break;
}if(!alt)ov75_022494CC(w,w->b,msg,1,0xf0f);else ov75_022494CC(w,w->a,msg,1,0xf0f);ov75_02247854(w,0x22,0x21);return 0;}
