#include "to53_overlay49_r73a_gap3_private.h"

void ov49_0226747C(void *state) {
    VecFx32 pos;
    int type;
    u32 x;
    u32 y;

    ov49_02259154(*(void **)((u8 *)state + 8), &pos);
    type = *(s8 *)((u8 *)state + 0x955);
    switch (type) {
    case 1:
        x = _fadd(0x3f000000, _fflt(15 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 14));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        break;
    case 2:
        x = _fadd(0x3f000000, _fflt(14 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 12));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        x = _fadd(0x3f000000, _fflt(14 << 14));
        y = _fadd(0x3f000000, _fflt(14 << 12));
        ModelInstance_SetPosition((u8 *)state + 0x84, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        break;
    case 3:
    default:
        x = _fadd(0x3f000000, _fflt(15 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 14));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        x = _fadd(0x3f000000, _fflt(14 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 12));
        ModelInstance_SetPosition((u8 *)state + 0x84, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        x = _fadd(0x3f000000, _fflt(14 << 14));
        y = _fadd(0x3f000000, _fflt(14 << 12));
        ModelInstance_SetPosition((u8 *)state + 0xfc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z + 0x1000);
        break;
    }
}

void ov49_02267674(void *state) {
    VecFx32 pos;
    int type;
    u32 x;
    u32 y;

    ov49_02259154(*(void **)((u8 *)state + 8), &pos);
    type = *(s8 *)((u8 *)state + 0x955);
    switch (type) {
    case 1:
        x = _fadd(0x3f000000, _fflt(3 << 16));
        y = _fadd(0x3f000000, _fflt(2 << 14));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        break;
    case 2:
        x = _fadd(0x3f000000, _fflt(11 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 12));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        x = _fadd(0x3f000000, _fflt(11 << 14));
        y = _fadd(0x3f000000, _fflt(14 << 12));
        ModelInstance_SetPosition((u8 *)state + 0x84, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        break;
    case 3:
    default:
        x = _fadd(0x3f000000, _fflt(3 << 16));
        y = _fadd(0x3f000000, _fflt(2 << 14));
        ModelInstance_SetPosition((u8 *)state + 0xc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        x = _fadd(0x3f000000, _fflt(11 << 14));
        y = _fadd(0x3f000000, _fflt(2 << 12));
        ModelInstance_SetPosition((u8 *)state + 0x84, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        x = _fadd(0x3f000000, _fflt(11 << 14));
        y = _fadd(0x3f000000, _fflt(14 << 12));
        ModelInstance_SetPosition((u8 *)state + 0xfc, pos.x + _ffix(y), pos.y + _ffix(x), pos.z - 0x8000);
        break;
    }
}
