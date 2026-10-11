#ifndef WL_INTERNAL_H
#define WL_INTERNAL_H

// ARM7 wireless driver (WL): the driver's work area and the functions shared between its source files.

#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/mi/memory.h>
#include <nitro/os/alarm.h>
#include <nitro/os/interrupt.h>
#include <nitro/os/message.h>
#include <nitro/os/spinLock.h>
#include <nitro/os/thread.h>

// Wireless hardware register at 0x04808000 + offset.
#define WL_REG(offset) (*(vu16 *)(0x04808000 + (offset)))

// Driver task (see pTaskFunc); queued tasks are linked through `next` (0xFFFF: end).
typedef struct WlTask {
    vu16 next;
    vu16 queued;
    void (*func)(void);
} WlTask;

// Header in front of every buffer of the driver heap; the buffers of one manager form a doubly
// linked list (0xFFFFFFFF: end).
typedef struct WlHeapBuf WlHeapBuf;

struct WlHeapBuf {
    WlHeapBuf *prev;
    WlHeapBuf *next;
    u16 owner; // id of the manager holding the buffer (0: none)
    u16 magic; // WL_HEAP_BUF_MAGIC
};

#define WL_HEAP_BUF_MAGIC 0xBF1D

// List of the buffers held by one part of the driver.
typedef struct WlHeapBufMan {
    WlHeapBuf *first;
    WlHeapBuf *last;
    vu16 count;
    u16 id;
} WlHeapBufMan;

// Where the driver heap takes its memory from (WL_InitDriver parameters 8..10).
typedef struct WlHeapParam {
    u32 mode; // 0: OS heap (arena, heap), 1: the alloc/free callbacks

    union {
        u32 arena;
        void *(*alloc)(u32 size);
    } source;

    union {
        u32 heap;
        void (*free)(void *ptr);
    } sink;
} WlHeapParam;

// Parameter block of WL_InitDriver, filled by WMSP_Init and its caller.
typedef struct WlInitParam {
    void *work; // 0x694-byte work area
    void *stack;
    u32 stackSize;
    u32 priority;
    OSMessageQueue *requestQueue;  // requests from the WMSP
    OSMessageQueue *indicateQueue; // indications to the WMSP
    u32 dmaNo;
    u32 unk_1c;
    WlHeapParam heap;
    void *camTable;   // station table
    u32 camTableSize; // in bytes, 28 per station
} WlInitParam;

// Flags of the driver parameters (WSetBeaconType, WSetDiversity ...).
typedef union WlParamFlags {
    u16 raw;

    struct {
        u16 beaconType : 1;
        u16 bcSsidResponse : 1;
        u16 preambleType : 1;
        u16 mainAntenna : 1;
        u16 diversity : 1;
        u16 otherAntenna : 1; // without diversity: use the other antenna
        u16 beaconIndicate : 1;
        u16 nullKeyMode : 1;
        u16 unk_8 : 8;
    } bits;
} WlParamFlags;

// Basic and supported rates, one bit per rate (bit 15: a rate without a bit).
typedef struct WlRateSet {
    u16 basic;
    u16 supported;
} WlRateSet;

// Driver parameters (the WSet* setters), cleared by InitializeParam.
typedef struct WlParam {
    void *camTable;
    u16 camTableSize; // stations
    u16 maxSta;
    u16 macAddress[3];
    u16 retryLimit;
    u16 enableChannel;
    u16 mode;
    u16 rate;
    u16 authAlgo;
    u16 wepMode;
    u16 wepKeyId;
    u16 frameLifeTime;
    WlParamFlags flags;
    u16 activeZoneTime;
    u16 errors;  // DiagMacRegister errors; bit 7: bad flash checksum
    u32 unk_340; // bits set by WSetMacAdrs (1), WSetEnableChannel (2), WSetMode (3)
} WlParam;

// BSS state, cleared by InitializeParam.
typedef struct WlBss {
    u8 unk_00[8];
    u16 state; // 0x40: own BSS started
    u16 unk_0a;
    u16 mode; // copy of WlParam.mode
    u16 powerMgtMode;
    u16 powerState; // WSetPowerState: hardware power state >> 1
    u16 unk_12;
    u16 unk_14;
    u8 unk_16[8];
    u16 ssidLength;
    u8 ssid[32];
    u16 ssidMask[16];
    WlRateSet rateSet;
    u16 bssid[3];
    u16 aid; // own association id (WSetAids / WClearAids)
    u16 unk_6c;
    u16 beaconPeriod;
    u16 listenInterval;
    u16 unk_72;
    u16 dtimPeriod;
    u8 unk_76[4];
    u16 channel;    // WSetChannel
    u16 capability; // capability information of the own beacon
    u16 beaconLostThreshold;
    u16 beaconLostCount;
    u8 unk_82[6];
    u16 unk_88; // station whose frames and CAM state WClearAids drops
    u16 unk_8a; // bit 14 set while WEP is on
    u16 unk_8c; // WSetFrameLifeTime: life time * beaconPeriod / 100 (0xFFFF: none)
    u8 unk_8e[4];
    u16 ssidOffset; // offset of the SSID in the beacon frame
    u8 unk_94[2];
    u16 timOffset; // offset of the TIM in the beacon frame
    u8 unk_98[4];
    u8 *gameInfo; // 129-byte buffer (InitializeHeapBuf)
    u16 gameInfoLength;
    u16 unk_a2;            // bit 0: WSetGameInfo writes gameInfo byte by byte (WL_WriteByte)
    u16 unk_a4;            // set by WSetGameInfo
    u16 tmpttPowerSaveOff; // WDisableTmpttPowerSave / WEnableTmpttPowerSave
    u8 unk_a8[0xc0 - 0xa8];
} WlBss;

// RF chip description from the flash (WConfigDevice: flash bytes 0x40..0x43), used by InitRF and WSetChannel.
typedef struct WlRf {
    u16 type;            // 2, 3 or 5
    u16 bits;            // bits per RF register write (low 7 bits)
    u16 initCount;       // RF init entries at flash 0xCE; the channel tables follow (type 3)
    u16 rfChannelCount;  // type 3: RF entries per channel
    u16 bbpChannelCount; // type 3: BBP entries per channel (InitRF reads the byte from the flash)
    u16 unk_0a;
    u32 unk_0c; // type 2: an RF register value kept by InitRF; types 2/5: bit 16 set: WSetChannel writes it with the channel's flash value
} WlRf;

// The driver's work area (WL_InitDriver stores its address at WL_WORK_ADDR).
typedef struct WlWork {
    vu16 taskHead[4]; // task queues by priority (0 highest)
    vu16 taskTail[4];
    u16 taskPriority; // highest priority that may have queued tasks
    u16 taskSelected; // priority MainTaskRoutine runs from
    u16 taskCurrent;  // task running (0xFFFF: none)
    u16 unk_16;
    OSThread thread;
    WlTask task[24];
    WlHeapParam heap;
    WlHeapBufMan heapBufMan[11];
    u8 unk_20c[0x304 - 0x20c];
    OSMessageQueue *indicateQueue;
    OSMessageQueue *requestQueue;
    u32 dmaNo;
    u32 unk_310;
    u32 lockId;
    u32 unk_318;
    WlParam param;
    WlBss bss;
    u16 unk_404; // 0x13: WCheckSSID accepts an SSID that starts with the own one
    u8 unk_406[0x468 - 0x406];
    u16 unk_468; // WDisableTmpttPowerSave leaves the hardware alone while set
    u8 unk_46a[0x4a4 - 0x46a];
    u16 beaconActive;
    u8 unk_4a6[6];
    u8 *beaconFrame;
    u8 unk_4b0[0x53c - 0x4b0];
    u32 counter[45]; // statistics: WUpdateCounter adds the hardware counters, WInitCounter clears
    u8 unk_5f0[8];
    WlRf rf;
    OSAlarm periodicTimeOutAlarm; // SetupPeriodicTimeOut
    OSAlarm timeOutAlarm;         // SetupTimeOut
    OSAlarm ksidAlarm;            // runs WClearKSID
    u32 unk_68c;
    u16 unk_690; // flash 0x3e
    u16 unk_692; // flash 0x3c
} WlWork;

#define WL_WORK_ADDR 0x0380FFF4
#define wlWork       (*(WlWork *volatile *)WL_WORK_ADDR)
// The same pointer as a plain variable: the compiler may keep a loaded value until the next store.
#define wlWorkPlain (*(WlWork **)WL_WORK_ADDR)

// wl_task.c
void WlessLibReboot(void);
u16 WL_InitDriver(WlInitParam *param);
void *WL_GetThreadStruct(void);
void WL_Terminate(void);
void InitializeTask(void);
void MainTaskRoutine(void *arg);
void AddTask(int priority, u32 index);
u16 DeleteTask(u32 priority);
void LowestIdleTask(void);
void ExecuteMessage(WlHeapBuf **message);
void __VENEER_MIi_CpuClearFast(u32 data, void *dest, u32 size);
u32 __VENEER_OS_GetLockID(void);
BOOL __VENEER_OS_IsThreadTerminated(const OSThread *thread);
void __VENEER_OS_JoinThread(OSThread *thread);
u32 __VENEER_OS_DisableIrqMask(u32 mask);
void __VENEER_OS_EnableIrqMask(u32 mask);

// wl_heap_buf.c
void *AllocateHeapBuf(WlHeapBufMan *man, u32 size);
int ReleaseHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf);
int MoveHeapBuf(WlHeapBufMan *from, WlHeapBufMan *to, WlHeapBuf *buf);
int NewHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf);
int AddHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf);
int DeleteHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf);
WlHeapBuf *GetHeapBufNextAdrs(WlHeapBuf *buf);
void InitHeapBufMan(WlHeapBufMan *man, u32 id);
void InitializeHeapBuf(WlHeapParam *param);
void ReleaseAllWlHeapBuf(void);
void ReleaseAllHeapBuf(WlHeapBufMan *man);

// wl_param.c
void InitializeParam(void *camTable, u32 camTableSize);
u32 WSetMacAdrs(const u16 *addr);
u32 WSetRetryLimit(u16 limit);
u32 WSetEnableChannel(u32 channels);
u32 WSetMode(u16 mode);
u32 WSetRate(u32 rate);
u32 WSetWepMode(u32 mode);
u32 WSetWepKeyId(u32 id);
u32 WSetWepKey(const u8 *keys);
u32 WSetBeaconType(u32 type);
u32 WSetBcSsidResponse(u32 enable);
u32 WSetBeaconLostThreshold(u32 threshold);
u32 WSetActiveZoneTime(u16 time, u32 write);
u32 WSetSsidMask(const u16 *mask);
u32 WSetPreambleType(u32 type);
u32 WSetAuthAlgo(u32 algo);
u32 WSetCCA_ED(u32 mode, u32 threshold);
u32 WSetMainAntenna(u32 antenna);
u32 WSetDiversity(u32 enable, u32 otherAntenna);
u32 WSetBeaconSendRecvIndicate(u32 enable);
u32 WSetNullKeyMode(u32 mode);
u32 WSetBssid(const u16 *bssid);
u32 WSetSsid(u32 length, const u8 *ssid);
u32 WSetBeaconPeriod(u16 period);
int WSetDTIMPeriod(u16 period);
int WSetListenInterval(u32 interval);
void WSetDefaultParameters(void);

// wl_state.c
int WSetRateSet(const u16 *rates);
void WSetTxTimeStampOffset(void);
int WSetPowerMgtMode(u32 mode);
int WSetPowerState(u32 state);
int WSetForcePowerState(u32 state);
void WShutdown(void);
void WWakeUp(void);
int WSetFrameLifeTime(u32 time);
void WDisableTmpttPowerSave(void);
void WEnableTmpttPowerSave(void);
int WInitGameInfo(u32 length, const void *data);
int WSetGameInfo(u32 length, const u8 *data);
void WSetAids(u16 aid);
void WClearAids(void);
void WSetKSID(u32 unused);
void WClearKSID(u32 unused);
void WSetStaState(u32 state);
void WSetMacAdrs1(u16 *dest, const u16 *addr);
void WSetMacAdrs2(u16 *dest, const u16 *addr1, const u16 *addr2);
void WSetMacAdrs3(u16 *dest, const u16 *addr1, const u16 *addr2, const u16 *addr3);

// wl_util.c
void WInitCounter(void);
void __VENEER_MIi_CpuClear32(u32 data, void *dest, u32 size);
void WUpdateCounter(void);
int WCheckSSID(u32 length, const u8 *ssid);
int MatchMacAdrs(const u16 *addr1, const u16 *addr2);
u32 CheckEnableChannel(u32 channel);
void WElement2RateSet(const u8 *element, WlRateSet *rateSet);
u32 WCalcManRate(void);

// wl_set_channel.c
int WSetChannel(u16 channel, int direct);

// veneers defined in other ARM7 files
void __VENEER_OS_CancelAlarm(OSAlarm *alarm);
u32 __VENEER__u32_div_f(u32 dividend, u32 divisor);
void __VENEER_OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void __VENEER_OS_WakeupThreadDirect(OSThread *thread);
BOOL __VENEER_OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL __VENEER_OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
void __VENEER_MIi_CpuClear16(u16 data, void *dest, u32 size);
void __VENEER_MIi_CpuCopy16(const void *src, void *dest, u32 size);
void *__VENEER_OS_AllocFromHeap(u32 id, u32 heap, u32 size);
void __VENEER_OS_FreeToHeap(u32 id, u32 heap, void *ptr);

// defined in the later WL files
void ClearTimeOut(void);
void WStop(void);
void WIntervalTimer(void);
void SetupPeriodicTimeOut(u32 ms, OSAlarmHandler handler);
void TimeoutDummy(void *arg);
void WaitLoop_Waitus(u32 us, OSAlarmHandler handler);
int WaitLoop_BbpAccess(void);
void WaitLoop_ClrAid(void);
void DeleteTxFrames(u32 station);
void CAM_SetStaState(u16 aid, u16 state);
int RF_Write(u32 value);
void FLASH_DirectRead(u32 addr, u32 size, void *dest);
void InitMac(void);
void InitializeCmdIf(void);
void InitializeMLME(void);
void InitializeCAM(void);
void InitializeAlarm(void);
void InitializeIntr(void);
void WConfigDevice(void);
void DiagMacRegister(void);
void DiagMacMemory(void);
void DiagBaseBand(void);
void InitRF(void);
void InitBaseBand(void);
void FLASH_MakeImage(void);
void FLASH_Read(u32 addr, u32 size, void *dest);
u32 FLASH_VerifyCheckSum(u32 *checksum);
void WL_WriteByte(void *dest, u32 value);
u8 WL_ReadByte(const void *src);
int BBP_Write(u32 reg, u32 value);
void RND_Init(u32 mul, u32 add);

#endif // WL_INTERNAL_H
