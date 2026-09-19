#include "to53_overlay112_w01_private.h"


void ov112_021E795C(void *state) {
    *(void **)((u8 *)state + 0x1e434) = Save_Bag_Get(*(void **)((u8 *)state + 0x20));
    *(void **)((u8 *)state + 0x1e438) = Save_PlayerData_GetProfile(*(void **)((u8 *)state + 0x20));
    *(void **)((u8 *)state + 0x1e43c) = Save_PlayerData_GetOptionsAddr(*(void **)((u8 *)state + 0x20));
    *(void **)((u8 *)state + 0x1e440) = Save_Pokewalker_Get(*(void **)((u8 *)state + 0x20));
    *(void **)((u8 *)state + 0x1e444) = Save_Pokedex_Get(*(void **)((u8 *)state + 0x20));
}
