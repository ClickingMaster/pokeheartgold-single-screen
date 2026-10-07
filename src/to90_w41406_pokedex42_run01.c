#include "global.h"
#include "pokedex.h"
#pragma force_active on
int ov70_0224342C(void*,Pokedex*,int,const u16*);
// @fn ov70_0224342C
int ov70_0224342C(void*unused,Pokedex*dex,int count,const u16*species){int visited=0;int seen=0;if(count>0){do{if(Pokedex_CheckMonSeenFlag(dex,*species))seen++;species++;visited++;}while(visited<count);}return seen;}
