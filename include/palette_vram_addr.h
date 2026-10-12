#ifndef POKEHEARTGOLD_PALETTE_VRAM_ADDR_H
#define POKEHEARTGOLD_PALETTE_VRAM_ADDR_H

#include <nitro.h>

int GetMainBgPlttSize(void);
int GetSubBgPlttSize(void);
u16 *GetMainBgPlttAddr(void);
u16 *GetSubBgPlttAddr(void);
u16 *GetMainObjPlttAddr(void);
u16 *GetSubObjPlttAddr(void);

#endif // POKEHEARTGOLD_PALETTE_VRAM_ADDR_H
