#ifndef OVERLAY96_COURSE_TOUCH_STATE_R108_PRIVATE_H
#define OVERLAY96_COURSE_TOUCH_STATE_R108_PRIVATE_H

#include "global.h"

#include "system.h"
#include "touchscreen.h"

typedef struct Ov96FlagState {
    u8 pad00[0x20];
    s32 word;
} Ov96FlagState;

typedef struct Ov96PackedState {
    u8 pad00[0x1C];
    volatile s32 field1C;
    volatile s32 field20;
} Ov96PackedState;

typedef struct Ov96TouchRec {
    u32 unk00;
    u8 unk04;
    u8 unk05;
} Ov96TouchRec;

void *PokeathlonCourse_GetHeapAllocPtr4(void *);
void *PokeathlonCourse_GetDataCopyArea(void *);
void *ov96_021E8A20(void *);
void ov96_021EB63C(void *);
void ov96_021EB144(void *, int);
void ov96_021E65A4(void *);
void ov96_021E8324(void *, void *);
void ov96_02214618(void);
BOOL ov96_021E5F24(void *);
void ov96_021E8228(void *, u8, int, int, int);
void ov96_022148E8(void *);
void ov96_02211F38(void *);
BOOL ov96_02211AF0(void *);
void ov96_0221236C(void *);

#endif
