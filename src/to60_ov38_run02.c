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

int ghiPostFileDiskStateDoPosting(GHIPostState *state, GHIConnection *c);

int ghiPostFileDiskStateDoPosting(GHIPostState *state, GHIConnection *c) {
    char buf[0x1000];
    int n;
    int r;
    ASSERT(state->pos >= 0, ov38_022257B8, ov38_02225644, ov38_022255C0, 0x336);
    ASSERT(state->pos < state->len, ov38_02225818, ov38_02225644, ov38_022255C0, 0x337);
    ASSERT(state->pos == ftell(state->file), ov38_02225840, ov38_02225644, ov38_022255C0, 0x338);
    do {
        n = fread(buf, 1, 0x1000, state->file);
        if (n <= 0) {
            c->completed = 1;
            c->result = 0xe;
            return 0;
        }
        state->pos += n;
        if (state->pos > state->len) {
            c->completed = 1;
            c->result = 0xe;
            return 0;
        }
        r = ov38_0221CE0C(c, buf, n);
        if (!r) {
            return 0;
        }
        if (state->pos == state->len) {
            return 1;
        }
    } while (r == 1);
    return 2;
}
