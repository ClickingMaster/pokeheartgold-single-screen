#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H

#include "global.h"

#include "constants/pokemon.h"

typedef struct PokewalkerIrClock {
    u32 rtcOffsetLow; // OS_GetOwnerRtcOffset()
    u32 rtcOffsetHigh;
    u32 entropy[8]; // OS_GetLowEntropyData()
} PokewalkerIrClock;

// Identity block exchanged with the Pokewalker (0x68 bytes, sent with the 0x32/0x40/0x52/0x60 commands).
typedef struct PokewalkerIrIdentity {
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    u32 unk_0C;
    PokewalkerIrClock clock; // 0x10
    u8 unk_38[0x23];
    u8 unk_5B_0 : 1;
    u8 unk_5B_1 : 1;
    u8 unk_5B_2 : 1;
    u8 unk_5B_3 : 5;
    u8 unk_5C;
    u8 unk_5D;
    u8 unk_5E[2];
    u32 currentTime;    // 0x60, seconds since 2000, big endian
    u32 unk_64;         // big endian on the wire
} PokewalkerIrIdentity; // size=0x68

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
} PokewalkerMon; // size=0x10

// Pokemon the Pokewalker holds for the game (block at Pokewalker address 0xBA44).
typedef struct PokewalkerIrGiftMon {
    PokewalkerMon mon;
    u8 unk_10[4];
    u32 otId; // 0x14
    u8 unk_18[2];
    u16 metLocation; // 0x1A
    u8 unk_1C[2];
    u16 otName[8];   // 0x1E
    u8 otGender : 1; // 0x2E
    u8 unk_2E_1 : 7;
    u8 ability;  // 0x2F
    u8 pokeball; // 0x30
    u8 unk_31[0x2CB];
} PokewalkerIrGiftMon; // size=0x2FC

// Item the Pokewalker holds for the game (block at Pokewalker address 0xBD40).
typedef struct PokewalkerIrGiftItem {
    u8 unk_00[6];
    u16 item; // 0x06
    u8 unk_08[0x180];
} PokewalkerIrGiftItem; // size=0x188

typedef struct PokewalkerIrItem {
    u16 item;
    u16 unk_02;
} PokewalkerIrItem;

// Flags byte at the start of the 0x6C8-byte block at Pokewalker address 0xB800. Bits 4-7: block 0-3 holds data the
// game has not taken yet (writing that block is refused).
typedef struct PokewalkerIrBlockFlags {
    u8 handshake : 4;
    u8 hasData0 : 1;
    u8 hasGiftMon : 1;
    u8 hasGiftItem : 1;
    u8 hasData3 : 1;
} PokewalkerIrBlockFlags;

// Block read from Pokewalker address 0xB800 (flags + data blocks 0-2; block 3 at 0xBF00 is transferred separately).
typedef struct PokewalkerIrBlocks {
    PokewalkerIrBlockFlags flags;
    u8 unk_01[3];
    u8 block0[0x240];              // 0x004, address 0xB804
    PokewalkerIrGiftMon giftMon;   // 0x244, address 0xBA44
    PokewalkerIrGiftItem giftItem; // 0x540, address 0xBD40
} PokewalkerIrBlocks;              // size=0x6C8

typedef struct PokewalkerIrRecord {
    u32 value;
    u8 unk_04[4];
    u16 unk_08;
    u16 unk_0A; // species
    u16 unk_0C; // species when unk_08 is 7 or 8
    u8 unk_0E[0x68];
    u8 unk_76;
    u8 unk_77;
    u16 unk_78;
    u16 unk_7A;
    u32 unk_7C;
    u32 unk_80;
    u8 eventType; // 1-29, 0 = empty entry
    u8 unk_85[3];
} PokewalkerIrRecord; // size=0x88

// Data read from Pokewalker address 0xCE80 (0xD4C bytes).
typedef struct PokewalkerIrStatus {
    u32 unk_00;
    u32 unk_04;
    u8 giveStarfBerry : 1; // 0x08
    u8 unk_08_1 : 7;
    u8 unk_09;
    u16 unk_0A;
    PokewalkerMon caughtMons[3]; // 0x0C
    PokewalkerIrItem items[3];   // 0x3C
    PokewalkerIrItem unk_48[10]; // 0x48
    u32 unk_70[7];
    PokewalkerIrRecord records[24];
} PokewalkerIrStatus; // size=0xD4C

u16 PokewalkerIr_Checksum(u8 *bytes, u32 len);
void ov112_021E5938(void);
void PokewalkerIr_ResetLink(void);
void PokewalkerIr_InitLink(void);
u32 PokewalkerIr_IsConnected(void);
void PokewalkerIr_SendPacket(const u8 *src, int len, u8 command, u8 arg, u32 sessionId);
void PokewalkerIr_CloseLink(int result);
void PokewalkerIr_Disconnect(void);
void PokewalkerIr_Send(void *data, int len, u8 command, u8 arg);
void ov112_021E5A80(void);
BOOL ov112_021E5A84(u32 size);
BOOL PokewalkerIr_CheckTimeout(void);
BOOL PokewalkerIr_VerifyPacket(u32 size);
int PokewalkerIr_Poll(void);
void PokewalkerIr_SetConnectCallback(void *callback);
void PokewalkerIr_SetReceiveCallback(void *callback);
void PokewalkerIr_SetCloseCallback(void *callback);
void ov112_021E5D5C(u8 value);
void PokewalkerIr_SetCompressBuffer(void *buffer);
void PokewalkerIr_CompressBlocks(u8 *src, u8 *dst, u16 size, u8 *sizes);
void PokewalkerIr_SetError(int error);
void PokewalkerIr_StoreClock(void);
void PokewalkerIr_StoreCurrentTime(void);
void PokewalkerIr_SetIdentities(void *identity, void *peerIdentity);
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
void ov112_021E7484(int active);
void ov112_021E74A8(u8 *a, u8 *b, u8 *c);
void ov112_021E7548(u8 *a, u8 *b, u8 *c, u8 *d);
void ov112_021E7594(u8 *a, u8 *b, u8 *c, u8 *d);
void ov112_021E7620(void *a, void *b);
void *ov112_021E7668(void);
u8 PokewalkerIr_GetChunkProgress(void);
u8 PokewalkerIr_GetTotalProgress(void);
u32 PokewalkerIr_GetAndClearError(void);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_H
