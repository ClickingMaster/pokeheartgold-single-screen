#ifndef POKEHEARTGOLD_OVERLAY96_UI_RESOURCES_57_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY96_UI_RESOURCES_57_PRIVATE_H

#include "global.h"

#include "pokemon.h"
#include "sprite_transfer.h"
#include "unk_02013FDC.h"

typedef struct R57Window {
    u8 raw[0x10];
} R57Window;

typedef struct R57Vec {
    s32 x, y, z;
} R57Vec;

extern void GF_AssertFail(void);
extern void PokeathlonCirclePushDisplay_PrintRound(void *, u32);
extern void PokeathlonCirclePushDisplayAnim_Stop(void *);
extern u32 PokeathlonCirclePushDisplayAnim_TakeNewMon(void *, u32);
extern u32 PokeathlonCirclePushDisplayAnim_TryStart(void *, u32);
extern void PokeathlonCirclePushDisplayAnim_StartScoreboard(void *, void *);
extern void PokeathlonCirclePushDisplayAnim_StartMonPic(void *, void *, u32);
extern void PokeathlonCirclePushDisplayAnim_StartIdle(void *, void *);
extern void ov96_021EB52C(void *, u32, u32);
extern void ov96_021EB2BC(void *, u32, u32, u32, u32);
extern void ov96_021EB2F4(void *, u32, u32, u32, u32, u32);
extern void ov96_021EB334(void *, u32, u32, u32);
extern void ov96_021EB36C(void *, u32, u32, u32);
extern void ov96_021EB408(void *, u32, u32, u32, u32);
extern void *ov96_021EB4F4(void *, u32, u32);
extern void *ov96_021EB5B8(void *);
extern void **ov96_021EB5EC(void *, u32, u32);
extern void FillWindowPixelBuffer(void *, u32);
extern void CopyWindowToVram(void *);
extern void AddWindow(void *, void *, const void *);
extern void BG_FillCharDataRange(void *, u32, u32, u32, u32);
extern void LoadFontPal0(u32, u32, u32);
extern void BufferIntegerAsString(void *, u32, u32, u32, u32, u32);
extern void *ReadMsgData_ExpandPlaceholders(void *, void *, u32, u32);
extern void AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, void *);
extern void PlaySE(u32);
extern u32 ov96_021E5F24(void *);

extern const u8 ov96_0221C028[];
extern const u32 ov96_0221BFF4[];

typedef struct R57ParticipantView {
    u16 species;
    u16 field2;
    u16 special;
    u8 kind;
    u8 field7;
    u32 participant;
    u32 fieldC;
} R57ParticipantView;

extern void ov96_021E6168(void *, u32, u32, R57ParticipantView *);

void PokeathlonCirclePushDisplay_SetRound(void *, u32);
void ov96_021F3E60(void *);
void ov96_021F3EC0(void *, void *);
void PokeathlonCirclePushDisplay_ShowPlayerEvent(void *, u32, u32);
void PokeathlonCirclePushDisplay_ResetAnim(void *);
void PokeathlonCirclePushDisplay_GetPicVramLocations(void *, void *);
void PokeathlonCirclePushDisplay_LoadMonPics(void *);
void PokeathlonCirclePushDisplayAnim_LoadMonPic(void *, u32, u32);
void PokeathlonCirclePushDisplay_CreateWindows(void *);
void PokeathlonCirclePushDisplay_Update(void *, u32, u32);
void PokeathlonCirclePushDisplay_SetScore(void *, u32, u32);

#endif // POKEHEARTGOLD_OVERLAY96_UI_RESOURCES_57_PRIVATE_H
