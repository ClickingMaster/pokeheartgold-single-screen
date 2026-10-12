#ifndef POKEHEARTGOLD_CHAR_TILES_H
#define POKEHEARTGOLD_CHAR_TILES_H

#include "filesystem.h"

typedef struct TileRect {
    int x;
    int y;
    int w;
    int h;
} TileRect;

void CharTiles_LoadRectFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, int x, int y, int width, int height, void *dest);
void *CharTiles_AllocAndLoadRectFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, int x, int y, int width, int height);
void CharTiles_LoadRectAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, TileRect *a3, void *dest);
void CharTiles_LoadPicAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, void *dest);
void *CharTiles_AllocAndLoadPicAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID);
void *AllocAndLoadPltt16FromNarc(NarcId narcId, s32 fileId, enum HeapID heapID);
void CharTiles_LoadMonObjBlocksFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, int x, int y, int width, int height, void *dest, u32 pid, BOOL isAnimated, int whichFacing, int species);
void CharTiles_LoadMonRectAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, TileRect *a3, void *dest, u32 personality, BOOL isAnimated, int whichFacing, int species);
void CharTiles_LoadMonPicAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, void *dest, u32 personality, BOOL isAnimated, int whichFacing, int species);
void *CharTiles_AllocAndLoadMonPicAsObjFromNarc(NarcId narcId, s32 fileId, enum HeapID heapID, u32 personality, BOOL isAnimated, int whichFacing, int species);
void CharTiles_CopyRectFromBuffer(const void *texSrc, int texDim, int x, int y, int w, int h, void *dest);

#endif // POKEHEARTGOLD_CHAR_TILES_H
