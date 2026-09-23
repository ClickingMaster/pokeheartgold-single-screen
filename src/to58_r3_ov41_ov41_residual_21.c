#include "global.h"

void ov41_02247480(void *work, int index);

typedef struct {
    void *field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    u32 field10;
    u32 field14;
    u32 field18;
    u32 field1c;
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2c;
} Ov41LoadArgs;

void ov41_02249C7C(void *, Ov41LoadArgs *);

void ov41_02247480(void *work, int index)
{
    Ov41LoadArgs args;
    args.field00 = *(void **)((u8 *)work + 0x40);
    args.field04 = 0x1a;
    args.field08 = 2 * index + 0x79;
    args.field0c = 0x85;
    args.field10 = 2 * index + 0x7a;
    args.field14 = 0;
    args.field18 = 0;
    args.field1c = 1;
    args.field20 = 1;
    args.field24 = 2;
    args.field28 = 0;
    args.field2c = 0xe;
    ov41_02249C7C((u8 *)work + 0x4b4, &args);
}
