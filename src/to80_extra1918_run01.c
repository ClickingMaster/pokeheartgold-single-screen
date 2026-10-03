#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 ov39_0222AA7C[];
void ov39_0222A4EC(const u8 *,int,u8 *);
void ov39_0222A544(const u8 *,int,u8 *);
static inline u32 keybyte(const u8 *src,int n,int i){if(i==0)return 0;return ov39_0222AA7C[(src[0]+(i+n))&255];}
static inline u32 XorValue(u32 byte,u32 key){return byte^key;}
void ov39_0222A544(const u8 *src,int n,u8 *dst){int i;for(i=0;i<n;i++){int key;if(i!=0){int index=src[0];index+=i+n;key=ov39_0222AA7C[index&255];}else key=0;dst[i]=XorValue(dst[i],key);}}
