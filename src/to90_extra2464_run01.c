#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int active;
    u8 pad[6];
    u16 port;
} Socket;

typedef struct {
    u32 a, b;
    u16 port;
} Global;

extern Global gCpsState;
u16 CPS_SocGetEport(void);

static inline BOOL inuse(OSThread *head, Global *global) {
    BOOL used = FALSE;
    OSThread *thread;
    for (thread = head; thread; thread = thread->next) {
        Socket *s = thread->specific[0];
        if (s && s->active && s->port == global->port) {
            used = TRUE;
            break;
        }
    }
    return used;
}

u16 CPS_SocGetEport(void) {
    BOOL used;
    do {
        OSThread *thread;
        gCpsState.port++;
        used = FALSE;
        if (gCpsState.port < 1024 || gCpsState.port >= 5000) {
            gCpsState.port = 1024;
        }
        for (thread = OSi_ThreadInfo.list; thread; thread = thread->next) {
            Socket *socket = thread->specific[0];
            if (socket && socket->active && socket->port == gCpsState.port) {
                used = TRUE;
                break;
            }
        }
    } while (used);
    return gCpsState.port;
}
