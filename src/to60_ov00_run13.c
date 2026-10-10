#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

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
    int handle;               // 0x00
    u32 ip;                   // 0x04
    u16 port;                 // 0x08
    void *connections;        // 0x0c
    void *closedConnections;  // 0x10
    int close;                // 0x14
    int error;                // 0x18
    int callbackLevel;        // 0x1c
    void *connectAttemptCB;   // 0x20
    void *unk24;              // 0x24
    GTSockErrCB unk28;        // 0x28
    GTSockErrCB unk2C;        // 0x2c
    GTUnrecCB unrecognizedCB; // 0x30
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
    u32 ip;                         // 0x00
    u16 port;                       // 0x04
    GTSocket *socket;               // 0x08
    int state;                      // 0x0c
    int initiated;                  // 0x10
    int freeAtAcceptReject;         // 0x14
    int connectionResult;           // 0x18
    u32 startTime;                  // 0x1c
    u32 timeout;                    // 0x20
    int callbackLevel;              // 0x24
    GTCallbacks callbacks;          // 0x28
    u8 *initialMessage;             // 0x38
    int initialMessageLen;          // 0x3c
    void *data;                     // 0x40
    GTBuffer incomingBuffer;        // 0x44
    GTBuffer outgoingBuffer;        // 0x50
    void *incomingBufferedMessages; // 0x5c
    void *outgoingBufferedMessages; // 0x60
    u16 serialNumber;               // 0x64
    u16 expectedSerialNumber;       // 0x66
    u8 response[32];                // 0x68
    u32 lastSend;                   // 0x88
    u32 challengeTime;              // 0x8c
    int pendingAck;                 // 0x90
    u32 pendingAckTime;             // 0x94
    void *sendFilters;              // 0x98
    void *receiveFilters;           // 0x9c
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

int ov00_0220D9E8(GTConn *conn, int type, u8 *msg, int len) {
    conn->expectedSerialNumber++;
    if (type == 0) {
        if (!ov00_0220D5BC(conn, msg, len)) {
            return 0;
        }
    } else if (type == 1) {
        if (!ov00_0220D668(conn, msg, len)) {
            return 0;
        }
    } else if (type == 2) {
        if (!ov00_0220D700(conn, msg, len)) {
            return 0;
        }
    } else if (type == 3) {
        if (!ov00_0220D7DC(conn, msg, len)) {
            return 0;
        }
    } else if (type == 4) {
        if (!ov00_0220D8EC(conn)) {
            return 0;
        }
    } else if (type == 5) {
        if (!ov00_0220D938(conn, msg, len)) {
            return 0;
        }
    } else if (type == 6) {
        if (!ov00_0220D9A4(conn)) {
            return 0;
        }
    }
    return 1;
}

s16 ov00_0220DAE8(GTInMsg *a, GTInMsg *b) {
    return ov00_0220D34C(a->serial, b->serial);
}

int ov00_0220DAFC(GTConn *conn, int type, u16 serial, u8 *msg, int len, int *overflow) {
    int n;
    int i;
    GTInMsg *m;
    GTInMsg newMsg;

    n = ArrayLength(conn->incomingBufferedMessages);
    for (i = 0; i < n; i++) {
        m = ov00_021FE6EC(conn->incomingBufferedMessages, i);
        if (m->serial == serial) {
            *overflow = 0;
            return 1;
        }
        if (ov00_0220D34C(m->serial, serial) > 0) {
            break;
        }
    }
    if (ov00_0220BF70(&conn->incomingBuffer) < len) {
        *overflow = 1;
        return 1;
    }
    newMsg.start = conn->incomingBuffer.len;
    newMsg.len = len;
    newMsg.type = type;
    newMsg.serial = serial;
    ov00_021FE858(conn->incomingBufferedMessages, &newMsg, ov00_0220DAE8);
    if (ArrayLength(conn->incomingBufferedMessages) != n + 1) {
        *overflow = 1;
        return 1;
    }
    ov00_0220C03C(&conn->incomingBuffer, msg, len);
    if (n == 0) {
        if (!ov00_0220ECF8(conn, conn->expectedSerialNumber, serial - 1)) {
            return 0;
        }
    } else {
        m = ov00_021FE6EC(conn->incomingBufferedMessages, n);
        if (m->serial == serial) {
            m = ov00_021FE6EC(conn->incomingBufferedMessages, n - 1);
            if ((u16)ov00_0220D34C(serial, m->serial) > 1) {
                if (!ov00_0220ECF8(conn, m->serial + 1, serial - 1)) {
                    return 0;
                }
            }
        }
    }
    *overflow = 0;
    return 1;
}

void ov00_0220DCB0(GTConn *conn, GTInMsg *msg, int index) {
    int start = msg->start;
    int len = msg->len;
    int n;
    int i;
    int max = 0;
    int end;
    GTInMsg *m;

    ov00_021FE968(conn->incomingBufferedMessages, index);
    n = ArrayLength(conn->incomingBufferedMessages);
    for (i = 0; i < n; i++) {
        m = ov00_021FE6EC(conn->incomingBufferedMessages, i);
        if (m->start > start) {
            m->start -= len;
            end = m->start + m->len;
            if (max <= end) {
                max = end;
            }
        }
    }
    ov00_0220C0C4(&conn->incomingBuffer, start, len);
}

int ov00_0220DD34(GTConn *conn) {
    int i;
    GTInMsg *m;
again:
    for (i = ArrayLength(conn->incomingBufferedMessages) - 1; i >= 0; i--) {
        m = ov00_021FE6EC(conn->incomingBufferedMessages, i);
        if (m->serial == conn->expectedSerialNumber) {
            if (!ov00_0220D9E8(conn, m->type, conn->incomingBuffer.buffer + m->start, m->len)) {
                return 0;
            }
            ov00_0220DCB0(conn, m, i);
            goto again;
        }
    }
    return 1;
}

void ov00_0220DDB4(GTConn *conn) {
    if (conn->pendingAck) {
        return;
    }
    conn->pendingAck = 1;
    conn->pendingAckTime = ov00_021FF368();
}

int ov00_0220DDDC(GTConn *conn, int type, u8 *msg, int len) {
    u16 serial;
    int overflow;
    if (len < 7) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    serial = ov00_0220D318(msg, 3);
    if (!ov00_0220D430(conn, ov00_0220D318(msg, 5))) {
        return 0;
    }
    if (serial == conn->expectedSerialNumber) {
        ov00_0220DDB4(conn);
        if (!ov00_0220D9E8(conn, type, msg + 7, len - 7)) {
            return 0;
        }
        if (!ov00_0220DD34(conn)) {
            return 0;
        }
        return 1;
    }
    if (ov00_0220D34C(serial, conn->expectedSerialNumber) < 0) {
        ov00_0220DDB4(conn);
        return 1;
    }
    if (!ov00_0220DAFC(conn, type, serial, msg + 7, len - 7, &overflow)) {
        return 0;
    }
    if (overflow) {
        if (!ov00_0220D404(conn)) {
            return 0;
        }
    }
    return 1;
}

int ov00_0220DF28(GTConn *conn, u8 *msg, int len) {
    if (len != 2) {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    if (!ov00_0220D430(conn, ov00_0220D318(msg, 0))) {
        return 0;
    }
    return 1;
}

int ov00_0220DF74(GTConn *conn, u8 *msg, int len) {
    u16 start;
    u16 end;
    int n;
    int i;
    GTOutMsg *m;

    start = ov00_0220D318(msg, 0);
    if (len == 2) {
        end = start;
    } else if (len == 4) {
        end = ov00_0220D318(msg, 2);
    } else {
        if (!ov00_0220D3F0(conn)) {
            return 0;
        }
        return 1;
    }
    n = ArrayLength(conn->outgoingBufferedMessages);
    for (i = 0; i < n; i++) {
        m = ov00_021FE6EC(conn->outgoingBufferedMessages, i);
        if (ov00_0220D34C(m->serial, start) >= 0 && ov00_0220D34C(m->serial, end) <= 0) {
            if (!ov00_0220EE00(conn, m)) {
                return 0;
            }
        }
    }
    return 1;
}

int ov00_0220E050(GTConn *conn, u8 *msg, int len) {
    return ov00_0220ED84(conn, msg, len);
}

int ov00_0220E05C(GTConn *conn, u8 *msg, int len) {
    u32 time;
    if (!conn->callbacks.ping) {
        return 1;
    }
    if (len != 8) {
        return 1;
    }
    if (memcmp(msg, ov00_02219CAC, 4)) {
        return 1;
    }
    memcpy(&time, msg + 4, 4);
    if (!ov00_0220C5AC(conn, ov00_021FF368() - time)) {
        return 0;
    }
    return 1;
}
