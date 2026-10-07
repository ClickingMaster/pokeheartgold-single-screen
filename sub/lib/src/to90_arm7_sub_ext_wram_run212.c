#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35536Station {u16 field[14];} W35536Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35536Station **)(ROOT+0x31c))[0])
void CAM_SetCapaInfo(u32 index,u16 value);
// @fn CAM_SetCapaInfo
void CAM_SetCapaInfo(u32 index,u16 value){STATION[index].field[6]=value;}
