#include "overlay80_object_updates_r37_private.h"
void ov80_0222EEC8(SysTask *task, void *work) {
    Ov80PositionCommand command;
    u16 opcode = R16(RP(work, 0xc), 0);
    u16 count = R16(RP(work, 0xc), 2);
    switch (R8(work, 0)) {
    case 0:
        if (opcode == 0xfd13) {
            R8(work, 0) = 2;
            return;
        }
    case 1:
        if (ov42_02228188(RP(RP(work, 0x14), 0), 5) != 0) {
            return;
        }
        if (opcode >= 0x20 && opcode <= 0x26) {
            RS16(work, 2)
            ++;
            if (RS16(work, 2) >= ov80_0223BD44[opcode - 0x20]) {
                R16(work, 2) = 0;
                RP(work, 0xc) = (u8 *)RP(work, 0xc) + 4;
                return;
            }
            return;
        }
        if (opcode >= 0x27 && opcode < 0x29) {
            switch (opcode) {
            case 0x27:
                ov42_02229200(RP(RP(work, 0x14), 4), 1);
                break;
            case 0x28:
                ov42_02229200(RP(RP(work, 0x14), 4), 0);
                break;
            default:
                GF_AssertFail();
                break;
            }
            RP(work, 0xc) = (u8 *)RP(work, 0xc) + 4;
            return;
        }
        ov80_0222EFD0(&command, RP(RP(work, 0x14), 0), R16(work, 4), opcode);
        ov42_022299C0(RP(work, 0x10), &command);
        R8(work, 1)
        ++;
        if (R8(work, 1) >= count) {
            R8(work, 1) = 0;
            RP(work, 0xc) = (u8 *)RP(work, 0xc) + 4;
        }
        R8(work, 0) = 0;
        return;
    case 2:
        if (ov42_02228188(RP(RP(work, 0x14), 0), 5) == 0) {
            R8(RP(work, 8), 0)
            --;
            RP(RP(work, 0x14), 0x38) = NULL;
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        return;
    }
}

void ov80_0222EFD0(Ov80PositionCommand *out, void *object, u16 sequence, s32 direction) {
    out->index = sequence;
    out->direction = ov80_0223DC22[direction * 2];
    out->sequence = _0223DC20[direction * 2];
    if (out->sequence == 2 || out->sequence == 3 || out->sequence == 6) {
        volatile u16 position[2];
        u32 raw = ov42_022282E8(object);
        position[0] = raw;
        position[1] = raw >> 16;
        out->x = position[0];
        out->y = position[1];
        return;
    } else {
        volatile u16 position[2];
        u32 raw = ov42_022282DC(object);
        position[0] = raw;
        position[1] = raw >> 16;
        out->x = position[0];
        out->y = position[1];
    }
}
