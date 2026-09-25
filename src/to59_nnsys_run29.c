typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))

typedef struct AnimFrame {
    u32 element;
    u16 duration;
    u16 pad;
} AnimFrame;
typedef struct AnimSeq {
    u16 count;
    u16 loop;
    u32 unknown;
    u32 type;
    AnimFrame *frames;
} AnimSeq;
typedef struct AnimCtrl {
    AnimFrame *current;
    AnimFrame *shown;
    u32 direction;
    u32 active;
    s32 elapsed;
    s32 speed;
    u32 typeOverride;
    AnimSeq *sequence;
    u32 callbackType;
    void *callbackArg;
    void (*callback)(void *, s32);
    u16 frame;
} AnimCtrl;

void NNS_G2dTickAnimCtrl(AnimCtrl *, s32);
void NNS_G2dResetAnimCtrlState(AnimCtrl *);
void NNS_G2dInitAnimCallBackFunctor(void *);
void MIi_CpuCopy16(const void *, void *, u32);
void NNSi_G2dUnpackUserExCellAttrBank(void *);
int SetAnimCtrlCurrentFrameImpl_(AnimCtrl *, u16);
u32 NNS_G2dGetAnimCtrlCurrentElement(AnimCtrl *);
int NNS_G2dSetAnimCtrlCurrentFrame(AnimCtrl *, u16);
u16 NNS_G2dGetAnimCtrlCurrentFrame(AnimCtrl *);
void NNS_G2dInitAnimCtrl(AnimCtrl *);
void NNS_G2dBindAnimCtrl(AnimCtrl *, AnimSeq *);
int NNSi_G2dIsAnimCtrlLoopAnim(AnimCtrl *);
void CpuLoadOAMSub_(const void *, u32, u32);
void CpuLoadOAMMain_(const void *, u32, u32);
void DrawOneQuad_(u32, u32, u32, u32);
void NNS_G2dDrawSpriteFast(short, short, int, int, int, int, int, int, int);
void SequenceEdgeHandle_(AnimCtrl *);

void NNS_G2dBindAnimCtrl(AnimCtrl *ctrl, AnimSeq *seq) {
    ctrl->sequence = seq;
    NNS_G2dResetAnimCtrlState(ctrl);
}

int NNSi_G2dIsAnimCtrlLoopAnim(AnimCtrl *ctrl) {
    u32 type = ctrl->typeOverride;
    if (type == 0) {
        type = ctrl->sequence->type;
    }
    return type == 2 || type == 4;
}

void CpuLoadOAMSub_(const void *src, u32 dest, u32 size) {
    MIi_CpuCopy16(src, (void *)(dest + 0x07000400), size);
}

void CpuLoadOAMMain_(const void *src, u32 dest, u32 size) {
    MIi_CpuCopy16(src, (void *)(dest + 0x07000000), size);
}
