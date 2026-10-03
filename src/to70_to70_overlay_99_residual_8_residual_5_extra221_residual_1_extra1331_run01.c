#pragma force_active on
#include "global.h"
typedef struct{u32 maximum:5,pad:9,count:5,pad2:8,reverse:1,changed:1,pad3:3;} Bits;
typedef struct {u8 pad[0x3f4];Bits bits;u32 unused;void*input;u8 p2[8];void*sprite;} Work;
void PlaySE(int);
int GridInputHandler_GetNextInput(void*);
void GridInputHandler_SetNextInput(void*,int);
void ov99_021E7C58(Work*,int);
void ManagedSprite_SetDrawFlag(void*,int);
void ov99_021E7AD4(Work*,int,int);
void ov99_021E7AD4(Work*w,int up,int special){u32 old=w->bits.count;int v;
if(up)w->bits.count=old+1;else if(old)w->bits.count=old-1;
w->bits.count=w->bits.count>w->bits.maximum?w->bits.maximum:w->bits.count;
if(w->bits.count!=old){PlaySE(1500);w->bits.reverse=!up;w->bits.changed=1;{u8 next=0;v=GridInputHandler_GetNextInput(w->input);
if(special){if(v==30)next=v;}else{v=(u8)v;if(up)next=v-5;else next=v+5;}
GridInputHandler_SetNextInput(w->input,next);ov99_021E7C58(w,next);ManagedSprite_SetDrawFlag(w->sprite,0);}}}
