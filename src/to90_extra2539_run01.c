#pragma force_active on
#include "global.h"
#include "touch_sampling_internal.h"
#include "bg_window.h"
#include "render_window.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"
#include "unk_02005D10.h"
#pragma force_active on
struct BitFlag{u8 bit:1,rest:7;};
typedef Window LocalWindow;
typedef struct{u8 beforeState[8];u8 state,gap0[5],slot,delay;u8 gap1[0x5c-0x10];LocalWindow promptWindow;u8 gap2[0xfc-0x6c];LocalWindow messageWindow;u8 gap3[0x12c-0x10c];LocalWindow window2,window1;u8 gap4[0x33c-0x14c];void*mainSprite,*effectSprite;u8 gap5[12];void*items[3],*exitSprite;u8 cells[9],cellPadding;u16 values[4];u16 valuePadding;u16 *result,*dest,*kind;u8 gap7[16];void*prompt;u8 selected[9],gap8[2],index;struct BitFlag flags;u8 failures,chosen[3],gap9[19];TouchOutput touch;u8 scratch[0x9600];} State;
extern const TouchscreenHitbox ov87_021E8308[10],ov87_021E8184[2],ov87_021E818C[2];
void ov87_021E7FEC(void*,int,int),ov87_021E7FD4(void*,int),ov87_021E803C(void*,int),ov87_021E8084(void*,int);

void ov87_021E7550(void*),ov87_021E7698(void*),ov87_021E6AE0(void*),ov87_021E6AF4(void*),ov87_021E6B28(void*),ov87_021E7998(void*),ov87_021E79A0(void*),ov87_021E7990(void*),ov87_021E75E0(void*,u8),ov87_021E74F4(void*,int);
u8 ov87_021E7490(void*);
int ov87_021E7008(void*),ov87_021E708C(void*),ov87_021E70D0(void*),ov87_021E71B4(void*),ov87_021E717C(void*),ov87_021E7134(void*),ov87_021E71EC(void*,int),ov87_021E7734(void*),ov87_021E78D8(void*,int),ov87_021E80B4(void*);
u32 ov87_021E788C(void*);
int ov87_021E6080(void*);
int ov87_021E6080(void*p){State*o=p;int i,hit;switch(o->state){case 0:if(o->slot==2){ClearWindowTilemapAndCopyToVram(&o->window2);ov87_021E71B4(p);ov87_021E7FEC(o->mainSprite,88,160);ov87_021E803C(o->mainSprite,0);}else{ClearWindowTilemapAndCopyToVram(&o->window1);ov87_021E717C(p);}o->delay=30;ov87_021E7FD4(o->effectSprite,1);ov87_021E708C(p);for(i=0;i<4;i++)ov87_021E71EC(p,(u8)i);ov87_021E7FD4(o->mainSprite,1);ov87_021E8084(o->mainSprite,0);PlaySE(1378);o->state=1;break;
case 1:GfGfx_EngineATogglePlanes(4,1);GfGfx_EngineBTogglePlanes(1,1);o->state=2;break;
case 2:if(o->delay==0){ov87_021E7FD4(o->effectSprite,0);FillWindowPixelBuffer(&o->messageWindow,0);ScheduleWindowCopyToVram(&o->messageWindow);o->state=3;}else o->delay--;break;
case 3:sub_02021280(&o->touch,4,2);hit=TouchscreenHitbox_FindRectAtTouchHeld(ov87_021E8308);if(hit!=-1){if(ov87_021E7490(p)<3)o->selected[hit]=1;if(o->selected[hit]==1){ov87_021E7550(p);ov87_021E75E0(p,hit);ov87_021E74F4(p,hit);if(!IsSEPlaying(1375))PlaySE(1375);}if(ov87_021E7734(p)==1){StopSE(1375,0);ov87_021E7FD4(o->effectSprite,1);o->dest[o->slot]=o->values[o->index];if(o->values[o->index]==92)o->kind[o->slot]=1;else o->kind[o->slot]=3;o->delay=ov87_021E788C(p)==1?30:0;ov87_021E70D0(p);o->state=4;return 0;}if(o->failures>=3){StopSE(1375,0);o->delay=60;ov87_021E7FD4(o->effectSprite,1);ov87_021E7134(p);o->state=7;return 0;}}else StopSE(1375,0);if(o->flags.bit==1)ov87_021E7698(p);if(o->slot==2){if(TouchscreenHitbox_FindRectAtTouchNew(ov87_021E8184)==-1)break;ov87_021E8084(o->mainSprite,1);ov87_021E7008(p);PlaySE(1508);ov87_021E803C(o->mainSprite,0);ov87_021E6AE0(p);ov87_021E6AF4(p);o->state=10;}else{if(TouchscreenHitbox_FindRectAtTouchNew(ov87_021E818C)==-1)break;ov87_021E8084(o->mainSprite,1);ov87_021E7008(p);PlaySE(1508);ov87_021E803C(o->mainSprite,1);ov87_021E6AE0(p);ov87_021E6AF4(p);o->state=9;}break;
case 4:if(o->delay==0){if(!ov87_021E78D8(p,0))o->state=5;}else o->delay--;break;
case 5:if(!ov87_021E78D8(p,1)){o->delay=0;o->state=6;}break;
case 6:if(ov87_021E80B4(o->exitSprite)==1)break;if(o->delay==0){for(i=0;i<3;i++){ov87_021E7FD4(o->items[i],0);}ov87_021E7FD4(o->exitSprite,0);ov87_021E7998(p);ov87_021E79A0(p);return 1;}o->delay--;break;
case 7:if(o->delay==0){GfGfx_EngineATogglePlanes(1,0);ov87_021E7998(p);ov87_021E79A0(p);o->delay=30;o->state=8;}else o->delay--;break;
case 8:if(o->delay==0)return 1;o->delay--;break;
case 9:hit=(int)YesNoPrompt_HandleInputForSave(o->prompt);if(hit==1){ov87_021E8084(o->mainSprite,0);sub_0200E5D4(&o->promptWindow,0);ClearWindowTilemapAndCopyToVram(&o->promptWindow);ov87_021E7998(p);ov87_021E6B28(p);return 1;}if(hit==2){ov87_021E8084(o->mainSprite,0);sub_0200E5D4(&o->promptWindow,0);ClearWindowTilemapAndCopyToVram(&o->promptWindow);ov87_021E6B28(p);o->state=11;}break;
case 10:hit=(int)YesNoPrompt_HandleInputForSave(o->prompt);if(hit==1){ov87_021E8084(o->mainSprite,0);sub_0200E5D4(&o->promptWindow,0);ClearWindowTilemapAndCopyToVram(&o->promptWindow);ov87_021E7990(p);ov87_021E6B28(p);return 1;}if(hit==2){ov87_021E8084(o->mainSprite,0);sub_0200E5D4(&o->promptWindow,0);ClearWindowTilemapAndCopyToVram(&o->promptWindow);ov87_021E6B28(p);o->state=11;}break;
case 11:if(!System_GetTouchHeld())o->state=3;break;}return 0;}
