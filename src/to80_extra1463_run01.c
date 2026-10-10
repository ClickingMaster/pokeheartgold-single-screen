#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int fd;
    s16 events, revents;
} Poll;

typedef struct {
    u8 pad[0x70];
    s16 flags;
    u8 pad2;
    s8 state;
} Socket;

extern int SOCL_GetStatus(int);
extern int SOCL_SocketIsInvalid(Socket *);
extern int SOCLi_ExecEnableSslCommand(Socket *, void *);
extern void OS_Sleep(u32);
int SOC_Poll(Poll *, u32, s64);
int SOCL_EnableSsl(Socket *, void *);

int SOCL_EnableSsl(Socket *s, void *p) {
    int valid;
    if (SOCL_SocketIsInvalid(s)) {
        return -28;
    }
    valid = 1;
    if (s->state != 0 && s->state != 4) {
        valid = 0;
    }
    if (!valid) {
        return -28;
    }
    valid = 0;
    if (s) {
        valid = (s->flags & 1) != 0;
    }
    if (!valid) {
        return -39;
    }
    if (s->flags & 2) {
        return -28;
    }
    return SOCLi_ExecEnableSslCommand(s, p);
}
