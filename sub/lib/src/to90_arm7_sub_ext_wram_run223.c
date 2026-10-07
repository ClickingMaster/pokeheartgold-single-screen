#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35547Station {u16 field[14];} W35547Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35547Station **)(ROOT+0x31c))[0])
u16 CAM_GetAuthSeed(u32 index);
// @fn CAM_GetAuthSeed
u16 CAM_GetAuthSeed(u32 index){return STATION[index].field[7];}
