#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[4];s16 cursor;}Ring;
typedef struct{int copied;u8*data;u16 length;u8 command,pad;}Packet;
typedef struct{u8 pad[8];u8*work;}Manager;
extern Manager _021D4140;
extern int sub_02033BC4(Ring*),sub_02033B4C(Ring*,void*,int);extern u8 sub_02033B68(Ring*);extern u32 sub_020341DC(int);extern int sub_02034244(int);extern void*sub_02034280(int,int,int);extern void sub_020371A8(int,int,int,void*,Packet*);
void sub_020371C4(Ring*,int,u8*,Packet*);
void sub_020371C4(Ring*r,int peer,u8*buffer,Packet*p){while(sub_02033BC4(r)){int command=p->command;if(command==238){command=sub_02033B68(r);if(command==238)continue;}int old=r->cursor;p->command=command;int length=p->length;if(length==65535){length=sub_020341DC(command);if(_021D4140.work[0x6b8])return;if(length==65535){if(sub_02033BC4(r)<1){r->cursor=old;return;}length=sub_02033B68(r)<<8;length+=sub_02033B68(r);old=r->cursor;}p->length=length;}if(sub_02034244(command)){if(!p->data)p->data=sub_02034280(command,peer,p->length);int copied=sub_02033B4C(r,buffer,length-p->copied);if(p->data)MI_CpuCopy8(buffer,p->data+p->copied,copied);p->copied=(s32)((u32)p->copied+(u32)copied);if(p->copied>=length){sub_020371A8(peer,command,length,p->data,p);if(command==17)break;}}else{if(sub_02033BC4(r)>=length){sub_02033B4C(r,buffer,length);sub_020371A8(peer,command,length,buffer,p);if(command==17)break;}else{r->cursor=old;return;}}}}
