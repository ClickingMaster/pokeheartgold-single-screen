#pragma force_active on
#include "global.h"
typedef struct {u32 a,b,state;} Head;
typedef struct {u8 pad[0x14];void *bg;u8 pad2[0x2d8];void *menu;} Inner;
typedef struct {Head *head;u8 pad[0x30];Inner *inner;} Context;
extern void *BgCellManager_New(void *,s32,s32,s32);
extern void BgCellManager_InitCell(void *,s32,s32,s32,s32);
extern void BgCellManager_DrawCellAt(void *,s32,s32,s32);
extern void PCBox_AttachPanelWindows(Inner *),ov14_021F62CC(Inner *),ov14_021F62E4(Inner *),ov14_021F62FC(Inner *),ov14_021F6314(Context *),ov14_021F63F0(Inner *),ov14_021F63C8(Inner *);
extern void PCBox_LoadPanelTilemap(void *,s32,s32),ov14_021E81A8(void *,s32),ov14_021E8394(void *),ov14_021E85AC(void *),PCBox_HideSummaryButton(void *);
void PCBox_InitPanels(Context *p);
#pragma force_active on
void PCBox_InitPanels(Context *p){
 p->inner->menu=BgCellManager_New(p->inner->bg,2,17,10);
 BgCellManager_InitCell(p->inner->menu,13,1,32,7);
 BgCellManager_InitCell(p->inner->menu,1,1,11,18);
 BgCellManager_InitCell(p->inner->menu,2,0,11,18);
 BgCellManager_InitCell(p->inner->menu,12,0,11,6);
 PCBox_AttachPanelWindows(p->inner);ov14_021F62CC(p->inner);ov14_021F62E4(p->inner);ov14_021F62FC(p->inner);ov14_021F6314(p);
 if(p->head->state==0){BgCellManager_InitCell(p->inner->menu,0,0,32,7);PCBox_LoadPanelTilemap(p->inner->menu,0,12);}else{BgCellManager_InitCell(p->inner->menu,0,1,32,6);PCBox_LoadPanelTilemap(p->inner->menu,0,11);}
 PCBox_LoadPanelTilemap(p->inner->menu,13,13);
 if(p->head->state<=1){PCBox_LoadPanelTilemap(p->inner->menu,1,9);}else{ov14_021E81A8(p->inner->menu,1);ov14_021F63F0(p->inner);}
 if(p->head->state==3){BgCellManager_InitCell(p->inner->menu,16,6,32,9);BgCellManager_DrawCellAt(p->inner->menu,16,0,24);PCBox_LoadPanelTilemap(p->inner->menu,16,85);}else{BgCellManager_InitCell(p->inner->menu,16,6,12,10);BgCellManager_DrawCellAt(p->inner->menu,16,32,12);PCBox_LoadPanelTilemap(p->inner->menu,16,86);}
 PCBox_LoadPanelTilemap(p->inner->menu,2,10);ov14_021F63C8(p->inner);ov14_021E8394(p->inner->menu);ov14_021E85AC(p->inner->menu);PCBox_HideSummaryButton(p->inner->menu);
}
