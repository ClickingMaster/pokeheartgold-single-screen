#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct {u8 pad[12];int state;u8 pad2[4];u16 code,flag;u8 pad3[0x1e428];void *text;u8 pad4[0xf4];void *sprite;u8 pad5[0xd84];u8 effect[20];u16 active;} Work;
extern void *PokewalkerIr_GetChunkProgress(Work *);extern int PokewalkerIr_GetTotalProgress(void *),ov112_021ED330(Work *,int),PokewalkerIr_GetAndClearError(void),ov112_021E7668(void),PokewalkerApp_GetTouchedButton(int);
extern u32 MTRandom(void);
extern void PokewalkerApp_SpawnRisingParticle(Work *),ov112_021EA60C(Work *),ov112_021ED314(int),ov112_021EDAF4(Work *,int),ov112_021EAA98(Work *,void *,int),ov112_021EAA10(Work *),ManagedSprite_SetDrawFlag(void *,int),sub_02032644(void *),PokewalkerApp_HideSprite(Work *,int),PokewalkerIr_Stop(void),PokewalkerApp_ShowSubBg1(Work *,int),PokewalkerApp_SelectMenuItem(Work *,int,int,int),PlaySE(int),PokewalkerApp_ShowMessageBox(Work *,int,int);
int ov112_021ED35C(Work *);
int ov112_021ED35C(Work *w){
u8 value=ov112_021ED330(w,PokewalkerIr_GetTotalProgress(PokewalkerIr_GetChunkProgress(w)));int code;
if(value>MTRandom()%255)PokewalkerApp_SpawnRisingParticle(w);ov112_021EA60C(w);ov112_021ED314(value);ov112_021EDAF4(w,value);
if(value>200){if(w->active==0){ov112_021EAA98(w,w->effect,2);w->active=1;}}else{ov112_021EAA10(w);w->active=0;}
code=PokewalkerIr_GetAndClearError();switch(code){case 2:if(w->flag==1)w->code=42;break;case 4:case 7:case 8:case 9:case 11:case 12:case 13:w->code=code;break;case 15:ManagedSprite_SetDrawFlag(w->sprite,0);sub_02032644(w->text);PokewalkerApp_HideSprite(w,8);ov112_021EAA98(w,w->effect,0);return 35;}
if(w->code){ManagedSprite_SetDrawFlag(w->sprite,0);w->state=58;PokewalkerIr_Stop();return 32;}
if(ov112_021E7668()==0){PokewalkerApp_ShowSubBg1(w,1);if(PokewalkerApp_GetTouchedButton(5)==0||(gSystem.newKeys&1)||(gSystem.newKeys&2)){w->code=33;PokewalkerIr_Stop();PokewalkerApp_SelectMenuItem(w,3,3,33);PlaySE(1501);return 65;}}
else{if(w->flag==0){PokewalkerApp_ShowMessageBox(w,2,6);w->flag=1;}PokewalkerApp_ShowSubBg1(w,0);}return 25;
}
