#ifndef POKEHEARTGOLD_TRAINER_CARD_DATA_H
#define POKEHEARTGOLD_TRAINER_CARD_DATA_H

#include "field_system.h"
#include "overlay_trainer_card.h"

// The trainer card as it is shown by the trainer card app and exchanged over a link (0x66C bytes).
// The app reads the same layout as TrainerCardMainCard (overlay_trainer_card_main_internal.h).
typedef struct TrainerCardData {
    u8 version;  // 0x00, game version of the card's owner
    u8 language; // 0x01
    u8 unk_02;
    u8 stars; // 0x03, selects the card colour
    u8 unk_04_0 : 1;
    u8 liveIgt : 1; // 0x04, play time is read from igt instead of the stored hours/minutes
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 unk_04_5 : 3;
    u8 caseVariant;   // 0x05, union room avatar; 0xFF = default front
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
    u32 linkCount;        // 0x38, link trades, battles and contests (local + Wi-Fi)
    u32 linkBattleWins;   // 0x3C
    u32 linkBattleLosses; // 0x40
    u32 linkTrades;       // 0x44
    u32 badgeShininess[8]; // 0x48
    u8 signature[0x600];  // 0x68, 1 bit per pixel
    u16 checksum;         // 0x668, XOR of all bytes, taken while checksum and received are 0
    u16 received;         // 0x66A, set once a link partner's card has arrived
} TrainerCardData;

TrainerCardAppArgs *TrainerCardAppArgs_New(enum HeapID heapID);
void TrainerCardData_Build(u8 unk_04_0, u8 liveIgt, u8 unk_02, u8 caseVariant, FieldSystem *fieldSystem, TrainerCardAppArgs *trainerCard);
void TrainerCardData_Free(TrainerCardAppArgs *trainerCard);
void LinkTrainerCard_StartTask(FieldSystem *fieldSystem);
u8 FieldSystem_GetTrainerCardStars(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_TRAINER_CARD_DATA_H
