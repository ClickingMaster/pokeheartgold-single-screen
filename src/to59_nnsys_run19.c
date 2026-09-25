typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))

void *NNS_G2dFindBinaryBlock(void *, u32);
void NNS_G2dUnpackNAN(void *);
void NNS_G2dUnpackNCE(void *);
void NNS_G2dUnpackNMC(void *);
void NNS_G2dUnpackNCG(void *);
void NNS_G2dUnpackBGNCG(void *);
void NNS_G2dUnpackNCL(void *);
void NNSi_G2dUnpackNCLCmpInfo(void *);
void NNSi_G2dUnpackUserExCellAttrBank(void *);
void *NNS_G2dGetCellDataByIdx(void *, u16);

int GetUnpackedAnimBankImpl_(void *file, void **out);
int NNS_G2dGetUnpackedAnimBank(void *file, void **out);
int NNS_G2dGetUnpackedMCAnimBank(void *file, void **out);
void *NNS_G2dGetAnimSequenceByIdx(void *bank, u16 idx);
void *GetPtrOamArrayHead_(void *bank);
void UnPackExtendedData_(void *p);
int NNS_G2dGetUnpackedCellBank(void *file, void **out);
void *NNS_G2dGetCellDataByIdx(void *bank, u16 idx);
int NNS_G2dGetUnpackedMultiCellBank(void *file, void **out);
void UnpackExtendedData_(void *p);
void *NNS_G2dGetMultiCellDataByIdx(void *bank, u16 idx);
int NNS_G2dGetUnpackedCharacterData(void *file, void **out);
void NNS_G2dUnpackNCG(void *p);
int NNS_G2dGetUnpackedBGCharacterData(void *file, void **out);
void NNS_G2dUnpackBGNCG(void *p);
int NNS_G2dGetUnpackedPaletteData(void *file, void **out);
int NNS_G2dGetUnpackedPaletteCompressInfo(void *file, void **out);
void NNSi_G2dUnpackNCLCmpInfo(void *p);
void NNS_G2dUnpackNCL(void *p);
int NNS_G2dGetUnpackedScreenData(void *file, void **out);

int GetUnpackedAnimBankImpl_(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x41424e4b);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackNAN(block + 8);
    *out = block + 8;
    return 1;
}

int NNS_G2dGetUnpackedAnimBank(void *file, void **out) {
    return GetUnpackedAnimBankImpl_(file, out);
}

int NNS_G2dGetUnpackedMCAnimBank(void *file, void **out) {
    return GetUnpackedAnimBankImpl_(file, out);
}
