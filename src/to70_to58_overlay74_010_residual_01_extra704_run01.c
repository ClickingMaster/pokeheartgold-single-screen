#pragma force_active on
#include "global.h"
typedef struct {u32 v[59];} Mon;typedef struct {int box,slot,x;} Entry;
typedef struct {u8 p[0x10];void *save;u8 q[0x3cc-0x14];Entry entries[6];u8 r[0xe880-0x414];u8 *storage;} Work;
extern void *Save_MigratedPokemon_Get(void*);extern void *Mon_GetBoxMon(Mon*);extern void MigrateBoxMon(void*,void*);extern void MigratedPokemon_CopyBoxPokemonToSlot(void*,void*,int);extern void ov74_022348B0(void*,int,void*);void ov74_0223195C(Work*);
void ov74_0223195C(Work*w){Mon mon;u16 zero;void*migrated=Save_MigratedPokemon_Get(w->save);void*box=Mon_GetBoxMon(&mon);int i;for(i=0;i<6;i++){MigrateBoxMon(w->storage+4+w->entries[i].slot*2400+w->entries[i].box*80,box);MigratedPokemon_CopyBoxPokemonToSlot(migrated,box,i);}zero=0;for(i=0;i<6;i++){int slot=w->entries[i].box,bank=w->entries[i].slot;if(slot!=-1&&bank!=14)ov74_022348B0(w->storage+4+bank*2400+slot*80,11,&zero);}}
