#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
typedef struct {u8 pad[12];int state;u32 pad10;u16 code,entered;u8 pad18[0x1e520];void *sprite;} View;
void PokewalkerIr_GetChunkProgress(void),PokewalkerIr_Stop(void);
int PokewalkerIr_GetTotalProgress(void),PokewalkerIr_GetAndClearError(void),ov112_021E7668(void);
void PokewalkerApp_SpawnFallingParticle(View*),ov112_021EA60C(View*),ov112_021ED530(View*),ov112_021ED5DC(View*);
void ov112_021ED314(int);
void PokewalkerApp_ShowSprite(View*,int),PokewalkerApp_ShowSubBg1(View*,int),PokewalkerApp_ShowMessageBox(View*,int,int),PokewalkerApp_SelectMenuItem(View*,int,int,int);
BOOL PokewalkerApp_GetTouchedButton(int);
void ManagedSprite_SetDrawFlag(void*,BOOL);
void PlaySE(u16);
void RTC_GetDateTime(RTCDate*,RTCTime*);
int ov112_021ED918(View*);
int ov112_021ED918(View *v){
 int changed,code;RTCDate date;RTCTime time;
 PokewalkerIr_GetChunkProgress();changed=PokewalkerIr_GetTotalProgress();if(changed)PokewalkerApp_SpawnFallingParticle(v);
 ov112_021ED314(changed);ov112_021EA60C(v);code=PokewalkerIr_GetAndClearError();
 switch(code){
 case 2:if(v->entered==1)v->code=42;break;
 case 4:case 5:case 7:case 8:case 9:case 11:case 12:case 13:v->code=code;break;
 case 15:
  ManagedSprite_SetDrawFlag(v->sprite,0);RTC_GetDateTime(&date,&time);RTC_ConvertDateTimeToSecond(&date,&time);
  ov112_021ED530(v);ov112_021ED5DC(v);PokewalkerApp_ShowSprite(v,6);return 41;
 }
 if(v->code){ManagedSprite_SetDrawFlag(v->sprite,0);v->state=61;PokewalkerIr_Stop();return 32;}
 if(!ov112_021E7668()){
  PokewalkerApp_ShowSubBg1(v,1);
  if(!PokewalkerApp_GetTouchedButton(5) || (gSystem.newKeys&1) || (gSystem.newKeys&2)){
   PokewalkerIr_Stop();v->code=33;v->state=66;PokewalkerApp_SelectMenuItem(v,3,3,33);PlaySE(1501);return 65;
  }
 }else{
  if(!v->entered){PokewalkerApp_ShowMessageBox(v,2,6);v->entered=1;}
  PokewalkerApp_ShowSubBg1(v,0);
 }
 return 29;
}
