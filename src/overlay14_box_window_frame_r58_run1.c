#include "overlay14_box_window_frame_r58_private.h"
void PCBox_DrawChangeBoxTitle(void *p) {
    ov14_021F5BD8(p, 0x29, 0x4d, 0, 3);
}
void PCBox_DrawBoxMenu(void *p) {
    PCBox_DrawChangeBoxTitle(p);
    PCBox_DrawButtonWindow(p, 0x20, 0x1a, 0);
    PCBox_DrawButtonWindow(p, 0x21, 0x1b, 0);
}
void PCBox_DrawChangeWallpaperTitle(void *p) {
    ov14_021F5BD8(p, 0x29, 0x4e, 0, 3);
}
void PCBox_DrawDepositTitle(void *p) {
    ov14_021F5BD8(p, 0x29, 0x4f, 0, 3);
}
