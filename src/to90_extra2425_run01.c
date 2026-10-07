#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *obj,*effect;fx32 x,y;u32 x10,x14,x18;fx32 position;u8 pad20[6];u8 state;u8 pad27[0x11];}Row;
typedef struct {u8 pad0[0x78];void *effects[3];u8 pad84[0xc];Row rows[3];u8 pad138[0xc];VecFx32 positions[3];u8 flag[3];u8 rest[0xe];}Work;
extern const u8 ov96_0221DC18[];
extern Work *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern u32 ov96_021E5F24(void *);
extern u8 *ov96_021E60C0(void *,u32,int);
extern void ov96_021EAF94(void *,int,int),ov96_021EB588(void *,VecFx32 *),ov96_021EB52C(void *,int,int),ov96_021EAD08(void *,int),sub_0200606C(int,int),ov96_021EAC0C(void *,int),ov96_021EB564(void *,int);
extern int sub_02006190(int);
void ov96_021F5D3C(void *);
static inline void Zero(VecFx32 *v){v->x=0;v->y=0;v->z=0;}
void ov96_021F5D3C(void *ctx){int i,x,y;Work *w=PokeathlonCourse_GetHeapAllocPtr4(ctx);Row *r;u8 *offset,*store;const u8 *audio;for(i=0,r=w->rows,offset=(u8 *)w,store=(u8 *)w,audio=ov96_0221DC18;i<3;r++,offset+=0x1c,store+=4,audio++,i++){r->y=0x120000-*(int *)(offset+0xfb4)*64;r->x=r->position;if(r->x<0x20000)r->x=0x20000;else if(r->x>0xdf000)r->x=0xdf000;*(int *)(store+0xfa0)=(u8)((r->x/4096)/2);x=r->x/4096;y=r->y/4096;ov96_021EAF94(r->obj,x,y);{VecFx32 pos={0,0,0};pos.x=x<<12;pos.y=y<<12;if(ov96_021E60C0(ctx,ov96_021E5F24(ctx),i)[5])pos.y-=0x18000;else pos.y-=0x10000;ov96_021EB588(r->effect,&pos);}if(r->state){ov96_021EB52C(r->effect,1,1);ov96_021EAD08(r->obj,20);if(!sub_02006190(*audio))sub_0200606C(0x890,*audio);}else{ov96_021EB52C(r->effect,1,0);ov96_021EAD08(r->obj,0);if(*(int *)(offset+0xfc0))ov96_021EAC0C(r->obj,1);else ov96_021EAC0C(r->obj,2);}}
 for(i=0;i<3;i++)if(w->flag[i]){w->flag[i]=0;ov96_021EB52C(w->effects[i],1,1);ov96_021EB588(w->effects[i],&w->positions[i]);ov96_021EB564(w->effects[i],9);}}
