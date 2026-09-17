#include "overlay80_menu_runtime_r36_private.h"

void ov80_0222EC90(SysTask *task, void *work) {
    s32 selection;
    u16 old;
    (void)task;
    if (U8_AT(work, 0x94) != 0) {
        U8_AT(work, 0x94)
        --;
        return;
    }
    if (IsPaletteFadeFinished()) {
        selection = ListMenu_ProcessInput(PTR_AT(work, 0x1b4));
        old = U16_AT(work, 0x2d4);
        ListMenuGetCurrentItemArrayId(PTR_AT(work, 0x1b4), (u16 *)((u8 *)work + 0x2d4));
        if (old != U16_AT(work, 0x2d4)) {
            PlaySE(0x5dc);
        }
        if ((*(u32 *)(gSystem + 0x4c) & 0x40) || (*(u32 *)(gSystem + 0x4c) & 0x80) || (*(u32 *)(gSystem + 0x4c) & 0x20) || (*(u32 *)(gSystem + 0x4c) & 0x10)) {
            ov80_0222EE7C(work);
        }
        if (selection == -1 && System_GetTouchNew() == 1) {
            selection = ListMenuGetValueByArrayId(PTR_AT(work, 0x1b4), U16_AT(work, 0x2d4));
        }
        if (U16_AT(PTR_AT(work, 0xa0), 0) == 0xeedd) {
            ov80_0222EDA0(work, 0);
            return;
        }
        switch (selection) {
        case -2:
            if (((Ov80Flags *)((u8 *)work + 0x97))->bit0 == 1) {
                PlaySE(0x5dc);
                U16_AT(PTR_AT(work, 0xa0), 0) = 0xfffe;
                ov80_0222EDA0(work, 1);
                return;
            }
            break;
        case -1:
            break;
        default:
            PlaySE(0x5dc);
            U16_AT(PTR_AT(work, 0xa0), 0) = selection;
            ov80_0222EDA0(work, 1);
            break;
        }
    }
}

void ov80_0222EDA0(void *work, int playSound) {
    int i;
    u8 *walker;
    void *owner = PTR_AT(work, 0);
    if (playSound == 1) {
        PlaySE(0x5dc);
    }
    DestroyListMenu(PTR_AT(work, 0x1b4), NULL, NULL);
    sub_0200E5D4(PTR_AT(work, 0x1a0), 0);
    RemoveWindow((Window *)((u8 *)work + 8));
    i = 0;
    walker = work;
    while (i < 28) {
        String_Delete(PTR_AT(walker, 0x1c));
        i++;
        walker += 4;
    }
    if ((((u32)U8_AT(work, 0x97) << 30) >> 31) == 1) {
        DestroyMsgData(PTR_AT(work, 0x8c));
    }
    SysTask_Destroy(PTR_AT(work, 4));
    Heap_Free(work);
    PTR_AT(owner, 0x60) = NULL;
}

void ov80_0222EE14(void *work, u16 msgNo, u32 speed) {
    String *raw;
    String *expanded;
    raw = String_New(0x50, *(u32 *)(PTR_AT(work, 0) + 0x34));
    expanded = String_New(0x50, *(u32 *)(PTR_AT(work, 0) + 0x34));
    FillWindowPixelBuffer(PTR_AT(work, 0x18), 0xf);
    ReadMsgDataIntoString(PTR_AT(work, 0x8c), msgNo, raw);
    StringExpandPlaceholders(PTR_AT(work, 0x90), expanded, raw);
    AddTextPrinterParameterized(PTR_AT(work, 0x18), 1, expanded, 0, 0, speed, NULL);
    String_Delete(raw);
    String_Delete(expanded);
}

void ov80_0222EE7C(void *work) {
    u16 value;
    ListMenuGetCurrentItemArrayId(PTR_AT(work, 0x1b4), (u16 *)((u8 *)work + 0x1ba));
    value = U16_AT((u8 *)work + U16_AT(work, 0x1ba) * 2, 0x29c);
    if (value != 0xff) {
        ov80_0222EE14(work, value, 0);
    }
}
