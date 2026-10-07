#pragma force_active on
#include "global.h"
extern int ov96_021E5F24(void *);
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern int PokeathlonCourse_GetParticipantCount(void *);
extern u8 *ov96_021E8A20(void *ptr);
extern void ov96_021FBA3C(void *,void *,void *,u8,u8,u8);
extern int ov96_021FC0FC(void *,void *,int);
void ov96_021FB630(void *);
int ov96_021FB6B4(void *,void *,int);
int ov96_021FB6B4(void *w,void *data,int arg){return ov96_021FC0FC(*(void**)((u8*)w+0xd8),(u8*)data+0x150,arg);}
