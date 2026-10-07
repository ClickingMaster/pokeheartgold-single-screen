#include "global.h"
#pragma force_active on
typedef struct Ov96R60Node Ov96R60Node;
typedef struct {u8 prefix[100];void**first;void**items[4];u8 rest[20];Ov96R60Node*aux;u8 remaining90[0x1004-0x90];} Work;
extern const VecFx32 ov96_0221C0C4,ov96_0221C0DC,ov96_0221C0D0;
extern void ov96_021F7544(Ov96R60Node*),ov96_021EB588(void**,const void*);
typedef char WorkExtentIsActualAllocation[(sizeof(Work)==0x1004)?1:-1];
void ov96_021F6DA4(void*,u8);
// @fn ov96_021F6DA4
void ov96_021F6DA4(void *context,u8 index){Work*w=context;VecFx32 a,b,temp,c;int i;u8 n;ov96_021F7544(w->aux);a=ov96_0221C0C4;ov96_021EB588(w->first,&a);b=ov96_0221C0DC;ov96_021EB588(w->items[index],&b);c=ov96_0221C0D0;n=0;for(i=0;i<4;i++){temp=c;if(i!=index){temp.y+=(u32)n<<16;ov96_021EB588(w->items[i],&temp);n++;}}}
