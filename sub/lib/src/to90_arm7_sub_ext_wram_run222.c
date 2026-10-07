#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35546Station {u16 field[14];} W35546Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35546Station **)(ROOT+0x31c))[0])
u8 * CAM_GetMacAdrs(u32 index);
// @fn CAM_GetMacAdrs
u8 * CAM_GetMacAdrs(u32 index){return (u8*)&STATION[index]+4;}
