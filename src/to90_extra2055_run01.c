#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
typedef struct{u8 *root;void *a,*b;}Data;
extern Data *OverlayManager_GetData(void *);extern int IsPaletteFadeFinished(void),ov90_02258B98(u8 *),ov90_022589BC(void *),ov90_02258AA0(void *);extern void sub_0200FB70(void),sub_0200FC20(int),ov90_02258938(void *),ov90_02258A04(void *);extern void *ov90_0225892C(u8 *,int),*ov90_022589F8(u8 *,void *,int);extern u32 ov90_022589CC(void *),ov90_02258AA4(void *);
int ov93_022625BC(void *,int *);
int ov93_022625BC(void *manager,int *state){Data *d=OverlayManager_GetData(manager);if(d->root[0x3d]==1){switch(d->root[0x3e]){case 0:if(IsPaletteFadeFinished()==1)sub_0200FB70();sub_0200FC20(0);d->root[0x3e]++;break;case 1:if(ov90_02258B98(d->root)==1)d->root[0x3e]++;break;case 2:default:if(d->a){ov90_02258938(d->a);d->a=0;}if(d->b){ov90_02258A04(d->b);d->b=0;}return 1;}return 0;}switch(*state){case 0:if(d->root[0x31]==0)*state=1;else *state=3;break;case 1:d->a=ov90_0225892C(d->root,0x75);(*state)++;break;case 2:if(ov90_022589BC(d->a)==1){W(d->root,0x34)=ov90_022589CC(d->a);ov90_02258938(d->a);d->a=0;*state=5;}break;case 3:d->b=ov90_022589F8(d->root,d->root+16,0x75);(*state)++;break;case 4:if(ov90_02258AA0(d->b)==1){W(d->root,0x38)=ov90_02258AA4(d->b);ov90_02258A04(d->b);d->b=0;*state=5;}break;case 5:default:return 1;}return 0;}
