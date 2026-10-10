#pragma force_active on
#pragma warn_unusedvar off
#include "global.h"

#include "WIFI_stubsmd5_dummy.h"
#include "dgt.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*ArrayElementFreeFn)(void *);
typedef int (*ArrayCompareFn)(const void *, const void *);

typedef struct DArray {
    int count;
    int capacity;
    int elemSize;
    int growBy;
    ArrayElementFreeFn freeFn;
    void *list;
} DArray;

extern void __msl_assertion_failed(const char *cond, const char *file, const char *func, int line);
extern void *ov00_021EC3A8(u32 size);
extern void *ov00_021EC3BC(void *p, u32 size);
extern void ov00_021EC3D8(void *p);
extern void *memcpy(void *, const void *, u32);
extern void *memmove(void *, const void *, u32);
extern void qsort(void *, u32, u32, ArrayCompareFn);

extern char ov00_02217C58[];
extern char ov00_02217C64[];
extern char ov00_02217C70[];
extern char ov00_02217C7C[];
extern char ov00_02217C88[];
extern char ov00_02217C94[];
extern char ov00_02217CA4[];
extern char ov00_02217CB4[];
extern char ov00_02217CC4[];
extern char ov00_02217CE8[];
extern char ov00_02217D10[];
extern char ov00_02217D1C[];
extern char ov00_02217D28[];
extern char ov00_02217D30[];
extern char ov00_02217D3C[];
extern char ov00_02217D5C[];
extern char ov00_02217D7C[];

typedef void (*ArrayMapFn)(void *, void *);
typedef int (*ArrayMapFn2)(void *, void *);
typedef int (*TableHashFn)(const void *, int);

typedef struct HashTable {
    DArray **buckets;
    int nbuckets;
    ArrayElementFreeFn freeFn;
    TableHashFn hashFn;
    ArrayCompareFn cmpFn;
} HashTable;

typedef struct SocPollFd {
    int fd;
    s16 events;
    s16 revents;
} SocPollFd;

typedef struct DGTHash1Context Md5Ctx;

extern char ov00_02217C4C[];
extern char ov00_02217CD4[];
extern char ov00_02217CFC[];
extern char ov00_02217D88[];
extern char ov00_02217D8C[];
extern char ov00_02217D98[];
extern char ov00_02217DA4[];
extern char ov00_02217DB0[];
extern char ov00_02217DBC[];
extern char ov00_02217DC8[];
extern char ov00_02217DD4[];
extern char ov00_02217DE0[];
extern char ov00_02217DF0[];
extern char ov00_02217E00[];
extern char ov00_02217E08[];
extern char ov00_02217E14[];
extern char ov00_02217E1C[];
extern char ov00_02217E28[];
extern char ov00_02217E34[];
extern char ov00_02217E3C[];
extern char ov00_02217E4C[];
extern char ov00_02217E50[];
extern char ov00_02217E5C[];
extern char ov00_02217E64[];
extern char ov00_02217E74[];
extern char ov00_02217E94[];
extern unsigned char __lower_mapC[];

extern int sprintf(char *, const char *, ...);
extern int SOC_Fcntl(int, int, int);
extern int SOC_Poll(SocPollFd *, int, s64);

HashTable *ov00_021FED4C(int elemSize, int nbuckets, int nchains, TableHashFn hashFn, ArrayCompareFn cmpFn, ArrayElementFreeFn freeFn);
void ov00_021FE678(DArray *a);
int ArrayLength(const DArray *a);
void ov00_021FE754(DArray *a, const void *elem);
void ov00_021FE968(DArray *a, int n);
void ov00_021FE9C0(DArray *a, const void *elem, int n);
int ov00_021FEA40(DArray *a, const void *key, ArrayCompareFn cmp, int from, int sorted);
void ov00_021FEAF8(DArray *a, ArrayMapFn fn, void *arg);
void ov00_021FEB64(DArray *a, ArrayMapFn fn, void *arg);
void *ov00_021FEBC4(DArray *a, ArrayMapFn2 fn, void *arg);
int ov00_021FF59C(int sock, int *readFlag, int *writeFlag, int *exceptFlag);
int ov00_021FF7EC(int ret, int err);
int ov00_021FF9DC(int sock, int level, int opt, void *val, int *len);
int ov00_021FFA04(int sock, int level, int opt, const void *val, int len);

static inline int ov00_tolower(int c) {
    if (c < 0 || c >= 0x80) {
        return c;
    }
    return __lower_mapC[c];
}

typedef struct SocSockAddr {
    u8 len;
    u8 family;
    u8 port[2];
    u8 addr[4];
} SocSockAddr;

typedef struct SockAddrIn {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} SockAddrIn;

typedef struct Ov00SocketState {
    int unk0;
    int lastError;
    char *hName;
    char **hAliases;
    s16 hAddrType;
    s16 hLength;
    char **hAddrList;
    u32 addr;
    u8 pad1C[0x2C - 0x1C];
    char *addrList[2];
} Ov00SocketState;

extern char ov00_02217EA0[];
extern char ov00_0221B1B4[];
extern char ov00_0221B1C4[];
extern char *ov00_0221B1D8[];
extern u32 SOCL_GetHostID(void);
extern void SOC_U32to4U8(u32, void *);

extern Ov00SocketState ov00_0221B1AC;
extern u32 ov00_02217E58;

extern int SOC_Socket(int, int, int);
extern int SOC_Close(int);
extern int SOC_Shutdown(int, int);
extern int SOC_Bind(int, SocSockAddr *);
extern int SOC_Connect(int, SocSockAddr *);
extern int SOC_Listen(int, int);
extern int SOC_Accept(int, SocSockAddr *);
extern int SOC_Recv(int, void *, int, int);
extern int SOC_RecvFrom(int, void *, int, int, SocSockAddr *);
extern int SOC_Send(int, const void *, int, int);
extern int SOC_SendTo(int, const void *, int, int, SocSockAddr *);
extern int SOC_GetSockName(int, SocSockAddr *);
extern int SOC_InetAtoN(const char *, u32 *);
u32 ov00_021FFAD4(u32 x);
u32 ov00_021FFB20(void);

typedef struct GPICallback {
    void *callback;
    void *param;
} GPICallback;

typedef struct GPIBuffer {
    u8 data[0x20];
} GPIBuffer;

typedef struct GPIConnection {
    char errorString[0x100];
    int unk100;
    int unk104;
    int simulation;
    u8 pad10C[0x198 - 0x10C];
    int sessKey;
    u8 pad19C[0x1A4 - 0x19C];
    GPICallback callbacks[6];
    int unk1D4;
    int connectState;
    u8 pad1DC[0x1F4 - 0x1DC];
    GPIBuffer outputBuffer;
    int unk214;
    char unk218[0x100];
    char unk318[0x100];
    u8 pad418[0x430 - 0x418];
    int numBuddies;
} GPIConnection;

typedef GPIConnection *GPConnection;
typedef void (*GPCallback)(GPConnection *, void *, void *);

typedef struct GPIBuddyStatus {
    int buddyIndex;
    int status;
    char *statusString;
    char *locationString;
    int ip;
    int port;
} GPIBuddyStatus;

typedef struct GPIProfile {
    int profileId;
    int unk4;
    GPIBuddyStatus *buddyStatus;
    int unkC;
    void *unk10;
    int unk14;
} GPIProfile;

typedef struct GPBuddyStatus {
    int profile;
    int status;
    char statusString[0x100];
    char locationString[0x100];
    int ip;
    int port;
} GPBuddyStatus;

typedef struct GPConnectResponseArg {
    int result;
    int profile;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
} GPConnectResponseArg;

typedef struct GPResultArg {
    int unk0;
    int unk4;
    int result;
    int unkC;
} GPResultArg;

typedef struct GPBigArg {
    int data[0x81];
} GPBigArg;

typedef struct Ov00DwcState {
    int state;
} Ov00DwcState;

extern Ov00DwcState ov00_0221B0D0;
extern char ov00_02216140[];
extern char ov00_02216144[];
extern char ov00_02216148[];
extern char ov00_02217EAC[];
extern char ov00_02217EC0[];
extern char ov00_02217ED0[];
extern char ov00_02217EE0[];
extern char ov00_02217EE4[];
extern char ov00_02217F14[];
extern char ov00_02217F24[];
extern char ov00_02217F30[];
extern char ov00_02217F3C[];
extern char ov00_02217F4C[];
extern char ov00_02217F58[];
extern char ov00_02217F60[];
extern char ov00_02217F70[];
extern char ov00_02217F80[];
extern char ov00_02217F8C[];
extern char ov00_02217F94[];
extern char ov00_02217FAC[];
extern char ov00_02217FC4[];
extern char ov00_02217FD0[];
extern char ov00_02217FE0[];

extern int ov00_022008F4(int a, int b, int c);
extern void ov00_02200A3C(GPConnection *conn);
extern int ov00_02200A98(GPConnection *conn);
extern int ov00_02200ED8(GPConnection *conn, int b);
extern int ov00_022031E4(GPConnection *conn, const char *a, const char *b, const char *c, const char *d, const char *e, const char *f, int g, int h, int i, int j, GPCallback cb, void *param);
extern int ov00_02204284(GPConnection *conn, int b);
extern int ov00_02208330(GPConnection *conn, int a, int b, int c, int d, int e, int f, int g, int h, GPCallback cb, void *param);
extern int ov00_02205EE8(GPConnection *conn, int a, int b, int c, GPCallback cb, void *param);
extern int ov00_02205678(GPConnection *conn, int a, int b);
extern int ov00_02201CAC(GPConnection *conn, int a);
extern int ov00_02201D9C(GPConnection *conn, int a);
extern int ov00_02207DD0(GPConnection *conn, int profile, GPIProfile **out);
extern GPIProfile *ov00_02207F88(GPConnection *conn, int index);
extern int ov00_02207FB8(GPIProfile *profile);
extern void ov00_02207E40(GPConnection *conn, GPIProfile *profile);
extern int ov00_022020EC(GPConnection *conn, GPIBuffer *buf, const char *str);
extern int ov00_0220211C(GPConnection *conn, GPIBuffer *buf, int val);
extern char *ov00_0220A348(char *dst, const char *src, int len);
extern void ov00_0220A890(GPConnection *conn, const char *str);
extern void *memset(void *, int, u32);
extern int strcmp(const char *, const char *);
void ov00_021FFB80(const char *in, char *out, int len);

static inline void ov00_fixSlashes(char *s) {
    char *p;
    if (*s) {
        p = s;
        do {
            if (*p == '\\') {
                *p = '/';
            }
        } while (*++p);
    }
}

void *ov00_021FE6EC(DArray *a, int n);
void ov00_021FE7A0(DArray *a, const void *elem, int n);
void ov00_021FE8D8(DArray *a, int n);
void *ov00_021FECC0(const void *key, const void *base, int count, int size, ArrayCompareFn cmp, int *found);
void *ov00_021FEC68(const void *key, const void *base, int count, int size, ArrayCompareFn cmp);

int ov00_021FFA18(int sock, SocSockAddr *addr, int *len);
u32 ov00_021FFA44(const char *s);
int ov00_021FFA60(void);
u32 ov00_021FFA70(u32 *out);
void ov00_021FFB40(u32 seed);
int ov00_021FFB5C(int lo, int hi);
void ov00_021FFC18(const char *in, char *out, int len, int type);
int ov00_021FFD50(int a, int b, int c);
void ov00_021FFD80(GPConnection *conn);
int ov00_021FFD9C(GPConnection *conn);
int ov00_021FFDD0(GPConnection *conn, int which, void *callback, void *param);
int ov00_021FFE20(GPConnection *conn, const char *nick, const char *email, const char *password, int firewall, GPCallback callback, void *param);
void ov00_021FFF24(GPConnection *conn);
int ov00_021FFF58(GPConnection *conn, int a, int b, int c, int d, int e, int f, int g, GPCallback callback, void *param);
int ov00_02200020(GPConnection *conn, int a, int b, int c, GPCallback callback, void *param);
int ov00_022000FC(GPConnection *conn, int a, int b);
int ov00_0220014C(GPConnection *conn, int profile, const char *reason);
int ov00_022002C0(GPConnection *conn, int a);
int ov00_02200310(GPConnection *conn, int profileId);
int ov00_022003D0(GPConnection *conn, int *num);
int ov00_02200400(GPConnection *conn, int index, GPBuddyStatus *status);
int ov00_02200540(GPConnection *conn, int profileId, int *index);
int ov00_022005B0(GPConnection *conn, int profileId);
int ov00_022005FC(GPConnection *conn, int a);
int ov00_02200654(GPConnection *conn, int status, const char *statusString, const char *locationString);

int ov00_021FF9DC(int sock, int level, int opt, void *val, int *len) {
    MI_CpuFill8(val, 0, *len);
    return ov00_021FF7EC(0, -1);
}

int ov00_021FFA04(int sock, int level, int opt, const void *val, int len) {
    return ov00_021FF7EC(0, -1);
}

int ov00_021FFA18(int sock, SocSockAddr *addr, int *len) {
    int ret;
    addr->len = *len;
    ret = SOC_GetSockName(sock, addr);
    *len = addr->len;
    return ov00_021FF7EC(ret, -1);
}

u32 ov00_021FFA44(const char *s) {
    u32 addr;
    if (SOC_InetAtoN(s, &addr) == 0) {
        return -1;
    }
    return addr;
}

int ov00_021FFA60(void) {
    return ov00_0221B1AC.lastError;
}

u32 ov00_021FFA70(u32 *out) {
    u32 t;
    if (OS_IsTickAvailable() != TRUE) {
        __msl_assertion_failed(ov00_02217E74, ov00_02217E94, ov00_02217E5C, 0x667);
    }
    t = (u32)((OS_GetTick() * 64) / 33514000);
    if (out) {
        *out = t;
    }
    return t;
}

u32 ov00_021FFAD4(u32 x) {
    u32 lo = (x & 0xffff) * 16807;
    u32 hi = (x >> 16) * 16807;
    lo += (hi & 0x7fff) << 16;
    if (lo > 0x7fffffff) {
        lo &= 0x7fffffff;
        lo++;
    }
    lo += hi >> 15;
    if (lo > 0x7fffffff) {
        lo &= 0x7fffffff;
        lo++;
    }
    return lo;
}

u32 ov00_021FFB20(void) {
    return ov00_02217E58 = ov00_021FFAD4(ov00_02217E58);
}

void ov00_021FFB40(u32 seed) {
    ov00_02217E58 = seed ? (seed & 0x7fffffff) : 1;
}

int ov00_021FFB5C(int lo, int hi) {
    int range = hi - lo;
    int r;
    if (range == 0) {
        return lo;
    }
    r = (int)ov00_021FFB20() % range;
    return r + lo;
}

void ov00_021FFB80(const char *in, char *out, int len) {
    u8 tmp[3];
    int i;
    for (i = 0; i < len; i++) {
        tmp[i] = in[i];
    }
    for (; i < 3; i++) {
        tmp[i] = 0;
    }
    out[0] = tmp[0] >> 2;
    out[1] = ((tmp[0] & 3) << 4) | (tmp[1] >> 4);
    out[2] = ((tmp[1] & 0xf) << 2) | (tmp[2] >> 6);
    out[3] = tmp[2] & 0x3f;
}

void ov00_021FFC18(const char *in, char *out, int len, int type) {
    char *tbl;
    char *start = out;
    int remaining = len;
    char *pad;
    int mod;
    int c;
    switch (type) {
    case 1:
        tbl = ov00_02216140;
        break;
    case 2:
        tbl = ov00_02216144;
        break;
    default:
        tbl = ov00_02216148;
        break;
    }
    while (remaining > 0) {
        ov00_021FFB80(in, out, remaining < 3 ? remaining : 3);
        remaining -= 3;
        out += 4;
        in += 3;
    }
    mod = len % 3;
    pad = out;
    if (mod == 1) {
        pad = out - 2;
    } else if (mod == 2) {
        pad = out - 1;
    }
    *out = 0;
    while (out > start) {
        out--;
        if (out >= pad) {
            *out = tbl[2];
        } else {
            c = *out;
            if (c <= 25) {
                *out = c + 'A';
            } else if (c <= 51) {
                *out = c + 'G';
            } else if (c <= 61) {
                *out = c - 4;
            } else if (c == 62) {
                *out = tbl[0];
            } else if (c == 63) {
                *out = tbl[1];
            }
        }
    }
}

int ov00_021FFD50(int a, int b, int c) {
    if (ov00_0221B0D0.state != 1) {
        return 2;
    }
    if (a == 0) {
        return 2;
    }
    return ov00_022008F4(a, b, c);
}

void ov00_021FFD80(GPConnection *conn) {
    if (!conn || !*conn) {
        return;
    }
    ov00_02200A3C(conn);
}

int ov00_021FFD9C(GPConnection *conn) {
    if (!conn || !*conn) {
        return 2;
    }
    if ((*conn)->simulation) {
        return 0;
    }
    return ov00_02200ED8(conn, 0);
}

int ov00_021FFDD0(GPConnection *conn, int which, void *callback, void *param) {
    GPIConnection *iconn;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (which < 0 || which >= 6) {
        ov00_0220A890(conn, ov00_02217EC0);
        return 2;
    }
    iconn->callbacks[which].callback = callback;
    iconn->callbacks[which].param = param;
    return 0;
}

int ov00_021FFE20(GPConnection *conn, const char *nick, const char *email, const char *password, int firewall, GPCallback callback, void *param) {
    GPIConnection *iconn;
    GPConnectResponseArg arg;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (!nick || !*nick) {
        return 2;
    }
    if (!email || !*email) {
        return 2;
    }
    if (!callback) {
        ov00_0220A890(conn, ov00_02217ED0);
        return 2;
    }
    if (iconn->simulation) {
        memset(&arg, 0, sizeof(arg));
        callback(conn, &arg, param);
        return 0;
    }
    return ov00_022031E4(conn, ov00_02217EE0, ov00_02217EE0, ov00_02217EE0, ov00_02217EE0, nick, email, 0, (int)password, 0, firewall, callback, param);
}

void ov00_021FFF24(GPConnection *conn) {
    if (!conn || !*conn) {
        return;
    }
    if ((*conn)->simulation) {
        return;
    }
    ov00_02204284(conn, 1);
    ov00_02200A98(conn);
}

int ov00_021FFF58(GPConnection *conn, int a, int b, int c, int d, int e, int f, int g, GPCallback callback, void *param) {
    GPIConnection *iconn;
    GPResultArg arg;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (!callback) {
        ov00_0220A890(conn, ov00_02217ED0);
        return 2;
    }
    if (iconn->simulation) {
        memset(&arg, 0, sizeof(arg));
        arg.result = 0x601;
        callback(conn, &arg, param);
        return 0;
    }
    return ov00_02208330(conn, a, b, c, d, e, f, 0, g, callback, param);
}

int ov00_02200020(GPConnection *conn, int a, int b, int c, GPCallback callback, void *param) {
    GPIConnection *iconn;
    if (!conn || !(iconn = *conn) || !a) {
        return 2;
    }
    if (!callback) {
        ov00_0220A890(conn, ov00_02217ED0);
        return 2;
    }
    if (iconn->simulation) {
        GPBigArg arg = { 0 };
        callback(conn, &arg, param);
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    return ov00_02205EE8(conn, a, b, c, callback, param);
}

int ov00_022000FC(GPConnection *conn, int a, int b) {
    GPIConnection *iconn;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    return ov00_02205678(conn, a, b);
}

int ov00_0220014C(GPConnection *conn, int profile, const char *reason) {
    GPIConnection *iconn;
    char buf[0x401];
    char *p;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    if (!reason) {
        ov00_0220A890(conn, ov00_02217F14);
        return 2;
    }
    ov00_0220A348(buf, reason, sizeof(buf));
    ov00_fixSlashes(buf);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F24);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F30);
    ov00_0220211C(conn, &iconn->outputBuffer, iconn->sessKey);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F3C);
    ov00_0220211C(conn, &iconn->outputBuffer, profile);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F4C);
    ov00_022020EC(conn, &iconn->outputBuffer, buf);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F58);
    return 0;
}

int ov00_022002C0(GPConnection *conn, int a) {
    GPIConnection *iconn;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    return ov00_02201CAC(conn, a);
}

int ov00_02200310(GPConnection *conn, int profileId) {
    GPIConnection *iconn;
    GPIProfile *profile;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    if (!ov00_02207DD0(conn, profileId, &profile)) {
        return 0;
    }
    profile->unk14--;
    if (!iconn->unk100 && profile->unk14 <= 0) {
        ov00_021EC3D8(profile->unk10);
        profile->unk10 = NULL;
        if (ov00_02207FB8(profile)) {
            ov00_02207E40(conn, profile);
        }
    }
    return 0;
}

int ov00_022003D0(GPConnection *conn, int *num) {
    GPIConnection *iconn;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        *num = 0;
        return 0;
    }
    *num = iconn->numBuddies;
    return 0;
}

int ov00_02200400(GPConnection *conn, int index, GPBuddyStatus *status) {
    GPIConnection *iconn;
    GPIProfile *profile;
    GPIBuddyStatus *bs;
    int num;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        memset(status, 0, sizeof(GPBuddyStatus));
        return 0;
    }
    if (!status) {
        ov00_0220A890(conn, ov00_02217F60);
        return 2;
    }
    num = iconn->numBuddies;
    if (index < 0 || index >= num) {
        ov00_0220A890(conn, ov00_02217F70);
        return 2;
    }
    profile = ov00_02207F88(conn, index);
    if (!profile) {
        ov00_0220A890(conn, ov00_02217F70);
        return 2;
    }
    bs = profile->buddyStatus;
    if (!bs) {
        __msl_assertion_failed(ov00_02217F80, ov00_02217F8C, ov00_02217EAC, 0x5CF);
    }
    status->profile = profile->profileId;
    status->status = bs->status;
    if (!bs->statusString) {
        bs->statusString[0] = 0;
    } else {
        ov00_0220A348(status->statusString, bs->statusString, 0x100);
    }
    if (!bs->locationString) {
        bs->locationString[0] = 0;
    } else {
        ov00_0220A348(status->locationString, bs->locationString, 0x100);
    }
    status->ip = bs->ip;
    status->port = bs->port;
    return 0;
}

int ov00_02200540(GPConnection *conn, int profileId, int *index) {
    GPIConnection *iconn;
    GPIProfile *profile;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        *index = 0;
        return 0;
    }
    if (ov00_02207DD0(conn, profileId, &profile) && profile->buddyStatus) {
        *index = profile->buddyStatus->buddyIndex;
    } else {
        *index = -1;
    }
    return 0;
}

int ov00_022005B0(GPConnection *conn, int profileId) {
    GPIConnection *iconn;
    GPIProfile *profile;
    if (!conn || !(iconn = *conn)) {
        return 0;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (ov00_02207DD0(conn, profileId, &profile) && profile->buddyStatus) {
        return 1;
    }
    return 0;
}

int ov00_022005FC(GPConnection *conn, int a) {
    GPIConnection *iconn;
    int result;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    result = ov00_02201D9C(conn, a);
    if (result) {
        return result;
    }
    return 0;
}

int ov00_02200654(GPConnection *conn, int status, const char *statusString, const char *locationString) {
    GPIConnection *iconn;
    char statBuf[0x100];
    char locBuf[0x100];
    char *p;
    if (!conn || !(iconn = *conn)) {
        return 2;
    }
    if (iconn->simulation) {
        return 0;
    }
    if (iconn->connectState == 4) {
        ov00_0220A890(conn, ov00_02217EE4);
        return 2;
    }
    if (!statusString) {
        ov00_0220A890(conn, ov00_02217F94);
        return 2;
    }
    if (!locationString) {
        ov00_0220A890(conn, ov00_02217FAC);
        return 2;
    }
    ov00_0220A348(statBuf, statusString, 0x100);
    ov00_fixSlashes(statBuf);
    ov00_0220A348(locBuf, locationString, 0x100);
    ov00_fixSlashes(locBuf);
    if (status == iconn->unk214 && !strcmp(statBuf, iconn->unk218) && !strcmp(locBuf, iconn->unk318)) {
        return 0;
    }
    iconn->unk214 = status;
    ov00_0220A348(iconn->unk218, statBuf, 0x100);
    ov00_0220A348(iconn->unk318, locBuf, 0x100);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217FC4);
    ov00_0220211C(conn, &iconn->outputBuffer, status);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F30);
    ov00_0220211C(conn, &iconn->outputBuffer, iconn->sessKey);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217FD0);
    ov00_022020EC(conn, &iconn->outputBuffer, statBuf);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217FE0);
    ov00_022020EC(conn, &iconn->outputBuffer, locBuf);
    ov00_022020EC(conn, &iconn->outputBuffer, ov00_02217F58);
    return 0;
}
