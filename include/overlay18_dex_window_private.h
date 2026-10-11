#include "global.h"

#include "bg_window.h"
#include "obj_char_transfer.h"

typedef struct {
    BgConfig *bgConfig;
    u8 pad04[0x10];
    enum HeapID heapId;
    u8 pad18[0x0c];
    Window windows[9];
    u8 padB4[0x12c];
    void *gfxManagers[4];
    u8 pad1F0[0x52];
    u16 paletteTimer;
} Ov18Work;

extern const ObjCharTransferTemplate sEntryPopupObjCharTransferTemplate;
void *Create2DGfxResObjMan(int num, int type, enum HeapID heapID);
void PokedexEntryPopup_FlashBanner(Ov18Work *work);
void PokedexEntryPopup_RemoveWindows(Ov18Work *work);
void PokedexEntryPopup_InitObjCharTransfer(Ov18Work *work);
void PokedexEntryPopup_DestroyObjCharTransfer(void);
void PokedexEntryPopup_CreateResourceManagers(Ov18Work *work);
