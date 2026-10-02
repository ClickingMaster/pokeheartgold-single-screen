#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern u8 *mbc;
extern u8 ov13_022474A0[], ov13_0224CF40[], ov13_0224CF84[], ov13_0224CF80[];
extern int MBi_StartParentCore(int); extern int MBi_CallReset(void); extern void MBi_CommEnd(void);
extern void MBi_SetTask(void *,void *,void *,int);
extern void MBi_MakeParentSendBuffer(void *,void *);
extern int MBi_SendMP(void *,int,u16);
void MB_CommSetParentStateCallback(void *cb);
void MBi_CommChangeParentStateCallbackOnly(u32 a,u32 b,void *c);
u32 MBi_calc_nextsendblock(u32 a,u32 b);
int IsChildAidValid(u32 a);
int MBi_BlockHeaderEnd(int len,u16 aid,void *p);
u16 MBi_calc_cksum(const u16 *p,int size);
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
void MBi_CommChangeParentState(u32 aid,u32 state,void *p);
void MBi_CommCallParentError(u32 aid,u32 err);
int MBi_CommParentSendMsg(u32 msg,u16 aid);
void MBi_InitSendFixedBeacon(void);
void MBi_EndCommon(void *arg);
void MBi_CheckWmErrcode(u16 api,u16 err);
void MB_InitSendGameInfoStatus(void);
int MB_StartParentFromIdle(int x);
int MBi_OnReset(void);
void MB_End(void);
u8 MBi_GetAttribute(void);
void MB_AddGameInfo(void *p);
int MBi_IsCommSizeValid(u32 p,u32 c,int n);
void MBi_EndTaskThread(void *cb);
void MBi_AttachCacheBuffer(void *p,u32 a,u32 b,u32 c,u32 d);
u32 MB_GetSegmentLength(FSFile *file);
void MBi_ReadSegmentHeader(void *p,u32 start,u32 end,int clear);
int MBi_ReadIconInfo(const char *name,void *out,int icon);
int MBi_ReadFromCache(void *p,u32 off,void *out,u32 len);
void MB_UpdateGameInfoMember(void *p,void *src,u32 bits,u32 value);
int MBi_ReadIconInfo(const char *name,void *out,int icon) { FSFile file; u32 size=icon?0x200:0x20; if(icon) out=(u8 *)out+0x20; if(!name) return 0; FS_InitFile(&file); if(!FS_OpenFile(&file,name)) return 0; if(size!=U32(&file,0x28)-U32(&file,0x24)) { FS_CloseFile(&file); return 0; } FS_ReadFile(&file,out,size); FS_CloseFile(&file); return 1; }

void MB_UpdateGameInfoMember(void *p,void *src,u32 bits,u32 value) { u8 count=1; int i; MIi_CpuCopy16(src,(void *)((u8 *)p+0x35e),0x14a); for(i=0;i<15;i++) if(bits&(2<<i)) ++count; U8(p,0x358)=count; U16(p,0x35a)=bits|1; U16(p,0x35c)=value; ++U8(p,0x4b4); }

int mystrlen(const u16 *p) { int n=0; while(*p++) ++n; return n; }

void MB_AddGameInfo(void *p) { u8 *q=(void *)U32(ov13_022474A0,0x18); if(!q) U32(ov13_022474A0,0x18)=(u32)p; else { while(U32(q,0x4bc)) q=(void *)U32(q,0x4bc); U32(q,0x4bc)=(u32)p; } U32(p,0x4bc)=0; }

void MB_InitSendGameInfoStatus(void) { U32(ov13_022474A0,0x18)=0; U32(ov13_022474A0,0x1c)=0; U8(ov13_022474A0,0x24)=1; U32(ov13_022474A0,0x14)=0; MBi_ClearSendStatus(); }

void MBi_ClearSendStatus(void) { U8(ov13_022474A0,0x25)=0; U8(ov13_022474A0,0x26)=0; U8(ov13_022474A0,0x27)=0; U8(ov13_022474A0,0x28)=0; U8(ov13_022474A0,0x29)=0; }
