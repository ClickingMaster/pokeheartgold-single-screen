#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct SocketView {
    u8 p[12];
    void *ssl;
} SocketView;

typedef struct SSLView {
    u8 p[0x800];
    void *arg;
    u8 gap[12];
    void (*callback)(void *);
    u8 tail[0x1c];
} SSLView;

typedef struct BufferView {
    char *base, *cursor, *end;
    int size;
} BufferView;

typedef struct Context {
    u8 p[0x101c];
    int timeout, state;
    u8 q[0x1124 - 0x1024];
    void *arg;
    u32 unknown, ip;
    int secure;
    u16 port, pad;
    SocketView socket;
    u8 a[0x119c - 0x1148];
    SSLView ssl;
    u8 b[0x19f8 - 0x19cc];
    char *request;
    u8 c[12];
    BufferView buffer;
    u8 mutex[24];
    int expected, count;
} Context;

extern int _021D4670;
extern const u8 ov00_02216650[];
void ov00_021E9B38(void *);
void ov00_021E92E4(Context *);
u32 ov00_021E933C(Context *);
int ov00_021E926C(Context *), ov00_021E9350(Context *);
void CPS_SocUse(void), CPS_SocRelease(void), CPS_SocUnRegister(void), CPS_SocFlush(void), CPS_TcpShutdown(void), CPS_TcpClose(void);
void CPS_SetRootCa(const void *, int), CPS_SetSsl(int), CPS_SocBind(int, u16, u32);
int CPS_TcpConnect(void), CPS_SocWrite(const void *, u32), CPS_SocGetLength(void);
void *CPS_SocRead(int *);
void CPS_SocConsume(u32);
void ov00_021E9438(Context *);

void ov00_021E9438(Context *c) {
    SocketView *soc = &c->socket;
    SSLView *ssl = &c->ssl;
    BufferView *b = &c->buffer;
    int timeout = c->timeout;
    int parsed = 0;
    u32 ip;
    int length;
    u64 last;
    if (timeout <= 0) {
        timeout = 60000;
    }
    ov00_021E92E4(c);
    ip = ov00_021E933C(c);
    if (!ip) {
        c->state = 2;
        return;
    }
    c->ip = ip;
    CPS_SocUse();
    if (c->secure == 1) {
        MI_CpuFill8(ssl, 0, 0x830);
        ssl->callback = ov00_021E9B38;
        ssl->arg = c->arg;
        soc->ssl = ssl;
        CPS_SetRootCa(ov00_02216650, 12);
        CPS_SetSsl(1);
    }
    CPS_SocBind(0, c->port, ip);
    if (CPS_TcpConnect() != 0) {
        c->state = 3;
        CPS_SocRelease();
        CPS_SocUnRegister();
        return;
    }
    {
        char *request = c->request;
        length = CPS_SocWrite(request, strlen(request));
    }
    if (length <= 0) {
        c->state = 5;
        goto fail;
    }
    CPS_SocFlush();
    if (!ov00_021E926C(c)) {
        c->state = 7;
        goto fail;
    }
    b->cursor = b->base;
    b->end = b->base + b->size;
    last = OS_GetTick();
    for (;;) {
        int take;
        void *data;
        if (!_021D4670) {
            c->state = 5;
            goto fail;
        }
        length = CPS_SocGetLength();
        if (length < 0) {
            break;
        }
        if (length > 0) {
            last = OS_GetTick();
            data = CPS_SocRead(&length);
            if (!data) {
                break;
            }
            take = length;
            if (take >= b->end - 1 - b->cursor) {
                take = b->end - 1 - b->cursor;
            }
            MI_CpuCopy8(data, b->cursor, take);
            b->cursor += take;
            *b->cursor = 0;
            if (parsed == 1) {
                OS_LockMutex((OSMutex *)c->mutex);
                c->count += take;
                OS_UnlockMutex((OSMutex *)c->mutex);
            } else {
                parsed = ov00_021E9350(c);
            }
            if ((u32)length > (u32)take) {
                CPS_SocConsume(length);
                break;
            }
            CPS_SocConsume(take);
        }
        if (c->expected >= 0 && c->count >= c->expected) {
            break;
        }
        if (((OS_GetTick() - last) << 6) / 33514 > (u64)(s64)timeout) {
            c->state = 6;
            goto fail;
        }
        if (!ov00_021E926C(c)) {
            c->state = 7;
            goto fail;
        }
    }
    CPS_TcpShutdown();
    CPS_TcpClose();
    CPS_SocRelease();
    CPS_SocUnRegister();
    c->state = 8;
    return;
fail:
    CPS_TcpShutdown();
    CPS_TcpClose();
    CPS_SocRelease();
    CPS_SocUnRegister();
}
