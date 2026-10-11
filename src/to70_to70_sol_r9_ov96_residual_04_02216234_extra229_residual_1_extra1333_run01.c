#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct{u32 x:8,y:8,held:1,rest:15;u32 p[7];u32 complete;} Input;
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *),*PokeathlonCourse_GetDataCopyArea(void *);
extern u8 *ov96_021E8A20(void *ptr);
extern void ov96_02217820(void *),ov96_021EB144(void *,int),ov96_0221964C(void *),PokeathlonCourse_SetFinishCallback(void *,void (*)(void)),ov96_02216200(void),PokeathlonCourse_AddMonCounter(void *,u8,int,int,int);
extern int System_GetTouchNew(void),System_GetTouchHeld(void),ov96_021E5F24(void *);
int ov96_02216234(void *);
int ov96_02216234(void *ctx){u32 *w=PokeathlonCourse_GetHeapAllocPtr4(ctx);u8 *data=PokeathlonCourse_GetDataCopyArea(ctx);Input *first=(Input *)ov96_021E8A20(data);Input *second=(Input *)ov96_021E8A20(data+0xf0);if((second->complete>>31)==1){ov96_02217820((u8 *)w+0x43c);ov96_021EB144((void *)w[7],1);ov96_0221964C((void *)w[0x60]);PokeathlonCourse_SetFinishCallback(ctx,ov96_02216200);return 1;}if(System_GetTouchNew())PokeathlonCourse_AddMonCounter(ctx,ov96_021E5F24(ctx),3,0,1);if(System_GetTouchHeld()){first->x=gSystem.touchX;first->y=gSystem.touchY;first->held=1;}else first->held=0;return 0;}
