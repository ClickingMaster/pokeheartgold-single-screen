#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(volatile s16 *)((u8 *)(p) + (o)))
#define S8(p, o)  (*(s8 *)((u8 *)(p) + (o)))
extern u8 *_02110628;
extern void CPS_TcpShutdown(void);
extern void CPS_TcpClose(void);
extern void CPS_SocRelease(void);
extern void CPS_SocUnRegister(void);
extern void SOCLi_SendCommandPacket(void *, void *);
extern void SOCLi_SocketUnregister(void *);
extern void SOCLi_SocketRegisterTrash(void *);
extern void SOCLi_FreeCommandPacket(void *);
extern int SOCLi_CloseCallBack(void *);
extern void SOCLi_FreeCommandPipe(void *);
extern int SOCL_SocketIsInTrash(void *);
extern int SOCL_SocketIsInvalid(void *);
extern void *SOCLi_CreateCommandPacket(int (*)(void *), void *, int);
extern void SOCLi_SendCommandPacketToCtrlPipe(void *, void *);
extern void SOCLi_SocketUnregisterTrash(void *);
extern int SOCL_Close(int);
extern void SOCLi_CleanupSocket(void *);

static inline BOOL isStreamSocket(void *sock) {
    int type = S8(sock, 0x73);
    return type == 0 || type == 4;
}

void SOCLi_FreeCommandPipe(void *pipe) {
    OSMessage message;
    OSIntrMode mode;
    if (!pipe) {
        return;
    }
    OS_JoinThread((void *)((u8 *)pipe + 0x20));
    mode = OS_DisableInterrupts();
    OS_DisableScheduler();
    while (OS_ReceiveMessage(pipe, &message, 0)) {
        if (message) {
            OSMessageQueue *reply = (void *)U32(message, 8);
            if (reply) {
                OS_SendMessage(reply, (OSMessage)-11, 0);
            }
            SOCLi_FreeCommandPacket(message);
        }
    }
    OS_EnableScheduler();
    OS_RescheduleThread();
    OS_RestoreInterrupts(mode);
}
