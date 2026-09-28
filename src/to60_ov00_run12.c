#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef struct GTSocket GTSocket;
typedef struct GTConn GTConn;

typedef struct GTCallbacks {
    void *connected;
    void *received;
    void *closed;
    void *ping;
} GTCallbacks;

typedef void (*GTConnectedCB)(GTConn *, int, u8 *, int);
typedef void (*GTReceivedCB)(GTConn *, u8 *, int, int);
typedef void (*GTClosedCB)(GTConn *, int);
typedef void (*GTPingCB)(GTConn *, int);
typedef void (*GTFilterCB)(GTConn *, int, u8 *, int, int);
typedef void (*GTSockErrCB)(GTSocket *, GTConn *, u32, u16, int, u8 *, int);
typedef int (*GTUnrecCB)(GTSocket *, u32, u16, u8 *, int);

struct GTSocket {
    int handle;                 // 0x00
    u32 ip;                     // 0x04
    u16 port;                   // 0x08
    void *connections;          // 0x0c
    void *closedConnections;    // 0x10
    int close;                  // 0x14
    int error;                  // 0x18
    int callbackLevel;          // 0x1c
    void *connectAttemptCB;     // 0x20
    void *unk24;                // 0x24
    GTSockErrCB unk28;          // 0x28
    GTSockErrCB unk2C;          // 0x2c
    GTUnrecCB unrecognizedCB;   // 0x30
};

typedef struct GTBuffer {
    u8 *buffer;
    int size;
    int len;
} GTBuffer;

typedef struct GTOutMsg {
    int start;  // 0x0
    int len;    // 0x4
    u16 serial; // 0x8
    u16 pad;
    u32 lastSend; // 0xc
} GTOutMsg;

typedef struct GTInMsg {
    int start;  // 0x0
    int len;    // 0x4
    int type;   // 0x8
    u16 serial; // 0xc
} GTInMsg;

struct GTConn {
    u32 ip;                    // 0x00
    u16 port;                  // 0x04
    GTSocket *socket;          // 0x08
    int state;                 // 0x0c
    int initiated;             // 0x10
    int freeAtAcceptReject;    // 0x14
    int connectionResult;      // 0x18
    u32 startTime;             // 0x1c
    u32 timeout;               // 0x20
    int callbackLevel;         // 0x24
    GTCallbacks callbacks;     // 0x28
    u8 *initialMessage;        // 0x38
    int initialMessageLen;     // 0x3c
    void *data;                // 0x40
    GTBuffer incomingBuffer;   // 0x44
    GTBuffer outgoingBuffer;   // 0x50
    void *incomingBufferedMessages; // 0x5c
    void *outgoingBufferedMessages; // 0x60
    u16 serialNumber;          // 0x64
    u16 expectedSerialNumber;  // 0x66
    u8 response[32];           // 0x68
    u32 lastSend;              // 0x88
    u32 challengeTime;         // 0x8c
    int pendingAck;            // 0x90
    u32 pendingAckTime;        // 0x94
    void *sendFilters;         // 0x98
    void *receiveFilters;      // 0x9c
};

typedef struct GTFilter {
    GTFilterCB callback;
} GTFilter;

typedef struct GTSockAddr {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} GTSockAddr;

extern void __msl_assertion_failed(const char *expr, const char *file, const char *func, int line);
extern void *memcpy(void *, const void *, u32);
extern int memcmp(const void *, const void *, u32);

extern char ov00_02219C40[];
extern char ov00_02219C18[];
extern char ov00_02219C10[];
extern char ov00_02219B70[];
extern char ov00_02219B58[];
extern char ov00_02219B44[];
extern char ov00_02219B30[];
extern char ov00_02219B88[];
extern char ov00_02219BB8[];
extern char ov00_02219B1C[];
extern char ov00_02219BF0[];
extern char ov00_02219C88[];
extern char ov00_02219C7C[];
extern char ov00_02219C4C[];
extern char ov00_02219C64[];
extern char ov00_02219CAC[];
extern char ov00_02219CB4[];
extern int ov00_0221B630;

extern void ov00_0220F144(GTSocket *socket);
extern void *ov00_021FE6EC(void *array, int n);
extern int ArrayLength(void *array);
extern void ov00_021FE968(void *array, int n);
extern void ov00_021FE678(void *array);
extern void ov00_021FE754(void *array, void *elem);
extern void ov00_021FE858(void *array, void *elem, void *cmp);
extern void ov00_021FF01C(void *table, void *elem);
extern void ov00_021FF1F8(void *table, void *fn, void *user);
extern u32 ov00_021FF368(void);
extern void ov00_021FF3C0(int ms);
extern int ov00_021FF694(int sock);
extern int ov00_021FF940(int sock, void *buf, int len, int flags, void *addr, int *addrlen);
extern int ov00_021FFA60(int sock);
extern void *ov00_021EC3A8(int size);
extern void ov00_021EC3D8(void *ptr);
extern int ov00_0220F1A0(GTSocket *socket, GTConn **conn, u32 ip, u16 port);
extern void ov00_0220F958(u8 **msg, int *len);
extern void ov00_0220BCEC(u8 *challenge);
extern void ov00_0220BE08(u8 *dst, u8 *src);
extern int ov00_0220BF10(u8 *a, u8 *b);
extern int ov00_0220BF70(GTBuffer *buf);
extern void ov00_0220C03C(GTBuffer *buf, u8 *data, int len);
extern void ov00_0220C0C4(GTBuffer *buf, int start, int len);
extern int ov00_0220C200(GTSocket *socket, GTConn *conn, u32 ip, u16 port, int latency, u8 *msg, int len);
extern int ov00_0220E8E0(GTConn *conn, u8 *challenge);
extern int ov00_0220E940(GTConn *conn, u8 *response, u8 *challenge);
extern int ov00_0220E9CC(GTConn *conn, u8 *response, u8 *msg, int len);
extern int ov00_0220EA44(GTConn *conn);
extern int ov00_0220EAA0(GTConn *conn, u8 *msg, int len);
extern int ov00_0220EB14(GTConn *conn);
extern int ov00_0220EB70(GTConn *conn);
extern int ov00_0220EC94(GTConn *conn);
extern int ov00_0220ECF8(GTConn *conn, u16 a, u16 b);
extern int ov00_0220ED84(GTConn *conn, u8 *msg, int len);
extern int ov00_0220ED98(GTConn *conn);
extern int ov00_0220EDB4(GTSocket *socket, u32 ip, u16 port);
extern int ov00_0220EE00(GTConn *conn, GTOutMsg *msg);
extern int ov00_0220EE6C(GTConn *conn, u8 *msg, int len, int reliable);
extern GTConn *ov00_0220EEEC(GTSocket *socket, u32 ip, u16 port);
extern int ov00_0220EF24(void *a, void *b, void *c, void *d, void *e);
extern int ov00_0220F188(void *a, void *b, void *c, void *d);
extern void ov00_0220F3B0(GTConn *conn);
extern int ov00_0220F474(GTSocket *socket, u32 ip, u16 port, u8 *msg, int len);
extern int ov00_0220F674(GTSocket *socket);
extern void ov00_0220F6B0(GTSocket *socket);
extern void ov00_0220F6E8(GTSocket *socket);
extern int ov00_0220F7D4(const char *addr, u32 *ip, u16 *port);

int ov00_0220C31C(GTConn *conn, int result, u8 *msg, int len);
int ov00_0220C404(GTConn *conn, u8 *msg, int len, int reliable);
int ov00_0220C4E8(GTConn *conn, int reason);
int ov00_0220C5AC(GTConn *conn, int latency);
int ov00_0220C670(GTConn *conn, int index, u8 *msg, int len, int reliable);
int ov00_0220C768(GTConn *conn, int index, u8 *msg, int len, int reliable);
int ov00_0220C860(GTSocket *socket, GTConn *conn, u32 ip, u16 port, int reset, u8 *msg, int len, int fatal);
int ov00_0220C984(GTSocket *socket, u32 ip, u16 port, u8 *msg, int len, int *handled);
int ov00_0220CA7C(GTSocket *socket, GTConn **conn, u32 ip, u16 port);
int ov00_0220CAAC(GTSocket *socket, GTConn **conn, u32 ip, u16 port);
int ov00_0220CADC(GTConn *conn, u8 *msg, int len, GTCallbacks *callbacks);
int ov00_0220CB84(GTConn *conn, GTCallbacks *callbacks);
void ov00_0220CBE0(GTConn *conn, u8 *msg, int len);
int ov00_0220CC80(GTConn *conn, u32 now);
int ov00_0220CD24(GTConn *conn, u32 now);
int ov00_0220CD90(GTConn *conn, u32 now);
int ov00_0220CDCC(GTConn *conn, u32 now);
void ov00_0220CE68(GTConn *conn, int hard);
void ov00_0220CEB8(GTConn *conn);
void ov00_0220CFC4(GTSocket *socket);
void ov00_0220D218(GTConn *conn);
void ov00_0220D238(GTSocket *socket);
void ov00_0220D228(GTConn **conn);
u16 ov00_0220D318(u8 *buf, int off);
s16 ov00_0220D34C(u16 a, u16 b);
int ov00_0220D35C(GTConn *conn, int reason, int type);
int ov00_0220D3F0(GTConn *conn);
int ov00_0220D404(GTConn *conn);
int ov00_0220D430(GTConn *conn, u16 ack);
int ov00_0220D52C(GTConn *conn, u8 *msg, int len);
int ov00_0220D5BC(GTConn *conn, u8 *msg, int len);
int ov00_0220D668(GTConn *conn, u8 *msg, int len);
int ov00_0220D700(GTConn *conn, u8 *msg, int len);
int ov00_0220D7DC(GTConn *conn, u8 *msg, int len);
int ov00_0220D8EC(GTConn *conn);
int ov00_0220D938(GTConn *conn, u8 *msg, int len);
int ov00_0220D9A4(GTConn *conn);
int ov00_0220D9E8(GTConn *conn, int type, u8 *msg, int len);
s16 ov00_0220DAE8(GTInMsg *a, GTInMsg *b);
int ov00_0220DAFC(GTConn *conn, int type, u16 serial, u8 *msg, int len, int *overflow);
void ov00_0220DCB0(GTConn *conn, GTInMsg *msg, int index);
int ov00_0220DD34(GTConn *conn);
void ov00_0220DDB4(GTConn *conn);
int ov00_0220DDDC(GTConn *conn, int type, u8 *msg, int len);
int ov00_0220DF28(GTConn *conn, u8 *msg, int len);
int ov00_0220DF74(GTConn *conn, u8 *msg, int len);
int ov00_0220E050(GTConn *conn, u8 *msg, int len);
int ov00_0220E05C(GTConn *conn, u8 *msg, int len);
int ov00_0220E0F0(GTConn *conn);
int ov00_0220E128(GTConn *conn, int type, u8 *msg, int len);
int ov00_0220E1E0(GTSocket *socket, u8 *msg, int len, u32 ip, u16 port);
int ov00_0220E490(GTSocket *socket, u32 ip, u16 port);
int ov00_0220E590(GTSocket *socket);


int ov00_0220CC38(GTConn *conn, u8 *msg, int len);
int ov00_0220CE00(GTConn *conn, u32 now);
void ov00_0220CF14(GTConn *conn);
int ov00_0220CF98(void *a, void *b, void *c, void *d, void *e);
void ov00_0220CFAC(GTSocket *socket);
int ov00_0220CFF4(void *a, void *b, void *c, void *d);
int ov00_0220D000(GTConn *conn, GTCallbacks *callbacks);
void ov00_0220D00C(GTConn *conn, u8 *msg, int len);
int ov00_0220D018(GTSocket *socket, GTConn **connp, const char *address, u8 *msg, int len, u32 timeout, GTCallbacks *callbacks, int blocking);
void ov00_0220D194(GTConn *conn, u8 *msg, int len, int reliable);
u16 ov00_0220D27C(GTSocket *socket);
int ov00_0220D284(GTConn *conn);
u32 ov00_0220D294(GTConn *conn);
void ov00_0220D29C(GTSocket *socket, GTUnrecCB cb);
void ov00_0220D2A4(GTConn *conn, void *data);
void *ov00_0220D2E0(GTConn *conn);
void ov00_0220D338(u8 *buf, int off, u16 val);

// @fn ov00_0220C31C
int ov00_0220C31C(GTConn *conn, int result, u8 *msg, int len) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219B70, 0x69);
    }
    if (!conn) {
        return 1;
    }
    conn->connectionResult = result;
    if (!conn->callbacks.connected) {
        return 1;
    }
    if (!len || !msg) {
        msg = NULL;
        len = 0;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    ((GTConnectedCB)conn->callbacks.connected)(conn, result, msg, len);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C404
int ov00_0220C404(GTConn *conn, u8 *msg, int len, int reliable) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219B58, 0x94);
    }
    if (!conn) {
        return 1;
    }
    if (!conn->callbacks.received) {
        return 1;
    }
    if (!len || !msg) {
        msg = NULL;
        len = 0;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    ((GTReceivedCB)conn->callbacks.received)(conn, msg, len, reliable);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C4E8
int ov00_0220C4E8(GTConn *conn, int reason) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219B44, 0xba);
    }
    if (!conn) {
        return 1;
    }
    if (!conn->callbacks.closed) {
        return 1;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    ((GTClosedCB)conn->callbacks.closed)(conn, reason);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C5AC
int ov00_0220C5AC(GTConn *conn, int latency) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219B30, 0xd9);
    }
    if (!conn) {
        return 1;
    }
    if (!conn->callbacks.ping) {
        return 1;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    ((GTPingCB)conn->callbacks.ping)(conn, latency);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C670
int ov00_0220C670(GTConn *conn, int index, u8 *msg, int len, int reliable) {
    GTFilter *filter;
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219B88, 0x101);
    }
    if (!conn) {
        return 1;
    }
    filter = ov00_021FE6EC(conn->sendFilters, index);
    if (!filter) {
        return 1;
    }
    if (!len || !msg) {
        msg = NULL;
        len = 0;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    filter->callback(conn, index, msg, len, reliable);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C768
int ov00_0220C768(GTConn *conn, int index, u8 *msg, int len, int reliable) {
    GTFilter *filter;
    if (!conn) {
        __msl_assertion_failed(ov00_02219C40, ov00_02219C18, ov00_02219BB8, 0x12d);
    }
    if (!conn) {
        return 1;
    }
    filter = ov00_021FE6EC(conn->receiveFilters, index);
    if (!filter) {
        return 1;
    }
    if (!len || !msg) {
        msg = NULL;
        len = 0;
    }
    conn->callbackLevel++;
    conn->socket->callbackLevel++;
    filter->callback(conn, index, msg, len, reliable);
    conn->callbackLevel--;
    conn->socket->callbackLevel--;
    if (conn->socket->close && !conn->socket->callbackLevel) {
        ov00_0220F144(conn->socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C860
int ov00_0220C860(GTSocket *socket, GTConn *conn, u32 ip, u16 port, int reset, u8 *msg, int len, int fatal) {
    GTSockErrCB cb;
    if (!socket) {
        __msl_assertion_failed(ov00_02219C10, ov00_02219C18, ov00_02219B1C, 0x160);
    }
    if (!socket) {
        return 1;
    }
    if (fatal) {
        cb = socket->unk28;
    } else {
        cb = socket->unk2C;
    }
    if (!cb) {
        return 1;
    }
    if (!len || !msg) {
        len = 0;
        msg = NULL;
    }
    socket->callbackLevel++;
    if (conn) {
        conn->callbackLevel++;
    }
    cb(socket, conn, ip, port, reset, msg, len);
    socket->callbackLevel--;
    if (conn) {
        conn->callbackLevel--;
    }
    if (socket->close && !socket->callbackLevel) {
        ov00_0220F144(socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220C984
int ov00_0220C984(GTSocket *socket, u32 ip, u16 port, u8 *msg, int len, int *handled) {
    *handled = 0;
    if (!socket) {
        __msl_assertion_failed(ov00_02219C10, ov00_02219C18, ov00_02219BF0, 0x197);
    }
    if (!socket) {
        return 1;
    }
    if (!socket->unrecognizedCB) {
        return 1;
    }
    if (!len || !msg) {
        msg = NULL;
        len = 0;
    }
    socket->callbackLevel++;
    *handled = socket->unrecognizedCB(socket, ip, port, msg, len);
    socket->callbackLevel--;
    if (socket->close && !socket->callbackLevel) {
        ov00_0220F144(socket);
        return 0;
    }
    return 1;
}

// @fn ov00_0220CA7C
int ov00_0220CA7C(GTSocket *socket, GTConn **conn, u32 ip, u16 port) {
    int result = ov00_0220F1A0(socket, conn, ip, port);
    if (result) {
        return result;
    }
    (*conn)->state = 0;
    (*conn)->initiated = 1;
    return 0;
}

// @fn ov00_0220CAAC
int ov00_0220CAAC(GTSocket *socket, GTConn **conn, u32 ip, u16 port) {
    int result = ov00_0220F1A0(socket, conn, ip, port);
    if (result) {
        return result;
    }
    (*conn)->state = 2;
    (*conn)->initiated = 0;
    return 0;
}

// @fn ov00_0220CADC
int ov00_0220CADC(GTConn *conn, u8 *msg, int len, GTCallbacks *callbacks) {
    u8 challenge[32];
    ov00_0220F958(&msg, &len);
    if (len > 0) {
        conn->initialMessage = ov00_021EC3A8(len);
        if (!conn->initialMessage) {
            return 1;
        }
        memcpy(conn->initialMessage, msg, len);
        conn->initialMessageLen = len;
    }
    if (callbacks) {
        conn->callbacks = *callbacks;
    }
    ov00_0220BCEC(challenge);
    ov00_0220BE08(conn->response, challenge);
    ov00_0220E8E0(conn, challenge);
    conn->state = 0;
    return 0;
}

// @fn ov00_0220CB84
int ov00_0220CB84(GTConn *conn, GTCallbacks *callbacks) {
    if (conn->freeAtAcceptReject) {
        conn->freeAtAcceptReject = 0;
        return 0;
    }
    conn->freeAtAcceptReject = 0;
    if (conn->state != 4) {
        return 0;
    }
    ov00_0220EA44(conn);
    conn->state = 5;
    if (callbacks) {
        conn->callbacks = *callbacks;
    }
    return 1;
}

// @fn ov00_0220CBE0
void ov00_0220CBE0(GTConn *conn, u8 *msg, int len) {
    conn->freeAtAcceptReject = 0;
    if (conn->state != 4) {
        return;
    }
    ov00_0220F958(&msg, &len);
    ov00_0220EAA0(conn, msg, len);
    conn->state = 6;
}

// @fn ov00_0220CC38
int ov00_0220CC38(GTConn *conn, u8 *msg, int len) {
    if (!ov00_0220F474(conn->socket, conn->ip, conn->port, msg, len)) {
        return 0;
    }
    conn->lastSend = ov00_021FF368();
    return 1;
}

// @fn ov00_0220CC80
int ov00_0220CC80(GTConn *conn, u32 now) {
    int timedOut;
    if (conn->state < 5) {
        timedOut = 0;
        if (conn->initiated) {
            if (conn->timeout) {
                if (now - conn->startTime > conn->timeout) {
                    timedOut = 1;
                }
            }
        } else if (conn->state < 4) {
            if (now - conn->startTime > 60000) {
                timedOut = 1;
            }
        }
        if (timedOut) {
            ov00_0220ED98(conn);
            ov00_0220CEB8(conn);
            if (!ov00_0220C31C(conn, 6, NULL, 0)) {
                return 0;
            }
        }
    }
    return 1;
}

// @fn ov00_0220CD24
int ov00_0220CD24(GTConn *conn, u32 now) {
    int i;
    int n;
    GTOutMsg *msg;
    n = ArrayLength(conn->outgoingBufferedMessages);
    for (i = 0; i < n; i++) {
        msg = ov00_021FE6EC(conn->outgoingBufferedMessages, i);
        if (now - msg->lastSend > 1000) {
            if (!ov00_0220EE00(conn, msg)) {
                return 0;
            }
        }
    }
    return 1;
}

// @fn ov00_0220CD90
int ov00_0220CD90(GTConn *conn, u32 now) {
    if (!conn->pendingAck) {
        return 1;
    }
    if (now - conn->pendingAckTime > 100) {
        if (!ov00_0220EC94(conn)) {
            return 0;
        }
    }
    return 1;
}

// @fn ov00_0220CDCC
int ov00_0220CDCC(GTConn *conn, u32 now) {
    if (now - conn->lastSend > 30000) {
        if (!ov00_0220EB70(conn)) {
            return 0;
        }
    }
    return 1;
}

// @fn ov00_0220CE00
int ov00_0220CE00(GTConn *conn, u32 now) {
    if (!ov00_0220CC80(conn, now)) {
        return 0;
    }
    if (!ov00_0220CDCC(conn, now)) {
        return 0;
    }
    if (!ov00_0220CD24(conn, now)) {
        return 0;
    }
    if (!ov00_0220CD90(conn, now)) {
        return 0;
    }
    return 1;
}

// @fn ov00_0220CE68
void ov00_0220CE68(GTConn *conn, int hard) {
    if (hard) {
        if (conn->state >= 7) {
            return;
        }
        ov00_0220CEB8(conn);
        ov00_0220ED98(conn);
        ov00_0220C4E8(conn, 0);
        ov00_0220F3B0(conn);
    } else {
        conn->state = 6;
        ov00_0220EB14(conn);
    }
}

// @fn ov00_0220CEB8
void ov00_0220CEB8(GTConn *conn) {
    if (conn->state == 7) {
        return;
    }
    conn->state = 7;
    ov00_021FF01C(conn->socket->connections, &conn);
    ov00_021FE754(conn->socket->closedConnections, &conn);
}

// @fn ov00_0220CF14
void ov00_0220CF14(GTConn *conn) {
    if (conn->initialMessage) {
        ov00_021EC3D8(conn->initialMessage);
    }
    if (conn->incomingBuffer.buffer) {
        ov00_021EC3D8(conn->incomingBuffer.buffer);
    }
    if (conn->outgoingBuffer.buffer) {
        ov00_021EC3D8(conn->outgoingBuffer.buffer);
    }
    if (conn->incomingBufferedMessages) {
        ov00_021FE678(conn->incomingBufferedMessages);
    }
    if (conn->outgoingBufferedMessages) {
        ov00_021FE678(conn->outgoingBufferedMessages);
    }
    if (conn->sendFilters) {
        ov00_021FE678(conn->sendFilters);
    }
    if (conn->receiveFilters) {
        ov00_021FE678(conn->receiveFilters);
    }
    ov00_021EC3D8(conn);
}

// @fn ov00_0220CF98
int ov00_0220CF98(void *a, void *b, void *c, void *d, void *e) {
    return ov00_0220EF24(a, b, c, d, e);
}

// @fn ov00_0220CFAC
void ov00_0220CFAC(GTSocket *socket) {
    ov00_0220D238(socket);
    ov00_0220F144(socket);
}

// @fn ov00_0220CFC4
void ov00_0220CFC4(GTSocket *socket) {
    if (!ov00_0220E590(socket)) {
        return;
    }
    if (!ov00_0220F674(socket)) {
        return;
    }
    ov00_0220F6B0(socket);
}

// @fn ov00_0220CFF4
int ov00_0220CFF4(void *a, void *b, void *c, void *d) {
    return ov00_0220F188(a, b, c, d);
}

// @fn ov00_0220D000
int ov00_0220D000(GTConn *conn, GTCallbacks *callbacks) {
    return ov00_0220CB84(conn, callbacks);
}

// @fn ov00_0220D00C
void ov00_0220D00C(GTConn *conn, u8 *msg, int len) {
    ov00_0220CBE0(conn, msg, len);
}

// @fn ov00_0220D018
int ov00_0220D018(GTSocket *socket, GTConn **connp, const char *address, u8 *msg, int len, u32 timeout, GTCallbacks *callbacks, int blocking) {
    GTConn *conn;
    u32 ip;
    u16 port;
    int result;
    int done;

    if (!ov00_0220F7D4(address, &ip, &port) || !ip || !port) {
        return 4;
    }
    if (((((ip >> 24) & 0xFF) | ((ip >> 8) & 0xFF00) | ((ip << 8) & 0xFF0000) | ((ip << 24) & 0xFF000000)) & 0xE0000000) == 0xE0000000) {
        return 4;
    }
    result = ov00_0220CA7C(socket, &conn, ip, port);
    if (result) {
        return result;
    }
    conn->timeout = timeout;
    result = ov00_0220CADC(conn, msg, len, callbacks);
    if (result) {
        ov00_0220F3B0(conn);
        return result;
    }
    if (!blocking) {
        if (connp) {
            *connp = conn;
        }
        return 0;
    }
    conn->callbackLevel++;
    do {
        ov00_0220CFC4(socket);
        done = conn->state >= 5;
        if (!done) {
            ov00_021FF3C0(1);
        }
    } while (!done);
    conn->callbackLevel--;
    if (conn->state == 5) {
        *connp = conn;
    }
    return conn->connectionResult;
}

// @fn ov00_0220D194
void ov00_0220D194(GTConn *conn, u8 *msg, int len, int reliable) {
    if (conn->state != 5) {
        return;
    }
    ov00_0220F958(&msg, &len);
    if (ArrayLength(conn->sendFilters)) {
        ov00_0220C670(conn, 0, msg, len, reliable);
        return;
    }
    ov00_0220EE6C(conn, msg, len, reliable);
}

// @fn ov00_0220D218
void ov00_0220D218(GTConn *conn) {
    ov00_0220CE68(conn, 1);
}

// @fn ov00_0220D228
void ov00_0220D228(GTConn **conn) {
    ov00_0220D218(*conn);
}

// @fn ov00_0220D238
void ov00_0220D238(GTSocket *socket) {
    if (ov00_0221B630 == 1) {
        return;
    }
    ov00_0221B630 = 1;
    ov00_021FF1F8(socket->connections, ov00_0220D228, NULL);
    ov00_0221B630 = 0;
}

// @fn ov00_0220D27C
u16 ov00_0220D27C(GTSocket *socket) {
    return socket->port;
}

// @fn ov00_0220D284
int ov00_0220D284(GTConn *conn) {
    return conn->outgoingBuffer.size - conn->outgoingBuffer.len;
}

// @fn ov00_0220D294
u32 ov00_0220D294(GTConn *conn) {
    return conn->ip;
}

// @fn ov00_0220D29C
void ov00_0220D29C(GTSocket *socket, GTUnrecCB cb) {
    socket->unrecognizedCB = cb;
}

// @fn ov00_0220D2A4
void ov00_0220D2A4(GTConn *conn, void *data) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C88, ov00_02219C7C, ov00_02219C4C, 0x1a3);
    }
    conn->data = data;
}

// @fn ov00_0220D2E0
void *ov00_0220D2E0(GTConn *conn) {
    if (!conn) {
        __msl_assertion_failed(ov00_02219C88, ov00_02219C7C, ov00_02219C64, 0x1aa);
    }
    return conn->data;
}

// @fn ov00_0220D318
u16 ov00_0220D318(u8 *buf, int off) {
    u16 v = (u16)((buf[off] << 8) & 0xFF00);
    v |= buf[off + 1];
    return v;
}

// @fn ov00_0220D338
void ov00_0220D338(u8 *buf, int off, u16 val) {
    buf[off] = val >> 8;
    buf[off + 1] = val;
}

// @fn ov00_0220D34C
s16 ov00_0220D34C(u16 a, u16 b) {
    return (s16)(a - b);
}

// @fn ov00_0220D35C
int ov00_0220D35C(GTConn *conn, int reason, int type) {
    if (conn->state < 5) {
        if (conn->initiated) {
            ov00_0220CEB8(conn);
            if (!ov00_0220C31C(conn, reason, NULL, 0)) {
                return 0;
            }
        } else {
            if (conn->state == 4) {
                conn->freeAtAcceptReject = 1;
            }
            ov00_0220CEB8(conn);
        }
    } else if (conn->state != 7) {
        ov00_0220CEB8(conn);
        if (!ov00_0220C4E8(conn, type)) {
            return 0;
        }
    }
    return 1;
}

// @fn ov00_0220D3F0
int ov00_0220D3F0(GTConn *conn) {
    return ov00_0220D35C(conn, 7, 2);
}

// @fn ov00_0220D404
int ov00_0220D404(GTConn *conn) {
    if (!ov00_0220ED98(conn)) {
        return 0;
    }
    return ov00_0220D35C(conn, 1, 4);
}

// @fn ov00_0220D430
int ov00_0220D430(GTConn *conn, u16 ack) {
    int n;
    int i;
    int first;
    GTOutMsg *msg;

    n = ArrayLength(conn->outgoingBufferedMessages);
    if (!n) {
        return 1;
    }
    for (i = 0; i < n; i++) {
        msg = ov00_021FE6EC(conn->outgoingBufferedMessages, i);
        if (ov00_0220D34C(msg->serial, ack) >= 0) {
            break;
        }
    }
    if (i == 0) {
        return 1;
    }
    while (i--) {
        ov00_021FE968(conn->outgoingBufferedMessages, i);
    }
    n = ArrayLength(conn->outgoingBufferedMessages);
    if (!n) {
        conn->outgoingBuffer.len = 0;
        return 1;
    }
    msg = ov00_021FE6EC(conn->outgoingBufferedMessages, 0);
    first = msg->start;
    for (i = 0; i < n; i++) {
        msg = ov00_021FE6EC(conn->outgoingBufferedMessages, i);
        msg->start -= first;
    }
    ov00_0220C0C4(&conn->outgoingBuffer, 0, first);
    return 1;
}

// @fn ov00_0220D52C
int ov00_0220D52C(GTConn *conn, u8 *msg, int len) {
    if (conn->state != 5 && conn->state != 6) {
        return 1;
    }
    if (ArrayLength(conn->receiveFilters)) {
        if (!ov00_0220C768(conn, 0, msg, len, 0)) {
            return 0;
        }
        return 1;
    }
    if (!ov00_0220C404(conn, msg, len, 0)) {
        return 0;
    }
    return 1;
}

// @fn ov00_0220D5BC
int ov00_0220D5BC(GTConn *conn, u8 *msg, int len) {
    if (conn->state != 5 && conn->state != 6) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
    } else {
        if (ArrayLength(conn->receiveFilters)) {
            if (!ov00_0220C768(conn, 0, msg, len, 1)) {
                return 0;
            }
            return 1;
        }
        if (!ov00_0220C404(conn, msg, len, 1)) {
            return 0;
        }
    }
    return 1;
}

// @fn ov00_0220D668
int ov00_0220D668(GTConn *conn, u8 *msg, int len) {
    u8 response[32];
    u8 challenge[32];
    if (conn->state != 2) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (len < 0x20) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    ov00_0220BE08(response, msg);
    ov00_0220BCEC(challenge);
    ov00_0220BE08(conn->response, challenge);
    if (!ov00_0220E940(conn, response, challenge)) {
        return 0;
    }
    conn->state = 3;
    return 1;
}

// @fn ov00_0220D700
int ov00_0220D700(GTConn *conn, u8 *msg, int len) {
    u8 response[32];
    if (conn->state != 0) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (len < 0x40) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (!ov00_0220BF10(msg, conn->response)) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    ov00_0220BE08(response, msg + 0x20);
    if (!ov00_0220E9CC(conn, response, conn->initialMessage, conn->initialMessageLen)) {
        return 0;
    }
    if (conn->initialMessage) {
        ov00_021EC3D8(conn->initialMessage);
        conn->initialMessage = NULL;
    }
    conn->state = 1;
    return 1;
}

// @fn ov00_0220D7DC
int ov00_0220D7DC(GTConn *conn, u8 *msg, int len) {
    if (conn->state != 3) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (len < 0x20) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (!ov00_0220BF10(msg, conn->response)) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (!conn->socket->connectAttemptCB) {
        if (!ov00_0220ED98(conn)) {
            return 0;
        }
        ov00_0220CEB8(conn);
        return 1;
    }
    conn->state = 4;
    if (!ov00_0220C200(conn->socket, conn, conn->ip, conn->port, ov00_021FF368() - conn->challengeTime, msg + 0x20, len - 0x20)) {
        return 0;
    }
    return 1;
}

// @fn ov00_0220D8EC
int ov00_0220D8EC(GTConn *conn) {
    if (conn->state != 1) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    conn->state = 5;
    if (!ov00_0220C31C(conn, 0, NULL, 0)) {
        return 0;
    }
    return 1;
}

// @fn ov00_0220D938
int ov00_0220D938(GTConn *conn, u8 *msg, int len) {
    if (conn->state != 1) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    ov00_0220CEB8(conn);
    if (!ov00_0220ED98(conn)) {
        return 0;
    }
    if (!ov00_0220C31C(conn, 2, msg, len)) {
        return 0;
    }
    return 1;
}
