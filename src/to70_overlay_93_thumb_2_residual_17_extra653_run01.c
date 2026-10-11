#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x28];void *a,*b;u8 p2[0x80-0x30];void*msg;u8 p3[12];void*sys;}Work;
extern void *NewString_ReadMsgData(void*,u32);
extern void TextOBJ_GetPosition(void*,int*,int*),String_Delete(void*);
extern void ov93_02261EB8(void*,void*,void*,void*,void*,int,int,int,int,int,int,int,int,int,int);

void ov93_02262250(Work*w);

void ov93_02262250(Work*w){int i;u32 n=*(u32*)((u8*)w+0x3848);i=5;u8 *cursor=(u8*)w+100,*other=(u8*)w+200;void*slot=(u8*)w+0x16b0;slot=(u8*)slot+100;for(;i>=0;i--){void*s;int x,y;if(*(void**)(cursor+0x16b0)!=0)GF_AssertFail();s=NewString_ReadMsgData(w->msg,n%10+4);n/=10;TextOBJ_GetPosition(*(void**)(other+0x15ac),&x,&y);ov93_02261EB8(w->b,w->a,w->sys,slot,s,0,0xe0f00,0,0x2713,x,168,0,1,12,2);String_Delete(s);cursor-=20;other-=40;slot=(u8*)slot-20;}}
