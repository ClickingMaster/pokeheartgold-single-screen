#ifndef POKEHEARTGOLD_OVY_42_H
#define POKEHEARTGOLD_OVY_42_H

#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "unk_02009D48.h"

typedef struct Field2DPoint {
    s16 unk0;
    s16 unk2;
} Field2DPoint;

typedef struct Field2DCommand {
    u16 unk0;
    u8 unk2;
    u8 unk3;
} Field2DCommand;

typedef struct Field2DMove {
    Field2DPoint unk0;
    u16 unk4;
    u8 unk6;
    u8 unk7;
} Field2DMove;

typedef struct Field2DCommandQueue {
    Field2DCommand *entries;
    u32 capacity;
    u16 head;
    u16 tail;
} Field2DCommandQueue;

typedef struct Field2DBgLayer {
    G2dRenderer *g2dRenderer;
    BgConfig *bgConfig;
    void *screenFile;
    NNSG2dScreenData *nnsg2dScreenData;
    u32 screen;
    u32 bgLayer;
    u16 tileX;
    u16 tileY;
} Field2DBgLayer;

typedef struct Field2DMoveQueueEntry {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Field2DMoveQueueEntry;

typedef struct Field2DMoveQueue {
    Field2DMoveQueueEntry *entries;
    u32 capacity;
    u16 head;
    u16 tail;
} Field2DMoveQueue;

typedef struct Field2DObject {
    u32 active;
    Field2DPoint pos;
    Field2DPoint prevPos;
    u16 id;
    u16 spriteId;
    u16 actionFrame;
    u16 actionDuration;
    u8 direction;
    u8 action;
    u16 stateFrames;
} Field2DObject;

typedef struct Field2DObjectManager {
    Field2DObject *objects;
    u32 capacity;
} Field2DObjectManager;

typedef struct Field2DMap {
    u16 width;
    u16 height;
    u32 *cells;
} Field2DMap;

typedef struct Field2DObjectTemplate {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
} Field2DObjectTemplate;

typedef struct CharSprite {
    u32 *resource;
    Sprite *sprite;
    Sprite *shadow;
    u32 animKind;
    u32 direction;
    s16 animFrames;
    u8 unk16[2];
} CharSprite;

typedef struct Field2DObjectSprite {
    Field2DObject *object;
    CharSprite *charSprite;
    u16 action;
    u16 direction;
    s16 id;
    u16 savedAnimFrame;
    u16 savedAnimKind;
    u8 priorityMode;
    u8 autoUpdate;
} Field2DObjectSprite;

typedef struct CharSpriteResource {
    u32 sharedPalette;
    SpriteResource *charResource;
    SpriteResource *plttResource;
    u32 vramType;
    u32 animSet;
    SpriteResourcesHeader header;
} CharSpriteResource;

typedef struct CharSpriteCellAnim {
    SpriteResource *cell;
    SpriteResource *anim;
    u8 unk8[8];
} CharSpriteCellAnim;

typedef struct CharSpriteManager {
    SpriteList *spriteList;
    PaletteData *paletteData;
    CharSprite *sprites;
    u32 maxSprites;
    CharSpriteResource chars[0x99];
    SpriteResource *cellAnims[4];
    u8 unk2198[0x20];
    CharSpriteCellAnim avatarCellAnims[0x10];
    SpriteResource *shadowResources[4];
    SpriteResourcesHeader shadowHeader;
    u32 shadowDrawPriority;
    GF_2DGfxResMan *resManagers[4];
    SpriteResource *avatarPalette;
    NARC *charNarc;
    NARC *avatarNarc;
} CharSpriteManager;

typedef struct Field2DObjectSpriteManager {
    CharSpriteManager *charSprites;
    Field2DObjectSprite *sprites;
    u16 maxSprites;
    u8 bgPriority;
    u8 vramType;
} Field2DObjectSpriteManager;

typedef struct Field2DMapCell {
    u32 value;
} Field2DMapCell;

typedef struct Field2DBgLayerTemplate {
    u8 unk_0;
    u8 unk_1;
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 bgExtPltt;
    u8 priority;
    u8 mosaic;
    u8 narcId;
    u8 fileId;
    u8 compressed;
} Field2DBgLayerTemplate;

typedef s32 (*Field2DCommandCheckFunc)(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
typedef void (*Field2DObjectStartFunc)(Field2DObject *arg0, Field2DPoint arg2, u32 arg3);
typedef s32 (*Field2DObjectUpdateFunc)(Field2DObject *arg0);
typedef void (*CharSpriteAnimUpdateFunc)(CharSprite *arg0);
typedef void (*CharSpriteAnimStartFunc)(CharSprite *arg0, s32 arg1);

CharSpriteManager *CharSpriteManager_New(SpriteList *arg0, PaletteData *arg1, s32 arg2, enum HeapID heapID);
void CharSpriteManager_Free(CharSpriteManager *arg0);
void CharSpriteManager_LoadChar(CharSpriteManager *arg0, s32 arg1, u8 arg2, s32 arg3, enum HeapID heapID);
void CharSpriteManager_UnloadChar(CharSpriteManager *arg0, s32 arg1);
s32 CharSpriteManager_IsCharLoaded(CharSpriteManager *arg0, s32 arg1);
void CharSpriteManager_UnloadAllChars(CharSpriteManager *arg0);
void CharSpriteManager_LoadAvatarChars(CharSpriteManager *arg0, u8 arg1, s32 arg2, enum HeapID heapID);
void CharSpriteManager_UnloadAvatarChars(CharSpriteManager *arg0);
void CharSpriteManager_LoadShadow(CharSpriteManager *arg0, u8 arg1, s32 arg2, enum HeapID heapID);
void CharSpriteManager_UnloadShadow(CharSpriteManager *arg0);
CharSprite *CharSpriteManager_CreateSprite(CharSpriteManager *arg0, s16 *arg1, s32 arg2, enum HeapID heapID);
void CharSprite_Delete(CharSprite *arg0);
Sprite *CharSprite_GetSprite(CharSprite *arg0);
void CharSprite_SetPosition(CharSprite *arg0, s16 arg1, s16 arg2);
void CharSprite_SetDrawPriority(CharSprite *arg0, u16 arg1);
u16 CharSprite_GetDrawPriority(CharSprite *arg0);
void CharSprite_SetVisible(CharSprite *arg0, u16 arg1);
BOOL CharSprite_IsVisible(CharSprite *arg0);
s32 CharSprite_GetAnimSet(CharSprite *arg0);
u32 CharSprite_GetAnimKind(CharSprite *arg0);
void CharSprite_SetAnimFrame(CharSprite *arg0, u16 arg1);
u16 CharSprite_GetAnimFrame(CharSprite *arg0);
void CharSprite_SetShadowPosition(CharSprite *arg0, s16 arg1, s16 arg2);
void CharSprite_StartAnim(CharSprite *arg0, s32 arg1, s32 arg2);
void CharSprite_UpdateAnim(CharSprite *arg0);
void CharSpriteManager_LoadCellAnims(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID);
void CharSpriteManager_UnloadCellAnims(CharSpriteManager *arg0, s32 arg1);
u16 CharSprite_GetCharIndex(s32 arg0);
s32 CharSprite_IsAnimSetAllowed(u32 arg0, s32 arg1);
s32 CharSprite_GetCellAnimResId(s32 arg0, s32 arg1, s32 arg2);
void CharSpriteManager_LoadCharResources(CharSpriteManager *arg0, s32 arg1, u8 arg2, s32 arg3, enum HeapID heapID, s32 arg5);
void CharSpriteManager_LoadCharWithPalette(CharSpriteManager *arg0, s32 arg1, s32 arg2, s32 arg3, enum HeapID heapID);
void CharSpriteManager_LoadSharedPaletteChar(CharSpriteManager *arg0, s32 arg1, s32 arg2, s32 arg3, enum HeapID heapID);
void CharSpriteManager_UnloadCharResources(CharSpriteManager *arg0, s32 arg1);
void CharSpriteManager_UnloadCharWithPalette(CharSpriteManager *arg0, s32 arg1);
void CharSpriteManager_UnloadSharedPaletteChar(CharSpriteManager *arg0, s32 arg1);
BOOL CharSpriteManager_IsCharIndexLoaded(CharSpriteManager *arg0, s32 arg1);
void CharSpriteManager_LoadShadowResources(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID);
void CharSpriteManager_UnloadShadowResources(CharSpriteManager *arg0);
BOOL CharSprite_HasShadowResources(SpriteResource **spriteResources);
void CharSpriteManager_LoadAvatarPalette(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID);
void CharSpriteManager_UnloadAvatarPalette(CharSpriteManager *arg0);
void CharSpriteManager_LoadAvatarCellAnims(CharSpriteManager *arg0, enum HeapID heapID);
void CharSpriteManager_UnloadAvatarCellAnims(CharSpriteManager *arg0);
CharSprite *CharSpriteManager_GetFreeSprite(CharSpriteManager *arg0);
void CharSprite_StepAnim(CharSprite *arg0);

void CharSprite_StartAnimStand(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimLoop(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimWalk(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimTurn(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimRun(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimBump(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimWalkSlow(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimWalkVeryFast(CharSprite *arg0, s32 arg1);
void CharSprite_StartAnimWalkFast(CharSprite *arg0, s32 arg1);
void CharSprite_UpdateAnimLoop(CharSprite *arg0);
void CharSprite_UpdateAnimWalk(CharSprite *arg0);
void CharSprite_UpdateAnimTurn(CharSprite *arg0);
void CharSprite_UpdateAnimRun(CharSprite *arg0);
void CharSprite_UpdateAnimBump(CharSprite *arg0);
void CharSprite_UpdateAnimWalkSlow(CharSprite *arg0);
void CharSprite_UpdateAnimWalkVeryFast(CharSprite *arg0);
void CharSprite_UpdateAnimWalkFast(CharSprite *arg0);
s32 CharSprite_IsFree(CharSprite *arg0);
void CharSpriteManager_LoadPaletteToBuffer(CharSpriteManager *arg0, SpriteResource *arg1, s32 arg2);
void *Field2DMap_New(u16 arg0, u16 arg1, enum HeapID heapID);
void Field2DMap_Free(Field2DMap *arg0);
u16 Field2DMap_GetWidth(Field2DMap *arg0);
u16 Field2DMap_GetHeight(Field2DMap *arg0);
void Field2DMap_Load(Field2DMap *arg0, const void *arg1);

Field2DMapCell Field2DMap_GetCell(Field2DMap *arg0, u32 arg1, u32 arg2);
s32 Field2DMap_IsBlocked(Field2DMap *arg0, u16 arg1, u16 arg2);
u32 Field2DMap_GetTag(Field2DMap *arg0, u16 arg1, u16 arg2);
Field2DObjectManager *Field2DObjectManager_New(u32 a0, enum HeapID heapID);
void Field2DObjectManager_Free(Field2DObjectManager *arg0);
void Field2DObjectManager_StartMove(Field2DObjectManager *arg0, Field2DMove *arg1);
void Field2DObjectManager_Update(Field2DObjectManager *arg0);
u32 Field2DObjectManager_GetCapacity(Field2DObjectManager *arg0);
Field2DObject *Field2DObjectManager_AddObject(Field2DObjectManager *arg0, Field2DObjectTemplate *arg1);
void Field2DObject_Delete(void *arg0);
Field2DObject *Field2DObjectManager_FindById(Field2DObjectManager *arg0, u8 arg1);
Field2DObject *Field2DObjectManager_GetSlot(Field2DObjectManager *arg0, u16 arg1);
Field2DObject *Field2DObjectManager_GetById(Field2DObjectManager *arg0, u8 arg1);
Field2DObject *Field2DObjectManager_GetByIndex(Field2DObjectManager *arg0, u16 arg1);
s32 Field2DObject_GetParam(Field2DObject *arg0, s32 arg1);
void Field2DObject_SetParam(Field2DObject *arg0, u32 arg1, s32 arg2);
Field2DPoint Field2D_StepInDirection(Field2DPoint arg0, s32 arg1);
u8 Field2D_GetOppositeDirection(s32 arg0);
Field2DObject *Field2DObjectManager_GetFreeSlot(Field2DObjectManager *arg0);
Field2DPoint Field2DObject_GetPos(Field2DObject *arg0);
Field2DPoint Field2DObject_GetPrevPos(Field2DObject *arg0);
Field2DPoint Field2DObject_GetDrawPos(Field2DObject *arg0);
void Field2DObject_SetPos(Field2DObject *arg0, Field2DPoint arg1);
void Field2DObject_SetPrevPos(Field2DObject *arg0, Field2DPoint arg1);
Field2DObject *Field2DObject_FindObjectInDirection(Field2DObject *arg0, Field2DObjectManager *arg1, s32 arg2);
Field2DObject *Field2DObjectManager_FindObjectAt(Field2DObjectManager *arg0, Field2DPoint arg1);
void Field2DObject_StartMove(Field2DObject *arg0, Field2DMove *arg1);
void Field2DObject_Update(Field2DObject *arg0);
void Field2DObject_StartStand(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartTurn(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalk(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartRun(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartPause(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartBump(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkSlow(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkInPlaceVeryFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkInPlaceFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkInPlace(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
void Field2DObject_StartWalkInPlaceSlow(Field2DObject *arg0, Field2DPoint arg1, u32 arg2);
s32 Field2DObject_UpdateStand(Field2DObject *arg0);
s32 Field2DObject_UpdateTurn(Field2DObject *arg0);
s32 Field2DObject_UpdateWalk(Field2DObject *arg0);
s32 Field2DObject_UpdateRun(Field2DObject *arg0);
s32 Field2DObject_UpdatePause(Field2DObject *arg0);
s32 Field2DObject_UpdateBump(Field2DObject *arg0);
s32 Field2DObject_UpdateWalkInPlace(Field2DObject *arg0);
Field2DPoint Field2D_GetStepTarget(Field2DPoint arg0, s32 arg1);
s32 Field2DObject_AdvanceActionFrame(Field2DObject *arg0);
void Field2DObject_EndAction(Field2DObject *arg0);
s32 Field2DObjectManager_TryCommand(Field2DMap *arg0, Field2DObjectManager *arg1, Field2DCommand *arg2, Field2DMove *arg3);

s32 Field2DCommand_CheckStand(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckTurn(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckWalk(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckRun(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckPause(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckBump(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckWalkSlow(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DCommand_CheckWalkInPlace(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4);
s32 Field2DObject_IsMapBlockedInDirection(Field2DObject *arg0, Field2DMap *arg1, u8 arg2);
BOOL Field2DObject_IsObjectInDirection(Field2DObject *arg0, Field2DObjectManager *arg1, u8 arg2);
void Field2DMove_Init(Field2DMove *arg0, Field2DObject *arg1, u16 arg2, u8 arg3, s32 arg4);
Field2DObjectSpriteManager *Field2DObjectSpriteManager_New(SpriteList *arg0, PaletteData *arg1, u16 arg2, s8 arg3, enum HeapID heapID);
Field2DObjectSpriteManager *Field2DObjectSpriteManager_NewWithShadow(SpriteList *arg0, PaletteData *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5, enum HeapID heapID);
void Field2DObjectSpriteManager_Free(Field2DObjectSpriteManager *arg0);
void Field2DObjectSpriteManager_LoadChar(Field2DObjectSpriteManager *arg0, s32 arg1, s32 arg2, enum HeapID heapID);
void Field2DObjectSpriteManager_UnloadChar(Field2DObjectSpriteManager *arg0, s32 arg1);
s32 Field2DObjectSpriteManager_IsCharLoaded(Field2DObjectSpriteManager *arg0, s32 arg1);
void Field2DObjectSpriteManager_UnloadAllChars(Field2DObjectSpriteManager *arg0);
void Field2DObjectSpriteManager_LoadAvatarChars(Field2DObjectSpriteManager *arg0, s32 arg1, enum HeapID heapID);
Field2DObjectSprite *Field2DObjectSpriteManager_CreateSprite(Field2DObjectSpriteManager *arg0, Field2DObject *arg1, u32 arg2, enum HeapID heapID);
void Field2DObjectSprite_Delete(Field2DObjectSprite *arg0);
void Field2DObjectSpriteManager_Update(Field2DObjectSpriteManager *arg0);
void Field2DObjectSprite_Update(Field2DObjectSprite *arg0);
void Field2DObjectSprite_SetAutoUpdate(Field2DObjectSprite *arg0, s8 arg1);
void Field2DObjectSprite_StartLoopAnim(Field2DObjectSprite *arg0);
void Field2DObjectSprite_UpdateAnim(Field2DObjectSprite *arg0);
void Field2DObjectSprite_SyncAnim(Field2DObjectSprite *arg0);
void Field2DObjectSprite_SetPosition(Field2DObjectSprite *arg0, Field2DPoint arg1);
s32 Field2DObjectSprite_IsVisible(Field2DObjectSprite *arg0);
void Field2DObjectSprite_SetVisible(Field2DObjectSprite *arg0, u16 arg1);
u16 Field2DObjectSprite_GetDrawPriority(Field2DObjectSprite *arg0);
void Field2DObjectSprite_FaceDirection(Field2DObjectSprite *arg0, s32 arg1);
u32 Field2DObjectSprite_CalcDrawPriority(s16 arg0, u8 arg1);
u8 Field2DObjectSprite_GetPaletteOffset(Field2DObjectSprite *arg0);
void Field2DObjectSprite_SetShadowPosition(Field2DObjectSprite *arg0, Field2DPoint arg1);
s32 Field2DObjectSprite_IsFree(Field2DObjectSprite *arg0);
Field2DObjectSprite *Field2DObjectSpriteManager_GetFreeSprite(Field2DObjectSpriteManager *arg0);
u8 Field2DObjectSprite_GetAnimKindForAction(CharSprite *arg0, s32 arg1);
void Field2DObjectSprite_GetDrawPos(Field2DObjectSprite *arg0, s16 *arg1, s16 *arg2);
void Field2DObjectSprite_StartAnim(Field2DObjectSprite *arg0, u8 arg1, s32 arg2);
void Field2DCamera_CenterOnObject(Field2DPoint *arg0, Field2DObject *arg1);
void Field2DCamera_Init(Field2DPoint *arg0);
void Field2DCamera_Set(Field2DPoint *arg0, s16 arg1, s16 arg2);
s16 Field2DCamera_GetY(Field2DPoint *arg0);
s16 Field2DCamera_GetX(Field2DPoint *arg0);
Field2DBgLayer *Field2DBgLayer_New(G2dRenderer *g2dRenderer, BgConfig *bgConfig, const Field2DBgLayerTemplate *arg2, enum HeapID heapID);
void Field2DBgLayer_Free(Field2DBgLayer *arg0);
void Field2DBgLayer_SetCamera(Field2DBgLayer *arg0, Field2DPoint *arg1);
void Field2DBgLayer_InitBg(BgConfig *bgConfig, s32 arg1, const Field2DBgLayerTemplate *arg2);
void Field2DBgLayer_Scroll(Field2DBgLayer *arg0, s16 arg1, s16 arg2);
void Field2DBgLayer_CopyVisibleTiles(BgConfig *bgConfig, s32 arg1, NNSG2dScreenData *nnsg2dScreenData, s16 arg3, s16 arg4);
void Field2DBgLayer_CopyTilemapRect(BgConfig *bgConfig, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, void *arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA);
u8 Field2DBgLayer_GetScreenBlockLayout(u8 arg0, u8 arg1);
void *Field2DBgLayer_GetScreenBlock(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, Field2DPoint *arg6);
Field2DCommandQueue *Field2DMoveQueue_New(s32 arg0, enum HeapID heapID);
void Field2DMoveQueue_Free(u32 *arg0);
void Field2DMoveQueue_Push(Field2DMoveQueue *arg0, Field2DMoveQueueEntry *arg1);
s32 Field2DMoveQueue_Pop(Field2DMoveQueue *arg0, Field2DMoveQueueEntry *arg1);
Field2DCommandQueue *Field2DCommandQueue_New(s32 arg0, enum HeapID heapID);
void Field2DCommandQueue_Free(Field2DCommandQueue *arg0);
void Field2DCommandQueue_Push(Field2DCommandQueue *arg0, Field2DCommand *arg1);
u32 Field2DCommandQueue_Pop(Field2DCommandQueue *arg0, Field2DCommand *arg1);

#endif // POKEHEARTGOLD_OVY_42_H
