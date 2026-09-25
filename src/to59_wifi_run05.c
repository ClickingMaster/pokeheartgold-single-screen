#include <nitro/code32.h>
#include <nitro/types.h>

typedef unsigned char byte;

extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void CPS_SocConsume(void *);
extern int SOCL_SocketIsInvalid(void *);
extern int OS_GetProcMode(void);
extern int OS_TryLockMutex(void *);
extern void OS_LockMutex(void *);
extern void OS_UnlockMutex(void *);
extern void MI_CpuCopy8(const void *, void *, u32);
extern void *SOCLi_CreateCommandPacket(int (*)(void *), void *, int);
extern int SOCLi_ExecCommandPacketInRecvPipe(void *, void *);

int SOCLi_ConsumeCPSBuffer(void *socket);
int SOCLi_ConsumeCallBack(void *packet);
int SOCLi_ReadCallBack(void *packet);
int SOCLi_ConsumeBuffer(void *socket);
int SOCLi_ExecReadCommand(void *socket, void *buffer, u32 size, u32 port, void *address);
void *SOCLi_ReadCPSBuffer(void *socket, s32 *available, u16 *port, u16 *sourcePort, void **address);
int SOCLi_CopyCPSBuffer(void *socket, void *buffer, u32 size, u32 port, void *address);
int SOCLi_ReadUdpBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);
int SOCLi_ReadBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);
int SOCLi_ReadAndConsumeBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 mode, u32 flags);
int SOCL_ReadFrom(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);

void *SOCLi_ReadCPSBuffer(void *socket, s32 *available, u16 *port, u16 *sourcePort, void **address) {
    byte *soc = *(byte **)((byte *)socket + 0x64);
    byte *cp = *(byte **)(soc + 0xc4);
    s32 consumed = *(s32 *)(soc + 0xf8);
    s32 offset = *(s32 *)(cp + 0x44) - consumed;
    if (offset >= 0) {
        *port = *(u16 *)(cp + 0xa);
        *sourcePort = *(u16 *)(cp + 0x18);
        *address = *(void **)(cp + 0x1c);
        *available = offset;
        if (offset == 0 && cp[8] != 4) {
            return 0;
        }
    } else {
        *available = -1;
        return 0;
    }
    return *(byte **)(cp + 0x40) + consumed;
}

int SOCLi_ExecReadCommand(void *socket, void *buffer, u32 size, u32 port, void *address) {
    byte *packet = SOCLi_CreateCommandPacket(SOCLi_ReadCallBack, socket, 1);
    *(void **)(packet + 0x10) = buffer;
    *(u32 *)(packet + 0x14) = size;
    *(u32 *)(packet + 0x18) = port;
    *(void **)(packet + 0x1c) = address;
    return SOCLi_ExecCommandPacketInRecvPipe(socket, packet);
}
