// Pokewalker IR transfer work area and the requests that upload data to the Pokewalker (commands 0x32, 0x40
// and 0x52). The work area is one .bss block 0x021FFAA4..0x021FFBD8; other TUs reach parts of it through the
// alias symbols this object exports (see pokewalker_ir_requests.alias-map.json).

#include "global.h"

#include "application/pokewalker/pokewalker_ir.h"

typedef struct PokewalkerIrWorkHeader {
    u16 unk_00;
    u8 unk_02[6];
    u8 *unk_08;
    u8 unk_0C[4];
    u8 *uploadImage;
    u32 error;
    u32 unk_18;
} PokewalkerIrWorkHeader; // size=0x1C

typedef struct PokewalkerIrWorkBuffers {
    OSMessage messages[1]; // 0x021FFAC0
    void *identity;
    void *peerIdentity;
    void *unk_28;
    u8 *strollData;
    u8 *records;
    u8 *blocks;
    u8 *status;
    u64 transferStartTick;
    u8 *compressedSizes; // 0x44 (offsets from 0x021FFAA4)
    u8 *compressBuffer;
    u8 *sizes0;            // 0x4C, compressed block sizes of the first upload
    u8 *upload0;           // 0x50, its compressed data
    u8 *sizes1;            // 0x54
    u8 *upload1;           // 0x58
    u8 *sizes2;            // 0x5C
    u8 *upload2;           // 0x60
} PokewalkerIrWorkBuffers; // size=0x48

typedef struct PokewalkerIrWorkSync {
    OSMutex mutex;        // 0x021FFB08
    OSMessageQueue queue; // 0x021FFB20
    u8 command;           // 0x021FFB40
    u8 step;
    u8 unk_1E[2];
    u8 *data;
    u8 unk_24[0x12];
    u16 chunk;
} PokewalkerIrWorkSync; // size=0x54

typedef struct PokewalkerIrWorkProgress {
    u16 totalChunks; // 0x021FFB5C
    u16 totalChunk;
    u16 totalSize;
    u16 unk_3E;
    u8 buffer[0x74];        // 0x021FFB64
} PokewalkerIrWorkProgress; // size=0x7C

#pragma force_active on

PokewalkerIrWorkHeader ov112_021FFAA4;
PokewalkerIrWorkBuffers ov112_021FFAC0;
PokewalkerIrWorkSync ov112_021FFB08;
PokewalkerIrWorkProgress ov112_021FFB5C;

void PokewalkerIr_RequestUpload(u8 *a, u8 *b, u8 *c) {
    ov112_021FFAA4.uploadImage = a;
    ov112_021FFAC0.strollData = b;
    ov112_021FFAC0.records = c;
    ov112_021FFB08.command = 0x32;
    ov112_021FFB08.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    ov112_021FFB5C.totalSize = 0xB732;
    ov112_021FFB5C.totalChunk = 0;
    ov112_021FFB5C.totalChunks = (ov112_021FFB5C.totalSize + 0x7F) / 0x80;
    if (ov112_021FFAC0.compressBuffer != NULL) {
        ov112_021FFAC0.sizes0 = ov112_021FFAC0.compressedSizes;
        ov112_021FFAC0.upload0 = ov112_021FFAC0.compressBuffer;
        PokewalkerIr_CompressBlocks(ov112_021FFAA4.uploadImage, ov112_021FFAC0.upload0, 0x8C50, ov112_021FFAC0.sizes0);
        ov112_021FFAC0.sizes1 = ov112_021FFAC0.compressedSizes + 0x119;
        ov112_021FFAC0.upload1 = ov112_021FFAC0.compressBuffer + 0x8C80;
        PokewalkerIr_CompressBlocks(ov112_021FFAC0.strollData, ov112_021FFAC0.upload1, 0x28BE, ov112_021FFAC0.sizes1);
        ov112_021FFAC0.sizes2 = ov112_021FFAC0.sizes1 + 0x52;
        ov112_021FFAC0.upload2 = ov112_021FFAC0.upload1 + 0x2900;
        PokewalkerIr_CompressBlocks(ov112_021FFAC0.records, ov112_021FFAC0.upload2, 0x224, ov112_021FFAC0.sizes2);
    }
}

void PokewalkerIr_RequestReceive(u8 *a, u8 *b, u8 *c, u8 *d) {
    ov112_021FFAA4.unk_08 = a;
    ov112_021FFAC0.status = b;
    ov112_021FFAC0.blocks = c;
    ov112_021FFAC0.records = d;
    ov112_021FFB08.command = 0x40;
    ov112_021FFB08.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    ov112_021FFB5C.totalSize = 0x2BCA;
    ov112_021FFB5C.totalChunk = 0;
    ov112_021FFB5C.totalChunks = (ov112_021FFB5C.totalSize + 0x7F) / 0x80;
}

void PokewalkerIr_RequestSendStroll(u8 *a, u8 *b, u8 *c, u8 *d) {
    ov112_021FFAC0.strollData = a;
    ov112_021FFAC0.records = b;
    ov112_021FFAC0.status = c;
    ov112_021FFAC0.blocks = d;
    ov112_021FFB08.command = 0x52;
    ov112_021FFB08.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    ov112_021FFB5C.totalSize = 0x382E;
    ov112_021FFB5C.totalChunk = 0;
    ov112_021FFB5C.totalChunks = (ov112_021FFB5C.totalSize + 0x7F) / 0x80;
    if (ov112_021FFAC0.compressBuffer != NULL) {
        ov112_021FFAC0.sizes1 = ov112_021FFAC0.compressedSizes + 0x119;
        ov112_021FFAC0.upload1 = ov112_021FFAC0.compressBuffer + 0x8C80;
        PokewalkerIr_CompressBlocks(ov112_021FFAC0.strollData, ov112_021FFAC0.upload1, 0x28BE, ov112_021FFAC0.sizes1);
        ov112_021FFAC0.sizes2 = ov112_021FFAC0.sizes1 + 0x52;
        ov112_021FFAC0.upload2 = ov112_021FFAC0.upload1 + 0x2900;
        PokewalkerIr_CompressBlocks(ov112_021FFAC0.records, ov112_021FFAC0.upload2, 0x224, ov112_021FFAC0.sizes2);
    }
}
