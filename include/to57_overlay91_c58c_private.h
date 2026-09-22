#ifndef TO57_NEW29_PRIVATE_H
#define TO57_NEW29_PRIVATE_H
#include "global.h"
typedef struct OverlayManager OverlayManager;
typedef struct Ov91Tail3 {
    u8 a, b, c;
} Ov91Tail3;
typedef struct Ov91Tail4 {
    u8 first;
    Ov91Tail3 rest;
} Ov91Tail4;
typedef struct Ov91Work {
    void *unk00;
    void *unk04;
    u8 unk08[8];
    u8 unk10;
    u8 pad11[7];
    u8 unk18[0x18];
    void *unk30;
    void *unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u16 unk44;
    u16 unk46;
    u8 unk48[0x28];
    u32 unk70;
    u32 unk74;
    u32 unk78;
    u32 unk7C;
    u32 unk80;
    Ov91Tail4 unk84;
} Ov91Work;
typedef struct Ov91Block16 {
    u32 w[4];
} Ov91Block16;
extern void *OverlayManager_GetData(OverlayManager *);
extern void *OverlayManager_GetArgs(OverlayManager *);
extern int ov90_02258B54(void *);
extern BOOL IsPaletteFadeFinished(void);
extern void sub_0200FB70(void);
extern void sub_0200FC20(int);
extern int ov90_02258B98(void *);
extern void ov91_0225CC4C(Ov91Work *, void *);
extern void GF_AssertFail(void);
extern void *ov90_02258914(void *, u32);
extern BOOL ov90_022589BC(void *);
extern u32 ov90_022589CC(void *);
extern void ov90_02258938(void *);
extern struct {
    u8 pad[0x44];
    u32 unk44;
} gSystem;
extern void ov91_0225CB58(void);
extern void Main_SetVBlankIntrCB(void (*)(void), void *);
extern void HBlankInterruptDisable(void);
extern void GF_CreateVramTransferManager(u32, u32);
extern void ov91_02261B2C(Ov91Work *);
extern u32 sub_020347A0(void);
extern u16 sub_0203769C(void);
extern u16 ov90_0225886C(void *, u16);
extern void *ov91_0225CCC4(u32, u32, u32, void *);
extern void *ov91_0225CDF4(u32, u32, u32, u16, void *);
extern void sub_0203A880(void);
extern void ov00_021E69A8(u32);
extern void ov91_0225D3C4(void *, void *);
extern void sub_02037AC0(u32);
extern BOOL sub_02037B38(u32);
extern void BeginNormalPaletteFade(u32, u32, u32, u16, u32, u32, u32);
extern BOOL ov91_0225CEB4(void *, u32);
extern BOOL sub_02037030(u32, void *, u32);
extern u32 ov91_0225D37C(void *);
extern u32 ov91_0225CCF4(void *);
extern BOOL ov91_0225CD5C(void *);
extern u32 ov91_0225CD6C(void *);
extern BOOL ov91_0225CCA8(Ov91Work *, u32, void *, u32);
extern void ov91_0225CD64(void *);
extern void ov91_0225D1DC(void *);
extern void ov91_0225CB64(Ov91Work *);
extern void ov91_0225CB98(Ov91Work *);
extern BOOL ov91_0225D078(void *, u32);
extern BOOL ov91_0225CDAC(void *);
extern void ov91_0225CDC4(void *, Ov91Block16 *);
extern void ov91_0225CCEC(void *);
extern void ov91_0225CE80(void *);
extern void GF_DestroyVramTransferManager(void);
extern void ov90_02258AA8(void *, u8);
extern void *ov90_022589E0(void *, void *, u32);
extern BOOL ov90_02258AA0(void *);
extern u32 ov90_02258AA4(void *);
extern void ov90_02258A04(void *);
int ov91_0225C58C(OverlayManager *, int *);
#endif
