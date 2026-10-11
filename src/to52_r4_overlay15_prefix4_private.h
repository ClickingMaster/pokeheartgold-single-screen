#ifndef W13_R4_PRIVATE_H
#define W13_R4_PRIVATE_H
#include "global.h"
typedef struct PocketEntry { void *items; u16 pos; s16 scroll; u8 pocket; u8 count; u8 pad[2]; } PocketEntry;
typedef struct PocketState { u32 unk0; PocketEntry entry[8]; u8 selected; u8 pad65[7]; void *cursor; } PocketState;
void BagApp_LoadCursorPositions(void *);
void BagApp_SaveCursorPositions(void *);
void BagCursor_Field_PocketGetPosition(void *, u32, u8 *, u8 *);
u32 BagCursor_Field_GetPocket(void *);
void BagCursor_Field_PocketSetPosition(void *, u32, u32, u32);
void BagCursor_Field_SetPocket(void *, u32);
#endif
