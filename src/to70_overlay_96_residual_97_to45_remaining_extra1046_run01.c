#pragma force_active on
#include "global.h"
typedef struct {u8 p[36];} Item;typedef struct {int a;void*b;Item items[3];u32 count;} Work;
extern int ov96_0220CA28(Item*);extern void ov96_0220CF50(Item*,int,void*);
void ov96_0220C9A0(Work*),ov96_0220C9CC(Work*);
void ov96_0220C9A0(Work*w){u32 i=0;Item*p;if(i<w->count){p=w->items;do{ov96_0220CF50(p,ov96_0220CA28(p),w->b);i++;p++;}while(i<w->count);}}
void ov96_0220C9CC(Work*w){u32 i=0;Item*p;if(i<w->count){p=w->items;do{ov96_0220CF50(p,0,w->b);i++;p++;}while(i<w->count);}}
