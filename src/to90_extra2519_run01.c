#pragma force_active on
#include "global.h"
#include "filesystem.h"
#include "battle_controller_opponent_r2_exact_private.h"
#include "bg_window.h"
#pragma force_active on
typedef BattleMessageVector Vec;
typedef struct {u8 a,b;u16 c;u32 d;u16 e,f;u32 g;u16 h,i,j,k;u8 pad[0x94];void *pAC,*pB0[4];u8 bC0[4];void *pC4[4];u32 pD4;u16 sD8[4];u8 bE0[4],bE4[4],bE8[4];u32 pEC[4],pFC[4];void *last[3];u32 flag0:1,flag1:1,flag2:1,rest:29;} Dest;
typedef struct {u32 a;u8 c,d;u16 e;void*f;} Slot;
struct BattleEngineWork {enum HeapID heap;NarcId archive;u8 p[8];int ready;void*file,*file2;u8 q[12];Slot slots[3];u8 r[0x20];u8 enabled[16],zero[16],s[8];u32 items[10];void(*callback)(void);Dest *dest;BgConfig *bg;PaletteData *other;u8 t[0x10];u32 more[10];u8 u[0x38];u32 small[5],small2[5];u8 v[0x1c];u32 sprites[4];Vec vector;u32 mode;u8 priorities[4];};typedef BattleEngineWork Work;
typedef BattleSourceR1 Info;
typedef BattleParamsR1 Params;
typedef struct {u32 v[25];} Map;
extern Map ov07_02234C64;extern void ov07_0221C69C(void),ov07_0221BE44(void);extern int ov07_0221C3DC(Work*);int ov07_0221C01C(Work*,Info*,u16,Params*);
int ov07_0221C01C(Work*w,Info*info,u16 incomingId,Params*p){Map map;int i;int id=incomingId;ov07_0221C69C();if(!ov07_0221C3DC(w))return 0;for(i=0;i<16;i++){w->enabled[i]=1;w->zero[i]=0;}for(i=0;i<10;i++)w->items[i]=0;for(i=0;i<3;i++){w->slots[i].f=0;w->slots[i].a=0;w->slots[i].c=0;w->slots[i].d=0;}w->dest->a=info->opcode;w->dest->b=info->byte1;w->dest->c=info->unk2;w->dest->d=info->value04;w->dest->e=info->value08;w->dest->f=info->value0C;w->dest->g=info->value10;w->dest->h=info->value0A;w->dest->i=info->terrain54;w->dest->j=info->value14;w->dest->k=info->value16;w->dest->flag0=info->flag0;w->dest->flag1=info->flag1;w->dest->flag2=info->flag2;w->dest->pAC=p->spriteSystem;if(!p->spriteSystem)GF_AssertFail();w->bg=p->bgConfig;w->other=p->paletteData;w->dest->pD4=p->battleType;for(i=0;i<4;i++){w->dest->pB0[i]=p->battlerValues[i];w->dest->bC0[i]=p->mapping[i];w->dest->pC4[i]=p->objects[i];w->dest->sD8[i]=p->values34[i];w->dest->bE0[i]=p->values3C[i];w->dest->bE4[i]=p->values40[i];w->dest->bE8[i]=p->values44[i];w->dest->pEC[i]=p->values48[i];w->dest->pFC[i]=p->values58[i];}w->vector=p->vector;w->dest->last[0]=p->chatot;w->dest->last[2]=p->bad8;w->dest->last[1]=p->bad0;if(id==290){map=ov07_02234C64;if(w->dest->i>=24)id=161;else id=map.v[w->dest->i];}if(id==0||id>467)id=1;w->archive=p->value68;w->file=AllocAndReadWholeNarcMemberByIdPair(w->archive,id,w->heap);if(!w->file){if(!w->file)GF_AssertFail();return 0;}w->file2=w->file;w->priorities[0]=GetBgPriority(w->bg,0);w->priorities[1]=GetBgPriority(w->bg,1);w->priorities[2]=GetBgPriority(w->bg,2);w->priorities[3]=GetBgPriority(w->bg,3);for(i=0;i<10;i++)w->more[i]=0;for(i=0;i<5;i++){w->small[i]=0;w->small2[i]=0;}for(i=0;i<4;i++)w->sprites[i]=0;w->callback=ov07_0221BE44;((u8*)w)[0x8d]=0;w->mode=255;w->ready=1;return 1;}
