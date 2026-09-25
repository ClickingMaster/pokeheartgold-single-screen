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

int NNS_G2dSetAnimCtrlCurrentFrame(AnimCtrl *ctrl, u16 frame) {
    int result = SetAnimCtrlCurrentFrameImpl_(ctrl, frame);
    if (result) {
        frame = 0;
        ctrl->elapsed = frame;
    }
    return result;
}
