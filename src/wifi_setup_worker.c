#include <string.h>

#include "global.h"

#include "wifi_setup_internal.h"

// Socket address
typedef struct WifiSetupSockAddr {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} WifiSetupSockAddr;

typedef struct Id8 {
    u8 b[8];
} Id8;

// gWifiSetupClientSettings
typedef struct WifiSetupSettings {
    u8 unk_00[20];
    s8 unk_14;
    u8 unk_15[43];
} WifiSetupSettings;

// gWifiSetupReply
typedef struct WifiSetupReply {
    u32 words[149];
} WifiSetupReply;

// Both labels point into gWifiSetupWork (gWifiSetupWcmSearchSsid + 0x20 is its unk_00, gWifiSetupExtendedRequest its
// extendedRequest); the code addresses them through these labels.
extern u8 gWifiSetupWcmSearchSsid[];
extern int gWifiSetupExtendedRequest[];
extern u8 gWifiSetupSessionKey[16];   // key
extern u8 gWifiSetupPacketBuffer[2048]; // packet buffer
extern u8 gWifiSetupRecordBuffer[2048]; // record buffer
extern u8 gWifiSetupSettingsKey[8];
extern u8 gWifiSetupTimestampDigest[24];
extern WifiSetupReply gWifiSetupReply;
extern WifiSetupSettings gWifiSetupClientSettings;

int SOC_Socket(int domain, int type, int protocol);
int SOC_Bind(int socket, void *addr);
int SOC_Close(int socket);
int SOC_RecvFrom(int socket, void *buf, int len, int flags, void *from);
int WifiSetup_FindSetupAccessPoint(void);
int WifiSetup_StoreSimpleStartResult(void);
int WifiSetup_BuildMacPair(u8 *pair, WifiSetupSockAddr *from);
int WifiSetup_CheckVersionReply(void *buf, int *out);
u32 WifiSetup_BuildVersionRequest(void *buf);
int WifiSetup_SendSetupPacket(int socket, WifiSetupSockAddr *to, const void *buf, int len);
int WifiSetup_ReadPacket(u8 *buf, u32 type, void *out, const u8 *key);
u8 *WifiSetup_FirstRecord(u8 *buf, u32 *type, u32 *size);
u32 WifiSetup_BuildSingleRecordList(u8 *buf, u16 type, const void *data, u32 len);
u32 WifiSetup_BuildPacket(u8 *out, u32 type, u8 *buf, u32 len, const u8 *key);
int WifiSetup_ParseSettingsRecords(void *buf);

// WifiSetup_RunSession steps (WifiSetupWork.step)
enum WifiSetupSessionStep {
    SESSION_STEP_FIND_AP = 1,
    SESSION_STEP_CONNECT_AP,
    SESSION_STEP_OPEN_SOCKET,
    SESSION_STEP_WAIT_VERSION,
    SESSION_STEP_SEND_VERSION_REQUEST,
    SESSION_STEP_WAIT_KEY,
    SESSION_STEP_SEND_KEY,
    SESSION_STEP_WAIT_SETTINGS,
    SESSION_STEP_SEND_RESULT,
    SESSION_STEP_RESEND_RESULT,
};

static inline int BindSocket(int socket, WifiSetupSockAddr *address) {
    memset(address, 0, sizeof(*address));
    address->len = 8;
    address->family = 2;
    address->port = 486;
    address->addr = 0;
    return SOC_Bind(socket, address);
}

#pragma force_active on
#pragma opt_dead_assignments off

// Worker state machine: scan for the access point, connect, open the UDP socket (port 486), wait for
// the request, exchange keys, then send and receive the settings records until done.
int WifiSetup_RunSession(void) {
    WifiSetupSockAddr bind, recv;
    u32 type, size;
    u32 timestamp;
    u8 *p;
    int sock;
    int done;
    int result;
    u32 last;
    int retry;

    sock = 0;
    result = -5;
    last = 0;
    retry = 0;
    done = 0;

    gWifiSetupWork.step = SESSION_STEP_FIND_AP;
    while (!done && !gWifiSetupWork.stop) {
        OS_Sleep(500);
        switch (gWifiSetupWork.step) {
        case SESSION_STEP_FIND_AP:
            result = WifiSetup_FindSetupAccessPoint();
            if (result != 1) {
                done = 1;
                break;
            }
            gWifiSetupWork.state = 3;
            WifiSetup_ReportSimpleStartStatus();
            gWifiSetupWork.step = SESSION_STEP_CONNECT_AP;
            break;
        case SESSION_STEP_CONNECT_AP:
            result = WifiSetupLink_ConnectSelectedAp();
            if (result != 1) {
                done = 1;
                break;
            }
            gWifiSetupWork.step = SESSION_STEP_OPEN_SOCKET;
            break;
        case SESSION_STEP_OPEN_SOCKET:
            sock = SOC_Socket(2, 2, 0);
            if (sock < 0) {
                result = -2;
                done = 1;
                break;
            }
            result = BindSocket(sock, &bind);
            if (result < 0) {
                result = -2;
                done = 1;
                break;
            }
            gWifiSetupWork.step = SESSION_STEP_WAIT_VERSION;
            break;
        case SESSION_STEP_WAIT_VERSION:
            if (WifiSetup_GetMilliseconds() >= gWifiSetupConfig.deadline) {
                SOC_Close(sock);
                result = -3;
                done = 1;
                break;
            }
            recv.len = 8;
            WifiSetup_BuildMacPair(gWifiSetupSessionKey, &recv);
            if (SOC_RecvFrom(sock, gWifiSetupPacketBuffer, 2048, 4, &recv) > 0 && WifiSetup_CheckVersionReply(gWifiSetupPacketBuffer, gWifiSetupExtendedRequest)) {
                gWifiSetupConfig.deadline = WifiSetup_GetMilliseconds() + 30000;
                gWifiSetupWork.step = SESSION_STEP_SEND_VERSION_REQUEST;
                gWifiSetupWork.state = 4;
                WifiSetup_ReportSimpleStartStatus();
            }
            break;
        case SESSION_STEP_SEND_VERSION_REQUEST:
            gWifiSetupWork.packetLength = WifiSetup_BuildVersionRequest(gWifiSetupPacketBuffer);
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, gWifiSetupWork.packetLength);
            last = WifiSetup_GetMilliseconds();
            gWifiSetupWork.step = SESSION_STEP_WAIT_KEY;
            break;
        case SESSION_STEP_WAIT_KEY:
            if (WifiSetup_GetMilliseconds() >= gWifiSetupConfig.deadline) {
                SOC_Close(sock);
                result = -4;
                done = 1;
                break;
            }
            if (SOC_RecvFrom(sock, gWifiSetupPacketBuffer, 2048, 4, &recv) > 0 && WifiSetup_ReadPacket(gWifiSetupPacketBuffer, 3, gWifiSetupRecordBuffer, gWifiSetupSessionKey)) {
                p = WifiSetup_FirstRecord(gWifiSetupRecordBuffer, &type, &size);
                if (type == 257) {
                    timestamp = WifiSetup_GetMilliseconds();
                    *(Id8 *)gWifiSetupSettingsKey = *(Id8 *)p;
                    WifiSetup_CalcMd5(gWifiSetupTimestampDigest, (const u8 *)&timestamp, 4);
                    retry = 0;
                    gWifiSetupWork.step = SESSION_STEP_SEND_KEY;
                    gWifiSetupWork.state = 5;
                    gWifiSetupConfig.deadline = -1;
                    WifiSetup_ReportSimpleStartStatus();
                }
                p = NULL;
                break;
            }
            if (WifiSetup_GetMilliseconds() >= last + 1000) {
                gWifiSetupWork.step = SESSION_STEP_SEND_VERSION_REQUEST;
            }
            break;
        case SESSION_STEP_SEND_KEY:
            gWifiSetupWork.recordLength = WifiSetup_BuildSingleRecordList(gWifiSetupRecordBuffer, 258, gWifiSetupTimestampDigest, 8);
            gWifiSetupWork.packetLength = WifiSetup_BuildPacket(gWifiSetupPacketBuffer, 4, gWifiSetupRecordBuffer, gWifiSetupWork.recordLength, gWifiSetupSessionKey);
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, gWifiSetupWork.packetLength);
            last = WifiSetup_GetMilliseconds();
            memset(&gWifiSetupReply, 0, sizeof(gWifiSetupReply));
            gWifiSetupWork.step = SESSION_STEP_WAIT_SETTINGS;
            break;
        case SESSION_STEP_WAIT_SETTINGS:
            if (SOC_RecvFrom(sock, gWifiSetupPacketBuffer, 2048, 4, &recv) > 0) {
                gWifiSetupWork.recordLength = WifiSetup_ReadPacket(gWifiSetupPacketBuffer, 5, gWifiSetupRecordBuffer, gWifiSetupSettingsKey);
                if (gWifiSetupWork.recordLength && WifiSetup_ParseSettingsRecords(gWifiSetupRecordBuffer)) {
                    if (gWifiSetupClientSettings.unk_14) {
                        gWifiSetupWork.unk_00 = 1;
                    } else {
                        gWifiSetupWork.unk_00 = 0;
                    }
                    retry = 0;
                    gWifiSetupWork.step = SESSION_STEP_SEND_RESULT;
                    break;
                }
            }
            if (WifiSetup_GetMilliseconds() >= last + 1000) {
                retry++;
                if (retry >= 10) {
                    SOC_Close(sock);
                    result = -2;
                    done = 1;
                } else {
                    gWifiSetupWork.step = SESSION_STEP_SEND_KEY;
                }
            }
            break;
        case SESSION_STEP_SEND_RESULT:
            gWifiSetupWork.recordLength = WifiSetup_BuildSingleRecordList(gWifiSetupRecordBuffer, 769, gWifiSetupWcmSearchSsid + 0x20, 1);
            gWifiSetupWork.packetLength = WifiSetup_BuildPacket(gWifiSetupPacketBuffer, 6, gWifiSetupRecordBuffer, gWifiSetupWork.recordLength, gWifiSetupSettingsKey);
            if (WifiSetupLink_GetState() != 7) {
                last = WifiSetup_GetMilliseconds() + 1000;
                retry = 10;
                gWifiSetupWork.step = SESSION_STEP_RESEND_RESULT;
                break;
            }
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, gWifiSetupWork.packetLength);
            last = WifiSetup_GetMilliseconds();
            gWifiSetupWork.step = SESSION_STEP_RESEND_RESULT;
            break;
        case SESSION_STEP_RESEND_RESULT:
            if (WifiSetup_GetMilliseconds() >= last + 1000) {
                retry++;
                if (retry >= 10) {
                    done = 1;
                    result = WifiSetup_StoreSimpleStartResult();
                } else {
                    gWifiSetupWork.step = SESSION_STEP_SEND_RESULT;
                }
            }
            break;
        }
    }
    if (sock) {
        SOC_Close(sock);
    }
    if (gWifiSetupWork.stop) {
        result = -8;
    }
    return result;
}

#pragma opt_dead_assignments reset
#pragma force_active reset
