#include "overlay59_lifecycle_gap8_private.h"
#define U8(w, o)  (*(u8 *)((u8 *)(w) + (o)))
#define U32(w, o) (*(u32 *)((u8 *)(w) + (o)))
#define PTR(w, o) (*(void **)((u8 *)(w) + (o)))
void ov59_0223A9E4(Ov59Gap8Work *work) {
    int i;
    void *options;
    u8 *entry;
    U32(work, 0x44) = MenuInputStateMgr_GetState(PTR(PTR(PTR(work, 4), 4), 0x14));
    PTR(work, 8) = PTR(PTR(PTR(work, 4), 4), 0x18);
    options = Save_PlayerData_GetOptionsAddr(PTR(work, 8));
    U8(work, 0x4B) = Options_GetTextFrameDelay(options);
    U8(work, 0x4C) = Options_GetFrame(options);
    PTR(work, 0x14) = Save_ApricornBox_Get(PTR(work, 8));
    sub_02031CEC(PTR(work, 0x14), PTR(PTR(work, 4), 0x14), (u8 *)work + 0x18);
    sub_02031DA0((u8 *)work + 0x18, (u8 *)work + 0x20);
    ov59_0223AE20(work);
    i = 0;
    if (i < U8(work, 0x49)) {
        entry = (u8 *)work;
        do {
            if (U8(PTR(work, 4), 0x1C) == U8(entry, 0x84)) {
                U8(work, 0x4A) = i;
                return;
            }
            i++;
            entry += 0x34;
        } while (i < U8(work, 0x49));
    }
}
void ov59_0223AA6C(Ov59Gap8Work *work) {
    u8 *entry;
    ov59_0223AE88(work);
    MenuInputStateMgr_SetState(PTR(PTR(PTR(work, 4), 4), 0x14), U32(work, 0x44));
    entry = (u8 *)work + U8(work, 0x4A) * 0x34;
    U8(PTR(work, 4), 0x1C) = U8(entry, 0x84);
    U8(PTR(work, 4), 0x18) = U8(work, 0x48) != 0 ? 1 : 3;
    if (PTR(PTR(PTR(work, 4), 4), 8) != NULL) {
        *(u16 *)PTR(PTR(PTR(work, 4), 4), 8) = U8(work, 0x48);
    }
}
