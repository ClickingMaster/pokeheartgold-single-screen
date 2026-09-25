#include "to59_nnsys_linked_vram_private.h"

u32 NNS_GfdGetLnkPlttVramManagerWorkSize(u32 numMemBlk) {
    return numMemBlk << 4;
}

void NNS_GfdInitLnkPlttVramManager(u32 szByte, void *pManagementWork, u32 szByteManagementWork, BOOL useAsDefault) {
    _021D8500[2] = szByte;
    _021D8500[3] = (u32)pManagementWork;
    _021D8500[4] = szByteManagementWork;
    NNS_GfdResetLnkPlttVramState();
    if (useAsDefault) {
        NNS_GfdDefaultFuncAllocPlttVram = NNS_GfdAllocLnkPlttVram;
        NNS_GfdDefaultFuncFreePlttVram = NNS_GfdFreeLnkPlttVram;
    }
}

u32 NNS_GfdAllocLnkPlttVram(u32 size, BOOL is4color, u32 opt) {
    u32 aligned;
    u32 offset;
    u32 ret;
    if (size == 0) {
        aligned = 8;
    } else {
        aligned = (size + 7) & ~7;
    }
    if (aligned >= 0x7FFF8) {
        return 0;
    }
    if (is4color) {
        ret = NNSi_GfdAllocLnkVramAligned(_021D8500, _021D8500 + 1, &offset, aligned, 8);
        if (offset + aligned > 0x10000) {
            NNSi_GfdFreeLnkVram(_021D8500, _021D8500 + 1, offset, aligned);
            return 0;
        }
    } else {
        ret = NNSi_GfdAllocLnkVramAligned(_021D8500, _021D8500 + 1, &offset, aligned, 16);
    }
    if (ret == 0) {
        return 0;
    }
    return ((aligned >> 3) << 16) | ((offset << 13) >> 16);
}

int NNS_GfdFreeLnkPlttVram(u32 key) {
    u32 size = ((key & 0xFFFF0000) >> 16) << 3;
    u32 offset = (key << 16) >> 13;
    int ret = NNSi_GfdFreeLnkVram(_021D8500, _021D8500 + 1, offset, size);
    return ret == 0;
}
