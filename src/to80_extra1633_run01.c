#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0x11c];u8 active,table;s8 index;u8 pad2[2],selected;} State;
typedef struct {u32 words[2];} Pair;
extern State *ov13_0224F4BC;extern u32 ov13_02245958[],ov13_02244FA4[],ov13_02244F68[],ov13_02244F9C[],ov13_02244F8C[],ov13_02244F7C[],ov13_02244F64[],ov13_02244F84[],ov13_02244F94[],ov13_02244F88[],ov13_02244F98[];
extern u8 *ov13_02245D68[];extern u8 ov13_02245208[],ov13_02244F58[];
extern int ov13_022418EC(void *);extern void ov13_02240E74(void *,void *,void *),ov13_0222936C(int),ov13_022294B8(void),ov13_02228C08(void),ov13_02228C38(void);
void ov13_02228920(void);
void ov13_02228920(void) {
 Pair pair;int i;u32 *a,*b;
 ov13_0224F4BC->active=0;if(!ov13_022418EC(ov13_02245958))return;
 for(i=0,a=ov13_02244FA4;i<47;i++,a++){ov13_02240E74(a,ov13_02244F68,&pair);if(ov13_022418EC(&pair)){if(ov13_0224F4BC->index!=i)return;ov13_0224F4BC->active=ov13_02245D68[ov13_0224F4BC->table][i];if(ov13_0224F4BC->table==1)ov13_0222936C(0);ov13_0224F4BC->selected=i;ov13_022294B8();return;}}
 for(i=2,b=ov13_02244F9C,a=ov13_02244F8C;i<4;i++,b++,a++){ov13_02240E74(a,b,&pair);if(ov13_022418EC(&pair)){if(ov13_0224F4BC->index!=i+47)return;ov13_0224F4BC->active=ov13_02245208[i];if(ov13_0224F4BC->table==1)ov13_0222936C(0);ov13_0224F4BC->selected=i+47;ov13_022294B8();return;}}
 for(i=0,a=ov13_02244F7C;i<2;i++,a++){ov13_02240E74(a,ov13_02244F64,&pair);if(ov13_022418EC(&pair)){if(ov13_0224F4BC->index!=i+51)return;ov13_0224F4BC->active=ov13_02244F58[i];ov13_0224F4BC->selected=i+51;ov13_022294B8();return;}}
 ov13_02240E74(ov13_02244F84,ov13_02244F94,&pair);if(ov13_022418EC(&pair)){if(ov13_0224F4BC->index!=47)return;ov13_02228C08();ov13_0224F4BC->selected=47;ov13_022294B8();return;}
 ov13_02240E74(ov13_02244F88,ov13_02244F98,&pair);if(ov13_022418EC(&pair)){if(ov13_0224F4BC->index!=48)return;ov13_02228C38();ov13_0224F4BC->selected=48;ov13_022294B8();}
}
