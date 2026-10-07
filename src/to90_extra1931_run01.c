#pragma force_active on
#include "global.h"
typedef struct {u16 x,y,valid,pad;} Sample;typedef struct {u32 x,y,pressed,held;u8 pad[2];Sample samples[4];} W;extern void sub_02021280(void *,int,int);void ov85_021E74AC(void *);
void ov85_021E74AC(void *w){int i=3;int held=0;W *p=(W*)((u8*)w+0xd2c);sub_02021280((u8*)p+16,2,0);{u8 *cursor=(u8*)p+0x18;u32 marker=1;do{u32 valid=*(u16*)(cursor+0x16);if(valid==1){p->x=*(u16*)(cursor+0x12);p->y=*(u16*)(cursor+0x14);held=marker;}cursor-=8;i--;}while(i>=0);}p->pressed=(p->held^held)&held;p->held=held;}
