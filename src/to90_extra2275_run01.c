#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[17];u8 selected;u8 q[34];u8 option;}Header;typedef struct{Header*header;u8 p[0x2088-4];void*menu;u8 a,b,c;}Work;extern const u32 ov08_022254BC[];void ov08_02224BCC(void*,u32),ov08_02224B98(void*,int),ov08_022216CC(Work*),ov08_022216F4(Work*);void ov08_0222171C(Work*,u32);
void ov08_0222171C(Work*w,u32 kind){ov08_02224BCC(w->menu,ov08_022254BC[kind]);switch(kind){case 0:ov08_02224B98(w->menu,w->header->selected);w->a=0;w->header->option=0;break;case 1:ov08_02224B98(w->menu,w->a);w->header->option=0;break;case 3:case 4:ov08_02224B98(w->menu,w->header->option);break;case 6:case 8:ov08_022216CC(w);ov08_02224B98(w->menu,w->b);break;case 7:case 9:ov08_022216F4(w);ov08_02224B98(w->menu,w->c);break;}}
