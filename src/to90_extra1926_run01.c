#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u64 pad,current;u8 data[0xd8];u64 saved;}Block;typedef struct{u8 gap[0xe4];u8 saved[8];}View;
typedef struct{void*save;} Work;
extern Block*ov39_0222A2B4(Work*);
extern u16 SaveArray_CalcCRC16(void*,void*,u32);
u32 ov39_022280D4(Work*,u8**);
u32 ov39_022280D4(Work*w,u8**out){Block*b=ov39_0222A2B4(w);View*v=(View*)((u8*)b+4);*out=(u8*)((u32)v+12);if(*(u64*)v->saved!=b->current){*(u64*)v->saved=b->current;*(u16*)(*out+0xe0)=SaveArray_CalcCRC16(w->save,*out+0x80,0x58);}return b->current;}
