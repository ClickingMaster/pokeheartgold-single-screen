#ifndef POKEHEARTGOLD_SLOP_R1_MODULE_05_PRIVATE_H
#define POKEHEARTGOLD_SLOP_R1_MODULE_05_PRIVATE_H

#include <nitro/gx/gx.h>

#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite.h"
#include "text.h"

typedef struct SlopR1Module05Obj {
    Sprite *sprite;
    SpriteResource *res[4];
} SlopR1Module05Obj;

typedef struct SlopR1Module05Work {
    BgConfig *bgConfig;
    PaletteData *paletteData;
    PokepicManager *pokepicManager;
    Pokemon *mon;
    BOOL natDexEnabled;
    enum HeapID heapId;
    void *task;
    void *gfxNarc;
    Pokepic *pokepic;
    Window windows[9];
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SlopR1Module05Obj objs[4];
    u16 unk_240;
    u16 palFlashTimer;
    u32 species;
    u32 form;
    u32 type1;
    u32 type2;
    u32 unk_254;
} SlopR1Module05Work;

extern const WindowTemplate ov18_021FBDB4[9];

String *ov18_021E590C(int species, int language, enum HeapID heapId);
String *ov18_021E595C(int species, int language, enum HeapID heapId);
String *ov18_021E59A8(int species, int language, int a2, enum HeapID heapId);
void ov18_021F8C0C(SlopR1Module05Work *work);
void ov18_021F8CCC(SlopR1Module05Work *work);
void ov18_021F94BC(SlopR1Module05Work *work);
void ov18_021F9518(SlopR1Module05Work *work);
void ov18_021F95CC(SlopR1Module05Work *work);
void ov18_021F9310(SpriteResource **res, GF_2DGfxResMan **resMan, SpriteResourcesHeader *header, int priority);
void ov18_021F95FC(Window *window, String *string, int x, int y, u32 fontId, u32 color, int align);
void ov18_021F9648(Window *window, MsgData *msgData, int msgId, int x, int y, u32 fontId, u32 color, int align);
void *ov18_021F9694(u32 species, enum HeapID heapId);

#endif
