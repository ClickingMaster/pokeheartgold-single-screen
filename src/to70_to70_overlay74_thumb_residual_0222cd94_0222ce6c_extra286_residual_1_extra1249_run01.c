#pragma force_active on
#include "global.h"
#include "heap.h"
#include "main.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
FS_EXTERN_OVERLAY(intro_title);FS_EXTERN_OVERLAY(OVY_74);
extern const OverlayManagerTemplate gApplication_TitleScreen,_0223B410,_0223B420;
extern void sub_0201A4B0(int),ov74_02236034(int);
int ov74_0222CD94(OverlayManager*);
int ov74_0222CD94(OverlayManager*m){u8*w=OverlayManager_GetData(m);int state=*(int*)(w+0x440);if(state==0)RegisterMainOverlay(FS_OVERLAY_ID(intro_title),&gApplication_TitleScreen);else if(state==1)RegisterMainOverlay(FS_OVERLAY_ID(OVY_74),&_0223B410);else if(state==2)RegisterMainOverlay(FS_OVERLAY_ID(OVY_74),&_0223B420);Heap_Destroy((enum HeapID)89);OverlayManager_FreeData(m);Heap_Destroy((enum HeapID)84);if(*(int*)(w+0x15d4)==2)sub_0201A4B0(0);ov74_02236034(0);return 1;}
