#ifndef TO53_OVERLAY43_PALPAD_LIST_HEAD_PRIVATE_H
#define TO53_OVERLAY43_PALPAD_LIST_HEAD_PRIVATE_H

#include "global.h"

extern const s16 ov43_0222EFA0[];
extern const s16 ov43_0222EFA2[];

void CopyToBgTilemapRect(void *, int, int, int, int, int, const void *, int, int, int, int);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ov43_0222CD3C(void *, void *, void *, int);

void ov43_0222CB34(s16 *, void *, void *, int);
void ov43_0222CBAC(void *, void *, void *, int);

#endif
