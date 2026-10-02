#pragma force_active on
#include "global.h"
#pragma force_active on
u8 *PokeathlonCourse_GetDataCopyArea(void *),*ov96_021E8A20(void *);void *PokeathlonCourse_GetHeapAllocPtr4(void *);int ov96_021E5F24(void *);
void ov96_021E8318(void *,int),ov96_02207774(void *,int),ov96_02207C64(void *);
void *Heap_Alloc(int,u32),*NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*String_New(int,int);void ov96_02208A4C(void *),CopyU16ArrayToString(void *,void *),ov96_022088AC(void *,int);u8 *PokeathlonCourse_GetParticipantUnk04(void *,int),*ov96_02207CCC(int,void *,void *);
void ov96_02207C64(void *course){u8 *p=ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course)+240);void *aux=PokeathlonCourse_GetHeapAllocPtr4(course);u8 index=(u8)ov96_021E5F24(course);u16 score=(u16)(((*(u32 *)(p+index*4)>>18)&1023));u8 extra=(u8)(((*(u16 *)(p+24+index*2)>>11)&31));score=(u16)(score+extra);if(score>999)score=999;ov96_021E8318(course,score);ov96_02207774(aux,score);}
