#include "metatile_behavior.h"

#include "global.h"

#include "constants/metatile_behavior.h"

static const u8 sMetatileBehaviorFlags[] = {
    [TILE_BEHAVIOR_0]                   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_1]                   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_ENCOUNTER_GRASS]     = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_3]                   = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_4]                   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_5]                   = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_HEADBUTT]            = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_7]                   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_8]                   = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_9]                   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_10]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_11]                  = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_12]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_13]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_14]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_15]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_16]                  = TILE_BEHAVIOR_FLAG_SURFABLE_ENCOUNTER,
    [TILE_BEHAVIOR_WHIRLPOOL]           = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_18]                  = TILE_BEHAVIOR_FLAG_SURFABLE_ENCOUNTER,
    [TILE_BEHAVIOR_WATERFALL]           = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_20]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_21]                  = TILE_BEHAVIOR_FLAG_SURFABLE_ENCOUNTER,
    [TILE_BEHAVIOR_22]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_23]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_24]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_25]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_26]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_27]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_28]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_29]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_30]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_31]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_32]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_33]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_34]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_35]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_36]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_37]                  = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_38]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_39]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_40]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_41]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_42]                  = TILE_BEHAVIOR_FLAG_SURFABLE_ENCOUNTER,
    [TILE_BEHAVIOR_43]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_44]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_45]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_46]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_47]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_48]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_49]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_50]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_51]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_52]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_53]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_54]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_55]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_56]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_57]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_58]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_59]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_LADDER_NORTH]        = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_LADDER_SOUTH]        = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_62]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_63]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_64]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_65]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_66]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_67]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_68]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_69]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_70]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_71]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_72]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_73]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_74]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_75]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_76]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_77]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_78]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_79]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_80]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_81]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_82]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_83]                  = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_84]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_85]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_86]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_87]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_88]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_89]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_90]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_91]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_92]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_93]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_STAIRS_EAST]    = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_STAIRS_WEST]    = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_96]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_97]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_ENTRANCE_EAST]  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_ENTRANCE_WEST]  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_ENTRANCE_NORTH] = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_ENTRANCE_SOUTH] = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_102]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_PANEL]          = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_104]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_DOOR]                = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_ESCALATOR_FLIP_FACE] = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_ESCALATOR]           = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_EAST]           = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_WEST]           = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_NORTH]          = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_WARP_SOUTH]          = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_112]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_113]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_114]                 = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_115]                 = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_116]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_117]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_118]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_119]                 = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_120]                 = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_121]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_122]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_123]                 = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_124]                 = TILE_BEHAVIOR_FLAG_SURFABLE,
    [TILE_BEHAVIOR_125]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_126]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_127]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_128]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_129]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_130]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_POKECENTER_PC]       = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_132]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_TOWN_MAP]            = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_TV]                  = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_135]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_136]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_137]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_138]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_139]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_140]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_141]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_142]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_143]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_144]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_145]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_146]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_147]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_148]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_149]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_150]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_151]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_152]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_153]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_154]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_155]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_156]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_157]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_158]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_159]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_160]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_161]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_162]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_163]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_164]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_165]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_166]                 = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_167]                 = TILE_BEHAVIOR_FLAG_ENCOUNTER,
    [TILE_BEHAVIOR_168]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_169]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_170]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_171]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_172]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_173]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_174]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_175]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_176]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_177]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_178]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_179]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_180]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_181]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_182]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_183]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_184]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_185]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_186]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_187]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_188]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_189]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_190]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_191]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_192]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_193]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_194]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_195]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_196]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_197]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_198]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_199]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_200]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_201]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_202]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_203]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_204]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_205]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_206]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_207]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_208]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_209]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_210]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_211]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_212]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_213]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_214]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_215]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_216]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_217]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_218]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_219]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_220]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_221]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_222]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_223]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_PICTURE_BOOKS]       = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_CHOCK_FULL]          = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_MAGAZINES]           = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_227]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_EMPTY_TRASH]         = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_POKEMON_GOODS]       = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_230]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_231]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_232]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_233]                 = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_BOOKS_FOR_POKEMON]   = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_CONVENIENT_ITEMS]    = TILE_BEHAVIOR_FLAG_NONE,
    [TILE_BEHAVIOR_POKEMON_MERCHANDISE] = TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE,
    TILE_BEHAVIOR_FLAG_NONE
};

BOOL MetatileBehavior_IsEncounterGrass(u8 tile) {
    return tile == TILE_BEHAVIOR_ENCOUNTER_GRASS;
}

BOOL sub_0205B6F4(u8 tile) {
    return tile == TILE_BEHAVIOR_3;
}

BOOL sub_0205B700(u8 tile) {
    return tile == TILE_BEHAVIOR_128;
}

BOOL MetatileBehavior_IsDoor(u8 tile) {
    return tile == TILE_BEHAVIOR_DOOR;
}

BOOL MetatileBehavior_IsWarpEntranceEast(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_ENTRANCE_EAST;
}

BOOL MetatileBehavior_IsWarpEntranceWest(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_ENTRANCE_WEST;
}

BOOL MetatileBehavior_IsWarpEntranceNorth(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_ENTRANCE_NORTH;
}

BOOL MetatileBehavior_IsWarpEntranceSouth(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_ENTRANCE_SOUTH;
}

BOOL MetatileBehavior_IsWarpEast(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_EAST;
}

BOOL MetatileBehavior_IsWarpWest(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_WEST;
}

BOOL MetatileBehavior_IsWarpNorth(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_NORTH;
}

BOOL MetatileBehavior_IsWarpSouth(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_SOUTH;
}

BOOL MetatileBehavior_IsSurfableWater(u8 tile) {
    return (sMetatileBehaviorFlags[tile] & 1) != 0;
}

BOOL sub_0205B78C(u8 tile) {
    return tile == TILE_BEHAVIOR_34;
}

BOOL sub_0205B798(u8 tile) {
    return tile == TILE_BEHAVIOR_33;
}

BOOL sub_0205B7A4(u8 tile) {
    return tile == TILE_BEHAVIOR_23;
}

BOOL sub_0205B7B0(u8 tile) {
    return tile == TILE_BEHAVIOR_58;
}

BOOL sub_0205B7BC(u8 tile) {
    return tile == TILE_BEHAVIOR_59;
}

BOOL sub_0205B7C8(u8 tile) {
    return tile == TILE_BEHAVIOR_57;
}

BOOL sub_0205B7D4(u8 tile) {
    return tile == TILE_BEHAVIOR_56;
}

BOOL MetatileBehavior_IsPokecenterPC(u8 tile) {
    return tile == TILE_BEHAVIOR_POKECENTER_PC;
}

BOOL MetatileBehavior_IsTownMap(u8 tile) {
    return tile == TILE_BEHAVIOR_TOWN_MAP;
}

BOOL MetatileBehavior_IsEscalatorFlipFace(u8 tile) {
    return tile == TILE_BEHAVIOR_ESCALATOR_FLIP_FACE;
}

BOOL MetatileBehavior_IsEscalator(u8 tile) {
    return tile == TILE_BEHAVIOR_ESCALATOR;
}

BOOL MetatileBehavior_IsWarpStairsEast(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_STAIRS_EAST;
}

BOOL MetatileBehavior_IsWarpStairsWest(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_STAIRS_WEST;
}

BOOL sub_0205B828(u8 tile) {
    return tile == TILE_BEHAVIOR_32;
}

BOOL sub_0205B834(u8 tile) {
    return tile == TILE_BEHAVIOR_75;
}

BOOL sub_0205B840(u8 tile) {
    return tile == TILE_BEHAVIOR_76;
}

BOOL MetatileBehavior_IsPictureBooks(u8 tile) {
    return tile == TILE_BEHAVIOR_PICTURE_BOOKS;
}

BOOL MetatileBehavior_IsBooksForPokemon(u8 tile) {
    return tile == TILE_BEHAVIOR_BOOKS_FOR_POKEMON;
}

BOOL MetatileBehavior_IsChockFull(u8 tile) {
    return tile == TILE_BEHAVIOR_CHOCK_FULL;
}

BOOL MetatileBehavior_IsMagazines(u8 tile) {
    return tile == TILE_BEHAVIOR_MAGAZINES;
}

BOOL MetatileBehavior_IsEmptyTrash(u8 tile) {
    return tile == TILE_BEHAVIOR_EMPTY_TRASH;
}

BOOL MetatileBehavior_IsPokemonGoods(u8 tile) {
    return tile == TILE_BEHAVIOR_POKEMON_GOODS;
}

BOOL MetatileBehavior_IsConvenientItems(u8 tile) {
    return tile == TILE_BEHAVIOR_CONVENIENT_ITEMS;
}

BOOL MetatileBehavior_IsPokemonMerchandise(u8 tile) {
    return tile == TILE_BEHAVIOR_POKEMON_MERCHANDISE;
}

BOOL sub_0205B8AC(u8 tile) {
    return tile == TILE_BEHAVIOR_164;
}

BOOL sub_0205B8B8(u8 tile) {
    return tile == TILE_BEHAVIOR_168;
}

BOOL sub_0205B8C4(u8 tile) {
    return tile == TILE_BEHAVIOR_168;
}

BOOL sub_0205B8D0(u8 tile) {
    return tile == TILE_BEHAVIOR_8;
}

BOOL MetatileBehavior_IsWaterfall(u8 tile) {
    return tile == TILE_BEHAVIOR_WATERFALL;
}

BOOL MetatileBehavior_IsWhirlpool(u8 tile) {
    return tile == TILE_BEHAVIOR_WHIRLPOOL;
}

BOOL sub_0205B8F4(u8 tile) {
    return tile == TILE_BEHAVIOR_50 || tile == TILE_BEHAVIOR_52 || tile == TILE_BEHAVIOR_53 || tile == TILE_BEHAVIOR_LADDER_NORTH || tile == TILE_BEHAVIOR_73;
}

BOOL sub_0205B918(u8 tile) {
    return tile == TILE_BEHAVIOR_51 || tile == TILE_BEHAVIOR_54 || tile == TILE_BEHAVIOR_55 || tile == TILE_BEHAVIOR_LADDER_SOUTH || tile == TILE_BEHAVIOR_73;
}

BOOL sub_0205B93C(u8 tile) {
    return tile == TILE_BEHAVIOR_49 || tile == TILE_BEHAVIOR_53 || tile == TILE_BEHAVIOR_55 || tile == TILE_BEHAVIOR_74;
}

BOOL sub_0205B960(u8 tile) {
    return tile == TILE_BEHAVIOR_48 || tile == TILE_BEHAVIOR_52 || tile == TILE_BEHAVIOR_54 || tile == TILE_BEHAVIOR_74;
}

BOOL sub_0205B984(u8 tile) {
    return tile == TILE_BEHAVIOR_22 || tile == TILE_BEHAVIOR_29;
}

BOOL MetatileBehavior_CanGenerateWalkingEncounters(u8 tile) {
    return (sMetatileBehaviorFlags[tile] & 2) != 0;
}

BOOL MetatileBehavior_IsTV(u8 tile) {
    return tile == TILE_BEHAVIOR_TV;
}

BOOL sub_0205B9B8(u8 tile) {
    return tile == TILE_BEHAVIOR_16 || tile == TILE_BEHAVIOR_22 || tile == TILE_BEHAVIOR_29 || tile == TILE_BEHAVIOR_32 || tile == TILE_BEHAVIOR_45;
}

BOOL sub_0205B9DC(u8 tile) {
    return tile == TILE_BEHAVIOR_64;
}

BOOL sub_0205B9E8(u8 tile) {
    return tile == TILE_BEHAVIOR_65;
}

BOOL sub_0205B9F4(u8 tile) {
    return tile == TILE_BEHAVIOR_66;
}

BOOL sub_0205BA00(u8 tile) {
    return tile == TILE_BEHAVIOR_67;
}

BOOL sub_0205BA0C(u8 tile) {
    return tile == TILE_BEHAVIOR_77;
}

BOOL MetatileBehavior_IsWarpPanel(u8 tile) {
    return tile == TILE_BEHAVIOR_WARP_PANEL;
}

BOOL sub_0205BA24(u8 tile) {
    return tile == TILE_BEHAVIOR_112;
}

BOOL sub_0205BA30(u8 tile) {
    return tile == TILE_BEHAVIOR_113 || tile == TILE_BEHAVIOR_114 || tile == TILE_BEHAVIOR_115;
}

BOOL sub_0205BA44(u8 tile) {
    return tile == TILE_BEHAVIOR_113 || tile == TILE_BEHAVIOR_114;
}

BOOL sub_0205BA54(u8 tile) {
    return tile == TILE_BEHAVIOR_115;
}

BOOL MetatileBehavior_IsNone(u8 tile) {
    return tile == TILE_BEHAVIOR_NONE;
}

u8 sub_0205BA6C(void) {
    return TILE_BEHAVIOR_NONE;
}

BOOL sub_0205BA70(u8 tile) {
    return tile == TILE_BEHAVIOR_29 || tile == TILE_BEHAVIOR_32 || tile == TILE_BEHAVIOR_45;
}

BOOL sub_0205BA94(u8 tile) {
    return tile == TILE_BEHAVIOR_169;
}

BOOL MetatileBehavior_IsLadderNorth(u8 tile) {
    return tile == TILE_BEHAVIOR_LADDER_NORTH;
}

BOOL MetatileBehavior_IsLadderSouth(u8 tile) {
    return tile == TILE_BEHAVIOR_LADDER_SOUTH;
}

BOOL sub_0205BAB8(u8 tile) {
    return tile == TILE_BEHAVIOR_62;
}

BOOL sub_0205BAC4(u8 tile) {
    return tile == TILE_BEHAVIOR_44;
}

BOOL sub_0205BAD0(u8 tile) {
    return tile == TILE_BEHAVIOR_0 || tile == TILE_BEHAVIOR_33 || tile == TILE_BEHAVIOR_164;
}

BOOL sub_0205BAE4(u8 tile) {
    return MetatileBehavior_IsSurfableWater(tile);
}

BOOL sub_0205BAEC(u8 tile) {
    return tile == TILE_BEHAVIOR_35;
}

BOOL MetatileBehavior_IsHeadbutt(u8 tile) {
    return tile == TILE_BEHAVIOR_HEADBUTT;
}

BOOL sub_0205BB04(u8 tile) {
    return sub_0205BA44(tile) || tile == TILE_BEHAVIOR_46;
}
