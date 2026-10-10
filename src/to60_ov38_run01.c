#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct GHIConnection GHIConnection;

typedef struct GHIBuffer {
    GHIConnection *connection;
    char *data;
    int size;
    int len;
    int pos;
    int sizeIncrement;
    int fixed;
    int dontFree;
    int encrypted;
} GHIBuffer;

typedef struct GHIEncryptor {
    int f00;
    int engine;
    int initialized;
    int f0c;
    int f10;
    void (*cleanupFunc)(GHIConnection *, struct GHIEncryptor *);
    int (*encryptFunc)(GHIConnection *, struct GHIEncryptor *, const char *, int *, char *, int *);
    int (*decryptFunc)(GHIConnection *, struct GHIEncryptor *, const char *, int *, char *, int *);
} GHIEncryptor;

typedef struct GHIPostState {
    struct GHIPostData *data;
    int pos;
    void *file;
    int len;
} GHIPostState;

typedef struct GHIPostData {
    int type;
    char *name;
    char *f08;
    int f0c;
    char *f10;
    int f14;
} GHIPostData;

typedef struct GHIPost {
    void *data;
    void *callback;
    void *param;
    int hasFiles;
    int autoFree;
} GHIPost;

typedef int (*CompletedCallback)(int, int, char *, int, void *);
typedef void (*ProgressCallback)(int, int, const char *, int, int, int, void *);
typedef void (*PostCallback)(int, int, int, int, int, void *);

struct GHIConnection {
    int inUse;                           // 0x00
    int request;                         // 0x04
    int uniqueID;                        // 0x08
    int type;                            // 0x0C
    int state;                           // 0x10
    char *URL;                           // 0x14
    char *serverAddress;                 // 0x18
    int serverIP;                        // 0x1C
    u16 serverPort;                      // 0x20
    char *requestPath;                   // 0x24
    char *sendHeaders;                   // 0x28
    int f2C;                             // 0x2C
    int blocking;                        // 0x30
    int f34;                             // 0x34
    int result;                          // 0x38
    ProgressCallback progressCallback;   // 0x3C
    CompletedCallback completedCallback; // 0x40
    void *callbackParam;                 // 0x44
    int socket;                          // 0x48
    int socketError;                     // 0x4C
    GHIBuffer sendBuffer;                // 0x50
    GHIBuffer encodeBuffer;              // 0x74
    GHIBuffer recvBuffer;                // 0x98
    GHIBuffer getFileBuffer;             // 0xBC
    int userBufferSupplied;              // 0xE0
    int fE4;
    int fE8;
    int statusCode; // 0xEC
    int fF0;
    int fF4;
    int fF8;
    int completed;         // 0xFC
    int fileBytesReceived; // 0x100
    int totalSize;         // 0x104
    char *redirectURL;     // 0x108
    int redirectCount;     // 0x10C
    int f110;
    int f114[6];
    int processing; // 0x12C
    int f130;
    int throttle;              // 0x134
    int lastThrottleRecv;      // 0x138
    GHIPost *post;             // 0x13C
    void *states;              // 0x140
    int index;                 // 0x144
    int bytesPosted;           // 0x148
    int totalBytes;            // 0x14C
    PostCallback postCallback; // 0x150
    void *postParam;           // 0x154
    int f158;
    char *f15C;
    u16 f160;
    GHIEncryptor encryptor; // 0x164
};

typedef struct GHIConnList {
    int count;
    GHIConnection **list;
    int nextUniqueID;
    int capacity;
} GHIConnList;

extern GHIConnList ov38_0222602C;
extern int ov38_0222603C;
extern void *ov38_02226028;

extern struct {
    int a;
    int b;
} ov38_0222531C;

extern int ov38_02225320;

extern struct {
    int f0;
    int f4;
    int f8;
    int fC;
} ov38_02226040;

typedef struct {
    char c[4];
} Hex4;

extern Hex4 _02224E60;

extern char ov38_022251D4[];
extern char ov38_022251DC[];
extern char ov38_02225168[];
extern char ov38_022251EC[];
extern char ov38_02225200[];
extern char ov38_02225148[];
extern char ov38_0222520C[];
extern char ov38_02225178[];
extern char ov38_0222521C[];
extern char ov38_02225228[];
extern char _02225138[];
extern char ov38_022251A4[];
extern char ov38_02225234[];
extern char ov38_0222523C[];
extern char ov38_0222524C[];
extern char ov38_02225288[];
extern char ov38_0222528C[];
extern char ov38_0222518C[];
extern char ov38_02225290[];
extern char ov38_02225158[];
extern char ov38_02225294[];
extern char ov38_022251BC[];
extern char ov38_022252A8[];
extern char ov38_022252FC[];
extern char ov38_02225308[];
extern char ov38_022252E0[];
extern char ov38_022252C8[];
extern char ov38_022252B4[];
extern char ov38_02225378[];
extern char ov38_022253A0[];
extern char ov38_02225324[];
extern char ov38_022253B4[];
extern char ov38_02225334[];
extern char ov38_022253C0[];
extern char ov38_022253DC[];
extern char ov38_02225404[];
extern char ov38_02225418[];
extern char ov38_02225360[];
extern char ov38_02225428[];
extern char ov38_02225348[];
extern char ov38_02225444[];
extern char ov38_02225480[];
extern char ov38_0222548C[];
extern char ov38_02225468[];
extern char ov38_02225498[];
extern char ov38_022254D4[];
extern char ov38_0222545C[];
extern char ov38_022254E4[];
extern char ov38_022254F4[];
extern char ov38_02225694[];
extern char ov38_02225644[];
extern char ov38_02225574[];
extern char ov38_0222569C[];
extern char ov38_022256A0[];
extern char ov38_022256E4[];
extern char ov38_02225708[];
extern char ov38_022255E0[];
extern char ov38_02225720[];
extern char ov38_02225748[];
extern char ov38_02225620[];
extern char ov38_02225640[];
extern char ov38_0222558C[];
extern char ov38_02225534[];
extern char ov38_02225548[];
extern char ov38_02225750[];
extern char ov38_0222550C[];
extern char ov38_02225764[];
extern char ov38_022257B8[];
extern char ov38_022255A4[];
extern char ov38_022257C8[];
extern char ov38_02225804[];
extern char ov38_02225650[];
extern char ov38_022257F4[];
extern char ov38_022255C0[];
extern char ov38_02225818[];
extern char ov38_02225840[];

void __msl_assertion_failed(const char *, const char *, const char *, int);
void *ov00_021EC3BC(void *, int);
void *ov00_021EC3A8(int);
void ov00_021EC3D8(void *);
int ov00_021FF59C(int, int, int *, int *);
int ov00_021FFA60(int);
int ov00_021FF368(void);
int ov00_021FF92C(int, char *, int, int);
int ov00_021FF970(int, const char *, int, int);
void ov00_021FF82C(int, int);
void ov00_021FF818(int);
char *ov00_021FF3D4(const char *);
void ov00_021FF3C0(int);
void ov00_021FE678(void *);
void *ov00_021FE6EC(void *, int);
void *ov00_021FE5B4(int, int, int);
void ov00_021FE754(void *, void *);
int ArrayLength(void *);
int sprintf(char *, const char *, ...);
unsigned strlen(const char *);
char *strchr(const char *, int);
int fseek(void *, long, int);
long ftell(void *);
void rewind(void *);
int fclose(void *);
unsigned fread(void *, unsigned, unsigned, void *);

int ghiResizeBuffer(GHIBuffer *buffer, int sizeIncrement);
int ghiAppendDataToBuffer(GHIBuffer *buffer, const char *data, int len);
int ghiAppendCharToBuffer(GHIBuffer *buffer, char c);
void ghiResetBuffer(GHIBuffer *buffer);
int ghiReadDataFromBuffer(GHIBuffer *buffer, char *out, int *len);
int ghiFreeConnection(GHIConnection *c);
void ov38_0221CA44(void);
void ov38_0221CA48(void);
void ov38_0221CA4C(void);
void ov38_0221CA50(void);
int ov38_0221CA54(GHIConnection *c);
int ov38_0221CD90(GHIConnection *c, const char *buf, int len);
int ov38_0221CE0C(GHIConnection *c, const char *buf, int len);
int ov38_0221DB00(GHIPost *post);
void ov38_0221DB08(GHIPost *post);
void ov38_0221E0DC(GHIConnection *c);
int ghiPostInitState(GHIConnection *c);
int ghiProcessConnection(GHIConnection *c);
void ov38_0221D3B0(void (*func)(GHIConnection *));
void ov38_0221D414(GHIConnection *c);
void ov38_0221D590(GHIConnection *c);
void ghiCallCompletedCallback(GHIConnection *c);
void ov38_0221D7B4(void);
void ov38_0221D518(void);
int ghttpGetExA(const char *, const char *, char *, int, GHIPost *, int, int, ProgressCallback, CompletedCallback, void *);
void ov38_0221EBB0(GHIConnection *);
void ov38_0221EC74(GHIConnection *);
void ghiDoSecuringSession(GHIConnection *);
void ov38_0221F024(GHIConnection *);
void ghiDoPosting(GHIConnection *);
void ov38_0221F330(GHIConnection *);
void ov38_0221F4FC(GHIConnection *);
void ov38_0221FA38(GHIConnection *);
void ov38_0221FEA8(GHIConnection *);
int ghiPostGetHasFilesContentLength(GHIConnection *c);
int ghiPostGetNoFilesContentLength(GHIConnection *c);
int ghiPostGetContentLength(GHIConnection *c);
int ghiPostStateInit(GHIPostState *s);
void ghiPostStateCleanup(GHIPostState *s);

static inline void ZeroState(void *p) {
    GHIPostState *s = p;
    s->data = NULL;
    s->pos = 0;
    s->file = NULL;
    s->len = 0;
}

#define ASSERT(e, s, f, fn, l) \
    if (!(e))                  \
    __msl_assertion_failed(s, f, fn, l)

int ghiInitBuffer(GHIConnection *connection, GHIBuffer *buffer, int initialSize, int sizeIncrement);
int ghiInitFixedBuffer(GHIConnection *connection, GHIBuffer *buffer, char *userBuffer, int size);
void ghiFreeBuffer(GHIBuffer *buffer);
int ov38_0221C5EC(GHIBuffer *buffer, const char *name, const char *value);
int ov38_0221C6CC(GHIBuffer *buffer, int i);
int ov38_0221C748(GHIConnection *c);
void ghiCallProgressCallback(GHIConnection *c, const char *buffer, int len);
void ghiCallPostCallback(GHIConnection *c);
int ov38_0221CB7C(GHIConnection *c, char *buffer, int *bufferLen);
int ghiFindFreeSlot(void);
GHIConnection *ov38_0221CF8C(void);
GHIConnection *ghiRequestToConnection(int request);
void ov38_0221D808(void);
int ov38_0221D864(const char *url, int blocking, CompletedCallback completed, void *param);
void ov38_0221DAEC(void);
char *ghiPostGetContentType(GHIConnection *c);

int ghiResizeBuffer(GHIBuffer *buffer, int sizeIncrement) {
    char *tmp;
    int newSize;
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_02225168, 0x20);
    ASSERT(sizeIncrement > 0, ov38_022251EC, ov38_022251DC, ov38_02225168, 0x21);
    if (!buffer) {
        return 0;
    }
    if (sizeIncrement <= 0) {
        return 0;
    }
    newSize = buffer->size + sizeIncrement;
    tmp = ov00_021EC3BC(buffer->data, newSize);
    if (!tmp) {
        return 0;
    }
    buffer->data = tmp;
    buffer->size = newSize;
    return 1;
}

int ghiInitBuffer(GHIConnection *connection, GHIBuffer *buffer, int initialSize, int sizeIncrement) {
    ASSERT(connection, ov38_02225200, ov38_022251DC, ov38_02225148, 0x43);
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_02225148, 0x44);
    ASSERT(initialSize > 0, ov38_0222520C, ov38_022251DC, ov38_02225148, 0x45);
    ASSERT(sizeIncrement > 0, ov38_022251EC, ov38_022251DC, ov38_02225148, 0x46);
    if (!connection) {
        return 0;
    }
    if (!buffer) {
        return 0;
    }
    if (initialSize <= 0) {
        return 0;
    }
    if (sizeIncrement <= 0) {
        return 0;
    }
    buffer->connection = connection;
    buffer->data = NULL;
    buffer->size = 0;
    buffer->len = 0;
    buffer->pos = 0;
    buffer->sizeIncrement = sizeIncrement;
    buffer->fixed = 0;
    buffer->dontFree = 0;
    buffer->encrypted = 0;
    if (!ghiResizeBuffer(buffer, initialSize)) {
        return 0;
    }
    buffer->data[0] = '\0';
    return 1;
}

int ghiInitFixedBuffer(GHIConnection *connection, GHIBuffer *buffer, char *userBuffer, int size) {
    ASSERT(connection, ov38_02225200, ov38_022251DC, ov38_02225178, 0x74);
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_02225178, 0x75);
    ASSERT(userBuffer, ov38_0222521C, ov38_022251DC, ov38_02225178, 0x76);
    ASSERT(size > 0, ov38_02225228, ov38_022251DC, ov38_02225178, 0x77);
    if (!connection) {
        return 0;
    }
    if (!buffer) {
        return 0;
    }
    if (!userBuffer) {
        return 0;
    }
    if (size <= 0) {
        return 0;
    }
    buffer->connection = connection;
    buffer->data = userBuffer;
    buffer->size = size;
    buffer->len = 0;
    buffer->sizeIncrement = 0;
    buffer->fixed = 1;
    buffer->dontFree = 1;
    buffer->encrypted = 0;
    buffer->data[0] = '\0';
    return 1;
}

void ghiFreeBuffer(GHIBuffer *buffer) {
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, _02225138, 0x9b);
    if (!buffer || !buffer->data) {
        return;
    }
    if (!buffer->dontFree) {
        ov00_021EC3D8(buffer->data);
    }
    memset(buffer, 0, sizeof(GHIBuffer));
}

int ghiAppendDataToBuffer(GHIBuffer *buffer, const char *data, int len) {
    GHIConnection *connection = buffer->connection;
    int newLen;
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_022251A4, 0xb7);
    ASSERT(data, ov38_02225234, ov38_022251DC, ov38_022251A4, 0xb8);
    ASSERT(len >= 0, ov38_0222523C, ov38_022251DC, ov38_022251A4, 0xb9);
    if (!buffer) {
        return 0;
    }
    if (!data) {
        return 0;
    }
    if (len < 0) {
        return 0;
    }
    if (len == 0) {
        len = strlen(data);
    }
    if (buffer->encrypted == 1) {
        int result;
        ASSERT(connection->encryptor.engine, ov38_0222524C, ov38_022251DC, ov38_022251A4, 0xce);
        do {
            int outLen = buffer->size - buffer->len;
            result = connection->encryptor.encryptFunc(connection, &connection->encryptor, data, &len, buffer->data + buffer->len, &outLen);
            if (result == 2) {
                if (buffer->fixed) {
                    buffer->connection->completed = 1;
                    buffer->connection->result = 2;
                    return 0;
                }
                if (ghiResizeBuffer(buffer, buffer->sizeIncrement)) {
                    buffer->connection->completed = 1;
                    buffer->connection->result = 1;
                    return 0;
                }
            } else {
                buffer->len += outLen;
            }
        } while (result == 2);
    } else {
        newLen = buffer->len + len;
        while (newLen >= buffer->size) {
            if (buffer->fixed) {
                buffer->connection->completed = 1;
                buffer->connection->result = 2;
                return 0;
            }
            if (!ghiResizeBuffer(buffer, buffer->sizeIncrement)) {
                buffer->connection->completed = 1;
                buffer->connection->result = 1;
                return 0;
            }
        }
        memcpy(buffer->data + buffer->len, data, len);
        buffer->len = newLen;
        buffer->data[newLen] = '\0';
    }
    return 1;
}

int ov38_0221C5EC(GHIBuffer *buffer, const char *name, const char *value) {
    if (!ghiAppendDataToBuffer(buffer, name, 0)) {
        return 0;
    }
    if (!ghiAppendDataToBuffer(buffer, ov38_02225288, 2)) {
        return 0;
    }
    if (!ghiAppendDataToBuffer(buffer, value, 0)) {
        return 0;
    }
    if (!ghiAppendDataToBuffer(buffer, ov38_0222528C, 2)) {
        return 0;
    }
    return 1;
}

int ghiAppendCharToBuffer(GHIBuffer *buffer, char c) {
    char ch = c;
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_0222518C, 0x131);
    if (!buffer) {
        return 0;
    }
    return ghiAppendDataToBuffer(buffer, &ch, 1);
}

int ov38_0221C6CC(GHIBuffer *buffer, int i) {
    char buf[16];
    sprintf(buf, ov38_02225290, i);
    return ghiAppendDataToBuffer(buffer, buf, 0);
}

void ghiResetBuffer(GHIBuffer *buffer) {
    ASSERT(buffer, ov38_022251D4, ov38_022251DC, ov38_02225158, 0x16c);
    buffer->len = 0;
    buffer->pos = 0;
    buffer->data[0] = '\0';
}

int ov38_0221C748(GHIConnection *c) {
    int writeFlag;
    int exceptFlag;
    int n;
    do {
        if (ov00_021FF59C(c->socket, 0, &writeFlag, &exceptFlag) == -1 || exceptFlag) {
            c->completed = 1;
            c->result = 5;
            c->socketError = ov00_021FFA60(c->socket);
            return 0;
        }
        if (!writeFlag) {
            return 1;
        }
        n = ov38_0221CD90(c, c->sendBuffer.data + c->sendBuffer.pos, c->sendBuffer.len - c->sendBuffer.pos);
        if (n == -1) {
            return 0;
        }
        c->sendBuffer.pos += n;
    } while (c->sendBuffer.pos < c->sendBuffer.len);
    return 1;
}

int ghiReadDataFromBuffer(GHIBuffer *buffer, char *out, int *len) {
    int n;
    int avail;
    ASSERT(buffer, ov38_02225294, ov38_022251DC, ov38_022251BC, 0x1b7);
    ASSERT(len, ov38_022252A8, ov38_022251DC, ov38_022251BC, 0x1b8);
    n = *len;
    if (n == 0) {
        return 0;
    }
    avail = buffer->len - buffer->pos;
    if (avail <= 0) {
        return 0;
    }
    if (n >= avail) {
        n = avail;
    }
    memcpy(out, buffer->data + buffer->pos, n);
    out[n] = '\0';
    *len = n;
    buffer->pos += n;
    return 1;
}

void ghiCallCompletedCallback(GHIConnection *c) {
    char *buffer;
    int len;
    int r;
    ASSERT(c, ov38_022252FC, ov38_02225308, ov38_022252E0, 0x1b);
    if (!c->completedCallback) {
        return;
    }
    if (c->type) {
        buffer = NULL;
        len = 0;
    } else {
        buffer = c->getFileBuffer.data;
        len = c->fileBytesReceived;
    }
    r = c->completedCallback(c->request, c->result, buffer, len, c->callbackParam);
    if (buffer && !r) {
        c->getFileBuffer.dontFree = 1;
    }
}

void ghiCallProgressCallback(GHIConnection *c, const char *buffer, int len) {
    ASSERT(c, ov38_022252FC, ov38_02225308, ov38_022252C8, 0x45);
    if (!c->progressCallback) {
        return;
    }
    c->progressCallback(c->request, c->state, buffer, len, c->fileBytesReceived, c->totalSize, c->callbackParam);
}

void ghiCallPostCallback(GHIConnection *c) {
    ASSERT(c, ov38_022252FC, ov38_02225308, ov38_022252B4, 0x5e);
    if (!c->postCallback) {
        return;
    }
    c->postCallback(c->request, c->bytesPosted, c->totalBytes, c->index, ArrayLength(c->states), c->callbackParam);
}

void ov38_0221CA44(void) {
}

void ov38_0221CA48(void) {
}

void ov38_0221CA4C(void) {
}

void ov38_0221CA50(void) {
}

int ov38_0221CA54(GHIConnection *c) {
    int inLen = 0;
    int outLen = 0;
    int result;
    int remain;
    char *inData;
    char *outData;
    do {
        inData = c->recvBuffer.data + c->recvBuffer.pos;
        inLen = c->recvBuffer.len - c->recvBuffer.pos;
        outData = c->encodeBuffer.data + c->encodeBuffer.len;
        outLen = c->encodeBuffer.size - c->encodeBuffer.len;
        result = c->encryptor.decryptFunc(c, &c->encryptor, inData, &inLen, outData, &outLen);
        if (result == 2) {
            if (!ghiResizeBuffer(&c->encodeBuffer, c->encodeBuffer.sizeIncrement)) {
                return 0;
            }
        }
    } while (result == 2 && outLen == 0);
    c->recvBuffer.pos += inLen;
    c->encodeBuffer.len += outLen;
    if (c->recvBuffer.pos > 0xff) {
        remain = c->recvBuffer.len - c->recvBuffer.pos;
        if (remain == 0) {
            ghiResetBuffer(&c->recvBuffer);
        } else {
            memmove(c->recvBuffer.data, c->recvBuffer.data + c->recvBuffer.pos, remain);
            c->recvBuffer.pos = 0;
            c->recvBuffer.len = remain;
        }
    }
    if (result == 3) {
        c->completed = 1;
        c->result = 0x11;
        return 0;
    }
    return 1;
}

int ov38_0221CB7C(GHIConnection *c, char *buffer, int *bufferLen) {
    int rcode;
    int len = *bufferLen - 1;
    if (c->throttle) {
        u32 now = ov00_021FF368();
        if (now < c->lastThrottleRecv + ov38_0222531C.a) {
            return 1;
        }
        c->lastThrottleRecv = now;
        if (len >= ov38_0222531C.b) {
            len = ov38_0222531C.b;
        }
    }
    if (c->encodeBuffer.pos < c->encodeBuffer.len) {
        ghiReadDataFromBuffer(&c->encodeBuffer, buffer, bufferLen);
        if (c->encodeBuffer.pos == c->encodeBuffer.len) {
            c->encodeBuffer.len = c->fF8;
            c->encodeBuffer.pos = c->fF8;
        }
        return 0;
    }
    rcode = ov00_021FF92C(c->socket, buffer, len, 0);
    if (rcode == -1) {
        int err = ov00_021FFA60(c->socket);
        if (err == -6 || err == -26 || err == -76) {
            return 1;
        }
        c->completed = 1;
        c->result = 5;
        c->socketError = err;
        c->f130 = 1;
        return 3;
    }
    if (rcode == 0) {
        c->f130 = 1;
        return 2;
    }
    if (c->encryptor.engine) {
        if (!ghiAppendDataToBuffer(&c->recvBuffer, buffer, rcode)) {
            return 3;
        }
        if (!ov38_0221CA54(c)) {
            c->completed = 1;
            c->result = 0x11;
            return 3;
        }
        if (c->encodeBuffer.len - c->encodeBuffer.pos <= 0) {
            buffer[0] = '\0';
            *bufferLen = 0;
            return 1;
        }
        rcode = *bufferLen - 1;
        if (!ghiReadDataFromBuffer(&c->encodeBuffer, buffer, &rcode)) {
            return 3;
        }
        if (c->encodeBuffer.pos == c->encodeBuffer.len) {
            c->encodeBuffer.len = c->fF8;
            c->encodeBuffer.pos = c->fF8;
        }
        if (rcode <= 0) {
            return 1;
        }
    }
    buffer[rcode] = '\0';
    *bufferLen = rcode;
    if (rcode <= 0) {
        return 1;
    }
    return 0;
}

int ov38_0221CD90(GHIConnection *c, const char *buf, int len) {
    int n = ov00_021FF970(c->socket, buf, len, 0);
    if (n == -1) {
        int err = ov00_021FFA60(c->socket);
        if (err == -6 || err == -26 || err == -76) {
            return 0;
        }
        c->completed = 1;
        c->result = 5;
        c->socketError = err;
        return -1;
    }
    if (c->state == 4) {
        c->bytesPosted += n;
    }
    return n;
}

int ov38_0221CE0C(GHIConnection *c, const char *data, int len) {
    int n = 0;
    if (c->sendBuffer.len == 0) {
        n = ov38_0221CD90(c, data, len);
        if (n == -1) {
            return 0;
        }
        if (n == len) {
            return 1;
        }
    }
    if (!ghiAppendDataToBuffer(&c->sendBuffer, data + n, len - n)) {
        return 0;
    }
    return 2;
}

int ghiFindFreeSlot(void) {
    int i;
    int oldCap;
    int newCap;
    GHIConnection **tmp;
    for (i = 0; i < ov38_0222602C.capacity; i++) {
        if (!ov38_0222602C.list[i]->inUse) {
            return i;
        }
    }
    ASSERT(ov38_0222602C.count == ov38_0222602C.capacity, ov38_02225378, ov38_022253A0, ov38_02225324, 0x33);
    oldCap = ov38_0222602C.capacity;
    newCap = ov38_0222602C.capacity + 4;
    tmp = ov00_021EC3BC(ov38_0222602C.list, newCap * sizeof(GHIConnection *));
    if (!tmp) {
        return -1;
    }
    ov38_0222602C.list = tmp;
    for (i = oldCap; i < newCap; i++) {
        ov38_0222602C.list[i] = ov00_021EC3A8(sizeof(GHIConnection));
        if (!ov38_0222602C.list[i]) {
            for (i--; i >= oldCap; i--) {
                ov00_021EC3D8(ov38_0222602C.list[i]);
            }
            return -1;
        }
        ov38_0222602C.list[i]->inUse = 0;
    }
    ov38_0222602C.capacity = newCap;
    return oldCap;
}

GHIConnection *ov38_0221CF8C(void) {
    int slot;
    GHIConnection *c;
    int ok;
    ov38_0221CA4C();
    slot = ghiFindFreeSlot();
    if (slot == -1) {
        ov38_0221CA50();
        return NULL;
    }
    c = ov38_0222602C.list[slot];
    memset(c, 0, sizeof(GHIConnection));
    c->inUse = 1;
    c->request = slot;
    c->uniqueID = ov38_0222602C.nextUniqueID++;
    c->type = 0;
    c->state = 0;
    c->URL = NULL;
    c->serverAddress = NULL;
    c->serverIP = 0;
    c->serverPort = 0;
    c->requestPath = NULL;
    c->sendHeaders = NULL;
    c->f2C = 0;
    c->blocking = 0;
    c->f34 = 0;
    c->result = 0;
    c->progressCallback = NULL;
    c->completedCallback = NULL;
    c->callbackParam = NULL;
    c->socket = -1;
    c->socketError = 0;
    c->userBufferSupplied = 0;
    c->fE4 = 0;
    c->fE8 = 0;
    c->statusCode = 0;
    c->fF0 = 0;
    c->fF4 = 0;
    c->fF8 = 0;
    c->completed = 0;
    c->fileBytesReceived = 0;
    c->totalSize = -1;
    c->redirectURL = NULL;
    c->redirectCount = 0;
    c->f110 = 0;
    c->processing = 0;
    c->throttle = 0;
    c->lastThrottleRecv = 0;
    c->post = NULL;
    c->f158 = 0x1f4;
    c->f160 = 0x50;
    c->f15C = NULL;
    c->encryptor.f00 = 0;
    ok = ghiInitBuffer(c, &c->sendBuffer, 0x800, 0x1000);
    if (ok) {
        ok = ghiInitBuffer(c, &c->encodeBuffer, 0x800, 0x800);
    }
    if (ok) {
        ok = ghiInitBuffer(c, &c->recvBuffer, 0x800, 0x400);
    }
    if (!ok) {
        ghiFreeConnection(c);
        ov38_0221CA50();
        return NULL;
    }
    ov38_0222602C.count++;
    ov38_0221CA50();
    return c;
}

int ghiFreeConnection(GHIConnection *c) {
    ASSERT(c, ov38_022253B4, ov38_022253A0, ov38_02225334, 0xb3);
    ASSERT(c->request >= 0, ov38_022253C0, ov38_022253A0, ov38_02225334, 0xb4);
    ASSERT(c->request < ov38_0222602C.capacity, ov38_022253DC, ov38_022253A0, ov38_02225334, 0xb5);
    ASSERT(c->inUse, ov38_02225404, ov38_022253A0, ov38_02225334, 0xb6);
    if (!c) {
        return 0;
    }
    if (!c->inUse) {
        return 0;
    }
    if (c->request < 0) {
        return 0;
    }
    if (c->request >= ov38_0222602C.capacity) {
        return 0;
    }
    ov38_0221CA4C();
    ov00_021EC3D8(c->URL);
    ov00_021EC3D8(c->serverAddress);
    ov00_021EC3D8(c->requestPath);
    ov00_021EC3D8(c->sendHeaders);
    ov00_021EC3D8(c->redirectURL);
    ov00_021EC3D8(c->f15C);
    if (c->socket != -1) {
        ov00_021FF82C(c->socket, 2);
        ov00_021FF818(c->socket);
    }
    ghiFreeBuffer(&c->sendBuffer);
    ghiFreeBuffer(&c->encodeBuffer);
    ghiFreeBuffer(&c->recvBuffer);
    ghiFreeBuffer(&c->getFileBuffer);
    if (c->states) {
        ov38_0221E0DC(c);
    }
    if (c->post) {
        if (ov38_0221DB00(c->post)) {
            ov38_0221DB08(c->post);
            c->post = NULL;
        }
    }
    if (c->encryptor.initialized) {
        if (c->encryptor.cleanupFunc) {
            c->encryptor.cleanupFunc(c, &c->encryptor);
        }
        c->encryptor.initialized = 0;
    }
    c->inUse = 0;
    ov38_0222602C.count--;
    ov38_0221CA50();
    return 1;
}

GHIConnection *ghiRequestToConnection(int request) {
    GHIConnection *c;
    ASSERT(request >= 0, ov38_02225418, ov38_022253A0, ov38_02225360, 0x101);
    ASSERT(request < ov38_0222602C.capacity, ov38_02225428, ov38_022253A0, ov38_02225360, 0x102);
    ov38_0221CA4C();
    if (request < 0 || request >= ov38_0222602C.capacity) {
        ov38_0221CA50();
        return NULL;
    }
    c = ov38_0222602C.list[request];
    if (!c->inUse) {
        c = NULL;
    }
    ov38_0221CA50();
    return c;
}

void ov38_0221D3B0(void (*func)(GHIConnection *)) {
    int i;
    if (ov38_0222602C.count <= 0) {
        return;
    }
    ov38_0221CA4C();
    for (i = 0; i < ov38_0222602C.capacity; i++) {
        GHIConnection *c = ov38_0222602C.list[i];
        if (c->inUse) {
            func(c);
        }
    }
    ov38_0221CA50();
}

void ov38_0221D414(GHIConnection *c) {
    ASSERT(c, ov38_022253B4, ov38_022253A0, ov38_02225348, 0x132);
    ASSERT(c->redirectURL, ov38_02225444, ov38_022253A0, ov38_02225348, 0x133);
    c->state = 0;
    ov00_021EC3D8(c->URL);
    c->URL = c->redirectURL;
    c->redirectURL = NULL;
    ov00_021EC3D8(c->serverAddress);
    c->serverAddress = NULL;
    c->serverIP = 0;
    c->serverPort = 0;
    ov00_021EC3D8(c->requestPath);
    c->requestPath = NULL;
    ov00_021FF82C(c->socket, 2);
    ov00_021FF818(c->socket);
    c->socket = -1;
    ghiResetBuffer(&c->sendBuffer);
    ghiResetBuffer(&c->encodeBuffer);
    ghiResetBuffer(&c->recvBuffer);
    c->fE4 = 0;
    c->fE8 = 0;
    c->statusCode = 0;
    c->fF0 = 0;
    c->fF4 = 0;
    c->fF8 = 0;
    c->f130 = 0;
    c->redirectCount++;
}

void ov38_0221D518(void) {
    int i;
    if (!ov38_0222602C.list) {
        return;
    }
    ov38_0221D3B0((void (*)(GHIConnection *))ghiFreeConnection);
    for (i = 0; i < ov38_0222602C.capacity; i++) {
        ov00_021EC3D8(ov38_0222602C.list[i]);
    }
    ov00_021EC3D8(ov38_0222602C.list);
    ov38_0222602C.list = NULL;
    ov38_0222602C.capacity = 0;
    ov38_0222602C.count = 0;
}

void ov38_0221D590(GHIConnection *c) {
    switch (c->statusCode / 100) {
    case 1:
    case 2:
    case 3:
        return;
    case 4:
        switch (c->statusCode) {
        case 401:
            c->result = 9;
            return;
        case 403:
            c->result = 10;
            return;
        case 404:
        case 410:
            c->result = 11;
            return;
        default:
            c->result = 8;
            return;
        }
    case 5:
        c->result = 12;
        return;
    }
}

int ghiProcessConnection(GHIConnection *c) {
    int completed;
    ASSERT(c, ov38_02225480, ov38_0222548C, ov38_02225468, 0x5b);
    ASSERT(ghiRequestToConnection(c->request) == c, ov38_02225498, ov38_0222548C, ov38_02225468, 0x5c);
    if (c->processing) {
        return 0;
    }
    c->processing = 1;
    if (c->state == 0) {
        ov38_0221EBB0(c);
    }
    if (c->state == 1) {
        ov38_0221EC74(c);
    }
    if (c->state == 2) {
        ghiDoSecuringSession(c);
    }
    if (c->state == 3) {
        ov38_0221F024(c);
    }
    if (c->state == 4) {
        ghiDoPosting(c);
    }
    if (c->state == 5) {
        ov38_0221F330(c);
    }
    if (c->state == 6) {
        ov38_0221F4FC(c);
    }
    if (c->state == 7) {
        ov38_0221FA38(c);
    }
    if (c->state == 8) {
        ov38_0221FEA8(c);
    }
    if (c->redirectURL) {
        ov38_0221D414(c);
    }
    completed = c->completed;
    if (completed) {
        ov38_0221D590(c);
        ghiCallCompletedCallback(c);
        ghiFreeConnection(c);
    } else {
        c->processing = 0;
    }
    return completed;
}

void ov38_0221D7B4(void) {
    ov38_0221CA4C();
    if (++ov38_0222603C == 1) {
        ov38_0221CA44();
        ov38_02225320 = 0x7d;
        ov38_0222531C.a = 0xfa;
        return;
    }
    ov38_0221CA50();
}

void ov38_0221D808(void) {
    ov38_0221CA4C();
    if (--ov38_0222603C == 0) {
        ov38_0221D518();
        if (ov38_02226028) {
            ov00_021EC3D8(ov38_02226028);
            ov38_02226028 = NULL;
        }
        ov38_0221CA50();
        ov38_0221CA48();
        return;
    }
    ov38_0221CA50();
}

int ov38_0221D864(const char *url, int blocking, CompletedCallback completed, void *param) {
    return ghttpGetExA(url, NULL, NULL, 0, NULL, 0, blocking, NULL, completed, param);
}

int ghttpGetExA(const char *url, const char *headers, char *buffer, int bufferSize, GHIPost *post, int throttle, int blocking, ProgressCallback progress, CompletedCallback completed, void *param) {
    GHIConnection *c;
    int ok;
    ASSERT(url && *url, ov38_022254D4, ov38_0222548C, ov38_0222545C, 0x128);
    ASSERT(bufferSize >= 0, ov38_022254E4, ov38_0222548C, ov38_0222545C, 0x129);
    ASSERT(!buffer || bufferSize, ov38_022254F4, ov38_0222548C, ov38_0222545C, 0x12a);
    if (!url || !*url) {
        return -1;
    }
    if (bufferSize < 0) {
        return -1;
    }
    if (buffer && !bufferSize) {
        return -1;
    }
    if (!ov38_0222603C) {
        ov38_0221D7B4();
    }
    c = ov38_0221CF8C();
    if (!c) {
        return -1;
    }
    c->type = 0;
    c->URL = ov00_021FF3D4(url);
    if (!c->URL) {
        ghiFreeConnection(c);
        return -1;
    }
    if (headers && *headers) {
        c->sendHeaders = ov00_021FF3D4(headers);
        if (!c->sendHeaders) {
            ghiFreeConnection(c);
            return -1;
        }
    }
    c->post = post;
    c->blocking = blocking;
    c->progressCallback = progress;
    c->completedCallback = completed;
    c->callbackParam = param;
    c->throttle = throttle;
    c->userBufferSupplied = buffer != NULL;
    if (c->userBufferSupplied) {
        ok = ghiInitFixedBuffer(c, &c->getFileBuffer, buffer, bufferSize);
    } else {
        ok = ghiInitBuffer(c, &c->getFileBuffer, 0x800, 0x800);
    }
    if (!ok) {
        ghiFreeConnection(c);
        return -1;
    }
    if (post) {
        if (!ghiPostInitState(c)) {
            ghiFreeConnection(c);
            return -1;
        }
    }
    if (blocking) {
        while (!ghiProcessConnection(c)) {
            ov00_021FF3C0(10);
        }
        return 0;
    }
    return c->request;
}

void ov38_0221DAEC(void) {
    ov38_0221D3B0((void (*)(GHIConnection *))ghiProcessConnection);
}

int ov38_0221DB00(GHIPost *post) {
    return post->autoFree;
}

void ov38_0221DB08(GHIPost *post) {
    ov00_021FE678(post->data);
    ov00_021EC3D8(post);
}

char *ghiPostGetContentType(GHIConnection *c) {
    GHIPost *post = c->post;
    ASSERT(post, ov38_02225694, ov38_02225644, ov38_02225574, 0x192);
    if (!post) {
        return ov38_0222569C;
    }
    if (post->hasFiles) {
        return ov38_022256A0;
    }
    return ov38_022256E4;
}

int ghiPostGetNoFilesContentLength(GHIConnection *c) {
    GHIPost *post = c->post;
    GHIPostData *d;
    int i;
    int num;
    int total = 0;
    num = ArrayLength(post->data);
    if (!num) {
        return 0;
    }
    for (i = 0; i < num; i++) {
        d = ov00_021FE6EC(post->data, i);
        ASSERT(d->type == 0, ov38_02225708, ov38_02225644, ov38_022255E0, 0x1b0);
        total += strlen(d->name);
        total += d->f0c;
        total += d->f14 * 2;
        total++;
    }
    total += num - 1;
    return total;
}

int ghiPostGetHasFilesContentLength(GHIConnection *c) {
    GHIPost *post = c->post;
    GHIPostData *d;
    int i;
    int num;
    int total = 0;
    GHIPostState *st;
    if (!ov38_02226040.f8) {
        ov38_02226040.f8 = strlen(ov38_02225720);
        ov38_02226040.f4 = ov38_02226040.f8 + 0x2f;
        ov38_02226040.fC = ov38_02226040.f8 + 0x4c;
        ov38_02226040.f0 = ov38_02226040.f8 + 4;
    }
    num = ArrayLength(post->data);
    for (i = 0; i < num; i++) {
        d = ov00_021FE6EC(post->data, i);
        if (d->type == 0) {
            total += ov38_02226040.f4;
            total += strlen(d->name);
            total += d->f0c;
        } else if (d->type == 1) {
            total += ov38_02226040.fC;
            total += strlen(d->name);
            total += strlen((char *)d->f0c);
            total += strlen(d->f10);
            st = ov00_021FE6EC(c->states, i);
            ASSERT(st, ov38_02225748, ov38_02225644, ov38_02225620, 0x1ec);
            total += st->len;
        } else if (d->type == 2) {
            total += ov38_02226040.fC;
            total += strlen(d->name);
            total += strlen(d->f10);
            total += strlen((char *)d->f14);
            total += d->f0c;
        } else {
            ASSERT(0, ov38_02225640, ov38_02225644, ov38_02225620, 0x1f9);
            return 0;
        }
    }
    return total + ov38_02226040.f0;
}

int ghiPostGetContentLength(GHIConnection *c) {
    GHIPost *post = c->post;
    ASSERT(post, ov38_02225694, ov38_02225644, ov38_0222558C, 0x20c);
    if (!post) {
        return 0;
    }
    if (post->hasFiles) {
        return ghiPostGetHasFilesContentLength(c);
    }
    return ghiPostGetNoFilesContentLength(c);
}

int ghiPostStateInit(GHIPostState *s) {
    int type = s->data->type;
    s->pos = -1;
    if (type == 0) {
    } else if (type == 1) {
        if (!s->file) {
            return 0;
        }
        if (fseek(s->file, 0, 2)) {
            return 0;
        }
        s->len = ftell(s->file);
        if (s->len == -1) {
            return 0;
        }
        rewind(s->file);
    } else if (type != 2) {
        ASSERT(0, ov38_02225640, ov38_02225644, ov38_02225534, 0x244);
        return 0;
    }
    return 1;
}

void ghiPostStateCleanup(GHIPostState *s) {
    int type = s->data->type;
    if (type == 0) {
    } else if (type == 1) {
        if (s->file) {
            fclose(s->file);
        }
        s->file = NULL;
    } else if (type != 2) {
        ASSERT(0, ov38_02225640, ov38_02225644, ov38_02225548, 0x269);
    }
}

int ghiPostInitState(GHIConnection *c) {
    int i;
    int num;
    GHIPostState state;
    GHIPostData *d;
    ASSERT(c->post, ov38_02225750, ov38_02225644, ov38_0222550C, 0x278);
    if (!c->post) {
        return 0;
    }
    c->index = 0;
    c->bytesPosted = 0;
    c->totalBytes = 0;
    c->postCallback = c->post->callback;
    c->postParam = c->post->param;
    num = ArrayLength(c->post->data);
    c->states = ov00_021FE5B4(sizeof(GHIPostState), num, 0);
    if (!c->states) {
        return 0;
    }
    for (i = 0; i < num; i++) {
        d = ov00_021FE6EC(c->post->data, i);
        ZeroState(&state);
        state.data = d;
        if (!ghiPostStateInit(&state)) {
            for (i--; i >= 0; i--) {
                ghiPostStateCleanup(ov00_021FE6EC(c->states, i));
            }
            ov00_021FE678(c->states);
            c->states = NULL;
            return 0;
        }
        ov00_021FE754(c->states, &state);
    }
    ASSERT(ArrayLength(c->post->data) == ArrayLength(c->states), ov38_02225764, ov38_02225644, ov38_0222550C, 0x2b1);
    c->totalBytes = ghiPostGetContentLength(c);
    return 1;
}

void ov38_0221E0DC(GHIConnection *c) {
    int i;
    int num;
    if (c->states) {
        num = ArrayLength(c->states);
        for (i = 0; i < num; i++) {
            ghiPostStateCleanup(ov00_021FE6EC(c->states, i));
        }
        ov00_021FE678(c->states);
        c->states = NULL;
    }
    if (c->post && c->post->autoFree) {
        ov38_0221DB08(c->post);
        c->post = NULL;
    }
}
