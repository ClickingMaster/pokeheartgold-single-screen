#ifndef TO55_T03_PRIVATE_H
#define TO55_T03_PRIVATE_H

#include "global.h"

void ov13_02223EA4(u32, int);
void ov13_02223ED4(u32);
int SOC_SendTo(int, const void *, int, int, const void *);
u32 SOCL_GetHostID(void);
void SOC_U32to4U8(u32, void *);
int ov13_022245C4(int, const void *, const void *, const void *, int);
int ov13_022245DC(int, const void *, int);
int ov13_02224618(int, int, const void *, int);

#endif
