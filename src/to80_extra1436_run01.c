#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{int state,error;}Status;
typedef struct{union{struct{int state,error;};Status status;};u8 p[0x40-8];void(*callback)(int,void*);u8 q[0x144-0x44];int value;}Work;
extern Work *_022577C4;extern int ov45_02234A24(void*,void*,void*,void**,u32*);extern void ov45_0223AF04(void*,void*);
void ov45_022353F0(void*,void*,void*);
void ov45_022353F0(void*a,void*b,void*c){void*data;u32 size;int result;Work*w=_022577C4;if(!w||w->state==9)return;data=0;size=0;result=ov45_02234A24(a,b,c,&data,&size);if(size!=0x2d8)result=2;if(result){{Work*target=_022577C4;target->error=7;target->state=10;}return;}ov45_0223AF04(&_022577C4->value,data);_022577C4->callback(result,data);_022577C4->value=2;}
