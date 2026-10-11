#include "overlay14_box_window_frame_r58_private.h"
void PCBox_AttachWindowToPanel(BgCellManager *mgr, int id, Window *window) {
    u32 width;
    u32 height;
    u32 bg = GetWindowBgId(window);
    width = GetWindowWidth(window);
    height = GetWindowHeight(window);
    BgCellManager_InitCell(mgr, id, bg, width, height);
    BgCellManager_PutWindowTiles(mgr, id, window);
}
void PCBox_AttachPanelWindows(void *p) {
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 3, (Window *)((u8 *)p + 0x200));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 4, (Window *)((u8 *)p + 0x210));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 5, (Window *)((u8 *)p + 0x220));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 6, (Window *)((u8 *)p + 0x230));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 7, (Window *)((u8 *)p + 0x240));
    PCBox_SetPanelWindowFrame(PTR_AT(p, 0x2f0), 0xc, (Window *)((u8 *)p + 0x2c0));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 0xe, (Window *)((u8 *)p + 0x2d0));
    PCBox_AttachWindowToPanel(PTR_AT(p, 0x2f0), 0xf, (Window *)((u8 *)p + 0x2e0));
}
