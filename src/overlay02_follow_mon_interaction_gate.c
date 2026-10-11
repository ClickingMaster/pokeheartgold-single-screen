#include "overlay02_4_landing_private.h"

WIP_LOCAL BOOL ov02_0224FB54(FieldSystem *fieldSystem, void *a1, void *arg2) {
    u8 v;
    if (sub_0205BB04((u8) * (u16 *)((u8 *)*(void **)((u8 *)fieldSystem + 0x120) + 0x882)) != 0) {
        return FALSE;
    }
    v = *(u8 *)((u8 *)arg2 + 6);
    if (v != 0) {
        if (v > 0xe) {
            return FALSE;
        }
        FieldSystem_ShowEmoteBubble(fieldSystem, *(LocalMapObject **)((u8 *)fieldSystem + 0xe4), v - 1);
        return TRUE;
    }
    return FALSE;
}
