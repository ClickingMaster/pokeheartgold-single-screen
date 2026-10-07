#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x2ec];void *window;} Work;
extern void sub_0200CE7C(void *,int,u16,int,int,int,int,int);extern void ov15_021FE8C4(Work *,u16,u16,u32);extern void PrintUIntOnWindow(void *,u16,int,int,int,int,int);extern void ov15_021FE9B0(Work *,int,int);
void ov15_021FE914(Work *,int,u16 *,int);
#pragma opt_propagation off
void ov15_021FE914(Work *w,int y,u16 *entry,int x){u16 item=entry[0];if(item<420){u16 number=item-327;sub_0200CE7C(w->window,2,number,2,2,y,0,x+5);ov15_021FE8C4(w,entry[1],(u16)x,0x10200);}else{u16 number=item-419;PrintUIntOnWindow(w->window,number,2,1,y,16,x+5);ov15_021FE9B0(w,y,16);}}
