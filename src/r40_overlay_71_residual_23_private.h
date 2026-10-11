#ifndef SOL_R40_NIGHT_OVERLAY71_RESIDUAL23_PRIVATE_H
#define SOL_R40_NIGHT_OVERLAY71_RESIDUAL23_PRIVATE_H
#include "global.h"
void SysTask_SetFunc(void *, void (*)(void *, void *));
void ov71_0224903C(void *);
void TradeSequence_SetModelPosition(void *, void *);
void TradeSequence_SetModelRotation(void *, void *);
int TradeSequence_IsModelInView(void *);
void ov71_022492FC(void *, void *);
void ov71_02249358(void *, void *);
void ov71_022493B8(void *, void *);
void ov71_02249414(void *, void *);
void ov71_02249460(void *, void *);
void *SysTask_GetData(void *);
void TradeSequence_SetCameraPerspectiveType(void *, int);
void TradeSequence_SetCameraAngle(void *, void *);
void ov71_02248FC8(void *, int, int, int);
void ov71_02249004(void *, int, int, int);
void TradeSequence_SetModelScale(void *, int);
void TradeSequence_SetModelVisible(void *, int);
void ov71_0224926C(void *);
#endif
