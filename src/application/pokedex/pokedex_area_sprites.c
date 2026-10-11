#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokemon_icon_idx.h"
#include "sprite_system.h"

extern const u16 ov18_021FA35A[];
extern u8 ov18_021FA3B0[]; // width and height of each area marker shape (not const: const lets loads move out of loops)
extern const ManagedSpriteTemplate sAreaMarkerSpriteTemplate;

#pragma force_active on

void PokedexApp_LoadAreaSpriteResources(PokedexAppData *pokedexApp) {
    SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 76, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC550);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->iconNarc, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0xC551);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->iconNarc, sub_0207449C(), FALSE, 0xC550);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->iconNarc, sub_020744A8(), FALSE, 0xC550);
    SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 115, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59C);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 118, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xC55E);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 116, TRUE, 0xC55A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 117, TRUE, 0xC55A);
    SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 119, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59A);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 122, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 0xC55C);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 120, TRUE, 0xC559);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 121, TRUE, 0xC559);
}

#pragma force_active reset

void PokedexApp_UnloadAreaSpriteResources(PokedexAppData *pokedexApp) {
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC550);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC551);
    SpriteManager_UnloadCellObjById(pokedexApp->spriteManager, 0xC550);
    SpriteManager_UnloadAnimObjById(pokedexApp->spriteManager, 0xC550);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC59C);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC55E);
    SpriteManager_UnloadCellObjById(pokedexApp->spriteManager, 0xC55A);
    SpriteManager_UnloadAnimObjById(pokedexApp->spriteManager, 0xC55A);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC59A);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC55C);
    SpriteManager_UnloadCellObjById(pokedexApp->spriteManager, 0xC559);
    SpriteManager_UnloadAnimObjById(pokedexApp->spriteManager, 0xC559);
}

void PokedexApp_UpdateTimeOfDayCursor(PokedexAppData *pokedexApp) {
    s16 y = pokedexApp->unk_18C9 * 32 + 76;
    PokedexApp_SetSpritePos(pokedexApp, 4, 32, y, 1);
}

void PokedexApp_SetupAreaListScrollbar(PokedexAppData *pokedexApp) {
    PokedexApp_SetScrollbarSizeAnim(pokedexApp, 5, (s8)pokedexApp->unk_18DC.unk_20.nMaps, ov18_021FA35A, 9);
    PokedexApp_SyncAreaScrollbar(pokedexApp);
    PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->unk_18CA, (s8)pokedexApp->unk_18DC.unk_20.nMaps, 6);
}

// The player's map position (args x, y in map blocks) marks the area map; the Kanto page starts at block 22.
void PokedexApp_UpdatePlayerMarker(PokedexAppData *pokedexApp) {
    PokedexArgs *args = pokedexApp->args;
    u16 x = args->x / 32;
    u16 y = args->y / 32;

    if (x >= 23) {
        x -= 22;
    }
    PokedexApp_SetSpritePos(pokedexApp, 2, x * 8 + 68, y * 8 + 44, 2);
    PokedexApp_UpdatePlayerMarkerVisibility(pokedexApp);
}

void PokedexApp_UpdatePlayerMarkerVisibility(PokedexAppData *pokedexApp) {
    if (pokedexApp->unk_18C8 == 0) {
        s32 x = pokedexApp->args->x / 32;
        if (x >= 23) {
            PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 2, TRUE);
        }
    } else {
        s32 x = pokedexApp->args->x / 32;
        if (x >= 23) {
            PokedexApp_SetSpriteVisible(pokedexApp, 2, TRUE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 2, FALSE);
        }
    }
}

void PokedexApp_CreateAreaMarkerSprites(PokedexAppData *pokedexApp) {
    u32 i;

    for (i = 9; i < 59; i++) {
        pokedexApp->sprites[i] = PokedexApp_NewSprite(pokedexApp, &sAreaMarkerSpriteTemplate);
        PokedexApp_SetSpriteTranslucent(pokedexApp, i, TRUE);
        PokedexApp_SetSpriteVisible(pokedexApp, i, FALSE);
    }
}

#pragma force_active on

// Position and shape of the marker of an overworld map. Rectangles come from the area layout table
// (unk_190C), except for three maps with fixed shapes.
void ov18_021F41C4(PokedexAppData *pokedexApp, int mapIndex, s16 *outX, s16 *outY, u8 *shape) {
    int x, y, width, height;

    if (ov18_021E8B24(mapIndex) == TRUE) {
        x = 32;
        y = 4;
        width = 1;
        height = 3;
    } else if (ov18_021E8B0C(mapIndex) == 18) {
        x = pokedexApp->unk_190C[mapIndex].unk_00;
        y = pokedexApp->unk_190C[mapIndex].unk_01;
        width = 1;
        height = 2;
    } else if (ov18_021E8B5C(mapIndex) == TRUE) {
        x = 35;
        y = 8;
        width = 2;
        height = 1;
    } else {
        x = pokedexApp->unk_190C[mapIndex].unk_00;
        y = pokedexApp->unk_190C[mapIndex].unk_01;
        width = pokedexApp->unk_190C[mapIndex].unk_02;
        height = pokedexApp->unk_190C[mapIndex].unk_03;
    }
    if (width == 1) {
        if (height == 1) {
            *shape = 0;
        } else if (height == 2) {
            *shape = 6;
        } else if (height == 3) {
            *shape = 7;
        } else if (height == 4) {
            *shape = 8;
        } else if (height == 5) {
            *shape = 9;
        }
    } else if (width == 2) {
        if (height == 1) {
            *shape = 1;
        } else {
            *shape = 10;
        }
    } else if (width == 3) {
        if (height == 1) {
            *shape = 2;
        } else {
            *shape = 11;
        }
    } else if (width == 4) {
        *shape = 3;
    } else if (width == 5) {
        *shape = 4;
    } else if (width == 6) {
        *shape = 5;
    } else {
        *shape = 0;
    }
    *outX = (s16)((s32)((u32)(x - 22 * pokedexApp->unk_18C8) << 3) + (s32)((u32)width << 3) / 2 + 64);
    *outY = (s16)((s32)((u32)y << 3) + (s32)((u32)height << 3) / 2 + 40);
}

#pragma force_active reset

// Position and shape of the marker of a dungeon map (unk_1908 holds one block position per dungeon).
void ov18_021F42E4(PokedexAppData *pokedexApp, u32 mapIndex, s16 *outX, s16 *outY, u8 *shape) {
    u32 mapId;

    *outX = (pokedexApp->unk_1908[mapIndex].unk_00 - pokedexApp->unk_18C8 * 22) * 8 + 68;
    *outY = pokedexApp->unk_1908[mapIndex].unk_01 * 8 + 44;
    mapId = ov18_021E8B18(mapIndex);

    if (mapId == 0x7C || mapId == 0xB2 || mapId == 0xB3) {
        *shape = 6;
        *outY += 4;
        return;
    }
    if (mapId == 0x60 || mapId == 0x1E7) {
        *shape = 0;
        *outX += 4;
        *outY += 4;
        return;
    }
    if (mapId == 0x71 || mapId == 0x13B || (u32)(mapId - 0x1EA) <= 2) {
        *shape = 6;
        *outY += 4;
        return;
    }
    *shape = 0;
}

typedef struct PokedexAreaMarker {
    u8 shape;
    s16 y;
    s16 x;
} PokedexAreaMarker;

#pragma force_active on

// Places a marker on every map where the species lives (or only on the selected list entry), and
// switches the player marker's animation when it lies inside one of them.
void PokedexApp_UpdateAreaMarkers(PokedexAppData *pokedexApp) {
    struct {
        PokedexAreaMarker marker;
        s16 playerY;
        s16 playerX;
        u32 specialAreas;
    } local;

    int hit = FALSE;
    u16 i;

    PokedexApp_GetSpritePos(pokedexApp, 2, &local.playerX, &local.playerY, 2);
    local.specialAreas = 0;
    if (pokedexApp->unk_18CA == 0) {
        for (i = 1; i < pokedexApp->unk_18DC.unk_20.nMaps; i++) {
            if (!ov18_021E8AB0(pokedexApp, i)) {
                ov18_021F41C4(pokedexApp, pokedexApp->unk_18DC.unk_20.maps[i], &local.marker.x, &local.marker.y, &local.marker.shape);
            } else {
                ov18_021F42E4(pokedexApp, pokedexApp->unk_18DC.unk_20.maps[i], &local.marker.x, &local.marker.y, &local.marker.shape);
                ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[i]), &local.specialAreas);
            }
            PokedexApp_SetSpritePos(pokedexApp, i + 8, local.marker.x, local.marker.y, 2);
            PokedexApp_SetSpriteAnim(pokedexApp, i + 8, local.marker.shape);
            PokedexApp_SetSpriteVisible(pokedexApp, i + 8, TRUE);
            {
                u8 *size = ov18_021FA3B0 + local.marker.shape * 2;
                int halfWidth = size[0] / 2;
                if (local.playerX >= local.marker.x - halfWidth && local.playerX < local.marker.x + halfWidth) {
                    int halfHeight = size[1] / 2;
                    if (local.playerY >= local.marker.y - halfHeight && local.playerY < local.marker.y + halfHeight) {
                        hit = TRUE;
                    }
                }
            }
        }
        {
            int next = i + 8;
            ov18_021F47F8(pokedexApp, local.specialAreas, next);
            if (!hit && local.specialAreas) {
                hit = ov18_021F4974(pokedexApp, next, local.playerX, local.playerY);
            }
        }
    } else {
        PokedexApp_HideAreaMarkers(pokedexApp);
        if (!ov18_021E8AB0(pokedexApp, pokedexApp->unk_18CA)) {
            ov18_021F41C4(pokedexApp, pokedexApp->unk_18DC.unk_20.maps[pokedexApp->unk_18CA], &local.marker.x, &local.marker.y, &local.marker.shape);
            PokedexApp_SetSpritePos(pokedexApp, 9, local.marker.x, local.marker.y, 2);
            PokedexApp_SetSpriteAnim(pokedexApp, 9, local.marker.shape);
            PokedexApp_SetSpriteVisible(pokedexApp, 9, TRUE);
            {
                int halfWidth = ov18_021FA3B0[local.marker.shape * 2] / 2;
                if (local.playerX >= local.marker.x - halfWidth && local.playerX < local.marker.x + halfWidth) {
                    int halfHeight = (ov18_021FA3B0 + 1)[local.marker.shape * 2] / 2;
                    if (local.playerY >= local.marker.y - halfHeight && local.playerY < local.marker.y + halfHeight) {
                        hit = TRUE;
                    }
                }
            }
        } else {
            ov18_021F42E4(pokedexApp, pokedexApp->unk_18DC.unk_20.maps[pokedexApp->unk_18CA], &local.marker.x, &local.marker.y, &local.marker.shape);
            ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[pokedexApp->unk_18CA]), &local.specialAreas);
            PokedexApp_SetSpritePos(pokedexApp, 9, local.marker.x, local.marker.y, 2);
            PokedexApp_SetSpriteAnim(pokedexApp, 9, local.marker.shape);
            PokedexApp_SetSpriteVisible(pokedexApp, 9, TRUE);
            {
                int halfWidth = ov18_021FA3B0[local.marker.shape * 2] / 2;
                if (local.playerX >= local.marker.x - halfWidth && local.playerX < local.marker.x + halfWidth) {
                    int halfHeight = (ov18_021FA3B0 + 1)[local.marker.shape * 2] / 2;
                    if (local.playerY >= local.marker.y - halfHeight && local.playerY < local.marker.y + halfHeight) {
                        hit = TRUE;
                    }
                }
            }
            ov18_021F47F8(pokedexApp, local.specialAreas, 10);
            if (!hit && local.specialAreas) {
                hit = ov18_021F4974(pokedexApp, 10, local.playerX, local.playerY);
            }
        }
    }
    PokedexApp_SetPlayerMarkerAnim(pokedexApp, hit);
}

#pragma force_active reset

void PokedexApp_HideAreaMarkers(PokedexAppData *pokedexApp) {
    u32 i;
    for (i = 9; i < 59; i++) {
        PokedexApp_SetSpriteVisible(pokedexApp, i, FALSE);
    }
}

#pragma force_active on

// Shows the markers for the current time of day (unk_18CB_6 picks the palette); maps without
// encounters at that time are hidden.
void ov18_021F463C(PokedexAppData *pokedexApp) {
    u32 specialAreas = 0;
    u32 hiddenSpecialAreas = 0;
    u16 i;

    if (pokedexApp->unk_18CA == 0) {
        for (i = 1; i < pokedexApp->unk_18DC.unk_20.nMaps; i++) {
            if (ov18_021E8ACC(pokedexApp, i, pokedexApp->unk_18CB_6)) {
                ManagedSprite_SetPaletteOverride(pokedexApp->sprites[i + 8], pokedexApp->unk_18CB_6 + 4);
                PokedexApp_SetSpriteVisible(pokedexApp, i + 8, TRUE);
            } else {
                PokedexApp_SetSpriteVisible(pokedexApp, i + 8, FALSE);
                if (ov18_021E8AB0(pokedexApp, i)) {
                    ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[i]), &hiddenSpecialAreas);
                }
            }
            if (ov18_021E8AB0(pokedexApp, i)) {
                ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[i]), &specialAreas);
            }
        }
        ov18_021F48AC(pokedexApp, specialAreas, hiddenSpecialAreas, i + 8, pokedexApp->unk_18CB_6 + 4);
    } else {
        PokedexApp_HideAreaMarkers(pokedexApp);
        if (ov18_021E8ACC(pokedexApp, pokedexApp->unk_18CA, pokedexApp->unk_18CB_6)) {
            ManagedSprite_SetPaletteOverride(pokedexApp->sprites[9], pokedexApp->unk_18CB_6 + 4);
            PokedexApp_SetSpriteVisible(pokedexApp, 9, TRUE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 9, FALSE);
            if (ov18_021E8AB0(pokedexApp, pokedexApp->unk_18CA)) {
                ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[pokedexApp->unk_18CA]), &hiddenSpecialAreas);
            }
        }
        if (ov18_021E8AB0(pokedexApp, pokedexApp->unk_18CA)) {
            ov18_021F47C0(ov18_021E8B18(pokedexApp->unk_18DC.unk_20.maps[pokedexApp->unk_18CA]), &specialAreas);
        }
        ov18_021F48AC(pokedexApp, specialAreas, hiddenSpecialAreas, 10, pokedexApp->unk_18CB_6 + 4);
    }
}

#pragma force_active reset

// Maps drawn with extra fixed markers instead of their own rectangle: bit 0, bit 1 and bit 2 select
// one, one and two extra markers.
void ov18_021F47C0(u32 mapId, u32 *specialAreas) {
    if (mapId == 0x6A) {
        *specialAreas |= 1;
    } else if (mapId == 0x78 || (mapId - 0xED) <= 2) {
        *specialAreas |= 2;
    } else if (mapId == 0x7B || mapId == 0xB0) {
        *specialAreas |= 4;
    }
}

void ov18_021F47F8(PokedexAppData *pokedexApp, u32 specialAreas, int index) {
    if (specialAreas & 1) {
        PokedexApp_SetSpritePos(pokedexApp, index, 0x94, 0x4C, 2);
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0);
        PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
        index++;
    }
    if (specialAreas & 2) {
        PokedexApp_SetSpritePos(pokedexApp, index, 0xEC, 0x4C, 2);
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0);
        PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
        index++;
    }
    if (specialAreas & 4) {
        PokedexApp_SetSpritePos(pokedexApp, index, 0xE4, 0x5C, 2);
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0);
        PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
        PokedexApp_SetSpritePos(pokedexApp, index + 1, 0xDC, 0x7C, 2);
        PokedexApp_SetSpriteAnim(pokedexApp, index + 1, 0);
        PokedexApp_SetSpriteVisible(pokedexApp, index + 1, TRUE);
    }
}

void ov18_021F48AC(PokedexAppData *pokedexApp, u32 specialAreas, u32 hiddenSpecialAreas, int index, u32 palette) {
    if (specialAreas & 1) {
        if (hiddenSpecialAreas & 1) {
            PokedexApp_SetSpriteVisible(pokedexApp, index, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
            ManagedSprite_SetPaletteOverride(pokedexApp->sprites[index], palette);
        }
        index++;
    }
    if (specialAreas & 2) {
        if (hiddenSpecialAreas & 2) {
            PokedexApp_SetSpriteVisible(pokedexApp, index, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
            ManagedSprite_SetPaletteOverride(pokedexApp->sprites[index], palette);
        }
        index++;
    }
    if (specialAreas & 4) {
        if (hiddenSpecialAreas & 4) {
            PokedexApp_SetSpriteVisible(pokedexApp, index, FALSE);
            PokedexApp_SetSpriteVisible(pokedexApp, index + 1, FALSE);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
            PokedexApp_SetSpriteVisible(pokedexApp, index + 1, TRUE);
            ManagedSprite_SetPaletteOverride(pokedexApp->sprites[index], palette);
            ManagedSprite_SetPaletteOverride((pokedexApp->sprites + 1)[index], palette);
        }
    }
}

BOOL ov18_021F4974(PokedexAppData *pokedexApp, u32 start, s32 x, s32 y) {
    u32 i;
    u32 end;
    s16 pos[2];
    u8 *walker;

    i = start;
    end = start + 4;
    if (i < end) {
        // a byte pointer walks the struct (indexing sprites[i] directly changes the code)
        walker = (u8 *)pokedexApp + i * sizeof(ManagedSprite *);
        do {
            if (ManagedSprite_GetDrawFlag(((PokedexAppData *)walker)->sprites[0]) != FALSE) {
                PokedexApp_GetSpritePos(pokedexApp, i, &pos[1], &pos[0], 2);
                if (x >= pos[1] - (s32)((u32)ov18_021FA3B0[0] >> 1) && x < pos[1] + (s32)((u32)ov18_021FA3B0[0] >> 1)
                    && y >= pos[0] - (s32)((u32)ov18_021FA3B0[1] >> 1) && y < pos[0] + (s32)((u32)ov18_021FA3B0[1] >> 1)) {
                    return TRUE;
                }
            }
            i++;
            walker += sizeof(ManagedSprite *);
        } while (i < end);
    }
    return FALSE;
}
