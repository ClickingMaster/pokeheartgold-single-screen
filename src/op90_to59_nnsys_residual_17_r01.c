#pragma force_active on
#include <nitro/types.h>
#include <nitro/fx/fx.h>
#include <nitro/gx/g3imm.h>
#include <nitro/code32.h>
extern void G3i_OrthoW_(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, BOOL draw, MtxFx44 *mtx);
extern void G3i_LookAt_(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, BOOL draw, MtxFx43 *mtx);
extern const VecFx32 _021092E0, _021092EC;
void NNS_G2dSetupSoftwareSpriteCamera(void);
static inline void OrthoW(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx) {
    G3i_OrthoW_(t, b, l, r, n, f, scaleW, TRUE, mtx);
}
static inline void LookAt(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, MtxFx43 *mtx) {
    G3i_LookAt_(camPos, camUp, target, TRUE, mtx);
}
// @fn NNS_G2dSetupSoftwareSpriteCamera
void NNS_G2dSetupSoftwareSpriteCamera(void) {
    G3_ViewPort(0, 0, 255, 191);
    OrthoW(0, 192 * FX32_ONE, 0, 256 * FX32_ONE, -1024 * FX32_ONE, 1024 * FX32_ONE, 1024 * FX32_ONE, NULL);
    reg_G3_MTX_STORE = 0;
    {
        VecFx32 camPos = {0, 0, 0};
        VecFx32 camUp = _021092E0;
        VecFx32 target = _021092EC;
        LookAt(&camPos, &camUp, &target, NULL);
    }
    G3_MtxMode(GX_MTXMODE_POSITION);
}
