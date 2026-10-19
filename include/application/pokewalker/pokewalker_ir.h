#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H

#include "global.h"

#include "constants/pokemon.h"

typedef struct PokewalkerIrClock {
    u32 rtcOffsetLow; // OS_GetOwnerRtcOffset()
    u32 rtcOffsetHigh;
    u32 entropy[8]; // OS_GetLowEntropyData()
} PokewalkerIrClock;

// Identity block exchanged with the Pokewalker (sent with the 0x32/0x40/0x52/0x60 commands).
typedef struct PokewalkerIrIdentity {
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    u32 trainerId;
    PokewalkerIrClock clock;
    u8 unk_38[0x10];
    u16 trainerName[PLAYER_NAME_LENGTH + 1];
    u8 unk_58[2];
    u8 unk_5A;
    u8 isRegistered : 1;
    u8 hasStrollMon : 1;
    u8 hasWalkerMon : 1;
    u8 unk_5B_3 : 5;
    u8 unk_5C;
    u8 unk_5D;
    u8 unk_5E[2];
    u32 currentTime;    // seconds since 2000, big endian
    u32 steps;          // big endian on the wire
} PokewalkerIrIdentity;

// A Pokemon in the Pokewalker's own format (stroll data, caught Pokemon, gift Pokemon).
typedef struct PokewalkerMon {
    u16 species;
    u16 heldItem;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 form : 5;
    u8 gender : 2;
    u8 unk_0D_7 : 1;
    u8 canFlip : 1; // BASE_FLIP of the species
    u8 isShiny : 1;
    u8 unk_0E_2 : 6;
    u8 padding_0F;
} PokewalkerMon;

// Pokemon the Pokewalker holds for the game.
typedef struct PokewalkerIrGiftMon {
    PokewalkerMon mon;
    u8 unk_10[4];
    u32 otId;
    u8 unk_18[2];
    u16 metLocation;
    u8 unk_1C[2];
    u16 otName[8];
    u8 otGender : 1;
    u8 unk_2E_1 : 7;
    u8 ability;
    u8 pokeball;
    u8 unk_31[0x2CB];
} PokewalkerIrGiftMon;

// Item the Pokewalker holds for the game.
typedef struct PokewalkerIrGiftItem {
    u8 unk_00[6];
    u16 item;
    u8 unk_08[0x180];
} PokewalkerIrGiftItem;

typedef struct PokewalkerIrItem {
    u16 item;
    u16 unk_02;
} PokewalkerIrItem;

// Flags byte at the start of the block read from the Pokewalker. Bits 4-7: block 0-3 holds data the
// game has not taken yet (writing that block is refused).
typedef struct PokewalkerIrBlockFlags {
    u8 handshake : 4;
    u8 hasData0 : 1;
    u8 hasGiftMon : 1;
    u8 hasGiftItem : 1;
    u8 hasData3 : 1;
} PokewalkerIrBlockFlags;

// Block read from the Pokewalker (flags + data blocks 0-2; block 3 is transferred separately).
typedef struct PokewalkerIrBlocks {
    PokewalkerIrBlockFlags flags;
    u8 unk_01[3];
    u8 block0[0x240];
    PokewalkerIrGiftMon giftMon;
    PokewalkerIrGiftItem giftItem;
} PokewalkerIrBlocks;

typedef struct PokewalkerIrRecord {
    u32 value;
    u8 unk_04[4];
    u16 gameVersion;
    u16 species;
    u16 peerSpecies;                             // species when gameVersion is 7 or 8
    u16 item;
    u16 peerTrainerName[PLAYER_NAME_LENGTH + 1]; // trainer of the other Pokewalker
    u16 nickname[POKEMON_NAME_LENGTH + 1];       // walking Pokemon
    u16 peerNickname[POKEMON_NAME_LENGTH + 1];   // the other Pokemon
    u16 routeName[21];
    u8 course;
    u8 friendship;
    u16 steps;
    u16 peerSteps;
    u32 strollSteps;
    u32 unk_80;
    u8 eventType; // 1-29, 0 = empty entry
    u8 form : 5;  // walking Pokemon
    u8 gender : 2;
    u8 isShiny : 1;
    u8 peerForm : 5; // the other Pokemon
    u8 peerGender : 2;
    u8 peerIsShiny : 1;
    u8 unk_87;
} PokewalkerIrRecord;

typedef struct PokewalkerIrStatus {
    u32 unk_00;
    u32 unk_04;
    u8 giveStarfBerry : 1;
    u8 unk_08_1 : 7;
    u8 unk_09;
    u16 watts;
    PokewalkerMon caughtMons[3];
    PokewalkerIrItem items[3];
    PokewalkerIrItem unk_48[10];
    u32 unk_70[7];
    PokewalkerIrRecord records[24];
} PokewalkerIrStatus;

u16 PokewalkerIr_Checksum(u8 *bytes, u32 len);
void PokewalkerIr_MarkDisconnected(void);
void PokewalkerIr_ResetLink(void);
void PokewalkerIr_InitLink(void);
u32 PokewalkerIr_IsConnected(void);
void PokewalkerIr_SendPacket(const u8 *src, int len, u8 command, u8 arg, u32 sessionId);
void PokewalkerIr_CloseLink(int result);
void PokewalkerIr_Disconnect(void);
void PokewalkerIr_Send(void *data, int len, u8 command, u8 arg);
void PokewalkerIr_PollDummy(void);
BOOL PokewalkerIr_ShouldDropShortPacket(u32 size);
BOOL PokewalkerIr_CheckTimeout(void);
BOOL PokewalkerIr_VerifyPacket(u32 size);
int PokewalkerIr_Poll(void);
void PokewalkerIr_SetConnectCallback(void *callback);
void PokewalkerIr_SetReceiveCallback(void *callback);
void PokewalkerIr_SetCloseCallback(void *callback);
void PokewalkerIr_SetSendParam(u8 value);
void PokewalkerIr_SetCompressBuffer(void *buffer);
void PokewalkerIr_CompressBlocks(u8 *src, u8 *dst, u16 size, u8 *sizes);
void PokewalkerIr_SetError(int error);
void PokewalkerIr_StoreClock(void);
void PokewalkerIr_StoreCurrentTime(void);
void PokewalkerIr_SetIdentities(PokewalkerIrIdentity *identity, PokewalkerIrIdentity *peerIdentity);
void PokewalkerIr_BeginTransfer(void *data, u32 address, int size);
void PokewalkerIr_SendChunk(void);
void PokewalkerIr_OnSendComplete(void);
void PokewalkerIr_RequestChunk(void);
void PokewalkerIr_OnReceiveComplete(void);
void PokewalkerIr_OnConnect(int result);
void PokewalkerIr_RestartTransfer(void);
void PokewalkerIr_ValidatePeer(void);
void PokewalkerIr_OnReceive(void *data, u32 size, int command);
void PokewalkerIr_ThreadMain(void *arg);
void PokewalkerIr_Start(void *stack, u32 stackSize, u32 priority);
void PokewalkerIr_Stop(void);
void PokewalkerIr_RequestEraseRecords(int active);
void PokewalkerIr_RequestUpload(u8 *uploadImage, u8 *strollData, u8 *records);
void PokewalkerIr_RequestReceive(u8 *walkerMon, u8 *status, u8 *blocks, u8 *records);
void PokewalkerIr_RequestSendStroll(u8 *strollData, u8 *records, u8 *status, u8 *blocks);
void PokewalkerIr_RequestReceiveGifts(PokewalkerIrStatus *status, void *records);
void *PokewalkerIrStatus_IsConnected(void);
u8 PokewalkerIr_GetChunkProgress(void);
u8 PokewalkerIr_GetTotalProgress(void);
u32 PokewalkerIr_GetAndClearError(void);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H
