#ifndef TO53_SOL_R2_H02_PRIVATE_H
#define TO53_SOL_R2_H02_PRIVATE_H

#include "global.h"

void CopyToBgTilemapRect(void *, u8, u8, u8, u8, u8, const void *, u8, u8, u8, u8);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ov43_0222C7F8(void *, void *, void *);
void ov43_0222C918(void *, void *, void *, void *, void *, u8 *, int, int);
void ov43_0222C844(void *, void *, int);

void ov43_0222C378(void *, void *, void *, int, int, int, int);
void ov43_0222C3E8(void *, void *, int);
void ov43_0222C46C(void *, void *, void *, int);
void ov43_0222C4B8(void *, void *, int);

#endif
