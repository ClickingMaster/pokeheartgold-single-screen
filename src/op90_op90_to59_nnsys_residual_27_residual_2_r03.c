#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u16 a, counter; BOOL translateOnly; void *renderer; s32 matrixIndex; } RendererState;
typedef struct { u16 a, cacheIdx, state, pad; } MtxCacheState;
typedef struct { RendererState st; fx32 z[32]; MtxCacheState cs[32]; MtxFx32 mtx[32]; MtxFx32 srt[32]; } MCM;
extern MCM _021D94D8;
extern u16 _021109DC;
void NNSi_G2dMtxConcat32(const MtxFx32 *a, const MtxFx32 *b, MtxFx32 *ab);
void NNS_G2dRotZ(fx32 sin, fx32 cos);
static inline BOOL IsReady_(void) { return !_021D94D8.st.translateOnly && _021109DC != 0xFFFE; }
// @fn NNS_G2dRotZ
void NNS_G2dRotZ(fx32 sin, fx32 cos)
{
    MtxFx32 m;
    MtxCacheState *p;
    m._00 = cos;
    m._01 = sin;
    m._10 = -sin;
    m._11 = cos;
    m._20 = 0;
    m._21 = 0;
    NNSi_G2dMtxConcat32(&m, &_021D94D8.mtx[_021D94D8.st.matrixIndex], &_021D94D8.mtx[_021D94D8.st.matrixIndex]);
    NNSi_G2dMtxConcat32(&m, &_021D94D8.srt[_021D94D8.st.matrixIndex], &_021D94D8.srt[_021D94D8.st.matrixIndex]);
    if (IsReady_() == FALSE) {
        _021109DC = (u16)_021D94D8.st.matrixIndex;
    }
    p = &_021D94D8.cs[(u16)_021D94D8.st.matrixIndex];
    switch (p->state) {
    case 0:
    case 2:
    case 3:
        p->cacheIdx = _021D94D8.st.counter++;
        p->state = 1;
        break;
    case 1:
        break;
    }
}
