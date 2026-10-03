#pragma force_active on
#include "global.h"
typedef struct {void *sprite;int radius,delta,angle,step;s16 life,pad;} Particle;
typedef struct {int active;Particle a[36],b[8],c[3];} Effect;
extern int GF_SinDegFX32(int),GF_CosDegFX32(int);
extern void Sprite_DeleteAndFreeResources(void *),ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int);
int ov93_02260660(u8 *);
#define STEP(E,R) do { \
 if((E)->sprite){if((E)->life==0){Sprite_DeleteAndFreeResources((E)->sprite);(E)->sprite=0;}else{ \
 (E)->angle+=(E)->step;(E)->radius+=(E)->delta;if((E)->radius>=0xa0000)(E)->radius=0xa0000; \
 int radius=(E)->radius;int sine=GF_SinDegFX32((E)->angle);R=(E)->radius;int cosine=GF_CosDegFX32((E)->angle); \
 s64 xx=(s64)sine*radius; s64 yy=(s64)cosine*R; \
 ManagedSprite_SetPositionXYWithSubscreenOffset((E)->sprite,(s16)((s32)((xx+0x800)>>12)/4096+128),(s16)(-(s32)((yy+0x800)>>12)/4096+96),0x160000);count++;(E)->life--;}} \
}while(0)
int ov93_02260660(u8 *p) {
 Effect *e=(Effect *)(p+0x339c);int count=0;Particle *part;int i,radiusA,radiusB,radiusC,k,j;
 if(!e->active)return 0;
 for(i=0,part=e->a;i<36;i++,part++){STEP(part,radiusA);}
 {for(j=0,part=e->b;j<8;j++,part++){STEP(part,radiusB);}}
 {for(k=0,part=e->c;k<3;k++,part++){STEP(part,radiusC);}}
 if(count==0)e->active=0;return 1;
}
