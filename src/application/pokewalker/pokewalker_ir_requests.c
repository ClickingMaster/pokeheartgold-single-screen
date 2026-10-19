// Pokewalker IR transfer work area and the requests that upload data to the Pokewalker (commands 0x32, 0x40
// and 0x52).

#include "global.h"

#include "application/pokewalker/pokewalker_ir.h"

typedef struct PokewalkerIrWorkHeader {
    u16 unk_00;
    u8 unk_02[6];
    u8 *walkerMon;
    u8 unk_0C[4];
    u8 *uploadImage;
    u32 error;
    u32 unk_18;
} PokewalkerIrWorkHeader;

typedef struct PokewalkerIrWorkBuffers {
    OSMessage messages[1];
    PokewalkerIrIdentity *identity;
    PokewalkerIrIdentity *peerIdentity;
    void *unk_28;
    u8 *strollData;
    u8 *records;
    u8 *blocks;
    u8 *status;
    u64 transferStartTick;
    u8 *compressedSizes;
    u8 *compressBuffer;
    u8 *compressedImageSizes;
    u8 *compressedImage;
    u8 *compressedStrollSizes;
    u8 *compressedStroll;
    u8 *compressedRecordSizes;
    u8 *compressedRecords;
} PokewalkerIrWorkBuffers;

typedef struct PokewalkerIrWorkSync {
    OSMutex mutex;
    OSMessageQueue queue;
    u8 command;
    u8 step;
    u8 unk_1E[2];
    u8 *data;
    u8 unk_24[0x12];
    u16 chunk;
} PokewalkerIrWorkSync;

typedef struct PokewalkerIrWorkProgress {
    u16 totalChunks;
    u16 totalChunk;
    u16 totalSize;
    u16 unk_3E;
    u8 buffer[0x74];
} PokewalkerIrWorkProgress;

PokewalkerIrWorkHeader gPokewalkerIrWork;
PokewalkerIrWorkBuffers gPokewalkerIrBuffers;
PokewalkerIrWorkSync gPokewalkerIrSync;
PokewalkerIrWorkProgress gPokewalkerIrProgress;

void PokewalkerIr_RequestUpload(u8 *uploadImage, u8 *strollData, u8 *records) {
    gPokewalkerIrWork.uploadImage = uploadImage;
    gPokewalkerIrBuffers.strollData = strollData;
    gPokewalkerIrBuffers.records = records;
    gPokewalkerIrSync.command = 0x32;
    gPokewalkerIrSync.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    gPokewalkerIrProgress.totalSize = 0xB732;
    gPokewalkerIrProgress.totalChunk = 0;
    gPokewalkerIrProgress.totalChunks = (gPokewalkerIrProgress.totalSize + 0x7F) / 0x80;
    if (gPokewalkerIrBuffers.compressBuffer != NULL) {
        gPokewalkerIrBuffers.compressedImageSizes = gPokewalkerIrBuffers.compressedSizes;
        gPokewalkerIrBuffers.compressedImage = gPokewalkerIrBuffers.compressBuffer;
        PokewalkerIr_CompressBlocks(gPokewalkerIrWork.uploadImage, gPokewalkerIrBuffers.compressedImage, 0x8C50, gPokewalkerIrBuffers.compressedImageSizes);
        gPokewalkerIrBuffers.compressedStrollSizes = gPokewalkerIrBuffers.compressedSizes + 0x119;
        gPokewalkerIrBuffers.compressedStroll = gPokewalkerIrBuffers.compressBuffer + 0x8C80;
        PokewalkerIr_CompressBlocks(gPokewalkerIrBuffers.strollData, gPokewalkerIrBuffers.compressedStroll, 0x28BE, gPokewalkerIrBuffers.compressedStrollSizes);
        gPokewalkerIrBuffers.compressedRecordSizes = gPokewalkerIrBuffers.compressedStrollSizes + 0x52;
        gPokewalkerIrBuffers.compressedRecords = gPokewalkerIrBuffers.compressedStroll + 0x2900;
        PokewalkerIr_CompressBlocks(gPokewalkerIrBuffers.records, gPokewalkerIrBuffers.compressedRecords, 0x224, gPokewalkerIrBuffers.compressedRecordSizes);
    }
}

void PokewalkerIr_RequestReceive(u8 *walkerMon, u8 *status, u8 *blocks, u8 *records) {
    gPokewalkerIrWork.walkerMon = walkerMon;
    gPokewalkerIrBuffers.status = status;
    gPokewalkerIrBuffers.blocks = blocks;
    gPokewalkerIrBuffers.records = records;
    gPokewalkerIrSync.command = 0x40;
    gPokewalkerIrSync.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    gPokewalkerIrProgress.totalSize = 0x2BCA;
    gPokewalkerIrProgress.totalChunk = 0;
    gPokewalkerIrProgress.totalChunks = (gPokewalkerIrProgress.totalSize + 0x7F) / 0x80;
}

void PokewalkerIr_RequestSendStroll(u8 *strollData, u8 *records, u8 *status, u8 *blocks) {
    gPokewalkerIrBuffers.strollData = strollData;
    gPokewalkerIrBuffers.records = records;
    gPokewalkerIrBuffers.status = status;
    gPokewalkerIrBuffers.blocks = blocks;
    gPokewalkerIrSync.command = 0x52;
    gPokewalkerIrSync.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    gPokewalkerIrProgress.totalSize = 0x382E;
    gPokewalkerIrProgress.totalChunk = 0;
    gPokewalkerIrProgress.totalChunks = (gPokewalkerIrProgress.totalSize + 0x7F) / 0x80;
    if (gPokewalkerIrBuffers.compressBuffer != NULL) {
        gPokewalkerIrBuffers.compressedStrollSizes = gPokewalkerIrBuffers.compressedSizes + 0x119;
        gPokewalkerIrBuffers.compressedStroll = gPokewalkerIrBuffers.compressBuffer + 0x8C80;
        PokewalkerIr_CompressBlocks(gPokewalkerIrBuffers.strollData, gPokewalkerIrBuffers.compressedStroll, 0x28BE, gPokewalkerIrBuffers.compressedStrollSizes);
        gPokewalkerIrBuffers.compressedRecordSizes = gPokewalkerIrBuffers.compressedStrollSizes + 0x52;
        gPokewalkerIrBuffers.compressedRecords = gPokewalkerIrBuffers.compressedStroll + 0x2900;
        PokewalkerIr_CompressBlocks(gPokewalkerIrBuffers.records, gPokewalkerIrBuffers.compressedRecords, 0x224, gPokewalkerIrBuffers.compressedRecordSizes);
    }
}
