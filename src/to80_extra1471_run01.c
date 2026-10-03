#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 p[0x2c];u8 texture;u8 q;u16 low:5,alpha:5,rest:6;u8 r[6];u16 color;}Particle;
typedef struct{u8 frames[8],count,step;}Texture;
typedef struct{u16 a:5,b:5,c:5,x:1;u8 random,pad,start,end;}Alpha;
typedef struct{u16 a,b;u8 start,middle,end,pad;u16 p:2,interpolate:1,q:13;}Color;
typedef struct{u8 p[0x22];u16 color;}Base;
typedef struct{Base*base;void*p;Color*color;Alpha*alpha;Texture*texture;}Resource;
extern u32 spl_rndm_seed;
void spl_tex_ptn_anm(Particle*,Resource*,int),spl_alp_in_out(Particle*,Resource*,int),spl_clr_in_out(Particle*,Resource*,int);
void spl_alp_in_out(Particle*p,Resource*r,int time){Alpha*a=r->alpha;int value,begin=a->start,end=a->end;if(time<begin)value=a->a+time*(a->b-a->a)/begin;else if(time<end)value=a->b;else value=a->c+(time-255)*(a->c-a->b)/(255-end);spl_rndm_seed=spl_rndm_seed*0x5eedf715+0x1b0cb173;p->alpha=(u16)((value*(255-((a->random*(int)(spl_rndm_seed>>24))>>8)))>>8);}
