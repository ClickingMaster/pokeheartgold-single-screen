#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u16 cacheCount, reserved; BOOL translateOnly; NNSG2dRendererInstance *renderer; s32 matrixIndex; } RendererState;
typedef struct { u16 transferIndex, reserved; u32 transferHandle; MtxFx32 matrix; } RendererContext;
extern RendererState _021D94D8; extern RendererContext _021109DC;
static inline BOOL IsMtxStackEmpty(const RendererState *s) { return s->matrixIndex <= 0; }
void NNS_G2dPopMtx(void);
#pragma push
#pragma opt_propagation off
// @fn NNS_G2dPopMtx
void NNS_G2dPopMtx(void) {
    if (!IsMtxStackEmpty(&_021D94D8)) {
        s32 index = _021D94D8.matrixIndex - 1;
        u16 transfer = _021109DC.transferIndex;
        _021D94D8.matrixIndex = index;
        if (transfer > index) {
            _021109DC.transferIndex = 0xFFFE;
        }
    }
}
#pragma pop
