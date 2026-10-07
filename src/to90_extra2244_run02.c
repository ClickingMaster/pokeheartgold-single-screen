#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *NewString_ReadMsgData(void*,int);
extern void StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),ov18_021F95FC(void*,void*,int,int,int,int,int);
void ov18_021EE3AC(u8*,void*,int,int,int,int,int,int,int);
void ov18_021EE3AC(u8 *s,void *msg,int index,int strno,int a,int b,int c,int d,int e){void *str=NewString_ReadMsgData(msg,strno);StringExpandPlaceholders(*(void**)(s+0x660),*(void**)(s+0x664),str);ov18_021F95FC(s+12+index*16,*(void**)(s+0x664),a,b,c,d,e);String_Delete(str);}
