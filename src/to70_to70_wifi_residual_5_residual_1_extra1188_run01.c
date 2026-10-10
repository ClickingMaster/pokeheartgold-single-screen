#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Packet {
    struct Packet *next;
    u16 length, port;
    u32 address;
    u8 data[1];
} Packet;

typedef struct {
    u8 p[0xfe];
    s8 peek;
    u8 pad;
    Packet *tail;
    Packet *volatile head;
    u16 count, pad2;
    OSThreadQueue wait;
} Recv;

typedef struct {
    u8 p[0x64];
    Recv *recv;
    u8 p2[8];
    s16 flags;
} Socket;

typedef struct {
    u8 p[0x1c];
    void (*free)(void *);
} Alloc;

extern Alloc *_02110628;
extern int SOCL_SocketIsInvalid(Socket *);
int SOCLi_ReadUdpBuffer(Socket *, void *, int, u16 *, u32 *, int);

static inline int active(Socket *s) {
    int result = 0;
    if (s == 0) {
        goto end;
    }
    if (s->flags & 1) {
        result = 1;
    }
end:
    return result;
}

int SOCLi_ReadUdpBuffer(Socket *s, void *buffer, int length, u16 *port, u32 *address, int flags) {
    Recv *r = s->recv;
    Packet *p;
    int result;
    OSIntrMode irq;
    (void)r->head;
    irq = OS_DisableInterrupts();
    p = r->head;
    if (p == 0) {
        do {
            if (!(flags & 1)) {
                result = -6;
                break;
            }
            OS_SleepThread(&r->wait);
            if (SOCL_SocketIsInvalid(s) || !active(s)) {
                result = -56;
                break;
            }
            p = r->head;
        } while (p == 0);
    }
    if (p != 0) {
        if (length > p->length) {
            length = p->length;
        }
        MI_CpuCopy8(p->data, buffer, length);
        if (port) {
            *port = p->port;
        }
        if (address) {
            *address = p->address;
        }
        result = p->length;
        if (r->peek == 0) {
            r->head = p->next;
            if (p->next == 0) {
                r->tail = 0;
            }
            _02110628->free(p);
            r->count -= result;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}
