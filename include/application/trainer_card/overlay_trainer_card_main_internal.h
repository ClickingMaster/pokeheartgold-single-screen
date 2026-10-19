#ifndef POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H

#include "bg_window.h"
#include "igt.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "overlay_trainer_card.h"
#include "overlay_trainer_card_main.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "yes_no_prompt.h"

// The card payload at the start of TrainerCardAppArgs, as the trainer card viewer reads it.
typedef struct TrainerCardMainCard {
    u8 version; // game version of the card's owner (HG/SS cards show badges)
    u8 language;
    u8 unk_02;
    u8 stars; // selects the card colour
    u8 unk_04_0 : 1;
    u8 liveIgt : 1; // play time is read from igt instead of the stored hours/minutes
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 unk_04_5 : 3;
    u8 caseVariant; // 0xFF = default front
    u16 badges;     // one bit per badge
    u16 name[8];
    IGT *igt;
    u32 money;
    u32 pokedexCount;
    u32 score;
    u16 trainerId;
    u16 playHours;
    u16 clearHours;
    u8 playMinutes;
    u8 startYear;
    u8 startMonth;
    u8 startDay;
    u8 clearYear;
    u8 clearMonth; // 0 = game not cleared
    u8 clearDay;
    u8 clearMinutes;
    u8 unk_36[2];
    u32 linkCount;
    u32 linkBattleWins;
    u32 linkBattleLosses;
    u32 linkTrades;
    u8 badgeShininess[0x20];
    u8 signature[0x600]; // 1 bit per pixel
} TrainerCardMainCard;

typedef struct TrainerCardMainSprites {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[2][4]; // [screen][char/pltt/cell/anim]
    SpriteResource *resourceObjects[2][4];
    Sprite *badgeSprites[16];
    u8 unk_1AC[0x44];
    Sprite *buttonSprites[2];
    void *paletteFiles[16];
    NNSG2dPaletteData *palettes[16];
} TrainerCardMainSprites;

typedef struct TrainerCardMain {
    BgConfig *bgConfig;
    Window windows[14];
    TrainerCardAppArgs *args;
    TrainerCardMainCard *card;  // the same object as args
    u8 signaturePixels[0x3000]; // the signature unpacked to 1 byte per pixel
    void *frontCharFile;
    NNSG2dCharacterData *frontCharData;
    BOOL showingBack;
    u32 unk_30F8;
    fx32 scaleX;
    fx32 scaleY;
    u32 unk_3104;
    int unk_3108;
    BOOL inTouchMode;
    u8 unk_3110[0xC];
    TrainerCardMainSprites sprites;
    int flipSpeed;
    u32 unk_3398;
    u32 unk_339C;
    u8 unk_33A0[4][2];
    u8 unk_33A8[8];
    void *frontScreenFile;
    NNSG2dScreenData *frontScreen;
    void *badgeFrameScreenFile;
    NNSG2dScreenData *badgeFrameScreen;
    YesNoPrompt *yesNoPrompt;
    MsgData *msgData;
    String *unk_33C8;
    String *colonString;
    String *formattedString;
    String *numberString;
    String *buttonStrings[3];
    String *promptStrings[2];
    String *labelStrings[14];
    u8 badgeFlags[16];
    u8 unk_3434;
    u8 unk_3435;
    u8 subState;
    u8 unk_3437;
    u8 unk_3438;
    u8 colonBlinkTimer;
    u8 showKantoBadges : 1;
    u8 isConnected : 1;
    u8 scaleChanged : 1;
    u8 unk_343A_3 : 5;
    u8 unk_343B;
    u8 textPrinterId;
    u8 frame;
    u8 textSpeed;
    u8 scrollOffset;
    u8 touchX;
    u8 touchY;
    u8 unk_3442[2];
} TrainerCardMain;

void TrainerCardMain_InitSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_CreateBadgeSprites(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges);
void TrainerCardMain_CreateButtonSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_FreeSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_SetButtonSpriteAnim(TrainerCardMainSprites *sprites, int button, int sequence, int draw);
void TrainerCardMain_InitUnk33A0Pairs(u8 *pairs);

#endif // POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_MAIN_INTERNAL_H
