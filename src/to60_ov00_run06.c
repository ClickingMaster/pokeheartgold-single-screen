#pragma force_active on
#pragma warn_unusedvar off
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

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

typedef struct Md5Ctx {
    u8 data[0x58];
} Md5Ctx;

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
extern void MD5Init(Md5Ctx *);
extern void MD5Update(Md5Ctx *, const void *, u32);
extern void MD5Final(u8 *, Md5Ctx *);
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


void ov00_021FE514(DArray *a, int n);
void ov00_021FE538(DArray *a);
void ov00_021FE590(DArray *a, const void *elem, int n);
DArray *ov00_021FE5B4(int elemSize, int growBy, ArrayElementFreeFn freeFn);
void ov00_021FE858(DArray *a, const void *elem, ArrayCompareFn cmp);
void ov00_021FEA20(DArray *a, ArrayCompareFn cmp);
void ov00_021FEC3C(DArray *a);
HashTable *ov00_021FED24(int elemSize, int nbuckets, TableHashFn hashFn, ArrayCompareFn cmpFn, ArrayElementFreeFn freeFn);
void ov00_021FEE8C(HashTable *t);
int ov00_021FEF00(HashTable *t);
void ov00_021FEF74(HashTable *t, const void *elem);
int ov00_021FF01C(HashTable *t, const void *elem);
void *ov00_021FF0C0(HashTable *t, const void *elem);
void ov00_021FF160(HashTable *t, ArrayMapFn fn, void *arg);
void ov00_021FF1F8(HashTable *t, ArrayMapFn fn, void *arg);
void *ov00_021FF264(HashTable *t, ArrayMapFn2 fn, void *arg);
void ov00_021FF2DC(u8 *digest, char *out);
void ov00_021FF31C(const void *data, u32 len, char *out);
u32 ov00_021FF368(void);
void ov00_021FF3C0(u32 ms);
void ov00_021FF3CC(void);
void ov00_021FF3D0(void);
char *ov00_021FF3D4(const char *s);
char *ov00_021FF408(char *s);
int ov00_021FF44C(int sock, int blocking);
int ov00_021FF48C(int sock, int size);
int ov00_021FF4D0(int sock, int size);
int ov00_021FF510(int sock);
int ov00_021FF558(int sock);
int ov00_021FF694(int sock);
int ov00_021FF6BC(int sock);
void *ov00_021FF6E4(void);
int ov00_021FF770(const u32 *in);
int ov00_021FF804(int domain, int type, int proto);
int ov00_021FF818(int sock);
int ov00_021FF82C(int sock, int how);
int ov00_021FF840(int sock, const SocSockAddr *addr, int len);
int ov00_021FF8A0(int sock, const SocSockAddr *addr, int len);
int ov00_021FF8EC(int sock, int backlog);
int ov00_021FF900(int sock, SocSockAddr *addr, int *len);
int ov00_021FF92C(int sock, void *buf, int len, int flags);
int ov00_021FF940(int sock, void *buf, int len, int flags, SocSockAddr *addr, int *addrlen);
int ov00_021FF970(int sock, const void *buf, int len, int flags);

// @fn ov00_021FE514
void ov00_021FE514(DArray *a, int n) {
    if (a->freeFn != NULL) {
        a->freeFn(ov00_021FE6EC(a, n));
    }
}

// @fn ov00_021FE538
void ov00_021FE538(DArray *a) {
    a->capacity += a->growBy;
    a->list = ov00_021EC3BC(a->list, a->capacity * a->elemSize);
    if (!a->list) {
        __msl_assertion_failed(ov00_02217D10, ov00_02217D1C, ov00_02217C7C, 0x41);
    }
}

// @fn ov00_021FE590
void ov00_021FE590(DArray *a, const void *elem, int n) {
    memcpy(ov00_021FE6EC(a, n), elem, a->elemSize);
}

// @fn ov00_021FE5B4
DArray *ov00_021FE5B4(int elemSize, int growBy, ArrayElementFreeFn freeFn) {
    DArray *a = ov00_021EC3A8(sizeof(DArray));
    if (!a) {
        __msl_assertion_failed(ov00_02217D28, ov00_02217D1C, ov00_02217C64, 0x52);
    }
    if (!elemSize) {
        __msl_assertion_failed(ov00_02217D30, ov00_02217D1C, ov00_02217C64, 0x53);
    }
    if (growBy == 0) {
        growBy = 8;
    }
    a->count = 0;
    a->capacity = growBy;
    a->elemSize = elemSize;
    a->growBy = growBy;
    a->freeFn = freeFn;
    if (a->capacity != 0) {
        a->list = ov00_021EC3A8(a->capacity * a->elemSize);
        if (!a->list) {
            __msl_assertion_failed(ov00_02217D10, ov00_02217D1C, ov00_02217C64, 0x5e);
        }
    } else {
        a->list = NULL;
    }
    return a;
}

// @fn ov00_021FE678
void ov00_021FE678(DArray *a) {
    int i;
    if (!a) {
        __msl_assertion_failed(ov00_02217D28, ov00_02217D1C, ov00_02217C70, 0x69);
    }
    for (i = 0; i < a->count; i++) {
        ov00_021FE514(a, i);
    }
    ov00_021EC3D8(a->list);
    ov00_021EC3D8(a);
}

// @fn ArrayLength
int ArrayLength(const DArray *a) {
    return a->count;
}

// @fn ov00_021FE6EC
void *ov00_021FE6EC(DArray *a, int n) {
    if (!(n >= 0 && n < a->count)) {
        __msl_assertion_failed(ov00_02217D3C, ov00_02217D1C, ov00_02217C58, 0x94);
    }
    if (n < 0 || n >= a->count) {
        return NULL;
    }
    return (char *)a->list + a->elemSize * n;
}

// @fn ov00_021FE754
void ov00_021FE754(DArray *a, const void *elem) {
    if (!a) {
        __msl_assertion_failed(ov00_02217D28, ov00_02217D1C, ov00_02217C88, 0xa0);
    }
    if (a) {
        ov00_021FE7A0(a, elem, a->count);
    }
}

// @fn ov00_021FE7A0
void ov00_021FE7A0(DArray *a, const void *elem, int n) {
    int last;
    if (!(n >= 0 && n <= a->count)) {
        __msl_assertion_failed(ov00_02217D5C, ov00_02217D1C, ov00_02217CB4, 0xa7);
    }
    if (a->count == a->capacity) {
        ov00_021FE538(a);
    }
    a->count++;
    if (n < a->count - 1) {
        memmove(ov00_021FE6EC(a, n + 1), ov00_021FE6EC(a, n), (a->count - 1 - n) * a->elemSize);
    }
    ov00_021FE590(a, elem, n);
}

// @fn ov00_021FE858
void ov00_021FE858(DArray *a, const void *elem, ArrayCompareFn cmp) {
    int found;
    void *p;
    if (!cmp) {
        __msl_assertion_failed(ov00_02217D7C, ov00_02217D1C, ov00_02217CE8, 0xb7);
    }
    p = ov00_021FECC0(elem, a->list, a->count, a->elemSize, cmp, &found);
    ov00_021FE7A0(a, elem, ((char *)p - (char *)a->list) / a->elemSize);
}

// @fn ov00_021FE8D8
void ov00_021FE8D8(DArray *a, int n) {
    if (!(n >= 0 && n < a->count)) {
        __msl_assertion_failed(ov00_02217D3C, ov00_02217D1C, ov00_02217C94, 0xc0);
    }
    if (n < a->count - 1) {
        memmove(ov00_021FE6EC(a, n), ov00_021FE6EC(a, n + 1), (a->count - 1 - n) * a->elemSize);
    }
    a->count--;
}

// @fn ov00_021FE968
void ov00_021FE968(DArray *a, int n) {
    if (!(n >= 0 && n < a->count)) {
        __msl_assertion_failed(ov00_02217D3C, ov00_02217D1C, ov00_02217CA4, 0xca);
    }
    ov00_021FE514(a, n);
    ov00_021FE8D8(a, n);
}

// @fn ov00_021FE9C0
void ov00_021FE9C0(DArray *a, const void *elem, int n) {
    if (!(n >= 0 && n < a->count)) {
        __msl_assertion_failed(ov00_02217D3C, ov00_02217D1C, ov00_02217CC4, 0xd3);
    }
    ov00_021FE514(a, n);
    ov00_021FE590(a, elem, n);
}

// @fn ov00_021FEA20
void ov00_021FEA20(DArray *a, ArrayCompareFn cmp) {
    qsort(a->list, a->count, a->elemSize, cmp);
}

// @fn ov00_021FEA40
int ov00_021FEA40(DArray *a, const void *key, ArrayCompareFn cmp, int from, int sorted) {
    int count;
    int found = 1;
    void *p;
    if (!a || !(count = a->count)) {
        return -1;
    }
    if (sorted) {
        p = ov00_021FECC0(key, ov00_021FE6EC(a, from), count - from, a->elemSize, cmp, &found);
    } else {
        p = ov00_021FEC68(key, ov00_021FE6EC(a, from), count - from, a->elemSize, cmp);
    }
    if (!p || !found) {
        return -1;
    }
    return ((char *)p - (char *)a->list) / a->elemSize;
}

// @fn ov00_021FEAF8
void ov00_021FEAF8(DArray *a, ArrayMapFn fn, void *arg) {
    int i;
    if (!fn) {
        __msl_assertion_failed(ov00_02217D88, ov00_02217D1C, ov00_02217C4C, 0xf9);
    }
    for (i = 0; i < a->count; i++) {
        fn(ov00_021FE6EC(a, i), arg);
    }
}

// @fn ov00_021FEB64
void ov00_021FEB64(DArray *a, ArrayMapFn fn, void *arg) {
    int i;
    if (!fn) {
        __msl_assertion_failed(ov00_02217D88, ov00_02217D1C, ov00_02217CD4, 0x104);
    }
    for (i = a->count - 1; i >= 0; i--) {
        fn(ov00_021FE6EC(a, i), arg);
    }
}

// @fn ov00_021FEBC4
void *ov00_021FEBC4(DArray *a, ArrayMapFn2 fn, void *arg) {
    int i;
    void *e;
    if (!fn) {
        __msl_assertion_failed(ov00_02217D88, ov00_02217D1C, ov00_02217CFC, 0x121);
    }
    for (i = a->count - 1; i >= 0; i--) {
        e = ov00_021FE6EC(a, i);
        if (!fn(e, arg)) {
            return e;
        }
    }
    return NULL;
}

// @fn ov00_021FEC3C
void ov00_021FEC3C(DArray *a) {
    int i;
    for (i = ArrayLength(a) - 1; i >= 0; i--) {
        ov00_021FE968(a, i);
    }
}

// @fn ov00_021FEC68
void *ov00_021FEC68(const void *key, const void *base, int count, int size, ArrayCompareFn cmp) {
    int i;
    for (i = 0; i < count; i++) {
        if (cmp(key, (char *)base + i * size) == 0) {
            return (char *)base + size * i;
        }
    }
    return NULL;
}

// @fn ov00_021FECC0
void *ov00_021FECC0(const void *key, const void *base, int count, int size, ArrayCompareFn cmp, int *found) {
    int lo = 0;
    int hi;
    int mid;
    int c;
    *found = 0;
    hi = count - 1;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        c = cmp((char *)base + mid * size, key);
        if (c == 0) {
            *found = 1;
        }
        if (c < 0) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return (char *)base + lo * size;
}

// @fn ov00_021FED24
HashTable *ov00_021FED24(int elemSize, int nbuckets, TableHashFn hashFn, ArrayCompareFn cmpFn, ArrayElementFreeFn freeFn) {
    return ov00_021FED4C(elemSize, nbuckets, 4, hashFn, cmpFn, freeFn);
}

// @fn ov00_021FED4C
HashTable *ov00_021FED4C(int elemSize, int nbuckets, int nchains, TableHashFn hashFn, ArrayCompareFn cmpFn, ArrayElementFreeFn freeFn) {
    HashTable *t;
    int i;
    if (!hashFn) {
        __msl_assertion_failed(ov00_02217E00, ov00_02217E08, ov00_02217D98, 0x38);
    }
    if (!cmpFn) {
        __msl_assertion_failed(ov00_02217E14, ov00_02217E08, ov00_02217D98, 0x39);
    }
    if (!elemSize) {
        __msl_assertion_failed(ov00_02217E1C, ov00_02217E08, ov00_02217D98, 0x3a);
    }
    if (!nbuckets) {
        __msl_assertion_failed(ov00_02217E28, ov00_02217E08, ov00_02217D98, 0x3b);
    }
    t = ov00_021EC3A8(sizeof(HashTable));
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217D98, 0x3e);
    }
    t->buckets = ov00_021EC3A8(nbuckets * sizeof(DArray *));
    if (!t->buckets) {
        __msl_assertion_failed(ov00_02217E3C, ov00_02217E08, ov00_02217D98, 0x41);
    }
    for (i = 0; i < nbuckets; i++) {
        t->buckets[i] = ov00_021FE5B4(elemSize, nchains, freeFn);
    }
    t->nbuckets = nbuckets;
    t->freeFn = freeFn;
    t->cmpFn = cmpFn;
    t->hashFn = hashFn;
    return t;
}

// @fn ov00_021FEE8C
void ov00_021FEE8C(HashTable *t) {
    int i;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217DA4, 0x51);
    }
    if (!t) {
        return;
    }
    for (i = 0; i < t->nbuckets; i++) {
        ov00_021FE678(t->buckets[i]);
    }
    ov00_021EC3D8(t->buckets);
    ov00_021EC3D8(t);
}

// @fn ov00_021FEF00
int ov00_021FEF00(HashTable *t) {
    int i;
    int count = 0;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217DBC, 0x61);
    }
    if (!t) {
        return 0;
    }
    for (i = 0; i < t->nbuckets; i++) {
        count += ArrayLength(t->buckets[i]);
    }
    return count;
}

// @fn ov00_021FEF74
void ov00_021FEF74(HashTable *t, const void *elem) {
    int hash;
    int idx;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217DB0, 0x71);
    }
    if (!t) {
        return;
    }
    hash = t->hashFn(elem, t->nbuckets);
    idx = ov00_021FEA40(t->buckets[hash], elem, t->cmpFn, 0, 0);
    if (idx == -1) {
        ov00_021FE754(t->buckets[hash], elem);
    } else {
        ov00_021FE9C0(t->buckets[hash], elem, idx);
    }
}

// @fn ov00_021FF01C
int ov00_021FF01C(HashTable *t, const void *elem) {
    int hash;
    int idx;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217DC8, 0x82);
    }
    if (!t) {
        return 0;
    }
    hash = t->hashFn(elem, t->nbuckets);
    idx = ov00_021FEA40(t->buckets[hash], elem, t->cmpFn, 0, 0);
    if (idx == -1) {
        return 0;
    }
    ov00_021FE968(t->buckets[hash], idx);
    return 1;
}

// @fn ov00_021FF0C0
void *ov00_021FF0C0(HashTable *t, const void *elem) {
    int hash;
    int idx;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217DD4, 0x94);
    }
    if (!t) {
        return NULL;
    }
    hash = t->hashFn(elem, t->nbuckets);
    idx = ov00_021FEA40(t->buckets[hash], elem, t->cmpFn, 0, 0);
    if (idx == -1) {
        return NULL;
    }
    return ov00_021FE6EC(t->buckets[hash], idx);
}

// @fn ov00_021FF160
void ov00_021FF160(HashTable *t, ArrayMapFn fn, void *arg) {
    int i;
    if (!t) {
        __msl_assertion_failed(ov00_02217E34, ov00_02217E08, ov00_02217D8C, 0xa7);
    }
    if (!fn) {
        __msl_assertion_failed(ov00_02217E4C, ov00_02217E08, ov00_02217D8C, 0xa8);
    }
    if (!t || !fn) {
        return;
    }
    for (i = 0; i < t->nbuckets; i++) {
        ov00_021FEAF8(t->buckets[i], fn, arg);
    }
}

// @fn ov00_021FF1F8
void ov00_021FF1F8(HashTable *t, ArrayMapFn fn, void *arg) {
    int i;
    if (!fn) {
        __msl_assertion_failed(ov00_02217E4C, ov00_02217E08, ov00_02217DE0, 0xb6);
    }
    for (i = 0; i < t->nbuckets; i++) {
        ov00_021FEB64(t->buckets[i], fn, arg);
    }
}

// @fn ov00_021FF264
void *ov00_021FF264(HashTable *t, ArrayMapFn2 fn, void *arg) {
    int i;
    void *e;
    if (!fn) {
        __msl_assertion_failed(ov00_02217E4C, ov00_02217E08, ov00_02217DF0, 0xd3);
    }
    for (i = 0; i < t->nbuckets; i++) {
        e = ov00_021FEBC4(t->buckets[i], fn, arg);
        if (e) {
            return e;
        }
    }
    return NULL;
}

// @fn ov00_021FF2DC
void ov00_021FF2DC(u8 *digest, char *out) {
    unsigned int i;
    for (i = 0; i < 16; i++) {
        sprintf(out + 2 * i, ov00_02217E50, digest[i]);
    }
}

// @fn ov00_021FF31C
void ov00_021FF31C(const void *data, u32 len, char *out) {
    u8 digest[16];
    Md5Ctx ctx;
    MD5Init(&ctx);
    MD5Update(&ctx, data, len);
    MD5Final(digest, &ctx);
    ov00_021FF2DC(digest, out);
}

// @fn ov00_021FF368
u32 ov00_021FF368(void) {
    if (OS_IsTickAvailable() != TRUE) {
        __msl_assertion_failed(ov00_02217E74, ov00_02217E94, ov00_02217E64, 0x109);
    }
    return (u32)((OS_GetTick() * 64) / 33514);
}

// @fn ov00_021FF3C0
void ov00_021FF3C0(u32 ms) {
    OS_Sleep(ms);
}

// @fn ov00_021FF3CC
void ov00_021FF3CC(void) {
}

// @fn ov00_021FF3D0
void ov00_021FF3D0(void) {
}

// @fn ov00_021FF3D4
char *ov00_021FF3D4(const char *s) {
    char *p;
    if (!s) {
        return NULL;
    }
    p = ov00_021EC3A8(strlen(s) + 1);
    if (p) {
        strcpy(p, s);
    }
    return p;
}

// @fn ov00_021FF408
char *ov00_021FF408(char *s) {
    char *p = s;
    while (*s) {
        *s = ov00_tolower(*s);
        s++;
    }
    return p;
}

// @fn ov00_021FF44C
int ov00_021FF44C(int sock, int blocking) {
    int flags = SOC_Fcntl(sock, 3, 0);
    if (blocking) {
        flags &= ~4;
    } else {
        flags |= 4;
    }
    return SOC_Fcntl(sock, 4, flags) == 0;
}

// @fn ov00_021FF48C
int ov00_021FF48C(int sock, int size) {
    return ov00_021FFA04(sock, 0xFFFF, 0x1002, &size, sizeof(size)) != -1;
}

// @fn ov00_021FF4D0
int ov00_021FF4D0(int sock, int size) {
    return ov00_021FFA04(sock, 0xFFFF, 0x1001, &size, sizeof(size)) != -1;
}

// @fn ov00_021FF510
int ov00_021FF510(int sock) {
    int size;
    int len;
    len = sizeof(size);
    if (ov00_021FF9DC(sock, 0xFFFF, 0x1002, &size, &len) == -1) {
        return -1;
    }
    return size;
}

// @fn ov00_021FF558
int ov00_021FF558(int sock) {
    int size;
    int len;
    len = sizeof(size);
    if (ov00_021FF9DC(sock, 0xFFFF, 0x1001, &size, &len) == -1) {
        return -1;
    }
    return size;
}

// @fn ov00_021FF59C
int ov00_021FF59C(int sock, int *readFlag, int *writeFlag, int *exceptFlag) {
    SocPollFd pfd;
    int rval;
    pfd.fd = sock;
    pfd.events = 0;
    if (readFlag) {
        pfd.events |= 1;
    }
    if (writeFlag) {
        pfd.events |= 8;
    }
    pfd.revents = 0;
    rval = SOC_Poll(&pfd, 1, 0);
    if (rval < 0) {
        return -1;
    }
    if (readFlag) {
        if (rval > 0 && (pfd.revents & 0x41)) {
            *readFlag = 1;
        } else {
            *readFlag = 0;
        }
    }
    if (writeFlag) {
        if (rval > 0 && (pfd.revents & 8)) {
            *writeFlag = 1;
        } else {
            *writeFlag = 0;
        }
    }
    if (exceptFlag) {
        if (rval > 0 && (pfd.revents & 0x20)) {
            *exceptFlag = 1;
        } else {
            *exceptFlag = 0;
        }
    }
    return rval;
}

// @fn ov00_021FF694
int ov00_021FF694(int sock) {
    int ready = 0;
    if (ov00_021FF59C(sock, &ready, NULL, NULL) == 1) {
        return ready;
    }
    return 0;
}

// @fn ov00_021FF6BC
int ov00_021FF6BC(int sock) {
    int ready = 0;
    if (ov00_021FF59C(sock, NULL, &ready, NULL) == 1) {
        return ready;
    }
    return 0;
}

// @fn ov00_021FF6E4
void *ov00_021FF6E4(void) {
    ov00_0221B1AC.hName = ov00_02217EA0;
    ov00_0221B1AC.hAliases = (char **)(ov00_0221B1B4 - 8);
    ov00_0221B1AC.hAddrType = 2;
    ov00_0221B1AC.hLength = 0;
    ov00_0221B1AC.hAddrList = ov00_0221B1D8;
    ov00_0221B1AC.addr = 0;
    SOC_U32to4U8(SOCL_GetHostID(), ov00_0221B1C4);
    if (ov00_0221B1AC.addr == 0) {
        return NULL;
    }
    ov00_0221B1AC.addrList[0] = ov00_0221B1C4;
    ov00_0221B1AC.hLength = 4;
    ov00_0221B1AC.addrList[1] = NULL;
    return ov00_0221B1B4;
}

// @fn ov00_021FF770
int ov00_021FF770(const u32 *in) {
    u32 x = *in;
    u32 ip = ((x >> 24) & 0xff) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | ((x << 24) & 0xff000000);
    int b1 = (ip >> 24) & 0xff;
    int b2 = (ip >> 16) & 0xff;
    if (b1 == 10) {
        return 1;
    }
    if (b1 == 172 && b2 >= 16 && b2 <= 31) {
        return 1;
    }
    return b1 == 192 && b2 == 168;
}

// @fn ov00_021FF7EC
int ov00_021FF7EC(int ret, int err) {
    if (ret < 0) {
        ov00_0221B1AC.lastError = ret;
        ret = err;
    }
    return ret;
}

// @fn ov00_021FF804
int ov00_021FF804(int domain, int type, int proto) {
    return ov00_021FF7EC(SOC_Socket(domain, type, proto), -1);
}

// @fn ov00_021FF818
int ov00_021FF818(int sock) {
    return ov00_021FF7EC(SOC_Close(sock), -1);
}

// @fn ov00_021FF82C
int ov00_021FF82C(int sock, int how) {
    return ov00_021FF7EC(SOC_Shutdown(sock, how), -1);
}

// @fn ov00_021FF840
int ov00_021FF840(int sock, const SocSockAddr *addr, int len) {
    SocSockAddr local;
    if (((const SockAddrIn *)addr)->port == 0) {
        return 0;
    }
    local = *addr;
    local.len = len;
    return ov00_021FF7EC(SOC_Bind(sock, &local), -1);
}

// @fn ov00_021FF8A0
int ov00_021FF8A0(int sock, const SocSockAddr *addr, int len) {
    SocSockAddr local;
    local = *addr;
    local.len = len;
    return ov00_021FF7EC(SOC_Connect(sock, &local), -1);
}

// @fn ov00_021FF8EC
int ov00_021FF8EC(int sock, int backlog) {
    return ov00_021FF7EC(SOC_Listen(sock, backlog), -1);
}

// @fn ov00_021FF900
int ov00_021FF900(int sock, SocSockAddr *addr, int *len) {
    int ret;
    addr->len = *len;
    ret = SOC_Accept(sock, addr);
    *len = addr->len;
    return ov00_021FF7EC(ret, -1);
}

// @fn ov00_021FF92C
int ov00_021FF92C(int sock, void *buf, int len, int flags) {
    return ov00_021FF7EC(SOC_Recv(sock, buf, len, flags), -1);
}

// @fn ov00_021FF940
int ov00_021FF940(int sock, void *buf, int len, int flags, SocSockAddr *addr, int *addrlen) {
    int ret;
    addr->len = *addrlen;
    ret = SOC_RecvFrom(sock, buf, len, flags, addr);
    *addrlen = addr->len;
    return ov00_021FF7EC(ret, -1);
}

// @fn ov00_021FF970
int ov00_021FF970(int sock, const void *buf, int len, int flags) {
    return ov00_021FF7EC(SOC_Send(sock, buf, len, flags), -1);
}
