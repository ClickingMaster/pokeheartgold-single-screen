#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[4];u16 x,y,a,b;u8 pad2[0x14];u32 stat;} Info;
typedef struct {s16 species;s8 direction;u8 pad[0x15];void *arg2,*arg0,*arg1;} Choice;
typedef struct {u8 pad[0x2c];int state;u8 pad2[0xb54];Choice choice;u8 pad3[0x370];u8 window[16];u8 pad4[0x130];u8 buffer[16];u8 pad5[0x140];void *menu;u8 pad6[0x18];Info *info;u8 pad7[0x6c];u8 output[4];} Work;
extern int ov70_02242144(void *);extern void ClearFrameAndWindow2(void *,int);extern void ov70_02238E50(Work *,int,int);extern void ov70_0223F960(void *,int,int,int);extern u32 GetMonBaseStat(int,int);extern int ov70_0223EDE4(Choice *,u32);extern void ov70_0223F3D8(void *,void *,void *,void *,int,int,int);
int ov70_0223EE10(Work *);
int ov70_0223EE10(Work *w){int result=ov70_02242144(w->menu);switch(result){case -2:ClearFrameAndWindow2(w->window,0);ov70_02238E50(w,5,5);w->state=2;ov70_0223F960(w->output,w->info->y+w->info->x,w->info->b,w->info->a);break;case -1:break;default:{int direction;w->choice.species=result;w->info->stat=GetMonBaseStat(result,18);if(ov70_0223EDE4(&w->choice,w->info->stat)){w->state=10;direction=w->choice.direction;}else{w->state=7;direction=3;}ov70_0223F3D8(w->choice.arg0,w->choice.arg1,w->choice.arg2,w->buffer,w->choice.species,direction,-1);ov70_0223F960(w->output,w->info->y+w->info->x,w->info->b,w->info->a);break;}}return 3;}
