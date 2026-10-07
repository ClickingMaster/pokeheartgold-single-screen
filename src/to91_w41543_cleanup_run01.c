#include "global.h"
#include "bg_window.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "overlay_manager.h"
#include "heap.h"
#pragma force_active on
extern void ov74_022359BC(void);
void ov74_02229E68(OverlayManager *);
void ov74_02229E68(OverlayManager *manager) {
    u8 *state=OverlayManager_GetData(manager);
    int i;
    u8 *owner;
    u8 *cursor;
    ListMenuItem *items;
    struct ListMenu *menu;
    ov74_022359BC();
    items=*(ListMenuItem **)(state+0x7c);
    if(items)ListMenuItems_Delete(items);
    menu=*(struct ListMenu **)(state+0x78);
    if(menu)DestroyListMenu(menu,NULL,NULL);
    ClearWindowTilemapAndCopyToVram((Window *)(state+0x18));
    RemoveWindow((Window *)(state+0x18));
    if(WindowIsInUse((Window *)(state+0x58))) {
        ClearWindowTilemapAndCopyToVram((Window *)(state+0x58));
        RemoveWindow((Window *)(state+0x58));
    }
    cursor=state;
    i=0;
    owner=state;
    cursor+=0x28;
    for(;i<3;i++,owner+=sizeof(Window),cursor+=sizeof(Window)) {
        if(((Window *)(owner+0x28))->bgConfig) {
            ClearWindowTilemapAndCopyToVram((Window *)cursor);
            RemoveWindow((Window *)cursor);
        }
    }
    FreeBgTilemapBuffer(*(BgConfig **)state,0);
    FreeBgTilemapBuffer(*(BgConfig **)state,1);
    FreeBgTilemapBuffer(*(BgConfig **)state,4);
    FreeBgTilemapBuffer(*(BgConfig **)state,5);
    Heap_Free(*(BgConfig **)state);
}
