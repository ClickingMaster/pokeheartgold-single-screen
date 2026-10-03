#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Row{u8 p[0x28];u8 kind,pad;}Row;typedef struct State{Row*items;u8 p4[0x34];void*task;u8 p3c[4];u16 max;u8 p42[0xe];s8 selected;u8 count,target,rows,busy,delay,p56,blocked,warn,repeat;}State;typedef struct Global{u8 selected,pad;u16 position;State*state;}Global;extern Global ov13_0224F538;extern void ov13_0223ABB8(int),ov13_02230F54(void),ov13_0223959C(void),ov13_022272E4(void),ov13_0223BE6C(int,int,int,int,int),ov13_0222B8E8(void(*)(void)),ov13_02230F84(void),ov13_02230520(void);extern int ov13_02227258(void),FX_DivS32(int,int);void ov13_02230414(void);
typedef struct Box{u32 a,b;}Box;extern Box ov13_02245958,ov13_022453FC[];extern int ov13_022417DC(Box*),ov13_022418EC(Box*),ov13_02241694(int),ov13_022416D4(int),ov13_02241714(int);extern void ov13_0222726C(int),ov13_02230CC4(void),ov13_02230ED0(void),ov13_02230E5C(void),ov13_02230D10(int);void ov13_0222FF50(void);
void ov13_02230414(void){int result,index;State*s;if(ov13_0224F538.state->task||ov13_0224F538.state->blocked)return;result=ov13_02227258();switch(result){case 0:ov13_0223ABB8(7);break;case 1:if(ov13_0224F538.selected==4){ov13_0224F538.state->busy=1;ov13_0223ABB8(6);ov13_02230F54();}else{index=FX_DivS32(ov13_0224F538.position,28)+ov13_0224F538.selected;s=ov13_0224F538.state;if(s->items[index].kind==2){ov13_0223ABB8(9);ov13_0223959C();ov13_022272E4();ov13_0223BE6C(14,1,1,-1,0);ov13_0222B8E8(ov13_02230F84);return;}s->busy=1;ov13_0224F538.state->target=index;ov13_0223ABB8(6);}break;default:return;}ov13_0222B8E8(ov13_02230520);}
