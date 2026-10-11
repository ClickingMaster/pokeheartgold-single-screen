#ifndef GUARD_POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_INTERNAL_H
#define GUARD_POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_INTERNAL_H

#include "global.h"

#include "application/pokedex/pokedex_internal_constants.h"

#include "bg_cell_manager.h" // 0x0201956C
#include "bg_window.h"
#include "msgdata.h"
#include "overlay_18.h"
#include "palette.h"
#include "sprite_system.h"
#include "unk_02019BA4.h" // 0x02019BA4

enum {
    POKEDEXAPP_MAINSEQ_00 = 0,
    POKEDEXAPP_MAINSEQ_01 = 1,
    POKEDEXAPP_MAINSEQ_02 = 2,
    POKEDEXAPP_MAINSEQ_03 = 3,
    POKEDEXAPP_MAINSEQ_04 = 4,
    POKEDEXAPP_MAINSEQ_05 = 5,
    POKEDEXAPP_MAINSEQ_06 = 6,
    POKEDEXAPP_MAINSEQ_07 = 7,
    POKEDEXAPP_MAINSEQ_08 = 8,
    POKEDEXAPP_MAINSEQ_09 = 9,
    POKEDEXAPP_MAINSEQ_10 = 10,
    POKEDEXAPP_MAINSEQ_11 = 11,
    POKEDEXAPP_MAINSEQ_12 = 12,
    POKEDEXAPP_MAINSEQ_13 = 13,
    POKEDEXAPP_MAINSEQ_14 = 14,
    POKEDEXAPP_MAINSEQ_15 = 15,
    POKEDEXAPP_MAINSEQ_16 = 16,
    POKEDEXAPP_MAINSEQ_17 = 17,
    POKEDEXAPP_MAINSEQ_18 = 18,
    POKEDEXAPP_MAINSEQ_19 = 19,
    POKEDEXAPP_MAINSEQ_20 = 20,
    POKEDEXAPP_MAINSEQ_21 = 21,
    POKEDEXAPP_MAINSEQ_22 = 22,
    POKEDEXAPP_MAINSEQ_23 = 23,
    POKEDEXAPP_MAINSEQ_24 = 24,
    POKEDEXAPP_MAINSEQ_25 = 25,
    POKEDEXAPP_MAINSEQ_26 = 26,
    POKEDEXAPP_MAINSEQ_27 = 27,
    POKEDEXAPP_MAINSEQ_28 = 28,
    POKEDEXAPP_MAINSEQ_29 = 29,
    POKEDEXAPP_MAINSEQ_30 = 30,
    POKEDEXAPP_MAINSEQ_31 = 31,
    POKEDEXAPP_MAINSEQ_32 = 32,
    POKEDEXAPP_MAINSEQ_33 = 33,
    POKEDEXAPP_MAINSEQ_34 = 34,
    POKEDEXAPP_MAINSEQ_35 = 35,
    POKEDEXAPP_MAINSEQ_36 = 36,
    POKEDEXAPP_MAINSEQ_37 = 37,
    POKEDEXAPP_MAINSEQ_38 = 38,
    POKEDEXAPP_MAINSEQ_39 = 39,
    POKEDEXAPP_MAINSEQ_40 = 40,
    POKEDEXAPP_MAINSEQ_41 = 41,
    POKEDEXAPP_MAINSEQ_42 = 42,
    POKEDEXAPP_MAINSEQ_43 = 43,
    POKEDEXAPP_MAINSEQ_44 = 44,
    POKEDEXAPP_MAINSEQ_45 = 45,
    POKEDEXAPP_MAINSEQ_46 = 46,
    POKEDEXAPP_MAINSEQ_47 = 47,
    POKEDEXAPP_MAINSEQ_48 = 48,
    POKEDEXAPP_MAINSEQ_49 = 49,
    POKEDEXAPP_MAINSEQ_50 = 50,
    POKEDEXAPP_MAINSEQ_51 = 51,
    POKEDEXAPP_MAINSEQ_52 = 52,
    POKEDEXAPP_MAINSEQ_53 = 53,
    POKEDEXAPP_MAINSEQ_54 = 54,
    POKEDEXAPP_MAINSEQ_55 = 55,
    POKEDEXAPP_MAINSEQ_56 = 56,
    POKEDEXAPP_MAINSEQ_57 = 57,
    POKEDEXAPP_MAINSEQ_58 = 58,
    POKEDEXAPP_MAINSEQ_59 = 59,
    POKEDEXAPP_MAINSEQ_60 = 60,
    POKEDEXAPP_MAINSEQ_61 = 61,
    POKEDEXAPP_MAINSEQ_62 = 62,
    POKEDEXAPP_MAINSEQ_63 = 63,
    POKEDEXAPP_MAINSEQ_64 = 64,
    POKEDEXAPP_MAINSEQ_65 = 65,
    POKEDEXAPP_MAINSEQ_66 = 66,
    POKEDEXAPP_MAINSEQ_67 = 67,
    POKEDEXAPP_MAINSEQ_68 = 68,
    POKEDEXAPP_MAINSEQ_69 = 69,
    POKEDEXAPP_MAINSEQ_70 = 70,
    POKEDEXAPP_MAINSEQ_71 = 71,
    POKEDEXAPP_MAINSEQ_72 = 72,
    POKEDEXAPP_MAINSEQ_73 = 73,
    POKEDEXAPP_MAINSEQ_74 = 74,
    POKEDEXAPP_MAINSEQ_75 = 75,
    POKEDEXAPP_MAINSEQ_76 = 76,
    POKEDEXAPP_MAINSEQ_77 = 77,
    POKEDEXAPP_MAINSEQ_78 = 78,
    POKEDEXAPP_MAINSEQ_79 = 79,
    POKEDEXAPP_MAINSEQ_80 = 80,
    POKEDEXAPP_MAINSEQ_81 = 81,
    POKEDEXAPP_MAINSEQ_82 = 82,
    POKEDEXAPP_MAINSEQ_83 = 83,
    POKEDEXAPP_MAINSEQ_84 = 84,
    POKEDEXAPP_MAINSEQ_85 = 85,
    POKEDEXAPP_MAINSEQ_86 = 86,
    POKEDEXAPP_MAINSEQ_87 = 87,
    POKEDEXAPP_MAINSEQ_88 = 88,
    POKEDEXAPP_MAINSEQ_89 = 89,
    POKEDEXAPP_MAINSEQ_90 = 90,
    POKEDEXAPP_MAINSEQ_91 = 91,
    POKEDEXAPP_MAINSEQ_92 = 92,
    POKEDEXAPP_MAINSEQ_MAX = 93,
};

typedef struct PokedexAppData PokedexAppData;

typedef struct PokedexAppData_UnkSub0868_State04 {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7_0 : 4;
    u8 unk_7_4 : 4;
} PokedexAppData_UnkSub0868_State04;

typedef struct PokedexAppData_UnkSub0868_State06 {
    u32 unk_0;
    u32 unk_4;
    s16 unk_8;
    u16 unk_A;
} PokedexAppData_UnkSub0868_State06;

typedef struct PokedexAppData_UnkSub0868_State09 {
    u16 unk_0;
    u8 unk_2;
    u8 unk_3;
} PokedexAppData_UnkSub0868_State09;

typedef struct PokedexAppData_UnkSub0868_State10 {
    u8 unk_0;
} PokedexAppData_UnkSub0868_State10;

typedef struct PokedexAppData_UnkSub0868_State13 {
    u16 *unk_0;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7;
} PokedexAppData_UnkSub0868_State13;

typedef struct PokedexAppData_UnkSub0868_State16 {
    u16 unk_0;
    u16 unk_2;
} PokedexAppData_UnkSub0868_State16;

typedef struct PokedexAppData_UnkSub0868_State17 {
    u32 unk_0;
    u32 unk_4;
    u8 filler_8[2];
    u16 unk_A;
} PokedexAppData_UnkSub0868_State17;

typedef struct PokedexAppData_UnkSub0868_State26 {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
} PokedexAppData_UnkSub0868_State26;

typedef struct PokedexAppData_UnkSub0868_State38 {
    u32 unk_0;
    u32 unk_4;
    u16 unk_8;
    s16 unk_A;
} PokedexAppData_UnkSub0868_State38;

typedef struct PokedexAppData_UnkSub0868_State69 {
    u16 unk_0;
    u8 unk_2;
} PokedexAppData_UnkSub0868_State69;

// Weight check: the scale animation (sWeightCheckAnimFuncs[animIndex], one per weight-difference class)
typedef struct PokedexAppData_UnkSub0868_State77_Sub4 {
    PokedexAppData *pokedexApp; // 0x00
    s32 dropOffset;             // 0x04 figures dropping onto the pans, fx32
    u16 animIndex;              // 0x08
    u8 monLighter;              // 0x0A the Pokemon is lighter than the player
    u8 step;                    // 0x0B
    s32 rotationSpeed;          // 0x0C beam rotation per frame
    s32 rotationAccel;          // 0x10
    s32 rotationEndSpeed;       // 0x14
    s16 dropSpeed;              // 0x18 the thrown figure's vertical speed
    s16 dropAccel;              // 0x1A
    u16 dropSprite;             // 0x1C the thrown figure (2 trainer, 4 Pokemon)
    u16 dropTargetSprite;       // 0x1E its pan (8, 9)
} PokedexAppData_UnkSub0868_State77_Sub4;

typedef struct PokedexAppData_UnkSub0868_State77 {
    int unk_0;
    PokedexAppData_UnkSub0868_State77_Sub4 *unk_4;
} PokedexAppData_UnkSub0868_State77;

typedef union PokedexAppData_UnkSub0868 {
    PokedexAppData_UnkSub0868_State04 state_04;
    PokedexAppData_UnkSub0868_State06 state_06;
    PokedexAppData_UnkSub0868_State09 state_09;
    PokedexAppData_UnkSub0868_State10 state_10;
    PokedexAppData_UnkSub0868_State13 state_13;
    PokedexAppData_UnkSub0868_State16 state_16;
    PokedexAppData_UnkSub0868_State17 state_17;
    PokedexAppData_UnkSub0868_State26 state_26;
    PokedexAppData_UnkSub0868_State38 state_38;
    PokedexAppData_UnkSub0868_State69 state_69;
    PokedexAppData_UnkSub0868_State77 state_77;
    u8 padding[16];
} PokedexAppData_UnkSub0868;

typedef struct PokedexAppData_UnkSub0878 {
    u16 unk_000[NATIONAL_DEX_COUNT][2];
    u16 unk_7B4;
    u16 unk_7B6;
} PokedexAppData_UnkSub0878;

typedef struct PokedexAppData_UnkSub1030 {
    u16 unk_0; // 0x0
    u16 unk_2; // 0x2
} PokedexAppData_UnkSub1030;

typedef struct PokedexAppData_UnkSub18CC {
    s16 *player_ypos;  // 0x0
    s16 *player_scale; // 0x4
    s16 *mon_ypos;     // 0x8
    s16 *mon_scale;    // 0xC
} PokedexAppData_UnkSub18CC;

typedef struct PokedexAppData_UnkSub18DC_0 {
    s32 *maps;
    s32 nMaps;
} PokedexAppData_UnkSub18DC_0;

typedef struct PokedexAppData_UnkSub18DC {
    PokedexAppData_UnkSub18DC_0 unk_00; // 0x00
    PokedexAppData_UnkSub18DC_0 unk_08; // 0x08
    PokedexAppData_UnkSub18DC_0 unk_10; // 0x10
    PokedexAppData_UnkSub18DC_0 unk_18; // 0x18
    PokedexAppData_UnkSub18DC_0 unk_20; // 0x20
    u32 *unk_28;                        // 0x28
} PokedexAppData_UnkSub18DC;

typedef struct PokedexAppData_UnkSub1908 {
    u8 unk_00; // 0x00
    u8 unk_01; // 0x01
    u8 unk_02; // 0x02
    u8 unk_03; // 0x03
} PokedexAppData_UnkSub1908;

typedef struct PokedexAppData_UnkSub190C {
    u8 unk_00;     // 0x00
    u8 unk_01;     // 0x01
    u8 unk_02;     // 0x02
    u8 unk_03;     // 0x03
    u8 unk_04[32]; // 0x04
} PokedexAppData_UnkSub190C;

typedef struct UnkStruct_ov18_021F9780 {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} UnkStruct_ov18_021F9780;

typedef struct PokedexAppData_UnkSub1850 {
    u16 unk_0;
    u16 unk_2;
} PokedexAppData_UnkSub1850;

struct PokedexAppData {
    PokedexArgs *args;                              // 0x0000
    BgConfig *bgConfig;                             // 0x0004
    BgCellManager *unk_0008;                        // 0x0008
    Window windows[101];                            // 0x000C
    MsgData *msgData;                               // 0x065C
    MessageFormat *msgFormat;                       // 0x0660
    String *strBufFormatted;                        // 0x0664
    SpriteSystem *spriteSystem;                     // 0x0668
    SpriteManager *spriteManager;                   // 0x066C
    ManagedSprite *sprites[120];                    // 0x0670
    PaletteData *paletteData;                       // 0x0850
    NARC *gfxNarc;                                  // 0x0854
    NARC *iconNarc;                                 // 0x0858
    int unk_085C;                                   // 0x085C
    int unk_0860;                                   // 0x0860
    u32 unk_0864;                                   // 0x0864
    PokedexAppData_UnkSub0868 unk_0868;             // 0x0868
    PokedexAppData_UnkSub0878 unk_0878;             // 0x0878
    PokedexAppData_UnkSub1030 unk_1030[518];        // 0x1030
    void *heights;                                  // 0x1848
    void *weights;                                  // 0x184C
    PokedexAppData_UnkSub1850 *unk_1850;            // 0x1850
    u8 *unk_1854;                                   // 0x1854
    u8 unk_1858;                                    // 0x1858
    u8 unk_1859;                                    // 0x1859
    u8 unk_185A;                                    // 0x185A
    u8 unk_185B;                                    // 0x185B
    u8 unk_185C;                                    // 0x185C
    u8 unk_185D;                                    // 0x185D
    u8 unk_185E;                                    // 0x185E
    int unk_1860;                                   // 0x1860
    GridInputHandler *unk_1864;                     // 0x1864
    u32 dexSearchCriteria[DEX_SEARCH_CRITERIA_NUM]; // 0x1868
    u32 dexSearchCriteriaBak[2];                    // 0x1890
    int unk_1898;                                   // 0x1898
    int prevGridInput;                              // 0x189C search grids: input before the OK / cancel buttons
    u8 unk_18A0;                                    // 0x18A0
    u8 unk_18A1;                                    // 0x18A1
    u16 curSpecies;                                 // 0x18A2
    u8 seenForms[0x20];                             // 0x18A4
    s8 numSeenForms;                                // 0x18C4
    s8 unk_18C5;                                    // 0x18C5
    s8 unk_18C6;                                    // 0x18C6
    u8 unk_18C7_0 : 5;                              // 0x18C7
    u8 formPicSwapA : 1;                            // 0x18C7 which sprite pair shows the (left) form picture
    u8 formPicSwapB : 1;                            // 0x18C7 same for the compare screen's right picture
    u8 unk_18C7_7 : 1;                              // 0x18C7
    s8 unk_18C8;                                    // 0x18C8
    s8 unk_18C9;                                    // 0x18C9
    s8 unk_18CA;                                    // 0x18CA
    u8 unk_18CB_0 : 6;                              // 0x18CB
    u8 unk_18CB_6 : 1;                              // 0x18CB
    u8 unk_18CB_7 : 1;                              // 0x18CB
    PokedexAppData_UnkSub18CC unk_18CC;             // 0x18CC
    PokedexAppData_UnkSub18DC unk_18DC;             // 0x18DC
    PokedexAppData_UnkSub1908 *unk_1908;            // 0x1908
    PokedexAppData_UnkSub190C *unk_190C;            // 0x190C
}; // size: 0x1910

void ov18_021E5C40(void *cb_arg);
void ov18_021E5C74(void);
void ov18_021E5C84(PokedexAppData *pokedexApp);
void ov18_021E5E70(PokedexAppData *pokedexApp, BOOL a1);
void ov18_021E5F58(PokedexAppData *pokedexApp);
void ov18_021E613C(PokedexAppData *pokedexApp, u8 bgId);
void ov18_021E6174(void);
void ov18_021E618C(u8 ev1, u8 ev2);
void ov18_021E61A4(PokedexAppData *appData);
void ov18_021E6204(PokedexAppData *pokedexApp);
void ov18_021E6244(PokedexAppData *pokedexApp);
void ov18_021E6280(PokedexAppData *pokedexApp);
void ov18_021E62A8(PokedexAppData *pokedexApp);
void ov18_021E62E8(PokedexAppData *pokedexApp);
void ov18_021E6308(PokedexAppData *pokedexApp);
void ov18_021E6540(PokedexAppData *pokedexApp);
void ov18_021E654C(PokedexAppData *pokedexApp);
void ov18_021E6574(PokedexAppData *pokedexApp);
void ov18_021E659C(PokedexAppData *pokedexApp);
void ov18_021E65AC(PokedexAppData *pokedexApp);
void ov18_021E65D4(PokedexAppData *pokedexApp);
void ov18_021E65FC(PokedexAppData *pokedexApp);
void ov18_021E6624(PokedexAppData *pokedexApp);
void ov18_021E664C(PokedexAppData *pokedexApp);
void ov18_021E6674(PokedexAppData *pokedexApp);
void ov18_021E669C(PokedexAppData *pokedexApp);
void ov18_021E66C4(PokedexAppData *pokedexApp);
void ov18_021E66EC(PokedexAppData *pokedexApp);
void ov18_021E6714(PokedexAppData *pokedexApp);
void ov18_021E673C(PokedexAppData *pokedexApp);
void ov18_021E6794(PokedexAppData *pokedexApp);
void ov18_021E67B0(PokedexAppData *pokedexApp);
void ov18_021E67C8(PokedexAppData *pokedexApp, int a1);
void ov18_021E6A70(PokedexAppData *pokedexApp);
u16 *ov18_021E6AEC(PokedexAppData *pokedexApp, u32 a1);
u16 *ov18_021E6BB8(PokedexAppData *pokedexApp, u32 a1);
void ov18_021E6C90(u16 *a0);
void ov18_021E6C98(PokedexAppData *pokedexApp);
void ov18_021E6CC0(PokedexAppData *pokedexApp);
void ov18_021E6CE8(PokedexAppData *pokedexApp, u16 *a1);
BOOL ov18_021E6D10(PokedexAppData *pokedexApp, u16 species, u16 language);
BOOL ov18_021E6D38(PokedexAppData *pokedexApp, u16 species);
void ov18_021E6D68(PokedexAppData *pokedexApp, u16 species, s16 a2);
void ov18_021E6E44(PokedexAppData *pokedexApp);
void ov18_021E6F6C(BgConfig *bgConfig, u8 bgId, u8 language, NARC *narc, enum HeapID heapId);
void ov18_021E6FB8(BgConfig *bgConfig, u8 bgId, NARC *narc, enum HeapID heapId);
void ov18_021E6FFC(PokedexAppData *pokedexApp);
void ov18_021E71D0(PokedexAppData *pokedexApp, int a1, int a2);
void ov18_021E74B8(PokedexAppData *pokedexApp, int a1);
void ov18_021E74E4(PokedexAppData *pokedexApp);
void ov18_021E7534(PokedexAppData *pokedexApp, int a1);
void ov18_021E766C(PokedexAppData *pokedexApp, u32 a1);
u8 ov18_021E7698(int a0);
void ov18_021E76EC(PokedexAppData *pokedexApp, u32 a1);
void ov18_021E7724(PokedexAppData *pokedexApp);
void ov18_021E78AC(PokedexAppData *pokedexApp);
void ov18_021E7A80(PokedexAppData *pokedexApp, u16 species, s16 a2);
void ov18_021E7B6C(BOOL a0);
void ov18_021E800C(PokedexAppData *pokedexApp);
void ov18_021E81A8(PokedexAppData *pokedexApp);
void ov18_021E8254(PokedexAppData *pokedexApp);
void ov18_021E8410(PokedexAppData *pokedexApp);
void ov18_021E84EC(PokedexAppData *pokedexApp);
void ov18_021E8528(PokedexAppData *pokedexApp, int a1, int a2);
void ov18_021E8648(PokedexAppData *pokedexApp);
BOOL ov18_021E8AB0(PokedexAppData *pokedexApp, u32 a1);
u32 ov18_021E8ACC(PokedexAppData *pokedexApp, u32 a1, u32 a2);
u32 ov18_021E8AE0(PokedexAppData *pokedexApp, u32 a1);
u32 ov18_021E8B0C(u32 a0);
u32 ov18_021E8B18(u32 a0);
BOOL ov18_021E8B24(u32 a0);
BOOL ov18_021E8B5C(u32 a0);
void ov18_021E8B94(PokedexAppData *pokedexApp);
void ov18_021E8BD4(PokedexAppData *pokedexApp);

BOOL PokedexApp_RunMainSeq(PokedexAppData *pokedexApp, int *pState);
void PokedexApp_AddWindows(PokedexAppData *pokedexApp, const WindowTemplate *templates, u32 count);

void PokedexApp_RemoveAllWindows(PokedexAppData *pokedexApp);
void PokedexApp_PrintFormattedMessage(PokedexAppData *pokedexApp, MsgData *msgData, int windowId, int msgNo, int x, int y, int fontId, int color, int align);
void PokedexApp_SetupListWindows(PokedexAppData *pokedexApp);
void ov18_021EE44C(PokedexAppData *pokedexApp, int windowId, u32 msgNo);
void ov18_021EE508(PokedexAppData *pokedexApp, u32 windowId, u32 msgNo);
void PokedexApp_DrawCountNumber(PokedexAppData *pokedexApp, u32 windowId, u32 value);
void PokedexApp_DrawCountNumberAndCopy(PokedexAppData *pokedexApp, u32 windowId, u32 value);
void PokedexApp_DrawSeenAndCaughtCounts(PokedexAppData *pokedexApp);
void PokedexApp_DrawCenteredSpeciesName(PokedexAppData *pokedexApp, u32 species, int windowId);
void PokedexApp_DrawListEntryHeader(PokedexAppData *pokedexApp, u32 species, u32 entry);
void PokedexApp_DrawSearchEntryHeader(PokedexAppData *pokedexApp, u32 species, u32 entry);
void PokedexApp_DrawInfoLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawDexNumber(PokedexAppData *pokedexApp, u32 species, u32 windowId);
void PokedexApp_DrawSpeciesName(PokedexAppData *pokedexApp, u32 species, u32 windowId);
void PokedexApp_DrawCategory(PokedexAppData *pokedexApp, u32 species, u32 entry, u32 windowId);
void PokedexApp_DrawListEntryDetails(PokedexAppData *pokedexApp, u32 species, u32 entry);
void PokedexApp_DrawFlavorText(PokedexAppData *pokedexApp, u32 species, u32 entry, u32 windowId);
void PokedexApp_DrawHeightLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawWeightLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_PrintHeight(PokedexAppData *pokedexApp, u32 species, u32 state, u32 windowId, u32 x, u32 y, u32 color, u32 align);
void PokedexApp_DrawHeight(PokedexAppData *pokedexApp, u32 species, u32 entry, u32 windowId);
void PokedexApp_PrintWeight(PokedexAppData *pokedexApp, u32 species, u32 state, u32 windowId, u32 x, u32 y, u32 color, u32 align);
void PokedexApp_DrawWeight(PokedexAppData *pokedexApp, u32 species, u32 entry, u32 windowId);
void PokedexApp_DrawForeignDexLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawForeignDexNumber(PokedexAppData *pokedexApp, u32 species, u32 windowId, u32 language);
void PokedexApp_DrawForeignSpeciesName(PokedexAppData *pokedexApp, u32 species, u32 windowId);
void PokedexApp_DrawForeignCategory(PokedexAppData *pokedexApp, u32 species, u32 entry, u32 windowId);
void PokedexApp_DrawSearchEntryDetails(PokedexAppData *pokedexApp, u32 species, u32 entry);
void PokedexApp_SetupSearchWindows(PokedexAppData *pokedexApp);
void ov18_021EEE84(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchMessage(PokedexAppData *pokedexApp, u32 page);
void PokedexApp_DrawSearchOrderLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchNameLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchTypeLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchHeightLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchWeightLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchAreaLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchFormLabel(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchButton(PokedexAppData *pokedexApp, int windowId, int msgNo);
void PokedexApp_DrawSearchMenuPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchCriteria(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchOrderPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchNamePage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchTypePage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchHeightPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchWeightPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchAreaPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchFormPage(PokedexAppData *pokedexApp);
void PokedexApp_DrawSearchOrderValue(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchNameValue(PokedexAppData *pokedexApp, int windowId);
void PokedexApp_DrawSearchTypeValue(PokedexAppData *pokedexApp, u32 type, u32 windowId, s32 x);
void PokedexApp_DrawHeightValue(PokedexAppData *pokedexApp, u32 height, u32 windowId);
void PokedexApp_DrawWeightValue(PokedexAppData *pokedexApp, u32 weight, u32 windowId);
void PokedexApp_DrawSearchAreaValue(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_DrawSearchCriteriaSummary(PokedexAppData *pokedexApp);
void ov18_021EFFEC(PokedexAppData *pokedexApp);
void PokedexApp_PutWindowTilemap(PokedexAppData *pokedexApp, u32 windowId);
void PokedexApp_SetSearchSummaryVisible(PokedexAppData *pokedexApp, int visible);
void PokedexApp_ClearSearchEntryWindows(PokedexAppData *pokedexApp);
void ov18_021F0168(PokedexAppData *pokedexApp);
void ov18_021F018C(PokedexAppData *pokedexApp);
void ov18_021F01D4(PokedexAppData *pokedexApp);
void PokedexApp_SetupAreaWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawSelectedFormName(PokedexAppData *pokedexApp);
void PokedexApp_FreeAreaWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawTimeOfDayLabel(PokedexAppData *pokedexApp);
void PokedexApp_DrawAreaList(PokedexAppData *pokedexApp);
u32 PokedexApp_GetAreaListEntryMsg(PokedexAppData *pokedexApp, int index);
void PokedexApp_ScrollAreaList(PokedexAppData *pokedexApp, int direction);
void PokedexApp_SetupSizeCheckWindows(PokedexAppData *pokedexApp);
void PokedexApp_FreeSizeCheckWindows(PokedexAppData *pokedexApp);
void ov18_021F0858(PokedexAppData *pokedexApp);
void ov18_021F08E0(PokedexAppData *pokedexApp);
void PokedexApp_SetupFormsWindows(PokedexAppData *pokedexApp);
void PokedexApp_FreeFormsWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawFormsWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawFormList(PokedexAppData *pokedexApp);
u32 PokedexApp_GetFormNameMsg(PokedexAppData *pokedexApp, int index);
void PokedexApp_ScrollFormList(PokedexAppData *pokedexApp, int direction);
void PokedexApp_DrawFormsTitle(PokedexAppData *pokedexApp);
void PokedexApp_ClearFormsWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawFormCompareWindows(PokedexAppData *pokedexApp);
void PokedexApp_ClearFormCompareWindows(PokedexAppData *pokedexApp);
void PokedexApp_DrawCenteredFormName(PokedexAppData *pokedexApp, int windowId, int index);
void PokedexApp_InitSprites(PokedexAppData *pokedexApp);
void PokedexApp_FreeSprites(PokedexAppData *pokedexApp);
void PokedexApp_TickSprites(PokedexAppData *pokedexApp);
void PokedexApp_DeleteSprite(PokedexAppData *pokedexApp, int index);
void PokedexApp_SetSpriteTranslucent(PokedexAppData *pokedexApp, int index, int enabled);
void PokedexApp_SetSpriteAnim(PokedexAppData *pokedexApp, int a1, int a2);
int PokedexApp_IsSpriteAnimating(PokedexAppData *pokedexApp, int a1);
void PokedexApp_SetSpriteVisible(PokedexAppData *pokedexApp, int a1, int a2);
ManagedSprite *PokedexApp_NewSprite(PokedexAppData *pokedexApp, const ManagedSpriteTemplate *template);
void PokedexApp_MoveSprite(PokedexAppData *pokedexApp, int index, int dx, int dy, int subscreen);
void PokedexApp_SetSpritePos(PokedexAppData *pokedexApp, int index, s16 x, s16 y, int subscreen);
void PokedexApp_GetSpritePos(PokedexAppData *pokedexApp, int index, s16 *x, s16 *y, int subscreen);
void PokedexApp_LoadMonIconResources(PokedexAppData *pokedexApp, u32 count);
void PokedexApp_UnloadMonIconResources(PokedexAppData *pokedexApp, u32 count);
void PokedexApp_SetMonIcon(PokedexAppData *pokedexApp, u32 species, u32 form, int index);
void PokedexApp_SetMonIconSub(PokedexAppData *pokedexApp, u32 species, u32 form, int index);
void PokedexApp_DrawListIconGrid(PokedexAppData *pokedexApp, int a1);
void PokedexApp_ScrollListIconsBackward(PokedexAppData *pokedexApp, int a1, int a2, u8 a3);
void PokedexApp_ScrollListIconsForward(PokedexAppData *pokedexApp, int a1, int a2, u8 a3);
void PokedexApp_LoadMonPicResources(PokedexAppData *pokedexApp);
void PokedexApp_UnloadMonPicResources(PokedexAppData *pokedexApp);
void ov18_021F193C(PokedexAppData *pokedexApp);
void ov18_021F19EC(PokedexAppData *pokedexApp);
void PokedexApp_LoadMonPicToSprite(PokedexAppData *pokedexApp, int species, u8 form, u8 gender, u8 facing, int sprite, int mode);
void PokedexApp_ShowMonPic(PokedexAppData *pokedexApp, int species, int a2, int a3);
void PokedexApp_UpdateFootprintSprite(PokedexAppData *pokedexApp, u32 species, int a2, int a3);
void PokedexApp_UpdateTypeIcons(PokedexAppData *pokedexApp, u32 species, int a2, int a3);
void PokedexApp_UpdateCurrentLanguageIcon(PokedexAppData *pokedexApp, u32 species, int a2);
void PokedexApp_UpdateLanguageIcons(PokedexAppData *pokedexApp, u16 species, int a2);
void PokedexApp_SetupListSprites(PokedexAppData *pokedexApp);
void PokedexApp_SetSpritePairVisibleIfInternationalView(PokedexAppData *pokedexApp, int a1, int a2);
void PokedexApp_SetSpriteVisibleIfInternationalView(PokedexAppData *pokedexApp, int a1, int a2);
void ov18_021F2AC0(PokedexAppData *pokedexApp, int a1);
BOOL PokedexApp_IsTouchOnSprite0(PokedexAppData *pokedexApp, u32 x, u32 y);
void PokedexApp_MoveSpriteX(PokedexAppData *pokedexApp, int a1, s16 a2);
void PokedexApp_SetSpriteX(PokedexAppData *pokedexApp, int a1, s16 a2);
s16 ov18_021F2B9C(PokedexAppData *pokedexApp);
void PokedexApp_MoveSpriteToListCursor(PokedexAppData *pokedexApp, int a1);
void ov18_021F2BFC(PokedexAppData *pokedexApp);
void PokedexApp_UpdateListPageArrows(PokedexAppData *pokedexApp, int a1, int a2);
void PokedexApp_UpdateListScrollbarSize(PokedexAppData *pokedexApp, int a1, int a2);
int PokedexApp_IsTouchOnListScrollbar(PokedexAppData *pokedexApp, int a1, u32 x, u32 y);
int PokedexApp_DragListScrollbar(PokedexAppData *pokedexApp, int a1, u32 x, u32 y, int a4);
int PokedexApp_GetListScrollbarYForPage(PokedexAppData *pokedexApp, int a1, int a2, int a3);
u8 ov18_021F2E14(PokedexAppData *pokedexApp, int a1, int a2, int a3);
void PokedexApp_MoveListScrollbar(PokedexAppData *pokedexApp, int a1, int a2);
void PokedexApp_SyncListScrollbarToPage(PokedexAppData *pokedexApp, int a1, int a2);
void PokedexApp_UpdateCaughtMarkSprite(PokedexAppData *pokedexApp, int a1, int a2);
void PokedexApp_FreeListSprites(PokedexAppData *pokedexApp);
void PokedexApp_SetupSearchSprites(PokedexAppData *pokedexApp);
void PokedexApp_CreateSearchEntrySprites(PokedexAppData *pokedexApp);
void PokedexApp_UnloadSearchEntryResources(PokedexAppData *pokedexApp);
void PokedexApp_FreeSearchSprites(PokedexAppData *pokedexApp);
void ov18_021F3448(PokedexAppData *pokedexApp);
void PokedexApp_UpdateBodyTypeSprite(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchSprites(PokedexAppData *pokedexApp, int a1);
void PokedexApp_SetHeightSliderX(PokedexAppData *pokedexApp, int a1, int a2, int a3);
void PokedexApp_SetWeightSliderX(PokedexAppData *pokedexApp, int a1, int a2, int a3);
void PokedexApp_DrawHeightDigits(PokedexAppData *pokedexApp, int a1, u16 a2);
void PokedexApp_DrawWeightDigits(PokedexAppData *pokedexApp, int a1, u16 a2);
void PokedexApp_UpdateSliderLimitSprites(PokedexAppData *pokedexApp, u32 a1, int a2);
u32 PokedexApp_GetSliderValue(PokedexAppData *pokedexApp, int a1);
int PokedexApp_SliderValueToX(PokedexAppData *pokedexApp, int a1);
void ov18_021F3AD8(PokedexAppData *pokedexApp);
void ov18_021F3B2C(PokedexAppData *pokedexApp, int a1);
void PokedexApp_SetSearchEntrySpritesVisible(PokedexAppData *pokedexApp, int a1);
void ov18_021F3BA4(PokedexAppData *pokedexApp);
void ov18_021F3BD4(PokedexAppData *pokedexApp, int a1);
void ov18_021F3C30(PokedexAppData *pokedexApp);
void ov18_021F3C54(PokedexAppData *pokedexApp, int a1);
void ov18_021F3C88(PokedexAppData *pokedexApp);
void PokedexApp_GetSeenFormAndGender(PokedexAppData *pokedexApp, u32 index, u8 *form, u8 *gender);
void ov18_021F3D34(PokedexAppData *pokedexApp);
void ov18_021F3D6C(PokedexAppData *pokedexApp);
void ov18_021F3D80(PokedexAppData *pokedexApp, int a1);
void PokedexApp_SetupAreaSprites(PokedexAppData *pokedexApp);
void PokedexApp_FreeAreaSprites(PokedexAppData *pokedexApp);
void PokedexApp_LoadAreaSpriteResources(PokedexAppData *pokedexApp);
void PokedexApp_UnloadAreaSpriteResources(PokedexAppData *pokedexApp);
void PokedexApp_SetupAreaListScrollbar(PokedexAppData *pokedexApp);
void PokedexApp_UpdateTimeOfDayCursor(PokedexAppData *pokedexApp);
void PokedexApp_UpdatePlayerMarker(PokedexAppData *pokedexApp);
void PokedexApp_UpdatePlayerMarkerVisibility(PokedexAppData *pokedexApp);
void PokedexApp_CreateAreaMarkerSprites(PokedexAppData *pokedexApp);
void ov18_021F41C4(PokedexAppData *pokedexApp, int mapIndex, s16 *outX, s16 *outY, u8 *shape);
void ov18_021F42E4(PokedexAppData *pokedexApp, u32 mapIndex, s16 *outX, s16 *outY, u8 *shape);
void PokedexApp_UpdateAreaMarkers(PokedexAppData *pokedexApp);
void PokedexApp_HideAreaMarkers(PokedexAppData *pokedexApp);
void ov18_021F463C(PokedexAppData *pokedexApp);
void ov18_021F47C0(u32 mapId, u32 *specialAreas);
void ov18_021F47F8(PokedexAppData *pokedexApp, u32 specialAreas, int index);
void ov18_021F48AC(PokedexAppData *pokedexApp, u32 specialAreas, u32 hiddenSpecialAreas, int index, u32 palette);
BOOL ov18_021F4974(PokedexAppData *pokedexApp, u32 start, s32 x, s32 y);
void PokedexApp_SetupSizeCheckSprites(PokedexAppData *pokedexApp);
void PokedexApp_FreeSizeCheckSprites(PokedexAppData *pokedexApp);
void PokedexApp_LoadSizeCheckResources(PokedexAppData *pokedexApp);
void PokedexApp_UnloadSizeCheckResources(PokedexAppData *pokedexApp);
void PokedexApp_SetSizeCheckMonIcons(PokedexAppData *pokedexApp);
void PokedexApp_DrawSizeCheckMonPic(PokedexAppData *pokedexApp);
void PokedexApp_ScaleSizeCheckTrainer(PokedexAppData *pokedexApp);
void ov18_021F4EB0(u16 angle, s16 *x, s16 *y);
void ov18_021F4F98(PokedexAppData *pokedexApp, int index, u16 angle, s16 x, s16 y);
void ov18_021F4FC8(PokedexAppData *pokedexApp, int index, int from);
void PokedexApp_UpdateScalePans(PokedexAppData *pokedexApp, u32 angle);
void PokedexApp_InitWeightCheckAnim(PokedexAppData *pokedexApp, PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F50C0(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL PokedexApp_RunWeightCheckAnim(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
void ov18_021F5180(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s32 speed, s32 accel);
void ov18_021F5198(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s32 speed, s32 accel, s32 endSpeed);
BOOL ov18_021F51BC(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F51CC(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5238(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F52A4(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5310(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F537C(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F53E8(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5454(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
void ov18_021F54C0(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s16 speed, s16 accel);
void ov18_021F555C(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F55D8(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5638(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F56DC(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F57B4(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F588C(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5964(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5A3C(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5B14(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5BEC(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
BOOL ov18_021F5CC4(PokedexAppData_UnkSub0868_State77_Sub4 *anim);
void PokedexApp_SetupFormsSprites(PokedexAppData *pokedexApp);
void PokedexApp_FreeFormsSprites(PokedexAppData *pokedexApp);
void PokedexApp_CreateFormsSprites(PokedexAppData *pokedexApp);
void ov18_021F5EF0(PokedexAppData *pokedexApp, int index);
void PokedexApp_ShowFormMonPics(PokedexAppData *pokedexApp, int index, int mode);
void PokedexApp_SetFormListIcon(PokedexAppData *pokedexApp, int sprite, int index);
void PokedexApp_DrawFormListIcons(PokedexAppData *pokedexApp);
void PokedexApp_ScrollFormListIcons(PokedexAppData *pokedexApp, int direction);
void PokedexApp_MoveFormListIcons(PokedexAppData *pokedexApp, int dy);
void PokedexApp_UpdateScrollArrows(PokedexAppData *pokedexApp, s8 index, s8 count, int sprite);
void PokedexApp_SetScrollbarSizeAnim(PokedexAppData *pokedexApp, int sprite, int count, const u16 *thresholds, u32 numSizes);
int PokedexApp_GetScrollbarSizeAnim(PokedexAppData *pokedexApp, int count, const u16 *thresholds, u32 numSizes);
u32 PokedexApp_GetScrollbarLength(PokedexAppData *pokedexApp, int sprite, const u8 *lengths);
int PokedexApp_GetScrollbarMinY(PokedexAppData *pokedexApp, int sprite, const u8 *lengths, int centre, u32 trackLength);
int PokedexApp_GetScrollbarMaxY(PokedexAppData *pokedexApp, int sprite, const u8 *lengths, int centre, u32 trackLength);
BOOL PokedexApp_IsTouchOnFormScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y);
BOOL PokedexApp_IsTouchOnAreaScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y);
BOOL PokedexApp_DragFormScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y);
BOOL PokedexApp_DragAreaScrollbar(PokedexAppData *pokedexApp, u32 x, u32 y);
u32 PokedexApp_GetFormScrollbarY(PokedexAppData *pokedexApp, int index);
u32 PokedexApp_GetAreaScrollbarY(PokedexAppData *pokedexApp, int index);
u32 ov18_021F6540(PokedexAppData *pokedexApp, int sprite, u32 y, u32 step);
void PokedexApp_MoveScrollbar(PokedexAppData *pokedexApp, int sprite, int dy);
void PokedexApp_SyncFormScrollbar(PokedexAppData *pokedexApp);
void PokedexApp_SyncAreaScrollbar(PokedexAppData *pokedexApp);
void PokedexApp_ShowFormListSprites(PokedexAppData *pokedexApp);
void PokedexApp_HideFormListSprites(PokedexAppData *pokedexApp);
void PokedexApp_ShowFormCompareSprites(PokedexAppData *pokedexApp);
void PokedexApp_HideFormCompareSprites(PokedexAppData *pokedexApp);
void ov18_021F6844(PokedexAppData *pokedexApp, int sprite, int index);
void PokedexApp_ShowFormCompareMonPic(PokedexAppData *pokedexApp, int side, int index, int mode);
void ov18_021F6984(PokedexAppData *pokedexApp, int side, int index);
void PokedexApp_UpdateFormCompareSelector(PokedexAppData *pokedexApp);
void PokedexApp_SetPlayerMarkerAnim(PokedexAppData *pokedexApp, int inArea);
void PokedexApp_LoadMonPicToBg(PokedexAppData *pokedexApp, u16 species, u8 form, u8 gender, u8 facing);
void PokedexApp_SetMonPicBgAffine(PokedexAppData *pokedexApp, int y, int scale);
int ov18_021F6B00(PokedexAppData *pokedexApp);
int PokedexApp_HandleListInput(PokedexAppData *pokedexApp, int mode);
void PokedexApp_CreateSearchMenuGrid(PokedexAppData *pokedexApp, int initialPos);
u32 PokedexApp_HandleSearchMenuInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchMenuCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchMenuGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchMenuGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchOrderGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchOrderInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchOrderCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchOrderGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchOrderGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchNameGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchNameInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchNameCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchNameGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchNameGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchTypeGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchTypeInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchTypeCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchTypeGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchTypeGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchHeightGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchHeightInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchHeightCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchHeightGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchHeightGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchWeightGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchWeightInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchWeightCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchWeightGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchWeightGridTouch(PokedexAppData *pokedexApp, int target, int input);
u32 PokedexApp_GetTouchHeldX(u32 *x);
void PokedexApp_CreateSearchAreaGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchAreaInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchAreaCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchAreaGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchAreaGridTouch(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_CreateSearchFormGrid(PokedexAppData *pokedexApp);
u32 PokedexApp_HandleSearchFormInput(PokedexAppData *pokedexApp);
void PokedexApp_UpdateSearchFormCursor(PokedexAppData *pokedexApp, int target);
void PokedexApp_OnSearchFormGridMove(PokedexAppData *pokedexApp, int target, int input);
void PokedexApp_OnSearchFormGridTouch(PokedexAppData *pokedexApp, int target, int input);
u32 ov18_021F7974(PokedexAppData *pokedexApp, int mode);
void PokedexApp_FreeGridInput(PokedexAppData *pokedexApp);
void PokedexApp_DummyGridCallback(PokedexAppData *pokedexApp, int target, int input);
int PokedexApp_HandleEntryTabTouch(PokedexAppData *pokedexApp, u32 *touched);
u32 PokedexApp_HandleAreaInput(PokedexAppData *pokedexApp, u32 *touched);
u32 PokedexApp_HandleSizeCheckInput(PokedexAppData *pokedexApp, u32 *touched);
u32 PokedexApp_HandleFormsInput(PokedexAppData *pokedexApp, u32 *touched);
u32 PokedexApp_HandleFormCompareInput(PokedexAppData *pokedexApp, u32 *touched);
u32 ov18_021F7E70(PokedexAppData *pokedexApp, u32 *touched);
BOOL PokedexApp_RunSearch(PokedexAppData *pokedexApp, u8 natDexFlag, u32 order, u32 letter, u32 type1, u32 type2, u32 heightMin, u32 heightMax, u32 weightMin, u32 weightMax, u32 areaMask, u32 bodyType);
void *PokedexApp_LoadSortData(u32 index, u32 *count);
void PokedexApp_FilterSeen(u16 *dst, u32 *count, const Pokedex *pokedex, const u16 *src, u32 length);
void PokedexApp_StoreSearchResults(PokedexAppData_UnkSub0878 *result, const Pokedex *pokedex, const u16 *src, u32 count);
void PokedexApp_ApplySortOrder(u32 order, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByLetter(u32 letter, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByType(u32 type, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByBodyType(u32 bodyType, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByArea(PokedexAppData *pokedexApp, u32 areaMask, u16 *dst, u32 *count, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByHeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByWeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_IntersectSpeciesLists(u16 *dst, u32 *count, const u16 *lookup, u32 lookupCount, const u16 *src, u32 srcCount, BOOL seenOnly, const Pokedex *pokedex);
u32 PokedexApp_GetListCursorIndex(PokedexAppData *pokedexApp);
u16 PokedexApp_GetCursorSpecies(PokedexAppData *pokedexApp);
u32 PokedexApp_FindSpeciesInResults(PokedexAppData_UnkSub0878 *results, u16 species);
void PokedexApp_BuildListEntries(PokedexAppData *pokedexApp, int mode);
u32 PokedexApp_GetListLength(PokedexAppData *pokedexApp, int mode);
u32 PokedexApp_GetLastListPage(PokedexAppData *pokedexApp, int mode);
u32 ov18_021F8970(u32 value);

#endif // GUARD_POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_INTERNAL_H
