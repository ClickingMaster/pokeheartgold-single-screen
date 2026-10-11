#ifndef WMSP_INTERNAL_H
#define WMSP_INTERNAL_H

// ARM7 wireless manager (WMSP): the work area, the status block shared with the ARM9 and the
// functions shared between its source files.

#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/mi/memory.h>
#include <nitro/os/alarm.h>
#include <nitro/os/alloc.h>
#include <nitro/os/message.h>
#include <nitro/os/mutex.h>
#include <nitro/os/system.h>
#include <nitro/os/thread.h>
#include <nitro/os/tick.h>

// Parent parameters given by the ARM9 (WMSP_SetParentParam).
typedef struct WmspParentParam {
    u8 unk_00[4];
    u16 userGameInfoLength;
    u16 unk_06;
    u32 ggid;
    u16 tgid;
    u16 unk_0e;
    u16 maxEntry; // passed to WMSP_WL_ParamSetMaxConnectableChild
    u16 unk_12;
    u16 entryFlag;
    u16 unk_16;
    u16 beaconPeriod;
    u8 unk_1a[0x32 - 0x1a];
    u16 channel;
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 unk_38[0x40 - 0x38];
} WmspParentParam;

// Wireless-manager state shared with the ARM9 (wmspW.status).
typedef struct WmspStatus {
    u16 state;
    u16 requestCommand; // request being run by WMSP_RequestThread
    u32 requestBusy;    // 1 while WMSP_RequestThread runs a request
    u32 unk_08;
    u32 mpActive;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u8 unk_20[0x30 - 0x20];
    u16 parentSize;
    u16 childSize;
    u16 parentMaxSize;
    u16 childMaxSize;
    u16 sendSize;
    u16 recvSize;
    u16 sendMaxSize;
    u16 recvMaxSize;
    u8 unk_40[0x44 - 0x40];
    u16 parentInterval;
    u16 childInterval;
    OSTick parentIntervalTick;
    OSTick childIntervalTick;
    u8 unk_58[0x5e - 0x58];
    u16 unk_5e;
    u16 unk_60;
    s16 unk_62;
    s16 unk_64;
    u16 resumePending; // set once WMSP_RequestResumeMP queued its request
    u16 unk_68;
    u8 unk_6a[0x70 - 0x6a];
    u16 mpBufIndex;
    u16 mpBufSize;
    u8 *mpBuf[2];
    u8 unk_7c[0x82 - 0x7c];
    u16 unk_82;
    u16 mpAckAlarmSet; // 1 while wmspMPAckAlarm is set
    u16 unk_86;
    u16 unk_88;
    u16 unk_8a;
    u16 unk_8c;
    u16 unk_8e;
    u16 unk_90;
    u8 unk_92[0xae - 0x92];
    u16 rxBufIndex;
    u8 *rxBuf[2];
    u16 unk_b8;
    u16 unk_ba;
    u16 linkLevel;
    u16 minRssi;
    u16 unk_c0;
    u16 unk_c2;
    u16 wepKeyId;
    u16 pwrMgtMode; // WMSP_StartParent: argument of WMSP_WL_MlmePowerManagement
    u32 unk_c8;
    u8 unk_cc[0xe0 - 0xcc];
    u8 macAddress[6];
    u16 unk_e6;
    WmspParentParam parentParam; // copied from the ARM9 by WMSP_SetParentParam
    u8 childMacAddress[15][6];
    u16 childBitmap; // bit n: child n (aid) is connected
    void *scanBuf;   // WMSP_StartScan: ARM9 buffer for the BSS description found
    u16 aid;         // 0: this station is the parent
    u8 bssid[6];
    u16 scanChannel;
    u8 unk_192[0x196 - 0x192];
    u16 wepMode; // WMSP_SetWEPKey: 0 none, 1-3 enabled
    u32 wepEnabled;
    u8 wepKey[0x50];
    u16 unk_1ec;
    u16 unk_1ee;
    u8 unk_1f0[0x1f4 - 0x1f0];
    u16 enableChannel;  // bit n: channel n usable (WMSP_SetParentParam)
    u16 allowedChannel; // bit n-1: channel n allowed (WMSP_StartParent)
    u8 unk_1f8[16][16];
    u8 unk_2f8[0x738 - 0x2f8];
    OSTick lastRecvTick[16]; // per aid; 0: not tracked
    OSTick recvTimeout;
    u8 unk_7c0[0x7cc - 0x7c0];
    u16 unk_7cc;
} WmspStatus;

// Buffer set up by the ARM9; 0xD0: pool of 32 16-byte request buffers (bit 15 of the first word: free).
typedef struct WmspArm9Buf {
    u8 unk_00[0x53];
    u8 unk_53; // 8: use the higher RSSI thresholds in WMSP_GetLinkLevel
} WmspArm9Buf;

// Thread priorities passed to WMSP_Init (config[1..6]).
typedef struct WmspPriorities {
    u32 indicateHigh;
    u32 wlHigh;
    u32 requestHigh;
    u32 indicateLow;
    u32 wlLow;
    u32 requestLow;
} WmspPriorities;

typedef struct WmspWork {
    OSMessageQueue requestQueue; // requests to the WL driver
    OSMessage requestMsgs[2];
    OSMessageQueue indicateQueue; // indications and replies of the WL driver
    OSMessage indicateMsgs[4];
    OSMessageQueue replyQueue; // replies of the WL driver, forwarded by WMSP_IndicateThread
    OSMessage replyMsgs[4];
    OSMessageQueue pxiQueue; // requests from the ARM9 and internal ones, run by WMSP_RequestThread
    OSMessage pxiMsgs[32];
    u8 requestStack[0x1000];
    u8 indicateStack[0x400];
    OSMutex mutex;
    u32 unk_1540;
    OSArenaId heapArena; // heap of the WL driver's buffers
    OSHeapHandle heapHandle;
    WmspArm9Buf *arm9Buf; // NULL until WMSP_Initialize
    WmspStatus *status;
    u8 rssiList[32];
    u32 rssiIndex;
    WmspPriorities priorities;
} WmspWork;

// Request block of the WL driver: header, then `paramLength` parameter words, then the reply
// (command, reply length, ...).
typedef struct WlRequestHeader {
    u16 unk_00[6];
    u16 command;
    u16 paramLength;
} WlRequestHeader;

extern WmspWork wmspW;
extern OSThread wmspIndicateThread;
extern OSThread wmspRequestThread;
extern OSAlarm wmspMPIntervalAlarm;
extern OSAlarm wmspMPAckAlarm;

// ARM veneers (ldr pc) to the functions they are named after.
void __VENEER_OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *msgArray, s32 msgCount);
void __VENEER_OS_InitMutex(OSMutex *mutex);
void __VENEER_OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void __VENEER_OS_WakeupThreadDirect(OSThread *thread);
void __VENEER_OS_ExitThread(void);
BOOL __VENEER_OS_IsVAlarmAvailable(void);
void __VENEER_OS_InitVAlarm(void);
void __VENEER_OS_CreateAlarm(OSAlarm *alarm);
void __VENEER_OS_CancelAlarm(OSAlarm *alarm);
void __VENEER_OS_SetAlarm(OSAlarm *alarm, OSTick tick, OSAlarmHandler handler, void *arg);
OSTick __VENEER_OS_GetTick(void);
void __VENEER_PXI_Init(void);
void __VENEER_PXI_SetFifoRecvCallback(int tag, void (*callback)(int, u16 *, BOOL));
BOOL __VENEER_OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL __VENEER_OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
void __VENEER_WMSP_ReturnResult2Wm9(void *result);
void __VENEER_SND_BeginSleep(void);
void __VENEER_OS_Terminate(void);
void __VENEER_MI_CpuCopy8(const void *src, void *dest, u32 size);
void __VENEER_MI_CpuFill8(void *dest, u8 data, u32 size);
void __VENEER_MIi_CpuCopy16(const void *src, void *dest, u32 size);
void __VENEER_MIi_CpuClear16(u16 data, void *dest, u32 size);
void *__VENEER_OS_AllocFromHeap(OSArenaId id, OSHeapHandle heap, u32 size);
void __VENEER_OS_FreeToHeap(OSArenaId id, OSHeapHandle heap, void *ptr);
OSIntrMode __VENEER_OS_DisableInterrupts(void);
OSIntrMode __VENEER_OS_RestoreInterrupts(OSIntrMode state);
u32 __VENEER_OS_DisableScheduler(void);
u32 __VENEER_OS_EnableScheduler(void);
BOOL __VENEER_OS_SetThreadPriority(OSThread *thread, u32 prio);

// wmsp_system.c
void WMSP_Init(u8 *driver, u32 *config);
void *WMSP_WlRequest(void *request);
void WmspPxiCallback(int tag, u16 *request, BOOL error);
BOOL WMSP_CheckMacAddress(const u8 *address);
void WMSP_CopyParentParam(u8 *dest, const u8 *src);
BOOL WMSP_SetAllParams(u32 command, u8 *buf);
void WMSP_AddRssiToList(u32 rssi);
void WMSP_FillRssiIntoList(u8 rssi);
u32 WMSP_GetAverageLinkLevel(void);
u32 WMSP_GetLinkLevel(u32 rssi);
void WMSP_SetThreadPriorityLow(void);
void WMSP_SetThreadPriorityHigh(void);
OSThread *WMSP_GetRequestThread(void);
OSThread *WMSP_GetIndicateThread(void);
void *WMSP_GetInternalRequestBuf(void);
void WMSP_ResetSizeVars(void);
void WMSP_SetParentMaxSize(u16 size);
void WMSP_SetChildMaxSize(u16 size);
void WMSP_SetParentSize(u32 size);
void WMSP_SetChildSize(u32 size);

// wmsp_indicate.c
void WMSP_IndicateThread(void *arg);
void WmspFreeBufOfWL(void *buf);
u8 WMSP_GetRssi8_06001B5C(int rssi);

// wmsp_alarm.c
void WMSP_RequestResumeMP(void);
void WmspMPParentIntervalAlarmCallback(void *arg);
void WmspKickMPParent(u16 bitmap);
void WmspMaMultiPollAckAlarmCallback(void *arg);
void WmspMPChildIntervalAlarmCallback(void *arg);
// Declared without a prototype: WMSP_IndicateThread calls it without the (unused) argument.
void WmspKickMPChild();
void WMSP_InitAlarm(void);
void WMSP_CancelAllAlarms(void);

// wmsp_request.c
void WMSP_RequestThread(void *arg);
void WmspRequestFuncDummy(void *request);
extern void (*WmspRequestFuncTable[])(void *request);

// wmsp_wl_command.c: build a WL driver request in `buf`, run it, return its reply.
u16 *WMSP_WL_MlmeReset(u16 *buf, u16 arg);
u16 *WMSP_WL_MlmePowerManagement(void *buf, u16 first, u16 second, u16 third);
u16 *WMSP_WL_MlmeScan(void *buf, u32 size, void *peer, u16 ssidLength, void *ssid, u16 scanType, void *channels, u16 dwell);
u16 *WMSP_WL_MlmeJoin(u16 *buf, u16 timeout, const void *bssDesc);
u16 *WMSP_WL_MlmeAuthenticate(void *buf, const void *peer, u16 algorithm, u16 timeout);
u16 *WMSP_WL_MlmeDeAuthenticate(u16 *buf, const void *peer, u16 reason);
u16 *WMSP_WL_MlmeAssociate(u16 *buf, const void *peer, u16 listenInterval, u16 timeout);
u16 *WMSP_WL_MlmeStart(void *buf, u16 ssidLength, void *ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, void *gameInfo);
u16 *WMSP_WL_MlmeMeasureChannel(void *buf, u16 a, u16 b, u16 duration, void *channels);
u16 *WMSP_WL_MaData(u16 *buf, u16 *frame);
u16 *WMSP_WL_MaKeyData(void *buf, u16 a, u16 b, void *data);
u16 *WMSP_WL_MaMp(void *buf, u32 a, u32 b, u32 c, u16 d, u16 e, u16 f, u16 g, u16 h, u32 encodedData);
u16 *WMSP_WL_MaClearData(void *buf, u16 a);
u16 *WMSP_WL_ParamSetAll(void *buf);
u16 *WMSP_WL_ParamSetWepKeyId(void *buf, u16 keyId);
u16 *WMSP_WL_ParamSetBeaconLostThreshold(void *buf, u16 threshold);
u16 *WMSP_WL_ParamSetSsidMask(void *buf, const void *mask);
u16 *WMSP_WL_ParamSetPreambleType(void *buf, u16 type);
u16 *WMSP_WL_ParamSetLifeTime(void *buf, u16 first, u16 second, u16 third);
u16 *WMSP_WL_ParamSetMaxConnectableChild(void *buf, u16 count);
u16 *WMSP_WL_ParamSetBeaconSendRecvInd(void *buf, u16 enable);
u16 *WMSP_WL_ParamSetNullKeyResponseMode(void *buf, u16 mode);
u16 *WMSP_WL_ParamSetBeaconPeriod(void *buf, u16 period);
u16 *WMSP_WL_ParamSetGameInfo(void *buf, u16 length, void *source);
u16 *WMSP_WL_ParamGetMacAddress(void *buf);
u16 *WMSP_WL_ParamGetEnableChannel(void *buf);
u16 *WMSP_WL_ParamGetMode(void *buf);
u16 *WMSP_WL_DevShutdown(void *buf);
u16 *WMSP_WL_DevIdle(void *buf);
u16 *WMSP_WL_DevClass1(void *buf);
u16 *WMSP_WL_DevRestart(void *buf);
u16 *WMSP_WL_DevSetInitializeWirelessCounter(void *buf);
u16 *WMSP_WL_DevGetVersion(void *buf);
u16 *WMSP_WL_DevGetWirelessCounter(void *buf);
u16 *WMSP_WL_DevGetStationState(void *buf);
u16 *WMSP_WL_DevTestSignal(void *buf, u16 first, u16 second, u16 third, u16 fourth);
u16 *WMSPi_WL_NoArg(u16 *buf, u16 command, u16 replyLength);

// wmsp_initialize.c
void WMSP_Initialize(u32 *request);

// wmsp_end.c
void WmspError_06002FC0(u16 command, u16 result);
void WMSP_End(void *request);
void __VENEER_PM_SetLEDPattern(int pattern);
void PM_SetLEDPattern(int pattern);
void WMSP_SetParentParam(u32 *request);

// wmsp_scan.c
void WmspError_060033F4(u16 command, u16 result);
void WMSP_EndParent(void *request);
void WmspError_060035F4(u16 command, u16 result);
void WMSP_StartScan(void *request);
u32 WMSP_GetRssi8_06003988(int rssi);
void WMSP_AddRssiToRandomPool(u32 rssi);

// wmsp_end_scan.c
void __VENEER__u32_div_f(void);
void _u32_div_f(void);
void WmspError_06003ED4(u16 command, u16 result, BOOL scanEx);
void WMSP_EndScan(void *request);
void WmspError_06003FF0(u16 command, u16 result);

// wmsp_disconnect.c
void __VENEER__s32_div_f(void);
void _s32_div_f(void);
void WmspError_060045D0(u16 command, u16 result, u16 detail);
void WMSP_Disconnect(u32 *request);

// wmsp_disconnect_ind.c
void WMSP_IndicateDisconnectionFromMyself(BOOL parent, u16 aid, void *mac);
void WmspError_06004CB4(u16 command, u16 result, u16 aidBitmap, u16 disconnected);
void WmspIndError(u16 a, u16 b, u16 c, u16 d);

// defined in the later WMSP files
BOOL WMSP_DisconnectCore(u32 *request, BOOL unk, u16 *result);
u16 *WMSP_GetBuffer4Callback2Wm9(void);
void WMSP_ReturnResult2Wm9(void *result);
void WMSP_CleanSendQueue(u16 aidBitmap);
void WMSP_CancelVAlarm(void);
int WMSP_FlushSendQueue(int timeout, u16 aidBitmap);
void WMSP_ParsePortPacket(u16 aid, u16 port, void *data, u16 length, void *buf);
void WMSPi_CommonInit(u32 arg);
BOOL WMSPi_CommonWlIdle(u16 *result, u16 *wlResult);

#endif // WMSP_INTERNAL_H
