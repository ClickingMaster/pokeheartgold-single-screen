#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Win {u32 v[4];} Win;
typedef struct Root {void *dex,*profile;} Root;
typedef struct Work {Root *root;u8 pad4[8];Win win[13];u8 padDC[0x580];void *msg;u8 pad660[0x1242];u16 species;} Work;
extern const u8 ov18_021F9DE4[];
extern void ov18_021EE35C(Work*,const void*,int);
extern void FillWindowPixelBuffer(Win*,u8);
extern int Pokedex_CheckMonCaughtFlag(void*,u16);
extern void ov18_021F9648(Win*,void*,int,int,int,int,int,int);
extern void *ov18_021E590C(u16,int,int);
extern void ov18_021F95FC(Win*,void*,int,int,int,int,int);
extern void String_Delete(void*);
extern void *PlayerProfile_GetPlayerName_NewString(void*,int);
extern int PlayerProfile_GetTrainerGender(void*);
extern void ov18_021EEA84(Work*,u16,int,int,int,int,int,int);
extern void ov18_021EEB34(Work*,u16,int,int,int,int,int,int);
extern void ScheduleWindowCopyToVram(Win*);
void ov18_021F05E8(Work*);
void ov18_021F05E8(Work *w){ov18_021EE35C(w,ov18_021F9DE4,13);for(u32 i=0;i<13;i++)FillWindowPixelBuffer(&w->win[i],0);int kind=Pokedex_CheckMonCaughtFlag(w->root->dex,w->species)?2:1;ov18_021F9648(&w->win[0],w->msg,0x8f,0,0,4,0x20100,0);ov18_021F9648(&w->win[3],w->msg,0x88,0x30,0,0,0x20100,2);ov18_021F9648(&w->win[4],w->msg,0xa,0x10,0,0,0x20100,2);ov18_021F9648(&w->win[6],w->msg,0xa,0x10,0,0,0x20100,2);void *str=ov18_021E590C(w->species,2,0x25);ov18_021F95FC(&w->win[1],str,0x20,0,0,0x20100,2);String_Delete(str);str=PlayerProfile_GetPlayerName_NewString(w->root->profile,0x25);ov18_021F95FC(&w->win[2],str,0x20,0,0,0x20100,2);String_Delete(str);ov18_021EEA84(w,w->species,kind,5,0x20,0,0x20100,2);ov18_021F9648(&w->win[8],w->msg,0x89,0x30,0,0,0x50900,2);ov18_021F9648(&w->win[9],w->msg,0xb,0x10,0,0,0xf0500,2);ov18_021F9648(&w->win[11],w->msg,0xb,0x10,0,0,0xf0500,2);ov18_021EEB34(w,w->species,kind,10,0x20,0,0xf0500,2);if(PlayerProfile_GetTrainerGender(w->root->profile)==0){ov18_021F9648(&w->win[7],w->msg,0x8a,0x20,0,0,0x20100,2);ov18_021F9648(&w->win[12],w->msg,0x8c,0x20,0,0,0xf0500,2);}else{ov18_021F9648(&w->win[7],w->msg,0x8b,0x20,0,0,0x20100,2);ov18_021F9648(&w->win[12],w->msg,0x8d,0x20,0,0,0xf0500,2);}for(u32 i=0;i<13;i++)ScheduleWindowCopyToVram(&w->win[i]);}
