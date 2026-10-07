#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void MI_Copy36B(const void*,void*);
// @fn MTX_TransApply43
void MTX_TransApply43(const MtxFx43*src,MtxFx43*dst,fx32 x,fx32 y,fx32 z){if(src!=dst)MI_Copy36B(src,dst);dst->_30=(fx32)((u32)src->_30+(u32)(((u64)((s64)x*src->_00)+(u64)((s64)y*src->_10)+(u64)((s64)z*src->_20))>>12));dst->_31=(fx32)((u32)src->_31+(u32)(((u64)((s64)x*src->_01)+(u64)((s64)y*src->_11)+(u64)((s64)z*src->_21))>>12));dst->_32=(fx32)((u32)src->_32+(u32)(((u64)((s64)x*src->_02)+(u64)((s64)y*src->_12)+(u64)((s64)z*src->_22))>>12));}
