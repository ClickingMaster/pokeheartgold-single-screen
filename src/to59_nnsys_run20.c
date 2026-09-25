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

void *NNS_G2dGetAnimSequenceByIdx(void *bank, u16 idx) {
    if (H(bank, 0) > idx) {
        return (u8 *)W(bank, 4) + idx * 16;
    }
    return 0;
}

void *GetPtrOamArrayHead_(void *bank) {
    if (H(bank, 2) & 1) {
        return (u8 *)W(bank, 4) + H(bank, 0) * 16;
    }
    return (u8 *)W(bank, 4) + H(bank, 0) * 8;
}

void UnPackExtendedData_(void *p) {
    NNSi_G2dUnpackUserExCellAttrBank((u8 *)p + 8);
}

int NNS_G2dGetUnpackedCellBank(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x4345424b);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackNCE(block + 8);
    *out = block + 8;
    return 1;
}

void *NNS_G2dGetCellDataByIdx(void *bank, u16 idx) {
    if (idx >= H(bank, 0)) {
        return 0;
    }
    if (H(bank, 2) & 1) {
        return (u8 *)W(bank, 4) + idx * 16;
    }
    return (u8 *)W(bank, 4) + idx * 8;
}

void NNS_G2dUnpackNCE(void *bank) {
    u8 *b = bank;
    u8 *oam;
    u16 i;
    W(b, 4) += (u32)b;
    oam = GetPtrOamArrayHead_(b);
    i = 0;
    if (H(b, 0) > i) {
        do {
            u8 *cell = NNS_G2dGetCellDataByIdx(b, i);
            W(cell, 4) += (u32)oam;
            i++;
        } while (i < H(b, 0));
    }
    if (W(b, 0xc) != 0) {
        W(b, 0xc) += (u32)b;
        u8 *extended = (u8 *)W(b, 0xc);
        W(extended, 4) += (u32)extended;
        W(b, 0xc) = (u32)extended;
    }
    if (W(b, 0x14) == 0) {
        return;
    }
    W(b, 0x14) += (u32)b;
    UnPackExtendedData_((void *)W(b, 0x14));
}

int NNS_G2dGetUnpackedMultiCellBank(void *file, void **out) {
    u8 *block = NNS_G2dFindBinaryBlock(file, 0x4d43424b);
    if (block == 0) {
        *out = 0;
        return 0;
    }
    NNS_G2dUnpackNMC(block + 8);
    *out = block + 8;
    return 1;
}

void UnpackExtendedData_(void *p) {
    NNSi_G2dUnpackUserExCellAttrBank((u8 *)p + 8);
}
