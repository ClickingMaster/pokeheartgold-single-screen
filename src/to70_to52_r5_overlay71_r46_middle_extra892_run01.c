#pragma force_active on
#include "global.h"
typedef struct{void *mon;u8 p[12];void *mgr;u8 p2[4];u8 script[0x6c];void *narc;} Work;
extern void *ov71_02247398(void *);
extern int ov71_022473BC(void *);extern u8 sub_02070854(void *,u8,int);
extern void GetBoxmonSpriteCharAndPlttNarcIds(void *,void *,int,int),NARC_ReadPokepicAnimScript(void *,void *,int,int);
extern void *PokepicManager_CreatePokepic(void *,void *,int,int,int,int,void *,int);
void *ov71_0224B7EC(Work *);
void *ov71_0224B7EC(Work *w){u32 ids[4];int y;void *mon=ov71_02247398(w->mon);GetBoxmonSpriteCharAndPlttNarcIds(ids,mon,2,0);NARC_ReadPokepicAnimScript(w->narc,w->script,ov71_022473BC(w->mon),1);y=sub_02070854(mon,2,0)+80;return PokepicManager_CreatePokepic(w->mgr,ids,128,y,0,0,w->script,0);}
