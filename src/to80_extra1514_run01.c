#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad0[0x1f];u8 a,pad20,b;u8 pad22[0x12];void *state;} Work;
extern void *ov14_021E60C0(Work*,int,int);
extern void BufferBoxMonNickname(void*,int,void*);
extern void ov14_021F6698(Work*,int,void*);
void ov14_021F67B0(Work*,int,void*);
void ov14_021F67B0(Work *work,int mode,void *argument) {int value;switch(mode){case 0:value=2;break;case 1:value=3;BufferBoxMonNickname(*(void**)((u8*)work->state+0x24),0,ov14_021E60C0(work,work->a,work->b));break;case 2:value=4;BufferBoxMonNickname(*(void**)((u8*)work->state+0x24),0,ov14_021E60C0(work,work->a,work->b));break;case 3:value=31;break;case 4:value=32;BufferBoxMonNickname(*(void**)((u8*)work->state+0x24),0,ov14_021E60C0(work,work->a,work->b));break;case 5:value=33;break;case 6:value=6;break;default:break;}ov14_021F6698(work,value,argument);}
