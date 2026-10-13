#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
int printf(const char *, ...);
void abort(void);
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef struct {
    int a, b;
} Pair;

typedef struct {
    Pair *p;
} Iter;

typedef struct {
    u8 x;
} Empty;

typedef struct {
    Pair *data;
    u32 size, capacity;
} Vec;

extern u32 ov45_02255198[];
extern u32 ov45_022577D0;
extern u32 ov45_022551A4;
extern char _ZSt7nothrow[];
extern char ov45_022551C4[];
void *_ZnamRKSt9nothrow_t(u32, void *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void DWC_Free(int, void *, int);
void *PPWLobby_GetWork(void);
int ov45_0223DFC8(void *);
void DWC_GetDateTime(RTCDate *, RTCTime *);
void ov45_0223B8D0(const char *);
void ov45_0223BD0C(void);
void ov45_0223AA80(void *, int);
int ov45_0223AC78(void *, void *, u32);
int ov45_0223AE84(void *, void *, u32);
int ov45_0223AEDC(void *, void *);
int ov45_0223AF04(void *, void *);
int ov45_0223B104(void *);
int ov45_0223B1C8(void *);
void ov45_0223B764(void *, u32);
void ov45_0223B81C(void *, u32);
u32 ov45_0223BBFC(void *, u32);
u32 ov45_0223BD94(Vec *, u32);
u32 ov45_0223BED0(Vec *, u32);
void ov45_0223A9C0(Iter, Iter);
void ov45_0223AAE4(Vec *, Pair *, Pair *);
void ov45_0223ACF0(Vec *, Pair *, Pair *);
void ov45_0223BE60(Vec *, u32, u32);
void ov45_0223BD24(Vec *, u32, u32);
void ov45_0223CE30(Vec *, u32, Empty);
void ov45_0223CE60(Vec *, u32, Empty);
int ov45_0223ACA4(void *, void *, u32);
void ov45_0223B250(void *, void *, u32);
void ov45_0223CD78(void *, u32);

int ov45_0223AEDC(void *p, void *out) {
    if (!W(p, 0x334)) {
        return 0;
    }
    MI_CpuCopy8((u8 *)p + 0x5c, out, 0x2d8);
    return 1;
}

int ov45_0223AF04(void *p, void *in) {
    MI_CpuCopy8(in, (u8 *)p + 0x5c, 0x2d8);
    W(p, 0x334) = 1;
    return 1;
}
