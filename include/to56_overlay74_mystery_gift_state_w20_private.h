#ifndef TO56_R2_W20_PRIVATE_H
#define TO56_R2_W20_PRIVATE_H
#include "global.h"

#include "constants/sndseq.h"

#include "overlay_manager.h"
#include "system.h"
#define M2C_FIELD(p, t, o) (*(t)((u8 *)(p) + (o)))
typedef struct Ov74E1F4Work {
    u8 unk0000[0x29FC];
    void *bgConfig;
    u8 unk2A00[0xF8];
    u8 window2AF8[0x10];
    u8 window2B08[0x98];
    void *mysteryGift;
    void *saveData;
    void *options;
    s32 frame;
    u8 unk2BB0[4];
    void *cards[3];
    s32 index;
    u8 unk2BC4[0xC];
    u8 window2BD0[0x10];
    u8 window2BE0[0x10];
    s32 printer;
    u8 unk2BF4[0x3C];
    s32 shown;
    s32 timer;
    u8 unk2C38[8];
    u32 nextState;
    s32 renderSprites;
    u8 unk2C48[0x190];
    u8 buffer2DD8[0xF70];
    void (*callback)(void *);
    u8 unk3D4C[4];
    void *waitingIcon;
} Ov74E1F4Work;
void ResetAllTextPrinters(void);
void LoadFontPal0(int, int, int);
void LoadUserFrameGfx1(void *, int, int, int, int, int);
void LoadUserFrameGfx2(void *, int, int, int, int, int);
void PlaySE(u32);
void GfGfx_EngineATogglePlanes(int, int);
void DrawFrameAndWindow2(void *, int, int, int);
void DrawFrameAndWindow1(void *, int, int, int);
void ClearWindowTilemapAndCopyToVram(void *);
void RemoveWindow(void *);
void FillWindowPixelBuffer(void *, int);
void CopyWindowToVram(void *);
BOOL IsPaletteFadeFinished(void);
void SpriteList_RenderAndAnimateSprites(void);
void *Save_MysteryGift_Get(void *);
void *Save_PlayerData_GetOptionsAddr(void *);
u8 Options_GetFrame(void *);
void *SaveMysteryGift_CardGetByIdx(void *, int);
int ov74_0222DAF8(void *, int, int);
void ov74_0222FCA4(void);
void ov74_0222FCC4(void *);
void ov74_0222DB70(void *, int);
void ov74_0222D824(void *, int, int);
void ov74_0222D7F0(void *, int, int, int *);
int ov74_0222D9E0(void *, void *, int, int);
void ov74_0222DC60(void *, int *, void *);
void ov74_0222DE8C(void *, int, int, u32);
int ov74_0222DF2C(void);
void ov74_0222DEF0(void *);
void ov74_0222D448(void);
void ov74_0222D104(void *, int);
void ov74_0222D0EC(void *, int);
void ov74_0222D7A4(void *, void *, int);
void ov74_0222D248(void *);
void ov74_0222CFFC(void *);
void ov74_0222D098(void *);
void ov74_0222E7EC(void);
void sub_0203A880(void);
int sub_020373B4(int);
void sub_020358B8(void *);
int ov74_0222E8B4(void *, void *);
void ov74_0222EA88(void *, void *, int);
void ov74_0222E0D4(void *, int, int *);
void ov74_0222D024(void);
int sub_02037D78(void);
void ov74_02235ED0(void *, void *, int);
void ov74_02229D0C(void *, int);
void ov74_0222E060(void *);
int ov74_0223615C(void);
void sub_02037AC0(int);
void sub_020398D4(int, int);
int ov74_0222E898(void);
int sub_02037B38(int);
void ov74_02236128(void);
void sub_0200F450(void *);
void ov74_0222EB28(void *, int *, int);
void ov74_022358BC(void);
void ov74_0222D414(void);
int ov74_0222E1F4(OverlayManager *, int *);
#endif
