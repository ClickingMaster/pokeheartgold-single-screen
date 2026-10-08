#pragma force_active on
#include "global.h"
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
    int inUse;             // 0x00
    int request;           // 0x04
    int uniqueID;          // 0x08
    int type;              // 0x0C
    int state;             // 0x10
    char *URL;             // 0x14
    char *serverAddress;   // 0x18
    int serverIP;          // 0x1C
    u16 serverPort;        // 0x20
    char *requestPath;     // 0x24
    char *sendHeaders;     // 0x28
    int f2C;               // 0x2C
    int blocking;          // 0x30
    int f34;               // 0x34
    int result;            // 0x38
    ProgressCallback progressCallback;   // 0x3C
    CompletedCallback completedCallback; // 0x40
    void *callbackParam;   // 0x44
    int socket;            // 0x48
    int socketError;       // 0x4C
    GHIBuffer sendBuffer;  // 0x50
    GHIBuffer encodeBuffer; // 0x74
    GHIBuffer recvBuffer;  // 0x98
    GHIBuffer getFileBuffer; // 0xBC
    int userBufferSupplied; // 0xE0
    int fE4;
    int fE8;
    int statusCode;        // 0xEC
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
    int processing;        // 0x12C
    int f130;
    int throttle;          // 0x134
    int lastThrottleRecv;  // 0x138
    GHIPost *post;         // 0x13C
    void *states;          // 0x140
    int index;             // 0x144
    int bytesPosted;       // 0x148
    int totalBytes;        // 0x14C
    PostCallback postCallback; // 0x150
    void *postParam;       // 0x154
    int f158;
    char *f15C;
    u16 f160;
    GHIEncryptor encryptor; // 0x164
};

typedef struct HostEnt {
    char *h_name;
    char **h_aliases;
    s16 h_addrtype;
    s16 h_length;
    u8 **h_addr_list;
} HostEnt;
extern char *ov38_02226028;
void ghiCallProgressCallback(GHIConnection *c, const char *buffer, int len);
void ov00_021FF3CC(void);
int ghiParseURL(GHIConnection *c);
int ov00_021FFA44(const char *s);
HostEnt *SOC_GetHostByName(const char *name);
void ov38_0221EBB0(GHIConnection *c);
// @fn ov38_0221EBB0
void ov38_0221EBB0(GHIConnection *c) {
    char *server;
    ghiCallProgressCallback(c, NULL, 0);
    ov00_021FF3CC();
    if (!ghiParseURL(c)) {
        c->completed = 1;
        c->result = 3;
        return;
    }
    if (c->f15C != NULL) {
        server = c->f15C;
    } else if (ov38_02226028 != NULL) {
        server = ov38_02226028;
    } else {
        server = c->serverAddress;
    }
    c->serverIP = ov00_021FFA44(server);
    if (c->serverIP == -1) {
        HostEnt *he = SOC_GetHostByName(server);
        if (he == NULL) {
            c->completed = 1;
            c->result = 4;
            return;
        }
        c->serverIP = *(int *)he->h_addr_list[0];
    }
    c->state = 1;
    ghiCallProgressCallback(c, NULL, 0);
}
