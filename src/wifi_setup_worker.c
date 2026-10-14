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

// Both labels point into ov13_0224DF30 (gWifiSetupWcmSearchSsid + 0x20 is its unk_00, ov13_0224DF80 its
// extendedRequest); the code addresses them through these labels.
extern u8 gWifiSetupWcmSearchSsid[];
extern int ov13_0224DF80[];
extern u8 gWifiSetupSessionKey[16];   // key
extern u8 gWifiSetupPacketBuffer[2048]; // packet buffer
extern u8 gWifiSetupRecordBuffer[2048]; // record buffer
extern u8 ov13_0224E400[8];
extern u8 ov13_0224E408[24];
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

    ov13_0224DF30.step = 1;
    while (!done && !ov13_0224DF30.stop) {
        OS_Sleep(500);
        switch (ov13_0224DF30.step) {
        case 1:
            result = WifiSetup_FindSetupAccessPoint();
            if (result != 1) {
                done = 1;
                break;
            }
            ov13_0224DF30.state = 3;
            WifiSetup_ReportSimpleStartStatus();
            ov13_0224DF30.step = 2;
            break;
        case 2:
            result = WifiSetupLink_ConnectSelectedAp();
            if (result != 1) {
                done = 1;
                break;
            }
            ov13_0224DF30.step = 3;
            break;
        case 3:
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
            ov13_0224DF30.step = 4;
            break;
        case 4:
            if (WifiSetup_GetMilliseconds() >= gWifiSetupConfig.deadline) {
                SOC_Close(sock);
                result = -3;
                done = 1;
                break;
            }
            recv.len = 8;
            WifiSetup_BuildMacPair(gWifiSetupSessionKey, &recv);
            if (SOC_RecvFrom(sock, gWifiSetupPacketBuffer, 2048, 4, &recv) > 0 && WifiSetup_CheckVersionReply(gWifiSetupPacketBuffer, ov13_0224DF80)) {
                gWifiSetupConfig.deadline = WifiSetup_GetMilliseconds() + 30000;
                ov13_0224DF30.step = 5;
                ov13_0224DF30.state = 4;
                WifiSetup_ReportSimpleStartStatus();
            }
            break;
        case 5:
            ov13_0224DF30.packetLength = WifiSetup_BuildVersionRequest(gWifiSetupPacketBuffer);
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, ov13_0224DF30.packetLength);
            last = WifiSetup_GetMilliseconds();
            ov13_0224DF30.step = 6;
            break;
        case 6:
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
                    *(Id8 *)ov13_0224E400 = *(Id8 *)p;
                    WifiSetup_CalcMd5(ov13_0224E408, (const u8 *)&timestamp, 4);
                    retry = 0;
                    ov13_0224DF30.step = 7;
                    ov13_0224DF30.state = 5;
                    gWifiSetupConfig.deadline = -1;
                    WifiSetup_ReportSimpleStartStatus();
                }
                p = NULL;
                break;
            }
            if (WifiSetup_GetMilliseconds() >= last + 1000) {
                ov13_0224DF30.step = 5;
            }
            break;
        case 7:
            ov13_0224DF30.recordLength = WifiSetup_BuildSingleRecordList(gWifiSetupRecordBuffer, 258, ov13_0224E408, 8);
            ov13_0224DF30.packetLength = WifiSetup_BuildPacket(gWifiSetupPacketBuffer, 4, gWifiSetupRecordBuffer, ov13_0224DF30.recordLength, gWifiSetupSessionKey);
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, ov13_0224DF30.packetLength);
            last = WifiSetup_GetMilliseconds();
            memset(&gWifiSetupReply, 0, sizeof(gWifiSetupReply));
            ov13_0224DF30.step = 8;
            break;
        case 8:
            if (SOC_RecvFrom(sock, gWifiSetupPacketBuffer, 2048, 4, &recv) > 0) {
                ov13_0224DF30.recordLength = WifiSetup_ReadPacket(gWifiSetupPacketBuffer, 5, gWifiSetupRecordBuffer, ov13_0224E400);
                if (ov13_0224DF30.recordLength && WifiSetup_ParseSettingsRecords(gWifiSetupRecordBuffer)) {
                    if (gWifiSetupClientSettings.unk_14) {
                        ov13_0224DF30.unk_00 = 1;
                    } else {
                        ov13_0224DF30.unk_00 = 0;
                    }
                    retry = 0;
                    ov13_0224DF30.step = 9;
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
                    ov13_0224DF30.step = 7;
                }
            }
            break;
        case 9:
            ov13_0224DF30.recordLength = WifiSetup_BuildSingleRecordList(gWifiSetupRecordBuffer, 769, gWifiSetupWcmSearchSsid + 0x20, 1);
            ov13_0224DF30.packetLength = WifiSetup_BuildPacket(gWifiSetupPacketBuffer, 6, gWifiSetupRecordBuffer, ov13_0224DF30.recordLength, ov13_0224E400);
            if (WifiSetupLink_GetState() != 7) {
                last = WifiSetup_GetMilliseconds() + 1000;
                retry = 10;
                ov13_0224DF30.step = 10;
                break;
            }
            WifiSetup_SendSetupPacket(sock, &recv, gWifiSetupPacketBuffer, ov13_0224DF30.packetLength);
            last = WifiSetup_GetMilliseconds();
            ov13_0224DF30.step = 10;
            break;
        case 10:
            if (WifiSetup_GetMilliseconds() >= last + 1000) {
                retry++;
                if (retry >= 10) {
                    done = 1;
                    result = WifiSetup_StoreSimpleStartResult();
                } else {
                    ov13_0224DF30.step = 9;
                }
            }
            break;
        }
    }
    if (sock) {
        SOC_Close(sock);
    }
    if (ov13_0224DF30.stop) {
        result = -8;
    }
    return result;
}

#pragma opt_dead_assignments reset
#pragma force_active reset
