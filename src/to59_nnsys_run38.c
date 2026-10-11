#pragma thumb off
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
extern u8 gCellTransferStateManager[];
extern u8 gOamSoftEmuState[];
extern u32 gOamSoftEmuAutoZOffsetStep;
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
void NNS_G2dDrawMultiCellAnimation(void *anim) {
    u8 *p = anim;
    if (*(u16 *)(p + 0x5a) == 0) {
        NNS_G2dDrawMultiCell(p + 0x34);
        return;
    }
    NNS_G2dPushMtx();
    if (*(u16 *)(p + 0x5a) & 8) {
        int x;
        int y = *(s16 *)(p + 0x56);
        x = *(s16 *)(p + 0x54);
        if (NNS_G2dIsRndCoreFlipH(*(void **)(_021D94D8 + 8))) {
            x = -x;
        }
        if (NNS_G2dIsRndCoreFlipV(*(void **)(_021D94D8 + 8))) {
            y = -y;
        }
        NNS_G2dTranslate(x << 12, y << 12, 0);
    }
    if (*(u16 *)(p + 0x5a) & 4) {
        u32 idx = (*(u16 *)(p + 0x58) >> 4) << 1;
        NNS_G2dRotZ(FX_SinCosTable_[idx], FX_SinCosTable_[idx + 1]);
    }
    if (*(u16 *)(p + 0x5a) & 2) {
        NNS_G2dScale(*(int *)(p + 0x4c), *(int *)(p + 0x50), 0x1000);
    }
    NNS_G2dDrawMultiCell(p + 0x34);
    NNS_G2dPopMtx(1);
}
