#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u16 cacheCount, reserved; BOOL translateOnly; void *renderer; s32 matrixIndex; } RendererState;
extern RendererState _021D94D8; extern fx32 _021D94E8[];
extern MtxFx32 _021D9668[32], _021D9968[32];
void NNSi_G2dMtxConcat32(const MtxFx32 *a, const MtxFx32 *b, MtxFx32 *ab);
void NNS_G2dTranslate(fx32 x, fx32 y, fx32 z);
// @fn NNS_G2dTranslate
void NNS_G2dTranslate(fx32 x, fx32 y, fx32 z) {
 if (_021D94D8.translateOnly) {
  _021D9668[_021D94D8.matrixIndex]._20 += x;
  _021D9668[_021D94D8.matrixIndex]._21 += y;
  _021D94E8[_021D94D8.matrixIndex] += z;
 } else {
  MtxFx32 m;
  m._00 = FX32_ONE;
  m._01 = 0;
  m._10 = 0;
  m._11 = FX32_ONE;
  m._20 = x;
  m._21 = y;
  NNSi_G2dMtxConcat32(&m, &_021D9668[_021D94D8.matrixIndex], &_021D9668[_021D94D8.matrixIndex]);
  NNSi_G2dMtxConcat32(&m, &_021D9968[_021D94D8.matrixIndex], &_021D9968[_021D94D8.matrixIndex]);
  _021D94E8[_021D94D8.matrixIndex] += z;
 }
}
