typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;

typedef struct {
    u32 type;
    s32 scaleX;
    s32 scaleY;
    s16 transX;
    s16 transY;
    u16 rotZ;
    u16 flags;
    u8 rest[8];
} Srtc;
typedef struct {
    u32 unk0;
    void *data;
    u32 unk8;
    Srtc srtc;
} G2dNode;
typedef struct {
    u8 animCtrl[0x1c];
    void *seq;
    u8 gap20[0x10];
    void *cell;
    void *bank;
    s32 transferHandle;
    Srtc srtc;
} CellAnim;

void NNSi_G2dInitializeVRamLocation(s32 *loc);
void NNSi_G2dSetVramLocation(s32 *loc, s32 index, s32 value);
s32 NNSi_G2dGetVramLocation(s32 *loc, s32 index);
void NNSi_G2dSrtcSetTrans(Srtc *s, s16 x, s16 y);
void NNSi_G2dSrtcSetSRTRotZ(Srtc *s, u16 angle);
void NNSi_G2dSrtcSetSRTScale(Srtc *s, s32 x, s32 y);
void NNSi_G2dSrtcInitControl(Srtc *s, u32 type);
void NNSi_G2dSrtcSetInitialValue(Srtc *s);
void NNSi_G2dInitializeNode(G2dNode *node, void *data);
void ApplyCurrentAnimResult_(CellAnim *anim);
void NNS_G2dInitCellAnimation(CellAnim *anim, void *seq, void *bank);
void NNS_G2dInitCellAnimationVramTransfered(CellAnim *anim, void *seq, void *bank, s32 handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
void NNS_G2dSetCellAnimationSequence(CellAnim *anim, void *seq);
void NNS_G2dTickCellAnimation(CellAnim *anim, u32 ticks);
void NNS_G2dSetCellAnimationCurrentFrame(CellAnim *anim, u32 frame);
extern void MIi_CpuClear16(u32 value, void *dst, u32 size);
extern void *NNS_G2dGetAnimCtrlCurrentElement(void *ctrl);
extern void *NNS_G2dGetCellDataByIdx(void *bank, u16 idx);
extern void NNS_G2dSetCellTransferStateRequested(s32 handle, u32 a, u32 b);
extern void NNS_G2dInitAnimCtrl(void *ctrl);
extern void NNS_G2dBindAnimCtrl(void *ctrl, void *seq);
extern s32 NNS_G2dTickAnimCtrl(void *ctrl, u32 dt);
extern s32 NNS_G2dSetAnimCtrlCurrentFrame(void *ctrl, u32 frame);
extern void NNSi_G2dInitCellTransferState(s32 handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);

void NNSi_G2dInitializeVRamLocation(s32 *loc) {
    s32 i;
    for (i = 0; i < 3; i++) {
        loc[i] = -1;
    }
}
void NNSi_G2dSetVramLocation(s32 *loc, s32 index, s32 value) {
    loc[index] = value;
}
s32 NNSi_G2dGetVramLocation(s32 *loc, s32 index) {
    return loc[index];
}
void NNSi_G2dSrtcSetTrans(Srtc *s, s16 x, s16 y) {
    if (s->type != 1) {
        return;
    }
    s->flags |= 8;
    s->transX = x;
    s->transY = y;
}
void NNSi_G2dSrtcSetSRTRotZ(Srtc *s, u16 angle) {
    if (s->type != 1) {
        return;
    }
    s->flags |= 4;
    s->rotZ = angle;
}
void NNSi_G2dSrtcSetSRTScale(Srtc *s, s32 x, s32 y) {
    if (s->type != 1) {
        return;
    }
    s->flags |= 2;
    s->scaleX = x;
    s->scaleY = y;
}
void NNSi_G2dSrtcSetInitialValue(Srtc *s);
void NNSi_G2dSrtcInitControl(Srtc *s, u32 type) {
    s->type = type;
    NNSi_G2dSrtcSetInitialValue(s);
}
void NNSi_G2dSrtcSetInitialValue(Srtc *s) {
    MIi_CpuClear16(0, &s->scaleX, 0x18);
    s->scaleX = 0x1000;
    s->scaleY = 0x1000;
}
void NNSi_G2dInitializeNode(G2dNode *node, void *data) {
    node->unk0 = 0;
    node->data = data;
    node->unk8 = 1;
    NNSi_G2dSrtcInitControl(&node->srtc, 1);
}
void ApplyCurrentAnimResult_(CellAnim *anim) {
    u8 *elem;
    u32 *bank;
    u32 type;
    if (*(u16 *)((u8 *)(*(void **)((u8 *)anim + 4)) + 4) == 0) {
        return;
    }
    NNS_G2dGetAnimCtrlCurrentElement(anim);
    elem = NNS_G2dGetAnimCtrlCurrentElement(anim);
    bank = anim->bank;
    anim->cell = NNS_G2dGetCellDataByIdx(bank, *(u16 *)elem);
    type = ((u32 *)anim->seq)[1] & 0xff;
    NNSi_G2dSrtcInitControl(&anim->srtc, 1);
    if (type != 0) {
        if (type == 2) {
            NNSi_G2dSrtcSetTrans(&anim->srtc, *(s16 *)(elem + 4), *(s16 *)(elem + 6));
        } else {
            NNSi_G2dSrtcSetSRTScale(&anim->srtc, *(s32 *)(elem + 4), *(s32 *)(elem + 8));
            NNSi_G2dSrtcSetSRTRotZ(&anim->srtc, *(u16 *)(elem + 2));
            NNSi_G2dSrtcSetTrans(&anim->srtc, *(s16 *)(elem + 12), *(s16 *)(elem + 14));
        }
    }
    u32 transfer = bank[3] != 0;
    if (!transfer) {
        return;
    }
    s32 h = (s32) * (volatile s32 *)&anim->transferHandle;
    u32 valid = h != -1;
    if (!valid) {
        return;
    }
    {
        u32 *xfer = (u32 *)(bank[3]);
        u32 *entry = (u32 *)(xfer[1] + *(u16 *)elem * 8);
        NNS_G2dSetCellTransferStateRequested(anim->transferHandle, entry[0], entry[1]);
    }
}
void NNS_G2dSetCellAnimationSequence(CellAnim *anim, void *seq);
void NNS_G2dInitCellAnimation(CellAnim *anim, void *seq, void *bank) {
    anim->bank = bank;
    anim->transferHandle = -1;
    NNSi_G2dSrtcInitControl(&anim->srtc, 1);
    NNS_G2dInitAnimCtrl(anim);
    NNS_G2dSetCellAnimationSequence(anim, seq);
}
void NNS_G2dInitCellAnimationVramTransfered(CellAnim *anim, void *seq, void *bank, s32 handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    NNSi_G2dInitCellTransferState(handle, a, b, c, *(u32 *)((u32 *)bank)[3], d, e, f);
    anim->transferHandle = handle;
    anim->bank = bank;
    *(volatile s32 *)&anim->transferHandle = handle;
    NNSi_G2dSrtcInitControl(&anim->srtc, 1);
    NNS_G2dInitAnimCtrl(anim);
    NNS_G2dSetCellAnimationSequence(anim, seq);
}
void NNS_G2dSetCellAnimationSequence(CellAnim *anim, void *seq) {
    NNS_G2dBindAnimCtrl(anim, seq);
    ApplyCurrentAnimResult_(anim);
}
void NNS_G2dTickCellAnimation(CellAnim *anim, u32 ticks) {
    if (NNS_G2dTickAnimCtrl(anim, ticks)) {
        ApplyCurrentAnimResult_(anim);
    }
}
void NNS_G2dSetCellAnimationCurrentFrame(CellAnim *anim, u32 frame) {
    if (NNS_G2dSetAnimCtrlCurrentFrame(anim, frame)) {
        ApplyCurrentAnimResult_(anim);
    }
}
