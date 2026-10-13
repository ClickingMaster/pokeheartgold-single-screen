// Pokewalker IR request 0x60 and the link status the app polls (connection, transfer progress, errors).

#include "global.h"

#include "application/pokewalker/pokewalker_ir.h"
#include "application/pokewalker/pokewalker_ir_internal.h"

void PokewalkerIr_RequestReceiveGifts(void *a, void *b) {
    ov112_021FFAA4.status = a;
    ov112_021FFAA4.records = b;
    ov112_021FFB24.command = 0x60;
    ov112_021FFB24.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    ov112_021FFB24.totalSize = 0x24D8;
    ov112_021FFB24.totalChunk = 0;
    ov112_021FFB24.totalChunks = (ov112_021FFB24.totalSize + 0x7F) / 0x80;
}

void *ov112_021E7668(void) {
    return (void *)PokewalkerIr_IsConnected();
}

// Progress of the current block, 0-255.
u8 PokewalkerIr_GetChunkProgress(void) {
    return 255 * ov112_021FFB24.chunk / ov112_021FFB24.numChunks;
}

// Progress of the whole request, 0-255.
u8 PokewalkerIr_GetTotalProgress(void) {
    return 255 * ov112_021FFB24.totalChunk / ov112_021FFB24.totalChunks;
}

// Returns the error of the last request and resets it (to 1 = not connected when the link is down).
u32 PokewalkerIr_GetAndClearError(void) {
    u32 error = ov112_021FFAA4.error;
    if (PokewalkerIr_IsConnected() != 0) {
        ov112_021FFAA4.error = 0;
    } else {
        ov112_021FFAA4.error = 1;
    }
    return error;
}
