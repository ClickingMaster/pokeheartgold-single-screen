#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35549Station {u16 field[14];} W35549Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35549Station **)(ROOT+0x31c))[0])
u16 CAM_GetTxRate(u32 index);
// @fn CAM_GetTxRate
u16 CAM_GetTxRate(u32 index){if(STATION[index].field[8]&2)return 20;return 10;}
