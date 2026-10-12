#pragma force_active on
#include "global.h"
#pragma force_active on
void PokemonSummary_ClearMoveInfo(void *),PokemonSummary_PrintMoveInfo(void *),PokemonSummary_SetMoveCategorySprite(void *,int),thunk_Sprite_SetDrawFlag(void *,int),sub_0208A7F8(u8 *,int);
typedef struct State {u8 pad0[556];u8 *config;u8 pad230[52];u16 moves[4];u8 pp[4];u8 bonus[4];u8 pad274[1353];u8 first:4;u8 second:4;} State;
void *sub_0208A520(State *);void BoxMonSwapMoves(void *,int,int),MonSwapMoves(void *,int,int),sub_0208A834(State *);
void sub_0208A834(State *s){void *mon=sub_0208A520(s);u16 move;u8 byte;if(s->config[17]==2)BoxMonSwapMoves(mon,s->first,s->second);else MonSwapMoves(mon,s->first,s->second);move=s->moves[s->first];s->moves[s->first]=s->moves[s->second];s->moves[s->second]=move;byte=s->pp[s->first];s->pp[s->first]=s->pp[s->second];s->pp[s->second]=byte;byte=s->bonus[s->first];s->bonus[s->first]=s->bonus[s->second];s->bonus[s->second]=byte;*(int *)((u8 *)s->config+56)=1;}
