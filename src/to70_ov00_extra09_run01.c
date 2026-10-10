#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Copy {
    u32 data[60];
} Copy;

extern int ov00_022020EC(void *, void *, const void *);
extern int ov00_0220211C(void *, void *, s32);
extern void *ov00_021EC3A8(u32);
extern void *ov00_021FF3D4(void *);
extern void *ov00_021FE6EC(void *, s32);
extern void ov00_021EC3D8(void *);
extern void ov00_021FF82C(void *, s32);
extern void ov00_021FF818(void *);
extern void ov00_021FE678(void *);
extern void ov00_021FE968(void *, s32);
extern s32 ArrayLength(void *);
extern void __msl_assertion_failed(const void *, const void *, const void *, s32);
extern void ov00_0220A890(void *, const void *);
extern void ov00_0220A3BC(void *, const void *, ...);
extern s32 ov00_02203A58(void *, void *);
extern s32 ov00_02207B8C(void *, void *);
extern s32 ov00_0220484C(void *, void *);
extern s32 ov00_0220A244(void *, void *);
extern s32 ov00_0220684C(void *, void *);
extern s32 ov00_02206BA8(void *, void *);
extern s32 ov00_02206ED8(void *, void *);
extern int ov00_0220267C(void *, void *, void *, s32 *, s32, const void *);
extern void ov00_02201AE8(void *, s32, s32, void *);
extern void ov00_021FF48C(void *, u32);
extern void ov00_021FF4D0(void *, u32);
extern void ov00_021FF510(void *);
extern void ov00_021FF558(void *);
extern char ov00_02218BAC[];
extern char ov00_02218CD0[];
extern char ov00_02218CE8[];
extern char ov00_02218CF0[];
extern char ov00_02218E2C[];
extern char ov00_02218E44[];
extern char ov00_02218E4C[];
extern char ov00_02218E74[];
extern char ov00_02218E9C[];
extern char ov00_02218ED4[];
extern char ov00_02218EE4[];
extern char ov00_02218EE8[];
extern char ov00_02218F08[];
extern char ov00_02218F60[];
extern char ov00_0221904C[];
extern char ov00_022190A8[];
extern char ov00_022190F0[];
int ov00_02205064(void *ctx, void *buf);
int ov00_02205128(void *ctx, void *key, void *value);
int ov00_02205168(void *ctx, void *key, void *value);
int ov00_02205E50(void *ctx, s32 a, s32 b);
int ov00_0220608C(void *ctx, void *node, Copy *input);
void ov00_02206164(void *node);
int ov00_0220657C(void *ctx, s32 type, void *data, void **result, s32 flag, void *a, void *b);
void ov00_02206634(void *ctx, void *node);
void ov00_022066D0(void *ctx, void *node);
int ov00_02206720(void *ctx, void **result, s32 id);
int ov00_0220676C(void *ctx);
int ov00_022067A8(void *ctx, void *node);
int ov00_02206E04(void *ctx, void *node);
int ov00_02207158(void *ctx, void *node);
void ov00_022071F4(void *ctx, void *node);
void ov00_02207370(void *ctx);

int ov00_02205064(void *ctx, void *buf) {
    void *s = P(ctx, 0);
    if (I(s, 0x448) > 0) {
        ov00_022020EC(ctx, buf, ov00_02218CD0);
        ov00_0220211C(ctx, buf, I(s, 0x198));
        ov00_022020EC(ctx, buf, P(s, 0x440));
        ov00_022020EC(ctx, buf, ov00_02218CE8);
        I(s, 0x448) = 0;
    }
    if (I(s, 0x458) > 0) {
        ov00_022020EC(ctx, buf, ov00_02218CF0);
        ov00_0220211C(ctx, buf, I(s, 0x198));
        ov00_022020EC(ctx, buf, P(s, 0x450));
        ov00_022020EC(ctx, buf, ov00_02218CE8);
        I(s, 0x458) = 0;
    }
    return 0;
}

int ov00_02205128(void *ctx, void *key, void *value) {
    void *s = P(ctx, 0);
    int e = ov00_022020EC(ctx, (u8 *)s + 1088, key);
    if (e) {
        return e;
    }
    e = ov00_022020EC(ctx, (u8 *)s + 1088, value);
    if (e) {
        return e;
    }
    return 0;
}

int ov00_02205168(void *ctx, void *key, void *value) {
    void *s = P(ctx, 0);
    int e = ov00_022020EC(ctx, (u8 *)s + 1104, key);
    if (e) {
        return e;
    }
    e = ov00_022020EC(ctx, (u8 *)s + 1104, value);
    if (e) {
        return e;
    }
    return 0;
}
