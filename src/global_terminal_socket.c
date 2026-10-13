#pragma force_active on
#include "global.h"

#include "global_terminal_net.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Socket client of the Global Terminal server. GlobalTerminalNet (Thumb) calls the GlobalTerminalServer_ API;
// each request runs on its own thread: fetch the login token, open a socket, connect, send the XOR-encoded
// request, receive the response into the caller's buffer.

#define GTS_SOCKET_TICKS_PER_SECOND 523656

enum GlobalTerminalSocketState {
    GTS_SOCKET_STATE_NONE,
    GTS_SOCKET_STATE_READY,
    GTS_SOCKET_STATE_FETCH_TOKEN,
    GTS_SOCKET_STATE_OPEN,
    GTS_SOCKET_STATE_CONNECT,
    GTS_SOCKET_STATE_SEND,
    GTS_SOCKET_STATE_RECEIVE,
    GTS_SOCKET_STATE_CANCELLED,
    GTS_SOCKET_STATE_DONE,
    GTS_SOCKET_STATE_ERROR,
};

enum GlobalTerminalSocketError {
    GTS_SOCKET_ERROR_NONE,
    GTS_SOCKET_ERROR_BUSY,
    GTS_SOCKET_ERROR_UNKNOWN_COMMAND,
    GTS_SOCKET_ERROR_SOCKET,
    GTS_SOCKET_ERROR_NONBLOCKING,
    GTS_SOCKET_ERROR_RESOLVE,
    GTS_SOCKET_ERROR_THREAD,
    GTS_SOCKET_ERROR_CONNECT,
    GTS_SOCKET_ERROR_UNK_8,
    GTS_SOCKET_ERROR_RECEIVE,
    GTS_SOCKET_ERROR_CONNECT_TIMEOUT,
    GTS_SOCKET_ERROR_SEND,
    GTS_SOCKET_ERROR_RECEIVE_TIMEOUT,
    GTS_SOCKET_ERROR_CANCELLED,
    GTS_SOCKET_ERROR_TOKEN,
};

typedef struct GlobalTerminalSocket {
    GlobalTerminalNetLogin login; // 0x000, the auth token is stored from 0x06
    BOOL hasToken;                // 0x138
    int state;                    // 0x13C
    char host[0xC0];              // 0x140, also labelled sGlobalTerminalSocketHost
    u8 unk_200[0x40];             // 0x200, the block labelled sGlobalTerminalSocketSettings starts here
    u16 port;                     // 0x240
    u16 unk_242;                  // 0x242
    int connectTimeout;           // 0x244, seconds, 0 = none
    int sendTimeout;              // 0x248
    int receiveTimeout;           // 0x24C
    int error;                    // 0x250
    int requestSize;              // 0x254
    u8 *request;                  // 0x258, size + command + login + payload, XOR-encoded after the size
    int responseSize;             // 0x25C
    u8 *response;                 // 0x260, caller's buffer: size + data
    u32 address;                  // 0x264, IPv4 address of host
    int cancel;                   // 0x268
    int socket;                   // 0x26C
    int threadPriority;           // 0x270
} GlobalTerminalSocket;           // size 0x274

// sGlobalTerminalSocket + 0x200, addressed through its own label in some functions
typedef struct GlobalTerminalSocketSettings {
    u8 unk_00[0x40];
    u16 port; // 0x40
} GlobalTerminalSocketSettings;

typedef struct GlobalTerminalServerCommandIds {
    int ids[12]; // every command id, -1 terminated
} GlobalTerminalServerCommandIds;

typedef struct GlobalTerminalSocketToken {
    u8 bytes[301];
} GlobalTerminalSocketToken;

// filled by ov00_021ECC6C
typedef struct GlobalTerminalSocketTokenRequest {
    u8 unk_00[0x45];
    GlobalTerminalSocketToken token; // 0x45
    u8 unk_172[2];
} GlobalTerminalSocketTokenRequest;

typedef struct GlobalTerminalSocketAddress {
    u8 length;
    u8 family;
    u16 port;
    u32 address;
} GlobalTerminalSocketAddress;

typedef struct GlobalTerminalSocketHostEntry {
    int unk_00;
    int unk_04;
    void *unk_08;
    u32 **addresses;
} GlobalTerminalSocketHostEntry;

typedef struct GlobalTerminalSocketInAddr {
    u32 address;
} GlobalTerminalSocketInAddr;

typedef struct GlobalTerminalServerHeader {
    u8 bytes[4];
} GlobalTerminalServerHeader;

typedef struct GlobalTerminalServerDressUpUpload {
    u32 data[56];
} GlobalTerminalServerDressUpUpload; // 0xE0

typedef struct GlobalTerminalServerIdRequest {
    u16 unk_00;
    u16 unk_02;
    int idLow;
    int idHigh;
} GlobalTerminalServerIdRequest; // 0x0C

typedef struct GlobalTerminalServerBoxData {
    u32 data[135];
} GlobalTerminalServerBoxData; // 0x21C

typedef struct GlobalTerminalServerBoxUpload {
    int tray;
    GlobalTerminalServerBoxData box;
} GlobalTerminalServerBoxUpload; // 0x220

typedef struct GlobalTerminalServerRankingsRequest {
    u32 data[9];
} GlobalTerminalServerRankingsRequest; // 0x24

typedef struct GlobalTerminalServerVideoUpload {
    u32 data[1875];
} GlobalTerminalServerVideoUpload; // 0x1D4C

typedef struct GlobalTerminalServerVideoSearchParams {
    u32 data[4];
} GlobalTerminalServerVideoSearchParams;

typedef struct GlobalTerminalServerVideoSearch {
    u16 kind;
    u16 unk_02;
    GlobalTerminalServerVideoSearchParams params;
    int unk_14[2];
} GlobalTerminalServerVideoSearch; // 0x1C

typedef struct GlobalTerminalServerVideoList {
    u16 kind;
    u16 unk_02;
    int data[6];
} GlobalTerminalServerVideoList; // 0x1C

extern GlobalTerminalSocket sGlobalTerminalSocket;
extern char sGlobalTerminalSocketHost[];                           // sGlobalTerminalSocket.host
extern GlobalTerminalSocketSettings sGlobalTerminalSocketSettings; // sGlobalTerminalSocket.unk_200
extern OSThread sGlobalTerminalSocketThread;
extern OSMutex sGlobalTerminalSocketMutex;
extern u8 sGlobalTerminalSocketThreadStackEnd[];
extern const GlobalTerminalServerCommandIds sGlobalTerminalServerCommandIds;
extern u8 _0222AA58[];
extern char sGlobalTerminalServerHostName[];
extern u8 ov39_0222AA78[];
extern u8 sGlobalTerminalSocketCipher[];

// no public header
void *ov00_021EC2A8(int category, int size);
void ov00_021EC2EC(int category, void *ptr, int size);
int gsSocketInetAddr(char *address);
int ov00_021ECC6C(const char *arg, void *request);
int ov00_021ECCE0(void);
int ov00_021EC11C(int *errorCode, int *errorType);
GlobalTerminalSocketHostEntry *SOC_GetHostByName(char *name);
char *SOC_InetNtoA(GlobalTerminalSocketInAddr address);
int SOC_Socket(int domain, int type, int protocol);
int SOC_Fcntl(int socket, int command, int mode);
int SOC_Connect(int socket, GlobalTerminalSocketAddress *address);
int SOC_Send(int socket, void *buffer, int length, int flags);
int SOC_Recv(int socket, void *buffer, int length, int flags);
int SOC_Shutdown(int socket, int how);
int SOC_Close(int socket);

void GlobalTerminalSocket_Reset(void);
int GlobalTerminalSocket_Start(char *host, int port, GlobalTerminalNetLogin *login);
void GlobalTerminalSocket_ClearRequest(void);
void GlobalTerminalSocket_RunRequest(void);
int GlobalTerminalSocket_SendRequest(int command, void *payload, void *response, int payloadSize);
int GlobalTerminalSocket_GetStateUnlocked(void);
void *GlobalTerminalSocket_GetResponse(void);
int GlobalTerminalSocket_GetResponseSize(void);
int GlobalTerminalSocket_GetError(void);
void *GlobalTerminalSocket_Alloc(int size);
void GlobalTerminalSocket_Free(void *ptr);
void GlobalTerminalSocket_Lock(void);
void GlobalTerminalSocket_Unlock(void);
int GlobalTerminalSocket_InitMutex(void);
int ov39_0222993C(void *settings);
int GlobalTerminalSocket_InitTick(void);
void ov39_02229970(void);
void GlobalTerminalSocket_ThreadMain(void);
int GlobalTerminalSocket_StartThread(void);
int GlobalTerminalSocket_IsIPv4Address(signed char *str);
int GlobalTerminalSocket_ResolveHost(char *host, char *out);
int GlobalTerminalSocket_Open(void);
void GlobalTerminalSocket_Close(void);
int GlobalTerminalSocket_Connect(void);
int GlobalTerminalSocket_Send(void);
int GlobalTerminalSocket_Receive(void);
void GlobalTerminalSocket_Sleep(u32 msec);
int GlobalTerminalSocket_FetchToken(void);
int GlobalTerminalServer_Init(GlobalTerminalNetLogin *login);
void GlobalTerminalServer_Stop(void);
int GlobalTerminalServer_GetState(void);
int GlobalTerminalServer_GetError(void);
int GlobalTerminalServer_UploadDressUp(GlobalTerminalServerDressUpUpload *upload, void *response);
int GlobalTerminalServer_DownloadDressUp(int id, void *response);
int GlobalTerminalServer_UploadBox(int tray, GlobalTerminalServerBoxData *box, void *response);
int GlobalTerminalServer_DownloadBox(int id, void *response);
int GlobalTerminalServer_GetResponseSize(void);
void *GlobalTerminalServer_GetResponse(void);
int GlobalTerminalServer_GetResponseBufferSize(int command);
int ov39_0222A2CC(void *response);
int GlobalTerminalServer_GetRankings(GlobalTerminalServerRankingsRequest *request, void *response);
int GlobalTerminalServer_UploadVideo(GlobalTerminalServerVideoUpload *video, void *response);
int GlobalTerminalServer_SearchVideos(GlobalTerminalServerVideoSearchParams *params, void *response);
int ov39_0222A3DC(int *arg, void *response);
int ov39_0222A434(int *arg, void *response);
int GlobalTerminalServer_DownloadVideo(int idLow, int idHigh, int arg2, void *response);
int ov39_0222A4C0(int idLow, int idHigh, void *response);
void GlobalTerminalSocket_Encode(const u8 *src, int size, u8 *dst);
void GlobalTerminalSocket_Decode(const u8 *src, int size, u8 *dst);
int GlobalTerminalSocket_GetRequestSize(int command);
int GlobalTerminalSocket_GetResponseBufferSize(int command);
void ov39_0222A8AC(void *request, void *arg);

void GlobalTerminalSocket_Reset(void) {
    GlobalTerminalSocket_ClearRequest();
    memset(&sGlobalTerminalSocket.login, 0, sizeof(sGlobalTerminalSocket.login));
    sGlobalTerminalSocket.hasToken = FALSE;
    sGlobalTerminalSocket.host[0] = '\0';
    sGlobalTerminalSocketSettings.port = 0;
    ov39_0222993C(&sGlobalTerminalSocketSettings);
    ov39_02229970();
}

int GlobalTerminalSocket_Start(char *host, int port, GlobalTerminalNetLogin *login) {
    if (login) {
        sGlobalTerminalSocket.login = *login;
        sGlobalTerminalSocket.hasToken = FALSE;
    }
    if (!GlobalTerminalSocket_InitTick()) {
        return 0;
    }
    GlobalTerminalSocket_InitMutex();
    strcpy(sGlobalTerminalSocketHost, host);
    // volatile keeps these stores in this order
    *(volatile u16 *)&sGlobalTerminalSocketSettings.port = port;
    *(u8 *volatile *)&sGlobalTerminalSocket.request = NULL;
    *(volatile int *)&sGlobalTerminalSocket.socket = -1;
    GlobalTerminalSocket_ClearRequest();
    return 1;
}

void GlobalTerminalSocket_ClearRequest(void) {
    GlobalTerminalSocket_Close();
    if (sGlobalTerminalSocket.request) {
        GlobalTerminalSocket_Free(sGlobalTerminalSocket.request);
        sGlobalTerminalSocket.request = NULL;
    }
    sGlobalTerminalSocket.connectTimeout = 15;
    sGlobalTerminalSocket.sendTimeout = 30;
    sGlobalTerminalSocket.receiveTimeout = 30;
    sGlobalTerminalSocket.requestSize = 0;
    sGlobalTerminalSocket.responseSize = 0;
    sGlobalTerminalSocket.response = NULL;
    sGlobalTerminalSocket.address = 0;
    sGlobalTerminalSocket.cancel = 0;
    sGlobalTerminalSocket.state = GTS_SOCKET_STATE_READY;
    sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_NONE;
}

static inline int GlobalTerminalSocket_FetchTokenNextState(BOOL *done) {
    int next;
    if (!GlobalTerminalSocket_FetchToken()) {
        next = sGlobalTerminalSocket.error == GTS_SOCKET_ERROR_CANCELLED ? GTS_SOCKET_STATE_CANCELLED : GTS_SOCKET_STATE_ERROR;
        *done = TRUE;
    } else {
        next = GTS_SOCKET_STATE_OPEN;
    }
    return next;
}

void GlobalTerminalSocket_RunRequest(void) {
    BOOL done;
    int next;
    for (;;) {
        done = FALSE;
        switch (sGlobalTerminalSocket.state) {
        case GTS_SOCKET_STATE_NONE:
        case GTS_SOCKET_STATE_READY:
        case GTS_SOCKET_STATE_CANCELLED:
        case GTS_SOCKET_STATE_DONE:
        case GTS_SOCKET_STATE_ERROR:
            done = TRUE;
            break;
        case GTS_SOCKET_STATE_FETCH_TOKEN:
            next = GlobalTerminalSocket_FetchTokenNextState(&done);
            GlobalTerminalSocket_Lock();
            sGlobalTerminalSocket.state = next;
            break;
        case GTS_SOCKET_STATE_OPEN:
            if (!GlobalTerminalSocket_Open()) {
                done = TRUE;
                next = sGlobalTerminalSocket.error == GTS_SOCKET_ERROR_CANCELLED ? GTS_SOCKET_STATE_CANCELLED : GTS_SOCKET_STATE_ERROR;
            } else {
                next = GTS_SOCKET_STATE_CONNECT;
            }
            GlobalTerminalSocket_Lock();
            sGlobalTerminalSocket.state = next;
            break;
        case GTS_SOCKET_STATE_CONNECT:
            if (!GlobalTerminalSocket_Connect()) {
                done = TRUE;
                next = sGlobalTerminalSocket.error == GTS_SOCKET_ERROR_CANCELLED ? GTS_SOCKET_STATE_CANCELLED : GTS_SOCKET_STATE_ERROR;
            } else {
                next = GTS_SOCKET_STATE_SEND;
            }
            GlobalTerminalSocket_Lock();
            sGlobalTerminalSocket.state = next;
            break;
        case GTS_SOCKET_STATE_SEND:
            if (!GlobalTerminalSocket_Send()) {
                done = TRUE;
                next = sGlobalTerminalSocket.error == GTS_SOCKET_ERROR_CANCELLED ? GTS_SOCKET_STATE_CANCELLED : GTS_SOCKET_STATE_ERROR;
            } else {
                next = GTS_SOCKET_STATE_RECEIVE;
            }
            if (sGlobalTerminalSocket.request) {
                GlobalTerminalSocket_Free(sGlobalTerminalSocket.request);
                sGlobalTerminalSocket.request = NULL;
            }
            GlobalTerminalSocket_Lock();
            sGlobalTerminalSocket.state = next;
            break;
        case GTS_SOCKET_STATE_RECEIVE:
            switch (GlobalTerminalSocket_Receive()) {
            case 0:
                next = sGlobalTerminalSocket.error == GTS_SOCKET_ERROR_CANCELLED ? GTS_SOCKET_STATE_CANCELLED : GTS_SOCKET_STATE_ERROR;
                break;
            default:
                next = GTS_SOCKET_STATE_DONE;
                break;
            }
            done = TRUE;
            GlobalTerminalSocket_Lock();
            sGlobalTerminalSocket.state = next;
            break;
        }
        if (done) {
            break;
        }
        if (sGlobalTerminalSocket.cancel == 1) {
            sGlobalTerminalSocket.state = GTS_SOCKET_STATE_CANCELLED;
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CANCELLED;
            break;
        }
        GlobalTerminalSocket_Unlock();
        GlobalTerminalSocket_Sleep(1);
    }
    GlobalTerminalSocket_Unlock();
}

int GlobalTerminalSocket_SendRequest(int command, void *payload, void *response, int payloadSize) {
    u32 size;
    GlobalTerminalSocket_Lock();
    switch (sGlobalTerminalSocket.state) {
    case GTS_SOCKET_STATE_NONE:
    case GTS_SOCKET_STATE_FETCH_TOKEN:
    case GTS_SOCKET_STATE_OPEN:
    case GTS_SOCKET_STATE_CONNECT:
    case GTS_SOCKET_STATE_SEND:
    case GTS_SOCKET_STATE_RECEIVE:
        GlobalTerminalSocket_Unlock();
        sGlobalTerminalSocket.state = GTS_SOCKET_STATE_ERROR;
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_BUSY;
        return 0;
    case GTS_SOCKET_STATE_CANCELLED:
    case GTS_SOCKET_STATE_DONE:
    case GTS_SOCKET_STATE_ERROR:
        GlobalTerminalSocket_ClearRequest();
        break;
    }
    GlobalTerminalSocket_Unlock();
    sGlobalTerminalSocket.responseSize = GlobalTerminalSocket_GetResponseBufferSize(command);
    if (sGlobalTerminalSocket.responseSize == -1) {
        sGlobalTerminalSocket.state = GTS_SOCKET_STATE_ERROR;
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_UNKNOWN_COMMAND;
        return 0;
    }
    sGlobalTerminalSocket.response = response;
    if (payloadSize != -1) {
        size = payloadSize + 0x140;
    } else {
        size = GlobalTerminalSocket_GetRequestSize(command);
        if (size == (u32)-1) {
            sGlobalTerminalSocket.state = GTS_SOCKET_STATE_ERROR;
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_UNKNOWN_COMMAND;
            return 0;
        }
    }
    sGlobalTerminalSocket.request = GlobalTerminalSocket_Alloc(size);
    if (!sGlobalTerminalSocket.request) {
        return 0;
    }
    sGlobalTerminalSocket.requestSize = size;
    {
        memcpy(sGlobalTerminalSocket.request, &size, 4);
    }
    {
        u8 *request = sGlobalTerminalSocket.request;
        *(u16 *)(request + 4) = command;
        *(GlobalTerminalNetLogin *)(request + 8) = sGlobalTerminalSocket.login;
        *(u16 *)(request + 6) = 0;
        memcpy(request + 0x140, payload, size - 0x140);
    }
    GlobalTerminalSocket_Encode(sGlobalTerminalSocket.request + 4, sGlobalTerminalSocket.requestSize - 4, sGlobalTerminalSocket.request + 4);
    sGlobalTerminalSocket.state = GTS_SOCKET_STATE_FETCH_TOKEN;
    GlobalTerminalSocket_Lock();
    if (!GlobalTerminalSocket_StartThread()) {
        sGlobalTerminalSocket.state = GTS_SOCKET_STATE_ERROR;
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_THREAD;
        GlobalTerminalSocket_Unlock();
        return 0;
    }
    GlobalTerminalSocket_Unlock();
    return 1;
}

int GlobalTerminalSocket_GetStateUnlocked(void) {
    return sGlobalTerminalSocket.state;
}

void *GlobalTerminalSocket_GetResponse(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_DONE) {
        GlobalTerminalSocket_Unlock();
        return NULL;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.response + 4;
}

int GlobalTerminalSocket_GetResponseSize(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_DONE) {
        GlobalTerminalSocket_Unlock();
        return -1;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.responseSize;
}

int GlobalTerminalSocket_GetError(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_ERROR) {
        GlobalTerminalSocket_Unlock();
        return GTS_SOCKET_ERROR_NONE;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.error;
}

void *GlobalTerminalSocket_Alloc(int size) {
    return ov00_021EC2A8(0, size);
}

void GlobalTerminalSocket_Free(void *ptr) {
    if (ptr) {
        ov00_021EC2EC(0, ptr, 0);
    }
}

void GlobalTerminalSocket_Lock(void) {
    OS_LockMutex(&sGlobalTerminalSocketMutex);
}

void GlobalTerminalSocket_Unlock(void) {
    OS_UnlockMutex(&sGlobalTerminalSocketMutex);
}

int GlobalTerminalSocket_InitMutex(void) {
    OS_InitMutex(&sGlobalTerminalSocketMutex);
    return 1;
}

int ov39_0222993C(void *settings) {
    return 1;
}

int GlobalTerminalSocket_InitTick(void) {
    if (!OS_IsTickAvailable()) {
        OS_InitTick();
    }
    sGlobalTerminalSocket.threadPriority = 16;
    return 1;
}

void ov39_02229970(void) {
}

void GlobalTerminalSocket_ThreadMain(void) {
    GlobalTerminalSocket_RunRequest();
}

int GlobalTerminalSocket_StartThread(void) {
    OS_CreateThread(&sGlobalTerminalSocketThread, (void (*)(void *))GlobalTerminalSocket_ThreadMain, NULL, sGlobalTerminalSocketThreadStackEnd, 0x800, sGlobalTerminalSocket.threadPriority);
    OS_WakeupThreadDirect(&sGlobalTerminalSocketThread);
    return 1;
}

int GlobalTerminalSocket_IsIPv4Address(signed char *str) {
    int digits = 0, dots = 0;
    int c;
    while ((c = *str) != 0) {
        if (c >= '0' && c <= '9') {
            if (++digits >= 4) {
                return 0;
            }
        } else if (c == '.') {
            if (!digits) {
                return 0;
            }
            digits = 0;
            if (++dots >= 4) {
                return 0;
            }
        } else {
            return 0;
        }
        str++;
    }
    if (dots == 3 && digits) {
        return 1;
    }
    return 0;
}

int GlobalTerminalSocket_ResolveHost(char *host, char *out) {
    if (!GlobalTerminalSocket_IsIPv4Address((signed char *)host)) {
        GlobalTerminalSocketHostEntry *entry = SOC_GetHostByName(host);
        if (entry) {
            GlobalTerminalSocketInAddr address = { **entry->addresses };
            strcpy(out, SOC_InetNtoA(address));
        } else {
            return 0;
        }
    } else {
        strcpy(out, host);
    }
    return 1;
}

int GlobalTerminalSocket_Open(void) {
    char address[16];
    sGlobalTerminalSocket.socket = SOC_Socket(2, 1, 0);
    if (sGlobalTerminalSocket.socket == -1) {
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_SOCKET;
        return 0;
    }
    int mode = SOC_Fcntl(sGlobalTerminalSocket.socket, 3, 0);
    if (SOC_Fcntl(sGlobalTerminalSocket.socket, 4, mode | 4) < 0) {
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_NONBLOCKING;
        return 0;
    }
    if (!GlobalTerminalSocket_ResolveHost(sGlobalTerminalSocketHost, address)) {
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_RESOLVE;
        return 0;
    }
    sGlobalTerminalSocket.address = gsSocketInetAddr(address);
    return 1;
}

void GlobalTerminalSocket_Close(void) {
    if (sGlobalTerminalSocket.socket == -1) {
        return;
    }
    SOC_Shutdown(sGlobalTerminalSocket.socket, 2);
    SOC_Close(sGlobalTerminalSocket.socket);
    sGlobalTerminalSocket.socket = -1;
}

static inline u16 GlobalTerminalSocket_SwapBytes(u16 value) {
    int v = value;
    return ((v >> 8) & 0xFF) | ((v << 8) & 0xFF00);
}

int GlobalTerminalSocket_Connect(void) {
    GlobalTerminalSocketAddress address;
    u32 start;
    int result;
    address.length = 8;
    address.family = 2;
    address.port = GlobalTerminalSocket_SwapBytes(sGlobalTerminalSocketSettings.port);
    address.address = sGlobalTerminalSocket.address;
    start = OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND;
    for (;;) {
        result = SOC_Connect(sGlobalTerminalSocket.socket, &address);
        if (result >= 0) {
            break;
        }
        if (result == -26) {
            if (sGlobalTerminalSocket.connectTimeout != 0) {
                int elapsed = OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND - start;
                if (elapsed >= sGlobalTerminalSocket.connectTimeout) {
                    sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CONNECT_TIMEOUT;
                    return 0;
                }
            }
        } else if (result == -30) {
            break;
        } else {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CONNECT;
            return 0;
        }
        GlobalTerminalSocket_Lock();
        if (sGlobalTerminalSocket.cancel == 1) {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CANCELLED;
            GlobalTerminalSocket_Unlock();
            return 0;
        }
        GlobalTerminalSocket_Unlock();
        GlobalTerminalSocket_Sleep(1);
    }
    return 1;
}

int GlobalTerminalSocket_Send(void) {
    int sent = 0;
    int start = OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND;
    while (1) {
        int size = sGlobalTerminalSocket.requestSize - sent;
        if (size > sGlobalTerminalSocket.requestSize) {
            size = sGlobalTerminalSocket.requestSize;
        }
        int result = SOC_Send(sGlobalTerminalSocket.socket, sGlobalTerminalSocket.request + sent, size, 0);
        if (result != -6) {
            if (result < 0) {
                sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_SEND;
                return 0;
            }
            sent += result;
            if (sent == sGlobalTerminalSocket.requestSize) {
                break;
            }
        }
        if (sGlobalTerminalSocket.sendTimeout) {
            if ((int)(OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND - start) >= sGlobalTerminalSocket.sendTimeout) {
                sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_SEND;
                return 0;
            }
        }
        GlobalTerminalSocket_Lock();
        if (sGlobalTerminalSocket.cancel == 1) {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CANCELLED;
            GlobalTerminalSocket_Unlock();
            return 0;
        }
        GlobalTerminalSocket_Unlock();
        GlobalTerminalSocket_Sleep(1);
    }
    return 1;
}

int GlobalTerminalSocket_Receive(void) {
    u64 start;
    u32 received = 0;

    union {
        u32 size;
        GlobalTerminalServerHeader bytes;
    } header;

    header.size = -1;
    start = OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND;
    for (;;) {
        int result = SOC_Recv(sGlobalTerminalSocket.socket, sGlobalTerminalSocket.response + received, sGlobalTerminalSocket.responseSize - received, 0);
        if (result != -6) {
            if (result <= 0) {
                sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_RECEIVE;
                return 0;
            }
            received += result;
            if (received >= 4 && header.size == -1) {
                memcpy(&header, sGlobalTerminalSocket.response, 4);
            }
            if (received == header.size && header.size != -1) {
                GlobalTerminalSocket_Decode(sGlobalTerminalSocket.response + 4, received - 4, sGlobalTerminalSocket.response + 4);
                sGlobalTerminalSocket.responseSize = header.size - 4;
                break;
            }
        }
        if (sGlobalTerminalSocket.receiveTimeout != 0) {
            s32 elapsed = OS_GetTick() / GTS_SOCKET_TICKS_PER_SECOND - start;
            if (elapsed >= sGlobalTerminalSocket.receiveTimeout) {
                sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_RECEIVE_TIMEOUT;
                return 0;
            }
        }
        GlobalTerminalSocket_Lock();
        if (sGlobalTerminalSocket.cancel == 1) {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CANCELLED;
            GlobalTerminalSocket_Unlock();
            return 0;
        }
        GlobalTerminalSocket_Unlock();
        GlobalTerminalSocket_Sleep(1);
    }
    return 1;
}

void GlobalTerminalSocket_Sleep(u32 msec) {
    OS_Sleep(msec);
}

#pragma push
#pragma force_active off
// The struct copy keeps its destination pointer in its own register, so the destination is a local that copy
// propagation must not fold back into the assignment.
#pragma opt_propagation off

int GlobalTerminalSocket_FetchToken(void) {
    int errorType, errorCode;
    GlobalTerminalSocketTokenRequest request;
    if (sGlobalTerminalSocket.hasToken == TRUE) {
        return 1;
    }
    if (!ov00_021ECC6C((const char *)_0222AA58, &request)) {
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_TOKEN;
        return 0;
    }
    for (;;) {
        int status = ov00_021ECCE0();
        if (status == 3) {
            GlobalTerminalSocketToken *token = (GlobalTerminalSocketToken *)sGlobalTerminalSocket.login.unk_06;
            *token = request.token;
            sGlobalTerminalSocket.hasToken = TRUE;
            return 1;
        }
        if (status == 4) {
            ov00_021EC11C(&errorCode, &errorType);
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_TOKEN;
            return 0;
        }
        if (status == 5) {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_TOKEN;
            return 0;
        }
        GlobalTerminalSocket_Lock();
        if (sGlobalTerminalSocket.cancel == 1) {
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_CANCELLED;
            GlobalTerminalSocket_Unlock();
            return 0;
        }
        GlobalTerminalSocket_Unlock();
        GlobalTerminalSocket_Sleep(1);
    }
}

#pragma pop

int GlobalTerminalServer_Init(GlobalTerminalNetLogin *login) {
    return GlobalTerminalSocket_Start(sGlobalTerminalServerHostName, 12400, login);
}

void GlobalTerminalServer_Stop(void) {
    GlobalTerminalSocket_Reset();
}

int GlobalTerminalServer_GetState(void) {
    int state;
    GlobalTerminalSocket_Lock();
    state = GlobalTerminalSocket_GetStateUnlocked();
    GlobalTerminalSocket_Unlock();
    return state;
}

int GlobalTerminalServer_GetError(void) {
    return GlobalTerminalSocket_GetError();
}

int GlobalTerminalServer_UploadDressUp(GlobalTerminalServerDressUpUpload *upload, void *response) {
    GlobalTerminalServerDressUpUpload payload = *upload;
    ov39_0222A8AC(upload, ov39_0222AA78);
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_DRESS_UP_UPLOAD, &payload, response, -1);
}

int GlobalTerminalServer_DownloadDressUp(int id, void *response) {
    GlobalTerminalServerIdRequest payload;
    payload.unk_00 = 0;
    payload.unk_02 = 0;
    payload.idLow = id;
    payload.idHigh = id >> 31;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_DRESS_UP_DOWNLOAD, &payload, response, -1);
}

int GlobalTerminalServer_UploadBox(int tray, GlobalTerminalServerBoxData *box, void *response) {
    GlobalTerminalServerBoxUpload payload;
    payload.tray = tray;
    payload.box = *box;
    ov39_0222A8AC(box, ov39_0222AA78);
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_BOX_UPLOAD, &payload, response, -1);
}

int GlobalTerminalServer_DownloadBox(int id, void *response) {
    GlobalTerminalServerIdRequest payload;
    payload.unk_00 = 0;
    payload.unk_02 = 0;
    payload.idLow = id;
    payload.idHigh = id >> 31;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_BOX_DOWNLOAD, &payload, response, -1);
}

int GlobalTerminalServer_GetResponseSize(void) {
    return GlobalTerminalSocket_GetResponseSize();
}

void *GlobalTerminalServer_GetResponse(void) {
    return GlobalTerminalSocket_GetResponse();
}

int GlobalTerminalServer_GetResponseBufferSize(int command) {
    return GlobalTerminalSocket_GetResponseBufferSize(command);
}

int ov39_0222A2CC(void *response) {
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_22000, NULL, response, -1);
}

int GlobalTerminalServer_GetRankings(GlobalTerminalServerRankingsRequest *request, void *response) {
    GlobalTerminalServerRankingsRequest payload = *request;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_RANKINGS, &payload, response, -1);
}

int GlobalTerminalServer_UploadVideo(GlobalTerminalServerVideoUpload *video, void *response) {
    GlobalTerminalServerVideoUpload payload = *video;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_UPLOAD, &payload, response, -1);
}

int GlobalTerminalServer_SearchVideos(GlobalTerminalServerVideoSearchParams *params, void *response) {
    GlobalTerminalServerVideoSearch payload;
    payload.kind = 0;
    payload.params = *params;
    payload.kind = 0;
    payload.unk_02 = 0;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_SEARCH, &payload, response, -1);
}

int ov39_0222A3DC(int *arg, void *response) {
    GlobalTerminalServerVideoList payload = { 0 };
    payload.kind = 1;
    payload.data[2] = *arg;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_SEARCH, &payload, response, -1);
}

int ov39_0222A434(int *arg, void *response) {
    GlobalTerminalServerVideoList payload = { 0 };
    payload.kind = 2;
    payload.data[2] = *arg;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_SEARCH, &payload, response, -1);
}

int GlobalTerminalServer_DownloadVideo(int idLow, int idHigh, int arg2, void *response) {
    int payload[3];
    payload[0] = idLow;
    payload[1] = idHigh;
    payload[2] = arg2;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_DOWNLOAD, payload, response, -1);
}

int ov39_0222A4C0(int idLow, int idHigh, void *response) {
    int payload[2];
    payload[0] = idLow;
    payload[1] = idHigh;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_23003, payload, response, -1);
}

void GlobalTerminalSocket_Encode(const u8 *src, int size, u8 *dst) {
    int i;
    for (i = 0; i < size; i++) {
        int key;
        if (i != 0) {
            int index = src[0];
            index += i + size;
            key = sGlobalTerminalSocketCipher[index & 0xFF];
        } else {
            key = 0;
        }
        *dst++ = key ^ src[i];
    }
}

static inline u32 GlobalTerminalSocket_Xor(u32 byte, u32 key) {
    return byte ^ key;
}

void GlobalTerminalSocket_Decode(const u8 *src, int size, u8 *dst) {
    int i;
    for (i = 0; i < size; i++) {
        int key;
        if (i != 0) {
            int index = src[0];
            index += i + size;
            key = sGlobalTerminalSocketCipher[index & 0xFF];
        } else {
            key = 0;
        }
        dst[i] = GlobalTerminalSocket_Xor(dst[i], key);
    }
}

int GlobalTerminalSocket_GetRequestSize(int command) {
    int size;
    switch (command) {
    case 0:
        size = 0x400;
        break;
    case GTS_REQUEST_DRESS_UP_UPLOAD:
        size = sizeof(GlobalTerminalServerDressUpUpload);
        break;
    case GTS_REQUEST_DRESS_UP_DOWNLOAD:
        size = sizeof(GlobalTerminalServerIdRequest);
        break;
    case GTS_REQUEST_BOX_UPLOAD:
        size = sizeof(GlobalTerminalServerBoxUpload);
        break;
    case GTS_REQUEST_BOX_DOWNLOAD:
        size = sizeof(GlobalTerminalServerIdRequest);
        break;
    case GTS_REQUEST_22000:
        size = 0;
        break;
    case GTS_REQUEST_RANKINGS:
        size = sizeof(GlobalTerminalServerRankingsRequest);
        break;
    case GTS_REQUEST_VIDEO_UPLOAD:
        size = sizeof(GlobalTerminalServerVideoUpload);
        break;
    case GTS_REQUEST_VIDEO_SEARCH:
        size = sizeof(GlobalTerminalServerVideoSearch);
        break;
    case GTS_REQUEST_VIDEO_DOWNLOAD:
        size = 12;
        break;
    case GTS_REQUEST_23003:
        size = 8;
        break;
    default:
        return -1;
    }
    // + size, command and login block
    return size + 0x140;
}

int GlobalTerminalSocket_GetResponseBufferSize(int command) {
    GlobalTerminalServerCommandIds commands = sGlobalTerminalServerCommandIds;
    int size;
    switch (command) {
    case -1: {
        // the largest of all
        int i = 0;
        size = -1;
        while (1) {
            int v = commands.ids[i];
            if (v == -1) {
                break;
            }
            v = GlobalTerminalSocket_GetResponseBufferSize(v);
            if (v > size) {
                size = v;
            }
            i++;
        }
        if (size == -1) {
            return -1;
        }
        break;
    }
    case 0:
        size = 0x400;
        break;
    case GTS_REQUEST_DRESS_UP_UPLOAD:
        size = 8;
        break;
    case GTS_REQUEST_DRESS_UP_DOWNLOAD:
        size = 0x93C;
        break;
    case GTS_REQUEST_BOX_UPLOAD:
        size = 8;
        break;
    case GTS_REQUEST_BOX_DOWNLOAD:
        size = 0x2B74;
        break;
    case GTS_REQUEST_22000:
        size = 4;
        break;
    case GTS_REQUEST_RANKINGS:
        size = 0x630;
        break;
    case GTS_REQUEST_VIDEO_UPLOAD:
        size = 8;
        break;
    case GTS_REQUEST_VIDEO_SEARCH:
        size = 0x1C24;
        break;
    case GTS_REQUEST_VIDEO_DOWNLOAD:
        size = 0x1D58;
        break;
    case GTS_REQUEST_23003:
        size = 0;
        break;
    default:
        return -1;
    }
    return size + 8;
}

void ov39_0222A8AC(void *request, void *arg) {
}
