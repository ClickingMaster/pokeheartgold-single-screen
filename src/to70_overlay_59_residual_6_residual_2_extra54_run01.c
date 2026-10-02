#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {u8 pad0[0x3e];u16 state;u8 pad40[0xd];u8 index;u8 pad4e[3];volatile u8 count;u8 pad52[0x202];void *sprites[8];} Work;
extern void Sprite_SetAnimCtrlSeq(void*,int);
extern void Sprite_OffsetPositionXY(void*,s16,s16);
int ov59_022390A8(Work*,int);
int ov59_022390A8(Work*w,int active){if(active){switch(w->state){case 0:Sprite_SetAnimCtrlSeq(w->sprites[1],4);w->count=0;w->state++;break;case 1:Sprite_OffsetPositionXY(w->sprites[1],0,2);Sprite_OffsetPositionXY(w->sprites[w->index+2],0,-1);u8 old=w->count;w->count=w->count+1;if(old>=2){Sprite_SetAnimCtrlSeq(w->sprites[1],5);w->count=0;w->state=0;return 1;}break;}}else{switch(w->state){case 0:Sprite_SetAnimCtrlSeq(w->sprites[1],4);w->count=0;w->state++;break;case 1:Sprite_OffsetPositionXY(w->sprites[1],0,-2);u8 old=w->count;w->count=w->count+1;if(old>=3){Sprite_SetAnimCtrlSeq(w->sprites[1],3);w->count=0;w->state=0;return 1;}break;}}return 0;}
