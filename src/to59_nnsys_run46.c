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
extern Byte _021093FC[];
void NNS_G2dCharCanvasInitForBG(void *canvas, void *base, u32 p2, u32 p3, u32 p4);
u32 NNSi_G2dBitReaderRead(Byte *reader, int n);

void NNS_G2dCharCanvasInitForBG(void *canvas, void *base, u32 p2, u32 p3, u32 p4) {
    InitCharCanvas(canvas, base, p2, p3, p4, (void *)_021093FC, p2);
}
