#ifndef TO52_R5_W19_PRIVATE_H
#define TO52_R5_W19_PRIVATE_H
#include "global.h"

typedef struct SpriteTemplateR5W19 {
    u16 x;
    u16 y;
    u16 z;
    u16 arg5;
    u32 arg6;
    u32 rest[10];
} SpriteTemplateR5W19;

typedef struct RecordR5W19 {
    u8 a;
    u8 pad1[2];
    u8 b;
    u8 c;
} RecordR5W19;

typedef union PackedR5W19 {
    u32 raw;
    struct {
        u32 low : 9;
        u32 target : 9;
        u32 current : 8;
        u32 top : 6;
    } bits;
} PackedR5W19;

extern const SpriteTemplateR5W19 ov96_0221CEB8;
void GF_AssertFail(void);
void *SpriteSystem_NewSpriteWithYOffset(void *, void *, SpriteTemplateR5W19 *, u32);
void ManagedSprite_SetAnimateFlag(void *, int);
void ReadWholeNarcMemberByIdPair(void *, u32, u32);
RecordR5W19 *ov96_021E60D8(void *, u32, u32);
void *ov96_0220D1A0(void *, void *, u16, u16, u16, u16);
void ov96_0220D200(PackedR5W19 *, u32, void *);
void ov96_0220D2AC(PackedR5W19 *, u32, u8);

#endif
