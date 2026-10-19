#ifndef POKEHEARTGOLD_TRAINER_CARD_DATA_H
#define POKEHEARTGOLD_TRAINER_CARD_DATA_H

#include "field_system.h"
#include "overlay_trainer_card.h"

// The trainer card as it is shown by the trainer card app and exchanged over a link.
typedef struct TrainerCardData {
    u8 version; // game version of the card's owner
    u8 language;
    u8 unk_02;
    u8 stars; // selects the card colour
    u8 unk_04_0 : 1;
    u8 liveIgt : 1; // play time is read from igt instead of the stored hours/minutes
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 unk_04_5 : 3;
    u8 caseVariant; // union room avatar; 0xFF = default front
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
    u32 linkCount; // link trades, battles and contests (local + Wi-Fi)
    u32 linkBattleWins;
    u32 linkBattleLosses;
    u32 linkTrades;
    u32 badgeShininess[8];
    u8 signature[0x600]; // 1 bit per pixel
    u16 checksum;        // XOR of all bytes, taken while checksum and received are 0
    u16 received;        // set once a link partner's card has arrived
} TrainerCardData;

TrainerCardAppArgs *TrainerCardAppArgs_New(enum HeapID heapID);
void TrainerCardData_Build(u8 unk_04_0, u8 liveIgt, u8 unk_02, u8 caseVariant, FieldSystem *fieldSystem, TrainerCardAppArgs *trainerCard);
void TrainerCardData_Free(TrainerCardAppArgs *trainerCard);
void LinkTrainerCard_StartTask(FieldSystem *fieldSystem);
u8 FieldSystem_GetTrainerCardStars(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_TRAINER_CARD_DATA_H
