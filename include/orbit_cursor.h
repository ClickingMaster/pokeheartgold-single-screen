#ifndef POKEHEARTGOLD_ORBIT_CURSOR_H
#define POKEHEARTGOLD_ORBIT_CURSOR_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "sprite_system.h"
#include "sys_task_api.h"

typedef struct OrbitCursorSprite {
    /* 0x00 */ ManagedSprite *sprite;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ int unk_08;
    /* 0x0C */ int unk_0C;
} OrbitCursorSprite; // size 0x10

typedef struct OrbitCursor {
    /* 0x000 */ NARC *narc;
    /* 0x004 */ SpriteSystem *spriteSys;
    /* 0x008 */ SpriteManager *spriteMan;
    /* 0x00C */ PaletteData *plttData;
    /* 0x010 */ int count;
    /* 0x014 */ enum HeapID heapId;
    /* 0x018 */ int resBase;
    /* 0x01C */ int unk_1C;
    /* 0x020 */ fx32 yOffset;
    /* 0x024 */ OrbitCursorSprite sprites[12];
    /* 0x0E4 */ int state;
    /* 0x0E8 */ int unk_E8;
    /* 0x0EC */ int unk_EC;
    /* 0x0F0 */ int active;
    /* 0x0F4 */ SysTask *task;
    /* 0x0F8 */ int unk_F8;
    /* 0x0FC */ int radiusX;
    /* 0x100 */ int radiusY;
    /* 0x104 */ int unk_104;
} OrbitCursor; // size 0x108

OrbitCursor *OrbitCursor_New(int a0, enum HeapID a1, int a2, int a3, fx32 a4, SpriteSystem *a5, SpriteManager *a6, PaletteData *a7);
void OrbitCursor_ReloadPalette(OrbitCursor *p, int palette);
void OrbitCursor_LoadResources(OrbitCursor *p, int palette);
void OrbitCursor_CreateSprites(OrbitCursor *p);
void OrbitCursor_DeleteSprites(OrbitCursor *p);
BOOL OrbitCursor_Start(OrbitCursor *p, int palette);
BOOL OrbitCursor_SetActive(OrbitCursor *p, int active);
BOOL OrbitCursor_SetTarget(OrbitCursor *p, int x, int y);
BOOL OrbitCursor_MoveTarget(OrbitCursor *p, int x, int y);
BOOL OrbitCursor_IsNewTarget(OrbitCursor *p, int x, int y);
BOOL OrbitCursor_SetPosition(OrbitCursor *p, s16 x, s16 y);
BOOL OrbitCursor_Delete(OrbitCursor *p);
BOOL OrbitCursor_SetVisible(OrbitCursor *p, int flag);
void OrbitCursor_SetRadius(OrbitCursor *p, int radiusX, int radiusY);
void OrbitCursor_SetAltAnim(OrbitCursor *p);
void OrbitCursor_SetDefaultAnim(OrbitCursor *p);
extern void OrbitCursor_Task(SysTask *task, void *data);

extern const int gOrbitCursorPaletteMembers[];

typedef char AssertTouchSpriteSize[(sizeof(OrbitCursorSprite) == 0x10) ? 1 : -1];
typedef char AssertTouchMotionSize[(sizeof(OrbitCursor) == 0x108) ? 1 : -1];
typedef char AssertTouchSprites[(offsetof(OrbitCursor, sprites) == 0x24) ? 1 : -1];
typedef char AssertTouchState[(offsetof(OrbitCursor, state) == 0xE4) ? 1 : -1];
typedef char AssertTouchTask[(offsetof(OrbitCursor, task) == 0xF4) ? 1 : -1];

#endif
