#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,u32);
extern void Heap_Free(void*);
extern void *TradeSequence_GetBgConfig(void*),*PokepicManager_Create(int),*String_New(int,int),*NARC_New(int,int);
extern void ov71_02248120(void*),ov71_022481EC(void*),ov71_0224809C(void*),String_Delete(void*),TradeSequence_DestroyModels(void*),FreeBgTilemapBuffer(void*,int),RemoveWindow(void*),NARC_Delete(void*),Pokepic_Delete(void*),PokepicManager_Delete(void*);
typedef struct {void*owner;int a;void*b,*manager,*pic;u8 pad[0x40];void*bg;u8 win[16];void*s1,*s2,*s3;int d,e,f;void*narc;} Work;
Work*ov71_022478C8(void*p);
void ov71_02247924(Work*w);
Work*ov71_022478C8(void*p){Work*w=Heap_Alloc(57,132);if(w){w->owner=p;w->a=0;w->bg=TradeSequence_GetBgConfig(p);w->manager=PokepicManager_Create(57);w->pic=0;w->s1=String_New(300,57);w->s2=String_New(300,57);w->e=0;w->f=0;w->narc=NARC_New(180,57);}return w;}
void ov71_02247924(Work*w){if(w){ov71_02248120(w);ov71_022481EC(w);ov71_0224809C(w);String_Delete(w->s1);String_Delete(w->s2);TradeSequence_DestroyModels(w->s3);FreeBgTilemapBuffer(w->bg,1);FreeBgTilemapBuffer(w->bg,2);FreeBgTilemapBuffer(w->bg,6);RemoveWindow(w->win);NARC_Delete(w->narc);if(w->pic)Pokepic_Delete(w->pic);PokepicManager_Delete(w->manager);Heap_Free(w);}}
