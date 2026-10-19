#include "global.h"

#include "constants/dwc_status.h"

#include "global_terminal/global_terminal_net.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Socket client of the Global Terminal server: each request runs on its own thread (login token, socket, connect,
// XOR-encoded request, response into the caller's buffer).

#define GTS_SOCKET_TICKS_PER_SECOND 523656

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
    GlobalTerminalNetLogin login;
    BOOL hasToken;
    int state;
    char host[0xC0];
    u8 unk_200[0x40];
    u16 port;
    u16 unk_242;
    int connectTimeout; // seconds, 0 = none
    int sendTimeout;
    int receiveTimeout;
    int error;
    int requestSize;
    u8 *request; // size + command + login + payload, XOR-encoded after the size
    int responseSize;
    u8 *response; // caller's buffer: size + data
    u32 address;  // IPv4 address of host
    int cancel;
    int socket;
    int threadPriority;
} GlobalTerminalSocket;

// The socket settings
typedef struct GlobalTerminalSocketSettings {
    u8 unk_00[0x40];
    u16 port;
} GlobalTerminalSocketSettings;

typedef struct GlobalTerminalServerCommandIds {
    int ids[12]; // every command id, -1 terminated
} GlobalTerminalServerCommandIds;

typedef struct GlobalTerminalSocketToken {
    u8 bytes[301];
} GlobalTerminalSocketToken;

// filled by DWC_SVLGetTokenAsync
typedef struct GlobalTerminalSocketTokenRequest {
    u8 unk_00[0x45];
    GlobalTerminalSocketToken token;
    u8 unk_172[2];
} GlobalTerminalSocketTokenRequest;

typedef struct GlobalTerminalSocketAddress {
    u8 length;
    u8 family;
    u16 port;
    u32 address;
} GlobalTerminalSocketAddress;

typedef struct GlobalTerminalSocketHostEntry {
    int name;
    int aliases;
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
} GlobalTerminalServerDressUpUpload;

typedef struct GlobalTerminalServerIdRequest {
    u16 unk_00;
    u16 unk_02;
    int idLow;
    int idHigh;
} GlobalTerminalServerIdRequest;

typedef struct GlobalTerminalServerBoxData {
    u32 data[135];
} GlobalTerminalServerBoxData;

typedef struct GlobalTerminalServerBoxUpload {
    int tray;
    GlobalTerminalServerBoxData box;
} GlobalTerminalServerBoxUpload;

typedef struct GlobalTerminalServerRankingsRequest {
    u32 data[9];
} GlobalTerminalServerRankingsRequest;

typedef struct GlobalTerminalServerVideoUpload {
    u32 data[1875];
} GlobalTerminalServerVideoUpload;

typedef struct GlobalTerminalServerVideoSearchParams {
    u32 data[4];
} GlobalTerminalServerVideoSearchParams;

typedef struct GlobalTerminalServerVideoSearch {
    u16 kind;
    u16 unk_02;
    GlobalTerminalServerVideoSearchParams params;
    int unk_14[2];
} GlobalTerminalServerVideoSearch;

typedef struct GlobalTerminalServerVideoList {
    u16 kind;
    u16 unk_02;
    int data[6];
} GlobalTerminalServerVideoList;

extern GlobalTerminalSocket sGlobalTerminalSocket;
extern char sGlobalTerminalSocketHost[];
extern GlobalTerminalSocketSettings sGlobalTerminalSocketSettings;
extern OSThread sGlobalTerminalSocketThread;
extern OSMutex sGlobalTerminalSocketMutex;
extern u8 sGlobalTerminalSocketThreadStackEnd[];
extern const GlobalTerminalServerCommandIds sGlobalTerminalServerCommandIds;
extern u8 sGlobalTerminalSocketSvl[];
extern char sGlobalTerminalServerHostName[];
extern u8 ov39_0222AA78[];
extern u8 sGlobalTerminalSocketCipher[];

static void GlobalTerminalSocket_Reset(void);
static int GlobalTerminalSocket_Start(char *host, int port, GlobalTerminalNetLogin *login);
static void GlobalTerminalSocket_ClearRequest(void);
static inline int GlobalTerminalSocket_FetchTokenNextState(BOOL *done);
static void GlobalTerminalSocket_RunRequest(void);
static int GlobalTerminalSocket_SendRequest(int command, void *payload, void *response, int payloadSize);
static int GlobalTerminalSocket_GetStateUnlocked(void);
static void *GlobalTerminalSocket_GetResponse(void);
static int GlobalTerminalSocket_GetResponseSize(void);
static int GlobalTerminalSocket_GetError(void);
static void *GlobalTerminalSocket_Alloc(int size);
static void GlobalTerminalSocket_Free(void *ptr);
static void GlobalTerminalSocket_Lock(void);
static void GlobalTerminalSocket_Unlock(void);
static int GlobalTerminalSocket_InitMutex(void);
static int GlobalTerminalSocket_InitSettingsStub(void *settings);
static int GlobalTerminalSocket_InitTick(void);
static void GlobalTerminalSocket_ResetStub(void);
static void GlobalTerminalSocket_ThreadMain(void);
static int GlobalTerminalSocket_StartThread(void);
static int GlobalTerminalSocket_IsIPv4Address(signed char *str);
static int GlobalTerminalSocket_ResolveHost(char *host, char *out);
static int GlobalTerminalSocket_Open(void);
static void GlobalTerminalSocket_Close(void);
static inline u16 GlobalTerminalSocket_SwapBytes(u16 value);
static int GlobalTerminalSocket_Connect(void);
static int GlobalTerminalSocket_Send(void);
static int GlobalTerminalSocket_Receive(void);
static void GlobalTerminalSocket_Sleep(u32 msec);
static int GlobalTerminalSocket_FetchToken(void);
static void GlobalTerminalSocket_Encode(const u8 *src, int size, u8 *dst);
static inline u32 GlobalTerminalSocket_Xor(u32 byte, u32 key);
static void GlobalTerminalSocket_Decode(const u8 *src, int size, u8 *dst);
static int GlobalTerminalSocket_GetRequestSize(int command);
static int GlobalTerminalSocket_GetResponseBufferSize(int command);
static void GlobalTerminalSocket_PrepareUploadStub(void *request, void *arg);

void *DWC_Alloc(int category, int size);
void DWC_Free(int category, void *ptr, int size);
int gsSocketInetAddr(char *address);
int DWC_SVLGetTokenAsync(const char *arg, void *request);
int DWC_SVLProcess(void);
int DWC_GetLastErrorEx(int *errorCode, int *errorType);
GlobalTerminalSocketHostEntry *SOC_GetHostByName(char *name);
char *SOC_InetNtoA(GlobalTerminalSocketInAddr address);
int SOC_Socket(int domain, int type, int protocol);
int SOC_Fcntl(int socket, int command, int mode);
int SOC_Connect(int socket, GlobalTerminalSocketAddress *address);
int SOC_Send(int socket, void *buffer, int length, int flags);
int SOC_Recv(int socket, void *buffer, int length, int flags);
int SOC_Shutdown(int socket, int how);
int SOC_Close(int socket);

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
int GlobalTerminalServer_SendRequest22000(void *response);
int GlobalTerminalServer_GetRankings(GlobalTerminalServerRankingsRequest *request, void *response);
int GlobalTerminalServer_UploadVideo(GlobalTerminalServerVideoUpload *video, void *response);
int GlobalTerminalServer_SearchVideos(GlobalTerminalServerVideoSearchParams *params, void *response);
int GlobalTerminalServer_SearchVideosKind1(int *arg, void *response);
int GlobalTerminalServer_SearchVideosKind2(int *arg, void *response);
int GlobalTerminalServer_DownloadVideo(int idLow, int idHigh, int arg2, void *response);
int GlobalTerminalServer_SendRequest23003(int idLow, int idHigh, void *response);

static void GlobalTerminalSocket_Reset(void) {
    GlobalTerminalSocket_ClearRequest();
    memset(&sGlobalTerminalSocket.login, 0, sizeof(sGlobalTerminalSocket.login));
    sGlobalTerminalSocket.hasToken = FALSE;
    sGlobalTerminalSocket.host[0] = '\0';
    sGlobalTerminalSocketSettings.port = 0;
    GlobalTerminalSocket_InitSettingsStub(&sGlobalTerminalSocketSettings);
    GlobalTerminalSocket_ResetStub();
}

static int GlobalTerminalSocket_Start(char *host, int port, GlobalTerminalNetLogin *login) {
    if (login) {
        sGlobalTerminalSocket.login = *login;
        sGlobalTerminalSocket.hasToken = FALSE;
    }
    if (!GlobalTerminalSocket_InitTick()) {
        return 0;
    }
    GlobalTerminalSocket_InitMutex();
    strcpy(sGlobalTerminalSocketHost, host);
    sGlobalTerminalSocketSettings.port = port;
    sGlobalTerminalSocket.request = NULL;
    sGlobalTerminalSocket.socket = -1;
    GlobalTerminalSocket_ClearRequest();
    return 1;
}

static void GlobalTerminalSocket_ClearRequest(void) {
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

static void GlobalTerminalSocket_RunRequest(void) {
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

static int GlobalTerminalSocket_SendRequest(int command, void *payload, void *response, int payloadSize) {
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

static int GlobalTerminalSocket_GetStateUnlocked(void) {
    return sGlobalTerminalSocket.state;
}

static void *GlobalTerminalSocket_GetResponse(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_DONE) {
        GlobalTerminalSocket_Unlock();
        return NULL;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.response + 4;
}

static int GlobalTerminalSocket_GetResponseSize(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_DONE) {
        GlobalTerminalSocket_Unlock();
        return -1;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.responseSize;
}

static int GlobalTerminalSocket_GetError(void) {
    GlobalTerminalSocket_Lock();
    if (GlobalTerminalSocket_GetStateUnlocked() != GTS_SOCKET_STATE_ERROR) {
        GlobalTerminalSocket_Unlock();
        return GTS_SOCKET_ERROR_NONE;
    }
    GlobalTerminalSocket_Unlock();
    return sGlobalTerminalSocket.error;
}

static void *GlobalTerminalSocket_Alloc(int size) {
    return DWC_Alloc(0, size);
}

static void GlobalTerminalSocket_Free(void *ptr) {
    if (ptr) {
        DWC_Free(0, ptr, 0);
    }
}

static void GlobalTerminalSocket_Lock(void) {
    OS_LockMutex(&sGlobalTerminalSocketMutex);
}

static void GlobalTerminalSocket_Unlock(void) {
    OS_UnlockMutex(&sGlobalTerminalSocketMutex);
}

static int GlobalTerminalSocket_InitMutex(void) {
    OS_InitMutex(&sGlobalTerminalSocketMutex);
    return 1;
}

static int GlobalTerminalSocket_InitSettingsStub(void *settings) {
    return 1;
}

static int GlobalTerminalSocket_InitTick(void) {
    if (!OS_IsTickAvailable()) {
        OS_InitTick();
    }
    sGlobalTerminalSocket.threadPriority = 16;
    return 1;
}

static void GlobalTerminalSocket_ResetStub(void) {
}

static void GlobalTerminalSocket_ThreadMain(void) {
    GlobalTerminalSocket_RunRequest();
}

static int GlobalTerminalSocket_StartThread(void) {
    OS_CreateThread(&sGlobalTerminalSocketThread, (void (*)(void *))GlobalTerminalSocket_ThreadMain, NULL, sGlobalTerminalSocketThreadStackEnd, 0x800, sGlobalTerminalSocket.threadPriority);
    OS_WakeupThreadDirect(&sGlobalTerminalSocketThread);
    return 1;
}

static int GlobalTerminalSocket_IsIPv4Address(signed char *str) {
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

static int GlobalTerminalSocket_ResolveHost(char *host, char *out) {
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

static int GlobalTerminalSocket_Open(void) {
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

static void GlobalTerminalSocket_Close(void) {
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

static int GlobalTerminalSocket_Connect(void) {
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

static int GlobalTerminalSocket_Send(void) {
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

static int GlobalTerminalSocket_Receive(void) {
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

static void GlobalTerminalSocket_Sleep(u32 msec) {
    OS_Sleep(msec);
}

#pragma push
#pragma opt_propagation off

static int GlobalTerminalSocket_FetchToken(void) {
    int errorType, errorCode;
    GlobalTerminalSocketTokenRequest request;
    if (sGlobalTerminalSocket.hasToken == TRUE) {
        return 1;
    }
    if (!DWC_SVLGetTokenAsync((const char *)sGlobalTerminalSocketSvl, &request)) {
        sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_TOKEN;
        return 0;
    }
    for (;;) {
        int status = DWC_SVLProcess();
        if (status == DWC_SVL_STATE_SUCCESS) {
            GlobalTerminalSocketToken *token = (GlobalTerminalSocketToken *)sGlobalTerminalSocket.login.token;
            *token = request.token;
            sGlobalTerminalSocket.hasToken = TRUE;
            return 1;
        }
        if (status == DWC_SVL_STATE_ERROR) {
            DWC_GetLastErrorEx(&errorCode, &errorType);
            sGlobalTerminalSocket.error = GTS_SOCKET_ERROR_TOKEN;
            return 0;
        }
        if (status == DWC_SVL_STATE_CANCELED) {
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
    GlobalTerminalSocket_PrepareUploadStub(upload, ov39_0222AA78);
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
    GlobalTerminalSocket_PrepareUploadStub(box, ov39_0222AA78);
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

int GlobalTerminalServer_SendRequest22000(void *response) {
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

int GlobalTerminalServer_SearchVideosKind1(int *arg, void *response) {
    GlobalTerminalServerVideoList payload = { 0 };
    payload.kind = 1;
    payload.data[2] = *arg;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_VIDEO_SEARCH, &payload, response, -1);
}

int GlobalTerminalServer_SearchVideosKind2(int *arg, void *response) {
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

int GlobalTerminalServer_SendRequest23003(int idLow, int idHigh, void *response) {
    int payload[2];
    payload[0] = idLow;
    payload[1] = idHigh;
    return GlobalTerminalSocket_SendRequest(GTS_REQUEST_23003, payload, response, -1);
}

static void GlobalTerminalSocket_Encode(const u8 *src, int size, u8 *dst) {
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

static void GlobalTerminalSocket_Decode(const u8 *src, int size, u8 *dst) {
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

static int GlobalTerminalSocket_GetRequestSize(int command) {
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

static int GlobalTerminalSocket_GetResponseBufferSize(int command) {
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

static void GlobalTerminalSocket_PrepareUploadStub(void *request, void *arg) {
}
