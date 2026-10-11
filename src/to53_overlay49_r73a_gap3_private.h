#ifndef TO53_R4_W11_PRIVATE_H
#define TO53_R4_W11_PRIVATE_H

#include "global.h"

void WiFiPlazaObj_GetPosition(void *object, VecFx32 *position);
void ModelInstance_SetPosition(void *object, fx32 x, fx32 y, fx32 z);
void ov49_0226747C(void *state);
void ov49_02267674(void *state);
u32 _fflt(s32 value);
u32 _fadd(u32 lhs, u32 rhs);
s32 _ffix(u32 value);

#endif
