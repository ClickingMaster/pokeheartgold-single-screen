#pragma force_active on
#include "global.h"
#include <string.h>
extern int ov96_021E5F24(void*);extern void *PokeathlonCourse_GetParticipantData(void*,int);extern void PokeathlonCourse_IncrementField1EF(void*);extern int PokeathlonCourse_GetParticipantCount(void*);extern int PokeathlonCourse_GetField1EF(void*);extern void ov96_021E8484(void*,u8);extern void PokeathlonCourse_ResetField1EF(void*);extern void PokeathlonCourse_SetStateField07(void*,int);
void ov96_021E9718(int,int,void*,void*);
void ov96_021E9718(int id,int size,void*data,void*w) {int count;if(ov96_021E5F24(w)==0){memcpy(PokeathlonCourse_GetParticipantData(w,id),data,size);PokeathlonCourse_IncrementField1EF(w);count=PokeathlonCourse_GetParticipantCount(w);if(count==PokeathlonCourse_GetField1EF(w)){if((u32)PokeathlonCourse_GetField1EF(w)<4){u32 missing=4-PokeathlonCourse_GetField1EF(w);ov96_021E8484(w,(u8)missing);}PokeathlonCourse_ResetField1EF(w);PokeathlonCourse_SetStateField07(w,4);}}}
