#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H

#include "application/pokewalker/pokewalker_ir.h"

// IR link layer state.
typedef struct PokewalkerIrLink {
    u8 unk_00;
    u8 unk_01[3];
    u32 sessionId;
    u32 connected;
    void (*onConnect)(int result, int arg);
    u32 numDisconnects;
    void (*onClose)(int result);
    u32 closing;
    void (*onReceive)(void *data, u8 size, u8 command, u8 arg);
    u32 ownId;
    u32 handshakeStarted;
    u32 handshakeState;
    u32 active;
    u64 lastReceiveTick;
} PokewalkerIrLink;

// Header of an IR packet; the payload follows (gPokewalkerIrPayload).
typedef struct PokewalkerIrPacket {
    u8 command;
    u8 arg;
    u8 checksumLo;
    u8 checksumHi;
    u32 sessionId;
} PokewalkerIrPacket;

// IR transfer work.
typedef struct PokewalkerIrWork {
    u16 unk_00;
    u16 unk_02;
    u32 blockIndex;
    void *unk_08;
    void *unk_0C;
    void *uploadImage;
    int error;
    u32 unk_18;
    u32 unk_1C;
    PokewalkerIrIdentity *identity;
    PokewalkerIrIdentity *peerIdentity;
    void *unk_28;
    void *strollData;
    u8 *records;
    PokewalkerIrBlocks *blocks;
    PokewalkerIrStatus *status;
    u64 transferStartTick;
    u8 *compressedSizes;
    u8 *compressBuffer;
    u8 *sizes0;    // compressed block sizes of the first upload
    void *upload0; // its compressed data
    u8 *sizes1;
    void *upload1;
} PokewalkerIrWork;

typedef struct PokewalkerIrTransfer {
    u8 unk_00[0x1C];
    u8 command;
    u8 step;
    u8 unk_1E[2];
    u8 *data; // source data (send) or Pokewalker address (receive)
    u8 *dataStart;
    u32 address; // Pokewalker address (send) or destination (receive)
    u32 addressStart;
    u16 remaining;
    u16 size;
    u16 numChunks;
    u16 chunk;
    u16 totalChunks;
    u16 totalChunk;
    volatile u16 totalSize;
} PokewalkerIrTransfer;

extern u8 _021FF500[];
extern PokewalkerIrLink gPokewalkerIrLink;
extern PokewalkerIrPacket gPokewalkerIrPacket;
extern u8 gPokewalkerIrPayload[];
extern PokewalkerIrWork ov112_021FFAA4;
extern OSMessage ov112_021FFAC0[];
extern u8 *ov112_021FFAF0[];
extern u8 *ov112_021FFAF8[];
extern OSMutex ov112_021FFB08;
extern OSMessageQueue ov112_021FFB20;
extern PokewalkerIrTransfer ov112_021FFB24;
extern u8 ov112_021FFB64[];
extern OSThread sPokewalkerIrThread;

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_IR_INTERNAL_H
