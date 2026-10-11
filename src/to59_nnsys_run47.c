#include <nitro/code32.h>
#include <nitro/types.h>

typedef unsigned char Byte;
typedef struct {
    Byte pad[0x1a];
    u16 map[1];
} HintMap;
typedef struct {
    u8 type;
    u8 pad;
    u16 kind;
    void (*callback)(Byte *, Byte *, void *);
} AnmInitEntry;

u16 NNSi_G2dSplitCharUTF16(const u16 **p);
u32 NNS_G3dAnmObjCalcSizeRequired(const Byte *anm, const Byte *model);
void NNS_G3dAnmObjEnableID(Byte *obj, int id);
void NNS_G3dAnmObjDisableID(Byte *obj, int id);
void updateHintVec_(u32 *hints, Byte *anm);
void MIi_CpuClear32(u32, void *, u32);
extern u32 _02110A20, _02110A1C, _02110A18;
void NNS_G3dRenderObjInit(Byte *obj, void *model);
void NNS_G2dMapScrToChar256x16Pltt(u16 *dst, int width, int height, int stride, u32 base, u32 palette);
u32 NNSi_G2dCalcRequiredOBJ(u32 x, u32 y);
extern u32 _02110A00;
extern Byte _02110A24[];
extern Byte _02110A28[];
void NNS_G3dAnmObjInit(Byte *obj, Byte *anm, void *model, void *tex);
void InitCharCanvas(void *, void *, u32, u32, u32, void *, u32);
extern Byte gCharCanvasVTableBG[];
void NNS_G2dCharCanvasInitForBG(void *canvas, void *base, u32 p2, u32 p3, u32 p4);
u32 NNSi_G2dBitReaderRead(Byte *reader, int n);

u32 NNSi_G2dBitReaderRead(Byte *reader, int n) {
    int available = *(s8 *)(reader + 4);
    u8 bits = reader[5];
    u32 value;
    if (available < n) {
        Byte *ptr = *(Byte **)reader;
        int remaining = n - available;
        *(Byte **)reader = ptr + 1;
        reader[5] = *ptr;
        reader[4] = 8;
        value = NNSi_G2dBitReaderRead(reader, remaining) | (bits << remaining);
    } else {
        int left = available - n;
        value = (u32)bits >> left;
        *(s8 *)(reader + 4) = left;
    }
    return value & ((s32)0xff >> (8 - n));
}

u16 NNSi_G2dSplitCharUTF16(const u16 **p) {
    u16 c = **p;
    ++*p;
    return c;
}

u32 NNS_G3dAnmObjCalcSizeRequired(const Byte *anm, const Byte *model) {
    if (anm == 0 || model == 0) {
        return 0;
    }
    switch (anm[0]) {
    case 0x4d:
        return (model[0x18] * 2 + 0x1c) & ~3;
    case 0x4a:
    case 0x56:
        return (model[0x17] * 2 + 0x1c) & ~3;
    default:
        return 0;
    }
}
