#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Motion{int value;u8 pad[32];}Motion;typedef struct Work{u8 state,side,index,pad3;void*owner;u8 pad8[32];void*pic;u8 pad2c[48];Motion motion;}Work;typedef struct Pair{int value,duration;}Pair;
extern Pair ov07_0223674C[],ov07_02236750[];extern void BattleAnimUtil_InitValueMotion(Motion*,int,int,int),Pokepic_SetAttr(void*,int,int),BattleAnimSystem_DestroyTask(void*,void*),Heap_Free(void*);extern int BattleAnimUtil_UpdateValueMotion(Motion*);void ov07_02228E70(void*,Work*);
void ov07_02228E70(void*parent,Work*w){switch(w->state){case 0:{int from,to;if(w->side==0){to=ov07_0223674C[w->index].value;from=-to;}else{from=ov07_0223674C[w->index].value;to=-from;}BattleAnimUtil_InitValueMotion(&w->motion,from,to,ov07_02236750[w->index].value);if(w->side==1)w->index++;w->side^=1;w->state++;}case 1:if(BattleAnimUtil_UpdateValueMotion(&w->motion)==1){Pokepic_SetAttr(w->pic,9,(u16)w->motion.value);return;}if(w->index>=5)w->state++;else w->state--;return;default:Pokepic_SetAttr(w->pic,9,0);BattleAnimSystem_DestroyTask(w->owner,parent);Heap_Free(w);return;}}
