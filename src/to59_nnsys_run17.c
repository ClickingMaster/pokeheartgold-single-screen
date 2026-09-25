#include "to59_nnsys_linked_vram_private.h"

u32 NNS_GfdGetLnkTexVramManagerWorkSize(u32 numMemBlk) {
    return numMemBlk << 4;
}

void NNS_GfdInitLnkTexVramManager(u32 szByte, u32 szByteFor4x4, void *pManagementWork, u32 szByteManagementWork, BOOL useAsDefault) {
    _021D84E4[3] = szByte;
    _021D84E4[4] = szByteFor4x4;
    _021D84E4[5] = (u32)pManagementWork;
    _021D84E4[6] = szByteManagementWork;
    NNS_GfdResetLnkTexVramState();
    if (useAsDefault) {
        NNS_GfdDefaultFuncAllocTexVram = NNS_GfdAllocLnkTexVram;
        NNS_GfdDefaultFuncFreeTexVram = NNS_GfdFreeLnkTexVram;
    }
}

u32 NNS_GfdAllocLnkTexVram(u32 size, BOOL is4x4, u32 opt) {
    u32 aligned;
    u32 offset;
    u32 ret;
    if (size == 0) {
        aligned = 16;
    } else {
        aligned = (size + 15) & ~15;
    }
    if (aligned >= 0x7FFF0) {
        return 0;
    }
    if (is4x4 != 0) {
        ret = NNSi_GfdAllocLnkVram(_021D84E4 + 1, _021D84E4 + 2, &offset, aligned);
    } else {
        ret = NNSi_GfdAllocLnkVram(_021D84E4, _021D84E4 + 2, &offset, aligned);
    }
    if (ret == 0) {
        return 0;
    }
    return ((aligned >> 4) << 16) | ((offset << 13) >> 16) | (is4x4 << 31);
}
