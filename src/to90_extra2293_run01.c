#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{VecFx32 pos;int id;}Info;
extern void sub_02015720(void *,VecFx32 *),ov07_02231B90(void *,void *,VecFx32 *),ov07_0221F9A8(void *,Info *,int),ov07_022202D8(int,s8,VecFx32 *),sub_02015708(void *,VecFx32 *);
void ov07_0222036C(void *,void *,void *,void *,int,s8,VecFx32 *);
void ov07_0222036C(void *p,void *target,void *a,void *b,int kind,s8 scale,VecFx32 *base){VecFx32 pos={0,0,0};switch(kind){case 0:break;case 1:sub_02015720(target,&pos);pos.x*=scale;pos.y*=scale;pos.z*=scale;break;case 2:pos.x=0;pos.y=3200*scale;pos.z=0;break;case 3:ov07_02231B90(p,b,&pos);break;case 4:ov07_02231B90(p,a,&pos);break;case 5:{Info info={0};ov07_0221F9A8(p,&info,4);pos.x=info.pos.x;pos.y=info.pos.y;pos.z=info.pos.z;ov07_022202D8(info.id,scale,&pos);break;}}pos.x-=base->x;pos.y-=base->y;pos.z-=base->z;sub_02015708(target,&pos);}
