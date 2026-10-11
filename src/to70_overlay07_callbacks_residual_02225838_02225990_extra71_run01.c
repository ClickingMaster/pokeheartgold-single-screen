#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Flow {u8 state,pad1[3];void *owner;u8 pad8[0x1c];void *pic;u8 pad28[0x1a];s16 delta;int interval,direction,count,y,amount;} Flow;
extern void Pokepic_SetAttr(void*,int,int),ov07_02223224(void*),BattleAnimSystem_DestroyTask(void*,void*),Heap_Free(void*);
void ov07_02225904(void*,Flow*);
void ov07_02225904(void*task,Flow*w){if(w->state==0){w->count++;if(w->count>=w->interval){w->count=0;if(w->direction==0){if(w->amount<0)w->amount=0;if(w->amount==0)w->state++;}else{if(w->amount>80)w->amount=80;if(w->amount==80)w->state++;}Pokepic_SetAttr(w->pic,18,w->amount);Pokepic_SetAttr(w->pic,1,w->y);w->y=w->y-w->delta;w->amount=w->amount+w->delta;}}else{ov07_02223224(w->pic);BattleAnimSystem_DestroyTask(w->owner,task);Heap_Free(w);}}
