#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Node {void*p;u32 a:5,busy:1,c:26;u32 other;} Node;
typedef struct Work {void*owner;Node nodes[2];u32 delay:8,kind:4,rest:20;} Work;
extern u16* ov96_021E8A20(void*);extern void ov96_0220FE38(Node*,u16*,void*);extern int ov96_0220FF68(void*);extern u32 MTRandom(void);extern void ov96_0220FB98(Node*,u8,u8);
void ov96_0220FF88(Work*,void*);
void ov96_02210030(Work*,void*,void*);
typedef struct Group {void*a[2],*b[2],*c,*d;u32 flags;} Group;
typedef struct Big {void*owner;Group g[2];struct{void*obj;u8 pad[24];} large[12];} Big;
extern void*ov96_0220ED34(void*,void*,int,int,int,int),*ov96_0220ECA4(void*,void*,int,int,int,int,int);
void ov96_0221007C(Big*,void*,void*,void*);
typedef struct Data {u8 pad0[0x1c];u32 unused:24,count:6,index:2;u32 value:7,unused2:25;} Data;
typedef struct Status {void*owner;u8 pad4[0x188];u16 previous[2];} Status;
extern void*PokeathlonCourse_GetDataCopyArea(void*),*PokeathlonCourse_GetHeapAllocPtr4(void*);extern void ov96_0220DC7C(Status*,Data*,void*),ov96_0220DEAC(Status*,Data*,void*),ov96_02210390(void*,u8,u16),ov96_0221040C(void*,int),ov96_021E6454(void*,int);
void ov96_0221013C(Status*,void*);
void ov96_0221013C(Status*w,void*course){Data*d=(Data*)ov96_021E8A20((u8*)PokeathlonCourse_GetDataCopyArea(course)+0xf0);void*heap=PokeathlonCourse_GetHeapAllocPtr4(course);ov96_0220DC7C(w,d,course);ov96_0220DEAC(w,d,course);u8 index=(u8)d->index;if(d->value!=w->previous[index])ov96_02210390(w->owner,index,(u16)d->value);w->previous[index]=d->value;int count=d->count-1;if(count<=0)count=0;if(count<=0)count=0;ov96_0221040C(*(void**)((u8*)heap+0xc4),count);ov96_021E6454(course,30*d->count);}
