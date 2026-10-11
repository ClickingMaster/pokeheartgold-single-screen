#ifndef POKEHEARTGOLD_WIFI_SETUP_UI_H
#define POKEHEARTGOLD_WIFI_SETUP_UI_H

#include "global.h"

// UI framework of the access-point setup screens (ARM code in overlay 13): heap, file loading,
// linked lists and free-item pools, OAM and character VRAM allocators, brightness fades, text
// canvases, pad/touch input and a prioritised task scheduler.

typedef struct WifiSetupListNode {
    struct WifiSetupListNode *prev;
    struct WifiSetupListNode *next;
} WifiSetupListNode;

// Doubly linked list with sentinel head and tail nodes.
typedef struct WifiSetupList {
    WifiSetupListNode head;
    WifiSetupListNode tail;
} WifiSetupList;

// Ring buffer of free items: WifiSetupPool_Pop hands one out, WifiSetupPool_Push returns it.
typedef struct WifiSetupPool {
    u16 size;
    u8 head;
    u8 tail;
    void *items[1];
} WifiSetupPool;

// A run of consecutive OAM entries, kept sorted in a list per screen.
typedef struct WifiSetupOamBlock {
    WifiSetupListNode node;
    GXOamAttr *oam;
    u8 count;
} WifiSetupOamBlock;

typedef struct WifiSetupPoint {
    u16 x;
    u16 y;
} WifiSetupPoint;

typedef struct WifiSetupRect {
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
} WifiSetupRect;

typedef void (*WifiSetupTaskFunc)(void *task, void *arg);

typedef struct WifiSetupTask {
    WifiSetupListNode node;
    WifiSetupTaskFunc func;
    void *arg;
    u8 priority;
    u8 ownsArg; // arg is freed together with the task
} WifiSetupTask;

typedef struct WifiSetupFontInfo {
    u8 unk_00[7];
    u8 flowDirection;
} WifiSetupFontInfo;

typedef struct WifiSetupFontResource {
    u8 unk_00[2];
    u16 alternateChar;
    u8 unk_04[4];
    WifiSetupFontInfo *info;
} WifiSetupFontResource;

typedef struct WifiSetupFont {
    WifiSetupFontResource *res;
    void *unk_04;
} WifiSetupFont;

typedef struct WifiSetupCharWidths {
    s8 left;
    u8 glyphWidth;
    s8 charWidth;
} WifiSetupCharWidths;

typedef struct WifiSetupCanvasVtbl {
    void (*drawChar)(void);
    void (*clear)(void *charCanvas, int color);
} WifiSetupCanvasVtbl;

typedef struct WifiSetupTextCanvas {
    void *charCanvas;
    WifiSetupFont *font;
    int hSpace;
    int vSpace;
} WifiSetupTextCanvas;

// Character canvas (BG screen or OBJ) with the text canvas drawing into it.
typedef struct WifiSetupCanvas {
    u8 charCanvas[0x14];
    WifiSetupCanvasVtbl *vtable;
    WifiSetupTextCanvas textCanvas;
    void *charBuf; // BG: character buffer; OBJ: character VRAM block

    union {
        void *task; // BG: transfer task

        struct {
            u16 charNo;
            u8 width;
            u8 height;
        } obj;
    } u;
} WifiSetupCanvas;

// The "rom:/dwc/utility.bin" archive (WifiSetupArchive_Load).
typedef struct WifiSetupArchiveState {
    void *tables;            // FS archive tables
    u8 poolItems[0x80];      // 0x04, items of filePool
    WifiSetupPool *filePool; // 0x84
    FSArchive arc;           // 0x88
    u8 unk_88[0xE4 - 0x88 - sizeof(FSArchive)];
    u16 lockId; // 0xE4
} WifiSetupArchiveState;

void WifiSetupArchive_Load(void);
void WifiSetupArchive_Unload(void);
void *WifiSetupFile_Load(const char *path, u32 *outSize, int align);
void WifiSetupFile_Free(void *data);
BOOL WifiSetup_HasSameSuffix(const char *a, const char *b, int len);

void WifiSetupOamBlock_InitManager(void);
void WifiSetupOamBlock_FreeManager(void);
WifiSetupOamBlock *WifiSetupOamBlock_Alloc(int screen, int count, int fromFront);
void WifiSetupOamBlock_Free(WifiSetupOamBlock *block);
GXOamAttr *WifiSetupOamBlock_GetOam(WifiSetupOamBlock *block, int index);
u8 WifiSetupOamBlock_GetCount(WifiSetupOamBlock *block);
void WifiSetupOamBlock_SetAffineMode(WifiSetupOamBlock *block, int index, u32 mode, u32 param);
void WifiSetupOamBlock_SetModeAndPalette(WifiSetupOamBlock *block, int index, u32 mode, u32 palette);
void WifiSetupOamBlock_SetPosition(WifiSetupOamBlock *block, int index, int x, int y);
void WifiSetupOamBlock_SetPriority(WifiSetupOamBlock *block, int index, u32 priority);
void WifiSetupOamBlock_GetPosition(WifiSetupOamBlock *block, int index, int *x, int *y);

void WifiSetupOamTemplate_Load(int slot, const char *path);
void WifiSetupOamTemplate_Free(int slot);
void WifiSetupOamTemplate_Copy(int slot, int index, GXOamAttr *dst);
GXOamAttr *WifiSetupOamTemplate_AllocOam(int slot, int index);
WifiSetupOamBlock *WifiSetupOamTemplate_AllocBlock(int slot, int index, int fromFront);

WifiSetupPool *WifiSetupPool_New(int capacity);
WifiSetupPool *WifiSetupPool_NewWithItems(int count, void *items, int stride);
void WifiSetupPool_Delete(WifiSetupPool *pool);
void *WifiSetupPool_Push(WifiSetupPool *pool, void *item);
void *WifiSetupPool_Pop(WifiSetupPool *pool);

void WifiSetupFade_Init(void);
void WifiSetupFade_Free(void);
u8 WifiSetupFade_IsActive(int screen);
int WifiSetupFade_Start(int kind, int screen, int planes, int duration);
int WifiSetupFade_StartDelay(int duration);

void WifiSetupText_Init(void);
void WifiSetupText_Free(void);
WifiSetupCanvas *WifiSetupText_AllocObjCanvas(int screen, int width, int height, int fromFront, u32 *numObjs, int font);
void WifiSetupText_FreeObjCanvas(WifiSetupCanvas *canvas);
WifiSetupCanvas *WifiSetupText_InitBgCanvas(int screen, int font);
void WifiSetupText_FreeBgCanvas(int screen);
void WifiSetupText_Draw(WifiSetupCanvas *canvas, int x, int y, int color, u32 flags, const u16 *text);
void WifiSetupText_DrawChar(void *charCanvas, int x, int y, int color, u16 ch, int font);
void WifiSetupText_DrawMonospace(void *charCanvas, int x, int y, int color, int advance, const u16 *text, int font);
void WifiSetupText_DrawInRect(WifiSetupCanvas *canvas, int x, int y, int width, int height, int color, u32 flags, const u16 *text);
void WifiSetupText_Clear(WifiSetupCanvas *canvas, int color);
void WifiSetupText_ShowObjCanvas(WifiSetupCanvas *canvas, int x, int y, WifiSetupOamBlock *block, u32 priority);
void WifiSetupText_MarkDirty(WifiSetupCanvas *canvas);

void WifiSetup_SetPoint(u16 x, u16 y, WifiSetupPoint *point);
void WifiSetup_SetRect(u16 left, u16 top, u16 right, u16 bottom, WifiSetupRect *rect);
void WifiSetup_RectFromPosSize(const WifiSetupPoint *pos, const WifiSetupPoint *size, WifiSetupRect *rect);
void WifiSetupDisplay_ShowPlanes(int screen, int planes);
void WifiSetupDisplay_HidePlanes(int screen, int planes);
void WifiSetupDisplay_SetWindowRect(int screen, int window, WifiSetupRect *rect);
void WifiSetupDisplay_SetWindowPlanes(int screen, int window, int planes, BOOL effect);

void WifiSetupHeap_Init(void *mem);
void WifiSetupHeap_Destroy(void);
void *WifiSetupHeap_Alloc(u32 size, int align);
void *WifiSetupHeap_AllocZeroed(u32 size, int align);
void WifiSetupHeap_FreeAndClear(void *ptr);
void WifiSetupHeap_Free(void *mem);

void WifiSetupInput_Init(void);
void WifiSetupInput_Free(void);
void WifiSetupInput_Update(void);
BOOL WifiSetupInput_IsKeyPressed(u16 mask);
BOOL WifiSetupInput_IsKeyRepeated(u16 mask);
BOOL WifiSetupInput_IsKeyReleased(u16 mask);
BOOL WifiSetupInput_IsTouchHeld(WifiSetupRect *rect);
BOOL WifiSetupInput_IsTouchPressed(WifiSetupRect *rect);
BOOL WifiSetupInput_IsTouchRepeated(WifiSetupRect *rect);
BOOL WifiSetupInput_IsTouchReleased(WifiSetupRect *rect);
BOOL WifiSetupInput_IsTouchPressedInBox(WifiSetupRect *box);
BOOL WifiSetupInput_GetTouchPos(WifiSetupPoint *out);
void WifiSetupInput_UpdateLcdPower(void);

void WifiSetup_InstallVBlankIntr(void);
void WifiSetup_RestoreVBlankIntr(void);

WifiSetupList *WifiSetupList_New(void);
void WifiSetupList_Delete(WifiSetupList *list);
void WifiSetupList_Remove(void *node);
void WifiSetupList_InsertBefore(void *pos, void *node);
void WifiSetupList_Append(WifiSetupList *list, void *node);
void WifiSetupList_Prepend(WifiSetupList *list, void *node);

void WifiSetupOam_Init(void);
void WifiSetupOam_Free(void);
GXOamAttr *WifiSetupOam_Alloc(int screen);
void WifiSetupOam_Release(GXOamAttr *oam);
GXOamAttr *WifiSetupOam_Get(int screen, int index);

void WifiSetupCharVram_Init(void);
void *WifiSetupCharVram_Alloc(int screen, int size, int fromFront, u32 *outCharNo);
void WifiSetupCharVram_Free(void *block);

int WifiSetup_StrNLen(const u8 *str, int max);
void WifiSetup_SaveVramBanks(void);
void WifiSetup_RestoreVramBanks(void);
void WifiSetup_ClearVram(void);
void ov13_022422E0(void);
void ov13_0224230C(void);

void WifiSetupTask_InitScheduler(void);
void WifiSetupTask_FreeScheduler(void);
void WifiSetupTask_RunQueue(int queue);
void *WifiSetupTask_Create(int queue, WifiSetupTaskFunc func, void *arg, u32 priority);
WifiSetupTask *WifiSetupTask_CreateEx(int queue, WifiSetupTaskFunc func, void *arg, u32 priority, u8 ownsArg);
void WifiSetupTask_SetFunc(WifiSetupTask *task, WifiSetupTaskFunc func);
void WifiSetupTask_DestroyLater(int queue, void *task);
void WifiSetupTask_Destroy(int queue, void *task);
void WifiSetupTask_SetQueueEnabled(int queue, u8 enabled);

#endif // POKEHEARTGOLD_WIFI_SETUP_UI_H
