#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "pokedex.h"
#include "trainer_memo.h"
extern u32 MTRandom(void);
#pragma force_active on
#define V(p,o) (*(void**)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
int ov112_021EE8BC(BoxPokemon*,Pokemon*,u32);void ov112_021EE970(BoxPokemon*,u32),ov112_021EE9A4(Pokemon*,PlayerProfile*,void*,u32,int);
PCStorage*SaveArray_PCStorage_Get(SaveData*);

typedef struct{u8 low:2,remote:1,high:5;}Flags;
typedef struct{u8 gap0[32];SaveData*save;u8 gap1[4291];Flags flags;u8 gap2[35932];u8 remoteData[0x146ec];BoxPokemon*received;u8 gap4[4];PlayerProfile*profile;u8 gap5[4];void*walker;Pokedex*pokedex;u8 gap7[2100];u32 counter;u8 gap8[1780];u16 result;}Work;
void ov112_021EE7A8(Work*);
#pragma opt_propagation off
void ov112_021EE7A8(Work*w){PCStorage*storage=SaveArray_PCStorage_Get(w->save);Pokemon*mon=AllocMonZeroed((enum HeapID)154);BoxPokemon*box=Mon_GetBoxMon(mon);int boxno,slot;u16 pair[2];sub_02032688(w->walker,&pair[0],&pair[1]);boxno=pair[1];slot=0;BOOL ok;if(!w->flags.remote){ok=Pokewalker_TryGetBoxMon(w->walker,box);if(ok)w->result=ov112_021EE8BC(box,mon,w->counter);ov112_021EE970(box,w->counter);w->counter=0;Pokewalker_ClearBoxMon(w->walker);}else{u8*table=w->remoteData;u32 random=MTRandom()%24;ov112_021EE9A4(mon,w->profile,table,random,0);BoxMonSetTrainerMemo(Mon_GetBoxMon(mon),w->profile,0,233,(enum HeapID)154);ok=1;}if(ok){PCStorage_FindFirstEmptySlot(storage,&boxno,&slot);PCStorage_PlaceMonInBoxByIndexPair(storage,boxno,slot,box);w->received=PCStorage_GetMonByIndexPair(storage,boxno,slot);Pokedex_SetMonSeenFlag(w->pokedex,mon);Pokedex_SetMonCaughtFlag(w->pokedex,mon);}Heap_Free(mon);}
