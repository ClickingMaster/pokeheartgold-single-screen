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

int SOCLi_ConsumeBuffer(void *socket) {
    byte *soc = *(byte **)((byte *)socket + 0x64);
    if (*(s32 *)(soc + 0xf8) < *(u16 *)(soc + 0xfc)) {
        return 0;
    }
    void *packet = SOCLi_CreateCommandPacket(SOCLi_ConsumeCallBack, socket, 0);
    if (packet == 0) {
        return -33;
    }
    return SOCLi_ExecCommandPacketInRecvPipe(socket, packet);
}

int SOCLi_ConsumeCallBack(void *packet) {
    return SOCLi_ConsumeCPSBuffer(*(void **)((byte *)packet + 4));
}

int SOCLi_ConsumeCPSBuffer(void *socket) {
    byte *soc = *(byte **)((byte *)socket + 0x64);
    u32 irq = OS_DisableInterrupts();
    void *buffer = *(void **)(soc + 0xf8);
    if (buffer != 0) {
        *(void **)(soc + 0xf8) = 0;
        CPS_SocConsume(buffer);
    }
    OS_RestoreInterrupts(irq);
    return (int)buffer;
}
