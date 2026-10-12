#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x44c];void*sprite;} Work;void PokemonSummary_ClearMoveInfo(Work*,int);void PokemonSummary_PrintMoveInfo(Work*,int);void PokemonSummary_SetMoveCategorySprite(Work*,int);void thunk_Sprite_SetDrawFlag(void*,int);void sub_0208A7F8(Work*,int);
void sub_0208A7F8(Work*w,int id){if(id==-1){PokemonSummary_ClearMoveInfo(w,id);thunk_Sprite_SetDrawFlag(w->sprite,0);return;}PokemonSummary_PrintMoveInfo(w,id);PokemonSummary_SetMoveCategorySprite(w,id);thunk_Sprite_SetDrawFlag(w->sprite,1);}
