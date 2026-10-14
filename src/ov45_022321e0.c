#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union {
    struct {
        u32 flag : 1, rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int state;
    u8 p[0xd0 - 4];
    int mode;
} Work;

extern Work *volatile _022577C4;
extern void PPWLobbyChannel_GetName(String *, void *);
extern void LobbyString_Destroy(String *);
extern int LobbyChat_IsInChannel(char *);

static inline BOOL active(Work *w) {
    return w->mode == 5 ? FALSE : TRUE;
}

int PPW_LobbyGetSubChannelState(void);
#pragma opt_propagation off

int PPW_LobbyGetSubChannelState(void) {
    String text;
    Work *w = _022577C4;
    int result;
    if (!w) {
        return 3;
    }
    if (w && w->state == 10) {
        return 3;
    }
    if (w && _022577C4->state != 8) {
        return 3;
    }
    {
        BOOL enabled = active(w);
        if (enabled == 0) {
            return 0;
        }
    }
    PPWLobbyChannel_GetName(&text, &w->mode);
    result = LobbyChat_IsInChannel(!text.flag ? &text.bytes[1] : *(char **)&text.raw[4]);
    LobbyString_Destroy(&text);
    if (result) {
        return 2;
    }
    return 1;
}
