#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern int MATH_CalcCRC8(void *, void *, u32);
int DWC_Acc_CheckFriendKey(u64, u32);

int DWC_Acc_CheckFriendKey(u64 key, u32 game) {
    MATHCRC8Table table;
    u32 info[2];
    int check;
    if ((key & 0x80000000ULL) != 0) {
        return 0;
    }
    info[0] = (u32)key;
    info[1] = game;
    MATHi_CRC8InitTable(&table, 7);
    check = MATH_CalcCRC8(&table, info, 8) & 127;
    return (u64)(s64)check == (key >> 32);
}
