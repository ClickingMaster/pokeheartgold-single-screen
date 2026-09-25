typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;

extern void *_021D448C;
extern void *_021D4490;
extern u32 _021D4680[2];
extern u32 _021D4670;
extern u32 _021D4428;
extern u32 _021D4424;
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void CPS_TcpShutdown(void);
extern int OS_GetProcMode(void);
extern void OS_Sleep(int);
extern void SOCLi_CleanupSocket(u8 *);
extern int CPS_CalmDown(void);
extern void WCM_SetRecvDCFCallback(void *);
extern u32 CPS_Resolve(const char *);
extern int SOCLi_GetWriteBufferFreeSize(u8 *);
extern void *SOCLi_CreateCommandPacket(void *, void *, int);
extern int SOCLi_ExecCommandPacketInSendPipe(void *, void *);
void SOCLi_SocketRegisterList(void **head, u8 *socket);
void SOCLi_SocketRegister(u8 *socket);
void SOCLi_SocketRegisterTrash(u8 *socket);
void **SOCLi_SocketGetNextPtr(void **head, u8 *socket);
void SOCLi_SocketUnregisterList(void **head, u8 *socket);
void SOCLi_SocketUnregister(u8 *socket);
void SOCLi_SocketUnregisterTrash(u8 *socket);
int SOCL_SocketIsInvalid(u8 *socket);
int SOCLi_RoundUp4(int n);
int SOCLi_GetReadBufferOccpiedSize(u8 *socket);
int SOCL_Shutdown(u8 *socket);
int SOCLi_ShutdownCallBack(u8 *packet);
int SOCL_IsClosed(u8 *socket);
int SOCL_SocketIsInTrash(u8 *socket);
void SOCLi_TrashSocket(void);
int SOCL_SetResolver(u32, u32);
u32 SOCL_GetHostID(void);
int SOCL_CloseAll(void);
int SOCL_Close(u8 *);
int SOCL_CalmDown(void);
u32 SOCL_InetAtoH(const char *);
int SOCL_GetStatus(u8 *);
extern void *_021D442C;

u32 SOCL_InetAtoH(const char *name) {
    u32 interrupts = OS_DisableInterrupts();
    struct {
        u32 primary, secondary;
    } old = { _021D4680[0], _021D4680[1] };
    _021D4680[0] = 0;
    _021D4680[1] = 0;
    u32 result = CPS_Resolve(name);
    _021D4680[0] = old.primary;
    _021D4680[1] = old.secondary;
    OS_RestoreInterrupts(interrupts);
    return result;
}

int SOCL_SetResolver(u32 first, u32 second) {
    if (SOCL_GetHostID() == 0) {
        return -0x27;
    }
    _021D4680[0] = first;
    _021D4680[1] = second;
    return 0;
}

u32 SOCL_GetHostID(void) {
    u32 id = _021D4670;
    if (id == 0) {
        if ((_021D4428 & 3) == 1 && OS_GetProcMode() != 0x12) {
            OS_Sleep(10);
        }
    } else if (_021D4424 == 0) {
        _021D4424 = id;
    }
    return _021D4670;
}
