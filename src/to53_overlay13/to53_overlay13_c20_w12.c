#pragma thumb on
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;

typedef struct HashCtx {
    u32 state[4];
    u32 bitsLo;
    u32 bitsHi;
    u8 buf[64];
} HashCtx;

typedef struct Runtime {
    u32 unk00;
    void *(*alloc)(u32);
    u32 unk08;
    void (*free)(void *);
    u32 stop;
    u8 unk14[0x1c];
    void *threadMem;
    u32 unk34;
    s32 state;
    u8 unk3c[0x28];
    s32 active;
    u8 unk68[0xc];
    s32 resultReady;
    s32 error;
    void (*callback)(void *);
} Runtime;

typedef struct Status {
    u32 unk00[2];
    u32 threadSize;
    s32 deadline;
    u32 arg;
} Status;

extern Runtime ov13_0224DF30;
extern Status ov13_02245A58;
extern u8 ov13_0224DFDC[];
extern u8 ov13_0224E09C[];
extern const u8 ov13_02245AD8[];

extern void *memset(void *, int, u32);
extern void *memcpy(void *, const void *, u32);
extern u64 OS_GetTick(void);
extern void OS_Sleep(u32);
extern int OS_IsThreadAvailable(void);
extern int OS_IsThreadTerminated(void *);
extern void OS_WakeupThreadDirect(void *);
extern void OS_JoinThread(void *);
extern void OS_CreateThread(void *, void (*)(void *), void *, void *, u32, int);
extern int ov13_02223EE0(u32);
extern int ov13_02223F84(void);
extern void ov13_02225320(void *);
void ov13_022262D0(HashCtx *);
void ov13_022256C8(const u8 *, const u8 *, u8 *);
void ov13_022262F8(HashCtx *, const u8 *, u32);
void ov13_02226370(u8 *, HashCtx *);
void ov13_022263C4(HashCtx *, const u8 *);
void ov13_02226C0C(u8 *, const volatile u32 *, u32);
void ov13_02226C38(u32 *, const u8 *, u32);
void ov13_02226C64(u8 *, const u8 *, u32);
void ov13_02226C7C(s8 *, s8, u32);
void ov13_02226C94(u8 *, const u8 *, u32);
u32 ov13_02226CBC(void);
void *ov13_02226CD8(u32, u32);
void ov13_02226CFC(void *);
int ov13_02226D0C(char *, int);
int ov13_02226D40(char *, const s8 *);
int ov13_02226D74(int, u32, void (*)(void *), void *(*)(u32), void (*)(void *), u32);
int ov13_02226E5C(void);
int ov13_02226EF0(s32 *);
int ov13_02226F28(void *);
void ov13_02226F3C(void);

void ov13_022262D0(HashCtx *ctx) {
    ctx->bitsHi = 0;
    ctx->bitsLo = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
}
void ov13_022262F8(HashCtx *ctx, const u8 *src, u32 len) {
    u32 idx = (ctx->bitsLo >> 3) & 0x3f;
    u32 old = ctx->bitsLo;
    u32 part;
    u32 i;
    ctx->bitsLo += len << 3;
    if (ctx->bitsLo < (len << 3)) ctx->bitsHi++;
    ctx->bitsHi += len >> 29;
    part = 64 - idx;
    if (len >= part) {
        ov13_02226C64(ctx->buf + idx, src, part);
        ov13_022263C4(ctx, ctx->buf);
        for (i = part; i + 63 < len; i += 64) ov13_022263C4(ctx, src + i);
        idx = 0;
    } else {
        i = 0;
    }
    ov13_02226C64(ctx->buf + idx, src + i, len - i);
}
void ov13_02226370(u8 *out, HashCtx *ctx) {
    u8 bits[8];
    u32 idx;
    u32 pad;
    ov13_02226C0C(bits, &ctx->bitsLo, 8);
    idx = (ctx->bitsLo >> 3) & 0x3f;
    pad = idx < 56 ? 56 - idx : 120 - idx;
    ov13_022262F8(ctx, ov13_02245AD8, pad);
    ov13_022262F8(ctx, bits, 8);
    ov13_02226C0C(out, ctx->state, 16);
    ov13_02226C7C((s8 *)ctx, 0, 0x58);
}
