#pragma force_active on
#include "global.h"
typedef struct MapObject MapObject;
extern int sub_02023F30(void *);extern void MapObject_SetFacingVector(MapObject *,VecFx32 *);
void ov01_021F8874(MapObject *,void *,u32);
void ov01_021F8874(MapObject *obj,void *data,u32 kind){VecFx32 pos={0,0,0};int n=sub_02023F30(data)/4096;switch(kind){case 0:break;case 1:if(n>=24)pos.z=0x6000;break;case 2:if(n<40)pos.x=0x1000;else pos.x=-0x5000;break;case 3:if(n<56)pos.x=-0x1000;else pos.x=0x5000;break;}MapObject_SetFacingVector(obj,&pos);}
