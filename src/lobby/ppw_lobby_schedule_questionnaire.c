#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))

int PPWLobbySchedule_GetQuestionnaire(void *p, void *out) {
    if (!W(p, 0x334)) {
        return 0;
    }
    MI_CpuCopy8((u8 *)p + 0x5c, out, 0x2d8);
    return 1;
}

int PPWLobbySchedule_SetQuestionnaire(void *p, void *in) {
    MI_CpuCopy8(in, (u8 *)p + 0x5c, 0x2d8);
    W(p, 0x334) = 1;
    return 1;
}
