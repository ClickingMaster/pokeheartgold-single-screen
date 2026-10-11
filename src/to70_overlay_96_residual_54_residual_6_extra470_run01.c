#pragma force_active on
#include "global.h"
typedef struct {u16 index,value;} Pair;
extern int ov96_021F2FBC(const void *,const void *,void *);
extern void ov96_021EB06C(void *,u8,u8,int *,int *),PokeathlonCourseMonObject_SetDrawPriority(void *,int);
extern void MATH_QSort(void *,u32,u32,int (*)(const void *,const void *,void *),void *);
void ov96_021F2FEC(void *,u8 *);
extern void MATH_QSort(void *,u32,u32,int (*)(const void *,const void *,void *),void *);
void ov96_021F2FEC(void *w,u8 *data){Pair pairs[12];int a,b;u8 i;for(i=0;i<12;i++){pairs[i].index=i;ov96_021EB06C(*(void**)((u8*)w+(i/3)*0x1b0+(i%3)*0x90+0x20),data[i],data[12+i],&a,&b);pairs[i].value=b;}MATH_QSort(pairs,12,4,ov96_021F2FBC,*(void**)((u8*)w+0x7f0));for(i=0;i<12;i++){u8 index=pairs[i].index;int row=index/3;int col=index%3;PokeathlonCourseMonObject_SetDrawPriority(*(void**)((u8*)w+row*0x1b0+col*0x90+0x20),i+6);}}
