#ifndef POKEHEARTGOLD_PALETTE_BLINK_H
#define POKEHEARTGOLD_PALETTE_BLINK_H

#include "palette.h"

typedef struct PaletteBlink PaletteBlink;

PaletteBlink *PaletteBlink_New(PaletteData *plttData, PMLCDTarget screen, u32 palette, enum HeapID heapID);
void PaletteBlink_SetState(void *blink, s32 command);
void PaletteBlink_Delete(PaletteBlink *blink);

#endif // POKEHEARTGOLD_PALETTE_BLINK_H
