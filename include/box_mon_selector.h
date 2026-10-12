#ifndef POKEHEARTGOLD_BOX_MON_SELECTOR_H
#define POKEHEARTGOLD_BOX_MON_SELECTOR_H

#include "global.h"

#include "bg_window.h"
#include "grid_cursor.h"
#include "heap.h"
#include "party.h"
#include "pm_string.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

// Picks up to 3 Pokemon from the PC boxes and the party (Pokewalker, Pokeathlon box). The caller reads the boxes
// through the getMon / getPageName callbacks; page BOX_MON_SELECTOR_PARTY_PAGE is the party.

#define BOX_MON_SELECTOR_PARTY_PAGE 18
#define BOX_MON_SELECTOR_MAX_PICKS  3
#define BOX_MON_SELECTOR_BOX_SLOTS  30

// A Pokemon as the getMon callback describes it; the caller's own data follows the common part.
typedef struct BoxMonSelectorMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 heldItem;
    u16 unk_0E;
    u8 callerData[0x20];
} BoxMonSelectorMon; // size: 0x30

typedef struct BoxMonSelectorIcon {
    Sprite *sprite;
    BoxMonSelectorMon mon;
} BoxMonSelectorIcon; // size: 0x34

// Icon graphics read from the icon NARC, copied to OBJ VRAM by the V-Blank task.
typedef struct BoxMonSelectorIconChars {
    u32 vramOffset;
    u32 palette;
    Sprite *sprite; // NULL: nothing to transfer
    u8 chars[0x200];
} BoxMonSelectorIconChars; // size: 0x20C

// A picked Pokemon's mark on its box icon.
typedef struct BoxMonSelectorMark {
    Sprite *sprite;
    int slot; // -1: unused
    u32 page;
} BoxMonSelectorMark;

typedef struct BoxMonSelectorPick {
    u32 page;
    u32 slot;
    u32 markIndex;
    u32 active;
} BoxMonSelectorPick;

typedef struct BoxMonSelectorDropArea {
    u8 top;
    u8 left;
    u8 bottom;
    u8 right;
    u32 occupied;
} BoxMonSelectorDropArea;

// One element of the caller's result array.
typedef struct BoxMonSelectorResult {
    u32 page;
    u32 slot;
} BoxMonSelectorResult;

typedef BOOL (*BoxMonSelectorGetMon)(void *storage, u32 page, int slot, BoxMonSelectorMon *mon);
typedef void (*BoxMonSelectorGetPageName)(String *dest, void *storage, int page);
typedef void (*BoxMonSelectorCursorCallback)(int page, int slot, void *arg); // -1, -1: no Pokemon
typedef void (*BoxMonSelectorCallback)(BoxMonSelectorIconChars *);

typedef struct BoxMonSelector {
    BgConfig *bgConfig;                                         // 0x0000
    enum HeapID heapId;                                         // 0x0004
    YesNoPrompt *yesNoPrompt;                                   // 0x0008
    u8 unused_0C;                                               // 0x000C
    u8 maxPicks;                                                // 0x000D
    u8 numPicked;                                               // 0x000E
    u8 page;                                                    // 0x000F
    u8 numPages;                                                // 0x0010
    u8 unused_11;                                               // 0x0011
    u8 unused_12;                                               // 0x0012
    u8 canCancel;                                               // 0x0013
    u32 state;                                                  // 0x0014
    SpriteList *spriteList;                                     // 0x0018
    G2dRenderer renderer;                                       // 0x001C
    GF_2DGfxResMan *resManagers[6];                             // 0x0144
    SpriteResource *spriteResources[4];                         // 0x015C char, palette, cell, anim
    u8 unused_16C[8];                                           // 0x016C
    SpriteResourcesHeader spriteHeader;                         // 0x0174
    BoxMonSelectorIcon boxIcons[BOX_MON_SELECTOR_BOX_SLOTS + 1]; // 0x0198 last: the icon being dragged
    TouchscreenHitbox *hitboxes;                                // 0x07E4
    BoxMonSelectorIcon pickIcons[BOX_MON_SELECTOR_MAX_PICKS];   // 0x07E8
    BoxMonSelectorPick picks[BOX_MON_SELECTOR_MAX_PICKS];       // 0x0884
    Sprite *cancelButton;                                       // 0x08B4
    Sprite *nextPageArrow;                                      // 0x08B8
    Sprite *prevPageArrow;                                      // 0x08BC
    Sprite *handCursor;                                         // 0x08C0
    Sprite *releaseEffect;                                      // 0x08C4
    Sprite *emptyPickSlots[BOX_MON_SELECTOR_MAX_PICKS - 1];     // 0x08C8
    BoxMonSelectorMark *marks;                                  // 0x08D0
    BoxMonSelectorIconChars iconChars[BOX_MON_SELECTOR_BOX_SLOTS]; // 0x08D4
    void *storage;                                              // 0x463C
    Party *party;                                               // 0x4640
    BoxMonSelectorCallback vblankTask;                          // 0x4644
    BoxMonSelectorGetMon getMon;                                // 0x4648
    BoxMonSelectorGetPageName getPageName;                      // 0x464C
    Window pageNameWindow;                                      // 0x4650
    Window messageWindow;                                       // 0x4660
    Window cancelWindow;                                        // 0x4670
    BOOL dragging;                                              // 0x4680
    int dragIndex;                                              // 0x4684
    VecFx32 dragOrigin;                                         // 0x4688
    u32 touchX;                                                 // 0x4694
    u32 touchY;                                                 // 0x4698
    int dragSource;                                             // 0x469C 1: box, 2: picks
    BoxMonSelectorIcon *dragIcons;                              // 0x46A0
    BoxMonSelectorDropArea *pickDropAreas;                      // 0x46A4
    BoxMonSelectorDropArea boxDropArea;                         // 0x46A8
    int lastPick;                                               // 0x46B0
    BoxMonSelectorResult *results;                              // 0x46B4
    GridCursor *cursor;                                         // 0x46B8
    BOOL keyDragging;                                           // 0x46BC
    BOOL touchHeld;                                             // 0x46C0
    BOOL touchStarted;                                          // 0x46C4
    void *screenFiles[2];                                       // 0x46C8 party, box
    NNSG2dScreenData *screens[2];                               // 0x46D0
    BoxMonSelectorCursorCallback onCursorMove;                  // 0x46D8
    void *onCursorMoveArg;                                      // 0x46DC
    int lastCursorSlot;                                         // 0x46E0
    int lastCursorPage;                                         // 0x46E4
} BoxMonSelector; // size: 0x46E8

BoxMonSelector *BoxMonSelector_New(enum HeapID heapId, int maxPicks, int numPages);
void BoxMonSelector_Delete(BoxMonSelector *selector);
void BoxMonSelector_Init(BoxMonSelector *selector, BgConfig *bgConfig, void *storage, Party *party, int unused11, int unused12, BOOL canCancel, int initialPage, BoxMonSelectorGetMon getMon, BoxMonSelectorGetPageName getPageName, BoxMonSelectorResult *results, BoxMonSelectorCursorCallback onCursorMove, void *onCursorMoveArg);
void BoxMonSelector_VBlank(BoxMonSelector *selector);
int BoxMonSelector_Run(BoxMonSelector *selector);
void BoxMonSelector_RenderSprites(BoxMonSelector *selector);

#endif // POKEHEARTGOLD_BOX_MON_SELECTOR_H
