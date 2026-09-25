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

void DrawOneQuad_(u32 u0, u32 u1, u32 v0, u32 v1) {
    u16 a = (s32)(u0 << 8) >> 16;
    u16 b = (s32)(u1 << 8) >> 16;
    u16 c = (s32)(v0 << 8) >> 16;
    u16 d = (short)(v1 >> 8);
    volatile u32 *tex = (volatile u32 *)0x04000488;
    volatile u32 *vtx = (volatile u32 *)0x0400048C;
    *(volatile u32 *)0x04000500 = 1;
    *tex = a | (d << 16);
    *vtx = 0x10000000;
    *vtx = 0;
    *tex = b | (d << 16);
    *vtx = 0x10001000;
    *vtx = 0;
    *tex = b | (c << 16);
    *vtx = 0x1000;
    *vtx = 0;
    *tex = a | (c << 16);
    *vtx = 0;
    *vtx = 0;
    *(volatile u32 *)0x04000504 = 0;
}
