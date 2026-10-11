#include "global.h"

#include "constants/pokemon.h"

#include "application/dress_up/dress_up_internal.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "touchscreen.h"

// Objects of the Dress-Up canvas and tray (shared with the Accessory Portrait): the mon picture with its hit box
// and margins, the node pool and lists, per-mode object helpers (slot or mon), and the BG screens (load, alternate
// screen animation, sliding, V-blank char / palette uploads).

// BG slide task
typedef struct DressUpBgSlide {
    DressUpBgScreen *screen;          // 0x00
    DressUpBgScreenTemplate template; // 0x04, loaded at the end
    BOOL *done;                       // 0x34
    int frames;                       // 0x38
    int stepX;                        // 0x3C
    int stepY;                        // 0x40
    int startX;                       // 0x44
    int startY;                       // 0x48
} DressUpBgSlide;                     // size 0x4C

typedef struct DressUpCharUpload {
    BgConfig *bgConfig;            // 0x00
    NNSG2dCharacterData *charData; // 0x04
    void *buffer;                  // 0x08
    u32 layer;                     // 0x0C
    u32 tileStart;                 // 0x10
} DressUpCharUpload;

typedef struct DressUpPlttUpload {
    NNSG2dPaletteData *plttData; // 0x00
    void *buffer;                // 0x04
    u32 location;                // 0x08, 0 main, 4 sub
    u32 offset;                  // 0x0C
    u32 size;                    // 0x10
} DressUpPlttUpload;

void DressUp_BgSlideTask(SysTask *task, DressUpBgSlide *slide);
void DressUpBgScreen_LoadCharsTask(SysTask *task, DressUpCharUpload *upload);
void DressUpBgScreen_LoadPlttTask(SysTask *task, DressUpPlttUpload *upload);

void DressUpMon_Create(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_CreateEx(monPic, pokepicManager, mon, template, heapID, 0);
}

#pragma force_active on

// the mon's Pokepic at (192, 56) with its hit box; mode 0 measures the transparent margins of the picture, else
// only the left edge (both sides) and the mon's own top / bottom value
void DressUpMon_CreateEx(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID, int mode) {
    int width, height;
    u8 margin;
    void *buffer;
    NNSG2dCharacterData *character;
    (void)GetMonData(mon, MON_DATA_SPECIES, NULL);
    sub_02070130(template, &mon->box, 2);
    monPic->pokepic = PokepicManager_CreatePokepic(pokepicManager, template, 192, 56, 0, 0, NULL, NULL);
    DressUpMon_GetSize(monPic, &width, &height);
    width /= 2;
    height /= 2;
    monPic->hitbox.rect.top = 56 - height;
    monPic->hitbox.rect.bottom = 56 + height;
    monPic->hitbox.rect.left = 192 - width;
    monPic->hitbox.rect.right = 192 + width;
    DressUpMon_MakeClampedRect(&monPic->hitbox, 192, 56, width, height);
    margin = sub_02070848(&mon->box, 2);
    buffer = GfGfxLoader_LoadFromNarc((NarcId) template->narcID, template->charDataID, FALSE, heapID, FALSE);
    NNS_G2dGetUnpackedCharacterData(buffer, &character);
    UnscanPokepic(character->pRawData, (NarcId) template->narcID);
    if (mode == 0) {
        DressUpMon_CalcPicMargins(character->pRawData, character->W * 8, character->H * 8, monPic->margins);
    } else {
        monPic->margins[0] = DressUpMon_FindPicLeftEdge(character->pRawData, character->W * 8);
        monPic->margins[1] = monPic->margins[0];
        monPic->margins[3] = margin;
        monPic->margins[2] = margin;
    }
    Heap_Free(buffer);
    monPic->mon = mon;
}

void DressUpMon_Delete(DressUpMonPic *monPic) {
    u32 i;
    u8 *bytes = (u8 *)monPic;
    Pokepic_Delete(monPic->pokepic);
    // clears the record byte by byte
    i = sizeof(DressUpMonPic);
    do {
        *bytes = 0;
        bytes++;
    } while (--i != 0);
}

#pragma force_active reset

void DressUpMon_SetPriority(DressUpMonPic *monPic, int priority) {
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_Z, priority);
}

int DressUpMon_GetPriority(DressUpMonPic *monPic) {
    return Pokepic_GetAttr(monPic->pokepic, POKEPIC_Z);
}

void DressUpMon_SetCenterPos(DressUpMonPic *monPic, int x, int y) {
    int halfWidth;
    int halfHeight;
    DressUpMon_GetSize(monPic, &halfWidth, &halfHeight);
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_X, x);
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_Y, y);
    halfHeight /= 2;
    halfWidth /= 2;
    DressUpMon_MakeClampedRect(&monPic->hitbox, x, y, halfWidth, halfHeight);
}

BOOL DressUpMon_IsTouchHeldIn(DressUpMonPic *monPic) {
    return TouchscreenHitbox_TouchHeldIsIn(&monPic->hitbox);
}

BOOL DressUpMon_IsPointIn(DressUpMonPic *monPic, u32 x, u32 y) {
    return TouchscreenHitbox_PointIsIn(&monPic->hitbox, x, y);
}

void DressUpMon_GetCenterPos(DressUpMonPic *monPic, int *x, int *y) {
    *x = Pokepic_GetAttr(monPic->pokepic, POKEPIC_X);
    *y = Pokepic_GetAttr(monPic->pokepic, POKEPIC_Y);
}

void DressUpMon_GetSize(DressUpMonPic *monPic, int *width, int *height) {
    *width = 80;
    *height = 80;
}

#pragma force_active on

// the touch is on an opaque pixel of the mon; (x, y) = touch position within the picture
BOOL DressUpMon_HitTestTouch(DressUpMonPic *monPic, int *x, int *y, NNSG2dCharacterData *hitMap) {
    int width, height, left, top;
    if (DressUpMon_IsTouchHeldIn(monPic) == FALSE) {
        return FALSE;
    }
    DressUpMon_GetCenterPos(monPic, &left, &top);
    DressUpMon_GetSize(monPic, &width, &height);
    left -= width / 2;
    top -= height / 2;
    *x = gSystem.touchX - left;
    *y = gSystem.touchY - top;
    if (DressUpGfx_CharPixelEquals(hitMap, *x, *y, 0) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL DressUpMon_HitTestPoint(DressUpMonPic *monPic, int x, int y, NNSG2dCharacterData *hitMap) {
    int width, height, left, top;
    if (!DressUpMon_IsPointIn(monPic, x, y)) {
        return FALSE;
    }
    DressUpMon_GetCenterPos(monPic, &left, &top);
    DressUpMon_GetSize(monPic, &width, &height);
    left -= width / 2;
    top -= height / 2;
    y -= top;
    x -= left;
    if (!DressUpGfx_CharPixelEquals(hitMap, x, y, 0)) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void DressUpMon_GetMargins(DressUpMonPic *monPic, u8 *margins) {
    u8 *src = (u8 *)monPic;
    u8 *dst = margins;
    dst[0] = src[8];
    dst[1] = src[9];
    dst[2] = src[10];
    dst[3] = src[11];
}

#pragma force_active on

// first column with an opaque pixel of an 80x80 4bpp picture (80 if none)
int DressUpMon_FindPicLeftEdge(const s8 *pixels, int stride) {
    int x, y;
    for (x = 0; x < 80; x++) {
        for (y = 0; y < 80; y++) {
            int index = x + y * stride;
            u8 mask = 15 << ((index % 2) * 4);
            if (pixels[index / 2] & mask) {
                return x;
            }
        }
    }
    return 80;
}

// transparent margins (left, right, top, bottom) of an 80x80 4bpp picture, at most 40
void DressUpMon_CalcPicMargins(s8 *pixels, int stride, int height, u8 *margins) {
    int x, y, row;
    margins[0] = 40;
    margins[1] = 40;
    margins[2] = 40;
    margins[3] = 40;
    for (x = 0; x < 80; x++) {
        int right;
        y = 0;
        right = 80 - x;
        row = y;
        for (; y < 80; y++) {
            int index = x + row;
            u8 mask = 15 << ((index % 2) * 4);
            if (pixels[index / 2] & mask) {
                if (margins[0] > x) {
                    margins[0] = x;
                }
                if (margins[1] > right) {
                    margins[1] = right;
                }
                if (margins[2] > y) {
                    margins[2] = y;
                }
                if (margins[3] > 80 - y) {
                    margins[3] = 80 - y;
                }
            }
            row += stride;
        }
    }
}

// hit box rect around (x, y), clamped to the screen
void DressUpMon_MakeClampedRect(TouchscreenHitbox *hitbox, int x, int y, int halfWidth, int halfHeight) {
    if (y - halfHeight >= 0) {
        hitbox->rect.top = y - halfHeight;
    } else {
        hitbox->rect.top = 0;
    }
    if (y + halfHeight <= 191) {
        hitbox->rect.bottom = y + halfHeight;
    } else {
        hitbox->rect.bottom = 191;
    }
    if (x - halfWidth >= 0) {
        hitbox->rect.left = x - halfWidth;
    } else {
        hitbox->rect.left = 0;
    }
    if (x + halfWidth <= 255) {
        hitbox->rect.right = x + halfWidth;
    } else {
        hitbox->rect.right = 255;
    }
}

#pragma force_active reset

void DressUpNodePool_Init(DressUpNodePool *pool, int count, enum HeapID heapID) {
    u32 size = count * sizeof(DressUpNode);
    pool->nodes = Heap_Alloc(heapID, size);
    if (pool->nodes == NULL) {
        GF_AssertFail();
    }
    memset(pool->nodes, 0, size);
    pool->count = count;
}

void DressUpNodePool_Free(DressUpNodePool *pool) {
    Heap_Free(pool->nodes);
    pool->nodes = NULL;
    pool->count = 0;
}

DressUpNode *DressUpNodePool_Acquire(DressUpNodePool *pool, void *object, u32 mode) {
    int i;
    if (pool->nodes == NULL) {
        GF_AssertFail();
    }
    if (pool->count == 0) {
        GF_AssertFail();
    }
    for (i = 0; i < pool->count; i++) {
        if (pool->nodes[i].object == NULL) {
            break;
        }
    }
    if (pool->count <= i) {
        GF_AssertFail();
    }
    pool->nodes[i].object = object;
    pool->nodes[i].mode = mode;
    return &pool->nodes[i];
}

void DressUpList_InitNode(DressUpNode *node) {
    memset(node, 0, sizeof(DressUpNode));
}

void DressUpList_InsertAfter(DressUpNode *node, DressUpNode *at) {
    node->next = at->next;
    at->next->prev = node;
    node->prev = at;
    at->next = node;
}

void DressUpList_Remove(DressUpNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

// unlinks and frees every node of the list
void DressUpList_Clear(DressUpNode *head) {
    DressUpNode *node = head->next;
    while (node != head) {
        DressUpNode *next = node->next;
        DressUpList_Remove(node);
        DressUpList_InitNode(node);
        node = next;
    }
}

void DressUpObj_SetPriority(DressUpNode *node, int priority) {
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_SetSlotPriority(node->object, priority);
    } else {
        DressUpMon_SetPriority(node->object, priority);
    }
}

// hit maps: [accessory], [100 + backdrop], [DRESS_UP_IMAGE_MAX] the mon
BOOL DressUpObj_HitTest(DressUpNode *node, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    if (node->mode == 0) {
        return DressUpGfx_HitTestSlotPixels(node->object, x, y, hitMaps[((DressUpSlot *)node->object)->id]);
    }
    if (node->mode == 1) {
        DressUpSlot *slot = node->object;
        u32 index = slot->id;
        index += DRESS_UP_ACCESSORY_NUM;
        return DressUpGfx_HitTestSlotPixels(slot, x, y, hitMaps[index]);
    }
    if (node->mode == 2) {
        DressUpSlot *slot = node->object;
        u32 index = slot->id;
        index += DRESS_UP_ACCESSORY_NUM;
        return DressUpGfx_HitTestSlotPixels(slot, x, y, hitMaps[index]);
    }
    return DressUpMon_HitTestTouch(node->object, x, y, hitMaps[DRESS_UP_IMAGE_MAX]);
}

// top-left position (the mon is positioned by its centre)
void DressUpObj_SetPos(DressUpNode *node, int x, int y) {
    int width;
    int height;
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_SetSlotPos(node->object, x, y);
        return;
    }
    {
        DressUpMonPic *monPic = node->object;
        DressUpMon_GetSize(monPic, &width, &height);
        width /= 2;
        height /= 2;
        DressUpMon_SetCenterPos(monPic, x + width, y + height);
    }
}

void DressUpObj_GetPos(DressUpNode *node, int *x, int *y) {
    int width;
    int height;
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_GetSlotPos(node->object, x, y);
        return;
    }
    {
        DressUpMonPic *monPic = node->object;
        DressUpMon_GetCenterPos(monPic, x, y);
        DressUpMon_GetSize(monPic, &width, &height);
        width /= 2;
        height /= 2;
        *x -= width;
        *y -= height;
    }
}

void DressUpObj_GetSize(DressUpNode *node, int *width, int *height) {
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_GetSlotSize(node->object, width, height);
    } else {
        DressUpMon_GetSize(node->object, width, height);
    }
}

void DressUpObj_GetMargins(DressUpNode *node, int *left, int *right, int *top, int *bottom) {
    u8 margins[4];
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        ov41_02246020(node->object, left, top);
        *right = *left;
        *bottom = *top;
    } else {
        DressUpMon_GetMargins(node->object, margins);
        *left = margins[0];
        *right = margins[1];
        *top = margins[2];
        *bottom = margins[3];
    }
}

void DressUpObj_MoveListBy(DressUpNode *head, int dx, int dy) {
    DressUpNode *node = head->next;
    while (node != head) {
        int x;
        int y;
        DressUpObj_GetPos(node, &x, &y);
        x += dx;
        y += dy;
        DressUpObj_SetPos(node, x, y);
        node = node->next;
    }
}

void DressUpObj_GetVisibleRect(DressUpNode *node, int *top, int *bottom, int *left, int *right) {
    int width;
    int height;
    int x;
    int y;
    int marginLeft;
    int marginTop;
    int marginRight;
    int marginBottom;
    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetMargins(node, &marginLeft, &marginRight, &marginTop, &marginBottom);
    *top = y + marginTop;
    *bottom = y + height - marginBottom;
    *left = x + marginLeft;
    *right = x + width - marginRight;
}

void DressUpBgScreen_LoadFromTemplate(DressUpBgScreen *screen, const DressUpBgScreenTemplate *template) {
    screen->bgConfig = template->bgConfig;
    screen->xTile = template->x / 8;
    screen->yTile = template->y / 8;
    screen->layer = template->layer;
    screen->plttSlot = template->plttSlot;
    screen->plttCount = template->plttCount;
    screen->charOffset = template->charOffset;
    screen->narcId = template->narcId;
    screen->screenFileId = template->screenFileId;
    DressUpBgScreen_Load(template, &screen->widthTiles, &screen->heightTiles);
}

void DressUpBgScreen_Unload(DressUpBgScreen *screen) {
    BgClearTilemapBufferAndCommit(screen->bgConfig, (u8)screen->layer);
    memset(screen, 0, sizeof(DressUpBgScreen));
}

void DressUpBgScreen_InitAltAnim(DressUpBgScreenAnim *anim, const DressUpBgScreenAnimTemplate *template) {
    anim->screen = template->screen;
    anim->altScreenFileId = template->altScreenFileId;
    anim->delay = template->delay;
    anim->ticks = 0;
    anim->showingAlt = FALSE;
    anim->heapID = template->heapID;
}

#pragma force_active on

// every `delay` ticks shows the other screen (alternate / base)
void DressUpBgScreen_UpdateAltAnim(DressUpBgScreenAnim *anim, int ticks) {
    anim->ticks += ticks;
    if (anim->ticks > anim->delay) {
        int fileId;
        NNSG2dScreenData *screenData;
        void *buffer;
        DressUpBgScreen *screen;
        anim->ticks = 0;
        if (anim->showingAlt == FALSE) {
            fileId = anim->altScreenFileId;
            anim->showingAlt = TRUE;
        } else {
            fileId = anim->screen->screenFileId;
            anim->showingAlt = FALSE;
        }
        buffer = GfGfxLoader_GetScrnData(anim->screen->narcId, fileId, FALSE, &screenData, anim->heapID);
        screen = anim->screen;
        DressUpBgScreen_CopyToTilemap(screen->bgConfig, screen->layer, screenData, screen->widthTiles, screen->heightTiles, screen->xTile, screen->yTile, screen->charOffset, screen->plttSlot);
        Heap_Free(buffer);
    }
}

#pragma force_active reset

void DressUpBgScreen_StopAltAnim(DressUpBgScreenAnim *anim) {
    NNSG2dScreenData *screenData;
    void *buffer = GfGfxLoader_GetScrnData(anim->screen->narcId, anim->screen->screenFileId, FALSE, &screenData, anim->heapID);
    DressUpBgScreen *screen = anim->screen;
    DressUpBgScreen_CopyToTilemap(screen->bgConfig, screen->layer, screenData, screen->widthTiles, screen->heightTiles, screen->xTile, screen->yTile, screen->charOffset, screen->plttSlot);
    Heap_Free(buffer);
    memset(anim, 0, sizeof(DressUpBgScreenAnim));
}

#pragma force_active on

// scrolls the screen by (dx, dy) over `frames` frames, then loads the template there (the new screen is first
// drawn at the offset position with chars at 128 and palette 5)
void DressUp_StartBgSlide(DressUpBgScreen *screen, DressUpBgScreenTemplate *template, int dx, int dy, int frames, BOOL *done) {
    DressUpBgSlide *slide = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_BgSlideTask, sizeof(DressUpBgSlide), 0, HEAP_ID_13));
    slide->screen = screen;
    slide->template = *template;
    slide->done = done;
    slide->frames = frames;
    slide->stepX = dx / frames;
    slide->stepY = dy / frames;
    slide->startX = Bg_GetXpos(screen->bgConfig, screen->layer);
    slide->startY = Bg_GetYpos(screen->bgConfig, screen->layer);
    template->charOffset = 128;
    template->plttSlot = 5;
    template->x -= dx;
    template->y -= dy;
    template->heapID = HEAP_ID_14;
    DressUpBgScreen_Load(template, NULL, NULL);
}

#pragma force_active reset

void DressUpBgScreen_AddCharOffset(NNSG2dScreenData *screenData, int charOffset) {
    int i;
    int count = screenData->szByte >> 1;
    u16 *tiles = (u16 *)screenData->rawData;
    for (i = 0; i < count; i++) {
        tiles[i] += charOffset;
    }
}

#pragma force_active on

// queues the chars and the palette for V-blank and copies the screen into the tilemap buffer
void DressUpBgScreen_Load(const DressUpBgScreenTemplate *template, int *outWidth, int *outHeight) {
    struct {
        int y;
        int x;
    } tile;

    NNSG2dScreenData *screenData;
    void *buffer;
    int width, height;
    tile.x = template->x / 8;
    tile.y = template->y / 8;
    DressUpBgScreen_QueueLoadChars(template->narcId, template->charFileId, template->bgConfig, template->layer, template->charOffset, template->heapID);
    DressUpBgScreen_QueueLoadPltt(template->narcId, template->plttFileId, template->layer < GF_BG_LYR_SUB_0 ? 0 : 4, template->plttSlot * 32, template->plttCount * 32, template->heapID);
    buffer = GfGfxLoader_GetScrnData(template->narcId, template->screenFileId, FALSE, &screenData, template->heapID);
    width = screenData->screenWidth / 8;
    height = screenData->screenHeight / 8;
    DressUpBgScreen_CopyToTilemap(template->bgConfig, template->layer, screenData, width, height, tile.x, tile.y, template->charOffset, template->plttSlot);
    Heap_Free(buffer);
    if (outWidth) {
        *outWidth = width;
    }
    if (outHeight) {
        *outHeight = height;
    }
}

#pragma force_active reset

void DressUp_BgSlideTask(SysTask *task, DressUpBgSlide *slide) {
    slide->frames--;
    if (slide->frames >= 0) {
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SUB_X, slide->stepX);
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SUB_Y, slide->stepY);
    } else {
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SET_X, slide->startX);
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SET_Y, slide->startY);
        DressUpBgScreen_LoadFromTemplate(slide->screen, &slide->template);
        if (slide->done) {
            *slide->done = TRUE;
        }
        DestroySysTaskAndEnvironment(task);
    }
}

#pragma force_active on

void DressUpBgScreen_CopyToTilemap(BgConfig *bgConfig, int layer, NNSG2dScreenData *screenData, int width, int height, int x, int y, int charOffset, int palette) {
    DressUpBgScreen_AddCharOffset(screenData, charOffset);
    CopyToBgTilemapRect(bgConfig, (u8)layer, (u8)x, (u8)y, (u8)width, (u8)height, screenData->rawData, 0, 0, (u8)width, (u8)height);
    BgTilemapRectChangePalette(bgConfig, (u8)layer, (u8)x, (u8)y, (u8)width, (u8)height, (u8)palette);
    ScheduleBgTilemapBufferTransfer(bgConfig, (u8)layer);
}

SysTask *DressUpBgScreen_QueueLoadChars(NarcId narcId, int fileId, BgConfig *bgConfig, u32 layer, u32 tileStart, enum HeapID heapID) {
    DressUpCharUpload *upload = Heap_Alloc(heapID, sizeof(DressUpCharUpload));
    memset(upload, 0, sizeof(DressUpCharUpload));
    upload->buffer = GfGfxLoader_GetCharData(narcId, fileId, FALSE, &upload->charData, heapID);
    upload->bgConfig = bgConfig;
    upload->layer = layer;
    upload->tileStart = tileStart;
    return SysTask_CreateOnVWaitQueue((SysTaskFunc)DressUpBgScreen_LoadCharsTask, upload, 128);
}

#pragma force_active reset

void DressUpBgScreen_QueueLoadPltt(NarcId narcId, int fileId, u32 location, u32 offset, u32 size, enum HeapID heapID) {
    DressUpPlttUpload *upload = Heap_Alloc(heapID, sizeof(DressUpPlttUpload));
    memset(upload, 0, sizeof(DressUpPlttUpload));
    upload->buffer = GfGfxLoader_GetPlttData(narcId, fileId, &upload->plttData, heapID);
    upload->location = location;
    upload->offset = offset;
    upload->size = size;
    SysTask_CreateOnVWaitQueue((SysTaskFunc)DressUpBgScreen_LoadPlttTask, upload, 128);
}

void DressUpBgScreen_LoadCharsTask(SysTask *task, DressUpCharUpload *upload) {
    DC_FlushRange(upload->charData->pRawData, upload->charData->szByte);
    BG_LoadCharTilesData(upload->bgConfig, upload->layer, upload->charData->pRawData, upload->charData->szByte, upload->tileStart);
    SysTask_Destroy(task);
    Heap_Free(upload->buffer);
    Heap_Free(upload);
}

void DressUpBgScreen_LoadPlttTask(SysTask *task, DressUpPlttUpload *upload) {
    DC_FlushRange(upload->plttData->pRawData, upload->size);
    if (upload->location == 0) {
        GX_LoadBGPltt(upload->plttData->pRawData, upload->offset, upload->size);
    } else if (upload->location == 4) {
        GXS_LoadBGPltt(upload->plttData->pRawData, upload->offset, upload->size);
    }
    SysTask_Destroy(task);
    Heap_Free(upload->buffer);
    Heap_Free(upload);
}

// buttons of the editor's panel (affine sprite, optional text on it)
void DressUpButton_Init(DressUpButton *button, const DressUpButtonTemplate *template) {
    if (!button) {
        GF_AssertFail();
    }
    if (!template) {
        GF_AssertFail();
    }
    button->sprite = Sprite_CreateAffine(template->spriteTemplate);
    if (!button->sprite) {
        GF_AssertFail();
    }
    button->trigger = template->trigger;
    button->arg = template->arg;
    if (template->callback) {
        button->callback = template->callback;
        return;
    }
    button->callback = ov41_0224A254;
}

void DressUpButton_InitWithText(DressUpTextButton *button, const DressUpTextButtonTemplate *template) {
    TextOBJTemplate textTemplate;
    DressUpButton_Init(&button->button, &template->button);
    textTemplate.fontSystem = template->fontSystem;
    textTemplate.window = template->window;
    textTemplate.spriteList = template->button.spriteTemplate->spriteList;
    textTemplate.plttResourceProxy = template->plttProxy;
    textTemplate.sprite = button->button.sprite;
    textTemplate.offset = template->charOffset;
    textTemplate.x = template->x;
    textTemplate.y = template->y;
    textTemplate.unk_20 = 0;
    textTemplate.unk_24 = 0;
    textTemplate.vram = template->button.spriteTemplate->whichScreen;
    textTemplate.heapID = template->button.spriteTemplate->heapID;
    button->textObj = FontOAM_Create(&textTemplate);
}
