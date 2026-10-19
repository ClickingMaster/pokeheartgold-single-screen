#include "application/main_menu/mystery_gift_beacon.h"

#include "global.h"

#include "library/nitrocrypto_rc4.h"

// Mystery Gift > Get via Wireless Communication: downloads a gift that a distribution station broadcasts in its
// beacons (no connection); the gift is RC4-encrypted, its id is the checksum of the decrypted data.

#define MYSTERY_GIFT_BEACON_GGID 0x400318

// WM results (only the ones this module tests)
#define WM_ERRCODE_SUCCESS   0
#define WM_ERRCODE_OPERATING 2

#define MYSTERY_GIFT_BEACON_ASSERT(cond) (void)((cond) || (OS_Terminate(), 0))

// parameters for WM_StartScan
typedef struct MysteryGiftBeaconScanParam {
    void *scanBuffer;
    u16 channel;
    u16 maxChannelTime;
    u8 bssid[6];
    u16 unk_0E;
} MysteryGiftBeaconScanParam;

// WM_StartScan callback argument: a station found (state 5) with its beacon
typedef struct MysteryGiftBeaconScanResult {
    u16 apiId;
    u16 errCode;
    u8 unk_04[4];
    u16 state;
    u8 macAddress[6];
    u16 unk_10;
    u16 linkLevel;
    u8 unk_14[0x22];
    u16 gameInfoLength;
    u8 unk_38[4];
    u32 ggid;
    u8 unk_40[8];
    MysteryGiftBeaconPacket packet;
} MysteryGiftBeaconScanResult;

// argument of the other WM callbacks (common head)
typedef struct MysteryGiftBeaconWmResult {
    u16 apiId;
    u16 errCode;
} MysteryGiftBeaconWmResult;

typedef struct MysteryGiftBeaconWork {
    u8 wmBuffer[0xF00]; // WM_Initialize
    u8 scanBuffer[0xC0];
    MysteryGiftBeaconScanParam scanParam;
    u8 unk_FD0[0x10];
    u16 title[MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE / 2]; // packet 0xFFFF
    BOOL titleReceived;
    CRYPTORC4Context rc4;
    int state;  // MYSTERY_GIFT_BEACON_STATE_*
    int error;  // the first WM error
    u16 mode;   // MYSTERY_GIFT_BEACON_MODE_*
    u16 dataId; // checksum of the gift
    u16 packetCount;
    u16 packetsReceived;
    u32 dataSize;
    u32 mismatches; // packets of another gift
    u32 lastPacket;
    u32 linkLevel;
    u8 parentMac[6];  // the station
    u16 channelIndex; // 16 + channel while searching
    void *dest;
    void (*callback)(int event); // MYSTERY_GIFT_BEACON_EVENT_*
    u8 packetReceived[];
} MysteryGiftBeaconWork;

typedef struct MysteryGiftBeacon {
    MysteryGiftBeaconWork *work;
    void (*print)(const char *format, ...); // debug output
} MysteryGiftBeacon;

#define MYSTERY_GIFT_BEACON_MODE_INIT     1
#define MYSTERY_GIFT_BEACON_MODE_SEARCH   2
#define MYSTERY_GIFT_BEACON_MODE_RECEIVE  3
#define MYSTERY_GIFT_BEACON_MODE_COMPLETE 4

int WM_Initialize(void *wmBuffer, void (*callback)(void *arg), int dmaNo);
int WM_SetIndCallback(void (*callback)(void *arg));
int WM_End(void (*callback)(void *arg));
int WM_Reset(void (*callback)(void *arg));
int WM_StartScan(void (*callback)(void *arg), void *scanParam);
int WM_EndScan(void (*callback)(void *arg));
int WM_GetAllowedChannel(void);
u32 _u32_div_f(u32 dividend, u32 divisor);

extern MysteryGiftBeacon gMysteryGiftBeacon;
extern const char *gMysteryGiftBeaconStateNames[];
extern const char gFmt_StrSpaceDashGtSpace[];
extern const char gFmt_StrLf[];
extern const char gFmt_NotMyParentGgid[];

void MysteryGiftBeacon_SetState(int state) {
    void (*print)(const char *format, ...) = gMysteryGiftBeacon.print;
    if (print != NULL) {
        int index = gMysteryGiftBeacon.work->state;
        print(gFmt_StrSpaceDashGtSpace, gMysteryGiftBeaconStateNames[index]); // "%s -> "
    }
    gMysteryGiftBeacon.work->state = state;
    print = gMysteryGiftBeacon.print;
    if (print != NULL) {
        int index = gMysteryGiftBeacon.work->state;
        print(gFmt_StrLf, gMysteryGiftBeaconStateNames[index]); // "%s\n"
    }
}

void MysteryGiftBeacon_SetError(int error) {
    MysteryGiftBeaconWork *work = gMysteryGiftBeacon.work;
    if ((u32)(work->state - MYSTERY_GIFT_BEACON_STATE_ERROR) > 1) { // not after an error
        work->error = error;
    }
}

BOOL MysteryGiftBeacon_StartScan(void) {
    if (gMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_CONNECTED) {
        OS_Terminate();
    }
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_SCANNING);
    *(u16 *)&gMysteryGiftBeacon.work->scanParam.bssid[4] = 0xFFFF;
    *(u16 *)&gMysteryGiftBeacon.work->scanParam.bssid[2] = 0xFFFF;
    *(u16 *)&gMysteryGiftBeacon.work->scanParam.bssid[0] = 0xFFFF;
    if (MysteryGiftBeacon_ScanChannel() == FALSE) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = gMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return FALSE;
    }
    gMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_SEARCH;
    void (*callback)(int) = gMysteryGiftBeacon.work->callback;
    if (callback != NULL) {
        callback(MYSTERY_GIFT_BEACON_EVENT_SEARCHING);
    }
    return TRUE;
}

#pragma push
#pragma opt_rotateloops off
#pragma opt_common_subs off
#pragma opt_propagation off
#pragma opt_dead_assignments off

// Scans the next allowed channel (while searching) or the station's channel
BOOL MysteryGiftBeacon_ScanChannel(void) {
    int channels;
    int index;
    int i;
    MYSTERY_GIFT_BEACON_ASSERT(gMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_SCANNING);
    channels = WM_GetAllowedChannel();
    if (channels == 0x8000) {
        MysteryGiftBeacon_SetError(3);
        return FALSE;
    }
    if (channels == 0) {
        MysteryGiftBeacon_SetError(22);
        return FALSE;
    }
    index = gMysteryGiftBeacon.work->channelIndex;
    if ((u32)index >= 16) {
        index -= 16;
        for (i = 0; i < 13; i++) {
            index = (index + 1) % 13;
            if (channels & (1 << index)) {
                break;
            }
        }
        gMysteryGiftBeacon.work->channelIndex = index + 16;
        gMysteryGiftBeacon.work->scanParam.channel = index + 1;
    } else {
        gMysteryGiftBeacon.work->scanParam.channel = index;
    }
    gMysteryGiftBeacon.work->scanParam.maxChannelTime = 220;
    gMysteryGiftBeacon.work->scanParam.scanBuffer = gMysteryGiftBeacon.work->scanBuffer;
    if (gMysteryGiftBeacon.work->mode == MYSTERY_GIFT_BEACON_MODE_RECEIVE) {
        for (i = 0; i < 6; i++) {
            gMysteryGiftBeacon.work->scanParam.bssid[i] = gMysteryGiftBeacon.work->parentMac[i];
        }
    }
    int result = WM_StartScan(MysteryGiftBeacon_ScanCallback, &gMysteryGiftBeacon.work->scanParam);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetError(result);
        return FALSE;
    }
    return TRUE;
}

#pragma pop

void MysteryGiftBeacon_ScanCallback(void *arg) {
    MysteryGiftBeaconScanResult *result = arg;
    if (result->errCode != WM_ERRCODE_SUCCESS) {
        MysteryGiftBeacon_SetError(result->errCode);
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        if (gMysteryGiftBeacon.work->callback != NULL) {
            gMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return;
    }
    if (gMysteryGiftBeacon.work->state != MYSTERY_GIFT_BEACON_STATE_SCANNING) {
        if (MysteryGiftBeacon_EndScan() == FALSE) {
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
            if (gMysteryGiftBeacon.work->callback != NULL) {
                gMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
            }
        }
        return;
    }
    switch (result->state) {
    case 3:
        return;
    case 4:
        break;
    case 5: // found a station
        DC_InvalidateRange(gMysteryGiftBeacon.work->scanBuffer, sizeof(gMysteryGiftBeacon.work->scanBuffer));
        if (result->gameInfoLength < 8 || result->ggid != MYSTERY_GIFT_BEACON_GGID) {
            if (gMysteryGiftBeacon.print != NULL) {
                gMysteryGiftBeacon.print(gFmt_NotMyParentGgid, result->ggid, MYSTERY_GIFT_BEACON_GGID); // "not my parent ggid (%d != %d)\n"
            }
            break;
        }
        gMysteryGiftBeacon.work->linkLevel = result->linkLevel;
        if (gMysteryGiftBeacon.work->mode == MYSTERY_GIFT_BEACON_MODE_SEARCH) {
            int i;
            MysteryGiftBeacon_StartReceive(&result->packet);
            for (i = 0; i < 6; i++) {
                gMysteryGiftBeacon.work->parentMac[i] = result->macAddress[i];
            }
            gMysteryGiftBeacon.work->channelIndex -= 15; // stay on the station's channel
            if (gMysteryGiftBeacon.work->callback != NULL) {
                gMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_FOUND);
            }
        }
        if (MysteryGiftBeacon_ReceivePacket(&result->packet)) {
            gMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_COMPLETE;
            MysteryGiftBeacon_Stop();
            return;
        }
        break;
    default:
        break;
    }
    if (!MysteryGiftBeacon_ScanChannel()) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        if (gMysteryGiftBeacon.work->callback != NULL) {
            gMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    }
}

BOOL MysteryGiftBeacon_EndScan(void) {
    int result = WM_EndScan(MysteryGiftBeacon_EndScanCallback);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void MysteryGiftBeacon_EndScanCallback(void *arg) {
    int result = ((MysteryGiftBeaconWmResult *)arg)->errCode;
    if (result != WM_ERRCODE_SUCCESS) {
        MysteryGiftBeacon_SetError(result);
    } else {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_IDLE);
    }
}

void MysteryGiftBeacon_EndCallback(void *arg) {
    if (((MysteryGiftBeaconWmResult *)arg)->errCode != WM_ERRCODE_SUCCESS) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_FATAL);
    } else {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_STOP);
        void (*callback)(int) = gMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_STOPPED);
        }
    }
}

BOOL MysteryGiftBeacon_Start(void) {
    gMysteryGiftBeacon.work->error = 0;
    if (MysteryGiftBeacon_Initialize() != FALSE) {
        return TRUE;
    }
    return FALSE;
}

// WM indication: the wireless hardware was shut down
void MysteryGiftBeacon_IndicationCallback(void *arg) {
    if (((MysteryGiftBeaconWmResult *)arg)->errCode == 8) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = gMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    }
}

BOOL MysteryGiftBeacon_Initialize(void) {
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_BUSY);
    int result = WM_Initialize(gMysteryGiftBeacon.work, MysteryGiftBeacon_InitializeCallback, 2);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetError(result);
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_FATAL);
        return FALSE;
    }
    return TRUE;
}

void MysteryGiftBeacon_InitializeCallback(void *arg) {
    int result = ((MysteryGiftBeaconWmResult *)arg)->errCode;
    if (result != WM_ERRCODE_SUCCESS) {
        MysteryGiftBeacon_SetError(result);
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_FATAL);
    } else {
        result = WM_SetIndCallback(MysteryGiftBeacon_IndicationCallback);
        if (result != WM_ERRCODE_SUCCESS) {
            MysteryGiftBeacon_SetError(result);
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_FATAL);
        } else {
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_IDLE);
            MysteryGiftBeacon_StartScan();
        }
    }
}

BOOL MysteryGiftBeacon_End(void) {
    if (gMysteryGiftBeacon.work->state != MYSTERY_GIFT_BEACON_STATE_IDLE) {
        OS_Terminate();
    }
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_BUSY);
    int result = WM_End(MysteryGiftBeacon_EndCallback);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = gMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return FALSE;
    }
    return TRUE;
}

// Stops: ends WM (resets it first unless idle); FALSE if that could not be started
BOOL MysteryGiftBeacon_Stop(void) {
    if (gMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_IDLE) {
        return MysteryGiftBeacon_End();
    } else {
        return MysteryGiftBeacon_Reset();
    }
}

BOOL MysteryGiftBeacon_Reset(void) {
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_BUSY);
    int result = WM_Reset(MysteryGiftBeacon_ResetCallback);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void MysteryGiftBeacon_ResetCallback(void *arg) {
    if (((MysteryGiftBeaconWmResult *)arg)->errCode != WM_ERRCODE_SUCCESS) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        MysteryGiftBeacon_SetError(((MysteryGiftBeaconWmResult *)arg)->errCode);
        void (*callback)(int) = gMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    } else {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_IDLE);
        MysteryGiftBeacon_End();
    }
}

BOOL MysteryGiftBeacon_Init(void *dest, void (*callback)(int event), void *memory) {
    gMysteryGiftBeacon.work = (MysteryGiftBeaconWork *)(((u32)memory + 31) & ~31);
    gMysteryGiftBeacon.work->state = MYSTERY_GIFT_BEACON_STATE_STOP;
    gMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_INIT;
    gMysteryGiftBeacon.work->dest = dest;
    gMysteryGiftBeacon.work->packetCount = 0;
    gMysteryGiftBeacon.work->packetsReceived = 0;
    gMysteryGiftBeacon.work->callback = callback;
    gMysteryGiftBeacon.work->channelIndex = 16;
    gMysteryGiftBeacon.work->titleReceived = FALSE;
    gMysteryGiftBeacon.work->linkLevel = 0xFFFFFFFF;
    return MysteryGiftBeacon_Start();
}

// The first packet of a gift: its id, size and packet count
void MysteryGiftBeacon_StartReceive(MysteryGiftBeaconPacket *packet) {
    gMysteryGiftBeacon.work->titleReceived = FALSE;
    gMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_RECEIVE;
    gMysteryGiftBeacon.work->dataId = packet->id;
    gMysteryGiftBeacon.work->dataSize = packet->size;
    MysteryGiftBeaconWork *work = gMysteryGiftBeacon.work;
    work->packetCount = _u32_div_f(work->dataSize - 1, MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE) + 1;
    gMysteryGiftBeacon.work->packetsReceived = 0;
    gMysteryGiftBeacon.work->mismatches = 0;
    for (int i = 0; i < gMysteryGiftBeacon.work->packetCount; i++) {
        gMysteryGiftBeacon.work->packetReceived[i] = FALSE;
    }
}

// Stores one packet; TRUE once the gift is complete
BOOL MysteryGiftBeacon_ReceivePacket(MysteryGiftBeaconPacket *packet) {
    MysteryGiftBeaconWork *work;
    void (*callback)(int);
    if (gMysteryGiftBeacon.work->dataId != packet->id || gMysteryGiftBeacon.work->dataSize != packet->size) {
        u32 mismatches = gMysteryGiftBeacon.work->mismatches++;
        if (mismatches >= 16) { // the station sends another gift now: start over
            MysteryGiftBeacon_StartReceive(packet);
            callback = gMysteryGiftBeacon.work->callback;
            if (callback != NULL) {
                callback(MYSTERY_GIFT_BEACON_EVENT_RESTARTED);
            }
        } else {
            return FALSE;
        }
    }
    if (packet->index == 0xFFFF) {
        MIi_CpuCopy32((const u32 *)packet->data, (u32 *)gMysteryGiftBeacon.work->title, MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
        gMysteryGiftBeacon.work->titleReceived = TRUE;
    } else {
        gMysteryGiftBeacon.work->lastPacket = packet->index;
        work = gMysteryGiftBeacon.work;
        if (packet->index >= work->packetCount) {
            callback = work->callback;
            if (callback != NULL) {
                callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
            }
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        }
        work = gMysteryGiftBeacon.work;
        {
            u8 *received = work->packetReceived;
            if (received[packet->index] == FALSE) {
                received[packet->index] = TRUE;
                work = gMysteryGiftBeacon.work;
                if (packet->index == work->packetCount - 1) {
                    MIi_CpuCopy32((const u32 *)packet->data, (u32 *)((u8 *)work->dest + packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE), work->dataSize - packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
                } else {
                    MIi_CpuCopy32((const u32 *)packet->data, (u32 *)((u8 *)work->dest + packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE), MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
                }
                gMysteryGiftBeacon.work->packetsReceived++;
                work = gMysteryGiftBeacon.work;
                if (work->packetsReceived == work->packetCount) {
                    callback = work->callback;
                    if (callback != NULL) {
                        callback(MYSTERY_GIFT_BEACON_EVENT_COMPLETE);
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

u16 MysteryGiftBeacon_Checksum(const u16 *data, u32 size) {
    u16 sum = 0;
    u16 i = 0;
    for (; i < size / 2; i++) {
        sum += data[i];
        sum = (sum << 1) | (sum >> 15);
    }
    return sum;
}

// Decrypts the downloaded gift in place (the key mixes the station's MAC address and the gift id); TRUE if the
// checksum matches the id
BOOL MysteryGiftBeacon_DecryptGift(void) {
    u16 key[4];
    MysteryGiftBeaconWork *work = gMysteryGiftBeacon.work;
    u16 *mac = (u16 *)work->parentMac;
    key[0] = mac[0];
    key[3] = mac[1];
    key[2] = mac[2];
    key[1] = work->dataId;
    int i = 0;
    u32 prev = 0x3FA2;
    for (; i < 4; i++) {
        key[i] ^= prev;
        prev = key[i];
    }
    CRYPTO_RC4Init(&gMysteryGiftBeacon.work->rc4, key, 8);
    work = gMysteryGiftBeacon.work;
    CRYPTO_RC4Encrypt(&work->rc4, work->dest, work->dataSize, work->dest);
    MIi_CpuClear32(0, (u32 *)&gMysteryGiftBeacon.work->rc4, sizeof(CRYPTORC4Context));
    u16 sum = MysteryGiftBeacon_Checksum(gMysteryGiftBeacon.work->dest, gMysteryGiftBeacon.work->dataSize);
    if (gMysteryGiftBeacon.work->dataId == sum) {
        return TRUE;
    }
    return FALSE;
}

// Size of the memory to pass to MysteryGiftBeacon_Init
u32 MysteryGiftBeacon_GetWorkSize(void) {
    return 0x16A0;
}

const u16 *MysteryGiftBeacon_GetTitle(void) {
    if (gMysteryGiftBeacon.work->titleReceived) {
        return gMysteryGiftBeacon.work->title;
    }
    return NULL;
}
