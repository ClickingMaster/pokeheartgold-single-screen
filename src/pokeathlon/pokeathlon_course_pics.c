#include "global.h"

#include "pokeathlon/pokeathlon_course_ceremony.h"
#include "pokeathlon/pokeathlon_course_ui.h"

#include "assert.h"
#include "char_tiles.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"

static void PokeathlonCourse_LoadMonPicEx(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 grayScale, enum HeapID heapId, BOOL loadPalette);
static void PokeathlonCourse_LoadSpriteCharsAt(Sprite *sprite, void *data, u32 size, u32 offset);
static void PokeathlonCourse_LoadSpriteChars(Sprite *sprite, void *data, u32 size);

static inline void *GetTexDictData(NNSG3dResTex *tex, u32 index) {
    if (tex) {
        return NNS_G3dGetResDataByIdx(&tex->dict, index);
    }
    return NULL;
}

// The trainer pic of an NPC trainer class.
typedef struct PokeathlonNpcTrainerPic {
    int trainerClass;
    int fileId;
} PokeathlonNpcTrainerPic;

static const PokeathlonNpcTrainerPic sPokeathlonNpcTrainerPics[253] = {
    { 1,      0     },
    { 2,      1     },
    { 3,      2     },
    { 4,      3     },
    { 5,      4     },
    { 6,      5     },
    { 7,      6     },
    { 8,      7     },
    { 9,      8     },
    { 10,     9     },
    { 11,     10    },
    { 12,     12    },
    { 13,     13    },
    { 14,     14    },
    { 15,     16    },
    { 16,     17    },
    { 17,     18    },
    { 18,     19    },
    { 19,     34    },
    { 20,     35    },
    { 21,     71    },
    { 22,     31    },
    { 23,     32    },
    { 24,     23    },
    { 25,     24    },
    { 29,     25    },
    { 30,     26    },
    { 31,     36    },
    { 33,     22    },
    { 34,     37    },
    { 35,     38    },
    { 36,     39    },
    { 37,     40    },
    { 38,     20    },
    { 39,     21    },
    { 40,     41    },
    { 41,     42    },
    { 42,     43    },
    { 43,     44    },
    { 44,     45    },
    { 45,     46    },
    { 50,     47    },
    { 51,     48    },
    { 52,     49    },
    { 53,     50    },
    { 54,     51    },
    { 55,     52    },
    { 56,     53    },
    { 59,     29    },
    { 60,     30    },
    { 62,     27    },
    { 63,     28    },
    { 68,     11    },
    { 69,     15    },
    { 70,     33    },
    { 71,     68    },
    { 84,     91    },
    { 85,     92    },
    { 86,     93    },
    { 87,     94    },
    { 97,     70    },
    { 98,     72    },
    { 99,     54    },
    { 141,    62    },
    { 142,    63    },
    { 143,    64    },
    { 144,    65    },
    { 145,    66    },
    { 146,    67    },
    { 148,    58    },
    { 168,    57    },
    { 169,    61    },
    { 175,    55    },
    { 176,    75    },
    { 177,    76    },
    { 180,    77    },
    { 181,    78    },
    { 178,    73    },
    { 179,    74    },
    { 188,    79    },
    { 189,    80    },
    { 248,    81    },
    { 249,    82    },
    { 193,    56    },
    { 196,    85    },
    { 197,    86    },
    { 198,    87    },
    { 199,    88    },
    { 200,    89    },
    { 201,    90    },
    { 211,    170   },
    { 218,    171   },
    { 219,    83    },
    { 220,    172   },
    { 221,    173   },
    { 222,    174   },
    { 223,    175   },
    { 224,    176   },
    { 225,    177   },
    { 227,    178   },
    { 229,    179   },
    { 232,    180   },
    { 235,    181   },
    { 236,    182   },
    { 237,    183   },
    { 238,    213   },
    { 239,    214   },
    { 250,    84    },
    { 0x13B,  113   },
    { 0x13C,  114   },
    { 0x13D,  115   },
    { 0x13E,  116   },
    { 0x13F,  117   },
    { 0x140,  118   },
    { 0x141,  119   },
    { 0x142,  120   },
    { 0x143,  121   },
    { 0x144,  122   },
    { 0x145,  123   },
    { 0x146,  124   },
    { 0x147,  125   },
    { 0x148,  126   },
    { 0x149,  127   },
    { 0x14A,  128   },
    { 0x14B,  129   },
    { 0x14C,  130   },
    { 0x14D,  131   },
    { 0x14E,  132   },
    { 0x14F,  133   },
    { 0x150,  134   },
    { 0x151,  135   },
    { 0x152,  136   },
    { 0x155,  137   },
    { 0x156,  138   },
    { 0x157,  139   },
    { 0x158,  140   },
    { 0x159,  141   },
    { 0x15A,  142   },
    { 0x15B,  143   },
    { 0x15C,  144   },
    { 0x15F,  145   },
    { 0x160,  146   },
    { 0x161,  147   },
    { 0x162,  148   },
    { 0x163,  149   },
    { 0x164,  150   },
    { 0x165,  151   },
    { 0x166,  152   },
    { 0x167,  153   },
    { 0x168,  154   },
    { 0x169,  155   },
    { 0x16A,  156   },
    { 0x16B,  157   },
    { 0x16C,  158   },
    { 0x16D,  159   },
    { 0x16E,  160   },
    { 0x16F,  161   },
    { 0x170,  162   },
    { 0x171,  163   },
    { 0x172,  164   },
    { 0x173,  165   },
    { 0x174,  166   },
    { 0x175,  167   },
    { 0x176,  168   },
    { 0x177,  169   },
    { 0x179,  184   },
    { 0x17A,  185   },
    { 0x17D,  186   },
    { 0x17E,  187   },
    { 0x17F,  188   },
    { 0x180,  189   },
    { 0x185,  190   },
    { 0x188,  191   },
    { 0x189,  192   },
    { 0x18A,  193   },
    { 0x18B,  194   },
    { 0x196,  195   },
    { 0x199,  200   },
    { 0x19A,  198   },
    { 0x19B,  197   },
    { 0x19C,  199   },
    { 0x19D,  196   },
    { 0x15D,  243   },
    { 234,    242   },
    { 0x15E,  229   },
    { 0x178,  230   },
    { 0x17B,  231   },
    { 210,    239   },
    { 0x18C,  241   },
    { 0x18D,  240   },
    { 0x18E,  237   },
    { 0x18F,  234   },
    { 0x190,  235   },
    { 0x191,  236   },
    { 0x192,  238   },
    { 0x193,  232   },
    { 0x194,  233   },
    { 0,      69    },
    { 0x102,  95    },
    { 0x103,  96    },
    { 0x104,  97    },
    { 0x105,  98    },
    { 0x197,  99    },
    { 0x198,  100   },
    { 0x106,  251   },
    { 0x107,  255   },
    { 0x108,  0x102 },
    { 0x109,  252   },
    { 0x10A,  254   },
    { 0x10B,  0x101 },
    { 0x10C,  0x100 },
    { 0x10D,  253   },
    { 0x10E,  0x106 },
    { 0x10F,  0x109 },
    { 0x110,  0x103 },
    { 0x111,  0x105 },
    { 0x112,  0x108 },
    { 0x113,  0x107 },
    { 0x114,  0x104 },
    { 0x115,  103   },
    { 0x116,  104   },
    { 0x117,  105   },
    { 0x118,  106   },
    { 0x119,  107   },
    { 0x11A,  108   },
    { 0x11B,  109   },
    { 0x11C,  110   },
    { 0x11D,  101   },
    { 0x11E,  102   },
    { 0x11F,  111   },
    { 0x19E,  209   },
    { 0x1A5,  59    },
    { 0x1A6,  60    },
    { 0x1A7,  207   },
    { 0x1A8,  208   },
    { 0x1A9,  210   },
    { 0x1AA,  211   },
    { 0x1AB,  212   },
    { 126,    215   },
    { 127,    216   },
    { 128,    217   },
    { 129,    218   },
    { 130,    219   },
    { 131,    220   },
    { 132,    221   },
    { 133,    222   },
    { 233,    223   },
    { 0x17C,  224   },
    { 0x181,  225   },
    { 0x182,  226   },
    { 0x183,  227   },
    { 0x186,  228   },
    { 0xFFFF, 0     },
};

static const TileRect sPokeathlonCourseMonPicRect = {
    .x = 0,
    .y = 0,
    .w = 10,
    .h = 10,
};

static const u32 sPokeathlonCourseTrainerPicFileIds[6] = { 99, 100, 189, 9, 6, 3 };

void PokeathlonCourse_LoadTrainerPic(ManagedSprite *sprite, int fileId, int index, enum HeapID heapId) {
    PokeathlonCourse_LoadTrainerPicIntoSprite(sprite->sprite, fileId, index, heapId);
}

// Converts texture index of the overworld model fileId (NARC data/mmodel/mmodel) into the sprite's characters
// and palette.

void PokeathlonCourse_LoadTrainerPicIntoSprite(Sprite *sprite, int fileId, int index, enum HeapID heapId) {
    NARC *narc;
    void *file;
    NNSG3dResTex *tex;
    void *buffer;
    u8 *texData;
    int offset;
    int location;
    int paletteOffset;
    NNS_G2D_VRAM_TYPE vramType;
    thunk_Sprite_SetDrawFlag(sprite, FALSE);
    narc = NARC_New(NARC_data_mmodel_mmodel, heapId);
    file = NARC_AllocAndReadWholeMember(narc, fileId, heapId);
    tex = NNS_G3dGetTex(file);
    offset = (*(u32 *)GetTexDictData(tex, index) & 0xFFFF) << 3;
    texData = (u8 *)tex + tex->texInfo.ofsTex;
    buffer = Heap_AllocAtEnd(heapId, 0x200);
    CharTiles_CopyRectFromBuffer(texData + offset, 4, 0, 0, 4, 4, buffer);
    PokeathlonCourse_LoadSpriteChars(sprite, buffer, 0x200);
    Heap_Free(buffer);
    vramType = Sprite_GetVramType(sprite);
    paletteOffset = tex->plttInfo.ofsPlttData;
    DC_FlushRange((u8 *)tex + paletteOffset, 0x20);
    location = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite), vramType);
    if (vramType == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJPltt((u8 *)tex + paletteOffset, location, 0x20);
    } else {
        GXS_LoadOBJPltt((u8 *)tex + paletteOffset, location, 0x20);
    }
    Heap_Free(file);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(sprite, TRUE);
}

void PokeathlonCourse_LoadTeamTrainerPic(ManagedSprite **sprites, PokeathlonCourseData *course, u8 team, int index) {
    PokeathlonCourse_LoadPlayerTrainerPic(sprites[team]->sprite, course, team, index);
}

void PokeathlonCourse_LoadPlayerTrainerPic(Sprite *sprite, PokeathlonCourseData *course, u32 team, u32 index) {
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(course, team);
    enum HeapID heapId = PokeathlonCourse_GetHeapID(course);
    u16 fileId;
    if (PlayerProfile_GetTrainerGender(profile) == 0) {
        fileId = PokeathlonCourse_GetTrainerPicFileId(0);
    } else {
        fileId = PokeathlonCourse_GetTrainerPicFileId(1);
    }
    PokeathlonCourse_LoadTrainerPicIntoSprite(sprite, fileId, (u8)index, heapId);
}

u16 PokeathlonCourse_GetTrainerPicFileId(u32 kind) {
    if (kind >= 6) {
        GF_AssertFail();
    }
    return sPokeathlonCourseTrainerPicFileIds[kind];
}

void PokeathlonCourse_LoadMonPic(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId) {
    PokeathlonCourse_LoadMonPicEx(sprite, info, mainScreen, palette, heapId, TRUE);
}

void PokeathlonCourse_LoadMonPicWithPalette(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId, BOOL loadPalette) {
    PokeathlonCourse_LoadMonPicEx(sprite, info, mainScreen, palette, heapId, loadPalette);
}

// Front picture of the Pokemon (no sprite if species is 0).

static void PokeathlonCourse_LoadMonPicEx(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 grayScale, enum HeapID heapId, BOOL loadPalette) {
    PokepicTemplate template;
    TileRect rect;
    void *buffer;
    if (info->species == SPECIES_NONE) {
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        return;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, info->species, info->gender, 2, info->isShiny, (u8)info->form, info->personality);
    rect = sPokeathlonCourseMonPicRect;
    buffer = Heap_AllocAtEnd(heapId, 0xC80);
    CharTiles_LoadMonRectAsObjFromNarc((NarcId) template.narcID, template.charDataID, heapId, &rect, buffer, info->personality, FALSE, 2, info->species);
    PokeathlonCourse_LoadSpriteChars(sprite->sprite, buffer, 0xC80);
    if (loadPalette) {
        PokeathlonCourse_LoadSpritePalette(sprite, (NarcId) template.narcID, template.palDataID, mainScreen, grayScale, heapId);
    }
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    Heap_Free(buffer);
}

void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, const PokeathlonCourseMonInfo *info, int palette, NARC *narc, u32 offset, enum HeapID heapId) {
    NNSG2dCharacterData *charData;
    void *file = GfGfxLoader_GetCharDataFromOpenNarc(narc, GetMonIconNaixEx(info->species, FALSE, info->form), FALSE, &charData, heapId);
    GF_ASSERT(file);
    PokeathlonCourse_LoadSpriteCharsAt(sprite->sprite, charData->pRawData, 0x200, offset);
    Heap_Free(file);
    ManagedSprite_SetPaletteOverride(sprite, palette + GetMonIconPaletteEx(info->species, info->form, FALSE));
}

static void PokeathlonCourse_LoadSpriteCharsAt(Sprite *sprite, void *data, u32 size, u32 offset) {
    NNS_G2D_VRAM_TYPE vramType = Sprite_GetVramType(sprite);
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), vramType);
    DC_FlushRange(data, size);
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, location + offset, size);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, location + offset, size);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

static void PokeathlonCourse_LoadSpriteChars(Sprite *sprite, void *data, u32 size) {
    PokeathlonCourse_LoadSpriteCharsAt(sprite, data, size, 0);
}

void PokeathlonCourse_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, int fileId, BOOL mainScreen, BOOL grayScale, enum HeapID heapId) {
    int vramType;
    int location;
    u32 offset;

    if (mainScreen) {
        vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        location = GF_PAL_LOCATION_MAIN_OBJ;
    } else {
        vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        location = GF_PAL_LOCATION_SUB_OBJ;
    }
    offset = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite->sprite), (NNS_G2D_VRAM_TYPE)vramType);
    GfGfxLoader_GXLoadPal(narcId, fileId, (enum GFPalLoadLocation)location, (enum GFPalSlotOffset)offset, 0x20, heapId);
    if (grayScale) {
        void *palette;
        if (mainScreen) {
            palette = (void *)HW_OBJ_PLTT;
        } else {
            palette = (void *)HW_DB_OBJ_PLTT;
        }
        TintPalette_GrayScale((u16 *)(offset + (u32)palette), 0x20);
    }
}

int PokeathlonCourse_GetNpcTrainerPicFileId(int trainerClass) {
    int i;

    for (i = 0; i < 9999; i++) {
        if (i == 0xFFFF) {
            return 0;
        }
        if (trainerClass == sPokeathlonNpcTrainerPics[i].trainerClass) {
            return sPokeathlonNpcTrainerPics[i].fileId;
        }
    }
    GF_AssertFail();
    return 0;
}
