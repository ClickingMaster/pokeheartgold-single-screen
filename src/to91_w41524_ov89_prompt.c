#include "global.h"
#include "system.h"
#include "text.h"
#include "render_window.h"
#include "msgdata.h"
#include "player_data.h"
#include "yes_no_prompt.h"
#include "unk_02005D10.h"
#include "sprite_system.h"
#include "overlay_manager.h"
#include "unk_0200FA24.h"
typedef struct{void *network;u8 column;u8 padding[3];} OwnerPrefix;
typedef struct Ov89PartyEntry {u16 value;u16 species;u32 personality;u8 form;u8 padding[3];} Entry;
typedef struct{OwnerPrefix *owner;SaveData *save;BgConfig *bg;PaletteData *palette;u8 p10[0xc];SpriteSystem *sys;SpriteManager *manager;YesNoPrompt *prompt;u8 promptActive,timer;u8 p2a[6];MsgData *messages;u8 p34[0x80];Window window;String *string;u8 printer,padC9[3];void*resource;u8 pD0[0x98];u32 active;void *network;u8 p170[0x20];int ticks;u8 animation[0x744];Entry entries[6];u8 selected,paletteSlot,p922[0x970-0x922];ManagedSprite *sprites[3];u8 p97c[0x9bc-0x97c];int phase;u8 remainder[0x19e0-0x9c0];void *networkOwner;} State;

typedef char EntrySize18[(sizeof(Entry)==12)?1:-1];
typedef char StateSize18[(sizeof(State)==0x19e4)?1:-1];

void ov89_0225A4F4(void*,void*,u32,u32);int ov89_0225C8DC(void*);void ov89_02259E28(void*,u8*);
BOOL ov89_0225A41C(void*,s32);BOOL ov89_02259EC4(u8*,Entry*,void*);BOOL ov89_0225C91C(void*);u32 ov89_0225C84C(const void*,int);void ov89_0225A16C(void*,int);




void ov45_0222EEB8(void);
void ov89_0225A21C(void*);void ov89_0225A398(void*);void ov89_02259F9C(void*);int ov45_0222D844(void);typedef struct Overlay45State Overlay45State;u32 ov45_0222A330(Overlay45State*);void ov45_0222A4A8(u8*);void ov89_0225A468(void*);
int ov89_02258B04(OverlayManager*,int*);
#pragma force_active on
typedef struct {u16 a,b;u32 c;u8 d,padding[3];} Scratch12;
int ov89_02258B04(OverlayManager*manager,int*state){State*d=OverlayManager_GetData(manager);Scratch12 scratch;YesNoPromptTemplate tmpl;
 ov89_0225A4F4(d->animation,d->resource,d->phase,d->owner->column);
 switch(*state){
 case 0:d->active=1;ov89_0225C8DC(d);ov89_02259E28(d,(u8*)&scratch);(*state)++;break;
 case 1:if(IsPaletteFadeFinished()==1){d->phase=1;(*state)++;}break;
 case 2:{
 int result;
 if(gSystem.touchNew&&gSystem.touchY<0xa0&&ov89_0225A41C(d,0x555)==1){
 d->entries[d->selected].padding[0]=gSystem.touchX;d->entries[d->selected].padding[1]=gSystem.touchY;
 ov89_02259EC4((u8*)d,&d->entries[d->selected],d->network);ov89_0225C91C(&d->entries[d->selected]);
 }
 result=(int)ov89_0225C84C(d->entries,d->selected);
 if(result<6){d->selected=result;ov89_0225A16C(d,result);}
 else if(*state==2&&result==0xfe){PlaySE(0x5e5);PaletteData_BlendPalette(d->palette,(PaletteBufferId)0,9,1,8,0);PaletteData_BlendPalette(d->palette,(PaletteBufferId)2,(u16)(d->paletteSlot<<4),16,8,0);*state=3;}
 break;}
 case 3:FillWindowPixelBuffer(&d->window,15);DrawFrameAndWindow2(&d->window,0,1,14);ReadMsgDataIntoString(d->messages,2,d->string);d->printer=AddTextPrinterParameterized(&d->window,1,d->string,0,0,Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(d->save)),NULL);(*state)++;break;
 case 4:if(!TextPrinterCheckActive(d->printer)){tmpl.bgConfig=d->bg;tmpl.bgId=1;tmpl.tileStart=31;tmpl.plttSlot=11;tmpl.x=25;tmpl.y=6;tmpl.shapeParam=0;YesNoPrompt_InitFromTemplate(d->prompt,&tmpl);d->promptActive=1;(*state)++;}break;
 case 5:PaletteData_LoadPaletteSlotFromHardware(d->palette,(PaletteBufferId)0,0xb0,0x40);switch(YesNoPrompt_HandleInput(d->prompt)){case 1:YesNoPrompt_Reset(d->prompt);d->promptActive=0;ClearFrameAndWindow2(&d->window,FALSE);d->active=0;d->phase=3;*state=9;break;case 2:YesNoPrompt_Reset(d->prompt);d->promptActive=0;ClearFrameAndWindow2(&d->window,FALSE);PaletteData_BlendPalette(d->palette,(PaletteBufferId)0,9,1,0,0);PaletteData_BlendPalette(d->palette,(PaletteBufferId)2,(u16)(d->paletteSlot<<4),16,0,0);*state=2;break;}break;
 case 6:FillWindowPixelBuffer(&d->window,15);DrawFrameAndWindow2(&d->window,0,1,14);ReadMsgDataIntoString(d->messages,3,d->string);d->printer=AddTextPrinterParameterized(&d->window,1,d->string,0,0,Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(d->save)),NULL);(*state)++;break;
 case 7:if(!TextPrinterCheckActive(d->printer))(*state)++;break;
 case 8:d->timer++;if(d->timer>90)*state=9;break;
 case 9:if(!IsPaletteFadeFinished())sub_0200FB70();BeginNormalPaletteFade(0,0,0,0,6,1,HEAP_ID_125);(*state)++;break;
 case 10:if(IsPaletteFadeFinished()==1)(*state)++;break;
 default:ov45_0222EEB8();return 1;
 }
 ov89_0225A21C(d);ov89_0225A398(d);
 if(d->phase==1){d->ticks=(s32)((u32)d->ticks+1);if(d->ticks>30){d->ticks=0;ov89_02259F9C(d);}
 if((ov45_0222D844()==1||ov45_0222A330(d->owner->network)==1)&&*state!=4){if(d->promptActive==1)YesNoPrompt_Reset(d->prompt);d->active=0;d->phase=3;if(ov45_0222D844()==1)*state=9;else{PlaySE(0x5f1);ov45_0222A4A8((u8*)d->owner->network);*state=6;}}
 }
 ov89_0225A468(d);return 0;
}
