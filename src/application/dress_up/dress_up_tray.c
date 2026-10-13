#include "global.h"

#include "application/dress_up/dress_up_internal.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "touchscreen.h"

// The tray of the Dress-Up editor: owned accessories (14 pages by category) and backdrops (2 pages of 9) as objects
// in per-page lists, the tab BG, and the page swap (old page slides out, new page slides in).

// page swap task
typedef struct DressUpPageSwap {
    DressUpTray *tray;       // 0x00
    int fromGroup;           // 0x04
    int fromPage;            // 0x08
    int toGroup;             // 0x0C
    int toPage;              // 0x10
    BOOL tabSlideDone;       // 0x14
    int counter;             // 0x18
    int state;               // 0x1C
    int fromLeft;            // 0x20, objects of the old page still to slide out
    int toLeft;              // 0x24, objects of the new page still to slide in
    DressUpSlideAnim *anims; // 0x28
    int animCount;           // 0x2C
} DressUpPageSwap;

void DressUpTray_PageSwapTask(SysTask *task, DressUpPageSwap *swap);

// how far the object sticks out of the canvas (0 if inside)
void DressUpCanvas_GetPushback(DressUpNode *node, int *outX, int *outY) {
    int width;
    int height;
    int x;
    int y;
    int left;
    int top;
    int right;
    int bottom;
    int overLeft;
    int overRight;
    int overTop;
    int overBottom;

    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetMargins(node, &left, &right, &top, &bottom);
    overLeft = 138 - (x + left);
    overRight = x + width - right - 246;
    overTop = 18 - (y + top);
    overBottom = y + height - bottom - 143;
    if (overLeft > 0) {
        *outX = overLeft;
    } else if (overRight > 0) {
        *outX = -overRight;
    } else {
        *outX = 0;
    }
    if (overTop > 0) {
        *outY = overTop;
    } else if (overBottom > 0) {
        *outY = -overBottom;
    } else {
        *outY = 0;
    }
}

void DressUpTray_Init(DressUpTray *tray, const DressUpTrayTemplate *template) {
    tray->graphicsManager = template->graphicsManager;
    tray->images = template->images;
    tray->palettes = template->palettes;
    tray->paletteSlots = template->paletteSlots;
    tray->bgConfig = template->bgConfig;
    tray->slotList = template->slotList;
    tray->nodePool = template->nodePool;
    tray->caseTable = template->caseTable;
    DressUpTray_LoadTabBg(tray, 0);
    DressUpTray_InitPageLists(tray, template);
}

void DressUpTray_Destroy(DressUpTray *tray) {
    int i;
    DressUpTrayGroup *group;

    DressUpTray_ClearObjects(tray);
    i = 0;
    group = tray->groups;
    while (i < 4) {
        DressUpTray_FreePageTable(group);
        i++;
        group++;
    }
    memset(tray, 0, sizeof(DressUpTray));
}

// page of an item: accessories by category, backdrops 9 per page in fashion case order
int DressUpTray_GetItemPage(int mode, int id, u32 *caseTable) {
    switch (mode) {
    case 0:
        if (id <= 5) {
            return 0;
        }
        if (id <= 11) {
            return 1;
        }
        if (id <= 17) {
            return 2;
        }
        if (id <= 21) {
            return 3;
        }
        if (id <= 28) {
            return 4;
        }
        if (id <= 33) {
            return 5;
        }
        if (id <= 38) {
            return 6;
        }
        if (id <= 42) {
            return 7;
        }
        if (id <= 49) {
            return 8;
        }
        if (id <= 55) {
            return 9;
        }
        if (id <= 60) {
            return 10;
        }
        if (id <= 71) {
            return 11;
        }
        if (id <= 91) {
            return 12;
        }
        if (id <= 99) {
            return 13;
        }
        break;
    case 1:
        return DressUp_GetBackdropOrder(caseTable, id) / 9;
    case 2:
        mode = DressUp_GetBackdropOrder(caseTable, id) / 9;
        break;
    }
    return mode;
}

// accessories land at a random spot of the tray, backdrops on a 3x3 grid
void DressUpTray_GetItemSpawnPos(int mode, int id, int *outX, int *outY, int width, int height, u32 *caseTable) {
    switch (mode) {
    case 0:
        *outX = 10;
        *outY = 18;
        *outX += MTRandom() % (108 - width);
        *outY += MTRandom() % (125 - height);
        break;
    case 1:
    case 2:
        DressUpTray_GetGridPos(DressUp_GetBackdropOrder(caseTable, id), outX, outY);
        break;
    }
}

#pragma force_active on

void DressUpTray_AddItem(DressUpTray *tray, int mode, int id) {
    DressUpSlotTemplate template;
    int x, y;
    int width, height;
    DressUpSlot *slot;
    DressUpNode *node;
    int imageIndex;
    int paletteIndex;
    int page;

    template.list = tray->slotList;
    template.id = id;
    template.graphicsManager = tray->graphicsManager;
    template.x = 0;
    template.y = 0;
    switch (mode) {
    case 0:
        imageIndex = id;
        paletteIndex = 0;
        template.paletteSlot = tray->paletteSlots[id];
        break;
    case 1:
        imageIndex = id;
        imageIndex += DRESS_UP_ACCESSORY_NUM;
        paletteIndex = id + 1;
        template.paletteSlot = 0;
        break;
    case 2:
        imageIndex = id;
        imageIndex += DRESS_UP_ACCESSORY_NUM;
        paletteIndex = id + 1;
        template.paletteSlot = 0;
        break;
    default:
        GF_AssertFail();
        break;
    }
    template.image = tray->images[imageIndex];
    template.palette = tray->palettes[paletteIndex];
    GF_ASSERT(template.image != NULL);
    GF_ASSERT(template.palette != NULL);
    page = DressUpTray_GetItemPage(mode, id, tray->caseTable);
    slot = DressUpGfx_AddSlotObject(&template);
    node = DressUpNodePool_Acquire(tray->nodePool, slot, mode);
    DressUpList_InsertAfter(node, tray->groups[mode].pages[page].prev);
    DressUpObj_GetSize(node, &width, &height);
    DressUpTray_GetItemSpawnPos(mode, id, &x, &y, width, height, tray->caseTable);
    DressUpObj_SetPos(node, x, y);
    DressUpTray_SyncObjectVisible(tray, slot, mode, page);
}

void DressUpTray_LinkObjectToPage(DressUpTray *tray, int mode, int page, DressUpNode *node) {
    DressUpList_InsertAfter(node, &tray->groups[mode].pages[page]);
    DressUpTray_SyncObjectVisible(tray, node->object, mode, page);
}

#pragma force_active reset

void DressUpTray_RemoveNode(DressUpNode *node) {
    DressUpList_Remove(node);
}

void DressUpTray_ClearObjects(DressUpTray *tray) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < tray->groups[i].pageCount; j++) {
            DressUpList_Clear(&tray->groups[i].pages[j]);
        }
    }
}

void DressUpTray_RefreshPriorities(DressUpTray *tray) {
    DressUpNode *node;
    int priority = -1;
    DressUpNode *head = DressUpTray_GetCurPageHead(tray);
    node = head->next;
    while (node != head) {
        if (node->mode <= 2) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }
}

BOOL DressUpTray_AnimateToPage(DressUpTray *tray, int group, int page) {
    if (tray->pageSwapActive == FALSE) {
        int oldGroup = tray->group;
        DressUpTray_StartPageSwapTask(tray, oldGroup, tray->groups[oldGroup].curPage, group, page);
        tray->group = group;
        tray->groups[group].curPage = page;
        DressUpTray_RefreshPriorities(tray);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// next / previous page of the group that has objects on it
int DressUpTray_CyclePage(DressUpTray *tray, int group, BOOL reverse) {
    int start = DressUpTray_GetGroupPage(tray, group);
    // codegen: the page count is re-read every time, through the tray shifted by the group
    volatile DressUpTray *shifted = (volatile DressUpTray *)((u8 *)tray + group * sizeof(DressUpTrayGroup));
    int i;
    for (i = 1; i < shifted->groups[0].pageCount; i++) {
        int page;
        if (reverse == FALSE) {
            page = (i + start) % shifted->groups[0].pageCount;
        } else {
            page = start - i;
            if (page < 0) {
                page += shifted->groups[0].pageCount;
            }
        }
        {
            DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
            if (head->next != head) {
                return DressUpTray_AnimateToPage(tray, group, page);
            }
        }
    }
    return 0;
}

#pragma force_active reset

void DressUpTray_SetPageImmediate(DressUpTray *tray, int group, int page) {
    DressUpTray_SetCurPageVisible(tray, FALSE);
    tray->group = group;
    tray->groups[group].curPage = page;
    DressUpTray_SetCurPageVisible(tray, TRUE);
    DressUpTray_RefreshPriorities(tray);
}

BOOL DressUpTray_IsTouchHeldIn(DressUpTray *tray) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 10;
    hitbox.rect.right = 118;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL DressUpTray_IsPointIn(DressUpTray *tray, u32 x, u32 y) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 10;
    hitbox.rect.right = 118;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

DressUpNode *DressUpTray_FindObjectAt(DressUpTray *tray, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    DressUpNode *node;
    DressUpNode *head;
    head = DressUpTray_GetCurPageHead(tray);
    node = head->next;
    while (node != head) {
        if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void DressUpTray_LoadTabBg(DressUpTray *tray, int tab) {
    DressUpBgScreenTemplate template;
    template.bgConfig = tray->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = tab * 2 + 0x81;
    template.plttFileId = 0x85;
    template.screenFileId = tab * 2 + 0x82;
    template.x = 8;
    template.y = 129;
    template.layer = GF_BG_LYR_MAIN_3;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&tray->tabBg, &template);
    tray->tabBgIndex = tab;
}

#pragma force_active on

// loads the tab BG and slides it in; flags: 1 from the side, 2 from below
void DressUpTray_SlideInTabBg(DressUpTray *tray, int tab, u32 flags, int frames, BOOL *done) {
    DressUpBgScreenTemplate template;
    int dx, dy;
    template.bgConfig = tray->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = tab * 2 + 0x81;
    template.plttFileId = 0x85;
    template.screenFileId = tab * 2 + 0x82;
    template.x = 8;
    template.y = 129;
    template.layer = GF_BG_LYR_MAIN_3;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    dx = 0;
    if (flags & 1) {
        dx = 112;
    }
    if (flags & 2) {
        dy = 129;
    } else {
        dy = 0;
    }
    DressUp_StartBgSlide(&tray->tabBg, &template, dx, dy, frames, done);
    tray->tabBgIndex = tab;
}

#pragma force_active reset

void DressUpTray_FreeTabBg(DressUpTray *tray) {
    DressUpBgScreen_Unload(&tray->tabBg);
}

int DressUpTray_GetGroup(DressUpTray *tray) {
    if (tray == NULL) {
        GF_AssertFail();
    }
    return tray->group;
}

int DressUpTray_GetGroupPage(DressUpTray *tray, int group) {
    DressUpTrayGroup pages;
    if (tray == NULL) {
        GF_AssertFail();
    }
    pages = tray->groups[group];
    return pages.curPage;
}

void DressUpTray_MovePageObjects(DressUpTray *tray, int group, int page, int dx, int dy) {
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpObj_MoveListBy(head, dx, dy);
}

BOOL DressUpTray_IsIdle(DressUpTray *tray) {
    return tray->pageSwapActive == FALSE;
}

// page lists per group: accessories, backdrops, group 2, group 3 (the empty tray while DONE)
void DressUpTray_InitPageLists(DressUpTray *tray, const DressUpTrayTemplate *template) {
    DressUpTray_AllocPageTable(&tray->groups[0], template->pageCounts[0]);
    tray->group = 0;
    DressUpTray_SetListVisible(tray->groups[0].pages, TRUE);
    DressUpTray_AllocPageTable(&tray->groups[1], template->pageCounts[1]);
    DressUpTray_AllocPageTable(&tray->groups[2], template->pageCounts[2]);
    DressUpTray_AllocPageTable(&tray->groups[3], 1);
}

void DressUpTray_SetListVisible(DressUpNode *head, BOOL visible) {
    DressUpNode *node = head->next;
    while (node != head) {
        if (node->mode <= 2) {
            DressUpGfx_SetSlotVisible(node->object, visible);
        }
        node = node->next;
    }
}

void DressUpTray_SetPageVisible(DressUpTray *tray, int group, int page, BOOL visible) {
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpTray_SetListVisible(head, visible);
}

void DressUpTray_SetCurPageVisible(DressUpTray *tray, BOOL visible) {
    DressUpNode *head = DressUpTray_GetCurPageHead(tray);
    DressUpTray_SetListVisible(head, visible);
}

#pragma force_active on

void DressUpTray_AllocPageTable(DressUpTrayGroup *group, int count) {
    int i;
    group->pages = Heap_Alloc(HEAP_ID_14, count * sizeof(DressUpNode));
    group->pageCount = count;
    group->curPage = 0;
    for (i = 0; i < group->pageCount; i++) {
        group->pages[i].next = &group->pages[i];
        group->pages[i].prev = &group->pages[i];
        DressUpTray_SetListVisible(&group->pages[i], FALSE);
    }
}

#pragma force_active reset

void DressUpTray_FreePageTable(DressUpTrayGroup *group) {
    int zero;
    u8 *bytes = (u8 *)group;
    Heap_Free(group->pages);
    zero = 0;
    group->pages = NULL;
    // clears the group byte by byte
    bytes[0] = zero;
    bytes[1] = zero;
    bytes[2] = zero;
    bytes[3] = zero;
    bytes[4] = zero;
    bytes[5] = zero;
    bytes[6] = zero;
    bytes[7] = zero;
    bytes[8] = zero;
    bytes[9] = zero;
    bytes[10] = zero;
    bytes[11] = zero;
}

DressUpNode *DressUpTray_GetCurPageHead(DressUpTray *tray) {
    DressUpTrayGroup pages = tray->groups[tray->group];
    return &pages.pages[pages.curPage];
}

DressUpNode *DressUpTray_GetPageHead(DressUpTray *tray, int group, int page) {
    DressUpTrayGroup pages = tray->groups[group];
    return &pages.pages[page];
}

int DressUpTray_CountPageObjects(DressUpTray *tray, int group, int page) {
    int count = 0;
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpNode *node = head->next;
    while (node != head) {
        node = node->next;
        count++;
    }
    return count;
}

DressUpNode *DressUpTray_GetPageObject(DressUpTray *tray, int group, int page, int index) {
    DressUpNode *node;
    int i = 0;
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    node = head->next;
    while (node != head) {
        if (i == index) {
            return node;
        }
        node = node->next;
        i++;
    }
    return NULL;
}

// an object is visible only on the shown page
void DressUpTray_SyncObjectVisible(DressUpTray *tray, DressUpSlot *slot, int group, int page) {
    if (tray->group != group || page != tray->groups[group].curPage) {
        DressUpGfx_SetSlotVisible(slot, FALSE);
    } else {
        DressUpGfx_SetSlotVisible(slot, TRUE);
    }
}

#pragma force_active on

// backdrop grid: 3x3 cells of 24x32 with 8 pixel gaps
void DressUpTray_GetGridPos(int index, int *x, int *y) {
    int cell;
    int row;
    int column;
    cell = index % 9;
    row = cell / 3;
    column = cell % 3;
    *y = (row + 1) * 8 + row * 32;
    *y += 16;
    *x = (column + 1) * 8 + column * 24;
    *x += 8;
}

void DressUpTray_StartPageSwapTask(DressUpTray *tray, int fromGroup, int fromPage, int toGroup, int toPage) {
    DressUpPageSwap *swap = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUpTray_PageSwapTask, sizeof(DressUpPageSwap), 0, HEAP_ID_13));
    swap->tray = tray;
    swap->fromGroup = fromGroup;
    swap->fromPage = fromPage;
    swap->toGroup = toGroup;
    swap->toPage = toPage;
    swap->state = 0;
    swap->fromLeft = DressUpTray_CountPageObjects(tray, fromGroup, fromPage);
    swap->toLeft = DressUpTray_CountPageObjects(tray, toGroup, toPage);
    swap->animCount = swap->fromLeft + swap->toLeft;
    swap->anims = Heap_Alloc(HEAP_ID_13, sizeof(DressUpSlideAnim) * swap->animCount);
    if (!swap->anims) {
        GF_AssertFail();
    }
    memset(swap->anims, 0, sizeof(DressUpSlideAnim) * swap->animCount);
    tray->pageSwapActive = TRUE;
}

// the new page waits 132 pixels up; the old page drops out one object per frame, then the new page drops in
// (two per frame) while the tab BG slides to the other tab
void DressUpTray_PageSwapTask(SysTask *task, DressUpPageSwap *swap) {
    int i;
    switch (swap->state) {
    case 0:
        DressUpTray_MovePageObjects(swap->tray, swap->toGroup, swap->toPage, 0, -132);
        DressUpTray_SetPageVisible(swap->tray, swap->toGroup, swap->toPage, TRUE);
        swap->tabSlideDone = FALSE;
        DressUpTray_SlideInTabBg(swap->tray, (swap->tray->tabBgIndex + 1) % 2, 2, 5, &swap->tabSlideDone);
        swap->counter = ov41_02248E10(swap->fromLeft, 1);
        swap->state++;
        break;
    case 1:
        for (i = 0; i < swap->counter; i++) {
            if (swap->fromLeft - 1 >= 0) {
                swap->fromLeft--;
                DressUp_StartSlideAnim(DressUpTray_GetPageObject(swap->tray, swap->fromGroup, swap->fromPage, swap->fromLeft), swap->anims, swap->animCount);
            }
        }
        if (swap->fromLeft == 0) {
            swap->counter = ov41_02248E10(swap->toLeft, 2);
            swap->state++;
        }
        break;
    case 2:
        for (i = 0; i < swap->counter; i++) {
            if (swap->toLeft - 1 >= 0) {
                swap->toLeft--;
                DressUp_StartSlideAnim(DressUpTray_GetPageObject(swap->tray, swap->toGroup, swap->toPage, swap->toLeft), swap->anims, swap->animCount);
            }
        }
        if (swap->toLeft == 0) {
            swap->state++;
            swap->counter = 0;
        }
        break;
    case 3:
        swap->counter++;
        if (swap->counter > 3 && swap->tabSlideDone) {
            swap->state++;
        }
        break;
    case 4:
        DressUpTray_SetPageVisible(swap->tray, swap->fromGroup, swap->fromPage, FALSE);
        DressUpTray_MovePageObjects(swap->tray, swap->fromGroup, swap->fromPage, 0, -132);
        swap->tray->pageSwapActive = FALSE;
        Heap_Free(swap->anims);
        DestroySysTaskAndEnvironment(task);
        return;
    default:
        GF_AssertFail();
        break;
    }
    DressUp_TickSlideAnims(swap->anims, swap->animCount);
}

#pragma force_active reset

void DressUp_StartSlideAnim(DressUpNode *node, DressUpSlideAnim *anims, int count) {
    DressUpSlideAnim *anim = DressUp_FindFreeSlideAnim(anims, count);
    anim->node = node;
    anim->remaining = 3;
    anim->delta = 44;
}

DressUpSlideAnim *DressUp_FindFreeSlideAnim(DressUpSlideAnim *anims, int count) {
    int i;
    i = 0;
    if (count > 0) {
        DressUpSlideAnim *anim = anims;
        do {
            if (anim->node == NULL) {
                return &anims[i];
            }
            i++;
            anim++;
        } while (i < count);
    }
    return NULL;
}

void DressUp_TickSlideAnims(DressUpSlideAnim *anims, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (anims->node != NULL) {
            DressUp_TickSlideAnim(anims);
        }
        anims++;
    }
}

#pragma force_active on

void DressUp_TickSlideAnim(DressUpSlideAnim *anim) {
    int x, y;
    DressUpObj_GetPos(anim->node, &x, &y);
    y += anim->delta;
    DressUpObj_SetPos(anim->node, x, y);
    if (--anim->remaining <= 0) {
        // clears the slot byte by byte
        u8 *bytes = (u8 *)anim;
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 0;
        bytes[4] = 0;
        bytes[5] = 0;
        bytes[6] = 0;
        bytes[7] = 0;
        bytes[8] = 0;
        bytes[9] = 0;
        bytes[10] = 0;
        bytes[11] = 0;
    }
}

#pragma force_active reset
