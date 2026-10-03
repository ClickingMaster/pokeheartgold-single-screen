#pragma force_active on
#include "global.h"
extern void *OverlayManager_GetData(void *);extern void OverlayManager_FreeData(void *);
extern void ClearWindowTilemapAndCopyToVram(void *),RemoveWindow(void *),FreeBgTilemapBuffer(void *,int),Heap_Free(void *),Heap_Destroy(int),RegisterMainOverlay(int,void *);
FS_EXTERN_OVERLAY(OVY_74);
extern u8 gApp_MainMenu_SelectOption_MysteryGift[];
extern void ov74_0222FCC4(void *),LoadFontPal0(int,int,int),ov74_0222D824(void *,int);
extern void *Heap_AllocAtEnd(int,int);extern void *memset(void *,int,u32);
typedef enum {HEAP_ZERO=0} ScratchHeap;
int ov74_0222EB44(void *);void ov74_0222EC08(void *,void *,ScratchHeap);
int ov74_0222EB44(void *manager) {
 u8 *w=OverlayManager_GetData(manager),*cur,*win;u32 i;
 for(i=0,cur=w,win=w+0x2a08;i<19;i++,cur+=16,win+=16)if(*(void**)(cur+0x2a08)){ClearWindowTilemapAndCopyToVram(win);RemoveWindow(win);}
 if(*(void**)(w+0x2bd0)){ClearWindowTilemapAndCopyToVram(w+0x2bd0);RemoveWindow(w+0x2bd0);}
 if(*(void**)(w+0x2be0)){ClearWindowTilemapAndCopyToVram(w+0x2be0);RemoveWindow(w+0x2be0);}
 FreeBgTilemapBuffer(*(void**)(w+0x29fc),0);FreeBgTilemapBuffer(*(void**)(w+0x29fc),1);FreeBgTilemapBuffer(*(void**)(w+0x29fc),2);FreeBgTilemapBuffer(*(void**)(w+0x29fc),3);Heap_Free(*(void**)(w+0x29fc));
 RegisterMainOverlay(FS_OVERLAY_ID(OVY_74),gApp_MainMenu_SelectOption_MysteryGift);Heap_Destroy(89);OverlayManager_FreeData(manager);Heap_Destroy(85);return 1;
}
