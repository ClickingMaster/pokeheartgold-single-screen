#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 *ov13_0224F4C0;
extern u8 ov13_02245958[],ov13_0224527A[],ov13_02245228[],ov13_02245314[],ov13_02245250[],ov13_02245230[],ov13_02245224[],ov13_02245258[],ov13_02245234[],ov13_02245220[];
int ov13_022418EC(const void*);
void ov13_02240E74(const void*,const void*,void*);
void ov13_0222AE24(void);
void ov13_0222A554(void);
void ov13_0222A554(void){u8 text[8];int i;u8 *entry;ov13_0224F4C0[0x60]=0;if(!ov13_022418EC(ov13_02245958))return;
entry=ov13_0224527A;for(i=0;i<10;i++,entry+=4){ov13_02240E74(entry,ov13_02245228,text);if(ov13_022418EC(text)){if(*(s8*)(ov13_0224F4C0+0x61)!=i)return;ov13_0224F4C0[0x60]=ov13_02245314[i];ov13_0224F4C0[0x63]=i;ov13_0222AE24();return;}}
entry=ov13_02245250;for(i=0;i<2;i++,entry+=4){ov13_02240E74(entry,ov13_02245230,text);if(ov13_022418EC(text)){if(*(s8*)(ov13_0224F4C0+0x61)!=i+10)return;ov13_0224F4C0[0x60]=ov13_02245224[i];ov13_0224F4C0[0x63]=i+10;ov13_0222AE24();return;}}
entry=ov13_02245258;for(i=0;i<2;i++,entry+=4){ov13_02240E74(entry,ov13_02245234,text);if(ov13_022418EC(text)){if(*(s8*)(ov13_0224F4C0+0x61)!=i+12)return;ov13_0224F4C0[0x60]=ov13_02245220[i];ov13_0224F4C0[0x63]=i+12;ov13_0222AE24();return;}}}
