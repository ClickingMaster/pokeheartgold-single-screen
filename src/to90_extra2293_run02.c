#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s32 x,y,z;} Vector;void sub_02015640(void*,Vector*),sub_02015628(void*,const Vector*),ov07_02231B90(void*,u32,Vector*),ov07_0221F9A8(void*,s32*,int),ov07_022202D8(s32,s8,Vector*),ov07_0222043C(void*,void*,u32,u32,u32,s8,const Vector*);
void ov07_0222043C(void*ctx,void*actor,u32 index,u32 kind,u32 mode,s8 sign,const Vector*origin){Vector v={0,0,0};switch(mode){case 1:sub_02015640(actor,&v);v.x*=sign;v.y*=sign;v.z*=sign;break;case 2:v.x=0;v.y=3200*sign;v.z=0;break;case 3:ov07_02231B90(ctx,kind,&v);break;case 4:ov07_02231B90(ctx,index,&v);break;case 5:{s32 temp[4]={0,0,0,0};ov07_0221F9A8(ctx,temp,4);v.x=temp[0];v.y=temp[1];v.z=temp[2];ov07_022202D8(temp[3],sign,&v);}break;}v.x-=origin->x;v.y-=origin->y;v.z-=origin->z;sub_02015628(actor,&v);}
