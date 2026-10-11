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

// The driver heap: where the memory comes from, and the buffer lists of the parts of the driver
// ([0]: messages to the WMSP, [1..3]: TX by priority, [5], [7], [8]: received frames for the MLME tasks ...).
typedef struct WlHeap {
    WlHeapParam param;
    WlHeapBufMan bufMan[11];
} WlHeap;

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
// Station table entry (CAM): index 0 is the broadcast entry.
typedef struct WlStation {
    u16 state; // 0: free, 0x20: authenticated, 0x40: associated
    u16 aid;
    u16 macAdrs[3];
    u16 rssi;
    u16 capaInfo;
    u16 authSeed;
    u16 supRate;
    u16 unk_12;
    u16 lastSeqCtrl;
    u16 frameCount; // frames queued for the station
    u16 timer;      // CAM_TimerTask counts it down and drops the station at 0 (0xFFFF: never)
    u16 lifeTime;   // reload value of timer
} WlStation;

// Station table state (InitCAM / InitializeCAM).
typedef struct WlCam {
    u16 numSta;      // used entries, the broadcast entry included
    u16 powerMgtMap; // stations in power save mode (bit per entry)
    u16 activeMap;   // stations awake (CAM_SetAwake / CAM_SetDoze)
    u16 unk_06;      // bit per entry: state below 0x40 (CAM_SetStaState)
    u16 unk_08;      // bit per entry: CAM_SetTIMElementBitmap leaves the entry alone, timer not reloaded
    u16 unk_0a;
    u16 aidCount; // AIDs handed out
    u16 aidMap;   // AIDs in use (bit 0: reserved)
} WlCam;

typedef struct WlParam {
    WlStation *camTable;
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
    u16 receiveDtims; // MLME_PwrMgtReqCmd: wake up for the DTIM beacons
    u16 unk_16;       // DEV_TestSignalReqCmd: rate of the test signal
    u16 unk_18;       // carrier suppression test running
    u16 unk_1a;       // 1 -> 2 -> 0 on the next two WlIntrActEnd; WlIntrTbtt sets power state 2 while it is 2
    u16 unk_1c;
    u16 ssidLength;
    u8 ssid[32];
    u16 ssidMask[16];
    WlRateSet rateSet;
    u16 bssid[3];
    u16 aid; // own association id (WSetAids / WClearAids)
    u16 unk_6c;
    u16 beaconPeriod;
    u16 listenInterval;
    u16 listenCount; // counted down by WlIntrTbtt, reloaded with listenInterval
    u16 dtimPeriod;
    u16 dtimCount; // counted down by WlIntrTbtt, reloaded with dtimPeriod - 1
    u16 unk_78;
    u16 channel;    // WSetChannel
    u16 capability; // capability information of the own beacon
    u16 beaconLostThreshold;
    u16 beaconLostCount;
    u16 unk_82[3]; // MLME_BeaconLostTask: address of the BSS whose beacons were lost
    u16 unk_88;    // station whose frames and CAM state WClearAids drops
    u16 unk_8a;    // bit 14 set while WEP is on
    u16 unk_8c;    // WSetFrameLifeTime: life time * beaconPeriod / 100 (0xFFFF: none)
    u16 unk_8e;    // TxqEndData: no own power save while set
    u16 unk_90;
    u16 ssidOffset;     // offset of the SSID in the beacon frame
    u16 unk_94;         // offset of the beacon's TIM element (MakeBeaconFrame; MAC memory 0x0480425C + unk_94)
    u16 gameInfoOffset; // offset of the beacon's game information element (MakeBeaconFrame)
    u16 unk_98;
    u16 unk_9a;   // DMA_Read: MAC memory reads past 0x04805F60 continue this many bytes lower
    u8 *gameInfo; // 129-byte buffer (InitializeHeapBuf)
    u16 gameInfoLength;
    u16 unk_a2;            // bit 0: WSetGameInfo writes gameInfo byte by byte (WL_WriteByte)
    u16 unk_a4;            // set by WSetGameInfo
    u16 tmpttPowerSaveOff; // WDisableTmpttPowerSave / WEnableTmpttPowerSave
    u32 intervalCount;     // WIntervalTimer calls (every 100 ms in state 0x40)
    u16 unk_ac;            // DEV_TestSignalReqCmd: BBP register 6 during the carrier suppression test
    u16 unk_ae;
    u16 fatalErr; // SetFatalErr bits; WIntervalTimer queues task 21 (SendFatalErrMsgTask) while set
    u16 unk_b2;
    u16 rxDropCount;       // WlIntrRxEnd dropped a received frame (bad length, status or rate)
    u16 txBufRepairCount;  // WCheckTxBufIdBeforeFrame rewrote a frame's id words
    u16 txFrameRetryCount; // RestoreTxFrame / WCheckTxBuf copied a frame to the MAC again
    u16 txResetCount;      // WlIntrTxErr / WlIntrRxCntup reset the MAC's TX side
    u16 mpTimeOutCount;    // WlIntrMpEnd waits for the MP answers (MacBugTxMp)
    u16 keyRestoreCount;   // WlIntrStartRx stopped the TX of a key data frame
} WlBss;

// 802.11 frame control.
typedef struct WlFrameCtrlBits {
    u16 version : 2;
    u16 type : 2; // 0: management, 1: control, 2: data
    u16 subtype : 4;
    u16 toDs : 1;
    u16 fromDs : 1;
    u16 moreFrag : 1;
    u16 retry : 1;
    u16 pwrMgt : 1;
    u16 moreData : 1;
    u16 wep : 1;
    u16 order : 1;
} WlFrameCtrlBits;

// 802.11 sequence control.
typedef struct WlSeqCtrlBits {
    u16 fragNo : 4;
    u16 seqNo : 12;
} WlSeqCtrlBits;

// TX frame in the MAC memory: hardware TX header, then the 802.11 header.
typedef struct WlTxFrame {
    u16 status;
    u16 unk_02; // MP: stations still to answer (bit per AID)
    u16 retry;  // low byte: retry count
    u16 unk_06; // MA_DataReqCmd: rate requested (0: the station's rate)
    u16 rate;   // 10: 1 Mbps, 20: 2 Mbps
    u16 length;
    u16 frameCtrl; // 0x4000: WEP
    u16 duration;
    u16 addr1[3];
    u16 addr2[3];
    u16 addr3[3];
    u16 seqCtrl;
    u16 unk_24; // the body: MP frame: time per answer; key data frame: its header
    u16 unk_26;
    u16 unk_28;
} WlTxFrame;

// TX frame in the driver heap (after its WlHeapBuf): queue data, then the frame as copied to the MAC memory.
typedef struct WlTxHeapFrame {
    u16 unk_00;
    u16 station; // CAM index
    u16 time;    // low half of WlBss.intervalCount when queued (CheckFrameTimeout)
    u16 bodyLength;
    WlTxFrame frame;
} WlTxHeapFrame;

// Indication to the WMSP (MLME_Issue*Indication): heap buffer, command (0x84 ..), parameter length in halfwords.
typedef struct WlIndHeader {
    WlHeapBuf buf;
    u16 command;
    u16 length;
} WlIndHeader;

// Entry of a station in the MP state.
typedef struct WlMpEntry {
    u16 unk_00;
    u16 unk_02;
    u16 aid;
    u16 missCount; // MP answers missing (WlIntrTxEnd)
} WlMpEntry;

// MP sequence state (WlTxCtrl.mpState): an indication (0x184) sent to the WMSP when the sequence ends.
typedef struct WlMpState {
    WlIndHeader header;
    u16 unk_10; // stations addressed (bit per AID)
    u16 unk_12; // WlIntrMpEndTask: stations addressed that answered
    u16 count;  // stations
    u16 entrySize;
    u16 unk_18;    // WlIntrTxEnd: MP frame retried up to the threshold
    u8 entries[8]; // WlMpEntry, entrySize bytes per station; the table continues past the struct
} WlMpState;

// Received frame in the driver heap (TakeoutRxFrame): driver data, then the hardware RX header and the
// 802.11 frame as read from the MAC memory.
typedef struct WlRxFrame {
    u8 unk_00[6];
    u16 length; // frame body (size - 24)
    u16 status; // RX header: bits 0..3 frame type, bit 9: fragment
    u16 unk_0a;
    u16 unk_0c;
    u16 unk_0e; // TakeoutRxFrame moves the low byte of unk_12 into the high byte
    u16 unk_10;
    u16 unk_12;
    u16 frameCtrl;
    u16 duration;
    u16 addr1[3];
    u16 addr2[3];
    u16 addr3[3];
    u16 seqCtrl;
} WlRxFrame;

// Driver heap buffer holding a received frame.
typedef struct WlRxBuf {
    WlHeapBuf header;
    u8 unk_0c[4];
    WlRxFrame frame;
} WlRxBuf;

// A frame waiting in the MAC memory (WlTxCtrl.txq / beacon).
typedef struct WlTxDesc {
    u16 active; // txq[4], txq[5]: 2 while the key data frame is sent, then 1 (CheckKeyTxEndMain)
    u16 unk_02;
    u16 count; // frames completed
    u16 unk_06;
    u8 *frame;                                 // the frame's place in the MAC memory
    WlTxHeapFrame *heapFrame;                  // the frame in the driver heap (-16 bytes: its WlHeapBuf)
    void (*callback)(void *frame, u32 result); // WlIntrTxEndTask: 1 timed out, 2 sent, 3 sent (no heap frame)
} WlTxDesc;

// TX side of the MAC (WlWork+0x42C).
typedef struct WlTxCtrl {
    WlTxDesc txq[6]; // [0..2]: by priority (ResetTxqPri); [3]: MP frame, its active flag makes WDisableTmpttPowerSave leave the hardware alone; [5]: PS-Poll
    WlTxDesc beacon;
    u32 unk_8c; // bit 1: SetParentTbttTxq is due at the next TX end
    WlMpState *mpState;
    u16 unk_94; // MA_MpReqCmd: interval
    u16 unk_96; // MA_MpReqCmd: period
    u16 unk_98; // MP: stations addressed (bit per AID)
    u16 unk_9a;
    u16 unk_9c;
    u16 unk_9e; // MP: retry count from which WlIntrTxEnd stops the TX queue
    u16 unk_a0; // WlIntrMpEndTask: WlMpState.unk_10 of the last MP sequence
    u16 unk_a2;
    u16 unk_a4; // TX pointer saved by WlIntrStartRx (0xFFFF: none)
    u16 unk_a6;
    u16 unk_a8; // WlIntrTxEnd: MP stopped at the retry threshold
    u16 unk_aa; // WlIntrTxEnd: MP ended
    u16 unk_ac; // WlIntrTxEnd: MP ended with the MP end interrupt pending
    u16 unk_ae; // WlIntrTbtt: queued frames timed out
} WlTxCtrl;

// RX side of the MAC (WlWork+0x4DC).
typedef struct WlRxCtrl {
    u16 lastMpSeqCtrl; // sequence control of the last MP frame received
    u16 unk_02;        // WlIntrRxCntup: RX counter interrupts while TX was busy
    u16 unk_04;        // next frame to read (halfword index into the MAC memory)
    u8 unk_06[0x50 - 6];
} WlRxCtrl;

// Random number generator state (RND_Init / RND_seed / RND_rand).
typedef struct WlRandom {
    u16 mul;
    u16 add;
    u16 seed;
    u16 unk_06;
} WlRandom;

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

// Message header of a request from the WMSP (queued in WlHeap.bufMan[10]); the request's parameters follow it,
// its confirm starts `length` halfwords after the start of the message.
typedef struct WlCmdRequest {
    WlHeapBuf buf;
    u16 command; // bits 8..15: group (0 MLME, 1 MA, 2 PARAMSET/PARAMGET, 3 DEV), bits 0..7: command
    vu16 length;
} WlCmdRequest;

// Confirm header; the command handlers get a pointer to it, set `length` (halfwords) and add their results.
typedef struct WlCmdCfm {
    u16 command;
    u16 length;
    u16 result;
} WlCmdCfm;

typedef struct WlCmdConfirm {
    u8 unk_00[16];
    WlCmdCfm cfm;
} WlCmdConfirm;

// Command table entry (WlibCmdTbl_*): minimum request and confirm lengths (halfwords) and the handler.
typedef struct WlCmdEntry {
    u16 reqLength;
    u16 cfmLength;
    u16 (*func)(WlCmdRequest *req, WlCmdCfm *cfm);
} WlCmdEntry;

// Command interface state (RequestCmdTask).
typedef struct WlCmdIf {
    WlCmdRequest *request; // request being executed (0xFFFFFFFF: none)
    u16 busy;              // command groups with a request in progress (bit per group)
    u16 unk_06;
} WlCmdIf;

// MLME request parameters (after the WlCmdRequest header) and confirms.
typedef struct WlMlmeResetReq {
    WlCmdRequest header;
    u16 initCounter;
} WlMlmeResetReq;

typedef struct WlMlmePwrMgtReq {
    WlCmdRequest header;
    u16 pwrMgtMode;
    u16 wakeUp;
    u16 receiveDtims;
} WlMlmePwrMgtReq;

typedef struct WlMlmeScanReq {
    WlCmdRequest header;
    u16 bssid[3];
    u16 ssidLength;
    u8 ssid[32];
    u16 scanType; // 0: active (probe requests)
    u8 channelList[16];
    u16 maxChannelTime;
    u16 unk_4c;
} WlMlmeScanReq;

typedef struct WlMlmeJoinReq {
    WlCmdRequest header;
    u16 timeOut;
    u16 unk_12[3];
    u16 bssid[3];
    u16 ssidLength;
    u8 ssid[32];
    u16 capaInfo; // 0x20: short preamble
    WlRateSet rateSet;
    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
} WlMlmeJoinReq;

// Auth / DeAuth / Ass / ReAss / DisAss: the peer, a parameter (algorithm, reason code, listen interval), a time out.
typedef struct WlMlmePeerReq {
    WlCmdRequest header;
    u16 peerMacAdrs[3];
    u16 param;
    u16 timeOut;
} WlMlmePeerReq;

typedef struct WlMlmeStartReq {
    WlCmdRequest header;
    u16 ssidLength;
    u8 ssid[32];
    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
    WlRateSet rateSet;
    u16 gameInfoLength;
    u8 gameInfo[128];
} WlMlmeStartReq;

typedef struct WlMlmeMeasChanReq {
    WlCmdRequest header;
    u16 unk_10;
    u16 ccaMode;
    u16 edThreshold;
    u16 measureTime;
    u8 channelList[16];
} WlMlmeMeasChanReq;

typedef struct WlMlmeAuthCfm {
    WlCmdCfm header;
    u16 unk_06;
    u16 peerMacAdrs[3];
    u16 algorithm;
} WlMlmeAuthCfm;

typedef struct WlMlmeDeAuthCfm {
    WlCmdCfm header;
    u16 peerMacAdrs[3];
} WlMlmeDeAuthCfm;

typedef struct WlMlmeScanCfm {
    WlCmdCfm header;
    u16 unk_06;
    u16 unk_08;
} WlMlmeScanCfm;

typedef struct WlMlmeJoinCfm {
    WlCmdCfm header;
    u16 reason;
} WlMlmeJoinCfm;

typedef struct WlMlmeMeasChanCfm {
    WlCmdCfm header;
    u16 unk_06;
    u16 result[16]; // per channel: channel | busy percentage << 8
} WlMlmeMeasChanCfm;

// MLME request in progress (the MLME_*ReqCmd handlers and the MLME tasks).
typedef struct WlMlme {
    u16 state; // request and its step: 0x10 scan, 0x20 join, 0x30 auth, 0x41 deauth, 0x50 ass, 0x60 reass, 0x71 disass, 0x80 measure channel
    u16 unk_02;

    union {
        u8 *frame; // deauth / disass: the frame being sent

        struct {
            u16 result;
            u16 reason;
        } join;

        struct {
            u16 cfmSpace; // confirm halfwords left for the results
            u16 channelIndex;
            u16 unk_08;
            u16 elapsed; // time spent on the channel
            u16 step;    // time out per probe
        } scan;

        struct {
            u32 samples;
            u32 total; // 100 per sample with the channel busy
            u16 power; // power state before the measurement
            u16 bbp13; // BBP registers 0x13 and 0x35 before the measurement
            u16 bbp35;
            u16 unk_12;
            u16 index;
            u16 result;
        } meas;
    } work;

    union {
        void *any;
        WlMlmeScanReq *scan;
        WlMlmeJoinReq *join;
        WlMlmePeerReq *peer;
        WlMlmeMeasChanReq *meas;
    } req;

    union {
        WlCmdCfm *header;
        WlMlmeAuthCfm *auth;
        WlMlmeDeAuthCfm *deauth;
        WlMlmeScanCfm *scan;
        WlMlmeJoinCfm *join;
        WlMlmeMeasChanCfm *meas;
    } cfm;
} WlMlme;

// Generic request and confirm (PARAMSET / PARAMGET, DEV ...): the parameters follow the headers.
typedef struct WlParamReq {
    WlCmdRequest header;
    u16 param[72];
} WlParamReq;

typedef struct WlParamCfm {
    WlCmdCfm header;
    u16 param[64];
} WlParamCfm;

typedef struct WlParamSetAllReq {
    WlCmdRequest header;
    u16 macAdrs[3];
    u16 retryLimit;
    u16 enableChannel;
    u16 unk_1a;
    u16 mode;
    u16 rate;
    u16 wepMode;
    u16 wepKeyId;
    u8 wepKey[80];
    u16 beaconType;
    u16 bcSsidResponse;
    u16 beaconLostThreshold;
    u16 activeZoneTime;
    u16 ssidMask[16];
    u16 preambleType;
    u16 authAlgo;
} WlParamSetAllReq;

// MA data request: the message itself is the TX frame.
typedef struct WlMaDataReq {
    WlCmdRequest header;
    WlTxHeapFrame frame;
} WlMaDataReq;

typedef struct WlMaDataConfirm {
    u8 unk_00[16];
    WlCmdCfm cfm;
    u16 status;
} WlMaDataConfirm;

typedef struct WlMaKeyDataReq {
    WlCmdRequest header;
    u16 length; // bytes of data
    u16 unk_12; // key data header
    void *data;
} WlMaKeyDataReq;

typedef struct WlMaMpReq {
    WlCmdRequest header;
    u16 flags;      // bit 15: keep the values of the last sequence for the fields whose bit (1..4) is clear
    u16 retryLimit; // WlTxCtrl.unk_9e
    u16 interval;   // bit 15: in microseconds, else in slots
    u16 bitmap;     // stations addressed (bit per AID)
    u16 period;     // 0: send now
    u16 offset;
    u16 payloadBytes;
    u16 sequence;
    void *payload;
} WlMaMpReq;

// Auth (0x84), DeAuth (0x85), DisAss (0x88): the peer and the algorithm / reason code.
typedef struct WlMlmePeerInd {
    WlIndHeader header;
    u16 peerMacAdrs[3];
    u16 param;
} WlMlmePeerInd;

// Ass (0x86) and ReAss (0x87).
typedef struct WlMlmeAssInd {
    WlIndHeader header;
    u16 peerMacAdrs[3];
    u16 aid;
    u16 ssidLength;
    u8 ssid[32];
} WlMlmeAssInd;

// Beacon lost (0x8B).
typedef struct WlMlmeBeaconLostInd {
    WlIndHeader header;
    u16 macAdrs[3];
} WlMlmeBeaconLostInd;

// Beacon received (0x8D): the sender, two bytes of the RX header and the game information.
typedef struct WlMlmeBeaconRecvInd {
    WlIndHeader header;
    u8 unk_10[6];
    u16 gameInfoLength;
    u8 unk_18[6];
    u8 unk_1e; // low byte of the RX header's unk_0e
    u8 unk_1f; // low byte of the RX header's unk_12
    u8 unk_20[14];
    u16 srcMacAdrs[3];
    u8 unk_34[8];
    u8 gameInfo[128];
} WlMlmeBeaconRecvInd;

// MAC register self test entry (DiagMacRegister): register offset and the bits that can be written.
typedef struct WlTestReg {
    u16 offset;
    u16 mask;
} WlTestReg;

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
    WlHeap heap;
    u8 unk_20c[0x304 - 0x20c];
    OSMessageQueue *indicateQueue;
    OSMessageQueue *requestQueue;
    u32 dmaNo;
    u32 unk_310;
    u32 lockId;
    u32 unk_318;
    WlParam param;
    WlBss bss;
    WlMlme mlme;
    WlCmdIf cmdIf;
    WlTxCtrl txCtrl;
    WlRxCtrl rxCtrl;
    WlCam cam;
    u32 counter[45]; // statistics: WUpdateCounter adds the hardware counters, WInitCounter clears
    WlRandom rnd;
    WlRf rf;
    OSAlarm periodicTimeOutAlarm; // SetupPeriodicTimeOut
    OSAlarm timeOutAlarm;         // SetupTimeOut
    OSAlarm ksidAlarm;            // runs WClearKSID
    u32 unk_68c;
    u16 unk_690; // flash 0x3e: hardware workarounds (bit 0: RX ring check, 3: TX reset, 4: MP timeout, 5: key search, 6: MP answer count)
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

// wl_device.c
u64 __VENEER__ll_udiv(u64 dividend, u64 divisor);
void WStop(void);
u32 BBP_Read(u32 reg);
int BBP_Write(u32 reg, u32 value);
int RF_Write(u32 value);
u32 CalcBbpCRC(void);
void WConfigDevice(void);
void InitMac(void);
void InitBaseBand(void);
void InitRF(void);
void InitializeAlarm(void);
BOOL __VENEER_OS_IsAlarmAvailable(void);
void TimeoutDummy(void *arg);
void WWaitus(u32 us);

// wl_start.c
void WStart(void);

// wl_timeout_periodic.c
void SetupPeriodicTimeOut(u32 ms, OSAlarmHandler handler);

// wl_timer.c
void __VENEER_OS_SetPeriodicAlarm(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg);
void ClearPeriodicTimeOut(void);
void WIntervalTimer(void);

// wl_timeout.c
void SetupTimeOut(u32 ms, OSAlarmHandler handler);
void SetupUsTimeOut(u32 us, OSAlarmHandler handler);

// wl_set_channel.c
int WSetChannel(u16 channel, int direct);

// wl_mac_memory.c
void ClearTimeOut(void);
void DMA_Read(void *dest, const void *src, u32 size);
void DMA_Write(void *dest, const void *src, u32 size);
void DMA_WriteHeaderData(void *dest, const void *header, const void *data, u32 size);
void DMA_WepWriteHeaderData(void *dest, const void *header, const void *data, u32 size);
void WL_WriteByte(void *dest, u32 value);
u8 WL_ReadByte(const void *src);

// wl_random.c
void RND_Init(u32 mul, u32 add);
void RND_seed(u32 seed);
u16 RND_rand(void);
u16 calc_NextCRC(int data, int crc);

// wl_tx_check.c
int WCheckTxBufIdBeforeFrame(WlTxDesc *desc);
void WaitMacStop(void);
void RestoreTxFrame(WlTxDesc *desc);
void WCheckTxBuf(void);

// wl_fatal_err.c
void SetFatalErr(u32 bits);
void SendFatalErrMsgTask(void);
void TerminateWlTask(void);
void ReleaseWlTask(void);

// wl_diag_mac_register.c
void DiagMacRegister(void);

// wl_diag_mac_memory.c
void DiagMacMemory(void);

// wl_diag_base_band.c
void DiagBaseBand(void);

// wl_intr.c
void WlIntr(void);
void WlIntrPreTbtt(void);
void WlIntrTbtt(void);
void WlIntrActEnd(void);
void WlIntrRfWakeup(void);
void WlIntrCntOvf(void);
void WlIntrTxErr(void);
void WlIntrRxCntup(void);
void WlIntrTxEnd(void);

// wl_intr_rx_end.c
void WlIntrRxEnd(void);

// wl_intr_mp_end.c
void WlIntrMpEnd(int timeout);

// wl_intr_start_tx.c
void WlIntrStartTx(void);

// wl_intr_ctrl.c
void WlIntrStartRx(void);
void SetParentTbttTxq(void);
void MacBugTxMp(void);
u16 *AdjustRingPointer(u16 *pointer);
u32 CheckKeyTxEnd(void);
u32 CheckKeyTxEndMain(WlTxDesc *desc);
void InitializeIntr(void);
void __VENEER_OS_SetIrqFunction(OSIrqMask intrBit, OSIrqFunction function);
void ReleaseIntr(void);
void WlIntrTxBeaconTask(void);
void WlIntrTxEndTask(void);

// wl_intr_rx_end_task.c
void WlIntrRxEndTask(void);

// wl_intr_task.c
void WlIntrMpEndTask(void);
void SetParentTbttTxqTask(void);
WlRxFrame *TakeoutRxFrame(u16 *src, u32 size);

// wl_wait_loop.c
void WaitLoop_ClrAid(void);
int WaitLoop_BbpAccess(void);
int WaitLoop_RfAccess(void);

// wl_cmd_if.c
void RequestCmdTask(void);
int CMD_ReservedReqCmd(WlCmdRequest *req, WlCmdCfm *cfm);
void SendMessageToWmDirect(WlHeapBufMan *man, void *msg);
void SendMessageToWmTask(void);
void InitializeCmdIf(void);

// wl_mlme_cmd.c
int MLME_ResetReqCmd(WlMlmeResetReq *req, WlCmdCfm *cfm);
int MLME_PwrMgtReqCmd(WlMlmePwrMgtReq *req, WlCmdCfm *cfm);
int MLME_ScanReqCmd(WlMlmeScanReq *req, WlMlmeScanCfm *cfm);
int MLME_JoinReqCmd(WlMlmeJoinReq *req, WlMlmeJoinCfm *cfm);
int MLME_AuthReqCmd(WlMlmePeerReq *req, WlMlmeAuthCfm *cfm);
int MLME_DeAuthReqCmd(WlMlmePeerReq *req, WlMlmeDeAuthCfm *cfm);
int MLME_AssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm);
int MLME_ReAssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm);
int MLME_DisAssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm);
int MLME_StartReqCmd(WlMlmeStartReq *req, WlCmdCfm *cfm);
int MLME_MeasChanReqCmd(WlMlmeMeasChanReq *req, WlMlmeMeasChanCfm *cfm);

// wl_mlme_task.c
void MLME_ScanTask(void);
void MLME_ScanTimeOut(void);
void MLME_JoinTask(void);
void MLME_JoinTimeOut(void);
void MLME_AuthTask(void);
void MLME_AuthTimeOut(void);
void MLME_AssTask(void);
void MLME_AssTimeOut(void);
void MLME_ReAssTask(void);
void MLME_ReAssTimeOut(void);
void MLME_MeasChannelTask(void);
void MLME_MeasChanTimeOut(void);

// wl_mlme_ind.c
void MLME_BeaconLostTask(void);
void IssueMlmeConfirm(void);
int MLME_IssueAuthIndication(const u16 *macAdrs, int algorithm);
int MLME_IssueDeAuthIndication(const u16 *macAdrs, int reason);
int MLME_IssueAssIndication(const u16 *macAdrs, u32 aid, const u8 *ssidElement);
int MLME_IssueReAssIndication(const u16 *macAdrs, u32 aid, const u8 *ssidElement);
int MLME_IssueDisAssIndication(const u16 *macAdrs, int reason);
int MLME_IssueBeaconLostIndication(const u16 *macAdrs);
int MLME_IssueBeaconSendIndication(void);
int MLME_IssueBeaconRecvIndication(WlRxFrame *frame);
void InitializeMLME(void);

// wl_paramset_cmd.c
int PARAMSET_AllReqCmd(WlParamSetAllReq *req, WlCmdCfm *cfm);
int PARAMSET_MacAdrsReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_RetryReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_EnableChannelReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_ModeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_RateReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_WepModeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_WepKeyIdReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_WepKeyReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_BeaconTypeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_ResBcSsidReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_BeaconLostThReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_ActiveZoneReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_SSIDMaskReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_PreambleTypeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_AuthAlgoReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_CCAModeEDThReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_LifeTimeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_MaxConnReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_MainAntennaReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_DiversityReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_BcnSendRecvIndReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_NullKeyModeReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_BSSIDReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_SSIDReqCmd(WlParamReq *req, WlCmdCfm *cfm);
u32 PARAMSET_BeaconPeriodReqCmd(WlParamReq *req, WlCmdCfm *cfm);
int PARAMSET_DTIMPeriodReqCmd(WlParamReq *req, WlCmdCfm *cfm);
int PARAMSET_ListenIntervalReqCmd(WlParamReq *req, WlCmdCfm *cfm);
int PARAMSET_GameInfoReqCmd(WlParamReq *req, WlCmdCfm *cfm);

// wl_paramget_cmd.c
int PARAMGET_AllReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_MacAdrsReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_RetryReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_EnableChannelReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_ModeReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_RateReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_WepModeReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_WepKeyIdReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_BeaconTypeReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_ResBcSsidReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_BeaconLostThReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_ActiveZoneReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_SSIDMaskReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_PreambleTypeReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_AuthAlgoReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_CCAModeEDThReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_MaxConnReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_MainAntennaReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_DiversityReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_BcnSendRecvIndReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_NullKeyModeReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_BSSIDReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_SSIDReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_BeaconPeriodReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_DTIMPeriodReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_ListenIntervalReqCmd(WlParamReq *req, WlParamCfm *cfm);
int PARAMGET_GameInfoReqCmd(WlParamReq *req, WlParamCfm *cfm);

// wl_dev_cmd.c
int DEV_ShutdownReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_IdleReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_Class1ReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_RebootReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_ClearWlInfoReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_GetVerInfoReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_GetWlInfoReqCmd(WlParamReq *req, WlParamCfm *cfm);
void __VENEER_MIi_CpuCopy32(const void *src, void *dest, u32 size);
int DEV_GetStateReqCmd(WlParamReq *req, WlParamCfm *cfm);
int DEV_TestSignalReqCmd(WlParamReq *req, WlParamCfm *cfm);
void IntrCarrierSuppresionSignal(void);
int DEV_TestRxReqCmd(WlParamReq *req, WlParamCfm *cfm);

// wl_ma_cmd.c
int MA_DataReqCmd(WlMaDataReq *req, WlCmdCfm *cfm);
int MA_KeyDataReqCmd(WlMaKeyDataReq *req, WlCmdCfm *cfm);

// wl_ma_mp_cmd.c
int MA_MpReqCmd(WlMaMpReq *req, WlCmdCfm *cfm);

// wl_ma_data.c
int MA_TestDataReqCmd(WlMaDataReq *req, WlCmdCfm *cfm);
int MA_ClrDataReqCmd(WlParamReq *req, WlCmdCfm *cfm);
void IssueMaDataConfirm(WlHeapBufMan *man, WlMaDataReq *req);

// wl_txq.c
void TxqPri(u32 pri);

// wl_tx_ctrl.c
void CopyTxFrmToMacBuf(void *dest, void *source);
int CheckFrameTimeout(WlTxHeapFrame *frame);
void TxqEndData(void *arg, u32 next);
void TxqEndManCtrl(void *arg, u32 schedule);
void TxqEndPsPoll(void *arg, u32 next);
void TxqEndBroadCast(void *arg, u32 next);
void TxEndKeyData(WlTxDesc *desc);
void ClearTxKeyData(void);
void ClearTxMp(void);
void ClearTxData(void);
void ClearQueuedPri(u32 pri);
void ResetTxqPri(u32 pri);
void DeleteTxFrames(u32 station);
void DeleteTxFrameByAdrs(const u16 *macAdrs);
void DeleteAllTxFrames(void);
void MessageDeleteTx(u32 pri, u32 confirm);

// wl_tx_frame.c
void TxManCtrlFrame(WlTxHeapFrame *frame);
void SetManCtrlFrame(WlTxHeapFrame *frame);
void TxPsPollFrame(void);
void StartBeaconFrame(void);
void StopBeaconFrame(void);
void MakeBeaconFrame(void);
void UpdateGameInfoElement(void);
int IsEnableManagement(void);
WlTxHeapFrame *MakeDisAssFrame(const u16 *macAdrs, u32 reason);
WlTxHeapFrame *MakeAssReqFrame(const u16 *macAdrs);
WlTxHeapFrame *MakeReAssReqFrame(const u16 *macAdrs);
WlTxHeapFrame *MakeAssResFrame(u32 station, u32 status, const u8 *element);
WlTxHeapFrame *MakeReAssResFrame(u32 station, u32 status, const u8 *extra);
WlTxHeapFrame *MakeProbeReqFrame(const u16 *bssid);

// wl_probe_res.c
WlTxHeapFrame *MakeProbeResFrame(const u16 *macAdrs);

// wl_man_frame.c
WlTxHeapFrame *MakeAuthFrame(const u16 *macAdrs, u16 length, u32 test);
WlTxHeapFrame *MakeDeAuthFrame(const u16 *macAdrs, u16 reason, u32 test);
void MakePsPollFrame(u16 aid);
void InitManHeader(WlTxHeapFrame *frame, const u16 *macAdrs);
int IsExistManFrame(const u16 *macAdrs, u16 control);
u32 SetSSIDElement(u8 *dest);
u32 SetSupRateSet(u8 *dest);

// wl_tx_init.c
void InitTxCtrl(void);

// wl_cam.c
u32 CAM_Search(const u16 *addr);
u32 CAM_SearchAdd(const u16 *addr);
void CAM_AddBcFrame(WlHeapBufMan *from, WlHeapBuf *frame);
void CAM_IncFrameCount(void *frame);
void CAM_DecFrameCount(void *frame);
void CAM_SetStaState(u16 aid, u16 state);
void CAM_SetRSSI(u32 index, u32 rssi);
void CAM_SetPowerMgtMode(u32 index, u16 mode);
void CAM_SetDoze(u32 index);
void CAM_SetAwake(u32 index);
void CAM_SetCapaInfo(u32 index, u16 capaInfo);
void CAM_SetSupRate(u32 index, u16 supRate);
void CAM_SetLastSeqCtrl(u32 index, u16 seqCtrl);
void CAM_SetAuthSeed(u32 index, u16 seed);
void CAM_UpdateLifeTime(u32 index);
u32 CAM_AllocateAID(u32 index);
void CAM_ReleaseAID(u32 index);
u16 CAM_GetStaState(u32 index);
u32 CAM_IsActive(u32 index);
u32 CAM_GetPowerMgtMode(u32 index);
u16 *CAM_GetMacAdrs(u32 index);
u16 CAM_GetAuthSeed(u32 index);
u16 CAM_GetLastSeqCtrl(u32 index);
u16 CAM_GetTxRate(u32 index);
u16 CAM_GetAID(u32 index);
u16 CAM_GetFrameCount(u32 index);
void CAM_SetTIMElementBitmap(u32 index);
void CAM_ClrTIMElementBitmap(u32 index);
void CAM_TimerTask(void);
void CAM_Delete(u32 index);
void InitializeCAM(void);
void InitCAM(void);
void CAM_InitElement(u32 index, const u8 *addr);

// wl_wait_loop_rxpe.c
void WaitLoop_Rxpe(void);

// wl_wait_loop_waitus.c
void WaitLoop_Waitus(u32 us, OSAlarmHandler handler);

// veneers defined in other ARM7 files
void __VENEER_OS_CancelAlarm(OSAlarm *alarm);
void __VENEER_OS_CreateAlarm(OSAlarm *alarm);
void __VENEER_OS_SetAlarm(OSAlarm *alarm, OSTick tick, OSAlarmHandler handler, void *arg);
OSTick __VENEER_OS_GetTick(void);
u32 __VENEER__u32_div_f(u32 dividend, u32 divisor);
void __VENEER_OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void __VENEER_OS_WakeupThreadDirect(OSThread *thread);
BOOL __VENEER_OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL __VENEER_OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
void __VENEER_MIi_CpuClear16(u16 data, void *dest, u32 size);
void __VENEER_MIi_CpuCopy16(const void *src, void *dest, u32 size);
void *__VENEER_OS_AllocFromHeap(u32 id, u32 heap, u32 size);
void __VENEER_OS_FreeToHeap(u32 id, u32 heap, void *ptr);
void __VENEER_OS_ExitThread(void);

// defined in the later WL files
void InitApList(void);
void InitRxCtrl(void);
void FLASH_DirectRead(u32 addr, u32 size, void *dest);
void FLASH_MakeImage(void);
void FLASH_Read(u32 addr, u32 size, void *dest);
u32 FLASH_VerifyCheckSum(u32 *checksum);
OSIntrMode __VENEER_OS_DisableInterrupts(void);
OSIntrMode __VENEER_OS_RestoreInterrupts(OSIntrMode state);
void RxBeaconFrame(WlRxFrame *frame);
void RxKeyDataFrame(WlRxFrame *frame);
u32 RxMpFrame(WlRxFrame *frame);
u32 RxMpAckFrame(WlRxFrame *frame);

extern const u8 BC_ADRS[];
extern const u8 MP_ADRS[];
extern const u8 MPKEY_ADRS[];

extern const u16 crc16_table[16];

extern const WlTestReg test_reg[27];
extern const u16 test_pattern[3];

#endif // WL_INTERNAL_H
