#pragma force_active on
#include "global.h"
typedef struct{u32 unused;u8 mode;} Settings;
typedef struct{Settings *settings;u8 p4[0xd4];u8 resource[16],object[64];} Work;
extern void ModelResource_LoadFromNarc(void *,int,int,int);extern void ModelInstance_Init(void *,void *);extern void ModelInstance_SetPosition(void *,int,int,int);extern void ModelInstance_SetScale(void *,int,int,int);extern void ModelInstance_SetVisible(void *,int);
void ov89_02259BAC(Work *,int);
void ov89_02259BAC(Work *w,int arg){int id=w->settings->mode==0?28:27;ModelResource_LoadFromNarc(w->resource,arg,id,125);ModelInstance_Init(w->object,w->resource);ModelInstance_SetPosition(w->object,0,0,0);ModelInstance_SetScale(w->object,4096,4096,4096);ModelInstance_SetVisible(w->object,1);}
