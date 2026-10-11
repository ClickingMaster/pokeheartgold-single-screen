#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "message_format.h"
#include "pm_string.h"
#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "options.h"
#include "player_data.h"
#include "screen_fade.h"
#include "vram_transfer_manager.h"
#include "render_text.h"
#include "wireless_status_icon_internal.h"
#include "overlay39_state_words.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void Main_SetVBlankIntrCB(void(*)(void*),void*);void HBlankInterruptDisable(void);void GfGfx_DisableEngineAPlanes(void);void GfGfx_DisableEngineBPlanes(void);

void ov39_02228440(void*);void GF_TouchpadResetSampling(void);void GF_TouchpadStartSampling(int);void ov39_022285CC(void*);void ov39_022288A0(void*);void GfGfx_SwapDisplay(void);void ov39_02228418(void*);
typedef struct{void*args;BgConfig*bg;u32 mode,nextMode;u8 gap[16];MessageFormat*format;struct{MsgData*one,*two,*three;}messages;u8 unmodeled30[4];String*string1,*string2,*string3;u8 rest[0x50];u32 textSpeed;u8 ownerRemainder[0x3d8-0x94];} Scene;
int ov39_02228140(OverlayManager*manager,int*state);
// @fn ov39_02228140
int ov39_02228140(OverlayManager*manager,int*state){Scene*w;
 Main_SetVBlankIntrCB(NULL,NULL);HBlankInterruptDisable();GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 GX_SetVisiblePlane(0);GXS_SetVisiblePlane(0);GX_SetVisibleWnd(0);GXS_SetVisibleWnd(0);G2_BlendNone();G2S_BlendNone();
 Heap_Create((enum HeapID)3,(enum HeapID)124,5<<16);w=OverlayManager_CreateAndGetData(manager,0x3d8,(enum HeapID)124);MI_CpuFill8(w,0,0x3d8);w->args=OverlayManager_GetArgs(manager);w->bg=BgConfig_Alloc((enum HeapID)124);GF_CreateVramTransferManager(64,(enum HeapID)124);SetKeyRepeatTimers(4,8);ov39_02228440(w->bg);GF_TouchpadResetSampling();GF_TouchpadStartSampling(4);
 {MessageFormat *format=MessageFormat_New_Custom(11,64,(enum HeapID)124);w->format=format;}w->messages.one=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,0x307,(enum HeapID)124);w->messages.two=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,0x30a,(enum HeapID)124);w->messages.three=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,0x320,(enum HeapID)124);
 w->string1=String_New(180,(enum HeapID)124);w->string3=String_New(256,(enum HeapID)124);w->string2=NewString_ReadMsgData(w->messages.one,31);ov39_022285CC(w);ov39_022288A0(w);BeginNormalPaletteFade(FADE_BOTH_SCREENS,FADE_TYPE_BRIGHTNESS_IN,FADE_TYPE_BRIGHTNESS_IN,0,6,1,(enum HeapID)124);
 GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineATogglePlanes(1,1);GfGfx_EngineATogglePlanes(2,1);GfGfx_EngineBTogglePlanes(1,1);GfGfx_EngineBTogglePlanes(2,1);B((u8*)&gSystem+0x60,9)=1;GfGfx_SwapDisplay();TextFlags_SetCanABSpeedUpPrint(1);TextFlags_SetAutoScrollParam(0);TextFlags_SetCanTouchSpeedUpPrint(1);Main_SetVBlankIntrCB(ov39_02228418,w);
 if(P(w->args,4)){WirelessStatusIcon_Create();ov39_02228B6C(w,13,8);}else {if(P(P(w->args,0),8)!=NULL)w->mode=1;else w->mode=0;}
 w->textSpeed=Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(P(P(w->args,0),4)));U(w->args,0x7c)=0;return 1;
}
