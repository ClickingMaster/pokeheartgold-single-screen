#include <nitro/code32.h>
#include <nitro/types.h>

void *SOCLi_StartupSocket(void *);
int SOCLi_GetSizeSocket(const void *);
void *SOCLi_InitSocket(void *, const void *);
void *SOCLi_InitCommandPipe(void *, void *, const void *);
void SOCLi_SocketRegister(void *);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32);
void MI_CpuFill8(void *, int, u32);
extern void *_02110628;
void *SOCLi_CreateCommandPacket(int (*)(void *), void *, int);
void SOCLi_ExecCommandPacketInCtrlPipe(void *, void *);
int SOCLi_CreateSocketCallBack(void *);
void CPS_SocRegister(void *);
void CPS_SocDup(void *);
void CPS_SocUse(void);
void CPS_SocDatagramMode(void);
void CPS_SetUdpCallback(void (*)(void));
void SOCLi_UdpRecvCallback(void);
int SOCLi_RoundUp4(int);
void OS_InitMessageQueue(void *, void *, int);
void OS_InitMutex(void *);
void OS_CreateThread(void *, void (*)(void *), void *, void *, u32, u32);
void OS_WakeupThreadDirect(void *);
void SOCLi_CommandPacketHandler(void *);
extern void *_021D442C;

void *SOCL_CreateSocket(void *config);
int SOCLi_GetSizeCommandPipe(const void *);
int SOCLi_GetSizeSocket(const void *);
void *SOCLi_InitSocketBuffer(void *, void *, u32);

void *SOCL_CreateSocket(void *config) {
    void *sock = SOCLi_StartupSocket(config);
    void *packet;
    if (sock == 0) {
        return (void *)-49;
    }
    packet = SOCLi_CreateCommandPacket(SOCLi_CreateSocketCallBack, sock, 1);
    SOCLi_ExecCommandPacketInCtrlPipe(sock, packet);
    return sock;
}

int SOCLi_CreateSocketCallBack(void *packet) {
    u8 *sock = *(u8 **)((u8 *)packet + 4);
    CPS_SocRegister(sock);
    void *soc = *(void **)(sock + 0x68);
    switch (*(s8 *)(sock + 0x73)) {
    case 0:
    case 4:
        CPS_SocDup((u8 *)soc + 0x20);
        CPS_SocUse();
        break;
    case 1:
        CPS_SocUse();
        CPS_SocDatagramMode();
        CPS_SetUdpCallback(SOCLi_UdpRecvCallback);
        break;
    case 2:
        CPS_SocDatagramMode();
        break;
    }
    *(u16 *)(sock + 0x70) = 1;
    return 0;
}

void *SOCLi_StartupSocket(void *config) {
    int size = SOCLi_GetSizeSocket(config);
    void *sock;
    u32 irq = OS_DisableInterrupts();
    sock = (*(void *(**)(int))((u8 *)_02110628 + 0x18))(size);
    if (sock != 0) {
        MI_CpuFill8(sock, 0, size);
        SOCLi_InitSocket(sock, config);
        SOCLi_SocketRegister(sock);
    }
    OS_RestoreInterrupts(irq);
    return sock;
}

int SOCLi_GetSizeSocket(const void *config) {
    const u8 *p = config;
    int size = 0x80;
    if (*(const u16 *)(p + 2) != 0) {
        size += 0x114;
        size += SOCLi_RoundUp4(*(const u16 *)(p + 2));
        size += SOCLi_RoundUp4(*(const u16 *)(p + 8));
        size += SOCLi_GetSizeCommandPipe(p + 0x10);
    }
    if (*(const u16 *)(p + 6) != 0) {
        size += 0x110;
        size += SOCLi_RoundUp4(*(const u16 *)(p + 6));
        size += SOCLi_RoundUp4(*(const u16 *)(p + 0xA));
        size += SOCLi_RoundUp4(*(const u16 *)(p + 0xC));
        size += SOCLi_GetSizeCommandPipe(p + 0x14);
    }
    return size;
}

int SOCLi_GetSizeCommandPipe(const void *config) {
    const u8 *p = config;
    int n = SOCLi_RoundUp4(p[3] * 4);
    return n + SOCLi_RoundUp4(*(const u16 *)p);
}

void *SOCLi_InitSocket(void *socket, const void *config) {
    u8 *sock = socket;
    const u8 *cfg = config;
    u8 *cursor = sock + 0x80;
    u8 *pipe;
    sock[0x73] = *(const s8 *)cfg;
    sock[0x72] = *(const s8 *)(cfg + 1);
    if (*(const u16 *)(cfg + 2) != 0) {
        *(void **)(sock + 0x64) = cursor;
        pipe = cursor;
        *(u16 *)(pipe + 0xfc) = *(const u16 *)(cfg + 4);
        cursor = SOCLi_InitCommandPipe(cursor + 0x114, pipe, cfg + 0x10);
        cursor = SOCLi_InitSocketBuffer(cursor, sock + 0x3c, *(const u16 *)(cfg + 2));
        cursor = SOCLi_InitSocketBuffer(cursor, sock + 0x50, *(const u16 *)(cfg + 8));
        *(u16 *)(pipe + 0x10a) = *(const u16 *)(cfg + 0xe);
        *(u32 *)(pipe + 0x110) = 0;
        *(u32 *)(pipe + 0x10c) = 0;
    }
    if (*(const u16 *)(cfg + 6) != 0) {
        *(void **)(sock + 0x68) = cursor;
        pipe = cursor;
        *(void **)(pipe + 0x10c) = sock;
        cursor = SOCLi_InitCommandPipe(cursor + 0x110, pipe, cfg + 0x14);
        cursor = SOCLi_InitSocketBuffer(cursor, sock + 0x48, *(const u16 *)(cfg + 6));
        cursor = SOCLi_InitSocketBuffer(cursor, sock + 0x58, *(const u16 *)(cfg + 0xa));
        cursor = SOCLi_InitSocketBuffer(cursor, pipe + 0xf8, *(const u16 *)(cfg + 0xc));
        *(u32 *)(pipe + 0x108) = 0;
        *(u32 *)(pipe + 0x104) = 0;
    } else {
        *(void **)(sock + 0x68) = *(void **)((u8 *)_021D442C + 0x68);
    }
    return cursor;
}

void *SOCLi_InitSocketBuffer(void *cursor, void *buffer, u32 size) {
    u8 *p = buffer;
    void *storage = cursor;
    if (size == 0) {
        storage = 0;
    }
    *(void **)(p + 4) = storage;
    *(u32 *)p = size;
    return (u8 *)cursor + SOCLi_RoundUp4(size);
}

void *SOCLi_InitCommandPipe(void *cursor, void *pipe, const void *config) {
    const u8 *p = config;
    int size = SOCLi_GetSizeCommandPipe(config);
    OS_InitMessageQueue(pipe, cursor, p[3]);
    OS_InitMutex((u8 *)pipe + 0xe0);
    OS_CreateThread((u8 *)pipe + 0x20, SOCLi_CommandPacketHandler, pipe, (u8 *)cursor + size, *(const u16 *)p, p[2]);
    OS_WakeupThreadDirect((u8 *)pipe + 0x20);
    return (u8 *)cursor + size;
}
