#pragma force_active on
#include "global.h"
typedef struct {u32 heap;u8 p[0x30c-4];void*msg;void*fmt;void*s0;void*s1;void*names[4];void*s2;void*s3;}Work;
extern void FontID_Alloc(int,u32);
extern void*NewMsgDataFromNarc(int,int,int,u32),*MessageFormat_New_Custom(int,int,u32),*String_New(int,u32),*NewString_ReadMsgData(void*,int);
void ov108_021E9C3C(Work*w);
void ov108_021E9C3C(Work*w){int i;FontID_Alloc(4,w->heap);w->msg=NewMsgDataFromNarc(0,27,430,w->heap);w->fmt=MessageFormat_New_Custom(2,16,w->heap);w->s0=String_New(256,w->heap);w->s1=String_New(256,w->heap);w->s2=NewString_ReadMsgData(w->msg,12);w->s3=NewString_ReadMsgData(w->msg,8);for(i=0;i<4;i++)w->names[i]=NewString_ReadMsgData(w->msg,i);}
