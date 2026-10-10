#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct{u8 p[0x31c];void *a,*b;u8 p2[0xd8];u32 state;}W;
extern void ManagedSprite_SetDrawPriority(void*,int),ManagedSprite_SetAnim(void*,int),ManagedSprite_SetDrawFlag(void*,int),ov57_02239728(void*,int,int,int),ov57_0223B12C(W*),ov57_0223B828(W*,int,int,int),EraseFrameAndWindow1(void*,int),ClearWindowTilemapAndScheduleTransfer(void*),ov57_0223866C(W*,int),ov57_02238FC4(W*),ov57_02239558(W*);
extern int ov57_0223B308(W*),System_GetTouchNew(void);
int ov57_0223A5AC(W*);
int ov57_0223A5AC(W*w){switch(w->state){case 0:ManagedSprite_SetDrawPriority(w->a,25);ManagedSprite_SetDrawPriority(w->b,20);ManagedSprite_SetAnim(w->a,1);ov57_02239728((u8*)w+0x11c,3,10,0);ManagedSprite_SetDrawFlag(w->b,1);w->state++;case 1:w->state=ov57_0223B308(w);break;case 2:ov57_0223B12C(w);if((gSystem.newKeys&0xcf3)||System_GetTouchNew())w->state=3;break;case 3:ManagedSprite_SetDrawPriority(w->a,20);ManagedSprite_SetDrawPriority(w->b,25);ov57_0223B828(w,0,255,0);ManagedSprite_SetAnim(w->a,0);EraseFrameAndWindow1((u8*)w+0xec,1);ClearWindowTilemapAndScheduleTransfer((u8*)w+0xec);ov57_0223866C(w,1);ov57_02239728((u8*)w+0x11c,3,7,0);break;}ov57_02238FC4(w);ov57_02239558(w);return 1;}
