#include "global.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg/msg_0215.h"

#include "bg_window.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

// The editor's save prompt, then the canvas (the stage) shared by the Dress-Up editor and the Accessory Portrait:
// backdrop, the mon and the accessories on it, kept in two lists (accessories touching the mon move with it).

void DressUp_ShowSavePrompt(DressUpWork *work) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = work->gfx.bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = work->menuInputState;
    YesNoPrompt_Reset(work->yesNoPrompt);
    YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
    DressUp_PrintPromptMsgWindow(work, msg_0215_00002);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

int DressUp_HandleSavePrompt(DressUpWork *work) {
    int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_WAIT:
        return DRESS_UP_UI_SAVE;
    case YESNORESPONSE_YES:
        result = DRESS_UP_UI_SAVE_YES;
        break;
    case YESNORESPONSE_NO:
        result = DRESS_UP_UI_SAVE_NO;
        break;
    }
    work->menuInputState = YesNoPrompt_IsInTouchMode(work->yesNoPrompt);
    return result;
}

// point `index` of a 4x4 grid over the rectangle, column by column
void DressUpCanvas_GetRectSamplePoint(u32 top, u32 bottom, u32 left, u32 right, u32 index, u32 *outX, u32 *outY) {
    u32 dx = (right - left) / 3;
    u32 x1 = left + dx;
    u32 x2 = left + dx * 2;
    u32 dy = (bottom - top) / 3;
    u32 y1 = top + dy;
    u32 y2 = top + dy * 2;
    switch (index) {
    case 0:
        *outX = left;
        *outY = top;
        return;
    case 1:
        *outX = left;
        *outY = y1;
        return;
    case 2:
        *outX = left;
        *outY = y2;
        return;
    case 3:
        *outX = left;
        *outY = bottom;
        return;
    case 4:
        *outX = x1;
        *outY = top;
        return;
    case 5:
        *outX = x1;
        *outY = y1;
        return;
    case 6:
        *outX = x1;
        *outY = y2;
        return;
    case 7:
        *outX = x1;
        *outY = bottom;
        return;
    case 8:
        *outX = x2;
        *outY = top;
        return;
    case 9:
        *outX = x2;
        *outY = y1;
        return;
    case 10:
        *outX = x2;
        *outY = y2;
        return;
    case 11:
        *outX = x2;
        *outY = bottom;
        return;
    case 12:
        *outX = right;
        *outY = top;
        return;
    case 13:
        *outX = right;
        *outY = y1;
        return;
    case 14:
        *outX = right;
        *outY = y2;
        return;
    case 15:
        *outX = right;
        *outY = bottom;
        return;
    }
}

void DressUpCanvas_Init(DressUpCanvas *canvas, const DressUpCanvasTemplate *template) {
    canvas->graphicsManager = template->graphicsManager;
    canvas->images = template->images;
    canvas->palettes = template->palettes;
    canvas->paletteSlots = template->paletteSlots;
    canvas->pokepicManager = template->pokepicManager;
    canvas->bgConfig = template->bgConfig;
    canvas->slotList = template->slotList;
    canvas->backdropId = 0;
    canvas->nodePool = template->nodePool;
    canvas->looseList.next = &canvas->looseList;
    canvas->looseList.prev = &canvas->looseList;
    canvas->monList.next = &canvas->monList;
    canvas->monList.prev = &canvas->monList;
    canvas->count = 0;
    canvas->limit = template->limit;
    DressUpList_InsertAfter(DressUpNodePool_Acquire(canvas->nodePool, &canvas->monPic, 3), &canvas->monList);
}

void DressUpCanvas_Destroy(DressUpCanvas *canvas) {
    DressUpCanvas_FreeMonPic(canvas);
    DressUpCanvas_ClearObjects(canvas);
    memset(canvas, 0, sizeof(DressUpCanvas));
}

void AccessoryPortrait_CreateCanvasMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, int x, int y, int priority, enum HeapID heapID) {
    DressUpMon_Create(&canvas->monPic, canvas->pokepicManager, mon, template, heapID);
    DressUpMon_SetPriority(&canvas->monPic, priority);
    DressUpMon_SetCenterPos(&canvas->monPic, x, y);
}

void DressUpCanvas_CreateMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_Create(&canvas->monPic, canvas->pokepicManager, mon, template, heapID);
}

void DressUpCanvas_CreateMonPicEx(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_CreateEx(&canvas->monPic, canvas->pokepicManager, mon, template, heapID, 1);
}

void DressUpCanvas_AddToMonList(DressUpCanvas *canvas, DressUpNode *node) {
    DressUpList_InsertAfter(node, canvas->monList.prev);
}

void DressUpCanvas_RemoveNode(DressUpNode *node) {
    DressUpList_Remove(node);
}

void DressUpCanvas_FreeMonPic(DressUpCanvas *canvas) {
    DressUpMon_Delete(&canvas->monPic);
}

#pragma force_active on

BOOL AccessoryPortrait_PlaceCanvasAccessory(DressUpCanvas *canvas, int accessory, int x, int y, int priority) {
    if (canvas->count < canvas->limit) {
        DressUpSlotTemplate template;
        DressUpSlot *slot;
        template.list = canvas->slotList;
        template.graphicsManager = canvas->graphicsManager;
        template.image = canvas->images[accessory];
        template.palette = *canvas->palettes;
        template.x = x;
        template.y = y;
        template.id = accessory;
        template.paletteSlot = canvas->paletteSlots[accessory];
        slot = DressUpGfx_AddSlotObject(&template);
        DressUpGfx_SetSlotPriority(slot, priority);
        DressUpList_InsertAfter(DressUpNodePool_Acquire(canvas->nodePool, slot, 0), &canvas->looseList);
        canvas->count++;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

BOOL DressUpCanvas_TryAddAccessory(DressUpCanvas *canvas, DressUpNode *node, NNSG2dCharacterData **hitMaps) {
    if (canvas->count < canvas->limit) {
        DressUpCanvas_LinkByMonOverlap(canvas, node, TRUE, hitMaps);
        canvas->count++;
        return TRUE;
    }
    return FALSE;
}

void DressUpCanvas_RemoveAccessory(DressUpCanvas *canvas, DressUpNode *node) {
    DressUpList_Remove(node);
    canvas->count--;
    if (canvas->count < 0) {
        GF_AssertFail();
    }
}

void DressUpCanvas_ClearObjects(DressUpCanvas *canvas) {
    DressUpList_Clear(&canvas->looseList);
    DressUpList_Clear(&canvas->monList);
    canvas->count = 0;
}

void DressUpCanvas_MoveAll(DressUpCanvas *canvas, int dx, int dy) {
    DressUpObj_MoveListBy(&canvas->looseList, dx, dy);
    DressUpObj_MoveListBy(&canvas->monList, dx, dy);
}

void DressUpCanvas_MoveMonGroup(DressUpCanvas *canvas, int dx, int dy) {
    DressUpObj_MoveListBy(&canvas->monList, dx, dy);
}

// moves everything from origin (x, y) to (newX, newY)
void DressUpCanvas_SetOrigin(DressUpCanvas *canvas, int x, int y, int newX, int newY) {
    int negY = -y;
    int negX = -x;
    DressUpObj_MoveListBy(&canvas->looseList, negX, negY);
    DressUpObj_MoveListBy(&canvas->looseList, newX, newY);
    DressUpObj_MoveListBy(&canvas->monList, negX, negY);
    DressUpObj_MoveListBy(&canvas->monList, newX, newY);
}

void DressUpCanvas_RefreshPriorities(DressUpCanvas *canvas) {
    DressUpCanvas_SetPriorities(canvas, -1);
}

// priorities count down from `priority`: the mon's list first, then (8 lower) the loose accessories
void DressUpCanvas_SetPriorities(DressUpCanvas *canvas, int priority) {
    DressUpNode *node = canvas->monList.next;
    DressUpNode *end = &canvas->monList;
    while (node != end) {
        if (node->mode == 0) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        } else {
            DressUpMon_SetPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }

    node = canvas->looseList.next;
    end = &canvas->looseList;
    priority -= 8;
    while (node != end) {
        if (node->mode == 0) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        } else {
            DressUpMon_SetPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }
}

BOOL DressUpCanvas_IsTouchHeldIn(DressUpCanvas *canvas) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL DressUpCanvas_IsPointIn(DressUpCanvas *canvas, u32 x, u32 y) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

#pragma force_active on

// topmost object at (x, y): the mon's list first, then the loose accessories
DressUpNode *DressUpCanvas_FindObjectAt(DressUpCanvas *canvas, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    DressUpNode *node = canvas->monList.next;

    struct {
        DressUpCanvas *canvas;
        DressUpNode *end;
    } it = { canvas, (DressUpNode *)canvas }; // codegen: a two-word local

    DressUpNode *end;
    it.end = &canvas->monList;
    if (node != it.end) {
        do {
            if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
                return node;
            }
            node = node->next;
        } while (node != it.end);
    }
    node = canvas->looseList.next;
    end = &canvas->looseList;
    while (node != end) {
        if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

#pragma force_active reset

void DressUpCanvas_SetBackdrop(DressUpCanvas *canvas, int backdrop, enum HeapID heapID) {
    DressUpBgScreenTemplate template;
    template.bgConfig = canvas->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = backdrop * 4 + 0x86;
    template.plttFileId = backdrop * 4 + 0x87;
    template.screenFileId = backdrop * 4 + 0x88;
    template.x = 136;
    template.y = 16;
    template.layer = GF_BG_LYR_MAIN_2;
    template.plttCount = 1;
    template.plttSlot = 13;
    template.charOffset = 0;
    template.heapID = heapID;
    DressUpBgScreen_LoadFromTemplate(&canvas->backdrop, &template);
    canvas->backdropId = backdrop;
}

void DressUpCanvas_FreeBackdrop(DressUpCanvas *canvas) {
    DressUpBgScreen_Unload(&canvas->backdrop);
}

int DressUpCanvas_GetCount(DressUpCanvas *canvas) {
    return canvas->count;
}

// largest pushback (by magnitude) any object on the mon needs to get back inside the canvas
void DressUpCanvas_GetMaxOverflow(DressUpCanvas *canvas, int *outX, int *outY) {
    DressUpNode *node = canvas->monList.next;
    int maxX = 0;
    int maxY = 0;
    DressUpNode *end = &canvas->monList;

    while (node != end) {
        int x;
        int y;
        DressUpCanvas_GetPushback(node, &x, &y);
        if ((x < 0 ? -x : x) > (maxX < 0 ? -maxX : maxX)) {
            maxX = x;
        }
        if ((y < 0 ? -y : y) > (maxY < 0 ? -maxY : maxY)) {
            maxY = y;
        }
        node = node->next;
    }
    *outX = maxX;
    *outY = maxY;
}

#pragma force_active on
#pragma opt_propagation off

// links the node into the mon's list if any of 16 sample points of its opaque area hits the mon, else into the
// loose list; at the front of the list or at the back
void DressUpCanvas_LinkByMonOverlap(DressUpCanvas *canvas, DressUpNode *node, BOOL atFront, NNSG2dCharacterData **hitMaps) {
    int x, y;
    int width, height;
    int left, top;
    u32 sampleX, sampleY;
    int right, bottom;
    DressUpCanvas *area;
    int marginTop;
    int y0;
    NNSG2dCharacterData *monHitMap;
    int x1;
    int y1;
    int x0;
    int marginLeft;
    int i;
    int hit;

    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetMargins(node, &left, &right, &top, &bottom);
    monHitMap = hitMaps[DRESS_UP_IMAGE_MAX];
    hit = 0;
    x0 = x;
    marginLeft = left;
    x1 = x0 + width - right;
    y0 = y;
    y1 = y0 + height - bottom;
    marginTop = top;
    area = canvas;
    i = 0;
    do {
        DressUpCanvas_GetRectSamplePoint(y0 + marginTop, y1, x0 + marginLeft, x1, i, &sampleX, &sampleY);
        hit |= DressUpMon_HitTestPoint(&area->monPic, sampleX, sampleY, monHitMap);
    } while (++i < 16);
    if (hit) {
        if (atFront) {
            DressUpList_InsertAfter(node, &canvas->monList);
        } else {
            DressUpList_InsertAfter(node, canvas->monList.prev);
        }
    } else {
        if (atFront) {
            DressUpList_InsertAfter(node, &canvas->looseList);
        } else {
            DressUpList_InsertAfter(node, canvas->looseList.prev);
        }
    }
}

#pragma opt_propagation reset
#pragma force_active reset
