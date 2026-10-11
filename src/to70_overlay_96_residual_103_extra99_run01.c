#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Node {void*p;u32 a:5,busy:1,c:26;u32 other;} Node;
typedef struct Work {void*owner;Node nodes[2];u32 delay:8,kind:4,rest:20;} Work;
extern u8 *ov96_021E8A20(void *ptr);extern void ov96_0220FE38(Node*,u16*,void*);extern int ov96_0220FF68(void*);extern u32 MTRandom(void);extern void ov96_0220FB98(Node*,u8,u8);
void ov96_0220FF88(Work*,void*);
void ov96_02210030(Work*,void*,void*);
typedef struct Group {void*a[2],*b[2],*c,*d;u32 flags;} Group;
typedef struct Big {void*owner;Group g[2];struct{void*obj;u8 pad[24];} large[12];} Big;
extern void*PokeathlonDiscCatch_NewEffectSprite(void*,void*,int,int,int,int),*PokeathlonDiscCatch_NewDiscSprite(void*,void*,int,int,int,int,int);
void ov96_0221007C(Big*,void*,void*,void*);
typedef struct Data {u8 pad0[0x1c];u32 unused:24,count:6,index:2;u32 value:7,unused2:25;} Data;
typedef struct Status {void*owner;u8 pad4[0x188];u16 previous[2];} Status;
extern void*PokeathlonCourse_GetDataCopyArea(void*),*PokeathlonCourse_GetHeapAllocPtr4(void*);extern void PokeathlonDiscCatch_UpdateDiscSprites(Status*,Data*,void*),PokeathlonDiscCatch_UpdateMonSprites(Status*,Data*,void*),ov96_02210390(void*,u8,u16),ov96_0221040C(void*,int),PokeathlonCourse_UpdateTimeUpCountdown(void*,int);
void ov96_0221013C(Status*,void*);
void ov96_0220FF88(Work*w,void*a){int count=1;w->kind=ov96_0220FF68(a);switch(w->kind){case 0:case 2:w->delay=30;break;case 1:case 3:w->delay=0;break;}u8 mode;if(w->kind<3)mode=4;else mode=((MTRandom()&1)+1)*4;if(w->kind!=0)count=2;for(int i=0;i<count;i++){if(!w->nodes[i].busy){ov96_0220FB98(&w->nodes[i],mode,(u8)i);return;}}}
