#ifndef POKEHEARTGOLD_TO52_OV96_R86_GAP3_TAIL_PRIVATE_H
#define POKEHEARTGOLD_TO52_OV96_R86_GAP3_TAIL_PRIVATE_H

#include "global.h"

#define PTR8(p, o) (*(void **)((u8 *)(p) + (o)))

extern u8 gSystem[];

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void *PokeathlonCourse_GetDataCopyArea(void *course);
void PokeathlonCourse_SetField5E0_AtIndex(void *course, u8 index, u16 value);
BOOL IsPaletteFadeFinished(void);
BOOL ov96_021E5F24(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_021E8228(void *course, u8 player, u8 type, u8 index, u32 value);
void ov96_021E8324(void *course, void (*callback)(void));
void ov96_021EB144(void *ptr, int);
void ov96_021EB63C(void *ptr);
void ov96_02207C64(void);
void ov96_02205D30(void *course);
BOOL System_GetTouchNew(void);
BOOL System_GetTouchHeld(void);

int ov96_02205520(void *course, u8 *state);
void ov96_02205AFC(u8 *a, u8 *dest);
BOOL ov96_02205C94(void *course);

#endif
