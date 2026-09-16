#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224FC08(void *work, void *window, int arg2) {
    void *options;
    *(String **)((u8 *)window + 0x10) = String_New(0x400, HEAP_ID_FIELD2);
    DialogBox_AddWindowToLayer3(*(BgConfig **)((u8 *)work + 8), window, 3);
    ov02_0224FCE0(work, *(String **)((u8 *)window + 0x10), HEAP_ID_FIELD2, arg2, *(u8 *)((u8 *)window + 0x816));
    options = Save_PlayerData_GetOptionsAddr(*(SaveData **)((u8 *)work + 0xc));
    DialogBox_LoadFrame(window, options);
    *(u16 *)((u8 *)window + 0x86e) = DialogBox_PrintMessage(window, *(String **)((u8 *)window + 0x10), options, 1);
    *(u8 *)((u8 *)work + 0xd2) |= 0x40;
}
