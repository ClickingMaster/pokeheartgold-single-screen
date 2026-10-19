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
#include "sprite_resource_header.h"
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
} BoxMonSelectorMon;

typedef struct BoxMonSelectorIcon {
    Sprite *sprite;
    BoxMonSelectorMon mon;
} BoxMonSelectorIcon;

// Icon graphics read from the icon NARC, copied to OBJ VRAM by the V-Blank task.
typedef struct BoxMonSelectorIconChars {
    u32 vramOffset;
    u32 palette;
    Sprite *sprite; // NULL: nothing to transfer
    u8 chars[0x200];
} BoxMonSelectorIconChars;

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
    BgConfig *bgConfig;
    enum HeapID heapId;
    YesNoPrompt *yesNoPrompt;
    u8 unused_0C;
    u8 maxPicks;
    u8 numPicked;
    u8 page;
    u8 numPages;
    u8 unused_11;
    u8 unused_12;
    u8 canCancel;
    u32 state;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resManagers[6];
    SpriteResource *spriteResources[4]; // char, palette, cell, anim
    u8 unused_16C[8];
    SpriteResourcesHeader spriteHeader;
    BoxMonSelectorIcon boxIcons[BOX_MON_SELECTOR_BOX_SLOTS + 1]; // last: the icon being dragged
    TouchscreenHitbox *hitboxes;
    BoxMonSelectorIcon pickIcons[BOX_MON_SELECTOR_MAX_PICKS];
    BoxMonSelectorPick picks[BOX_MON_SELECTOR_MAX_PICKS];
    Sprite *cancelButton;
    Sprite *nextPageArrow;
    Sprite *prevPageArrow;
    Sprite *handCursor;
    Sprite *releaseEffect;
    Sprite *emptyPickSlots[BOX_MON_SELECTOR_MAX_PICKS - 1];
    BoxMonSelectorMark *marks;
    BoxMonSelectorIconChars iconChars[BOX_MON_SELECTOR_BOX_SLOTS];
    void *storage;
    Party *party;
    BoxMonSelectorCallback vblankTask;
    BoxMonSelectorGetMon getMon;
    BoxMonSelectorGetPageName getPageName;
    Window pageNameWindow;
    Window messageWindow;
    Window cancelWindow;
    BOOL dragging;
    int dragIndex;
    VecFx32 dragOrigin;
    u32 touchX;
    u32 touchY;
    int dragSource; // 1: box, 2: picks
    BoxMonSelectorIcon *dragIcons;
    BoxMonSelectorDropArea *pickDropAreas;
    BoxMonSelectorDropArea boxDropArea;
    int lastPick;
    BoxMonSelectorResult *results;
    GridCursor *cursor;
    BOOL keyDragging;
    BOOL touchHeld;
    BOOL touchStarted;
    void *screenFiles[2]; // party, box
    NNSG2dScreenData *screens[2];
    BoxMonSelectorCursorCallback onCursorMove;
    void *onCursorMoveArg;
    int lastCursorSlot;
    int lastCursorPage;
} BoxMonSelector;

BoxMonSelector *BoxMonSelector_New(enum HeapID heapId, int maxPicks, int numPages);
void BoxMonSelector_Delete(BoxMonSelector *selector);
void BoxMonSelector_Init(BoxMonSelector *selector, BgConfig *bgConfig, void *storage, Party *party, int unused11, int unused12, BOOL canCancel, int initialPage, BoxMonSelectorGetMon getMon, BoxMonSelectorGetPageName getPageName, BoxMonSelectorResult *results, BoxMonSelectorCursorCallback onCursorMove, void *onCursorMoveArg);
void BoxMonSelector_VBlank(BoxMonSelector *selector);
int BoxMonSelector_Run(BoxMonSelector *selector);
void BoxMonSelector_RenderSprites(BoxMonSelector *selector);

#endif // POKEHEARTGOLD_BOX_MON_SELECTOR_H
