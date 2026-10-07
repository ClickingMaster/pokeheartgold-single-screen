#pragma force_active on
#include "global.h"
#pragma force_active on
fx32 GF_SinDegFX32(fx32);
typedef struct{u8 bytes[0x78];} Object;
typedef struct{Object objects[2];s32 angle;u16 distance;u8 token,state;u32 transform[3][4];} Pool;
typedef struct{u8 padding[12],resource[16];Object master;Pool pool;} Context;
u8 ov89_0225AD00(void*);void ov89_0225AD64(void*,u8);void ov89_0225ADA4(void*,void*,void*,int);
void sub_020182B0(void*,s32*,s32*,s32*);void sub_020181B0(void*,void*);void sub_020182A8(void*,s32,s32,s32);void sub_020182A0(void*,int);
int ov89_0225BBB4(void*,Context*);
int ov89_0225BBB4(void*argument,Context*c){Pool*p=&c->pool;s32 x,y,z;int i;
 switch(p->state){case 0:p->token=ov89_0225AD00(argument);sub_020182B0(&c->master,&x,&y,&z);for(i=0;i<2;i++){sub_020181B0(&p->objects[i],c->resource);sub_020182A8(&p->objects[i],x,y,z);sub_020182A0(&p->objects[i],1);}p->distance=0x1f00;p->state++;
 case 1:if((int)p->distance-160<256){for(i=0;i<2;i++)sub_020182A0(&p->objects[i],0);sub_020182A0(&c->master,0);p->state++;}else{s32 displacement;p->distance-=160;p->angle+=0x20000;if(p->angle>=0x168000)p->angle-=0x168000;displacement=(s32)(((s64)GF_SinDegFX32(p->angle)*65536+2048)>>12);sub_020182B0(&c->master,&x,&y,&z);for(i=0;i<2;i++){if(i&1)sub_020182A8(&p->objects[i],x,y+displacement,z);else sub_020182A8(&p->objects[i],x,y-displacement,z);}}break;
 default:ov89_0225AD64(argument,p->token);return 1;}
 ov89_0225ADA4(&c->master,c,p->transform[0],0);for(i=0;i<2;i++)ov89_0225ADA4(&p->objects[i],c,p->transform[i+1],0);return 0;
}
