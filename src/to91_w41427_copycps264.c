#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pipe[0xf8];
    u32 consumed;
    u16 threshold;
    s8 consumeMode;
    u8 pad;
    u8 tail[0x14];
} Receive;

typedef struct {
    u8 cps[0x64];
    void *recv;
    u8 pad68[8];
    s16 flags;
    s8 mode, kind;
    u16 port;
    u8 tail[10];
} Sock;

extern void *SOCLi_ReadCPSBuffer(void *, int *, u16 *, u16 *, u32 *);

static inline void AddRead(Receive *r, int n) {
    if (r->consumeMode == 0) {
        r->consumed += (u32)n;
    }
}

static inline int CopyReply(Sock *s, const void *src, void *dst, int size, int available) {
    int result = available;
    if (size > result) {
        size = result;
    }
    {
        int tcp = 1;
        if (s->kind != 0 && s->kind != 4) {
            tcp = 0;
        }
        if (tcp) {
            result = size;
        }
    }
    MI_CpuCopy8(src, dst, size);
    AddRead((Receive *)s->recv, result);
    return result;
}

int SOCLi_CopyCPSBuffer(void *, void *, int, u16 *, u32 *);

int SOCLi_CopyCPSBuffer(void *socket, void *dst, int size, u16 *port, u32 *address) {
    Sock *s = socket;
    OSIntrMode irq;

    struct {
        u16 remote, local;
        u32 ip;
        int available;
    } reply;

    int result;
    void *src;
    irq = OS_DisableInterrupts();
    src = SOCLi_ReadCPSBuffer(s, &reply.available, &reply.local, &reply.remote, &reply.ip);
    if (src) {
        result = reply.available;
        if (result == 0) {
            result = -6;
        } else {
            result = CopyReply(s, src, dst, size, result);
        }
    } else {
        result = reply.available == 0 ? 0 : -28;
        s->flags &= ~6;
    }
    if (result >= 0) {
        if (port && address) {
            *port = reply.remote;
            *address = reply.ip;
        }
        if (!s->port) {
            s->port = reply.local;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}
