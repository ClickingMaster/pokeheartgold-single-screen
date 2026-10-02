#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct State {u16 count,x2;u32 pad;void *renderer;u32 index;} State;
typedef struct Cache {u32 pad[4],values[4];} Cache;extern volatile State _021D94D8;extern volatile u16 _021109DC;extern Cache _021D9C68[];extern u8 _021D9568[];
void MIi_CpuClearFast(u32,void*,u32);void NNSi_G2dIdentity(void);
void NNS_G2dBeginRendering(void*);
void NNS_G2dBeginRendering(void *renderer){_021D94D8.renderer=renderer;_021D94D8.index=0;_021109DC=0xfffe;int i=0;if(i<_021D94D8.count){Cache *cache=_021D9C68;do{MIi_CpuClearFast(0xffffffff,cache->values,16);i++;cache++;}while(i<_021D94D8.count);}_021D94D8.count=0;_021D94D8.x2=0;MIi_CpuClearFast(0,_021D9568,256);*(volatile u32*)0x04000444=0;*(volatile u32*)0x04000454=0;NNSi_G2dIdentity();}
