#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[0xc0];void*task;void*entries[8];int*state;int stop,dirty,timer,busy;}W;
extern void ov71_02246F60(W*,int),ov71_02246EAC(W*),SysTask_Destroy(void*),Heap_Free(void*),PlaySE(int);
void ov71_02246D54(W*);void ov71_02246D9C(void*,W*);
void ov71_02246D54(W*w){if(*w->state==1){int i;for(i=0;i<8;i++){if(w->entries[i])ov71_02246F60(w,i);}*w->state=0;SysTask_Destroy(w->task);Heap_Free(w);}}
void ov71_02246D9C(void*task,W*w){if(w->stop==0){w->timer--;if(w->timer<=0){PlaySE(0x6ad);w->timer=30;ov71_02246EAC(w);}if(w->dirty){GX_BeginLoadBGExtPltt();GXS_BeginLoadBGExtPltt();DC_FlushRange(w,96);GX_LoadBGExtPltt(w,0x6000,96);GXS_LoadBGExtPltt(w,0x6000,96);GX_EndLoadBGExtPltt();GXS_EndLoadBGExtPltt();w->dirty=0;}}else{if(w->dirty){GX_BeginLoadBGExtPltt();GXS_BeginLoadBGExtPltt();DC_FlushRange(w,96);GX_LoadBGExtPltt(w,0x6000,96);GXS_LoadBGExtPltt(w,0x6000,96);GX_EndLoadBGExtPltt();GXS_EndLoadBGExtPltt();w->dirty=0;}if(w->busy==0){*w->state=0;Heap_Free(w);SysTask_Destroy(task);}}}
