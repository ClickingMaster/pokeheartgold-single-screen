#ifndef TO45_OVERLAY18_SCROLL_MATH_PRIVATE_H
#define TO45_OVERLAY18_SCROLL_MATH_PRIVATE_H

#include "global.h"

#include "sprite_system.h"

typedef struct PokedexAppData PokedexAppData;

u32 PokedexApp_GetListScrollbarLength(PokedexAppData *pokedexApp, int index);
int PokedexApp_GetListScrollbarMinY(PokedexAppData *pokedexApp, int index);
int PokedexApp_GetListScrollbarMaxY(PokedexAppData *pokedexApp, int index);
u32 PokedexApp_GetLastListPage(PokedexAppData *pokedexApp, int mode);
BOOL PokedexApp_IsTouchOnListScrollbar(PokedexAppData *pokedexApp, int index, u32 x, u32 y);
BOOL PokedexApp_DragListScrollbar(PokedexAppData *pokedexApp, int index, u32 unused, u32 y, int mode);
int PokedexApp_GetListScrollbarYForPage(PokedexAppData *pokedexApp, int page, int index, int mode);
u32 ov18_021F2E14(PokedexAppData *pokedexApp, int index, u32 position, u32 divisor);
void PokedexApp_MoveListScrollbar(PokedexAppData *pokedexApp, int index, int delta);
void PokedexApp_SyncListScrollbarToPage(PokedexAppData *pokedexApp, int index, int mode);

#define POKEDEX_SPRITE(app, index) (((ManagedSprite **)((u8 *)(app) + 0x670))[index])
#define POKEDEX_PAGE(app)          (*(u8 *)((u8 *)(app) + 0x1859))

#endif
