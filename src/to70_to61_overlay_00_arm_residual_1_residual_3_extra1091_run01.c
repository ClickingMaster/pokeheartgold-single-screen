#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 value;u16 status,flag,priority;u8 pad[2];u8 retry;u8 pad2[3];u32 state;u8 data[1];} S;
typedef struct {s32 error;S *p;} G;
extern G ov00_0221A75C;
extern void ov00_021EC60C(void),ov00_021EC24C(int,int),ov00_021EC8D8(void),ov00_021EC3F0(S *,u16,int,u16),ov00_021FDCDC(void *);
extern s32 ov00_021FA9D4(void);extern int CPS_GetSslHandshakePriority(void);
int ov00_021EC724(void);
int ov00_021EC724(void){s32 result;if(!ov00_0221A75C.p)return 0;switch(ov00_0221A75C.p->state){case 1:return 2;case 2:return 2;case 3:ov00_021EC60C();ov00_021EC24C(5,ov00_0221A75C.error);ov00_0221A75C.p->status=7;ov00_0221A75C.p->state=4;return 7;case 4:return 7;}{u16 status=ov00_0221A75C.p->status;if(status==1)return status;}result=ov00_021FA9D4();if(result==5){ov00_0221A75C.p->status=4;ov00_0221A75C.p->flag=1;ov00_0221A75C.p->retry=1;return 4;}if(result<0){if(result>=-10){ov00_021EC24C(9,(s32)0xffff3be9);ov00_0221A75C.p->status=8;return 8;}if(ov00_0221A75C.p->retry==0){S *p=ov00_0221A75C.p;u16 priority=p->priority;int handshake=CPS_GetSslHandshakePriority();ov00_0221A75C.error=result;ov00_021EC8D8();ov00_021EC3F0(p,priority,1,(u16)handshake);ov00_021FDCDC(ov00_0221A75C.p->data);ov00_0221A75C.p->state=1;ov00_0221A75C.p->value=0;return 2;}if(ov00_0221A75C.error<result)result=ov00_0221A75C.error;ov00_021EC24C(5,result);ov00_0221A75C.p->status=7;return 7;}return 2;}
