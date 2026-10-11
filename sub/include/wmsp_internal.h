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
#include <nitro/os/valarm.h>

// Parent parameters given by the ARM9 (WMSP_SetParentParam).
typedef struct WmspParentParam {
    void *userGameInfo; // ARM9 buffer (WMSP_SetGameInfo)
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

// Send queue entry (WMSP_PutSendQueue); the entries are linked through `next` (0xFFFF: end).
typedef struct WmspSendEntry {
    u16 next;
    u16 port;
    u16 destBitmap;
    u16 restBitmap; // destinations still to send to
    u16 sentBitmap;
    u16 inFlightBitmap; // destinations of the MP frame that carries the entry
    u16 unk_0c;
    u16 size;
    u16 seqNo;      // port bit 3: sequence number << 1 (0xFFFF: none assigned yet)
    u16 retryCount; // resends left after a failed MP frame (port bit 3: unlimited)
    u16 *data;
    void *callback;
    void *arg;
} WmspSendEntry;

// Head and tail entry index of a send queue (0xFFFF: empty).
typedef struct WmspSendQueue {
    u16 head;
    u16 tail;
} WmspSendQueue;

// Wireless-manager state shared with the ARM9 (wmspW.status).
typedef struct WmspStatus {
    u16 state;
    u16 requestCommand; // request being run by WMSP_RequestThread
    u32 requestBusy;    // 1 while WMSP_RequestThread runs a request
    u32 unk_08;
    u32 mpActive;
    u32 dcfActive; // 1 from WMSP_StartDCF until WMSP_EndDCF
    u32 unk_14;
    u32 unk_18; // cleared by WMSP_StartDCF, set by WMSP_SetDCFData
    u32 unk_1c; // child: 1 once the VSync adjustment is done (WmspChildAdjustVSync1/2)
    u8 unk_20[0x30 - 0x20];
    u16 parentSize;
    u16 childSize;
    u16 parentMaxSize;
    u16 childMaxSize;
    u16 sendSize;
    u16 recvSize;
    u16 sendMaxSize;
    u16 recvMaxSize;
    s16 parentVAlarmLine; // VCOUNT line of the parent's MP VAlarm (WmspParentVAlarmMP)
    s16 childVAlarmLine;  // VCOUNT line of the child's MP VAlarm (WmspChildVAlarmMP)
    u16 parentInterval;
    u16 childInterval;
    OSTick parentIntervalTick;
    OSTick childIntervalTick;
    u16 unk_58;
    u16 unk_5a;
    u16 unk_5c;
    u16 unk_5e;
    u16 unk_60;
    s16 unk_62;
    s16 unk_64;
    u16 resumePending; // set once WMSP_RequestResumeMP queued its request
    u16 unk_68;
    u16 unk_6a;
    u16 unk_6c;
    u16 unk_6e;
    u16 mpBufIndex;
    u16 mpBufSize;
    u8 *mpBuf[2];
    u8 *sendBuf; // WMSP_StartMP: ARM9 buffer of the MP send data
    u16 sendBufSize;
    u16 unk_82;
    u16 mpAckAlarmSet; // 1 while wmspMPAckAlarm is set
    u16 unk_86;
    u16 unk_88;
    u16 unk_8a;
    u16 unk_8c;
    u16 unk_8e;
    u16 unk_90;
    u16 unk_92;
    u16 unk_94;
    u16 unk_96;
    u16 unk_98;
    u16 unk_9a;
    u16 unk_9c; // non-zero: WMSP_StartMP does not check the buffer sizes
    u16 unk_9e;
    u16 unk_a0;
    u8 dcfDestAddress[6]; // WMSP_SetDCFData: destination of the DCF frame
    u16 *dcfData;
    u16 dcfDataSize;
    u16 rxBufIndex;
    u8 *rxBuf[2];
    u16 rxBufSize; // size of each rxBuf (WMSP_StartDCF)
    u16 unk_ba;
    u16 linkLevel;
    u16 minRssi;
    u16 linkLevelCountdown; // MP frames until linkLevel is updated from minRssi
    u16 unk_c2;
    u16 wepKeyId;
    u16 pwrMgtMode; // WMSP_StartParent: argument of WMSP_WL_MlmePowerManagement
    u32 unk_c8;
    u16 unk_cc;
    u16 vAlarmRequestQueued; // 1 while the VAlarm's request 28 (WMSP_VAlarmSetMPData) is queued
    u32 vsyncTime;           // child: VSync time stamp of the parent, then the next VSync after now
    u32 lastVSyncWord;       // child: last WMSP_VSYNC_TIME value used
    u32 vsyncAdjust;         // child: time to the parent's next VSync (0: none)
    u16 vcountResyncCount;   // parent: VAlarms since VCOUNT was last rewritten
    u16 unk_de;
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
    u16 unk_1ec; // 1: 1 Mbps, 2: 2 Mbps (from the parent's basic rate set)
    u16 unk_1ee; // preamble type last set with WMSP_WL_ParamSetPreambleType
    u8 unk_1f0[0x1f4 - 0x1f0];
    u16 enableChannel;    // bit n: channel n usable (WMSP_SetParentParam)
    u16 allowedChannel;   // bit n-1: channel n allowed (WMSP_StartParent)
    u16 portSeqNo[16][8]; // per aid and port 0-7: next sequence number (bit 0 set: none yet)
    WmspSendEntry sendEntry[32];
    WmspSendQueue sendFreeQueue;
    WmspSendQueue sentQueue[4]; // per priority: entries of the MP frame being sent
    WmspSendQueue sendQueue[4]; // per priority (WMSP_PutSendQueue)
    OSMutex sendQueueMutex;
    u32 sendInFlight;        // 1 from WMSP_SendMaMP / WMSP_SendMaKeyData until WMSP_FlushSendQueue
    OSTick lastRecvTick[16]; // per aid; 0: not tracked
    OSTick recvTimeout;
    u16 unk_7c0;
    s16 unk_7c2;
    u16 unk_7c4;
    u16 unk_7c6;
    u16 unk_7c8;
    u16 sendRetryCount; // retryCount of new send queue entries
    u16 unk_7cc;
} WmspStatus;

// BSS description of a parent, as reported by a scan.
typedef struct WmspBssDesc {
    u16 length; // in half-words
    u16 rssi;
    u16 bssid[3];
    u16 ssidLength;
    u8 ssid[32];
    u16 capaInfo; // bit 5: short preamble

    struct {
        u16 basic; // bit 0: 1 Mbps, bit 1: 2 Mbps
        u16 support;
    } rateSet;

    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
    u16 cfpPeriod;
    u16 cfpMaxDuration;
    u16 gameInfoLength;
    u16 unk_3e;
} WmspBssDesc;

// Buffer set up by the ARM9; 0xD0: pool of 32 16-byte request buffers (bit 15 of the first word: free).
typedef struct WmspArm9Buf {
    u8 unk_00[0x10];
    WmspBssDesc bssDesc; // parent to connect to, followed by its game info (WMSP_StartConnectEx)
    u8 unk_50[3];
    u8 unk_53; // 8: use the higher RSSI thresholds in WMSP_GetLinkLevel
    u32 ggid;
    u16 tgid;
    u8 unk_5a;
    u8 attribute; // bit 0: entry allowed, bit 2: entry flag (adds the entry header to the MP sizes)
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 unk_60[0xd0 - 0x60];
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

// ARM9 request of WMSP_StartScanEx.
typedef struct WmspStartScanExRequest {
    u16 command;
    u16 channels; // bit n-1: scan channel n
    void *output; // buffer for the BSS descriptions (4-byte aligned, at least 64 bytes)
    u16 capacity;
    u16 duration; // maximum scan time per channel
    u8 bssid[6];
    u16 mode; // 0/1: scan type, 2/3: the same with the SSID match mask
    u16 length;
    u8 ssid[32];
    u16 matchlen; // leading SSID bytes to match (mode 2/3)
} WmspStartScanExRequest;

// ARM9 request of WMSP_StartConnectEx.
typedef struct WmspStartConnectExRequest {
    u32 command;
    const void *parent; // 0xC0 bytes: WmspBssDesc and game info, copied to arm9Buf->bssDesc
    u8 ssid[24];        // user part of the SSID
    u32 powerSave;
    u16 unk_24;
    u16 authMode;
} WmspStartConnectExRequest;

// ARM9 request of WMSP_StartMP.
typedef struct WmspStartMPRequest {
    u32 command;
    u8 *recvBuf; // two halves of recvBufSize bytes
    u32 recvBufSize;
    u8 *sendBuf;
    u32 sendBufSize;
    u8 param[0x1c]; // given to WMSP_SetMPParameterCore
    u32 flags;      // bit n: take the matching field below instead of the status default
    u16 unk_34;
    u16 unk_36;
    u16 unk_38;
    u16 unk_3a;
    u8 unk_3c;
    u8 unk_3d;
    u8 unk_3e;
} WmspStartMPRequest;

// ARM9 request of WMSP_SetMPData: the WMSP_PutSendQueue arguments.
typedef struct WmspSetMPDataRequest {
    u32 command;
    u16 *data;
    u32 size;
    u32 destBitmap;
    u32 port;
    u32 priority;
    void *callback;
    void *arg;
} WmspSetMPDataRequest;

// Callback of a port send (indication 129, api 20) to the ARM9.
typedef struct WmspPortSendCallback {
    u16 command;
    u16 result;
    u16 unk_04[2];
    u16 api;
    u16 port;
    u16 destBitmap;
    u16 failedBitmap; // destinations not reached (result 10: the connected ones, queue full)
    u16 sentBitmap;
    u16 unk_12;
    u16 *data;
    u16 size;
    u16 seqNo;
    void *callback;
    void *arg;
    u16 sendSize; // own MP size: parentSize on the parent, childSize on a child
    u16 recvSize; // the other side's
} WmspPortSendCallback;

// Callback of a received port packet (indication 130, api 21) to the ARM9.
typedef struct WmspPortRecvCallback {
    u16 command;
    u16 result;
    u16 api;
    u16 port;
    void *context; // the MP receive buffer the packet is in
    u16 *data;
    u16 size;
    u16 srcAid;
    u8 unk_14[6];
    u16 seqNo; // 0xFFFF: none
    u8 unk_1c[4];
    u16 destAid; // own aid
    u8 unk_22[0x40 - 0x22];
    u16 sendSize; // own MP size: parentSize on the parent, childSize on a child
    u16 recvSize; // the other side's
} WmspPortRecvCallback;

extern WmspWork wmspW;
extern OSThread wmspIndicateThread;
extern OSThread wmspRequestThread;
extern OSAlarm wmspMPIntervalAlarm;
extern OSAlarm wmspMPAckAlarm;
extern OSVAlarm wmspVAlarm; // MP timing alarm (WMSP_SetVAlarm)

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
void __VENEER_OS_CancelVAlarm(OSVAlarm *alarm);
void __VENEER_OS_SetVAlarm(OSVAlarm *alarm, s16 count, s16 delay, OSVAlarmHandler handler, void *arg);
void __VENEER_OS_LockMutex(OSMutex *mutex);
void __VENEER_OS_UnlockMutex(OSMutex *mutex);
u32 __VENEER_MATH_CountPopulation(u32 value);
u32 MATH_CountPopulation(u32 value);
void __VENEER_OS_Sleep(u32 msec);

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
u32 __VENEER__u32_div_f(u32 dividend, u32 divisor);
u32 _u32_div_f(u32 dividend, u32 divisor);
void WmspError_06003ED4(u16 command, u16 result, BOOL scanEx);
void WMSP_EndScan(void *request);
void WmspError_06003FF0(u16 command, u16 result);

// wmsp_disconnect.c
s32 __VENEER__s32_div_f(s32 dividend, s32 divisor);
s32 _s32_div_f(s32 dividend, s32 divisor);
void WmspError_060045D0(u16 command, u16 result, u16 detail);
void WMSP_Disconnect(u32 *request);

// wmsp_disconnect_ind.c
void WMSP_IndicateDisconnectionFromMyself(BOOL parent, u16 aid, void *mac);
void WmspError_06004CB4(u16 command, u16 result, u16 aidBitmap, u16 disconnected);
void WmspIndError(u16 a, u16 b, u16 c, u16 d);

// wmsp_param.c
void WMSP_EndMP(void *request);
void WmspError_06005328(u16 command, u16 result);
void WMSP_StartDCF(u32 *request);
void WMSP_SetDCFData(u32 *request);
void WMSP_EndDCF(void *request);
void WMSP_SetWEPKey(u32 *request);
void WMSP_SetWEPKeyEx(u32 *request);
void WMSP_SetGameInfo(u32 *request);
void WMSP_SetBeaconTxRxInd(u32 *request);
void WMSP_StartTestMode(void *request);
void WMSP_StopTestMode(void *request);
void WMSP_SetLifeTime(u32 *request);
void WMSP_MeasureChannel(u16 *request);
void WmspError_06005BB8(u16 command, u16 result);
void WMSP_InitWirelessCounter(void *request);
void WMSP_GetWirelessCounter(void *request);

// wmsp_valarm.c
void WMSP_InitVAlarm(void);
void WMSP_CancelVAlarm(void);
void WMSP_SetVAlarm(void);
void WmspChildAdjustVSync1(void *arg);
void WmspChildAdjustVSync2(void *arg);
void WmspChildVAlarmMP(void *arg);
void WmspParentAdjustVSync(void *arg);
void WmspParentVAlarmMP(void *arg);
void WmspFromVAlarmToWmspThread(void);
void WMSP_VAlarmSetMPData(void *request);
void WMSP_KickNextMP_Parent(u32 *request);
void WMSP_KickNextMP_Child(void *request);
void WMSP_KickNextMP_Resume(u32 *request);

// wmsp_send_queue_init.c
void WMSP_InitSendQueue(void);

// wmsp_reset.c
void WMSP_Reset(void);

// wmsp_start_parent.c
void WMSP_StartParent(u32 *request);

// wmsp_start_scan_ex.c
void WMSP_StartScanEx(WmspStartScanExRequest *request);

// wmsp_start_connect_ex.c
void WMSP_StartConnectEx(WmspStartConnectExRequest *request);

// wmsp_disconnect_core.c
BOOL WMSP_DisconnectCore(u32 *request, BOOL indication, u16 *disconnected);

// wmsp_start_mp.c
void WMSP_StartMP(WmspStartMPRequest *request);

// wmsp_set_mp_data.c
void WMSP_SetMPData(WmspSetMPDataRequest *request);

// wmsp_send_queue.c
void WMSP_ResumeMaMP(u32 bitmap);
u32 WMSP_PutSendQueue(u32 childBitmap, u32 priority, u32 port, u32 destBitmap, u16 *data, u16 size, void *callback, void *arg);

// wmsp_send_ma.c
void WMSP_SendMaKeyData(void);
void WMSP_SendMaMP(u16 bitmap);

// wmsp_send_queue_flush.c
int WMSP_FlushSendQueue(int retransmit, u16 failedBitmap);
void WMSP_CleanSendQueue(u32 aidBitmap);
void WMSP_ParsePortPacket(u16 srcAid, u16 header, void *data, s32 size, void *context);

// wmsp_get_tmptt.c
u16 WmspGetTmptt(u32 headerSize, u32 size, u32 bitmap, u32 line);

// defined in the later WMSP files
u32 WMSP_SetMPParameterCore(void *param, void *result);
u16 *WMSP_GetBuffer4Callback2Wm9(void);
void WMSP_ReturnResult2Wm9(void *result);
void WMSPi_CommonInit(u32 arg);
BOOL WMSPi_CommonWlIdle(u16 *result, u16 *wlResult);

#endif // WMSP_INTERNAL_H
