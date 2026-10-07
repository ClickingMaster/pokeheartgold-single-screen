#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "unk_02091278.h"
#include "unk_020912AC.h"
#pragma force_active on
void ov18_021F2530(PokedexAppData *w,u16 species,int start){s16 index,shown;
if(!species||!Pokedex_GetInternationalViewFlag(w->args->pokedex)){for(index=(s16)start;(u32)index<(u32)start+6;index++)ManagedSprite_SetDrawFlag(w->sprites[index],FALSE);}
else{shown=0;for(index=(s16)(start+5);(u32)index>=(u32)start;index--){u32 which=index-start;int language=sub_02091294(sub_020912AC(which));if(ov18_021E6D10(w,species,(u16)language)==TRUE||language==GAME_LANGUAGE){int native=LanguageToDexFlag(w->unk_185C);int flag=sub_020912AC(which);struct {s16 y,x;} position;if(flag==native)ov18_021F118C(w,index,flag);else ov18_021F118C(w,index,flag+6);ManagedSprite_GetPositionXY(w->sprites[index],&position.x,&position.y);ManagedSprite_SetPositionXY(w->sprites[index],(s16)(124+24*(5-shown)),position.y);ManagedSprite_SetDrawFlag(w->sprites[index],TRUE);shown++;}else ManagedSprite_SetDrawFlag(w->sprites[index],FALSE);}}}
