typedef unsigned char u8;
typedef unsigned int u32;

#define WORD(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define PTR(p, n)  (*(void **)((u8 *)(p) + (n)))

extern u8 _021D94D8[];
extern u8 _021D9C68[];
extern u8 _021D9568[];
extern unsigned short _021109DC;
extern void MIi_CpuClear16(u32, void *, u32);
extern void MIi_CpuClearFast(u32, void *, u32);
extern void NNS_G2dInitRndCore(void *);
extern void NNSi_G2dIdentity(void);
extern void RndCoreCBFuncBeforeCell_(void);
extern void RndCoreCBFuncAfterCell_(void);
extern void RndCoreCBFuncBeforeOBJ_(void);
extern void RndCoreCBFuncAfterOBJ_(void);
extern void NNS_G2dRndCoreEndRendering(void);
extern int NNSi_G2dGetOamSoftEmuAutoZOffsetStep(void);
extern void NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(int);
extern void NNSi_G2dSetOamSoftEmuAutoZOffsetStep(int);
extern void NNSi_G2dResetOamSoftEmuAutoZOffset(void);
extern void DrawCellAnimationImpl_(void *);

void NNS_G2dAddRendererTargetSurface(void *, void *);
void NNS_G2dInitRenderSurface(void *);
void NNS_G2dEndRendering(void);
void NNS_G2dDrawCellAnimation(void *);
void NNS_G2dInitRenderer(void *);
void NNS_G2dBeginRendering(void *);

void NNS_G2dEndRendering(void) {
    *(volatile u32 *)0x04000448 = 1;
    u32 flags = WORD(PTR(_021D94D8, 8), 0x7c);
    void *renderer = PTR(_021D94D8, 8);
    if (flags != 0) {
        if (flags & 1) {
            WORD(_021D94D8, 4) = 0;
        }
        if (flags & 2) {
            WORD(renderer, 0x74) = 0;
            NNS_G2dRndCoreEndRendering();
        }
        WORD(PTR(_021D94D8, 8), 0x7c) = 0;
    }
    PTR(_021D94D8, 8) = 0;
}

void NNS_G2dDrawCellAnimation(void *animation) {
    if (PTR(PTR(_021D94D8, 8), 0x80) != 0) {
        int prev = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(1);
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep((int)PTR(PTR(_021D94D8, 8), 0x80));
        DrawCellAnimationImpl_(animation);
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(0);
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep(prev);
        NNSi_G2dResetOamSoftEmuAutoZOffset();
    } else {
        DrawCellAnimationImpl_(animation);
    }
}
