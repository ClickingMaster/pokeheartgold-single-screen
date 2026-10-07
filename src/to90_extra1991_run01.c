#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int n[6];} Capacities;
typedef struct{int heap;u8 pad[0xac-4];void*system,*manager,*icons;u8 pad2[0xfc-0xb8];void*narc;} State;
extern u8 ov113_021E6CB0[],ov113_021E6BA4[],ov113_021E6B74[];
extern Capacities ov113_021E6BB8;
extern void GF_CreateVramTransferManager(int,int);
extern void*SpriteSystem_Alloc(int);
extern void SpriteSystem_Init(void*,void*,void*,int),thunk_ClearMainOAM(int),thunk_ClearSubOAM(int);
extern void*SpriteManager_New(void*);
extern void SpriteSystem_InitSprites(void*,void*,int),sub_0200D2A4(void*,void*,void*,int,int);
extern void SpriteSystem_InitManagerWithCapacities(void*,void*,Capacities*);
extern void*SpriteManager_GetSpriteList(void*);
extern void SpriteManager_SetSpriteList(void*,void*);
extern void*NARC_New(int,int);
extern int sub_02074490(void),sub_02074494(void),sub_020744A0(void),GetMonIconNaixEx(int,int,int);
extern void SpriteSystem_LoadPlttResObjFromOpenNarc(void*,void*,void*,int,int,int,int,int);
extern void SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int);
extern void SpriteSystem_LoadAnimResObj(void*,void*,int,int,int,int);
extern void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int);
void ov113_021E677C(State*);
void ov113_021E677C(State*s){Capacities capacities;int i;
GF_CreateVramTransferManager(32,s->heap);s->system=SpriteSystem_Alloc(s->heap);SpriteSystem_Init(s->system,ov113_021E6CB0,ov113_021E6BA4,2);thunk_ClearMainOAM(s->heap);thunk_ClearSubOAM(s->heap);s->manager=SpriteManager_New(s->system);SpriteSystem_InitSprites(s->system,s->manager,17);sub_0200D2A4(s->system,s->manager,ov113_021E6B74,0,0);
capacities=ov113_021E6BB8;s->icons=SpriteManager_New(s->system);SpriteSystem_InitManagerWithCapacities(s->system,s->icons,&capacities);SpriteManager_SetSpriteList(s->icons,SpriteManager_GetSpriteList(s->manager));s->narc=NARC_New(20,s->heap);
SpriteSystem_LoadPlttResObjFromOpenNarc(s->system,s->icons,s->narc,sub_02074490(),0,1,3,1);SpriteSystem_LoadCellResObjFromOpenNarc(s->system,s->icons,s->narc,sub_02074494(),0,1);SpriteSystem_LoadAnimResObj(s->system,s->icons,20,sub_020744A0(),0,1);
for(i=0;i<7;i++)SpriteSystem_LoadCharResObjFromOpenNarc(s->system,s->icons,s->narc,GetMonIconNaixEx(201,0,i),0,2,i+1);
for(i=7;i<14;i++)SpriteSystem_LoadCharResObjFromOpenNarc(s->system,s->icons,s->narc,GetMonIconNaixEx(201,0,i),0,1,i+1);
}
