#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35543Station {u16 field[14];} W35543Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35543Station **)(ROOT+0x31c))[0])
u16 CAM_GetStaState(u32 index);
// @fn CAM_GetStaState
u16 CAM_GetStaState(u32 index){return STATION[index].field[0];}
