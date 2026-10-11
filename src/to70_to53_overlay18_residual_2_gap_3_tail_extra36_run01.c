#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Win {u32 v[4];} Win;
typedef struct Root {void *dex,*profile;} Root;
typedef struct Work {Root *root;u8 pad4[8];Win win[13];u8 padDC[0x580];void *msg;u8 pad660[0x1242];u16 species;} Work;
extern const u8 sSizeCheckWindowTemplates[];
extern void PokedexApp_AddWindows(Work*,const void*,int);
extern void FillWindowPixelBuffer(Win*,u8);
extern int Pokedex_CheckMonCaughtFlag(void*,u16);
extern void PokedexApp_PrintMessage(Win*,void*,int,int,int,int,int,int);
extern void *ov18_021E590C(u16,int,int);
extern void PokedexApp_PrintString(Win*,void*,int,int,int,int,int);
extern void String_Delete(void*);
extern void *PlayerProfile_GetPlayerName_NewString(void*,int);
extern int PlayerProfile_GetTrainerGender(void*);
extern void PokedexApp_PrintHeight(Work*,u16,int,int,int,int,int,int);
extern void PokedexApp_PrintWeight(Work*,u16,int,int,int,int,int,int);
extern void ScheduleWindowCopyToVram(Win*);
void PokedexApp_SetupSizeCheckWindows(Work*);
void PokedexApp_SetupSizeCheckWindows(Work *w){PokedexApp_AddWindows(w,sSizeCheckWindowTemplates,13);for(u32 i=0;i<13;i++)FillWindowPixelBuffer(&w->win[i],0);int kind=Pokedex_CheckMonCaughtFlag(w->root->dex,w->species)?2:1;PokedexApp_PrintMessage(&w->win[0],w->msg,0x8f,0,0,4,0x20100,0);PokedexApp_PrintMessage(&w->win[3],w->msg,0x88,0x30,0,0,0x20100,2);PokedexApp_PrintMessage(&w->win[4],w->msg,0xa,0x10,0,0,0x20100,2);PokedexApp_PrintMessage(&w->win[6],w->msg,0xa,0x10,0,0,0x20100,2);void *str=ov18_021E590C(w->species,2,0x25);PokedexApp_PrintString(&w->win[1],str,0x20,0,0,0x20100,2);String_Delete(str);str=PlayerProfile_GetPlayerName_NewString(w->root->profile,0x25);PokedexApp_PrintString(&w->win[2],str,0x20,0,0,0x20100,2);String_Delete(str);PokedexApp_PrintHeight(w,w->species,kind,5,0x20,0,0x20100,2);PokedexApp_PrintMessage(&w->win[8],w->msg,0x89,0x30,0,0,0x50900,2);PokedexApp_PrintMessage(&w->win[9],w->msg,0xb,0x10,0,0,0xf0500,2);PokedexApp_PrintMessage(&w->win[11],w->msg,0xb,0x10,0,0,0xf0500,2);PokedexApp_PrintWeight(w,w->species,kind,10,0x20,0,0xf0500,2);if(PlayerProfile_GetTrainerGender(w->root->profile)==0){PokedexApp_PrintMessage(&w->win[7],w->msg,0x8a,0x20,0,0,0x20100,2);PokedexApp_PrintMessage(&w->win[12],w->msg,0x8c,0x20,0,0,0xf0500,2);}else{PokedexApp_PrintMessage(&w->win[7],w->msg,0x8b,0x20,0,0,0x20100,2);PokedexApp_PrintMessage(&w->win[12],w->msg,0x8d,0x20,0,0,0xf0500,2);}for(u32 i=0;i<13;i++)ScheduleWindowCopyToVram(&w->win[i]);}
