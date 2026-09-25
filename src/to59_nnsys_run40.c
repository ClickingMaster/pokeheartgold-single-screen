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
void *NNSi_G2dGetCellTransferState(int handle) {
    return *(u8 **)(_021DA470 + 8) + handle * 0x30;
}

void NNSi_G2dInitCellTransferState(int handle, int a, int b, int c, u32 d, u32 e, u32 f, u32 g) {
    u8 *state = *(u8 **)(_021DA470 + 8) + handle * 0x30;
    NNSi_G2dInitializeVRamLocation(state);
    if (a != -1) {
        NNSi_G2dSetVramLocation(state, 0, a);
    }
    if (b != -1) {
        NNSi_G2dSetVramLocation(state, 1, b);
    }
    if (c != -1) {
        NNSi_G2dSetVramLocation(state, 2, c);
    }
    *(u32 *)(state + 0xc) = d;
    *(u32 *)(state + 0x10) = e;
    *(u32 *)(state + 0x14) = f;
    *(u32 *)(state + 0x18) = g;
}

void NNS_G2dInitCellTransferStateManager(void *states, u32 count, void *callback) {
    u8 *state = states;
    u32 *manager = (u32 *)_021DA470;
    u32 i;
    manager[0] = (u32)callback;
    manager[1] = count;
    manager[2] = (u32)states;
    for (i = 0; i < count; i++, state += 0x30) {
        u32 *words = (u32 *)(state + 0xc);
        NNSi_G2dInitializeVRamLocation(state);
        words[0] = 0;
        words[1] = 0;
        words[2] = 0;
        words[3] = 0;
        words[4] = 0;
        words[5] = 0;
        words[6] = 0;
        words[7] = 0;
        words[8] = 0;
    }
}
