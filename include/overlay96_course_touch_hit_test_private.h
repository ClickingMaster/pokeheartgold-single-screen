#ifndef POKEHEARTGOLD_OVERLAY96_COURSE_TOUCH_HIT_TEST_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY96_COURSE_TOUCH_HIT_TEST_PRIVATE_H

#include "global.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void GF_AssertFail(void);
void PokeathlonCourseObject_GroundToCenter(void *, int, int, int *, int *);
BOOL PokeathlonCourseObject_IsInTouchRange(void *, int, int, int, int);

BOOL ov96_021FDE08(u8 *, int, int, int);

#endif
