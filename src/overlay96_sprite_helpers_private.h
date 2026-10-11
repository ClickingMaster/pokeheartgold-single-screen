#ifndef OV96_R31_PRIVATE_H
#define OV96_R31_PRIVATE_H
#include "global.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "heap.h"
#include "unk_0200A090.h"
#include "unk_02009D48.h"
#include <nitro/mi/memory.h>

typedef struct Ov96R31Rec { u16 active; u16 shown; Sprite *sprite; u16 kind; u16 id; } Ov96R31Rec;
typedef struct Ov96R31Config { s32 capacity; s32 resourceCount; u8 managerCounts[4]; } Ov96R31Config;
typedef struct Ov96R31Work { u32 heap; s32 used; Ov96R31Config config; void *spriteList; u8 pad18[0x128]; void *resources[4]; u8 *resourceRecords; Ov96R31Rec *records; } Ov96R31Work;
extern void GF_AssertFail(void);
extern void *PokeathlonCourseMonObjects_Get(void *, int);
extern void *PokeathlonCourseMonObject_GetGfx(void *);
extern void *PokeathlonCourseMonGfx_GetSprite(void *);
extern void **PokeathlonCourseSpritePool_GetResourceSlot(Ov96R31Work *, int, int);
extern Sprite *PokeathlonCourseSpritePool_CreateSprite(Ov96R31Work *, int, int, int, u8);
extern Ov96R31Rec *PokeathlonCourseSpritePool_Take(Ov96R31Work *, int, int);
Ov96R31Work *PokeathlonCourseSpritePool_New(enum HeapID, const Ov96R31Config *);
void PokeathlonCourseSpritePool_Free(Ov96R31Work *);
void PokeathlonCourseObjects_PauseAnims(Ov96R31Work *, BOOL);
void PokeathlonCourseSpritePool_LoadChar(Ov96R31Work *, NarcId, int, int, int);
void PokeathlonCourseSpritePool_LoadPltt(Ov96R31Work *, NarcId, int, int, int, u8);
void PokeathlonCourseSpritePool_LoadCell(Ov96R31Work *, NarcId, int, int);
void PokeathlonCourseSpritePool_LoadAnim(Ov96R31Work *, NarcId, int, int);
void PokeathlonCourseSpritePool_SetResourceId(Ov96R31Work *, int, void *);
void PokeathlonCourseSpritePool_TransferResources(Ov96R31Work *);
Ov96R31Rec *PokeathlonCourseSpritePool_CreateAndTake(Ov96R31Work *, int, int, int, u8);
Ov96R31Rec *PokeathlonCourseSpritePool_Take(Ov96R31Work *, int, int);
void PokeathlonCourseSpritePoolSprite_Show(Ov96R31Rec *, BOOL, BOOL);
#endif
