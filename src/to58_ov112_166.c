#include "to58_ov112_166_private.h"

void ov112_021F1288(u8 *state) {
    const u8 *template;
    u8 *window;
    int i;
    if (state == NULL) {
        GF_AssertFail();
    }
    if (*(void **)(state + 0x14) == NULL) {
        GF_AssertFail();
    }
    *(void **)(state + 0x5C) = NewMsgDataFromNarc(1, 0x1B, 0x113, *(int *)(state + 4));
    *(void **)(state + 0x60) = NewMsgDataFromNarc(1, 0x1B, 0xED, *(int *)(state + 4));
    *(void **)(state + 0x64) = NewMsgDataFromNarc(1, 0x1B, 0xDE, *(int *)(state + 4));
    *(void **)(state + 0x58) = MessageFormat_New_Custom(0xD, 0x20, *(int *)(state + 4));
    window = state;
    template = ov112_021FF350;
    i = 0;
    window += 0x18;
    for (; i < 4; i++) {
        AddWindow(*(void **)(state + 0x14), window, template);
        template += 8;
        window += 0x10;
    }
    BG_FillCharDataRange(*(void **)(state + 0x14), 2, 0, 1, 0);
    LoadFontPal1(0, 0x1C0, *(int *)(state + 4));
    LoadFontPal1(4, 0x1C0, *(int *)(state + 4));
    FontID_Alloc(4, *(int *)(state + 4));
}
