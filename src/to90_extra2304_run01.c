#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x113e];u8 state,timer,index;union {u8 raw;struct {u8 active:4,other:4;} bits;} flags;u8 pad2[10];u8 arg;} Work;
extern void BattleBagMenu_DrawButton(Work*,u8,int,u8),BattleBagMenu_OffsetButtonSprite(Work*,u8,int);
extern void BattleBagMenu_ScrollButtonWindows(void*,u8,int);
void BattleBagMenu_UpdateButtonAnim(Work*);
void BattleBagMenu_UpdateButtonAnim(Work *w){if(w->flags.bits.active){switch(w->state){case 0:BattleBagMenu_DrawButton(w,w->index,1,w->arg);BattleBagMenu_ScrollButtonWindows(w,w->index,1);BattleBagMenu_OffsetButtonSprite(w,w->index,1);w->timer=0;w->state=1;break;case 1:BattleBagMenu_DrawButton(w,w->index,2,w->arg);BattleBagMenu_ScrollButtonWindows(w,w->index,2);BattleBagMenu_OffsetButtonSprite(w,w->index,2);w->timer=0;w->state=2;break;case 2:BattleBagMenu_DrawButton(w,w->index,0,w->arg);BattleBagMenu_ScrollButtonWindows(w,w->index,0);BattleBagMenu_OffsetButtonSprite(w,w->index,0);w->timer=0;w->state=0;w->flags.bits.active=0;break;}}}
