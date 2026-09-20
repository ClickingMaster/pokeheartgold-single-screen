#ifndef TO54_X03_PRIVATE_H
#define TO54_X03_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "render_window.h"

typedef struct Ov91Work {
    void *root;
    u8 pad004[0x10 - 4];
    void *unk10;
    void *unk14;
    u8 pad018[0x1C - 0x18];
    void *spriteList;
    u8 pad020[0x1A0 - 0x20];
    u8 f01A0;
    u8 pad01A1[0x384 - 0x1A1];
    u8 f0384;
    u8 pad0385[0x4C0 - 0x385];
    u8 f04C0;
    u8 pad04C1[0x6F4 - 0x4C1];
    u8 f06F4;
    u8 pad06F5[0x19CC - 0x6F5];
    u8 f19CC;
    u8 pad19CD[0x1AB4 - 0x19CD];
    u8 f1AB4;
    u8 pad1AB5[0x1AD0 - 0x1AB5];
    void *unk1AD0;
    u8 pad1AD4[0x1C54 - 0x1AD4];
    u8 f1C54;
    u8 pad1C55[0x1E38 - 0x1C55];
    u8 f1E38;
    u8 pad1E39[0x1F38 - 0x1E39];
    u8 f1F38;
    u8 pad1F39[0x1F74 - 0x1F39];
    u8 f1F74;
    u8 pad1F75[0x21A4 - 0x1F75];
    void *unk21A4;
    u8 f21A8;
    u8 pad21A9[0x6A90 - 0x21A9];
    u8 f6A90;
    u8 pad6A91[0x6B78 - 0x6A91];
    u8 f6B78;
    u8 pad6B79[0x8544 - 0x6B79];
    u8 f8544;
    u8 pad8545[0x854C - 0x8545];
    u8 f854C;
    u8 pad854D[0x862C - 0x854D];
    u8 f862C;
    u8 pad862D[0x86CC - 0x862D];
    u8 f86CC;
    u8 pad86CD[0x877C - 0x86CD];
    void *unk877C;
} Ov91Work;

void ov91_0225F74C(Ov91Work *, Ov91Work *);
void ov91_0225F66C(Ov91Work *);
void ov91_0225F7A8(Ov91Work *, Ov91Work *);

void ov91_0226014C(void *, void *, void *);
void ov91_022601AC(void *, void *, void *);
void ov91_02260CC8(void *);
void ov91_022613C8(void *, void *);
void ov91_02261928(void *);

void ov91_02261790(void *, void *);
void ov91_0225D67C(void *, void *);
void ov91_0225DBE0(void *, void *);
void ov91_0225DB0C(void *, void *);
void ov91_0225FEFC(void *);
void ov91_02260D80(void *, void *);
void ov91_02260CA8(void *);
void ov91_0226066C(void *);
void ov91_022600FC(void *, void *);
void ov91_02261070(void *, void *, void *);
void ov91_0225FC84(void *);
void ov90_02258C38(void *);
void ov91_0225FA60(void *);
void ov91_0225FB48(void *);
void ov91_0225FD0C(void *);
void ov91_0225F4F8(void *);
void ov91_02260D98(void *);
void ov91_0225D6A0(void *);
void Thunk_G3X_Reset(void);
void ov91_0225FDD8(void *);
void NNS_G3dGlbMaterialColorDiffAmb(u16, u16, BOOL);
void NNS_G3dGlbMaterialColorSpecEmi(u16, u16, BOOL);
void NNS_G3dGeBufferOP_N(u32, const u32 *, u32);
void ov91_022610A8(void *);
void ov91_0226023C(void *, void *);
void ov91_0226078C(void *, void *);
void RequestSwap3DBuffers(int, int);
void SpriteList_RenderAndAnimateSprites(void *);
extern const GraphicsModes ov91_02261C74;
extern const BgTemplate ov91_02261C9C;
extern const BgTemplate ov91_02261CB8;
extern const BgTemplate ov91_02261CD4;
extern const BgTemplate ov91_02261CF0;
extern const BgTemplate ov91_02261D0C;
extern const BgTemplate ov91_02261D28;
void ov91_0225F878(BgConfig **, enum HeapID);
void LoadFontPal1(int, int, enum HeapID);
void ov91_0225F508(Ov91Work *, enum HeapID);
void ov91_0225F414(void);
void ov91_0225F4C0(void *, void *, void *, enum HeapID);
void ov91_0225FA9C(void *, enum HeapID);
void ov91_0225FCD8(void *, enum HeapID);
void ov91_0225FB80(void *, void *, enum HeapID);
void ov91_02260F50(void *, void *, void *, void *, void *, enum HeapID, void *);
void ov91_0225FF00(void *, void *, enum HeapID, void *);
void ov91_022604F4(void *, void *, enum HeapID);
void ov91_02260C48(void *, void *, void *, enum HeapID);
void ov91_02260D14(void *, void *, void *, enum HeapID);
void ov91_0225FDE0(void *, void *, void *, enum HeapID);
void ov91_0225D8F0(void *, void *, void *, void *, void *, void *, enum HeapID);
void ov91_0225DBD0(void *, void *, void *, enum HeapID);
void ov91_0225D628(void *, void *, void *, void *, enum HeapID);
void ov91_022613AC(void *, u8);
void ov91_02261580(void *, void *, void *, enum HeapID);
void *ov90_02258BD4(void *, enum HeapID);

#endif
