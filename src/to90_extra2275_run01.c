#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[17];u8 selected;u8 q[34];u8 option;}Header;typedef struct{Header*header;u8 p[0x2088-4];void*menu;u8 a,b,c;}Work;extern const u32 ov08_022254BC[];void BattleDpadCursor_SetBoxes(void*,u32),BattleDpadCursor_SetPosition(void*,int),ov08_022216CC(Work*),ov08_022216F4(Work*);void ov08_0222171C(Work*,u32);
void ov08_0222171C(Work*w,u32 kind){BattleDpadCursor_SetBoxes(w->menu,ov08_022254BC[kind]);switch(kind){case 0:BattleDpadCursor_SetPosition(w->menu,w->header->selected);w->a=0;w->header->option=0;break;case 1:BattleDpadCursor_SetPosition(w->menu,w->a);w->header->option=0;break;case 3:case 4:BattleDpadCursor_SetPosition(w->menu,w->header->option);break;case 6:case 8:ov08_022216CC(w);BattleDpadCursor_SetPosition(w->menu,w->b);break;case 7:case 9:ov08_022216F4(w);BattleDpadCursor_SetPosition(w->menu,w->c);break;}}
