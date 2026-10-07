#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 data[0x5ca8];} State;
extern void *String_New(int,int),*YesNoPrompt_Create(int);
extern void ReadMsgDataIntoString(void *,int,void *);
void ov52_021E84CC(State *);
void ov52_021E84CC(State *s){
 u8 *p,*q;int i;u32 mask=240;
 for(i=0,p=s->data,q=s->data+0x433d;i<5;i++,p+=4,q+=17){
  *(void **)(p+24)=String_New(8,39);*q&=~mask;*(u16 *)(p+0x4384)=0;
 }
 *(void **)(s->data+44)=String_New(20,39);*(void **)(s->data+48)=String_New(40,39);*(void **)(s->data+52)=String_New(80,39);
 s->data[0x431a]=0;*(u32 *)(s->data+0x30c)=1;
 ReadMsgDataIntoString(*(void **)(s->data+20),12,*(void **)(s->data+44));ReadMsgDataIntoString(*(void **)(s->data+20),9,*(void **)(s->data+48));
 *(void **)(s->data+0x5c9c)=YesNoPrompt_Create(39);MIi_CpuClearFast(0,(u32 *)(s->data+0x5ca0),5);
}
