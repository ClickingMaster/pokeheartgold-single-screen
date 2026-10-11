#pragma force_active on
#include "global.h"
typedef struct {u8 p[16];u8 mode;u8 q[0x74-17];void*party;u8 r[0x330-0x78];u8 records[1][56];}Work;
extern void SaveArray_Party_Init(void*),Heap_Free(void*),FrontierMon_ToPokemon(void*,void*,int),ov80_022383A8(Work*,void*,void*),SetMonData(void*,int,void*);
extern int BattleArcade_GetOpponentMonCount(int,int),ov80_02238370(Work*);extern void*AllocMonZeroed(int),*Party_GetMonByIndex(void*,int);
void ov80_022383C0(Work*);
void ov80_022383C0(Work*w){int n,i;u32 zero;void*mon;SaveArray_Party_Init(w->party);n=BattleArcade_GetOpponentMonCount(w->mode,1);mon=AllocMonZeroed(11);for(i=0;i<n;i++){FrontierMon_ToPokemon(w->records[i],mon,ov80_02238370(w));ov80_022383A8(w,w->party,mon);{void*member=Party_GetMonByIndex(w->party,i);zero=0;SetMonData(member,6,&zero);}}Heap_Free(mon);}
