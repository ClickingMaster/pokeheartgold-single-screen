#ifndef POKEHEARTGOLD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H

#include "bg_window.h"
#include "igt.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "overlay_trainer_card.h"
#include "overlay_trainer_card_main.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

// The card payload at the start of TrainerCardAppArgs, as the trainer card viewer reads it.
typedef struct TrainerCardMainCard {
    u8 version; // 0x00, game version of the card's owner (HG/SS cards show badges)
    u8 language;
    u8 unk_02;
    u8 stars; // 0x03, selects the card colour
    u8 unk_04_0 : 1;
    u8 liveIgt : 1; // 0x04, play time is read from igt instead of the stored hours/minutes
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 unk_04_5 : 3;
    u8 caseVariant;   // 0x05, 0xFF = default front
    u16 badges;       // 0x06, one bit per badge
    u16 name[8];      // 0x08
    IGT *igt;         // 0x18
    u32 money;        // 0x1C
    u32 pokedexCount; // 0x20
    u32 score;        // 0x24
    u16 trainerId;    // 0x28
    u16 playHours;    // 0x2A
    u16 clearHours;   // 0x2C
    u8 playMinutes;   // 0x2E
    u8 startYear;     // 0x2F
    u8 startMonth;    // 0x30
    u8 startDay;      // 0x31
    u8 clearYear;     // 0x32
    u8 clearMonth;    // 0x33, 0 = game not cleared
    u8 clearDay;      // 0x34
    u8 clearMinutes;  // 0x35
    u8 unk_36[2];
    u32 linkCount;
    u32 linkBattleWins;
    u32 linkBattleLosses;
    u32 linkTrades;
    u8 unk_48[0x20];
    u8 signature[0x600]; // 0x68, 1 bit per pixel
} TrainerCardMainCard;

typedef struct TrainerCardMainSprites {
    SpriteList *spriteList;                 // 0x000
    G2dRenderer renderer;                   // 0x004
    GF_2DGfxResMan *resourceManagers[2][4]; // 0x12C, [screen][char/pltt/cell/anim]
    SpriteResource *resourceObjects[2][4];  // 0x14C
    Sprite *badgeSprites[16];               // 0x16C
    u8 unk_1AC[0x44];
    Sprite *buttonSprites[2];        // 0x1F0
    void *paletteFiles[16];          // 0x1F8
    NNSG2dPaletteData *palettes[16]; // 0x238
} TrainerCardMainSprites;            // size: 0x278

typedef struct TrainerCardMain {
    BgConfig *bgConfig;                 // 0x0000
    Window windows[14];                 // 0x0004
    TrainerCardAppArgs *args;           // 0x00E4
    TrainerCardMainCard *card;          // 0x00E8, the same object as args
    u8 signaturePixels[0x3000];         // 0x00EC, the signature unpacked to 1 byte per pixel
    void *frontCharFile;                // 0x30EC
    NNSG2dCharacterData *frontCharData; // 0x30F0
    BOOL showingBack;                   // 0x30F4
    u32 unk_30F8;
    fx32 scaleX; // 0x30FC
    fx32 scaleY; // 0x3100
    u32 unk_3104;
    int unk_3108;
    BOOL inTouchMode; // 0x310C
    u8 unk_3110[0xC];
    TrainerCardMainSprites sprites; // 0x311C
    int flipSpeed;                  // 0x3394
    u32 unk_3398;
    u32 unk_339C;
    u8 unk_33A0[4][2];
    u8 unk_33A8[8];
    void *frontScreenFile;              // 0x33B0
    NNSG2dScreenData *frontScreen;      // 0x33B4
    void *badgeFrameScreenFile;         // 0x33B8
    NNSG2dScreenData *badgeFrameScreen; // 0x33BC
    YesNoPrompt *yesNoPrompt;           // 0x33C0
    MsgData *msgData;                   // 0x33C4
    String *unk_33C8;
    String *colonString;      // 0x33CC
    String *formattedString;  // 0x33D0
    String *numberString;     // 0x33D4
    String *buttonStrings[3]; // 0x33D8
    String *promptStrings[2]; // 0x33E4
    String *labelStrings[14]; // 0x33EC
    u8 badgeFlags[16];        // 0x3424
    u8 unk_3434;
    u8 unk_3435;
    u8 subState; // 0x3436
    u8 unk_3437;
    u8 unk_3438;
    u8 colonBlinkTimer;     // 0x3439
    u8 showKantoBadges : 1; // 0x343A
    u8 isConnected : 1;
    u8 scaleChanged : 1;
    u8 unk_343A_3 : 5;
    u8 unk_343B;
    u8 textPrinterId; // 0x343C
    u8 frame;         // 0x343D
    u8 textSpeed;     // 0x343E
    u8 scrollOffset;  // 0x343F
    u8 touchX;        // 0x3440
    u8 touchY;        // 0x3441
    u8 unk_3442[2];
} TrainerCardMain; // size: 0x3444

#endif // POKEHEARTGOLD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H
