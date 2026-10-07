#pragma force_active on
#include "global.h"
#pragma force_active on
u16 SVC_GetCRC16(u32,const void*,u32);
typedef struct{u32 type:8,category:4,crypt:4,id:16;}Info;
typedef struct{Info first,second;u32 kind:8,mode:8,crc:16;u32 header:8,length:24;}Packet;
Info*ov74_022311DC(void);u32 ov74_02231264(void);u32 ov74_02231260(void);void ov74_0223145C(Packet*),ov74_0223127C(u32,u32,void*);void ov74_022312C0(Packet*,const void*,u32,int,int);
void ov74_022312C0(Packet*p,const void*data,u32 length,int mode,int kind){Info*info=ov74_022311DC();p->first.type=info[0].type;p->first.category=info[0].category;p->first.crypt=info[0].crypt;p->first.id=(u16)ov74_02231264();p->second.type=info[1].type;p->second.category=info[1].category;p->second.crypt=info[1].crypt;p->second.id=0;p->kind=(u8)kind;p->mode=mode;p->crc=SVC_GetCRC16(0,data,length);p->header=(u8)ov74_02231260();p->length=length;if(length)MI_CpuCopy8(data,(u8*)p+p->header,length);ov74_0223145C(p);if(info[0].crypt==1)ov74_0223127C(p->first.id,length+p->header-4,(u8*)p+4);}
