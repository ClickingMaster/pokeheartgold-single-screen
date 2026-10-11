#ifndef TO45_OV96_R71_U3_PRIVATE_H
#define TO45_OV96_R71_U3_PRIVATE_H

#include "global.h"

#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PAT(p, o)   (*(void **)((u8 *)(p) + (o)))

typedef struct Ov96R71Entry {
    u8 pad00[0xE0];
    u16 value;
    u8 padE2[6];
    u8 active;
} Ov96R71Entry;

typedef struct Ov96R71Work {
    u8 pad00[0x230];
    u32 value230;
} Ov96R71Work;

typedef struct Ov96R71Sort12 {
    struct Ov96R71Sort12 *prev;
    struct Ov96R71Sort12 *next;
    u16 key;
    u16 index;
} Ov96R71Sort12;

typedef struct Ov96R71Node {
    struct Ov96R71Node *prev;
    struct Ov96R71Node *next;
    u16 h8;
    u16 hA;
    u16 hC;
    u16 hE;
    u32 w10;
} Ov96R71Node;

typedef struct Ov96R71NodeList {
    Ov96R71Node head;
    Ov96R71Node nodes[12];
} Ov96R71NodeList;

typedef union Ov96R71NodeUnion {
    Ov96R71Node all[13];
    Ov96R71NodeList named;
} Ov96R71NodeUnion;

extern void GF_AssertFail(void);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern void ScheduleBgTilemapBufferTransfer(void *, u32);
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern u32 PokeathlonCourse_GetParticipantCount(void *);
extern u32 ov96_021E5F24(void *);
extern u8 *ov96_021E8A20(void *ptr);
extern void ov96_021FC2B4(void *, u32);
extern void ov96_021FC248(void *, u32, u32);
extern void ov96_021FC28C(void *, u32, u32);
extern void ov96_021FC2E0(void *, u32, u32);
extern void *PokeathlonCourseSpritePoolSprite_GetSprite(void *);
extern void PokeathlonCourseSpritePoolSprite_Show(void *, u32, u32);
extern void *Sprite_GetMatrixPtr(void *);
extern void Sprite_SetMatrix(void *, const VecFx32 *);
extern void *Sprite_GetScalePtr(void *);
extern void Sprite_SetAffineScale(void *, const VecFx32 *);
extern void Sprite_SetAnimCtrlSeq(void *, u32);
extern void ov96_021FB994(void *);
extern u32 ov96_021FB514(Ov96R71Node *, Ov96R71Node *);
extern u32 ov96_021FB56C(Ov96R71Node *, Ov96R71Node *);
extern void ov96_021FB5C8(u8 *, u8, u8);

void ov96_021FAB24(void *, void *);
void ov96_021FAF1C(void *);
void ov96_021FAFFC(void *, u8 *);
void ov96_021FB0F4(void *);
void ov96_021FB400(void *);

#endif
