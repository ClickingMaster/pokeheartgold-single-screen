#include <nitro/code32.h>
#include <nitro/types.h>

extern u8 _021D4464[];
extern u8 _021D4460[];
extern void *_02110628;
int OS_ReceiveMessage(void *queue, void **out, int flags);
int OS_SendMessage(void *queue, void *message, int flags);
void OS_InitMessageQueue(void *queue, void **array, int count);
int OS_ReadMessage(void *queue, void **out, int flags);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 previous);
u32 OS_DisableScheduler(void);
u32 OS_EnableScheduler(void);

int SOCLi_CleanupCommandPacketQueue(void);
int SOCLi_StartupCommandPacketQueue(int count);
void SOCLi_CommandPacketHandler(void *queue);
void *SOCLi_AllocCommandPacket(int flags);
void *SOCLi_CreateCommandPacket(void *callback, void *socket, int flags);
void SOCLi_FreeCommandPacket(void *packet);
void *SOCLi_GetCtrlPipe(void *socket);
int SOCLi_ExecCommandPacket(void *pipe, void *packet);
int SOCLi_ExecCommandPacketInRecvPipe(void *socket, void *packet);
int SOCLi_ExecCommandPacketInSendPipe(void *socket, void *packet);
int SOCLi_ExecCommandPacketInCtrlPipe(void *socket, void *packet);
int SOCLi_SendCommandPacket(void *pipe, void *packet);
int SOCLi_SendCommandPacketToCtrlPipe(void *socket, void *packet);

typedef struct CommandExecFrame {
    void *array[1];
    void *result;
    u8 queue[0x20];
} CommandExecFrame;

int SOCLi_StartupCommandPacketQueue(int count) {
    int packetBytes = (count * 0x2c + 3) & ~3;
    int arrayBytes = ((count << 2) + 3) & ~3;
    u8 *base;
    u8 *packet;
    void *(*allocFn)(int) = *(void *(**)(int))((u8 *)_02110628 + 0x18);
    base = allocFn(packetBytes + arrayBytes);
    if (base == 0) {
        return -1;
    }
    OS_InitMessageQueue(_021D4464, (void **)base, count);
    packet = base + arrayBytes;
    while (count > 0) {
        SOCLi_FreeCommandPacket(packet);
        count--;
        packet += 0x2c;
    }
    *(void **)_021D4460 = base;
    return 0;
}

int SOCLi_CleanupCommandPacketQueue(void) {
    void (*freeFn)(void *);
    if (*(int *)(_021D4460 + 0x20) < *(int *)(_021D4460 + 0x18)) {
        return -1;
    }
    freeFn = *(void (**)(void *))((u8 *)_02110628 + 0x1c);
    freeFn(*(void **)_021D4460);
    *(void **)_021D4460 = 0;
    return 0;
}

void *SOCLi_AllocCommandPacket(int flags) {
    void *packet;
    if (OS_ReceiveMessage(_021D4464, &packet, flags)) {
        return packet;
    }
    return 0;
}

void *SOCLi_CreateCommandPacket(void *callback, void *socket, int flags) {
    u8 *packet = SOCLi_AllocCommandPacket(flags);
    if (packet == 0) {
        return packet;
    }
    *(void **)(packet + 0) = callback;
    *(void **)(packet + 4) = socket;
    *(void **)(packet + 8) = 0;
    packet[0xc] = *(s8 *)((u8 *)socket + 0x73);
    packet[0xd] = flags;
    return packet;
}

void SOCLi_FreeCommandPacket(void *packet) {
    if (packet != 0) {
        OS_SendMessage(_021D4464, packet, 0);
    }
}

void *SOCLi_GetCtrlPipe(void *socket) {
    void *pipe = *(void **)((u8 *)socket + 0x64);
    if (pipe == 0) {
        pipe = *(void **)((u8 *)socket + 0x68);
    }
    return pipe;
}

int SOCLi_SendCommandPacket(void *pipe, void *packet) {
    int flags;
    int result;
    if (packet == 0 || (((s8 *)packet)[0xd] & 1) != 0) {
        flags = 1;
    } else {
        flags = 0;
    }
    result = OS_SendMessage(pipe, packet, flags);
    if (result == 0) {
        SOCLi_FreeCommandPacket(packet);
    }
    return result ? 0 : -42;
}

int SOCLi_SendCommandPacketToCtrlPipe(void *socket, void *packet) {
    return SOCLi_SendCommandPacket(SOCLi_GetCtrlPipe(socket), packet);
}

int SOCLi_ExecCommandPacket(void *pipe, void *packet) {
    CommandExecFrame frame;
    if (*(s8 *)((u8 *)packet + 0xd) == 1) {
        OS_InitMessageQueue(frame.queue, frame.array, 1);
        *(void **)((u8 *)packet + 8) = frame.queue;
        SOCLi_SendCommandPacket(pipe, packet);
        OS_ReceiveMessage(frame.queue, &frame.result, 1);
    } else {
        *(void **)((u8 *)packet + 8) = 0;
        frame.result = (void *)SOCLi_SendCommandPacket(pipe, packet);
    }
    return (int)frame.result;
}

int SOCLi_ExecCommandPacketInRecvPipe(void *socket, void *packet) {
    return SOCLi_ExecCommandPacket(*(void **)((u8 *)socket + 0x64), packet);
}

int SOCLi_ExecCommandPacketInSendPipe(void *socket, void *packet) {
    return SOCLi_ExecCommandPacket(*(void **)((u8 *)socket + 0x68), packet);
}

int SOCLi_ExecCommandPacketInCtrlPipe(void *socket, void *packet) {
    return SOCLi_ExecCommandPacket(SOCLi_GetCtrlPipe(socket), packet);
}

void SOCLi_CommandPacketHandler(void *queue) {
    void *packet;
    int zero = 0;
    int blocking = 1;
    u32 previous;
    int result;
    for (;;) {
        OS_ReadMessage(queue, &packet, blocking);
        if (packet == 0) {
            return;
        }
        result = ((int (*)(void *)) * (void **)packet)(packet);
        previous = OS_DisableInterrupts();
        OS_DisableScheduler();
        OS_ReceiveMessage(queue, 0, zero);
        if (*(void **)((u8 *)packet + 4) != 0) {
            *(int *)((u8 *)*(void **)((u8 *)packet + 4) + 0x6c) = result;
        }
        if (*(void **)((u8 *)packet + 8) != 0) {
            OS_SendMessage(*(void **)((u8 *)packet + 8), (void *)result, zero);
        }
        SOCLi_FreeCommandPacket(packet);
        OS_EnableScheduler();
        OS_RestoreInterrupts(previous);
    }
}
