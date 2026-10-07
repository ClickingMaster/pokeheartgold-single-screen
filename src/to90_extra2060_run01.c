#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
typedef struct {u32 lo:16,bit:1,rest:15;} Flags;
typedef struct {u32 low:9,pad:17,type:6;} Item;
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *),*PokeathlonCourse_GetDataCopyArea(void *);
extern u8 *ov96_021E8A20(void *ptr);
extern int ov96_021E5F24(void *),ov96_0220A910(void *),ov96_0220A8CC(u8 *,int);
struct Work;
struct R22Scene;
extern u16 ov96_0220AAEC(struct Work *,int,s16,int,int,int *);
extern u16 ov96_0220AD4C(u8 *);
extern u32 ov96_0220AD34(u8 *),ov96_0220B774(u8 *),ov96_0220B7CC(struct R22Scene *);
extern u8 ov96_0220B7B4(struct R22Scene *);
extern void *ov96_0220B758(u8 *);
extern void ov96_0220A5DC(void *,int),ov96_02209BF8(void *),ov96_0220B708(void *,int),ov96_0220B8F0(void *),ov96_0220A87C(void *),ov96_021EB144(void *,int),ov96_021E8228(void *,u8,int,int,int),ov96_02209E70(void *),ov96_0220A840(void *),ov96_0220B528(void *,int),ov96_0220C9A0(void *),ov96_0220B8D8(void *),ov96_0220A298(void *,int),ov96_0220B634(void *,int,int),ov96_02209BB0(void *),ov96_0220A704(void *,int,int),ov96_021E6454(void *,int);
extern int System_GetTouchNew(void);
int ov96_022091B4(void *);
int ov96_022091B4(void *p){u8 *s=PokeathlonCourse_GetHeapAllocPtr4(p);int state,active;u32 *item;int saved,kind,out;if((((Flags *)((u8 *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(p)+0xf0)+16))->bit)==1){ov96_0220A5DC(p,ov96_0220AD4C(P(s,64)));ov96_02209BF8(s);ov96_0220B708(P(s,76),0);ov96_0220B8F0(P(s,68));ov96_0220A87C(P(s,64));ov96_021EB144(P(s,16),1);return 1;}if(System_GetTouchNew())ov96_021E8228(p,(u8)ov96_021E5F24(p),3,0,1);ov96_02209E70(s);ov96_0220A840(P(s,64));state=ov96_0220AD34(P(s,64));ov96_0220B528(P(s,76),state==1);if(ov96_021E5F24(p)==0)ov96_0220C9A0(P(s,72));ov96_0220B8D8(P(s,68));active=ov96_0220A910(P(s,64));ov96_0220A298(s,active);if(ov96_0220B7CC((struct R22Scene *)P(s,76))&&state==2&&active){item=(u32 *)ov96_0220B758(P(s,76));saved=ov96_0220B774(P(s,76));kind=ov96_0220A8CC(P(s,64),(int)((Item *)item)->type);{int value=ov96_0220AAEC((struct Work *)P(s,64),ov96_0220B7B4((struct R22Scene *)P(s,76)),(s16)(u16)(((Item *)item)->low),kind,saved,&out);ov96_0220B634(P(s,76),kind,value);}ov96_02209BB0(s);if(kind)ov96_0220A704(s,16,2);else if(out)ov96_0220A704(s,4,1);else if(ov96_0220B774(P(s,76)))ov96_0220A704(s,4,1);}if((int)W(s,0x260)>0)W(s,0x260)--;ov96_021E6454(p,W(s,0x260));if(W(s,0x260)==0&&ov96_021E5F24(p)==0){void *x=(void *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(p)+40);W(x,16)|=1<<16;}return 0;}
