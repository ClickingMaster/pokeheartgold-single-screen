#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "party.h"
#include "move.h"
typedef struct {u8 pre[13];u8 choice;u8 mid[6];u8 slot;u8 pad[0x78f];Party *party;u8 gap[0x5c];Pokemon *mon;BoxPokemon *box;u16 species;u8 gender:7;u8 flag:1;u8 level,ability,nature;u16 item;u32 personality;u16 hp,maxhp,attack,defense,speed,spattack,spdef;u8 mail;u8 pad2;u16 moves[4];u8 pp[4],maxpp[4];} Context;
extern s32 ov83_02247768(u8,u8);
void ov83_02241E18(Context *p);
#pragma force_active on
void ov83_02241E18(Context *p){
 Pokemon *mon=Party_GetMonByIndex(p->party,ov83_02247768(p->slot,p->choice));
 BOOL lock=AcquireMonLock(mon);u16 i;
 p->mon=mon;p->box=Mon_GetBoxMon(mon);
 p->species=GetMonData(mon,5,NULL);p->level=GetMonData(mon,161,NULL);p->ability=GetMonData(mon,10,NULL);p->nature=GetMonNature(mon);p->item=GetMonData(mon,6,NULL);
 p->hp=GetMonData(mon,163,NULL);p->maxhp=GetMonData(mon,164,NULL);p->attack=GetMonData(mon,165,NULL);p->defense=GetMonData(mon,168,NULL);p->speed=GetMonData(mon,166,NULL);p->spattack=GetMonData(mon,169,NULL);p->spdef=GetMonData(mon,167,NULL);p->mail=GetMonData(mon,112,NULL);p->personality=GetMonData(mon,0,NULL);
 if(GetMonData(mon,176,NULL)==1)p->flag=0;else p->flag=1;
 p->gender=GetMonGender(mon);
 for(i=0;i<4;i++){p->moves[i]=GetMonData(mon,54+i,NULL);p->pp[i]=GetMonData(mon,58+i,NULL);{u16 ups=GetMonData(mon,62+i,NULL);p->maxpp[i]=GetMoveMaxPP(p->moves[i],ups);}}
 ReleaseMonLock(mon,lock);
}
