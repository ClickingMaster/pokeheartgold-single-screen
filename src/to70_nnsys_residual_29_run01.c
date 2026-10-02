#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef enum Space {SPACE_3D,SPACE_MAIN,SPACE_SUB,SPACE_MAX} Space;
typedef struct Transfer {u8 pad[16];const u8 *base2d,*base3d;u32 pad18,active,draw,request,offset,size;} Transfer;
typedef struct Manager {int(*queue)(int,u32,const u8*,u32);u32 count;Transfer *states;} Manager;
extern Manager _021DA470;extern int _02109328[3];
u32 NNSi_G2dGetVramLocation(Transfer*,int);void NNS_G2dUpdateCellTransferStateManager(void);
void NNS_G2dUpdateCellTransferStateManager(void){for(u32 i=0;i<_021DA470.count;i++){Space j;Transfer *t=&_021DA470.states[i];if(t->active){for(j=SPACE_3D;j<SPACE_MAX;j=(Space)(j+1)){int requested=0;if(t->request&(1<<j)){if(t->draw&(1<<j))requested=1;}if(requested){int type=_02109328[j];const u8 *base;if(j==0)base=t->base3d;else base=t->base2d;u32 destination=NNSi_G2dGetVramLocation(t,j);if(_021DA470.queue(type,destination,base+t->offset,t->size))t->request&=~(1<<j);}}t->draw=0;}}}
