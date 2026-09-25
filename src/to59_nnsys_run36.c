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

void NNS_G2dInitRenderer(void *renderer) {
    int i;
    u8 *cursor;
    NNS_G2dInitRndCore(renderer);
    WORD(renderer, 0x70) = 0;
    WORD(renderer, 0x74) = 0;
    WORD(renderer, 0x78) = 0;
    WORD(renderer, 0x7c) = 0;
    WORD(renderer, 0x80) = 0;
    WORD(renderer, 0x84) = 0;
    *(unsigned short *)((u8 *)renderer + 0x88) = 0;
    *(unsigned short *)((u8 *)renderer + 0x8a) = 0;
    WORD(renderer, 0x8c) = 0;
    WORD(renderer, 0x90) = 0;
    *(unsigned short *)((u8 *)renderer + 0x94) = 0;
    WORD(_021D94D8, 0xc) = 0;
    _021109DC = 0xfffe;
    cursor = _021D9C68;
    for (i = 0; i < 32; i++, cursor += 0x20) {
        MIi_CpuClearFast(0xffffffff, cursor + 0x10, 0x10);
    }
    *(unsigned short *)_021D94D8 = 0;
    *(unsigned short *)(_021D94D8 + 2) = 0;
    MIi_CpuClearFast(0, _021D9568, 0x100);
    WORD(_021D94D8, 4) = 0;
}

void NNS_G2dAddRendererTargetSurface(void *renderer, void *surface) {
    PTR(surface, 0x30) = PTR(renderer, 0x70);
    PTR(renderer, 0x70) = surface;
}

void NNS_G2dInitRenderSurface(void *surface) {
    MIi_CpuClear16(0, surface, 0x48);
    WORD(surface, 0x10) = 1;
    WORD(surface, 0x14) = 3;
    PTR(surface, 0x18) = (void *)RndCoreCBFuncBeforeCell_;
    PTR(surface, 0x1c) = (void *)RndCoreCBFuncAfterCell_;
    PTR(surface, 0x20) = (void *)RndCoreCBFuncBeforeOBJ_;
    PTR(surface, 0x24) = (void *)RndCoreCBFuncAfterOBJ_;
}
