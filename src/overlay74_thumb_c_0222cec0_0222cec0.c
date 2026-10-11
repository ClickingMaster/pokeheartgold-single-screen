#include "global.h"
#include "nitro/gx/g2.h"

#define S8AT(p, o) (*(s8 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void BG_SetMaskColor(u32, u32);
extern void Heap_Free(void *);
extern void GfGfx_EngineATogglePlanes(u32, u32);
extern void GfGfx_EngineBTogglePlanes(u32, u32);
extern void ov74_0222AF28(void *);
extern void ov74_0222B288(void *);
extern void ov74_0222B3A4(void *);
extern void ov74_0222B144(void *);
extern void sub_02034DE0(void);
extern void WirelessStatusIcon_Destroy(void);
extern void sub_02038D28(void *);
extern u32 sub_02038D80(void);
extern void sub_02038D64(void);
extern void *ov74_0223D0A8;

void ov74_0222BED8(void *p);
void ov74_0222BEF0(void *p);
void ov74_0222BF08(void *p);
void ov74_0222BF18(void);
BOOL ov74_0222BF2C(void *p, int mode, int step);
BOOL ov74_0222BFA0(void *p, int mode, int step);
void ov74_0222C014(void *p);
void *ov74_0222CD7C(void);
void ov74_0222CD88(void *p);
void ov74_0222CE6C(u32 value);
void ov74_0222CE10(void *p);
void ov74_0222CEC0(void);

void ov74_0222CEC0(void) {
    sub_02034DE0();
    Heap_Free(((void **)&ov74_0223D0A8)[1]);
    WirelessStatusIcon_Destroy();
    ((void **)&ov74_0223D0A8)[1] = NULL;
}
