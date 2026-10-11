#ifndef OVERLAY96_COURSE_COLLISION_UPDATE_MIN_INTERNAL_H
#define OVERLAY96_COURSE_COLLISION_UPDATE_MIN_INTERNAL_H

#include "global.h"

typedef struct Ov96CourseCollisionCounter {
    u8 padding_00[0x20];
    u16 unk20;
    u8 padding_22[2];
    u16 unk24;
} Ov96CourseCollisionCounter;

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern u32 ov96_021E5F24(void *course);
extern void ov96_021F6600(void *course, void *work);
extern void ov96_021F6798(void *course, u8 *work, int slot);
extern void ov96_021EB0A4(void *object, s32 x, s32 y, s32 *outY, s32 *outX);
extern void ov96_021EAF78(void *object, s32 y, s32 x, s32 *outX, s32 *outY, s32 *outRadius);
extern void *ov96_021F75E0(void *manager, u8 index);
extern void *ov96_021F75D4(void *manager, u8 index);
extern BOOL PokeathlonLampJump_CheckCollision(const VecFx32 *position, fx32 radius, const VecFx32 *other, fx32 otherRadius);
extern void ov96_021F75BC(void *manager, u8 index);
extern u8 ov96_021F65D8(Ov96CourseCollisionCounter *counter);
extern void ov96_021F75E8(void *manager, u8 index, u8 value);
extern void PlaySEOnPlayer(u32 seqNo, u32 playerNo);
extern void GF_SndSetTrackPitchByPlayerNo(u32 playerNo, u32 volume, u32 value);
extern void ov96_021E8228(void *course, u8 participant, u8 slot, u32 event, u32 value);

extern const u8 ov96_0221DC18[];
extern const u32 ov96_0221C110[];

#endif
