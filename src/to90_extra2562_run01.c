#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "player_data.h"
#include "message_format.h"
#include "yes_no_prompt.h"
#include "save.h"
#include "screen_fade.h"
#include "touchscreen.h"
#pragma opt_pointer_analysis off
#pragma force_active on
typedef struct State{u32 mode,timer;Window primaryWindow;void *screenData;void *dimensions;Window messageWindow;u32 unused30,printer,speed;String *text;} State;
typedef struct UI{u32 touchSpeed;SaveData *save;u8 state;u8 pad9[0x4f];String *name,*number;void *prompt;u32 unused64;void *arguments;} UI;
extern void ov43_0222B1FC(u8*,u8*,u8*);
extern int ov43_0222B374(u32*,u32*,u8*),ov43_0222B574(int*,void*),ov43_0222B534(u8*,u32*);
extern void ov43_0222B440(u32*,u8*),ov43_0222B458(u8*,u8*),ov43_0222B4BC(u8*,u8*,int,int);
extern void Heap_Free(void*);
typedef struct {u8 pad[0x50];void *format;} FormatWork;
extern void ov43_0222AAA4(FormatWork*,u64);
typedef struct {BgConfig *bg;u8 pad[0x5c];YesNoPrompt *prompt;} Config;
extern void ov43_0222ADB8(int*,Config*,int);
typedef struct {u32 pad;void *save;} SaveWork;
extern int ov43_0222AE2C(u32*,u8*),ov43_0222A358(SaveWork*,void*,void*);
typedef struct {BgConfig *bg;} BgWork;
extern void ov43_0222B55C(u8*),ov43_0222B408(void*,BgWork*);
extern const u8 ov43_0222F0C8[];

int ov43_0222AE64(void*,void*,void*,u32);
int ov43_0222AE64(void *state,void *shared,void *base,u32 heap){State *p=state;UI *ui=shared;u8 *ctx=base;
 int valid;
 switch(ui->state){
 case 0:ov43_0222B1FC((u8*)p,(u8*)ui,ctx);BeginNormalPaletteFade(FADE_BOTH_SCREENS,(enum FadeType)17,(enum FadeType)17,0,6,1,(enum HeapID)heap);ui->state=1;break;
 case 1:if(IsPaletteFadeFinished())ui->state=3;break;
 case 2:ov43_0222B1FC((u8*)p,(u8*)ui,ctx);ui->state=3;break;
 case 3:valid=ov43_0222B374(&p->mode,&ui->touchSpeed,ctx);if(valid){p->timer=2;ui->state=4;}break;
 case 4:{u32 before=p->timer;p->timer=before-1;if(before==0){p->timer=0;if(ov43_0222B574((int*)&p->mode,ui)==1){ov43_0222B440(&p->mode,(u8*)ui);if(p->mode==3 || p->mode==1)ui->state=5;else{ov43_0222B458((u8*)p,ctx);return 1;}}else if(p->mode==1){ov43_0222B4BC((u8*)p,ctx,0x3a,heap);ui->state=11;}}}break;
 case 5:BeginNormalPaletteFade(FADE_BOTH_SCREENS,(enum FadeType)16,(enum FadeType)16,0,6,1,(enum HeapID)heap);ui->state++;break;
 case 6:if(IsPaletteFadeFinished()==1)return 1;break;
 case 7:ov43_0222B1FC((u8*)p,(u8*)ui,ctx);BeginNormalPaletteFade(FADE_BOTH_SCREENS,(enum FadeType)17,(enum FadeType)17,0,6,1,(enum HeapID)heap);ui->state=8;break;
 case 8:if(IsPaletteFadeFinished()){
  PlayerProfile *profile=PlayerProfile_New((enum HeapID)heap);u64 number;
  Save_Profile_PlayerName_Set(profile,String_cstr(ui->name));BufferPlayersName(*(void **)(ctx+0x50),0,profile);Heap_Free(profile);
  number=String_atoi(ui->number,&valid);
  if(valid){ov43_0222AAA4((FormatWork*)ctx,number);ov43_0222B4BC((u8*)p,ctx,0x37,heap);ui->state=9;}
  else{ov43_0222B4BC((u8*)p,ctx,0x38,heap);ui->state=11;}
 }break;
 case 9:if(ov43_0222B534((u8*)p,&ui->touchSpeed)){ov43_0222ADB8((int*)&ui->touchSpeed,(Config*)ctx,0);ui->state=10;}break;
 case 10:switch(ov43_0222AE2C(&ui->touchSpeed,ctx)){
  case 1:{int ret=ov43_0222A358((SaveWork*)ui,ui->number,ui->name);if(ret==0){ov43_0222B55C((u8*)p);ui->state=3;}else{ov43_0222B4BC((u8*)p,ctx,ov43_0222F0C8[ret],heap);ui->state=11;}break;}
  case 2:ov43_0222B55C((u8*)p);ui->state=3;break;
 }break;
 case 11:if(ov43_0222B534((u8*)p,&ui->touchSpeed) && ((gSystem.newKeys&1)||(gSystem.newKeys&2)||System_GetTouchNew())){ov43_0222B55C((u8*)p);ov43_0222B408(p,(BgWork*)ctx);ui->state=3;}break;
 }
 return 0;
}
