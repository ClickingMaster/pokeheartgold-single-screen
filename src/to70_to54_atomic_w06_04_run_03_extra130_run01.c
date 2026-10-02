#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_text.h"
#include "render_window.h"
#include "options.h"
#include "font.h"
#include "overlay_manager.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#pragma force_active on
void ov74_0222AA18(void *,Window *,int);
void ov74_0222A94C(void *,int,int);
void ov74_0222A81C(void *);
int ov74_02236AE0(void *,void *);
void ov74_02229E28(void *,int);
void PlaySE(int);
int ov74_0222AB70(void *,u8 *);
void ov74_0222AC1C(void *,int *);
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define W(p,o) (*(int *)((u8 *)(p)+(o)))
int ov74_0222AB70(void *man,u8 *w){
 ResetAllTextPrinters();
 LoadFontPal0(GF_PAL_LOCATION_MAIN_BG,(enum GFPalSlotOffset)0,(enum HeapID)84);LoadFontPal0(GF_PAL_LOCATION_MAIN_BG,(enum GFPalSlotOffset)32,(enum HeapID)84);
 LoadUserFrameGfx2(P(w,0),(GFBgLayer)0,1,2,(u8)Options_GetFrame(P(w,8)),(enum HeapID)84);
 LoadUserFrameGfx1(P(w,0),(GFBgLayer)0,31,3,1,(enum HeapID)84);
 *(volatile u16 *)0x05000000=0x7d8c;
 if(!WindowIsInUse((Window *)(w+0x18)))
  AddWindowParameterized(P(w,0),(Window *)(w+0x18),0,2,19,27,4,0,40);
 ov74_0222AA18(man,(Window *)(w+0x18),0);
 ov74_0222A94C(man,196,0);ov74_0222A81C(P(w,0));
 return 1;
}
void ov74_0222AC1C(void *man,int *state){
 u8 *w=OverlayManager_GetData(man);
 switch(ov74_02236AE0(man,w+0x15e0)){
 case 1:
  memcpy(w+0x8c,w+0x1658,936);
  ov74_02229E28(w,0);W(w,0x84)=27;*state=19;W(w,0x15dc)=0;break;
 case 2:case 3:
  ov74_02229E28(w,0);W(w,0x15dc)=0;*state=17;break;
 case 4:
  ClearWindowTilemapAndCopyToVram((Window *)(w+0x2680));
  RemoveWindow((Window *)(w+0x2680));
  BgClearTilemapBufferAndCommit(P(w,0),0);PlaySE(1500);
  ov74_0222A94C(man,196,0);GfGfx_EngineATogglePlanes(16,1);*state=3;break;
 }
}
