// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void CPSi_md5_block(u32 *,const u8 *);
void CPSi_md5_calc(u8 *,u8 *,u32);
static inline void MdAddBytes(u32 *context,u32 length){
    context[4] += length<<3;
    if(context[4]<(length<<3))context[5]++;
    context[5] += length>>29;
}
void k3(u8 *storage,u8 *data,u32 length);
// @fn CPSi_md5_calc
void CPSi_md5_calc(u8 *storage,u8 *data,u32 length) {
    u32 i;u32 index;u32 saved;u32 *context;u32 part;
    context=(u32 *)storage; saved=context[4]; index=(u32)((saved>>3)&63U);
    MdAddBytes(context,length);
    part=64-index;
    if(length>=part){
        MI_CpuCopy8(data,storage+24+index,part);
        index=0;
        CPSi_md5_block(context,storage+24);
        for(i=part;i+63<length;i+=64)CPSi_md5_block(context,data+i);
    }else i=0;
    MI_CpuCopy8(data+i,storage+24+index,length-i);
}
