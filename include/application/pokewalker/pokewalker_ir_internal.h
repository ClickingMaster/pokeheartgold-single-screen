#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H

#include "application/pokewalker/pokewalker_ir.h"

// IR link layer state (0x38 bytes).
typedef struct PokewalkerIrLink {
    u8 unk_00;
    u8 unk_01[3];
    u32 sessionId;                                              // 0x04
    u32 connected;                                              // 0x08
    void (*onConnect)(int result, int arg);                     // 0x0C
    u32 numDisconnects;                                         // 0x10
    void (*onClose)(int result);                                // 0x14
    u32 closing;                                                // 0x18
    void (*onReceive)(void *data, u8 size, u8 command, u8 arg); // 0x1C
    u32 ownId;                                                  // 0x20
    u32 handshakeStarted;                                       // 0x24
    u32 handshakeState;                                         // 0x28
    u32 active;                                                 // 0x2C
    u64 lastReceiveTick;                                        // 0x30
} PokewalkerIrLink;

// Header of an IR packet; the payload follows (ov112_021FFA20).
typedef struct PokewalkerIrPacket {
    u8 command;
    u8 arg;
    u8 checksumLo;
    u8 checksumHi;
    u32 sessionId;
} PokewalkerIrPacket;

// IR transfer work (0x021FFAA4..). The transfer state below is addressed from 0x021FFB24.
typedef struct PokewalkerIrWork {
    u16 unk_00;
    u16 unk_02;
    u32 blockIndex; // 0x04
    void *unk_08;
    void *unk_0C;
    void *uploadImage;
    int error; // 0x14
    u32 unk_18;
    u32 unk_1C;
    PokewalkerIrIdentity *identity;     // 0x20
    PokewalkerIrIdentity *peerIdentity; // 0x24
    void *unk_28;
    void *strollData;
    u8 *records;
    PokewalkerIrBlocks *blocks; // 0x34
    PokewalkerIrStatus *status; // 0x38
    u64 transferStartTick;      // 0x3C
    u8 *compressedSizes;        // 0x44
    u8 *compressBuffer;         // 0x48
    u8 *sizes0;                 // 0x4C, compressed block sizes of the first upload (0x021FFAF0)
    void *upload0;              // 0x50, its compressed data
    u8 *sizes1;                 // 0x54 (0x021FFAF8)
    void *upload1;              // 0x58
} PokewalkerIrWork;

typedef struct PokewalkerIrTransfer {
    u8 unk_00[0x1C];
    u8 command; // 0x1C
    u8 step;    // 0x1D
    u8 unk_1E[2];
    u8 *data;               // 0x20, source data (send) or Pokewalker address (receive)
    u8 *dataStart;          // 0x24
    u32 address;            // 0x28, Pokewalker address (send) or destination (receive)
    u32 addressStart;       // 0x2C
    u16 remaining;          // 0x30
    u16 size;               // 0x32
    u16 numChunks;          // 0x34
    u16 chunk;              // 0x36
    u16 totalChunks;        // 0x38
    u16 totalChunk;         // 0x3A
    volatile u16 totalSize; // 0x3C
} PokewalkerIrTransfer;

extern u8 _021FF500[];
extern PokewalkerIrLink _021FF9E0;
extern PokewalkerIrPacket ov112_021FFA18;
extern u8 ov112_021FFA20[];
extern PokewalkerIrWork ov112_021FFAA4;
extern OSMessage ov112_021FFAC0[];
extern u8 *ov112_021FFAF0[];
extern u8 *ov112_021FFAF8[];
extern OSMutex ov112_021FFB08;
extern OSMessageQueue ov112_021FFB20;
extern PokewalkerIrTransfer ov112_021FFB24;
extern u8 ov112_021FFB64[];
extern OSThread ov112_021FFBD8;

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H
