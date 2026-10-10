#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int zero, sleep;
    u8 state;
    u8 pad[7];
    u32 tick;
    u8 pad2[20];
    u32 seq;
} Socket;

extern u8 _021D4620[];
u32 get_seqno(void);
void tcp_send_handshake(Socket *, int, int);
int CPSi_TcpConnectRaw(Socket *);

int CPSi_TcpConnectRaw(Socket *s) {
    u32 attempt;
    u32 seq = get_seqno();
    attempt = 0;
    do {
        OSIntrMode saved;
        s->seq = seq;
        s->state = 2;
        s->tick = OS_GetTick() >> 16;
        tcp_send_handshake(s, 2, 24);
        saved = OS_DisableInterrupts();
        if (s->state == 2 && *(int *)(_021D4620 + 0x50)) {
            s->sleep = 1;
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(saved);
        if (s->state == 4) {
            return 0;
        }
        if (!*(int *)(_021D4620 + 0x50)) {
            break;
        }
        attempt++;
    } while (attempt < 3);
    return 1;
}
