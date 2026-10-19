// Pokewalker IR request 0x60 and the link status the app polls (connection, transfer progress, errors).

#include "global.h"

#include "application/pokewalker/pokewalker_ir.h"
#include "application/pokewalker/pokewalker_ir_internal.h"

void PokewalkerIr_RequestReceiveGifts(PokewalkerIrStatus *status, void *records) {
    gPokewalkerIrWork.status = status;
    gPokewalkerIrWork.records = records;
    gPokewalkerIrTransfer.command = 0x60;
    gPokewalkerIrTransfer.step = 0x20;
    PokewalkerIr_BeginTransfer(NULL, 0, 0xD4C);
    gPokewalkerIrTransfer.totalSize = 0x24D8;
    gPokewalkerIrTransfer.totalChunk = 0;
    gPokewalkerIrTransfer.totalChunks = (gPokewalkerIrTransfer.totalSize + 0x7F) / 0x80;
}

void *PokewalkerIrStatus_IsConnected(void) {
    return (void *)PokewalkerIr_IsConnected();
}

// Progress of the current block, 0-255.
u8 PokewalkerIr_GetChunkProgress(void) {
    return 255 * gPokewalkerIrTransfer.chunk / gPokewalkerIrTransfer.numChunks;
}

// Progress of the whole request, 0-255.
u8 PokewalkerIr_GetTotalProgress(void) {
    return 255 * gPokewalkerIrTransfer.totalChunk / gPokewalkerIrTransfer.totalChunks;
}

// Returns the error of the last request and resets it (to 1 = not connected when the link is down).
u32 PokewalkerIr_GetAndClearError(void) {
    u32 error = gPokewalkerIrWork.error;
    if (PokewalkerIr_IsConnected() != 0) {
        gPokewalkerIrWork.error = 0;
    } else {
        gPokewalkerIrWork.error = 1;
    }
    return error;
}
