#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))

extern int ov95_021E5EDC(void *);
extern int ov95_021E6150(void *, int);
extern void ov95_021E5EC0(void *, int);
extern void ov95_021E62F0(void *, int);
extern void ManagedSprite_SetAnimationFrame(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ManagedSprite_OffsetPositionXY(void *, s16, s16);
extern void *Sprite_GetPaletteProxy(void *);
extern u32 ObjPlttTransfer_GetPaletteVramOffset(void *, int);
extern int PaletteData_BeginPaletteFade(void *, int, u16, int, int, int, u16);
extern u16 PaletteData_GetSelectedBuffersBitmask(void *);
extern void Pokepic_StartPaletteFade(void *, int, int, int, int);
extern int Pokepic_ResumePaletteFade(void *);
extern void GfGfx_EngineATogglePlanes(int, int);
extern void PlaySE(int);

int ov95_021E6964(void *work);
int ov95_021E6B74(void *work);

int ov95_021E6964(void *work) {
    struct { s16 first; s16 second; } pos;
    int done;
    u32 status;
    u32 palette;

    switch (U32_AT(work, 0x68)) {
    case 0:
        if (ov95_021E5EDC(PTR_AT(work, 0x44)) == 1) {
            break;
        }
        ManagedSprite_SetAnimationFrame(PTR_AT(work, 0x74), 4);
        GfGfx_EngineATogglePlanes(2, 0);
        U32_AT(work, 0x68)++;
        break;
    case 1:
        status = ov95_021E6150(work, 3);
        if (status == 1) {
            ManagedSprite_SetAnimationFrame(PTR_AT(work, 0x74), 5);
            PlaySE(0x707);
            ov95_021E5EC0(PTR_AT(work, 0x44), 2);
            ov95_021E5EC0(PTR_AT(work, 0x44), 3);
        }
        if (status == 2) {
            ManagedSprite_SetDrawFlag(PTR_AT(work, 0x74), 0);
            U32_AT(work, 0x64) = 0;
            U32_AT(work, 0x68)++;
        }
        break;
    case 2:
        S32_AT(work, 0x64)++;
        if (S32_AT(work, 0x64) < 10) {
            break;
        }
        U32_AT(work, 0x64) = 0;
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 1, 0xffff, 0, 0, 16, 0xffff);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)PTR_AT(work, 0x78)), 1);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 4, (u16)(0xffff ^ (1 << palette)), 0, 0, 16, 0xffff);
        Pokepic_StartPaletteFade(PTR_AT(work, 0x70), 0, 16, 0, 0xffff);
        U32_AT(work, 0x68)++;
        break;
    case 3:
        done = 0;
        ManagedSprite_GetPositionXY(PTR_AT(work, 0x78), &pos.second, &pos.first);
        if (pos.first > -48) {
            ManagedSprite_OffsetPositionXY(PTR_AT(work, 0x78), 0, -4);
        } else {
            done++;
        }
        ManagedSprite_GetPositionXY(PTR_AT(work, 0x7c), &pos.second, &pos.first);
        if (pos.first < 192) {
            ManagedSprite_OffsetPositionXY(PTR_AT(work, 0x7c), 0, 4);
        } else {
            done++;
        }
        if (PaletteData_GetSelectedBuffersBitmask(PTR_AT(work, 8)) != 0 || done != 2 || Pokepic_ResumePaletteFade(PTR_AT(work, 0x70)) != 0) {
            break;
        }
        GfGfx_EngineATogglePlanes(2, 1);
        ov95_021E62F0(work, 0);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x74), 0);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x78), 0);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x7c), 0);
        U32_AT(work, 0x68)++;
        break;
    case 4:
        S32_AT(work, 0x64)++;
        if (S32_AT(work, 0x64) < 25) {
            break;
        }
        U32_AT(work, 0x64) = 0;
        Pokepic_StartPaletteFade(PTR_AT(work, 0x70), 16, 0, 0, 0xffff);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 1, 0xffff, 0, 16, 0, 0xffff);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 4, 0xffff, 0, 16, 0, 0xffff);
        U32_AT(work, 0x68)++;
        break;
    case 5:
        if (PaletteData_GetSelectedBuffersBitmask(PTR_AT(work, 8)) != 0 || Pokepic_ResumePaletteFade(PTR_AT(work, 0x70)) != 0) {
            break;
        }
        U32_AT(work, 0x68)++;
        break;
    default:
        if (ov95_021E5EDC(PTR_AT(work, 0x44)) == 1) {
            break;
        }
        U32_AT(work, 0x68) = 0;
        U32_AT(work, 0x64) = 0;
        return 0;
    }
    return 1;
}

int ov95_021E6B74(void *work) {
    struct { s16 first; s16 second; } pos;
    int done;
    u32 palette;

    switch (U32_AT(work, 0x68)) {
    case 0:
        ov95_021E5EC0(PTR_AT(work, 0x44), 0);
        ov95_021E5EC0(PTR_AT(work, 0x44), 1);
        ov95_021E5EC0(PTR_AT(work, 0x44), 2);
        ov95_021E5EC0(PTR_AT(work, 0x44), 3);
        GfGfx_EngineATogglePlanes(2, 0);
        U32_AT(work, 0x68)++;
        break;
    case 1:
        switch (U32_AT(work, 0x64)) {
        case 25:
            PlaySE(0x806);
            break;
        case 80:
            PlaySE(0x7aa);
            break;
        case 142:
            PlaySE(0x815);
            break;
        }
        S32_AT(work, 0x64)++;
        if (S32_AT(work, 0x64) < 160) {
            break;
        }
        U32_AT(work, 0x64) = 0;
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 1, 0xffff, 0, 0, 16, 0xffff);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)PTR_AT(work, 0x78)), 1);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 4, (u16)(0xffff ^ (1 << palette)), 0, 0, 16, 0xffff);
        Pokepic_StartPaletteFade(PTR_AT(work, 0x70), 0, 16, 0, 0xffff);
        U32_AT(work, 0x68)++;
        break;
    case 2:
        done = 0;
        ManagedSprite_GetPositionXY(PTR_AT(work, 0x78), &pos.second, &pos.first);
        if (pos.first > -48) {
            ManagedSprite_OffsetPositionXY(PTR_AT(work, 0x78), 0, -4);
        } else {
            done++;
        }
        ManagedSprite_GetPositionXY(PTR_AT(work, 0x7c), &pos.second, &pos.first);
        if (pos.first < 192) {
            ManagedSprite_OffsetPositionXY(PTR_AT(work, 0x7c), 0, 4);
        } else {
            done++;
        }
        if (PaletteData_GetSelectedBuffersBitmask(PTR_AT(work, 8)) != 0 || done != 2 || Pokepic_ResumePaletteFade(PTR_AT(work, 0x70)) != 0) {
            break;
        }
        ov95_021E62F0(work, 0);
        GfGfx_EngineATogglePlanes(2, 1);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x74), 0);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x78), 0);
        ManagedSprite_SetDrawFlag(PTR_AT(work, 0x7c), 0);
        U32_AT(work, 0x68)++;
        break;
    case 3:
        S32_AT(work, 0x64)++;
        if (S32_AT(work, 0x64) < 25) {
            break;
        }
        U32_AT(work, 0x64) = 0;
        Pokepic_StartPaletteFade(PTR_AT(work, 0x70), 16, 0, 0, 0xffff);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 1, 0xffff, 0, 16, 0, 0xffff);
        PaletteData_BeginPaletteFade(PTR_AT(work, 8), 4, 0xffff, 0, 16, 0, 0xffff);
        U32_AT(work, 0x68)++;
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(PTR_AT(work, 8)) != 0 || Pokepic_ResumePaletteFade(PTR_AT(work, 0x70)) != 0) {
            break;
        }
        U32_AT(work, 0x68)++;
        break;
    default:
        U32_AT(work, 0x68) = 0;
        U32_AT(work, 0x64) = 0;
        return 0;
    }
    return 1;
}
