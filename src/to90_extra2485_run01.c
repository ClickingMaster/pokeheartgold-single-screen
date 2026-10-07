#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x1d54];u8 input[0x1b30];volatile s32 count;}W40461Sub;typedef struct{u8 pad[0x8b4];W40461Sub sub;}W40461Owner;typedef struct{u8 pad[0xa7];u8 flag;u8 rest[60];}W40461Record;typedef struct{u8 pad[0x2608];W40461Record*record;}W40461View;typedef struct{u8 pad[0x2680];W40461Record records[30];u8 gap[0x413c-0x2680-228*30];u8 flags[30];}W40461Output;void*ov40_0223D540(void*);int ov39_0222801C(void*,void*,int);void ov40_0222FC4C(void*,int*);
#pragma opt_strength_reduction off
void ov40_0222FC4C(void*w,int*state){W40461Sub*sub=&((W40461Owner*)w)->sub;if(*state!=1){int result=ov39_0222801C(ov40_0223D540(w),sub->input,30);int i=0;sub->count=result;if(sub->count>0){W40461View*source=w;W40461Record*dest=((W40461Output*)w)->records;do{GF_ASSERT(source->record);MI_CpuCopy8(source->record,dest,228);((W40461Output*)w)->flags[i]=source->record->flag;source=(void*)((u8*)source+4);dest++;i++;}while(i<sub->count);}}}
