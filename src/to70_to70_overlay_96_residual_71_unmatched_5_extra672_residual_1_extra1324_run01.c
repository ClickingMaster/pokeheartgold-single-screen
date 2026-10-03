#pragma force_active on
#include "global.h"
extern int ov96_021E5F24(void*),PokeathlonCourse_GetParticipantCount(void*);extern void *PokeathlonCourse_GetDataCopyArea(void*),*PokeathlonCourse_GetHeapAllocPtr4(void*),*ov96_021E8A20(void*);extern void ov96_021FBA3C(void*,void*,void*,u8,u8,u8);
void ov96_021FB630(void*w);
void ov96_021FB630(void*w){u8*area;void*heap;int count,i,j,k;void*p;u8*c;u8 idx;if(ov96_021E5F24(w)==0){area=PokeathlonCourse_GetDataCopyArea(w);heap=PokeathlonCourse_GetHeapAllocPtr4(w);count=PokeathlonCourse_GetParticipantCount(w);i=count;if(i<4){c=area+80+i*40;do{p=ov96_021E8A20(c);j=0;k=(i-count)*3;idx=(u8)i;do{ov96_021FBA3C(w,heap,p,idx,(u8)j,(u8)k);j++;k++;}while(j<3);c+=40;i++;}while(i<4);}}}
