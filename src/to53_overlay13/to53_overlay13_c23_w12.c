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

int ov13_02226D40(char *dst, const s8 *src) {
    char *p = dst;
    int i;
    for (i = 0; i < 6; i++) {
        p += ov13_02226D0C(p, *src++);
        if (i < 5) *p++ = ':';
    }
    *p = 0;
    return p - dst;
}
