#pragma force_active on
#include "global.h"
typedef struct {u32 v[29];} Block;typedef struct {void*save;} Context;typedef struct {u8 p[0x1f8];Context*context;u8 q[0x288-0x1fc];int arg;u8 r[0x900-0x28c];Block block;} Work;
extern void*Save_Pokeathlon_Get(void*);extern Block*PokeathlonSave_GetAgainUnkB00(void*);extern int ov96_021E9A10(void),ov96_021E87EC(int,Block*,int,int);extern void PokeathlonCourse_SetStateField07(Work*,int);
int ov96_021E6A00(Work*);
int ov96_021E6A00(Work*w){w->block=*PokeathlonSave_GetAgainUnkB00(Save_Pokeathlon_Get(w->context->save));if(ov96_021E87EC(26,&w->block,ov96_021E9A10(),w->arg))PokeathlonCourse_SetStateField07(w,38);return 0;}
