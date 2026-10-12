#include "global.h"

#include "constants/species.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "heap.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "char_tiles.h"

extern const ManagedSpriteTemplate ov18_021FAC28[];
extern const u16 ov18_021FA348[];
extern const u8 ov18_021FA304[]; // area list scrollbar length per size animation
extern const u8 ov18_021FA310[]; // form list scrollbar length per size animation
extern const UnkStruct_02014E30 ov18_021FA338;
extern const u16 ov18_021FB5B4[];

void PokedexApp_SetupFormsSprites(PokedexAppData *pokedexApp) {
    PokedexApp_LoadMonIconResources(pokedexApp, 6);
    PokedexApp_LoadMonPicResources(pokedexApp);
    ov18_021F193C(pokedexApp);
    PokedexApp_CreateFormsSprites(pokedexApp);
}

void PokedexApp_FreeFormsSprites(PokedexAppData *pokedexApp) {
    u32 i;

    for (i = 1; i < 20; i++) {
        PokedexApp_DeleteSprite(pokedexApp, i);
    }
    PokedexApp_UnloadMonIconResources(pokedexApp, 6);
    PokedexApp_UnloadMonPicResources(pokedexApp);
    ov18_021F19EC(pokedexApp);
}

#pragma force_active on

// Sprites 1-4: the two pictures of the selected form (two sets, swapped on every change); 5-8 scrollbar and
// arrows; 9-13 the compare screen's selector; 14-19 the form list icons.
void PokedexApp_CreateFormsSprites(PokedexAppData *pokedexApp) {
    u32 i;

    for (i = 1; i <= 4; i++) {
        pokedexApp->sprites[i] = SpriteSystem_NewSpriteWithYOffset(pokedexApp->spriteSystem, pokedexApp->spriteManager, &ov18_021FAC28[i - 1], FX32_CONST(512));
    }
    for (i = 5; i < 20; i++) {
        pokedexApp->sprites[i] = SpriteSystem_NewSprite(pokedexApp->spriteSystem, pokedexApp->spriteManager, &ov18_021FAC28[i - 1]);
    }
    PokedexApp_SetSpriteVisible(pokedexApp, 9, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 10, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 11, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 12, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 13, FALSE);
    PokedexApp_ShowFormMonPics(pokedexApp, 0, 0);
    PokedexApp_DrawFormListIcons(pokedexApp);
    PokedexApp_SetScrollbarSizeAnim(pokedexApp, 5, pokedexApp->numSeenForms, ov18_021FA348, 9);
    PokedexApp_SyncFormScrollbar(pokedexApp);
    PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
}

#pragma force_active reset

void ov18_021F5EF0(PokedexAppData *pokedexApp, int index) {
    PokedexApp_ShowFormMonPics(pokedexApp, index, 0);
}

// Front picture (left) and back picture (right) of seen form index, drawn into the sprite set not on screen.
void PokedexApp_ShowFormMonPics(PokedexAppData *pokedexApp, int index, int mode) {
    u8 gender;
    u8 form;
    int front;
    int back;
    int height;

    PokedexApp_GetSeenFormAndGender(pokedexApp, index, &form, &gender);
    if (!pokedexApp->formPicSwapA) {
        front = 1;
        back = 2;
        PokedexApp_SetSpriteVisible(pokedexApp, 3, FALSE);
        PokedexApp_SetSpriteVisible(pokedexApp, 4, FALSE);
    } else {
        front = 3;
        back = 4;
        PokedexApp_SetSpriteVisible(pokedexApp, 1, FALSE);
        PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
    }
    pokedexApp->formPicSwapA ^= 1;
    PokedexApp_LoadMonPicToSprite(pokedexApp, pokedexApp->curSpecies, form, gender, 2, front, mode);
    ManagedSprite_SetPositionXYWithSubscreenOffset(pokedexApp->sprites[front], 0x40, 0x78, FX32_CONST(512));
    PokedexApp_LoadMonPicToSprite(pokedexApp, pokedexApp->curSpecies, form, gender, 0, back, mode);
    height = GetMonPicHeightBySpeciesGenderForm(pokedexApp->curSpecies, gender, 0, form, 0);
    ManagedSprite_SetPositionXYWithSubscreenOffset(pokedexApp->sprites[back], 0xC0, height + 0x78, FX32_CONST(512));
    PokedexApp_SetSpriteVisible(pokedexApp, front, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, back, TRUE);
}

void PokedexApp_SetFormListIcon(PokedexAppData *pokedexApp, int sprite, int index) {
    int value;
    int raw;
    int form;

    value = pokedexApp->seenForms[index];
    raw = value;
    if (raw & 0x80) {
        int species = pokedexApp->curSpecies;
        form = value ^ 0x80;
        if (species == SPECIES_PICHU) {
            if (form == 2) {
                form = 1;
            } else {
                form = 0;
            }
        }
    } else {
        form = 0;
    }
    PokedexApp_SetMonIcon(pokedexApp, pokedexApp->curSpecies, form, sprite);
}

// Six icons, the selected form in the middle (third).
void PokedexApp_DrawFormListIcons(PokedexAppData *pokedexApp) {
    int i = 0;
    int y = 14;
    u32 entry;

    do {
        entry = pokedexApp->selectedForm + i - 2;
        if (entry >= (u32)pokedexApp->numSeenForms) {
            PokedexApp_SetSpriteVisible(pokedexApp, i + 14, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, i + 14, TRUE);
            PokedexApp_SetFormListIcon(pokedexApp, i + 14, entry);
        }
        PokedexApp_SetSpritePos(pokedexApp, i + 14, 0x30, (s16)y, 0);
        i++;
        y += 24;
    } while ((u32)i < 6);
}

// Recycles the icon that scrolled out (y -10 or 134) for the entry coming in.
void PokedexApp_ScrollFormListIcons(PokedexAppData *pokedexApp, int direction) {
    int i = 0;
    int subscreen = 0;
    s16 x;
    s16 y;

    do {
        PokedexApp_GetSpritePos(pokedexApp, i + 14, &x, &y, subscreen);
        if (y == -10 || y == 134) {
            break;
        }
        i++;
    } while ((u32)i < 6);
    if (direction < 0) {
        PokedexApp_SetSpritePos(pokedexApp, i + 14, 0x30, 134, 0);
        if (pokedexApp->selectedForm + 2 >= pokedexApp->numSeenForms) {
            i += 14;
            PokedexApp_SetSpriteVisible(pokedexApp, i, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, i + 14, TRUE);
            i += 14;
            PokedexApp_SetFormListIcon(pokedexApp, i, pokedexApp->selectedForm + 2);
        }
    } else {
        PokedexApp_SetSpritePos(pokedexApp, i + 14, 0x30, -10, 0);
        if (pokedexApp->selectedForm - 2 < 0) {
            i += 14;
            PokedexApp_SetSpriteVisible(pokedexApp, i, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, i + 14, TRUE);
            i += 14;
            PokedexApp_SetFormListIcon(pokedexApp, i, pokedexApp->selectedForm - 2);
        }
    }
}

void PokedexApp_MoveFormListIcons(PokedexAppData *pokedexApp, int dy) {
    u32 i;

    for (i = 0; i < 6; i++) {
        PokedexApp_MoveSprite(pokedexApp, i + 14, 0, dy, 0);
    }
}

// Arrow sprites sprite (up) and sprite + 1 (down) are greyed out at the ends of the list.
void PokedexApp_UpdateScrollArrows(PokedexAppData *pokedexApp, s8 index, s8 count, int sprite) {
    if (index == 0) {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite, 7);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite, 5);
    }
    if (index == count - 1) {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite + 1, 10);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite + 1, 8);
    }
}

void PokedexApp_SetScrollbarSizeAnim(PokedexAppData *pokedexApp, int sprite, int count, const u16 *thresholds, u32 numSizes) {
    PokedexApp_SetSpriteAnim(pokedexApp, sprite, PokedexApp_GetScrollbarSizeAnim(pokedexApp, count, thresholds, numSizes));
}

// The scrollbar's size animations start at 14; the first threshold not below count picks one.
int PokedexApp_GetScrollbarSizeAnim(PokedexAppData *pokedexApp, int count, const u16 *thresholds, u32 numSizes) {
    u32 i = 0;

    if (numSizes > i) {
        do {
            u16 threshold = *thresholds;
            if (count <= threshold) {
                break;
            }
            i++;
            thresholds++;
        } while (i < numSizes);
    }
    return i + 14;
}

u32 PokedexApp_GetScrollbarLength(PokedexAppData *pokedexApp, int sprite, const u8 *lengths) {
    u32 anim = ManagedSprite_GetActiveAnim(pokedexApp->sprites[sprite]);
    return lengths[anim - 14];
}

#pragma force_active on

// Highest scrollbar centre in a track of trackLength pixels around centre.
int PokedexApp_GetScrollbarMinY(PokedexAppData *pokedexApp, int sprite, const u8 *lengths, int centre, u32 trackLength) {
    u32 length = PokedexApp_GetScrollbarLength(pokedexApp, sprite, lengths);
    return centre - (trackLength >> 1) + (length >> 1);
}

#pragma force_active reset

int PokedexApp_GetScrollbarMaxY(PokedexAppData *pokedexApp, int sprite, const u8 *lengths, int centre, u32 trackLength) {
    return centre + (trackLength >> 1) - (PokedexApp_GetScrollbarLength(pokedexApp, sprite, lengths) >> 1);
}

BOOL PokedexApp_IsTouchOnFormScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 spriteX;
    s16 spriteY;
    u32 length;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &spriteX, &spriteY);
    length = PokedexApp_GetScrollbarLength(pokedexApp, 5, ov18_021FA310);
    if (x >= spriteX - 11 && x <= spriteX + 11 && y >= spriteY - (length >> 1) && y <= spriteY + (length >> 1)) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokedexApp_IsTouchOnAreaScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 spriteX;
    s16 spriteY;
    u32 length;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &spriteX, &spriteY);
    length = PokedexApp_GetScrollbarLength(pokedexApp, 5, ov18_021FA304);
    if (x >= spriteX - 11 && x <= spriteX + 11 && y >= spriteY - (length >> 1) && y <= spriteY + (length >> 1)) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Moves the scrollbar to the touch y (clamped to its track) and selects the form whose segment it lies in;
// TRUE if the selection changed.
BOOL PokedexApp_DragFormScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 spriteX, oldY;
    u32 min, max, segments, width, position, index, lower, upper;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &spriteX, &oldY);
    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA310, 64, 86);
    if (y < min) {
        y = min;
    }
    max = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA310, 64, 86);
    if (y > max) {
        y = max;
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[5], spriteX, (s16)y);
    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA310, 64, 86);
    max = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA310, 64, 86);
    max -= min;
    segments = pokedexApp->numSeenForms - 1;
    width = (max << 8) / segments;
    position = (y - min) << 8;
    index = 0;
    lower = index;
    upper = index;
    for (; index <= segments; index++) {
        if (position >= lower && position < upper + width) {
            if (pokedexApp->selectedForm != index) {
                pokedexApp->selectedForm = index;
                return TRUE;
            }
            break;
        }
        lower += width;
        upper += width;
    }
    return FALSE;
}

BOOL PokedexApp_DragAreaScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 spriteX, oldY;
    u32 min, max, segments, width, position, index, lower, upper;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &spriteX, &oldY);
    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA304, 96, 86);
    if (y < min) {
        y = min;
    }
    max = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA304, 96, 86);
    if (y > max) {
        y = max;
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[5], spriteX, (s16)y);
    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA304, 96, 86);
    max = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA304, 96, 86);
    max -= min;
    segments = pokedexApp->areaList.unk_20.nMaps - 1;
    width = (max << 8) / segments;
    position = (y - min) << 8;
    index = 0;
    lower = index;
    upper = index;
    for (; index <= segments; index++) {
        if (position >= lower && position < upper + width) {
            if (pokedexApp->areaListPos != index) {
                pokedexApp->areaListPos = index;
                return TRUE;
            }
            break;
        }
        lower += width;
        upper += width;
    }
    return FALSE;
}

#pragma force_active reset

u32 PokedexApp_GetFormScrollbarY(PokedexAppData *pokedexApp, int index) {
    u32 min;
    u32 y;
    int last;
    u32 step;
    u32 offset;

    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA310, 0x40, 0x56);
    y = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA310, 0x40, 0x56);
    last = pokedexApp->numSeenForms - 1;
    if (index != last) {
        step = ((y - min) << 8) / last;
        offset = step * index;
        y = min + (offset >> 8);
    }
    return y;
}

u32 PokedexApp_GetAreaScrollbarY(PokedexAppData *pokedexApp, int index) {
    u32 min;
    u32 y;
    int last;
    u32 step;
    u32 offset;

    min = PokedexApp_GetScrollbarMinY(pokedexApp, 5, ov18_021FA304, 0x60, 0x56);
    y = PokedexApp_GetScrollbarMaxY(pokedexApp, 5, ov18_021FA304, 0x60, 0x56);
    last = pokedexApp->areaList.unk_20.nMaps - 1;
    if (index != last) {
        step = ((y - min) << 8) / last;
        offset = step * index;
        y = min + (offset >> 8);
    }
    return y;
}

// Distance from sprite's y to y, in units of step.
u32 PokedexApp_GetScrollbarStepCount(PokedexAppData *pokedexApp, int sprite, u32 y, u32 step) {
    s16 spriteX;
    s16 spriteY;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[sprite], &spriteX, &spriteY);
    if (y >= spriteY) {
        return (y - spriteY) / step;
    } else {
        return (spriteY - y) / step;
    }
}

void PokedexApp_MoveScrollbar(PokedexAppData *pokedexApp, int sprite, int dy) {
    s16 x;
    s16 y;
    ManagedSprite **sprites = pokedexApp->sprites;

    ManagedSprite_GetPositionXY(sprites[sprite], &x, &y);
    ManagedSprite_SetPositionXY(sprites[sprite], x, y + dy);
}

void PokedexApp_SyncFormScrollbar(PokedexAppData *pokedexApp) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[5], x, PokedexApp_GetFormScrollbarY(pokedexApp, pokedexApp->selectedForm));
}

void PokedexApp_SyncAreaScrollbar(PokedexAppData *pokedexApp) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(pokedexApp->sprites[5], &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[5], x, PokedexApp_GetAreaScrollbarY(pokedexApp, pokedexApp->areaListPos));
}

void PokedexApp_ShowFormListSprites(PokedexAppData *pokedexApp) {
    PokedexApp_ShowFormMonPics(pokedexApp, pokedexApp->selectedForm, 1);
    PokedexApp_DrawFormListIcons(pokedexApp);
    PokedexApp_SyncFormScrollbar(pokedexApp);
    PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
    PokedexApp_SetSpriteVisible(pokedexApp, 5, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, 6, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, 7, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, 8, TRUE);
}

void PokedexApp_HideFormListSprites(PokedexAppData *pokedexApp) {
    PokedexApp_SetSpriteVisible(pokedexApp, 5, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 6, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 7, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 8, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 14, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 15, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 16, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 17, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 18, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 19, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 1, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 3, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 4, FALSE);
}

// Compare screen: the forms selectedForm and compareForm side by side.
void PokedexApp_ShowFormCompareSprites(PokedexAppData *pokedexApp) {
    if (pokedexApp->numSeenForms >= 3) {
        PokedexApp_SetSpriteVisible(pokedexApp, 9, TRUE);
        PokedexApp_SetSpriteVisible(pokedexApp, 10, TRUE);
        PokedexApp_SetSpriteVisible(pokedexApp, 11, TRUE);
        PokedexApp_SetSpriteVisible(pokedexApp, 12, TRUE);
        PokedexApp_SetSpriteVisible(pokedexApp, 13, TRUE);
    }
    PokedexApp_SetCompareFormIcon(pokedexApp, 14, pokedexApp->selectedForm);
    PokedexApp_SetCompareFormIcon(pokedexApp, 15, pokedexApp->compareForm);
    PokedexApp_SetSpritePos(pokedexApp, 14, 0x40, 0x50, 0);
    PokedexApp_SetSpritePos(pokedexApp, 15, 0xC0, 0x50, 0);
    PokedexApp_ShowFormCompareMonPic(pokedexApp, 1, pokedexApp->selectedForm, 1);
    PokedexApp_ShowFormCompareMonPic(pokedexApp, 2, pokedexApp->compareForm, 1);
    PokedexApp_UpdateFormCompareSelector(pokedexApp);
    PokedexApp_SetSpriteVisible(pokedexApp, 14, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, 15, TRUE);
}

void PokedexApp_HideFormCompareSprites(PokedexAppData *pokedexApp) {
    PokedexApp_SetSpriteVisible(pokedexApp, 9, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 10, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 11, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 12, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 13, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 14, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 15, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 1, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 3, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 4, FALSE);
}

void PokedexApp_SetCompareFormIcon(PokedexAppData *pokedexApp, int sprite, int index) {
    PokedexApp_SetFormListIcon(pokedexApp, sprite, index);
}

#pragma force_active on

// side 1 (left) or 2 (right) of the compare screen; unk_18C7_7 shows the back pictures.
void PokedexApp_ShowFormCompareMonPic(PokedexAppData *pokedexApp, int side, int index, int mode) {
    struct {
        u8 form, gender;
    } attributes;

    u8 x, y;
    u8 facing;
    int offset;

    PokedexApp_GetSeenFormAndGender(pokedexApp, index, &attributes.form, &attributes.gender);
    if (!pokedexApp->unk_18C7_7) {
        facing = 2;
        offset = 0;
    } else {
        facing = 0;
        offset = GetMonPicHeightBySpeciesGenderForm(pokedexApp->curSpecies, attributes.gender, 0, attributes.form, 0);
    }
    if (side == 1) {
        x = 64;
        y = offset + 120;
        if (pokedexApp->formPicSwapA == 1) {
            side = 3;
            PokedexApp_SetSpriteVisible(pokedexApp, 1, FALSE);
            PokedexApp_SetSpriteVisible(pokedexApp, side, TRUE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 1, TRUE);
            PokedexApp_SetSpriteVisible(pokedexApp, 3, FALSE);
        }
        pokedexApp->formPicSwapA = (u8)(pokedexApp->formPicSwapA ^ 1);
    } else if (side == 2) {
        x = 192;
        y = offset + 120;
        if (pokedexApp->formPicSwapB == 1) {
            side = 4;
            PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
            PokedexApp_SetSpriteVisible(pokedexApp, side, TRUE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 2, TRUE);
            PokedexApp_SetSpriteVisible(pokedexApp, 4, FALSE);
        }
        pokedexApp->formPicSwapB = (u8)(pokedexApp->formPicSwapB ^ 1);
    }
    PokedexApp_LoadMonPicToSprite(pokedexApp, pokedexApp->curSpecies, attributes.form, attributes.gender, facing, side, mode);
    PokedexApp_SetSpritePos(pokedexApp, side, x, y, 1);
}

#pragma force_active reset

void ov18_021F6984(PokedexAppData *pokedexApp, int side, int index) {
    PokedexApp_ShowFormCompareMonPic(pokedexApp, side, index, 0);
}

void PokedexApp_UpdateFormCompareSelector(PokedexAppData *pokedexApp) {
    if (pokedexApp->compareSide == 0) {
        PokedexApp_SetSpritePos(pokedexApp, 13, 0x40, 0x58, 0);
    } else {
        PokedexApp_SetSpritePos(pokedexApp, 13, 0xC0, 0x58, 0);
    }
}

// The player marker's animation: boy / girl, plus 2 while the player stands in an area of the species.
void PokedexApp_SetPlayerMarkerAnim(PokedexAppData *pokedexApp, int inArea) {
    int anim;

    if (PlayerProfile_GetTrainerGender(pokedexApp->args->playerProfile) == PLAYER_GENDER_MALE) {
        anim = 0;
    } else {
        anim = 1;
    }
    if (inArea == TRUE) {
        anim += 2;
    }
    PokedexApp_SetSpriteAnim(pokedexApp, 2, anim);
}

#pragma force_active on

// Draws the Pokemon picture as 8bpp tiles into BG 7 (the size check's affine background).
void PokedexApp_LoadMonPicToBg(PokedexAppData *pokedexApp, u16 species, u8 form, u8 gender, u8 facing) {
    PokepicTemplate pokepic;
    UnkStruct_02014E30 rect = ov18_021FA338;
    u8 *buffer = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, 3200);
    u32 personality;
    u8 *converted;

    if (species == SPECIES_SPINDA) {
        personality = Pokedex_GetSeenSpindaPersonality(pokedexApp->args->pokedex, 0);
    } else {
        personality = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, species, gender, facing, 0, form, personality);
    sub_02014510((NarcId)pokepic.narcID, pokepic.charDataID, HEAP_ID_POKEDEX_APP, &rect, buffer, personality, FALSE, 2, species);
    converted = Convert4bppTo8bpp(buffer, 3200, 15, HEAP_ID_POKEDEX_APP);
    BG_LoadCharTilesData(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, converted, 6400, 1);
    Heap_Free(converted);
    Heap_Free(buffer);
    LoadRectToBgTilemapRect(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, ov18_021FB5B4, 11, 11, 10, 10);
    ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active reset

// Scales BG 7 by 256 / scale around its centre and moves it to y.
void PokedexApp_SetMonPicBgAffine(PokedexAppData *pokedexApp, int y, int scale) {
    MtxFx22 mtx;
    fx32 value = FX_Inv(FX_Div(0x100000, scale << 12));

    mtx._00 = value;
    mtx._11 = value;
    mtx._01 = 0;
    mtx._10 = 0;
    Bg_SetTextDimAndAffineParams(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, 0x38, &mtx, 0x80, 0x80);
    Bg_SetTextDimAndAffineParams(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0x10 - y, &mtx, 0x80, 0x80);
}
