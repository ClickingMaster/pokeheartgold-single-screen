#include "overlay_49_sol_partial_internal.h"

#pragma require_prototypes off

#define U8_AT(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U16_AT(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define U32_AT(p, n) (*(u32 *)((u8 *)(p) + (n)))

extern u32 ov42_02227FA4(void *);
extern u32 ov42_02229A8C(void *);
extern u32 ov45_022306B4(void *);
extern u32 ov45_022306F4(void *);
extern u32 ov45_022308B8(void *);
extern u32 ov45_0223089C(void *);
extern u32 ov45_022308C0(void *);
extern u32 ov45_022308E4(void *);
extern u32 ov45_02230908(void *);
extern u32 ov45_02230920(void *);
extern u32 ov45_022308B0(void *);
extern u32 ov45_02230978(void *);
extern u32 ov45_02230994(void *);
extern void WiFiPlazaObj_SlideOneCell(void *, u32, u32);
extern void ov49_0225AB14(void *, u32, u32);
extern void ov49_0225B124(void *);
extern void ov49_0225CD58(void *);
extern void ov49_0225CED0(void *);
extern void ov49_0225CEFC(void *);
extern void ov49_0225F018(void *, u32);
extern void ov49_0225A134(void *);
extern void ov49_02262E10(void *, u32, u32, u32);
extern void ov49_02267A84(void *, u32, u32);
extern void ov49_02267C20(void *, u32, u32);
extern void ov49_02266B28(void *, u32, u32);
extern void ov49_02267908(void *, u32, u32);
extern void ov49_02267AF0(void *, u32, u32);
extern void WiFiPlazaLight_UnpackColor(void *);
extern void ov49_02269154(void *);
extern void ov49_0225E82C(void *, u32, u32);
extern void ModelInstance_SetVisible();
extern void ModelInstance_SetScale(void *);
extern void ModelInstance_SetRotation(void *, u32, u32);
extern void ModelResource_Free(void *);
extern void ov42_02227F28(void *);
extern u32 ov42_02227FDC(void *);
extern void ov45_0223093C(void *, u32);
extern void ov45_02230968(void *, u32);
extern void GF_AssertFail(void);
extern void WaitingIcon_Delete(void *);
extern void WaitingIcon_DeleteWithoutRestore(void *);
extern void ListMenuItems_Delete(void *);
extern void PlaySE(u32);
extern void ov49_0225BF80(void *);
extern void ov49_0225CAA8(void *);
extern void Camera_UnsetStaticPtr(void *);
extern void Camera_Delete(void *);

void ov49_0225CF94(void *p) {
    ModelInstance_SetVisible((u8 *)p + 4, 0);
    U8_AT(p, 0) = 0;
}

typedef struct Ov49Residual39Work {
    u8 pad00[0x9C];
    VecFx32 first;
    VecFx32 second;
} Ov49Residual39Work;

extern void ModelInstance_SetPosition(void *, fx32, fx32, fx32);
extern void ModelInstance_GetPosition(void *, void *, void *, void *);

void ov49_0225CFA8(Ov49Residual39Work *work, const VecFx32 *value) {
    work->first = *value;
    ModelInstance_SetPosition((u8 *)work + 4,
        work->first.x + work->second.x,
        work->first.y + work->second.y,
        work->first.z + work->second.z);
}

void ov49_0225CFEC(Ov49Residual39Work *work, const VecFx32 *value) {
    work->second = *value;
    ModelInstance_SetPosition((u8 *)work + 4,
        work->first.x + work->second.x,
        work->first.y + work->second.y,
        work->first.z + work->second.z);
}

void ov49_0225D030(void *work, void *parts) {
    ModelInstance_GetPosition((u8 *)work + 4, parts, (u8 *)parts + 4, (u8 *)parts + 8);
}
