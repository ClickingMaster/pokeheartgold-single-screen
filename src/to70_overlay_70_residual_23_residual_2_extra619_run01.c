#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 species;union {struct {u8 gender,level,extra;};u16 pair[2];};} Info;
typedef struct {u8 p[0xec];u16 info[5];} Work;
typedef struct {u8 p[0x124];void*box;u32 unused;int index;u8 p2[0x130];u8 mons[7][0x124];} Arg;
extern u32 GetBoxMonData(void*,int,void*);
extern u8 CalcBoxMonLevel(void*);
extern void* Mon_GetBoxMon(void*);
extern void ov70_0223F6E4(Work*,Arg*);
void ov70_0223E690(Work*,Arg*);
void ov70_0223E690(Work*w,Arg*a){Info first;Info second;void*box;first.species=GetBoxMonData(a->box,5,0);first.gender=GetBoxMonData(a->box,0x6f,0)+1;first.level=CalcBoxMonLevel(a->box);w->info[0]=first.species;w->info[1]=first.pair[0];ov70_0223F6E4(w,a);box=Mon_GetBoxMon(a->mons[a->index]);second.species=GetBoxMonData(box,5,0);second.gender=GetBoxMonData(box,0x6f,0)+1;second.level=0;second.extra=0;w->info[2]=second.species;w->info[3]=second.pair[0];w->info[4]=second.pair[1];}
