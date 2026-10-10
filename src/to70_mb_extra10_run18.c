#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
extern u8 *mbc;
extern u8 ov13_022474A0[];
extern u8 ov13_0224CF40[];
extern u8 ov13_0224CF84[];
extern u8 ov13_0224CF80[];
extern int MBi_StartParentCore(int);
extern int MBi_CallReset(void);
extern void MBi_CommEnd(void);
extern void MBi_SetTask(void *, void *, void *, int);
extern void MBi_MakeParentSendBuffer(void *, void *);
extern int MBi_SendMP(void *, int, u16);
void MB_CommSetParentStateCallback(void *cb);
void MBi_CommChangeParentStateCallbackOnly(u32 a, u32 b, void *c);
u32 MBi_calc_nextsendblock(u32 a, u32 b);
int IsChildAidValid(u32 a);
int MBi_BlockHeaderEnd(int len, u16 aid, void *p);
u16 MBi_calc_cksum(const u16 *p, int size);
int mystrlen(const u16 *p);
void MBi_ClearSendStatus(void);
void MBi_InitSendVolatBeacon(void);
u32 MBi_GetBeaconPeriodDispersion(void);
void MBi_SetMaxScanTime(u16 x);
u32 MBi_GetGgid(void);
u16 MBi_GetTgid(void);
int MBi_IsStarted(void);
void MBi_InitCache(void *p);
int MBi_IsTaskAvailable(void);
void MBi_InitTaskInfo(void *p);
int MBi_IsTaskBusy(void *p);
void MBi_SetParentPieceBuffer(void *p);
void MBi_ClearParentPieceBuffer(u32 aid);
void MBi_SetChildMPMaxSize(int size);
int IsGetAllRequestData(u32 aid);
void *MB_CommGetChildUser(u32 aid);
int MB_CommIsBootable(u32 aid);
void MBi_CommChangeParentState(u32 aid, u32 state, void *p);
void MBi_CommCallParentError(u32 aid, u32 err);
int MBi_CommParentSendMsg(u32 msg, u16 aid);
void MBi_InitSendFixedBeacon(void);
void MBi_EndCommon(void *arg);
void MBi_CheckWmErrcode(u16 api, u16 err);
void MB_InitSendGameInfoStatus(void);
int MB_StartParentFromIdle(int x);
int MBi_OnReset(void);
void MB_End(void);
u8 MBi_GetAttribute(void);
void MB_AddGameInfo(void *p);
int MBi_IsCommSizeValid(u32 p, u32 c, int n);
void MBi_EndTaskThread(void *cb);
void MBi_AttachCacheBuffer(void *p, u32 a, u32 b, u32 c, u32 d);
u32 MB_GetSegmentLength(FSFile *file);
void MBi_ReadSegmentHeader(void *p, u32 start, u32 end, int clear);
int MBi_ReadIconInfo(const char *name, void *out, int icon);
int MBi_ReadFromCache(void *p, u32 off, void *out, u32 len);
void MB_UpdateGameInfoMember(void *p, void *src, u32 bits, u32 value);

void MBi_EndTaskThread(void *cb) {
    OSIntrMode x = OS_DisableInterrupts();
    if (MBi_IsTaskAvailable()) {
        MBi_SetTask((u8 *)U32(ov13_0224CF80, 0) + 0xc4, 0, cb, 0);
    }
    OS_RestoreInterrupts(x);
}

void MBi_SetChildMPMaxSize(int size) {
    U32(ov13_0224CF84, 8) = size - 2;
    U32(ov13_0224CF84, 12) = 30 / (size - 2);
    U32(ov13_0224CF84, 16) = 30;
}

void MBi_SetParentPieceBuffer(void *p) {
    U32(ov13_0224CF84, 4) = (u32)p;
    MI_CpuFill8(p, 0, 0x21c);
}

void MBi_ClearParentPieceBuffer(u32 aid) {
    u8 *p = (void *)U32(ov13_0224CF84, 4);
    u32 x;
    if (!p) {
        return;
    }
    x = aid - 1;
    MI_CpuFill8(p + (x << 5), 0, 0x1e);
    U32((void *)U32(ov13_0224CF84, 4), 0x1e0 + (x << 2)) = 0;
}
