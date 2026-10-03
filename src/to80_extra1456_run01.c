#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Renderer Renderer;typedef struct Surface Surface;
struct Surface{u8 p[0x10];int active;u32 type;void(*before)(Renderer*,const void*);void(*after)(Renderer*,const void*);};
struct Renderer{Surface*surface;u8 p[0x2c];int enabled;};extern Renderer *_021DA498;
extern u8 *NNSi_G2dGetCellTransferState(u32);extern void DrawCellToSurface3D_(Surface*,const void*);extern void DrawCellToSurface2D_(Surface*,const void*);
void NNS_G2dRndCoreDrawCellVramTransfer(const void*,u32);
void NNS_G2dRndCoreDrawCellVramTransfer(const void*cell,u32 handle){u32 type;Surface*s=_021DA498->surface;if(!s->active)return;_021DA498->enabled=1;if(s->before)s->before(_021DA498,cell);if(_021DA498->enabled){if(handle!=-1){type=s->type;u32 *state=(u32*)(NNSi_G2dGetCellTransferState(handle)+0x20);*state=(*state&~(1U<<type))|(1U<<type);}switch(s->type){case 0:DrawCellToSurface3D_(s,cell);break;case 1:case 2:DrawCellToSurface2D_(s,cell);break;case 3:default:break;}}if(s->after)s->after(_021DA498,cell);}
