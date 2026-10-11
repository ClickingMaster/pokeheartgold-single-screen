#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Request blocks of WMSP_WL_MaKeyData and WMSP_WL_MaMp.
typedef union WlKeyMpRequest {
    u16 words[256];

    struct {
        u16 headerAndArgs[10];
        u32 encodedData;
    } key;

    struct {
        u16 headerAndArgs[16];
        u32 encodedData;
    } mp;
} WlKeyMpRequest;

u16 *WMSP_WL_MlmeReset(u16 *buf, u16 arg) {
    u16 *reply;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 1;
    buf[8] = arg;
    reply = buf + buf[7];
    reply[8] = buf[6];
    reply[9] = 1;
    WMSP_WlRequest(buf);
    return reply + 8;
}

static inline void InitMlmePowerManagementHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 1;
    packet[7] = 3;
}

static inline void InitMlmeScanHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 2;
    packet[7] = 31;
}

u16 *WMSP_WL_MlmePowerManagement(void *buffer, u16 first, u16 second, u16 third) {
    u16 *packet = buffer, *reply;
    InitMlmePowerManagementHeader(packet);
    packet[8] = first;
    packet[9] = second;
    packet[10] = third;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MlmeScan(void *buffer, u32 size, void *peer, u16 ssidLength, void *ssid, u16 scanType, void *channels, u16 dwell) {
    u16 *packet = buffer, *reply;
    InitMlmeScanHeader(packet);
    __VENEER_MIi_CpuCopy16(peer, packet + 8, 6);
    packet[11] = ssidLength;
    __VENEER_MIi_CpuCopy16(ssid, packet + 12, 32);
    packet[28] = scanType;
    __VENEER_MIi_CpuCopy16(channels, packet + 29, 16);
    packet[37] = dwell;
    packet[38] = 0;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = (size >> 1) - 44;
    WMSP_WlRequest(packet);
    return reply;
}

u16 *WMSP_WL_MlmeJoin(u16 *buf, u16 timeout, const void *bssDesc) {
    u16 *reply;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 3;
    buf[7] = 34;
    buf[8] = timeout;
    buf[9] = 0;
    __VENEER_MIi_CpuCopy16(bssDesc, buf + 10, 68);
    reply = buf + buf[7];
    reply[8] = buf[6];
    reply[9] = 5;
    WMSP_WlRequest(buf);
    return reply + 8;
}

static inline void InitMlmeAuthenticateHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 4;
    packet[7] = 5;
}

u16 *WMSP_WL_MlmeAuthenticate(void *buffer, const void *peer, u16 algorithm, u16 timeout) {
    u16 *packet = buffer, *reply;
    InitMlmeAuthenticateHeader(packet);
    __VENEER_MIi_CpuCopy16(peer, packet + 8, 6);
    packet[11] = algorithm;
    packet[12] = timeout;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = 6;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MlmeDeAuthenticate(u16 *buf, const void *peer, u16 reason) {
    u16 *reply;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 5;
    buf[7] = 4;
    __VENEER_MIi_CpuCopy16(peer, buf + 8, 6);
    buf[11] = reason;
    reply = buf + buf[7];
    reply[8] = buf[6];
    reply[9] = 4;
    WMSP_WlRequest(buf);
    return reply + 8;
}

u16 *WMSP_WL_MlmeAssociate(u16 *buf, const void *peer, u16 listenInterval, u16 timeout) {
    u16 *reply;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 6;
    buf[7] = 5;
    __VENEER_MIi_CpuCopy16(peer, buf + 8, 6);
    buf[11] = listenInterval;
    buf[12] = timeout;
    reply = buf + buf[7];
    reply[8] = buf[6];
    reply[9] = 3;
    WMSP_WlRequest(buf);
    return reply + 8;
}

static inline void InitMlmeStartHeader(u16 *p, u16 n) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 9;
    p[7] = (n + 1) / 2 + 23;
}

static inline void InitMlmeMeasureChannelHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 10;
    p[7] = 12;
}

u16 *WMSP_WL_MlmeStart(void *buffer, u16 ssidLength, void *ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, void *gameInfo) {
    u16 *p = buffer, *reply;
    InitMlmeStartHeader(p, gameInfoLength);
    p[8] = ssidLength;
    __VENEER_MIi_CpuCopy16(ssid, p + 9, 32);
    p[25] = beaconPeriod;
    p[26] = dtimPeriod;
    p[27] = channel;
    p[28] = basicRateSet;
    p[29] = supportRateSet;
    p[30] = gameInfoLength;
    __VENEER_MIi_CpuCopy16(gameInfo, p + 31, gameInfoLength);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MlmeMeasureChannel(void *buffer, u16 a, u16 b, u16 duration, void *channels) {
    u16 *p = buffer, *reply;
    InitMlmeMeasureChannelHeader(p);
    p[8] = 0;
    p[9] = a;
    p[10] = b;
    p[11] = duration;
    __VENEER_MIi_CpuCopy16(channels, p + 12, 16);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 18;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MaData(u16 *buf, u16 *frame) {
    u16 *reply;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 256;
    buf[7] = 24;
    __VENEER_MIi_CpuCopy16(frame, buf + 8, 48);
    frame[1] = 0;
    frame[2] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[8] = 0;
    frame[9] = 0;
    frame[10] = 0;
    frame[11] = 0;
    frame[18] = 0;
    frame[19] = 0;
    frame[20] = 0;
    frame[21] = 0;
    reply = buf + buf[7];
    reply[8] = buf[6];
    reply[9] = 2;
    WMSP_WlRequest(buf);
    return reply + 8;
}

static inline void InitMaKeyDataHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 257;
    p[7] = 4;
}

static inline void InitMaMpHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 258;
    p[7] = 10;
}

u16 *WMSP_WL_MaKeyData(void *buffer, u16 a, u16 b, void *data) {
    WlKeyMpRequest *packet = buffer;
    u16 *p = packet->words, *reply;
    InitMaKeyDataHeader(p);
    p[8] = a;
    p[9] = b;
    packet->key.encodedData = (u32)data;
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MaMp(void *buffer, u32 a, u32 b, u32 c, u16 d, u16 e, u16 f, u16 g, u16 h, u32 encodedData) {
    WlKeyMpRequest *packet = buffer;
    u16 *p = packet->words, *reply;
    InitMaMpHeader(p);
    p[8] = a;
    p[9] = b;
    p[10] = c;
    p[11] = d;
    p[12] = e;
    p[13] = f;
    p[14] = g;
    p[15] = h;
    packet->mp.encodedData = encodedData;
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MaClearData(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 260;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetAll(void *buf) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 512;
    q->paramLength = 72;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetWepKeyId(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 519;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetBeaconLostThreshold(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 523;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetSsidMask(void *buf, const void *src) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 525;
    q->paramLength = 16;
    __VENEER_MIi_CpuCopy16(src, param, 32);
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetPreambleType(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 526;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

static inline void InitParamSetLifeTimeHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 529;
    packet[7] = 3;
}

u16 *WMSP_WL_ParamSetLifeTime(void *buffer, u16 first, u16 second, u16 third) {
    u16 *packet = buffer, *reply;
    InitParamSetLifeTimeHeader(packet);
    packet[8] = first;
    packet[9] = second;
    packet[10] = third;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_ParamSetMaxConnectableChild(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 530;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetBeaconSendRecvInd(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 533;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetNullKeyResponseMode(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 534;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

u16 *WMSP_WL_ParamSetBeaconPeriod(void *buf, u16 a) {
    WlRequestHeader *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    WlRequestHeader *reply;
    q->unk_00[0] = 0;
    q->unk_00[1] = 0;
    q->unk_00[2] = 0;
    q->unk_00[3] = 0;
    q->unk_00[4] = 0;
    q->unk_00[5] = 0;
    q->command = 578;
    q->paramLength = 1;
    param[0] = a;
    reply = (WlRequestHeader *)((u8 *)buf + q->paramLength * 2);
    *((u16 *)((u8 *)reply + 16)) = q->command;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u16 *)((u8 *)reply + 16);
}

static inline void InitParamSetGameInfoHeader(u16 *p, u16 length) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 581;
    p[7] = (length + 1) / 2 + 1;
}

u16 *WMSP_WL_ParamSetGameInfo(void *buffer, u16 length, void *source) {
    u16 *p = buffer, *reply;
    InitParamSetGameInfoHeader(p, length);
    p[8] = length;
    __VENEER_MIi_CpuCopy16(source, p + 9, length);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

// -ipa file would otherwise inline WMSPi_WL_NoArg into these requests.
#pragma dont_inline on

u16 *WMSP_WL_ParamGetMacAddress(void *buf) {
    return WMSPi_WL_NoArg(buf, 641, 4);
}

u16 *WMSP_WL_ParamGetEnableChannel(void *buf) {
    return WMSPi_WL_NoArg(buf, 643, 3);
}

u16 *WMSP_WL_ParamGetMode(void *buf) {
    return WMSPi_WL_NoArg(buf, 644, 2);
}

u16 *WMSP_WL_DevShutdown(void *buf) {
    return WMSPi_WL_NoArg(buf, 769, 1);
}

u16 *WMSP_WL_DevIdle(void *buf) {
    return WMSPi_WL_NoArg(buf, 770, 1);
}

u16 *WMSP_WL_DevClass1(void *buf) {
    return WMSPi_WL_NoArg(buf, 771, 1);
}

u16 *WMSP_WL_DevRestart(void *buf) {
    return WMSPi_WL_NoArg(buf, 772, 1);
}

u16 *WMSP_WL_DevSetInitializeWirelessCounter(void *buf) {
    return WMSPi_WL_NoArg(buf, 773, 1);
}

u16 *WMSP_WL_DevGetVersion(void *buf) {
    return WMSPi_WL_NoArg(buf, 774, 9);
}

u16 *WMSP_WL_DevGetWirelessCounter(void *buf) {
    return WMSPi_WL_NoArg(buf, 775, 92);
}

u16 *WMSP_WL_DevGetStationState(void *buf) {
    return WMSPi_WL_NoArg(buf, 776, 2);
}

#pragma dont_inline reset

static inline void InitDevTestSignalHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 777;
    packet[7] = 4;
}

u16 *WMSP_WL_DevTestSignal(void *buffer, u16 first, u16 second, u16 third, u16 fourth) {
    u16 *packet = buffer, *reply;
    InitDevTestSignalHeader(packet);
    packet[8] = first;
    packet[9] = second;
    packet[10] = third;
    packet[11] = fourth;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

#pragma opt_propagation off

// Request without parameters.
u16 *WMSPi_WL_NoArg(u16 *p, u16 cmd, u16 size) {
    u32 i = 0;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = cmd;
    p[7] = 0;
    u16 *out = p + i;
    out[8] = p[6];
    out[9] = size;
    WMSP_WlRequest(p);
    return out + 8;
}

#pragma opt_propagation reset
