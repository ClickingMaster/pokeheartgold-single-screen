#include "overlay87_sprite_resources_r13_private.h"

void ov87_021E7BC4(void *work, u32 unused) {
    PTR_AT(work, 0x15C) = AddCharResObjFromNarc(PTR_AT(work, 0x12C), 0xCF, 0xC, 0, 2, 1, 0x7A);
    PTR_AT(work, 0x160) = AddPlttResObjFromNarc(PTR_AT(work, 0x130), 0xCF, 0xD, 0, 2, 1, 4, 0x7A);
    PTR_AT(work, 0x164) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x134), 0xCF, 0xB, 0, 2, 2, 0x7A);
    PTR_AT(work, 0x168) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x138), 0xCF, 0xA, 0, 2, 3, 0x7A);
}

void ov87_021E7C50(void *work, u32 slot, u32 id, u32 vram) {
    PTR_AT(work, 0x13C + slot * 0x10) = AddCharResObjFromNarc(PTR_AT(work, 0x12C), 0xCF, 0x20, 0, id, vram, 0x7A);
    PTR_AT(work, 0x140 + slot * 0x10) = AddPlttResObjFromNarc(PTR_AT(work, 0x130), 0xCF, 0x21, 0, id, vram, 5, 0x7A);
    PTR_AT(work, 0x144 + slot * 0x10) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x134), 0xCF, 0x1F, 0, id, 2, 0x7A);
    PTR_AT(work, 0x148 + slot * 0x10) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x138), 0xCF, 0x1E, 0, id, 3, 0x7A);
}

void ov87_021E7CE0(void *work, u32 unused) {
    PTR_AT(work, 0x16C) = AddCharResObjFromNarc(PTR_AT(work, 0x12C), 0xCF, 0x10, 0, 3, 1, 0x7A);
    PTR_AT(work, 0x170) = AddPlttResObjFromNarc(PTR_AT(work, 0x130), 0xCF, 0x11, 0, 3, 1, 4, 0x7A);
    PTR_AT(work, 0x174) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x134), 0xCF, 0xF, 0, 3, 2, 0x7A);
    PTR_AT(work, 0x178) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x138), 0xCF, 0xE, 0, 3, 3, 0x7A);
}

void ov87_021E7D6C(void *work, u32 unused) {
    PTR_AT(work, 0x17C) = AddCharResObjFromNarc(PTR_AT(work, 0x12C), 0xCF, 0x14, 0, 4, 1, 0x7A);
    PTR_AT(work, 0x180) = AddPlttResObjFromNarc(PTR_AT(work, 0x130), 0xCF, 0x15, 0, 4, 1, 1, 0x7A);
    PTR_AT(work, 0x184) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x134), 0xCF, 0x13, 0, 4, 2, 0x7A);
    PTR_AT(work, 0x188) = AddCellOrAnimResObjFromNarc(PTR_AT(work, 0x138), 0xCF, 0x12, 0, 4, 3, 0x7A);
}
