#pragma force_active on
#include "global.h"
extern void ov85_021E78D8(void *,void *);
extern void ov85_021E7A54(void *,void *);
extern void ov85_021E7B04(void *,void *);
void ov85_021E7798(void *);
#pragma force_active on
void ov85_021E7798(void *arg){u8 *w=arg;int i=0,count;u8 *a,*ap,*b,*bp,*c,*cp;count=*(int*)(w+48);a=w+0x190;if(count>0){b=a;ap=a+0x140;bp=a+0x4b0;c=a;cp=a+0x71c;do{if(*(int*)(a+0x140))ov85_021E78D8(ap,w+0x104);if(*(int*)(b+0x4b0))ov85_021E7A54(w,bp);if(*(int*)(c+0x71c))ov85_021E7B04(w,cp);a+=0xb0;ap+=0xb0;b+=0x7c;bp+=0x7c;c+=0x68;cp+=0x68;i++;}while(i<count);}}
