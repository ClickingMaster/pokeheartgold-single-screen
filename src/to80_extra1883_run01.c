#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 heap;u8 p[0x58];void *msg,*fmt,*s1,*s2,*s3,*s4,*s5;void *names[22];void *extra[7];} Work;
extern void FontID_Alloc(int,u32);extern void *NewMsgDataFromNarc(int,int,int,u32);extern void *MessageFormat_New_Custom(int,int,u32);extern void *String_New(u32,u32);extern void *NewString_ReadMsgData(void *,u32);void ov59_02239704(Work *);
void ov59_02239704(Work *w){int i,j;FontID_Alloc(4,w->heap);w->msg=NewMsgDataFromNarc(0,27,21,w->heap);w->fmt=MessageFormat_New_Custom(16,32,w->heap);w->s1=String_New(256,w->heap);w->s2=String_New(256,w->heap);w->s3=NewString_ReadMsgData(w->msg,1);w->s5=NewString_ReadMsgData(w->msg,25);for(i=0;i<22;i++)w->names[i]=NewString_ReadMsgData(w->msg,i+27);for(j=0;j<7;j++)w->extra[j]=NewString_ReadMsgData(w->msg,j+7);}
