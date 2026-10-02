#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x1c4];void*mgr,*pic;u32 y;u8 pad2[0xb4];void*anim;u8 pad3[0x1d0];void*mon,*narc;}Work;
extern void GetPokemonSpriteCharAndPlttNarcIds(void*,void*,int),NARC_ReadPokepicAnimScript(void*,void*,u16,u16),Pokepic_StartAnim(void*,int),sub_0207294C(void*,void*,void*,u16,int,int,int);
extern u32 GetMonData(void*,int,void*);extern int sub_0207083C(void*,int),GetMonNature(void*);extern void*PokepicManager_CreatePokepic(void*,void*,int,int,int,int,void*,void*);
void ov57_0223B950(Work*);void ov57_0223B9C8(Work*);
void ov57_0223B950(Work*w){u8 ids[16];u8 script[40];int species,y;GetPokemonSpriteCharAndPlttNarcIds(ids,w->mon,2);species=GetMonData(w->mon,5,NULL);y=sub_0207083C(w->mon,2);w->y=y;NARC_ReadPokepicAnimScript(w->narc,script,(u16)species,1);w->pic=PokepicManager_CreatePokepic(w->mgr,ids,192,y+48,-640,0,script,NULL);}
void ov57_0223B9C8(Work*w){int species=GetMonData(w->mon,5,NULL);GetMonNature(w->mon);Pokepic_StartAnim(w->pic,1);sub_0207294C(w->narc,w->anim,w->pic,(u16)species,2,0,0);}
