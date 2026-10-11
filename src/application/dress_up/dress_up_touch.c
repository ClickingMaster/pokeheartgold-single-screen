#include "global.h"

#include "constants/sndseq.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "assert.h"
#include "fashion_case.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "pokepic.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"

// Touch handling of the Dress-Up editor: the touch dispatcher (new touch / held / released), the fashion case
// table, and the drag controller that picks objects up from the canvas or the tray and drops them again.

int FashionCase_GetWallpaperOrder(FashionCase *fashionCase, int index); // u8 in save_fashion.c; the caller does not truncate

// object tween back into the tray
typedef struct DressUpTrayTween {
    DressUpTray *tray; // 0x00
    DressUpNode *node; // 0x04
    int stepX;         // 0x08
    int stepY;         // 0x0C
    int destX;         // 0x10
    int destY;         // 0x14
    int mode;          // 0x18
    int page;          // 0x1C
    int frames;        // 0x20
    BOOL *paused;      // 0x24, DressUpTray.pageSwapActive
    BOOL *dragging;    // 0x28, DressUpDrag.dragging
} DressUpTrayTween;

void DressUp_TweenToTrayTask(SysTask *task, DressUpTrayTween *tween);

// ceil(value / divisor) for positive values that are not multiples, value / divisor + 1 otherwise
int ov41_02248E10(int value, int divisor) {
    int d = divisor;
    int v = value;
    int rem = v % d;
    return (d - rem + v) / d;
}

void DressUpTouch_Reset(DressUpTouch *touch) {
    u8 *bytes = (u8 *)touch;
    u32 n = sizeof(DressUpTouch);
    while (n != 0) {
        *bytes++ = 0;
        n--;
    }
    touch->onTouch = ov41_02248E80;
    touch->onRelease = ov41_02248E80;
    touch->onHeld = ov41_02248E80;
}

void DressUpTouch_Update(DressUpTouch *touch) {
    if (gSystem.touchNew != 0) {
        touch->onTouch(touch);
    } else if (gSystem.touchHeld != 0) {
        touch->onHeld(touch);
    } else if (touch->wasHeld != 0) {
        touch->onRelease(touch);
    }
    touch->lastX = gSystem.touchX;
    touch->lastY = gSystem.touchY;
    touch->wasHeld = gSystem.touchHeld;
}

void ov41_02248E80(DressUpTouch *touch) {
    return;
}

// owned count per accessory, then the order of each owned backdrop (18 = not owned)
void DressUp_LoadFashionCaseTable(FashionCase *fashionCase, u32 *caseTable) {
    int i;
    for (i = 0; i < DRESS_UP_ACCESSORY_NUM; i++) {
        caseTable[i] = FashionCase_GetAccessoryCount(fashionCase, i);
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        caseTable[DRESS_UP_ACCESSORY_NUM + i] = DRESS_UP_BACKDROP_NUM;
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        int backdrop = FashionCase_GetWallpaperOrder(fashionCase, i);
        if (backdrop != DRESS_UP_BACKDROP_NUM) {
            caseTable[DRESS_UP_ACCESSORY_NUM + backdrop] = i;
        }
    }
}

u32 DressUp_GetAccessoryCount(u32 *caseTable, u32 accessory) {
    if (accessory >= DRESS_UP_ACCESSORY_NUM) {
        GF_AssertFail();
    }
    return caseTable[accessory];
}

u32 DressUp_GetBackdropByOrder(u32 *caseTable, u32 order) {
    u32 *entry = &caseTable[order];
    return entry[DRESS_UP_ACCESSORY_NUM];
}

int DressUp_GetBackdropOrder(u32 *caseTable, u32 backdrop) {
    int i;
    i = 0;
    do {
        if (backdrop == caseTable[DRESS_UP_ACCESSORY_NUM]) {
            return i;
        }
        i++;
        caseTable++;
    } while (i < DRESS_UP_BACKDROP_NUM);
    return i;
}

void DressUpDrag_Init(DressUpTouch *touch, DressUpCanvas *canvas, DressUpTray *tray, DressUpGfx *gfx, DressUpHud *hud, BOOL allowMonDrag) {
    DressUpDrag *drag;
    DressUpTouch_Reset(touch);
    drag = Heap_Alloc(HEAP_ID_13, sizeof(DressUpDrag));
    touch->drag = drag;
    memset(drag, 0, sizeof(DressUpDrag));
    drag = touch->drag;
    drag->canvas = canvas;
    drag->tray = tray;
    drag->gfx = gfx;
    drag->hud = hud;
    drag->allowMonDrag = allowMonDrag;
    touch->onTouch = DressUpDrag_OnTouchIdle;
    touch->onRelease = DressUpDrag_OnReleaseAccessory;
    touch->onHeld = DressUpDrag_OnHeldAccessory;
    touch->destroy = DressUpDrag_Destroy;
}

void DressUpDrag_Destroy(DressUpTouch *touch) {
    Heap_Free(touch->drag);
    DressUpTouch_Reset(touch);
}

#pragma force_active on

// new touch: picks up the object under the stylus (canvas first, then the tray) and sets the drag callbacks
void DressUpDrag_OnTouchIdle(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int x, y;
    DressUpNode *node;
    int priority;
    if (DressUpCanvas_IsTouchHeldIn(drag->canvas)) {
        node = DressUpCanvas_FindObjectAt(drag->canvas, &x, &y, drag->gfx->hitMaps);
        if (node && node->mode == 3 && !drag->allowMonDrag) {
            node = NULL;
        }
        if (node) {
            if (!node->mode) {
                DressUpCanvas_RemoveAccessory(drag->canvas, node);
                priority = 0;
            } else {
                ov41_02248030(node);
                priority = DressUpMon_GetPriority(node->object);
                DressUpObj_GetPos(node, &drag->monX, &drag->monY);
            }
            DressUpCanvas_RefreshPriorities(drag->canvas);
            DressUpDrag_BeginDrag(drag, node, TRUE, x, y, priority);
            DressUp_EnableDragWindow();
        }
    } else if (DressUpTray_IsTouchHeldIn(drag->tray)) {
        node = DressUpTray_FindObjectAt(drag->tray, &x, &y, drag->gfx->hitMaps);
        if (node) {
            ov41_022486F0(node);
            DressUpTray_RefreshPriorities(drag->tray);
            DressUpDrag_BeginDrag(drag, node, FALSE, x, y, 0);
            DressUp_EnableDragWindow();
        }
    }
    if (drag->selected) {
        int se = SEQ_SE_DP_BOX02;
        switch (drag->selected->mode) {
        case 0:
            touch->onRelease = DressUpDrag_OnReleaseAccessory;
            touch->onHeld = DressUpDrag_OnHeldAccessory;
            break;
        case 1:
            touch->onRelease = DressUpDrag_OnReleaseBackdrop;
            touch->onHeld = ov41_022490AC;
            DressUpDrag_ApplyBackdrop(touch);
            se = SEQ_SE_DP_SYARAAN;
            break;
        case 3:
            touch->onRelease = DressUpDrag_OnReleaseMon;
            touch->onHeld = DressUpDrag_OnHeldMon;
            break;
        }
        PlaySE(se);
        DressUp_PrintSelectedItemName(drag);
    }
}

#pragma force_active reset

void ov41_022490AC(DressUpTouch *touch) {
    return;
}

// a backdrop is applied on touch; on release its tray item goes back
void DressUpDrag_OnReleaseBackdrop(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        GF_ASSERT(drag->selected->mode == 1);
        DressUp_StartTweenToTray(drag, 4, drag->originX, drag->originY, drag->selected->mode, drag->trayPage);
        DressUpDrag_ClearSelection(drag);
    }
}

// dropped fully inside the canvas: placed (if there is room); else back to the tray
void DressUpDrag_OnReleaseAccessory(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int top;
    int bottom;
    int left;
    int right;
    int width;
    int height;
    int x;
    int y;
    int n;
    if (drag->selected != NULL) {
        DressUpDrag_GetSelectedVisibleRect(touch, &top, &bottom, &left, &right);
        n = DressUpCanvas_IsPointIn(drag->canvas, left, top);
        n += DressUpCanvas_IsPointIn(drag->canvas, right, top);
        n += DressUpCanvas_IsPointIn(drag->canvas, left, bottom);
        if (n + DressUpCanvas_IsPointIn(drag->canvas, right, bottom) >= 4) {
            if (!DressUpCanvas_TryAddAccessory(drag->canvas, drag->selected, drag->gfx->hitMaps)) {
                x = drag->originX;
                y = drag->originY;
                PlaySE(SEQ_SE_DP_HYUN2);
                DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00003);
                DressUp_StartTweenToTray(drag, 4, x, y, drag->selected->mode, drag->trayPage);
            } else {
                DressUpCanvas_RefreshPriorities(drag->canvas);
                DressUp_DisableDragWindow();
                drag->dragging = FALSE;
                PlaySE(SEQ_SE_DP_BOX01);
            }
        } else {
            DressUpSlot *slot = drag->selected->object;
            DressUpDrag_GetSelectedRect(touch, &top, &bottom, &left, &right);
            n = DressUpTray_IsPointIn(drag->tray, left, top);
            if (n + DressUpTray_IsPointIn(drag->tray, right, bottom) < 2) {
                // dropped outside the tray: a random spot for items from the canvas, else where it came from
                if (drag->fromCanvas == TRUE) {
                    DressUpObj_GetSize(drag->selected, &width, &height);
                    x = MTRandom() % (108 - width) + 10;
                    y = MTRandom() % (125 - height) + 18;
                } else {
                    x = drag->originX;
                    y = drag->originY;
                }
                PlaySE(SEQ_SE_DP_HYUN2);
            } else {
                DressUpObj_GetPos(drag->selected, &x, &y);
                PlaySE(SEQ_SE_DP_BOX02);
            }
            if (drag->fromCanvas == TRUE) {
                drag->trayPage = DressUpTray_GetItemPage(drag->selected->mode, slot->id, drag->tray->caseTable);
            }
            DressUp_StartTweenToTray(drag, 4, x, y, drag->selected->mode, drag->trayPage);
        }
        DressUpDrag_ClearSelection(drag);
    }
}

void DressUpDrag_OnHeldAccessory(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        DressUpObj_SetPos(drag->selected, (int)gSystem.touchX - drag->grabX, (int)gSystem.touchY - drag->grabY);
    }
}

void DressUpDrag_OnReleaseMon(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        ov41_02248020(drag->canvas, drag->selected);
        DressUpCanvas_RefreshPriorities(drag->canvas);
        DressUp_DisableDragWindow();
        drag->dragging = FALSE;
        PlaySE(SEQ_SE_DP_BOX02);
        DressUpDrag_ClearSelection(drag);
    }
}

// moves the mon (and what is on it), kept inside the canvas
void DressUpDrag_OnHeldMon(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int x;
    int y;
    int width;
    int height;
    int left;
    int top;
    int right;
    int bottom;
    int edgeTop;
    if (drag->selected != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        DressUpObj_GetMargins(drag->selected, &left, &right, &top, &bottom);
        DressUpObj_GetSize(drag->selected, &width, &height);
        x = (int)gSystem.touchX - drag->grabX;
        y = (int)gSystem.touchY - drag->grabY;
        edgeTop = y + top;
        width = width - right;
        height = height - bottom;
        if (x + left <= 138) {
            x = 138 - left;
        } else if (x + width >= 246) {
            x = 246 - width;
        }
        if (edgeTop <= 18) {
            y = 18 - top;
        } else if (y + height >= 143) {
            y = 143 - height;
        }
        DressUp_MoveMonWithAccessories(touch, x, y);
        DressUpCanvas_GetMaxOverflow(drag->canvas, &left, &top);
        DressUp_MoveMonWithAccessories(touch, x + left, y + top);
    }
}

void DressUpDrag_ApplyBackdrop(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        DressUpSlot *slot;
        GF_ASSERT(drag->selected->mode == 1);
        slot = drag->selected->object;
        DressUpCanvas_FreeBackdrop(drag->canvas);
        DressUpCanvas_SetBackdrop(drag->canvas, slot->id, HEAP_ID_14);
    }
}

void DressUpDrag_BeginDrag(DressUpDrag *drag, DressUpNode *node, BOOL fromCanvas, int grabX, int grabY, int priority) {
    int x;
    int y;
    DressUpObj_GetPos(node, &x, &y);
    drag->selected = node;
    drag->originX = x;
    drag->originY = y;
    drag->fromCanvas = fromCanvas;
    drag->grabX = grabX;
    drag->grabY = grabY;
    if (fromCanvas == FALSE) {
        drag->trayPage = DressUpTray_GetGroupPage(drag->tray, drag->selected->mode);
    } else {
        drag->trayPage = 0;
    }
    drag->dragging = TRUE;
    DressUpObj_SetPriority(node, priority);
}

void DressUpDrag_ClearSelection(DressUpDrag *drag) {
    drag->selected = NULL;
    drag->originX = 0;
    drag->originY = 0;
    drag->fromCanvas = FALSE;
    drag->trayPage = 0;
}

void DressUpDrag_GetSelectedRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right) {
    DressUpDrag *drag = touch->drag;
    int width;
    int height;
    int x;
    int y;
    DressUpObj_GetSize(drag->selected, &width, &height);
    DressUpObj_GetPos(drag->selected, &x, &y);
    *top = y;
    *bottom = y + height;
    *left = x;
    *right = x + width;
}

void DressUpDrag_GetSelectedVisibleRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right) {
    DressUpObj_GetVisibleRect(touch->drag->selected, top, bottom, left, right);
}

void DressUp_StartTweenToTray(DressUpDrag *drag, int frames, int destX, int destY, int mode, int page) {
    int x;
    int y;
    DressUpTrayTween *tween = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_TweenToTrayTask, sizeof(DressUpTrayTween), 0, HEAP_ID_13));
    tween->tray = drag->tray;
    tween->node = drag->selected;
    tween->frames = frames;
    tween->destX = destX;
    tween->destY = destY;
    tween->mode = mode;
    tween->page = page;
    tween->paused = &drag->tray->pageSwapActive;
    tween->dragging = &drag->dragging;
    DressUpObj_GetPos(drag->selected, &x, &y);
    tween->stepX = (tween->destX - x) / frames;
    tween->stepY = (tween->destY - y) / frames;
    drag->dragging = FALSE;
}

void DressUp_TweenToTrayTask(SysTask *task, DressUpTrayTween *tween) {
    int x;
    int y;
    if (*tween->paused != TRUE) {
        DressUpObj_GetPos(tween->node, &x, &y);
        x += tween->stepX;
        y += tween->stepY;
        tween->frames--;
        if (tween->frames < 0 || (tween->stepX == 0 && tween->stepY == 0)) {
            DressUpObj_SetPos(tween->node, tween->destX, tween->destY);
            DressUpTray_LinkObjectToPage(tween->tray, tween->mode, tween->page, tween->node);
            DressUpTray_RefreshPriorities(tween->tray);
            if (*tween->dragging == FALSE) {
                DressUp_DisableDragWindow();
            }
            DestroySysTaskAndEnvironment(task);
            return;
        }
        DressUpObj_SetPos(tween->node, x, y);
    }
}

void DressUp_PrintSelectedItemName(DressUpDrag *drag) {
    if (drag->selected->mode == 0) {
        DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0216_bin, ((DressUpSlot *)drag->selected->object)->id);
    } else if (drag->selected->mode == 1) {
        DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0218_bin, ((DressUpSlot *)drag->selected->object)->id);
    }
}

void DressUp_MoveMonWithAccessories(DressUpTouch *touch, int x, int y) {
    DressUpDrag *drag = touch->drag;
    DressUpObj_SetPos(drag->selected, x, y);
    DressUpCanvas_MoveMonGroup(drag->canvas, x - drag->monX, y - drag->monY);
    drag->monX = x;
    drag->monY = y;
}

// the mon's hit map: its picture, unscanned
void DressUp_LoadMonHitMap(DressUpGfx *gfx, const PokepicTemplate *template) {
    void *data = GfGfxLoader_LoadFromNarc((NarcId) template->narcID, template->charDataID, FALSE, HEAP_ID_14, FALSE);
    NNSG2dCharacterData *hitMap = DressUpGfx_StoreUnpackedChar(gfx, data, DRESS_UP_IMAGE_MAX);
    UnscanPokepic(hitMap->pRawData, (NarcId) template->narcID);
}
