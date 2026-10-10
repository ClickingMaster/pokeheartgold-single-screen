#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0xf8];
    s32 size;
    u8 *buffer;
    u16 write, read;
    OSThreadQueue queue;
    void *pipe;
} Send;

typedef struct {
    u8 pad[0x48];
    s32 len;
    u8 *buffer;
    u8 pad2[0x18];
    Send *send;
    u8 pad3[4];
    s16 flags;
    s8 unused, type;
    u16 local, remote;
    u32 ip;
} Socket;

typedef struct {
    u32 unused;
    Socket *socket;
    u8 pad[5];
    u8 mode;
    u8 pad2[2];
    u8 *first;
    int firstLen;
    u8 *second;
    int secondLen;
    u16 write, pad3, local, remote;
    u32 ip;
} Packet;

int SOCLi_GetWriteBufferFreeSize(Socket *);
int SOCLi_ExecWriteCommand(Socket *, void *, int, int, u16, u32, int);
int SOCLi_WriteCallBack(Packet *);
extern Packet *SOCLi_CreateCommandPacket(int (*)(Packet *), void *, int);
extern int SOCLi_ExecCommandPacketInSendPipe(void *, Packet *);
extern u16 CPS_SocGetEport(void);
extern void CPS_SocBind(u16, u16, u32);
extern int SOCLi_GetOptimumSendBufLen(int);
extern int SOCLi_MemCpy(void *, int, Packet *);
extern int CPS_SocWrite(void *, int);
extern void CPS_TcpClose(void);
#pragma force_active on

int SOCLi_ExecWriteCommand(Socket *s, void *data, int len, int offset, u16 port, u32 ip, int flags) {
    register Send *w = s->send;
    Packet *p;
    int next, size;
    u8 *base;
    u16 old;
    p = SOCLi_CreateCommandPacket(SOCLi_WriteCallBack, w->pipe, flags);
    if (!p) {
        return -33;
    }
    if ((flags & 1) && s->type != 1) {
        p->mode = 3;
    } else {
        p->mode = 0;
    }
    base = w->buffer;
    size = w->size;
    next = offset + len;
    if (next < size) {
        p->first = base + offset;
        p->firstLen = len;
        p->second = NULL;
        offset = next;
        p->secondLen = 0;
    } else {
        p->first = base + offset;
        p->firstLen = size - offset;
        p->second = base;
        offset = len - p->firstLen;
        p->secondLen = offset;
        MI_CpuCopy8((u8 *)data + p->firstLen, p->second, offset);
    }
    MI_CpuCopy8(data, p->first, p->firstLen);
    old = w->write;
    p->write = offset;
    w->write = p->write;
    if (s->type == 1) {
        if (!s->local) {
            s->local = CPS_SocGetEport();
            *(u16 *)((u8 *)s + 10) = s->local;
        }
        p->local = s->local;
        if (!s->ip || ip) {
            p->ip = ip;
            p->remote = port;
        } else {
            p->ip = s->ip;
            p->remote = s->remote;
        }
    } else {
        p->ip = 0;
    }
    if (SOCLi_ExecCommandPacketInSendPipe(w->pipe, p)) {
        w->write = old;
        len = 0;
    }
    return len;
}
