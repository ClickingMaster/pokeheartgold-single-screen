#include "global.h"

#include "wifi_setup.h"
#include "wifi_setup_wcm.h"

typedef struct Pair {
    s16 max;
    s16 delay;
} Pair;

typedef struct Config {
    u32 ip;
    u32 pad[3];
    u32 gateway;
} Config;

typedef struct Context {
    u8 bytes[60];
} Context;

typedef struct Params {
    u8 bytes[24];
} Params;

typedef struct Data {
    int socket;
    int length;
    u32 pad;
    u8 bytes[1500];
} Data;

typedef struct AossSession {
    SocAddress address;
    WifiSetupPollTime time;
    SocPollFd set;
    int length;
    SocAddress remote;
    int attempts;
    int option;
    Pair retry;
    Pair connection;
    Config config;
    Context context;
    Params params;
} AossSession;

extern const Pair _0224267C;
extern void *ov13_0224CF9C;
extern s8 ov13_0224CFB8[];
extern int gWifiSetupSocket;

void WifiSetup_InitPacketAddresses(int, Params *, void *);
int WifiSetup_FindAossAccessPoint(void *);
int WifiSetup_InitAossConnectInfo(Context *);
int WifiSetup_ConnectAccessPoint(Context *, int *);
int WifiSetup_SendRequest(int, Config *, Params *, int);
int WifiSetup_HandlePacket(int, Data *, int *, Params *, int);
int WifiSetup_StoreAossResult(WifiSetupRequest *);

static inline void PutPort(SocAddress *address, u16 port) {
    address->port = port;
}

static inline int GetTimeout(WifiSetupRequest *request) {
    return request->receiveTimeout;
}

static inline void ClearRetry(Pair *pair, int *mode) {
    *(s16 *)((u32)pair) = 0;
    *mode = 0;
    *(s16 *)((u32)pair + 2) = 0;
}

#pragma opt_loop_invariants off
// Find the AOSS access point and connect to it, then send requests and handle the replies until the settings arrive.
int WifiSetup_RunAossSession(WifiSetupRequest *request) {
    AossSession session;
    int connected;
    int mode;
    int nextMode;
    u32 ip;
    Data *data;
    int seconds;
    int microseconds;
    int timeout;
    int tmp;
    int result;
    int zero;
    u8 error;
    s16 i;
    s16 limit;
    s16 delay;
    session.connection = _0224267C;
    ClearRetry(&session.retry, &mode);
    session.option = 1;
    zero = mode;
    session.attempts = zero;
    ip = zero;
    WifiSetup_MemFill(&session.params, mode, sizeof(Params));
    session.connection.max = request->connectRetries;
    if (session.connection.max == -1) {
        session.connection.max = 10;
    }
    session.retry.max = request->receiveRetries;
    if (session.retry.max == -1) {
        session.retry.max = 10;
    }
    session.connection.delay = request->connectDelay;
    if (session.connection.delay == -1) {
        session.connection.delay = 100;
    }
    session.retry.delay = request->receiveDelay;
    if (session.retry.delay == -1) {
        session.retry.delay = 100;
    }
    tmp = GetTimeout(request);
    timeout = tmp;
    if (tmp == -1) {
        timeout = 2000;
    }
    WifiSetup_InitState(request);
    if ((gWifiSetupState.requestedMask & 1) != 1) {
        WifiSetup_SetError(19);
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    i = 0;
    WifiSetup_SetStatus(0);
    delay = session.connection.delay;
    limit = session.connection.max;
    for (;;) {
        if (gWifiSetupState.connection) {
            WifiSetup_Free(gWifiSetupState.connection);
            gWifiSetupState.connection = 0;
        }
        if (WifiSetup_SearchAccessPoints(&ov13_0224CF9C) == -1) {
            request->error = 15;
            WifiSetup_FreeWorkBuffers();
            return -1;
        }
        tmp = WifiSetup_FindAossAccessPoint(gWifiSetupState.connection);
        if (tmp == 4) {
            request->error = 2;
            WifiSetup_FreeWorkBuffers();
            return -1;
        }
        if (tmp == 0) {
            break;
        }
        if (i >= limit) {
            request->error = 1;
            WifiSetup_FreeWorkBuffers();
            return -1;
        }
        WifiSetup_Sleep(delay);
        i++;
    }
    WifiSetup_SetStatus(1);
    WifiSetup_MemFill(&session.context, 0, sizeof(Context));
    if (WifiSetup_InitAossConnectInfo(&session.context)) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    gWifiSetupState.status = WifiSetup_Alloc(88);
    if (!gWifiSetupState.status) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    WifiSetup_MemFill(gWifiSetupState.status, 0, 88);
    limit = session.connection.max;
    for (i = 0; i < limit; i++) {
        tmp = WifiSetup_ConnectAccessPoint(&session.context, gWifiSetupState.status);
        if (tmp == -1) {
            request->error = 15;
            WifiSetup_FreeWorkBuffers();
            return -1;
        }
        if (tmp == 0) {
            if (tmp != 0 || *gWifiSetupState.status == 1) {
                break;
            }
        }
        WifiSetup_Sleep(delay);
    }
    if (i == session.connection.max) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    if (WifiSetup_StartupSocket(0xc0a80b65, 0xffffff00, 0xc0a80b65)) {
        WifiSetup_SetError(12);
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    WifiSetup_FreeWorkBuffers();
    WifiSetup_InitPacketAddresses(3, &session.params, request->macAddress);
    gWifiSetupSocket = WifiSetup_Socket(2, 2, 0);
    if (gWifiSetupSocket < 0) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    if (WifiSetup_SetSockOpt(gWifiSetupSocket, 65535, 1, &session.option, 4) < 0) {
        WifiSetup_SetError(11);
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    WifiSetup_MemFill(&session.address, 0, sizeof(SocAddress));
    session.address.family = 2;
    session.address.ip = WifiSetup_HostToNet32(0xc0a80b65);
    PutPort(&session.address, WifiSetup_HostToNet16(0x5790));
    if (WifiSetup_Bind(gWifiSetupSocket, &session.address, 8) < 0) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
restart:
    data = gWifiSetupState.data;
    WifiSetup_MemFill(&session.config, 0, sizeof(Config));
    session.config.gateway = 0xc0a80b65;
    session.config.ip = 0xc0a80b01;
    seconds = (timeout) / 1000;
    microseconds = ((timeout) % 1000) * 1000;
    limit = session.connection.max;
    for (;;) {
        if (mode == 1 && ov13_0224CFB8[28] != 1) {
            if (gWifiSetupSocket != -1) {
                WifiSetup_Close(gWifiSetupSocket);
            }
            gWifiSetupSocket = -1;
            if (WifiSetup_CleanupSocket()) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            gWifiSetupState.connection = WifiSetup_Alloc(88);
            if (!gWifiSetupState.connection) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            for (;;) {
                if (gWifiSetupState.connection) {
                    WifiSetup_Free(gWifiSetupState.connection);
                    gWifiSetupState.connection = 0;
                }
                connected = WifiSetup_SearchAccessPoints(&ov13_0224CF9C);
                if (connected == -1) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                tmp = WifiSetup_FindAossAccessPoint(gWifiSetupState.connection);
                if (tmp == 4) {
                    request->error = 2;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                if (tmp == 0) {
                    break;
                }
                if (i >= limit) {
                    request->error = 1;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                WifiSetup_Sleep(delay);
                i++;
            }
            if (connected == -1) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            gWifiSetupState.status = WifiSetup_Alloc(88);
            if (!gWifiSetupState.status) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            WifiSetup_MemFill(gWifiSetupState.status, 0, 88);
            for (i = 0; i < limit; i++) {
                tmp = WifiSetup_ConnectAccessPoint(&session.context, gWifiSetupState.status);
                if (tmp == -1) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                if (tmp == 0) {
                    if (tmp != 0 || *gWifiSetupState.status == 1) {
                        break;
                    }
                }
                WifiSetup_Sleep(delay);
            }
            if (i == limit) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            ip = WifiSetup_NextHostAddress(gWifiSetupState.address, gWifiSetupState.netmask);
            if (WifiSetup_StartupSocket(ip, gWifiSetupState.netmask, ip)) {
                WifiSetup_SetError(12);
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            ov13_0224CFB8[28] = 1;
            WifiSetup_FreeWorkBuffers();
            gWifiSetupSocket = WifiSetup_Socket(2, 2, 0);
            if (gWifiSetupSocket < 0) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            if (WifiSetup_SetSockOpt(gWifiSetupSocket, 65535, 1, &session.option, 4) < 0) {
                WifiSetup_SetError(11);
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
            WifiSetup_MemFill(&session.address, 0, sizeof(SocAddress));
            session.address.family = 2;
            session.address.ip = WifiSetup_HostToNet32(ip);
            PutPort(&session.address, WifiSetup_HostToNet16(0x5790));
            if (WifiSetup_Bind(gWifiSetupSocket, &session.address, 8) < 0) {
                request->error = 15;
                WifiSetup_FreeWorkBuffers();
                return -1;
            }
        }
        if (WifiSetup_SendRequest(mode, &session.config, &session.params, gWifiSetupSocket) == -1) {
            {
                int current = mode;
                WifiSetup_SetError(current + 0x1000);
            }
            request->error = 15;
            WifiSetup_FreeWorkBuffers();
            return -1;
        }
        WifiSetup_MemFill(data, 0, 0x5f8);
        WifiSetup_ClearPollSet(&session.set);
        WifiSetup_AddToPollSet(gWifiSetupSocket, &session.set);
        session.time.seconds = seconds;
        session.time.microseconds = microseconds;
        if (WifiSetup_Poll(gWifiSetupSocket + 1, &session.set, 0, 0, &session.time) <= 0) {
            session.attempts++;
            if (session.attempts > session.retry.max) {
                int current = mode;
                if (current == 0) {
                    WifiSetup_SetError(15);
                } else if (current == 1) {
                    WifiSetup_SetError(16);
                } else {
                    WifiSetup_SetError(17);
                }
                result = -1;
                break;
            }
            WifiSetup_Sleep(session.retry.delay);
            continue;
        }
        session.length = 8;
        tmp = WifiSetup_RecvFrom(gWifiSetupSocket, data->bytes, 1500, 0, &session.remote, &session.length);
        data->socket = gWifiSetupSocket;
        data->length = WifiSetup_NetToHost16((u16)tmp);
        nextMode = WifiSetup_HandlePacket(mode, data, &session.attempts, &session.params, gWifiSetupSocket);
        if (nextMode == 100) {
            result = 0;
            break;
        }
        result = -1;
        if (nextMode == -1) {
            break;
        }
        if (mode != nextMode) {
            if (nextMode == 2) {
                if (gWifiSetupSocket != -1) {
                    WifiSetup_Close(gWifiSetupSocket);
                }
                gWifiSetupSocket = -1;
                if (WifiSetup_CleanupSocket()) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                i = 0;
                WifiSetup_SetStatus(4);
                limit = session.connection.max;
                for (;;) {
                    if (gWifiSetupState.connection) {
                        WifiSetup_Free(gWifiSetupState.connection);
                        gWifiSetupState.connection = 0;
                    }
                    if (WifiSetup_SearchAccessPoints(&ov13_0224CF9C) == -1) {
                        request->error = 15;
                        WifiSetup_FreeWorkBuffers();
                        return -1;
                    }
                    tmp = WifiSetup_FindAossAccessPoint(gWifiSetupState.connection);
                    if (tmp == 4) {
                        request->error = 2;
                        WifiSetup_FreeWorkBuffers();
                        return -1;
                    }
                    if (tmp == 0) {
                        break;
                    }
                    if (i >= limit) {
                        request->error = 1;
                        WifiSetup_FreeWorkBuffers();
                        return -1;
                    }
                    WifiSetup_Sleep(delay);
                    i++;
                }
                gWifiSetupState.status = WifiSetup_Alloc(88);
                if (!gWifiSetupState.status) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                WifiSetup_MemFill(gWifiSetupState.status, 0, 88);
                limit = session.connection.max;
                for (i = 0; i < limit; i++) {
                    tmp = WifiSetup_ConnectAccessPoint(&session.context, gWifiSetupState.status);
                    if (tmp == -1) {
                        request->error = 15;
                        WifiSetup_FreeWorkBuffers();
                        return -1;
                    }
                    if (tmp == 0) {
                        if (tmp != 0 || *gWifiSetupState.status == 1) {
                            break;
                        }
                    }
                    WifiSetup_Sleep(delay);
                }
                if (i == session.connection.max) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                if (WifiSetup_StartupSocket(ip, gWifiSetupState.netmask, ip)) {
                    WifiSetup_SetError(12);
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                WifiSetup_FreeWorkBuffers();
                gWifiSetupSocket = WifiSetup_Socket(2, 2, 0);
                if (gWifiSetupSocket < 0) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                if (WifiSetup_SetSockOpt(gWifiSetupSocket, 65535, 1, &session.option, 4) < 0) {
                    WifiSetup_SetError(11);
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
                WifiSetup_MemFill(&session.address, 0, sizeof(SocAddress));
                session.address.family = 2;
                session.address.ip = WifiSetup_HostToNet32(ip);
                PutPort(&session.address, WifiSetup_HostToNet16(0x5790));
                if (WifiSetup_Bind(gWifiSetupSocket, &session.address, 8) < 0) {
                    request->error = 15;
                    WifiSetup_FreeWorkBuffers();
                    return -1;
                }
            }
            mode = nextMode;
            goto restart;
        }
        mode = nextMode;
        if (session.attempts > session.retry.max) {
            int current = nextMode;
            if (current == 0) {
                WifiSetup_SetError(15);
            } else if (current == 1) {
                WifiSetup_SetError(16);
            } else {
                WifiSetup_SetError(17);
            }
            result = -1;
            break;
        }
        WifiSetup_Sleep(session.retry.delay);
        goto restart;
    }
    if (gWifiSetupSocket != -1) {
        WifiSetup_Close(gWifiSetupSocket);
    }
    gWifiSetupSocket = -1;
    if (WifiSetup_CleanupSocket()) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    if (result) {
        switch (WifiSetup_GetError()) {
        case 15:
            error = 3;
            break;
        case 16:
            error = 4;
            break;
        case 17:
            error = 5;
            break;
        case 20:
            error = 7;
            break;
        case 21:
            error = 8;
            break;
        default:
            error = 15;
            break;
        }
        request->error = error;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    if (WifiSetup_StoreAossResult(request)) {
        request->error = 6;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    return 0;
}
#pragma opt_loop_invariants reset
