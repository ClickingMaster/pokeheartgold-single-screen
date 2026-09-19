#ifndef W18_R3_LANDING_PRIVATE_H
#define W18_R3_LANDING_PRIVATE_H

#include "global.h"

#define W18_U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define W18_PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void *PokeathlonCourse_GetDataCopyArea(void *);
void *PokeathlonCourse_GetHeapAllocPtr4(void *);
void *ov96_021E8A20(void *);
u32 ov96_021E5F24(void *);
BOOL ov96_021EB57C(void *);
void ov96_021EB564(void *, int);
void ov96_021F5EC4(void *);

#endif
