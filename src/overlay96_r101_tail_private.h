#ifndef TO45_OVERLAY96_R102_GAP1_PRIVATE_H
#define TO45_OVERLAY96_R102_GAP1_PRIVATE_H

#include <nitro/fx/fx_vec.h>

#include "global.h"
#include "system.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov96ParticipantView {
    u16 species;
    u16 field2;
    u16 special;
    u8 kind;
    u8 field7;
    u32 participant;
    u32 fieldC;
} Ov96ParticipantView;

typedef struct Ov96Object {
    void *course;
    void *sprite;
    void *data;
    u32 state;
    VecFx32 position;
    VecFx32 previous;
    VecFx32 target;
    VecFx32 velocity;
    u32 flags;
    u8 field44;
    u8 row;
    u8 column;
    u8 field47;
} Ov96Object;

typedef union Ov96GroupFlags {
    u32 raw;
    struct {
        u32 field00 : 14;
        u32 flag14 : 1;
        u32 flag15 : 1;
        u32 counter : 13;
        u32 flag29 : 1;
        u32 group : 2;
    } bits;
} Ov96GroupFlags;

typedef struct Ov96ObjectGroup {
    s16 x;
    s16 y;
    Ov96GroupFlags flags;
    Ov96Object objects[3];
    u32 padE0;
} Ov96ObjectGroup;

typedef struct Ov96ClearEntry {
    u32 flags;
    u8 pad04[0x44];
} Ov96ClearEntry;

typedef struct Ov96ClearView {
    u8 pad00[0x48];
    Ov96ClearEntry entry[3];
} Ov96ClearView;

typedef struct Ov96ObjectRow {
    Ov96Object objects[3];
    u32 flags;
    u8 padDC[8];
} Ov96ObjectRow;

typedef struct Ov96Work {
    u32 heapId;
    void *course;
    u8 pad08[8];
    void *spriteManager;
    u8 pad14[8];
    void *field1C;
    void *objectManager;
    u8 pad24[0xA8];
    u8 occupancy[12][12];
    u8 pad15C[8];
    Ov96ObjectRow rows[4];
} Ov96Work;

typedef struct Ov96Mini {
    u8 pad[6];
    u8 type6;
    u8 pad7;
    u8 type8;
} Ov96Mini;

void *SpriteManager_GetSpriteList(void *);
void *PokeathlonCourseMonObjects_Create(u32, u32, u32, void *, void *);
void PokeathlonCourseObjects_ShiftLargeUp(void *);
void PokeathlonCourse_GetMonInfo(void *, int, int, Ov96ParticipantView *);
void *PokeathlonCourse_GetPerformance(void *, int, int);
int ov96_021E6108(const Ov96Mini *);
void PokeathlonCourseMonObjects_Load(void *, int, Ov96ParticipantView *, u32 *, u32, u32);
void ReadWholeNarcMemberByIdPair(void *, u32, u32);
void GF_AssertFail(void);
void ov96_0220F280(void *);
void *PokeathlonCourseMonObjects_Get(void *, u8);
void PokeathlonCourseMonObject_SetDrawFlag(void *, u32);
int ov96_021E6138(const Ov96Mini *);
void PokeathlonCourseObject_SetCollision(void *, u32, u32);
void *PokeathlonCourseMonObject_GetGfx(void *);
void *PokeathlonCourseMonGfx_GetInfo(void *);
void PokeathlonDiscCatch_SetMonScaleCenter(void *, u32);
void PokeathlonCourseObject_SetScale(void *, f32, f32);
void PokeathlonCourseMonObject_SetAnimState(void *, u32);
void PokeathlonCourseObject_SetGroundPosition(void *, s32, s32);
u32 ov96_021E6104(void);
void PokeathlonCourseObject_SetTouchRadius(void *, u32);
void PokeathlonCourseObject_GroundToCenter(void *, s32, s32, s32 *, s32 *);
u32 ov96_021E5F24(void *);
void *PokeathlonCourse_AddMarker(void *, void *, void *, void *, u32);
void Sprite_SetDrawPriority(void *, u32);
void PokeathlonCourseMonObject_SetDrawPriority(void *, u32);
void *PokeathlonCourse_GetDataCopyArea(void *);
u8 *ov96_021E8A20(void *ptr);
void PokeathlonCourse_CreateStartSprites(void *, int, void *, void *, u32, u32, void *);
void *PokeathlonCourse_GetHeapAllocPtr4(void *);
BOOL PokeathlonDiscCatchMon_IsInPlay(Ov96Object *);
BOOL PokeathlonDiscCatchMon_IsTouching(Ov96Object *, Ov96Object *);
void PokeathlonDiscCatch_ClampVelocity(s32 *, s32);
u32 PokeathlonDiscCatch_GetFacing(const VecFx32 *);
int PokeathlonDiscCatch_FindTouchedMon(void *, s16, s16);
void PokeathlonDiscCatchMon_SetTarget(Ov96Object *, const s16 *);
BOOL PokeathlonDiscCatchMon_IsOffField(Ov96Object *, fx32, fx32);
void PokeathlonDiscCatchMon_Jump(Ov96Object *);
void PokeathlonDiscCatchMon_Update(Ov96Object *, void *);
void PokeathlonCourseObject_CenterToGround(void *, s32, s32, s32 *, s32 *);
void *ov96_0220F378(void *, u32, u32);

extern const s16 ov96_0221CFBC[][3][2];

void ov96_0220F3FC(Ov96Work *);
void ov96_0220F4A0(Ov96Work *);
void ov96_0220F710(void *);
void ov96_0220F8C8(Ov96ObjectGroup *, void *);
void ov96_0220FA18(Ov96Object *, void *);

#endif

void *PokeathlonCourse_GetMonStats(void *, int, int);
void PokeathlonDiscCatch_StopAnims(void *);
void PokeathlonCourse_StopMarkerAnims(void *);
void PokeathlonDiscCatch_EndGame(void *);
BOOL System_GetTouchNew(void);
BOOL System_GetTouchHeld(void);
void PokeathlonCourse_AddMonCounter(void *, u8, int, int, int);
void ov96_0221013C(void *, void *);
extern struct System gSystem;
u32 ov96_0220F1CC(void *);
extern float _fflt(int);
extern float _fadd(float, float);
extern float _fsub(float, float);
extern int _ffix(float);
extern int _s32_div_f(int, int);
