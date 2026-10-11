#include "application/main_menu/mystery_gift_beacon.h"

#include "global.h"

#include "library/nitrocrypto_rc4.h"

// Mystery Gift > Get via Wireless Communication: downloads a gift that a distribution station broadcasts in its
// beacons (no connection). Every beacon of the station (GGID 0x400318) carries one MysteryGiftBeaconPacket; the gift
// is RC4-encrypted, its id is the checksum of the decrypted data.

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
    u8 wmBuffer[0xF00];                                  // 0x0000, WM_Initialize
    u8 scanBuffer[0xC0];                                 // 0x0F00
    MysteryGiftBeaconScanParam scanParam;                // 0x0FC0
    u8 unk_FD0[0x10];                                    // 0x0FD0
    u16 title[MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE / 2]; // 0x0FE0, packet 0xFFFF
    BOOL titleReceived;                                  // 0x1048
    CRYPTORC4Context rc4;                                // 0x104C
    int state;                                           // 0x1150, MYSTERY_GIFT_BEACON_STATE_*
    int error;                                           // 0x1154, the first WM error
    u16 mode;                                            // 0x1158, MYSTERY_GIFT_BEACON_MODE_*
    u16 dataId;                                          // 0x115A, checksum of the gift
    u16 packetCount;                                     // 0x115C
    u16 packetsReceived;                                 // 0x115E
    u32 dataSize;                                        // 0x1160
    u32 mismatches;                                      // 0x1164, packets of another gift
    u32 lastPacket;                                      // 0x1168
    u32 linkLevel;                                       // 0x116C
    u8 parentMac[6];                                     // 0x1170, the station
    u16 channelIndex;                                    // 0x1176, 16 + channel while searching
    void *dest;                                          // 0x1178
    void (*callback)(int event);                         // 0x117C, MYSTERY_GIFT_BEACON_EVENT_*
    u8 packetReceived[];                                 // 0x1180
} MysteryGiftBeaconWork;

typedef struct MysteryGiftBeacon {
    MysteryGiftBeaconWork *work;
    void (*print)(const char *format, ...); // debug output, never set
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
void OS_Terminate(void);
u32 _u32_div_f(u32 dividend, u32 divisor);

extern MysteryGiftBeacon sMysteryGiftBeacon;
extern const char *sMysteryGiftBeaconStateNames[];
extern const char ov74_0223D00C[];
extern const char ov74_0223D014[];
extern const char ov74_0223D018[];

void MysteryGiftBeacon_SetState(int state) {
    void (*print)(const char *format, ...) = sMysteryGiftBeacon.print;
    if (print != NULL) {
        int index = sMysteryGiftBeacon.work->state;
        print(ov74_0223D00C, sMysteryGiftBeaconStateNames[index]); // "%s -> "
    }
    sMysteryGiftBeacon.work->state = state;
    print = sMysteryGiftBeacon.print;
    if (print != NULL) {
        int index = sMysteryGiftBeacon.work->state;
        print(ov74_0223D014, sMysteryGiftBeaconStateNames[index]); // "%s\n"
    }
}

void MysteryGiftBeacon_SetError(int error) {
    MysteryGiftBeaconWork *work = sMysteryGiftBeacon.work;
    if ((u32)(work->state - MYSTERY_GIFT_BEACON_STATE_ERROR) > 1) { // not after an error
        work->error = error;
    }
}

BOOL MysteryGiftBeacon_StartScan(void) {
    if (sMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_CONNECTED) {
        OS_Terminate();
    }
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_SCANNING);
    *(u16 *)&sMysteryGiftBeacon.work->scanParam.bssid[4] = 0xFFFF;
    *(u16 *)&sMysteryGiftBeacon.work->scanParam.bssid[2] = 0xFFFF;
    *(u16 *)&sMysteryGiftBeacon.work->scanParam.bssid[0] = 0xFFFF;
    if (MysteryGiftBeacon_ScanChannel() == FALSE) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = sMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return FALSE;
    }
    sMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_SEARCH;
    void (*callback)(int) = sMysteryGiftBeacon.work->callback;
    if (callback != NULL) {
        callback(MYSTERY_GIFT_BEACON_EVENT_SEARCHING);
    }
    return TRUE;
}

#pragma force_active on
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
    MYSTERY_GIFT_BEACON_ASSERT(sMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_SCANNING);
    channels = WM_GetAllowedChannel();
    if (channels == 0x8000) {
        MysteryGiftBeacon_SetError(3);
        return FALSE;
    }
    if (channels == 0) {
        MysteryGiftBeacon_SetError(22);
        return FALSE;
    }
    index = sMysteryGiftBeacon.work->channelIndex;
    if ((u32)index >= 16) {
        index -= 16;
        for (i = 0; i < 13; i++) {
            index = (index + 1) % 13;
            if (channels & (1 << index)) {
                break;
            }
        }
        sMysteryGiftBeacon.work->channelIndex = index + 16;
        sMysteryGiftBeacon.work->scanParam.channel = index + 1;
    } else {
        sMysteryGiftBeacon.work->scanParam.channel = index;
    }
    sMysteryGiftBeacon.work->scanParam.maxChannelTime = 220;
    sMysteryGiftBeacon.work->scanParam.scanBuffer = sMysteryGiftBeacon.work->scanBuffer;
    if (sMysteryGiftBeacon.work->mode == MYSTERY_GIFT_BEACON_MODE_RECEIVE) {
        for (i = 0; i < 6; i++) {
            sMysteryGiftBeacon.work->scanParam.bssid[i] = sMysteryGiftBeacon.work->parentMac[i];
        }
    }
    int result = WM_StartScan(MysteryGiftBeacon_ScanCallback, &sMysteryGiftBeacon.work->scanParam);
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
        if (sMysteryGiftBeacon.work->callback != NULL) {
            sMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return;
    }
    if (sMysteryGiftBeacon.work->state != MYSTERY_GIFT_BEACON_STATE_SCANNING) {
        if (MysteryGiftBeacon_EndScan() == FALSE) {
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
            if (sMysteryGiftBeacon.work->callback != NULL) {
                sMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
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
        DC_InvalidateRange(sMysteryGiftBeacon.work->scanBuffer, sizeof(sMysteryGiftBeacon.work->scanBuffer));
        if (result->gameInfoLength < 8 || result->ggid != MYSTERY_GIFT_BEACON_GGID) {
            if (sMysteryGiftBeacon.print != NULL) {
                sMysteryGiftBeacon.print(ov74_0223D018, result->ggid, MYSTERY_GIFT_BEACON_GGID); // "not my parent ggid (%d != %d)\n"
            }
            break;
        }
        sMysteryGiftBeacon.work->linkLevel = result->linkLevel;
        if (sMysteryGiftBeacon.work->mode == MYSTERY_GIFT_BEACON_MODE_SEARCH) {
            int i;
            MysteryGiftBeacon_StartReceive(&result->packet);
            for (i = 0; i < 6; i++) {
                sMysteryGiftBeacon.work->parentMac[i] = result->macAddress[i];
            }
            sMysteryGiftBeacon.work->channelIndex -= 15; // stay on the station's channel
            if (sMysteryGiftBeacon.work->callback != NULL) {
                sMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_FOUND);
            }
        }
        if (MysteryGiftBeacon_ReceivePacket(&result->packet)) {
            sMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_COMPLETE;
            MysteryGiftBeacon_Stop();
            return;
        }
        break;
    default:
        break;
    }
    if (!MysteryGiftBeacon_ScanChannel()) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        if (sMysteryGiftBeacon.work->callback != NULL) {
            sMysteryGiftBeacon.work->callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    }
}

#pragma force_active reset

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
        void (*callback)(int) = sMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_STOPPED);
        }
    }
}

BOOL MysteryGiftBeacon_Start(void) {
    sMysteryGiftBeacon.work->error = 0;
    if (MysteryGiftBeacon_Initialize() != FALSE) {
        return TRUE;
    }
    return FALSE;
}

// WM indication: the wireless hardware was shut down
void MysteryGiftBeacon_IndicationCallback(void *arg) {
    if (((MysteryGiftBeaconWmResult *)arg)->errCode == 8) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = sMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    }
}

BOOL MysteryGiftBeacon_Initialize(void) {
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_BUSY);
    int result = WM_Initialize(sMysteryGiftBeacon.work, MysteryGiftBeacon_InitializeCallback, 2);
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
    if (sMysteryGiftBeacon.work->state != MYSTERY_GIFT_BEACON_STATE_IDLE) {
        OS_Terminate();
    }
    MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_BUSY);
    int result = WM_End(MysteryGiftBeacon_EndCallback);
    if (result != WM_ERRCODE_OPERATING) {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        void (*callback)(int) = sMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
        return FALSE;
    }
    return TRUE;
}

// Stops: ends WM (resets it first unless idle); FALSE if that could not be started
BOOL MysteryGiftBeacon_Stop(void) {
    if (sMysteryGiftBeacon.work->state == MYSTERY_GIFT_BEACON_STATE_IDLE) {
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
        void (*callback)(int) = sMysteryGiftBeacon.work->callback;
        if (callback != NULL) {
            callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
        }
    } else {
        MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_IDLE);
        MysteryGiftBeacon_End();
    }
}

BOOL MysteryGiftBeacon_Init(void *dest, void (*callback)(int event), void *memory) {
    sMysteryGiftBeacon.work = (MysteryGiftBeaconWork *)(((u32)memory + 31) & ~31);
    sMysteryGiftBeacon.work->state = MYSTERY_GIFT_BEACON_STATE_STOP;
    sMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_INIT;
    sMysteryGiftBeacon.work->dest = dest;
    sMysteryGiftBeacon.work->packetCount = 0;
    sMysteryGiftBeacon.work->packetsReceived = 0;
    sMysteryGiftBeacon.work->callback = callback;
    sMysteryGiftBeacon.work->channelIndex = 16;
    sMysteryGiftBeacon.work->titleReceived = FALSE;
    sMysteryGiftBeacon.work->linkLevel = 0xFFFFFFFF;
    return MysteryGiftBeacon_Start();
}

// The first packet of a gift: its id, size and packet count
void MysteryGiftBeacon_StartReceive(MysteryGiftBeaconPacket *packet) {
    sMysteryGiftBeacon.work->titleReceived = FALSE;
    sMysteryGiftBeacon.work->mode = MYSTERY_GIFT_BEACON_MODE_RECEIVE;
    sMysteryGiftBeacon.work->dataId = packet->id;
    sMysteryGiftBeacon.work->dataSize = packet->size;
    MysteryGiftBeaconWork *work = sMysteryGiftBeacon.work;
    work->packetCount = _u32_div_f(work->dataSize - 1, MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE) + 1;
    sMysteryGiftBeacon.work->packetsReceived = 0;
    sMysteryGiftBeacon.work->mismatches = 0;
    for (int i = 0; i < sMysteryGiftBeacon.work->packetCount; i++) {
        sMysteryGiftBeacon.work->packetReceived[i] = FALSE;
    }
}

#pragma force_active on

// Stores one packet; TRUE once the gift is complete
BOOL MysteryGiftBeacon_ReceivePacket(MysteryGiftBeaconPacket *packet) {
    MysteryGiftBeaconWork *work;
    void (*callback)(int);
    if (sMysteryGiftBeacon.work->dataId != packet->id || sMysteryGiftBeacon.work->dataSize != packet->size) {
        u32 mismatches = sMysteryGiftBeacon.work->mismatches++;
        if (mismatches >= 16) { // the station sends another gift now: start over
            MysteryGiftBeacon_StartReceive(packet);
            callback = sMysteryGiftBeacon.work->callback;
            if (callback != NULL) {
                callback(MYSTERY_GIFT_BEACON_EVENT_RESTARTED);
            }
        } else {
            return FALSE;
        }
    }
    if (packet->index == 0xFFFF) {
        MIi_CpuCopy32((const u32 *)packet->data, (u32 *)sMysteryGiftBeacon.work->title, MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
        sMysteryGiftBeacon.work->titleReceived = TRUE;
    } else {
        sMysteryGiftBeacon.work->lastPacket = packet->index;
        work = sMysteryGiftBeacon.work;
        if (packet->index >= work->packetCount) {
            callback = work->callback;
            if (callback != NULL) {
                callback(MYSTERY_GIFT_BEACON_EVENT_ERROR);
            }
            MysteryGiftBeacon_SetState(MYSTERY_GIFT_BEACON_STATE_ERROR);
        }
        work = sMysteryGiftBeacon.work;
        {
            u8 *received = work->packetReceived;
            if (received[packet->index] == FALSE) {
                received[packet->index] = TRUE;
                work = sMysteryGiftBeacon.work;
                if (packet->index == work->packetCount - 1) {
                    MIi_CpuCopy32((const u32 *)packet->data, (u32 *)((u8 *)work->dest + packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE), work->dataSize - packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
                } else {
                    MIi_CpuCopy32((const u32 *)packet->data, (u32 *)((u8 *)work->dest + packet->index * MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE), MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE);
                }
                sMysteryGiftBeacon.work->packetsReceived++;
                work = sMysteryGiftBeacon.work;
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
    MysteryGiftBeaconWork *work = sMysteryGiftBeacon.work;
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
    CRYPTO_RC4Init(&sMysteryGiftBeacon.work->rc4, key, 8);
    work = sMysteryGiftBeacon.work;
    CRYPTO_RC4Encrypt(&work->rc4, work->dest, work->dataSize, work->dest);
    MIi_CpuClear32(0, (u32 *)&sMysteryGiftBeacon.work->rc4, sizeof(CRYPTORC4Context));
    u16 sum = MysteryGiftBeacon_Checksum(sMysteryGiftBeacon.work->dest, sMysteryGiftBeacon.work->dataSize);
    if (sMysteryGiftBeacon.work->dataId == sum) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// Size of the memory to pass to MysteryGiftBeacon_Init
u32 MysteryGiftBeacon_GetWorkSize(void) {
    return 0x16A0;
}

#pragma force_active on

const u16 *MysteryGiftBeacon_GetTitle(void) {
    if (sMysteryGiftBeacon.work->titleReceived) {
        return sMysteryGiftBeacon.work->title;
    }
    return NULL;
}

#pragma force_active reset
