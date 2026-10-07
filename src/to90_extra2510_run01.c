#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct E{int active;u8 pad[12];void*object;u8 tail[8];}E;extern u16 LCRandom(void);extern void ov96_021EB564(void*,int);void ov96_021F7620(E*);
#pragma opt_scalarizeliveranges off
void ov96_021F7620(E*entries){struct {E *pool[29];} local;struct {int count;E **end;} status;int i;status.count=0;for(i=0;i<29;i++,entries++){if(entries->active==0)local.pool[status.count++]=entries;}i=0;status.end=local.pool+status.count;for(;i<25;i++){u8 index;if(status.count<=0)break;index=LCRandom()%status.count;local.pool[index]->active=1;ov96_021EB564(local.pool[index]->object,7);local.pool[index]=status.end[-1];status.end--;status.count--;}}
