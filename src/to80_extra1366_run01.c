#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "party.h"
extern BoxPokemon *sub_0208A520(u8 *);
extern void sub_0208B780(void *,int,int,int,int,int,int),sub_0208B714(u8 *);
typedef struct{u16 speed:3,stamina:3,power:3,skill:3,jump:3,pad:1;u8 color[5],dummy;} Stars;
typedef struct{Stars stars;PartyExtraSub juice;u8 pad;struct PokeathlonTodayPerformance perf;} Local;
void sub_0208B89C(u8 *);
void sub_0208B89C(u8 *w){Local data;BoxPokemon *mon;if(*(s8 *)(w+0x7bc)==2&&*(u32 *)((u8 *)*(void **)(w+0x22c)+0x34)){mon=sub_0208A520(w);MI_CpuFill8(&data.juice,0,5);CalcBoxMonPokeathlonPerformance(mon,&data.perf);if(*((u8 *)*(void **)(w+0x22c)+0x11)==1)Party_GetMonAprijuiceModifiers(*(Party **)*(void **)(w+0x22c),&data.juice,*((u8 *)*(void **)(w+0x22c)+0x14));CalcBoxmonPokeathlonStars((struct PokeathlonPerformanceStars *)&data.stars,mon,(s8 *)&data.juice,(enum HeapID)19);sub_0208B780(w+0x404,79,data.perf.stats[4].hi,data.stars.jump,data.stars.color[4],(s8)data.juice.unk_00[4],104);sub_0208B780(w+0x404,84,data.perf.stats[0].hi,data.stars.speed,data.stars.color[0],(s8)data.juice.unk_00[0],105);sub_0208B780(w+0x404,89,data.perf.stats[2].hi,data.stars.power,data.stars.color[2],(s8)data.juice.unk_00[2],106);sub_0208B780(w+0x404,94,data.perf.stats[1].hi,data.stars.stamina,data.stars.color[1],(s8)data.juice.unk_00[1],107);sub_0208B780(w+0x404,99,data.perf.stats[3].hi,data.stars.skill,data.stars.color[3],(s8)data.juice.unk_00[3],108);}else sub_0208B714(w);}
