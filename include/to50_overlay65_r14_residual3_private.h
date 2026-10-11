#ifndef TO46_R2D_OVERLAY65_PRIVATE_H
#define TO46_R2D_OVERLAY65_PRIVATE_H

#include "global.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "pokemon.h"
#include "sprite.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))

extern int Party_GetCount(void *party);
extern u32 GetMonIconNaixEx(u32 species, u32 isEgg, u32 form);
extern void ov65_0221BFBC(void *charData, u32 species, u32 gender, u32 form, u32 selection, void *resource);
extern void ov65_0221F864(void *work);
extern void sub_02037AC0(int);
extern void sub_0201A728(int);
extern void ov65_0221F760(void *work);
extern BOOL sub_02037B38(int);
extern int Comm_GetLocalNetId(void);
extern u32 LCRandom(void);
extern void ov65_0221DE10(int, int, int);
extern void ov65_0221DE8C(void *);
extern void ov65_0221F850(void *);
extern void sub_02039EAC(void *);
extern BOOL sub_02039EB4(void *, int, void *);
extern void sub_020378E4(int);
extern void ov65_0221DE64(int, void *, int);
extern int sub_02034818(int);
extern void ov65_0221DEA0(int, void *, void *);
extern void ov65_0221DF0C(void *);
extern void ov65_0221C1C4(void *);
extern void StartBrightnessTransition(int, int, int, int, int);
extern BOOL IsBrightnessTransitionActive(int);
extern void ov65_0221F780(void *);
extern void ClearFrameAndWindow2(void *, int);
extern void ov65_0221FB90(void *, int, int, void *, void *);
extern void sub_0201A738(int);
BOOL ov65_0221C5E0(void *work);

typedef struct Ov65MonSummary {
    u16 species;
    u16 mail;
    u8 gender;
    u8 language;
    u8 form;
    u8 padding;
    u16 unk8;
    u16 unka;
    u32 personality;
} Ov65MonSummary;

typedef struct Ov65IconRecordR3 {
    u16 species;
    u16 heldItem;
    u8 unk_04;
    u8 form;
    u8 gender;
    u8 unk_07;
    u32 unk_08;
    void *extra;
} Ov65IconRecordR3;

typedef struct Ov65WorkR3 {
    u8 filler_000[0x37C];
    Sprite *mainSprites[12];
    Sprite *itemSprites[12];
    Sprite *extraSprites[12];
    u8 filler_40C[0x69C - 0x40C];
    Ov65IconRecordR3 records[12];
    u8 filler_75C[0x79C - 0x75C];
    void *allocations[12];
    NNSG2dCharacterData *charData[12];
} Ov65WorkR3;

void ov65_0221C46C(void *party, int start, void *work);

extern BOOL ItemIdIsMail(u16 itemId);
extern const u32 ov65_0221FEEC[];
extern void *Party_GetMonByIndex(void *party, int index);
extern void *ov65_0221D57C(int side, void *mon, void *a2, void *a3);
extern void ov65_0221D674(void *a0, int side, void *party, int index, void *work);
extern void ov65_0221D8C4(void *a0, int side, void *work);
extern BOOL ov65_0221DDC0(void *a0, void *state, void *sprite, void *summaries, int side);
extern void ov65_0221DCBC(void *state);
extern void ov65_0221DE24(void *work, int message, int selection);
void ov65_0221C9D8(Pokemon *mon, Ov65MonSummary *summary);
void ov65_0221CA64(void *work, int index, int form);
void ov65_0221CADC(void *work, int selection);
void ov65_0221CB5C(void *work);
int ov65_0221CC0C(void *work);

#endif
