#include <nitro/code32.h>
#include <nitro/types.h>

int SOCL_SocketIsInvalid(void *socket);
void *SOCLi_CreateCommandPacket(int (*callback)(void *), void *socket, int flags);
int SOCLi_ExecCommandPacketInRecvPipe(void *socket, void *packet);
int SOCLi_BindCallBack(void *packet);
void OS_LockMutex(void *mutex);
void OS_UnlockMutex(void *mutex);
void CPS_SocBind(u16 port, u16 remotePort, u32 address);
int CPS_TcpConnect(void);
void CPS_TcpListen(void);
int CPS_SocWho(u16 *port, u16 *remotePort);
void *SOCL_CreateSocket(void *config);
extern u8 _02110648[];
extern int _0211062C;

int SOCL_Listen(void *socket);
int SOCL_Bind(void *socket, u16 port);
int SOCLi_ExecBindCommand(void *socket);
int SOCLi_BindCallBack(void *packet);
int SOCL_Accept(void *socket, u16 *port, u32 *address);
int SOCL_ListenAccept(void *socket, u16 *port, u32 *address);
int SOCLi_ListenAcceptCallBack(void *packet);
int SOCL_Connect(void *socket, u16 port, u32 address);

int SOCL_Listen(void *socket) {
    u8 *p = socket;
    int active;
    if (SOCL_SocketIsInvalid(socket)) {
        return -28;
    }
    active = 0;
    if (p == NULL) {
        goto inactive;
    }
    if (*(s16 *)(p + 0x70) & 1) {
        active = 1;
    }
inactive:
    if (!active) {
        return -39;
    }
    if (*(s16 *)(p + 0x70) & 2) {
        return -28;
    }
    active = 1;
    if (*(s8 *)(p + 0x73) != 0 && *(s8 *)(p + 0x73) != 4) {
        active = 0;
    }
    if (!active) {
        return -28;
    }
    if (*(s8 *)(p + 0x72) == 1) {
        return 0;
    }
    return -6;
}

int SOCL_Accept(void *socket, u16 *port, u32 *address) {
    u8 *p = socket;
    int active;
    void *newSocket;
    int result;
    if (SOCL_SocketIsInvalid(socket)) {
        return -28;
    }
    active = 0;
    if (p == NULL) {
        goto inactive;
    }
    if (*(s16 *)(p + 0x70) & 1) {
        active = 1;
    }
inactive:
    if (!active) {
        return -39;
    }
    if (*(s16 *)(p + 0x70) & 2) {
        return -28;
    }
    active = 1;
    if (*(s8 *)(p + 0x73) != 0 && *(s8 *)(p + 0x73) != 4) {
        active = 0;
    }
    if (!active) {
        return -28;
    }
    if (*(s8 *)(p + 0x72) != 1) {
        return -6;
    }
    newSocket = SOCL_CreateSocket(_02110648);
    if ((s32)newSocket < 0) {
        return (s32)newSocket;
    }
    result = SOCL_Bind(newSocket, *(u16 *)(p + 0x74));
    if (result < 0) {
        return result;
    }
    result = SOCL_ListenAccept(newSocket, port, address);
    if (result >= 0) {
        result = (s32)newSocket;
    }
    return result;
}

int SOCL_ListenAccept(void *socket, u16 *port, u32 *address) {
    u8 *p = socket;
    int active;
    u8 *packet;
    int mode;
    if (SOCL_SocketIsInvalid(socket)) {
        return -28;
    }
    active = 0;
    if (p == NULL) {
        goto inactive;
    }
    if (*(s16 *)(p + 0x70) & 1) {
        active = 1;
    }
inactive:
    if (!active) {
        return -39;
    }
    if (*(s16 *)(p + 0x70) & 2) {
        return -28;
    }
    active = 1;
    if (*(s8 *)(p + 0x73) != 0 && *(s8 *)(p + 0x73) != 4) {
        active = 0;
    }
    if (!active) {
        return -28;
    }
    mode = *(s8 *)(p + 0x72);
    if (mode != 1) {
        return -6;
    }
    if (*(u16 *)(p + 0x74) == 0) {
        return -28;
    }
    packet = SOCLi_CreateCommandPacket(SOCLi_ListenAcceptCallBack, socket, mode);
    *(u16 *)(packet + 0x10) = *(u16 *)(p + 0x74);
    *(u16 **)(packet + 0x14) = port;
    *(u32 **)(packet + 0x18) = address;
    *(u16 *)(p + 0x70) = *(s16 *)(p + 0x70) | 2;
    return SOCLi_ExecCommandPacketInRecvPipe(socket, packet);
}

int SOCLi_ListenAcceptCallBack(void *packet) {
    u8 *cmd = packet;
    u8 *socket = *(u8 **)(cmd + 4);
    u8 *soc = *(u8 **)(socket + 0x64);
    u32 remotePort;
    u32 localPort;
    int result;
    OS_LockMutex(soc + 0xE0);
    CPS_SocBind(*(u16 *)(cmd + 0x10), 0, 0);
    CPS_TcpListen();
    *(u32 *)(soc + 0xF8) = 0;
    result = CPS_SocWho((u16 *)&localPort, (u16 *)&remotePort);
    **(u16 **)(cmd + 0x14) = *(u16 *)&localPort;
    **(u32 **)(cmd + 0x18) = result;
    *(u16 *)(socket + 0x70) = *(s16 *)(socket + 0x70) | 4;
    OS_UnlockMutex(soc + 0xE0);
    return 0;
}

int SOCL_Bind(void *socket, u16 port) {
    u8 *p = socket;
    int active;
    if (SOCL_SocketIsInvalid(socket)) {
        return -28;
    }
    active = 0;
    if (p == NULL) {
        goto inactive;
    }
    if (*(s16 *)(p + 0x70) & 1) {
        active = 1;
    }
inactive:
    if (!active) {
        return -39;
    }
    if (*(s16 *)(p + 0x70) & 2) {
        return -7;
    }
    *(u16 *)(p + 0x74) = port;
    if (*(s8 *)(p + 0x73) != 1) {
        return 0;
    }
    return SOCLi_ExecBindCommand(socket);
}

int SOCL_Connect(void *socket, u16 port, u32 address) {
    u8 *p = socket;
    int active;
    int result;
    if (SOCL_SocketIsInvalid(socket) || (*(volatile s16 *)(p + 0x70) & 8)) {
        return -28;
    }
    active = 0;
    if (p == NULL) {
        goto inactive;
    }
    if (*(volatile s16 *)(p + 0x70) & 1) {
        active = 1;
    }
inactive:
    if (!active) {
        return -39;
    }
    active = 1;
    if (*(s8 *)(p + 0x73) != 0 && *(s8 *)(p + 0x73) != 4) {
        active = 0;
    }
    if (!active) {
        goto inactive_socket;
    }
    if (*(volatile s16 *)(p + 0x70) & 4) {
        return *(s8 *)(p + 0x72) == 1 ? -30 : 0;
    }
    if (*(volatile s16 *)(p + 0x70) & 2) {
        if (*(volatile s16 *)(p + 0x70) & 0x40) {
            return *(int *)(p + 0x6C);
        }
        return _0211062C;
    }
    *(u16 *)(p + 0x76) = port;
    *(u32 *)(p + 0x78) = address;
    result = SOCLi_ExecBindCommand(socket);
    if (*(s8 *)(p + 0x72) != 1) {
        return -26;
    }
    return result;
inactive_socket:
    *(u16 *)(p + 0x76) = port;
    *(u32 *)(p + 0x78) = address;
    return 0;
}

int SOCLi_ExecBindCommand(void *socket) {
    u8 *p = socket;
    u8 *packet = SOCLi_CreateCommandPacket(SOCLi_BindCallBack, socket, *(s8 *)(p + 0x72));
    if (packet == NULL) {
        return -33;
    }
    *(u16 *)(packet + 0x10) = *(u16 *)(p + 0x74);
    *(u16 *)(packet + 0x12) = *(u16 *)(p + 0x76);
    *(u32 *)(packet + 0x14) = *(u32 *)(p + 0x78);
    *(u16 *)(p + 0x70) = *(s16 *)(p + 0x70) | 2;
    return SOCLi_ExecCommandPacketInRecvPipe(socket, packet);
}

int SOCLi_BindCallBack(void *packet) {
    u8 *cmd = packet;
    u8 *socket = *(u8 **)(cmd + 4);
    u8 *soc = *(u8 **)(socket + 0x64);
    int result = 0;
    OS_LockMutex(soc + 0xE0);
    CPS_SocBind(*(u16 *)(cmd + 0x10), *(u16 *)(cmd + 0x12), *(u32 *)(cmd + 0x14));
    *(u32 *)(soc + 0xF8) = 0;
    if (*(s8 *)(cmd + 0x0C) == 0 || *(s8 *)(cmd + 0x0C) == 4) {
        result = CPS_TcpConnect();
    }
    OS_UnlockMutex(soc + 0xE0);
    if (result != 0) {
        *(volatile u16 *)(socket + 0x70) = *(volatile s16 *)(socket + 0x70) | 0x40;
        return -76;
    }
    *(volatile u16 *)(socket + 0x70) = *(volatile s16 *)(socket + 0x70) | 4;
    return 0;
}
