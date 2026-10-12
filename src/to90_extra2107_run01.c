#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x504];void *up,*down;u8 pad2[0x2b8];u8 cell,page,count,selected;} State;
extern u8 sub_0208ACDC(State *,int);
extern void PokemonSummary_UpdateRibbonCursorSprite(State *),PokemonSummary_UpdateRibbonSprites(State *),PokemonSummary_PrintRibbonInfo(State *),PlaySE(int),Sprite_SetDrawFlag(void *,int);
void sub_0208AB58(State *,int);
void sub_0208AB58(State *s,int direction){
 u8 cell=s->cell,page=s->page;
 if(direction==1){if(cell%3==2)return;s->cell++;}
 else if(direction==-1){if(cell%3==0)return;s->cell--;}
 else if(direction==3){
  if(cell>=6){if((page+1)*9<s->count){s->page++;s->cell=s->cell%3;}}
  else s->cell=cell+3;
 }else if(direction==-3){
  if(cell<=2){if(page!=0){s->page--;s->cell=s->cell%3+6;}}
  else s->cell=cell-3;
 }else if(direction==9){if((page+1)*9<s->count)s->page++;}
 else if(direction==-9){if(page!=0)s->page--;}
 if((cell!=s->cell||page!=s->page)&&direction!=9&&direction!=-9)PlaySE(1500);
 s->selected=sub_0208ACDC(s,s->cell);PokemonSummary_UpdateRibbonCursorSprite(s);
 if(page!=s->page)PokemonSummary_UpdateRibbonSprites(s);
 PokemonSummary_PrintRibbonInfo(s);
 if(s->page!=0)Sprite_SetDrawFlag(s->up,1);else Sprite_SetDrawFlag(s->up,0);
 if((s->page+1)*9<s->count)Sprite_SetDrawFlag(s->down,1);else Sprite_SetDrawFlag(s->down,0);
}
