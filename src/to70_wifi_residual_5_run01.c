#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S8(p, o)  (*(s8 *)((u8 *)(p) + (o)))
extern int SOCLi_GetWriteBufferFreeSize(void *);
extern int SOCLi_AllocWriteBuffer(void *, int, int, int *, int);
extern int SOCLi_UdpRecvCallback(const void *, u32, void *);
extern u8 *_02110628;
extern u32 _021D4484[];
extern int SOCLi_ExecWriteCommand(void *, const void *, int, int, u32, u32, int);
extern int SOCLi_WriteBuffer(void *, const void *, int, u32, u32, int);

int SOCLi_UdpRecvCallback(const void *data, u32 len, void *sock) {
    u8 *pipe = (void *)U32(sock, 0x64);
    u8 *p = NULL;
    OSIntrMode mode = OS_DisableInterrupts();
    if (U16(pipe, 0x10a) >= U16(pipe, 0x108) + len) {
        void *(*alloc)(int) = (void *)U32(_02110628, 0x18);
        if ((p = alloc(len + 12)) != NULL) {
            U16(pipe, 0x108) += len;
            U32(p, 0) = 0;
            U16(p, 4) = len;
            U16(p, 6) = U16(sock, 0x18);
            U32(p, 8) = U32(sock, 0x1c);
            MI_CpuCopy8(data, p + 12, len);
            if (*(volatile u32 *)(pipe + 0x100)) {
                U32((void *)U32(pipe, 0x100), 0) = (u32)p;
            }
            U32(pipe, 0x100) = (u32)p;
            if (!U32(pipe, 0x104)) {
                U32(pipe, 0x104) = (u32)p;
            }
        } else {
            ++_021D4484[0];
        }
    } else {
        ++_021D4484[1];
    }
    if (!U16(sock, 0x74)) {
        U16(sock, 0x74) = U16(sock, 0xa);
    }
    U16(sock, 0x18) = U16(sock, 0x1a);
    U32(sock, 0x1c) = U32(sock, 0x20);
    OS_WakeupThread((void *)(pipe + 0x10c));
    OS_RestoreInterrupts(mode);
    return 1;
}
