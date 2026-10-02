#pragma force_active on
#include "global.h"
extern void CopyPokemonToPokemon(void *,void *);
extern u32 GetMonData(void *,int,void *);
typedef struct {int count;void *mon[6];} Party;
typedef struct {Party *party;u32 selection[2];u8 p[0x3ec-12];u32 index;u8 p2[0x454-0x3f0];void *selected,*fallback;} Work;
void ov57_0223B7C4(Work *w);
void ov57_0223B7C4(Work *w){int n=w->selection[w->index*2];int i;if(n!=255){CopyPokemonToPokemon(w->party->mon[n],w->fallback);return;}for(i=0;i<w->party->count;i++){void *mon=w->party->mon[i];if(GetMonData(mon,76,0)==0){CopyPokemonToPokemon(mon,w->fallback);return;}}}
