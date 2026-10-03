#pragma force_active on
#include "global.h"
typedef struct{int heap;void *ctx,*other;u8 p[0x10];void *msg,*format;u8 p2[0x184];void *names[3];} Work;
extern void *Heap_Alloc(int,int),*NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*String_New(int,int),*PokeathlonCourse_GetParticipantUnk04(void *,int);
extern int ov96_021E5F24(void *);extern void ov96_02208A4C(Work *),CopyU16ArrayToString(void *,u16 *),ov96_022088AC(Work *,int);
Work *ov96_02207CCC(int,void *,void *);
Work *ov96_02207CCC(int heap,void *other,void *ctx){Work *w=Heap_Alloc(heap,456);int i;u8 *data;MI_CpuFill8(w,0,456);w->heap=heap;w->other=other;w->ctx=ctx;ov96_02208A4C(w);w->msg=NewMsgDataFromNarc(1,27,309,heap);w->format=MessageFormat_New(heap);for(i=0;i<3;i++)w->names[i]=String_New(11,heap);data=PokeathlonCourse_GetParticipantUnk04(ctx,ov96_021E5F24(ctx));for(int j=0;j<3;j++){CopyU16ArrayToString(w->names[j],(u16 *)(data+0x12));data+=0x28;}ov96_022088AC(w,1);return w;}
