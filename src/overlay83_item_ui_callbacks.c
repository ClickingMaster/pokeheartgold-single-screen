#include "overlay83_item_ui_callbacks_private.h"
#define PTR(p, o) (*(void **)((p) + (o)))

void ov83_022415F4(u8 *p, int mode, int action) {
    void *mon;
    int index = ov83_02247768(p[0x14], mode);
    mon = Party_GetMonByIndex(PTR(p, 0x7A4), index);
    ov83_0224777C(PTR(p, 0x50C), p[9], 0);
    ov83_02240C60(p, 0, Mon_GetBoxMon(mon));
    ov83_02247944(p + 0xB0, Options_GetFrame(PTR(p, 0x508)));
    p[0xA] = ov83_0223FD14(p, _02247D0C[action - 1], 1);
    switch (action) {
    case 1:
        ov83_0224152C(mon, 0x18);
        break;
    case 2:
        ov83_0224152C(mon, 0x29);
        break;
    case 3:
        ov83_0224152C(mon, 0x18);
        ov83_0224152C(mon, 0x29);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PlaySE(0x5EC);
}

void ov83_022416A0(u8 *p, int item, u16 value) {
    void *mon = Party_GetMonByIndex(PTR(p, 0x7A4), ov83_02247768(p[0x14], item));
    SetMonData(mon, 6, &value);
    if (p[0xD] == item) {
        ov83_02241E18(p);
        ov83_022421E0(p, 0);
    }
    ov83_0224755C(PTR(p, 0x74C + ov83_02247768(p[0x14], item) * 4), 1);
    ov83_02240C60(p, 0, Mon_GetBoxMon(mon));
    BufferItemName(PTR(p, 0x24), 1, value);
    p[0xA] = ov83_0223FD14(p, 0x3B, 1);
    PlaySE(0x623);
}

void ov83_02241730(u8 *raw) {
    Work *p = (Work *)raw;
    if (p->flag == 1) {
        TouchscreenListMenu_DestroyButtons(*(void **)(raw + 0x840));
        ov83_02242D5C(raw);
    }
    ov83_022478B4(raw + 0x84C);
}
