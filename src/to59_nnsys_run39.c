#pragma thumb off
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
extern u8 _021DA470[];
extern u8 _021DA47C[];
extern u32 _021109FC;
void NNS_G2dSetRndCoreImageProxy(void *, const void *, const void *);
void NNSi_G2dInitializeVRamLocation(void *);
void NNSi_G2dSetVramLocation(void *, int, int);
void *NNSi_G2dGetCellTransferState(int);
void *NNS_G2dGetRendererPaletteTbl(void *);
void NNS_G2dSetRendererImageProxy(void *, const void *, const void *);
void NNSi_G2dInitCellTransferState(int, int, int, int, u32, u32, u32, u32);
void NNS_G2dInitCellTransferStateManager(void *, u32, void *);
void NNS_G2dSetCellTransferStateRequested(int, u32, u32);
int NNS_G2dGetNewCellTransferStateHandle(void);
void NNS_G2dFreeCellTransferStateHandle(int);
void NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(u32);
void NNSi_G2dResetOamSoftEmuAutoZOffset(void);
void NNSi_G2dSetOamSoftEmuAutoZOffsetStep(u32);
u32 NNSi_G2dGetOamSoftEmuAutoZOffsetStep(void);
typedef signed short s16;
extern u8 _021D94D8[];
extern s16 FX_SinCosTable_[];
void NNS_G2dDrawMultiCell(void *);
void NNS_G2dPushMtx(void);
void NNS_G2dPopMtx(int);
int NNS_G2dIsRndCoreFlipH(void *);
int NNS_G2dIsRndCoreFlipV(void *);
void NNS_G2dTranslate(int, int, int);
void NNS_G2dRotZ(int, int);
void NNS_G2dScale(int, int, int);
void NNS_G2dDrawMultiCellAnimation(void *);
void *NNS_G2dGetRendererPaletteTbl(void *renderer) {
    return *(void **)((u8 *)renderer + 0x78);
}

void NNS_G2dSetRendererImageProxy(void *renderer, const void *image, const void *palette) {
    NNS_G2dSetRndCoreImageProxy(renderer, image, palette);
}
