#pragma force_active on
#include "global.h"
typedef struct {void*arg;u8 pad[12];void*other;}Work;
typedef struct {u32 a;u8*window;}Other;
extern void*PokeathlonUtil_GetMessageFormat(void*);
extern void BufferPokeathlonMedalName(void*,int,u8),BufferPokeathlonCourseName(void*,int,u8),ov98_0221EC08(void*,int,int,int,int),ov98_0221EBD8(void*,int,int,int),FillWindowPixelBuffer(void*,int),ScheduleWindowCopyToVram(void*);
extern int ov99_021E5B70(void*),ov99_021E5B90(void*,int),ov98_0221EF64(int);
void ov99_021E6188(Work*,int);
void ov99_021E6188(Work*w,int n){Other*other=w->other;void*fmt=PokeathlonUtil_GetMessageFormat(other);int v;BufferPokeathlonMedalName(fmt,0,(u8)n);ov98_0221EC08(w->other,1,4,1,0);BufferPokeathlonCourseName(fmt,0,(u8)n);ov98_0221EBD8(w->other,3,3,1);if(ov99_021E5B70(w->arg)){v=ov98_0221EF64(ov99_021E5B90(w->arg,n));if(v==0)ov98_0221EBD8(w->other,4,5,1);else if(v==1)ov98_0221EBD8(w->other,4,6,1);else{FillWindowPixelBuffer(other->window+64,0);ScheduleWindowCopyToVram(other->window+64);}}}
