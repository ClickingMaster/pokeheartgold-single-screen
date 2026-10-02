#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Transfer {u8 pad[0x1c];int active;u8 tail[0x10];} Transfer;
typedef struct Manager {void *queue;u32 count;Transfer *states;} Manager;
extern Manager _021DA470;
u32 NNS_G2dGetNewCellTransferStateHandle(void);
u32 NNS_G2dGetNewCellTransferStateHandle(void){for(u32 i=0;i<_021DA470.count;i++){if(_021DA470.states[i].active!=1){_021DA470.states[i].active=1;return i;}}return -1;}
