#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35544Station {u16 field[14];} W35544Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35544Station **)(ROOT+0x31c))[0])
u32 CAM_IsActive(u32 index);
// @fn CAM_IsActive
u32 CAM_IsActive(u32 index){return ((int)HALF(0x530)>>index)&1;}
