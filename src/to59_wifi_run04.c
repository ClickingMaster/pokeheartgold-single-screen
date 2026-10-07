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
void *SOCLi_ReadCPSBuffer(void *socket, s32 *available, u16 *port, u16 *sourcePort, u32 *address);
int SOCLi_CopyCPSBuffer(void *socket, void *buffer, int size, u16 *port, u32 *address);
int SOCLi_ReadUdpBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);
int SOCLi_ReadBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);
int SOCLi_ReadAndConsumeBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 mode, u32 flags);
int SOCL_ReadFrom(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags);

int SOCLi_ReadAndConsumeBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 mode, u32 flags) {
    byte *soc = *(byte **)((byte *)socket + 0x64);
    int changed = (flags & 2) != 0 && soc != 0;
    s8 old;
    int result;
    if (changed) {
        old = *(s8 *)(soc + 0xfe);
        *(s8 *)(soc + 0xfe) = 1;
    }
    if (*(s8 *)((byte *)socket + 0x73) == 1) {
        result = SOCLi_ReadUdpBuffer(socket, buffer, size, port, address, mode);
    } else {
        result = SOCLi_ReadBuffer(socket, buffer, size, port, address, mode);
        if (result >= 0) {
            SOCLi_ConsumeBuffer(socket);
        }
    }
    if (changed) {
        *(s8 *)(soc + 0xfe) = old;
    }
    return result;
}

int SOCLi_ReadBuffer(void *socket, void *buffer, u32 size, u32 port, void *address, u32 flags) {
    if (*(s8 *)((byte *)socket + 0x73) == 4) {
        return SOCLi_ExecReadCommand(socket, buffer, size, port, address);
    }
    int result = SOCLi_CopyCPSBuffer(socket, buffer, (s32)size, (u16*)port, address);
    if (result != -6 || (flags & 1) == 0) {
        return result;
    }
    return SOCLi_ExecReadCommand(socket, buffer, size, port, address);
}
