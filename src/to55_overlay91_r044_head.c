#include "global.h"

extern void ov91_0225EDB0(void *p);
extern void ov91_02260D00(void *p);
extern void GF_AssertFail(void);
extern void ov91_0225E1E0(u16 *p, u16 v);
extern void ov91_0226137C(void *p, u32 v);
extern void ov91_02261384(void *p, u16 v);
extern void ov91_0225E1E4(void *p, u32 v);
extern void ov91_0225E09C(void *p, u32 v);
extern void Sprite_Delete(void *p);
extern void SpriteTransfer_DeleteCharTransferTask(void *p);
extern void SpriteTransfer_DeletePlttTransferTask(void *p);
extern void DestroySingle2DGfxResObj(void *manager, void *obj);

void ov91_0225F05C(void *a, void *b, int index, void *out);
void ov91_0225F010(void *a, void *b);
void ov91_0225EE9C(void *arg);
void ov91_0225EEBC(void *p, void *a, void *b);
void ov91_0225EED0(void *p, void *a, void *b, int first, u16 second);
BOOL ov91_0225EF70(void *arg);
void ov91_0225F008(void *a, void *b);
void ov91_0225F1E8(void *a, void *b);

void ov91_0225EE9C(void *arg) {
    u8 *p = arg;
    *(u32 *)(p + 0x20) = 0;
    ov91_0225EDB0(p + 0x20);
    ov91_02260D00(p + 0x8544);
}

void ov91_0225EEBC(void *p, void *a, void *b) {
    *(u32 *)((u8 *)p + 0x48) = 0x10;
    ov91_0225F05C(a, b, 0, p);
}

void ov91_0225EED0(void *arg, void *a, void *b, int first, u16 second) {
    typedef struct {
        s32 word[6];
    } Rec18;
    u8 *p = arg;
    Rec18 src;
    Rec18 dst;
    s32 normal;
    s32 wrapped;
    ov91_0225F05C(a, b, first, &src);
    ov91_0225F05C(a, b, second, &dst);
    *(Rec18 *)(p + 0x18) = src;
    *(Rec18 *)p = src;
    *(s32 *)(p + 0x30) = dst.word[0] - src.word[0];
    *(s32 *)(p + 0x38) = dst.word[2] - src.word[2];
    *(s32 *)(p + 0x40) = dst.word[4] - src.word[4];
    *(s32 *)(p + 0x44) = dst.word[5] - src.word[5];
    wrapped = dst.word[1] + 0xffff - src.word[1];
    normal = dst.word[1] - src.word[1];
    if (normal < wrapped) {
        *(s32 *)(p + 0x34) = normal;
    } else {
        *(s32 *)(p + 0x34) = wrapped;
    }
    wrapped = dst.word[3] + 0xffff - src.word[3];
    normal = dst.word[3] - src.word[3];
    if (normal < wrapped) {
        *(s32 *)(p + 0x3c) = normal;
    } else {
        *(s32 *)(p + 0x3c) = wrapped;
    }
    *(s32 *)(p + 0x48) = 0;
}

BOOL ov91_0225EF70(void *arg) {
    s32 *p = arg;
    s32 step = p[18];
    if (step >= 16) {
        return TRUE;
    }
    step++;
    p[18] = step;
    p[0] = p[6] + p[12] * step / 16;
    p[1] = p[7] + p[13] * p[18] / 16;
    p[2] = p[8] + p[14] * p[18] / 16;
    p[3] = p[9] + p[15] * p[18] / 16;
    p[4] = p[10] + p[16] * p[18] / 16;
    p[5] = p[11] + p[17] * p[18] / 16;
    return FALSE;
}

void ov91_0225F008(void *a, void *b) {
    ov91_0225F010(a, b);
}

void ov91_0225F010(void *a, void *b) {
    u8 *p = a;
    u8 *q = b;
    ov91_0225E1E0((u16 *)(p + 0x19cc), (u16) * (u32 *)(q + 4));
    ov91_0226137C(p + 0x1ab4, *(u32 *)(q + 8));
    ov91_02261384(p + 0x1ab4, (u16) * (u32 *)(q + 0xc));
    ov91_0225E1E4(p + 0x19cc, *(u32 *)(q + 0x10));
    ov91_0225E09C(p + 0x19cc, *(u32 *)(q + 0x14));
}

void ov91_0225F05C(void *a, void *b, int index, void *out) {
    typedef struct {
        u32 word[6];
    } Rec18;
    u8 *src;
    s32 n;
    if ((u32)index >= *(u32 *)a) {
        GF_AssertFail();
    }
    src = *(u8 **)((u8 *)a + 4) + index * 0x18;
    *(Rec18 *)out = *(Rec18 *)src;
    *(u32 *)((u8 *)out + 4) = (u16) * (u32 *)((u8 *)out + 4);
    *(u32 *)((u8 *)out + 0xc) = (u16) * (u32 *)((u8 *)out + 0xc);
    if (*(u8 *)b != 0) {
        n = 6 - (index + 1);
        if (n < 0) {
            n = 0;
        }
        *(u32 *)((u8 *)out + 0x14) = *(u32 *)(*(u8 **)((u8 *)a + 4) + n * 0x18 + 0x14);
    }
    if (*((u8 *)b + 1) != 0) {
        *(s32 *)((u8 *)out + 4) = -*(s32 *)((u8 *)out + 4);
    }
}
