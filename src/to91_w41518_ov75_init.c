#include "global.h"
#include "overlay_manager.h"
#include "overlay75_lifecycle_internal.h"
#include "system.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "gf_gfx_planes.h"
#include "vram_transfer_manager.h"
#include "render_text.h"
#include "unk_020210A0.h"
#include "unk_0200FA24.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#pragma force_active on
typedef Overlay75State Args;
typedef struct {void*args;void*bg;u32 mode;u8 pc[0x20-12];void*format;void*messages[5];void*str,*name,*str2;u8 p44[0xe8-0x44];u32 local;u8 pec[0x41c-0xec];} Work;
extern void ov75_0224725C(void*),ov75_02247450(void*),ov75_02247790(void*),ov75_02246BE0(void*,int),ov75_02247234(void*);
extern void *sub_02039528(SaveData*);
extern u8 ov75_02246BE8(void*);
extern int ov00_021EC5B4(void);
int ov75_02246F0C(OverlayManager*,int*);
int ov75_02246F0C(OverlayManager*manager,int*unused){Work*w;Main_SetVBlankIntrCB(0,0);HBlankInterruptDisable();GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();GX_SetVisiblePlane(0);GXS_SetVisiblePlane(0);GX_SetVisibleWnd(0);GXS_SetVisibleWnd(0);G2_BlendNone();G2S_BlendNone();Heap_Create(HEAP_ID_3,HEAP_ID_116,0x70000);w=OverlayManager_CreateAndGetData(manager,0x41c,HEAP_ID_116);MI_CpuFill8(w,0,0x41c);w->args=OverlayManager_GetArgs(manager);w->bg=BgConfig_Alloc(HEAP_ID_116);GF_CreateVramTransferManager(64,HEAP_ID_116);SetKeyRepeatTimers(4,8);ov75_0224725C(w->bg);sub_020210BC();sub_02021148(4);w->format=MessageFormat_New_Custom(11,64,HEAP_ID_116);w->messages[0]=NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT,(NarcId)27,775,HEAP_ID_116);w->messages[2]=NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT,(NarcId)27,778,HEAP_ID_116);w->messages[3]=NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT,(NarcId)27,800,HEAP_ID_116);w->messages[1]=NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT,(NarcId)27,237,HEAP_ID_116);w->messages[4]=NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT,(NarcId)27,188,HEAP_ID_116);w->str=String_New(270,HEAP_ID_116);w->str2=String_New(256,HEAP_ID_116);w->name=NewString_ReadMsgData(w->messages[0],31);ov75_02247450(w);ov75_02247790(w);switch(ov75_02246BE8(w->args)){case 12:if(ov00_021EC5B4()==0&&sub_0203A05C(((Args*)w->args)->saveData)==1){w->mode=12;sub_02039528(((Args*)w->args)->saveData);sub_0203A880();}else w->mode=0;break;case 22:{u32*local=&w->local;*local=0;sub_0203A880();}w->mode=ov75_02246BE8(w->args);break;default:w->mode=ov75_02246BE8(w->args);break;}ov75_02246BE0(w->args,0);BeginNormalPaletteFade(0,1,1,0,6,1,HEAP_ID_116);GfGfx_EngineATogglePlanes(1,1);GfGfx_EngineATogglePlanes(2,1);GfGfx_EngineBTogglePlanes(1,1);GfGfx_EngineBTogglePlanes(2,1);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);gSystem.screensFlipped=1;GfGfx_SwapDisplay();TextFlags_SetCanABSpeedUpPrint(1);TextFlags_SetAutoScrollParam(0);TextFlags_SetCanTouchSpeedUpPrint(0);Main_SetVBlankIntrCB(ov75_02247234,w);return 1;}
