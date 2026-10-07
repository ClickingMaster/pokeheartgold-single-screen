#pragma force_active on
#include "global.h"
typedef struct{u8 a[16];void*sprites[2];int x,value;u8 gap[0x90-0x20];int active;} Work;extern void Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetPalIndexRespectVramOffset(void*,int);

void ov41_0224B298(Work*w);

#pragma force_active on
#pragma opt_treetransformation off
void ov41_0224B298(Work*w){struct {volatile int value,digit;} state;state.value=w->value;int divisor=1;int i;Work*cursor;divisor*=10;i=0;cursor=w;for(;i<2;i++){int quotient=state.value/divisor;state.digit=quotient;if(quotient>10)GF_AssertFail();Sprite_SetAnimCtrlSeq(cursor->sprites[0],state.digit);u32 consumed=(u32)state.digit*(u32)divisor;state.value=(s32)((u32)state.value-consumed);divisor/=10;if(w->active==0 && w->value<=10){Sprite_SetPalIndexRespectVramOffset(cursor->sprites[0],1);if(i==1)w->active=1;}cursor=(Work*)((u8*)cursor+4);}}
