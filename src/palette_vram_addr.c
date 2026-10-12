#include "palette_vram_addr.h"

#include "global.h"

u16 *GetMainBgPlttAddr(void) {
    return (u16 *)HW_BG_PLTT;
}

int GetMainBgPlttSize(void) {
    return HW_BG_PLTT_SIZE;
}

u16 *GetSubBgPlttAddr(void) {
    return (u16 *)HW_DB_BG_PLTT;
}

int GetSubBgPlttSize(void) {
    return HW_BG_PLTT_SIZE;
}

u16 *GetMainObjPlttAddr(void) {
    return (u16 *)HW_OBJ_PLTT;
}

u16 *GetSubObjPlttAddr(void) {
    return (u16 *)HW_DB_OBJ_PLTT;
}
