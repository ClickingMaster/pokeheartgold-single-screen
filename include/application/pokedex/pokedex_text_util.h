#ifndef POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H
#define POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H

#include "bg_window.h"
#include "filesystem.h"
#include "msgdata.h"
#include "pm_string.h"

// Species name / category / flavor text strings, src/application/pokedex/ov18_021E590C.c. Its definitions take
// a u16 species, but every caller was compiled against a 32-bit species (no narrowing at the call), so they are
// declared that way here; the definition file has its own prototypes.
String *ov18_021E590C(u32 species, int language, enum HeapID heapId);
String *ov18_021E595C(u32 species, int language, enum HeapID heapId);
String *ov18_021E59A8(u32 species, int language, int a2, enum HeapID heapId);

// footprint graphics, src/poketool/pokefoot.c (which has no header)
NarcId ov18_021E5900(void);
int ov18_021E5904(int species);
int ov18_021E5908(void);

// align: 0 left, 1 right (x is the right edge), 2 centred on x
void PokedexApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void PokedexApp_PrintMessage(Window *window, MsgData *msgData, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
u32 PokedexApp_GetTypeIconCharFile(u32 type);
u8 PokedexApp_GetTypeIconPalette(u32 type);
void *PokedexApp_LoadFootprintGraphics(u32 species, enum HeapID heapId);

#endif // POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H
