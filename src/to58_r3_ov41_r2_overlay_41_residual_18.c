#include "global.h"

void ov41_02247288(void *work, void *arg, u32 value, int useOther);
void ov41_02247F3C(void *, const u32 *);
void ov41_02247FE0(void *, void *, void *, int);
void ov41_02247FFC(void *, void *, void *, int);
void ov41_022495C8(void *, void *);
void ov41_02248158(void *);
void ov41_0224825C(void *, int, int);

void ov41_02247288(void *work, void *arg, u32 value, int useOther)
{
    u32 args[9];
    u32 result[4];
    args[0] = *(u32 *)work;
    args[1] = *(u32 *)((u8 *)work + 4);
    args[2] = *(u32 *)((u8 *)work + 0x10);
    args[3] = *(u32 *)((u8 *)work + 0x30);
    args[4] = *(u32 *)((u8 *)work + 0x20);
    {
        u32 f40 = *(u32 *)((u8 *)work + 0x40);
        args[8] = value;
        args[5] = f40;
    }
    args[6] = *(u32 *)((u8 *)work + 0x364);
    args[7] = (u32)((u8 *)work + 0x35c);
    ov41_02247F3C((u8 *)work + 0x3f4, args);
    if (useOther == 0) {
        ov41_02247FE0((u8 *)work + 0x3f4, arg, result, 0xe);
    } else {
        ov41_02247FFC((u8 *)work + 0x3f4, arg, result, 0xe);
    }
    ov41_022495C8(work, result);
    ov41_02248158((u8 *)work + 0x3f4);
    ov41_0224825C((u8 *)work + 0x3f4, 0, 0xe);
}
