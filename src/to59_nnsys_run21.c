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

void *NNS_G2dGetMultiCellDataByIdx(void *bank, u16 idx) {
    if (idx < H(bank, 0)) {
        return (u8 *)W(bank, 4) + idx * 8;
    }
    return 0;
}

int NNS_G2dGetUnpackedCharacterData(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x43484152);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackNCG(block + 8);
    *out = block + 8;
    return 1;
}

void NNS_G2dUnpackNCG(void *p) {
    W(p, 0x14) += (u32)p;
}

int NNS_G2dGetUnpackedBGCharacterData(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x43484152);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackBGNCG(block + 8);
    *out = block + 8;
    return 1;
}

void NNS_G2dUnpackBGNCG(void *p) {
    W(p, 0x14) += (u32)p;
}

int NNS_G2dGetUnpackedPaletteData(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x504c5454);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackNCL(block + 8);
    *out = block + 8;
    return 1;
}

int NNS_G2dGetUnpackedPaletteCompressInfo(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x50434d50);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNSi_G2dUnpackNCLCmpInfo(block + 8);
    *out = block + 8;
    return 1;
}

void NNSi_G2dUnpackNCLCmpInfo(void *p) {
    W(p, 4) += (u32)p;
}

void NNS_G2dUnpackNCL(void *p) {
    W(p, 0xc) += (u32)p;
}

int NNS_G2dGetUnpackedScreenData(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x5343524e);
    if (block != 0) {
        *out = block + 8;
        return 1;
    }
    *out = 0;
    return 0;
}

void *NNS_G2dFindBinaryBlock(void *file, u32 signature) {
    u8 *block = (u8 *)file + H(file, 0xc);
    u16 count = H(file, 0xe);
    u16 i = 0;
    for (; i < count; i++) {
        if (W(block, 0) == signature) {
            return block;
        }
        block += W(block, 4);
    }
    return 0;
}
