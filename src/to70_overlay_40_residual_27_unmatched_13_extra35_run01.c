#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Anim {int blend;float values[5];int positions[5];int angles[5];u8 pad40[0x50];u32 points;u8 pad94[0x40];int a,b;} Anim;
typedef struct Work {u8 pad0[8];u32 state;u8 padC[0x18];void *bg,*palette;u8 pad2C[0x2c];u32 color;u8 pad5C[0x694];void *object;u8 pad6F4[0x13c];void *save;u8 pad834[0x2c];Anim *anim;} Work;
extern void *Heap_Alloc(int,u32);
extern void BgClearTilemapBufferAndCommit(void*,int);
extern void ov40_0222D9E8(int*,int*,int);
extern void ov40_0222D980(int*,int*,int,int,int,int,int);
extern void PlaySE(int);
extern int ov40_0222DA84(Anim*,int);
extern void *Save_FrontierData_Get(void*);
extern u32 FrontierData_BattlePointAction(void*,int,int);
extern void PaletteData_BlendPalettes(void*,int,u16,u8,u16);
extern void ov40_02230964(Work*,int);
extern void ov40_0222D874(Work*);
extern void ov40_0222FB90(Work*,int);
extern void sub_020879E0(void*,int);
extern void ov40_02230738(void);
extern void SetBgPriority(int,int);
extern void ov40_02233238(Work*);
extern int ov40_0222FBB4(Work*);
extern void ov40_0223316C(Work*);
extern void ov40_022335F4(Work*);
extern void ov40_02233550(Work*);
extern void GfGfx_EngineBTogglePlanes(int,int);
extern int ov40_022339CC(Work*);
extern void ov40_0222DA00(int*,int*,int,int);
extern void ov40_02233C3C(Work*);
extern void ov40_0222BF80(Work*,int);
int ov40_02233CAC(Work*);
int ov40_02233CAC(Work*w){Anim *a;if(w->state==0){a=Heap_Alloc(109,sizeof(*a));MI_CpuFill8(a,0,sizeof(*a));w->anim=a;a->blend=0;int i=0;Anim *p=a;int angle=0;for(;i<5;i++,p=(Anim*)((u8*)p+4),angle+=72){if((u32)(i-2)<=1)p->positions[0]=52;else p->positions[0]=64;p->angles[0]=angle;p->values[0]=0.2f;}BgClearTilemapBufferAndCommit(w->bg,3);BgClearTilemapBufferAndCommit(w->bg,2);BgClearTilemapBufferAndCommit(w->bg,7);BgClearTilemapBufferAndCommit(w->bg,6);ov40_0222D9E8(&a->a,&a->b,0);ov40_0222D980(&a->a,&a->b,0,0,4,2,0);PlaySE(0x579);w->state++;}else a=w->anim;switch(w->state){case 1:if(ov40_0222DA84(a,1)){a->points=FrontierData_BattlePointAction(Save_FrontierData_Get(w->save),0,0);w->state++;}PaletteData_BlendPalettes(w->palette,2,12,(u8)a->blend,(u16)w->color);PaletteData_BlendPalettes(w->palette,3,12,(u8)a->blend,(u16)w->color);PaletteData_BlendPalettes(w->palette,1,2,(u8)a->blend,(u16)w->color);break;case 2:ov40_02230964(w,1);ov40_0222D874(w);ov40_02230964(w,0);ov40_0222FB90(w,0);sub_020879E0(w->object,0);ov40_02230738();SetBgPriority(6,2);ov40_02233238(w);PaletteData_BlendPalettes(w->palette,3,28,(u8)a->blend,(u16)w->color);w->state++;break;case 3:if(ov40_0222FBB4(w)){ov40_02230964(w,1);ov40_0223316C(w);ov40_022335F4(w);ov40_02233550(w);ov40_02230964(w,0);GfGfx_EngineBTogglePlanes(8,1);GfGfx_EngineBTogglePlanes(4,1);w->state++;}break;case 4:{int result=ov40_022339CC(w);ov40_0222DA00(&a->a,&a->b,0,0);ov40_0222DA84(a,0);if(result==0){w->state++;ov40_02233C3C(w);}PaletteData_BlendPalettes(w->palette,3,28,(u8)a->blend,(u16)w->color);break;}default:ov40_0222BF80(w,1);break;}return 0;}
