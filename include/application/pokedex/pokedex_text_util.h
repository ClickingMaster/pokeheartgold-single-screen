#ifndef POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H
#define POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H

#include "bg_window.h"
#include "filesystem.h"
#include "msgdata.h"
#include "pm_string.h"

// Species name / category / flavor text strings.
String *PokedexApp_GetSpeciesNameString(u32 species, int language, enum HeapID heapId);
String *PokedexApp_GetCategoryString(u32 species, int language, enum HeapID heapId);
String *PokedexApp_GetFlavorTextString(u32 species, int language, int a2, enum HeapID heapId);

// footprint graphics
NarcId Pokefoot_GetNarcId(void);
int Pokefoot_GetCharFileId(int species);
int Pokefoot_GetPlttFileId(void);

// align: 0 left, 1 right (x is the right edge), 2 centred on x
void PokedexApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void PokedexApp_PrintMessage(Window *window, MsgData *msgData, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
u32 PokedexApp_GetTypeIconCharFile(u32 type);
u8 PokedexApp_GetTypeIconPalette(u32 type);
void *PokedexApp_LoadFootprintGraphics(u32 species, enum HeapID heapId);

#endif // POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_TEXT_UTIL_H
