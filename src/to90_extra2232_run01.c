#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;void *course;u8 entries[3][60];u8 count;} State;
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);extern int PokeathlonDiscCatch_GetTeamScore(void *,u8);extern void ov96_022108EC(void *,u32,u16);
void ov96_02210858(State *);
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs off
void ov96_02210858(State *s){void *data=PokeathlonCourse_GetHeapAllocPtr4(s->course);int total=0;u32 walk=0;for(;(s32)walk<4;walk++)total+=PokeathlonDiscCatch_GetTeamScore(data,walk);int j=0;if(j<s->count){u32 adjustment=(u32)(total>>1)>>30;walk=(u32)&s->entries[0];total+=adjustment;u32 shifted=(u32)total<<14;total=shifted>>16;do{ov96_022108EC((void*)walk,total,PokeathlonDiscCatch_GetTeamScore(data,j));j++;walk+=60;}while(j<s->count);}}
