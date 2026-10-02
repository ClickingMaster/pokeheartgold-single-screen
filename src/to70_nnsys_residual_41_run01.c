#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Header {u8 p[12];u16 offset;} Header;
typedef struct Dict {u8 p[1];u8 count;u8 pad[4];u16 offset;} Dict;
void *NNS_G3dGetAnmByIdx(const void*,u32);
void *NNS_G3dGetAnmByIdx(const void *data,u32 index){const Header *base=data;if(base){u8 *b=(u8*)base+*(u32*)((u8*)base+base->offset);Dict *d=(Dict*)(b+8);u32 *entry;if(d && index<b[9]){u8 *table=(u8*)d+*(u16*)(b+14);u16 stride=*(u16*)table;entry=(u32*)(table+4+stride*index);}else entry=0;if(entry)return (u8*)b+*entry;}return 0;}
