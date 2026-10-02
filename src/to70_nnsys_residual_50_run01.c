#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Player {u8 pad[0x2d];u8 prepared;u8 pad2e[14];u8 id;} Player;
int SND_GetPlayerLocalVariable(u32,int);
int NNS_SndPlayerReadVariable(Player *volatile*,int,s16*);
int NNS_SndPlayerReadVariable(Player *volatile *handle,int variable,s16*out){Player*p;if(!*handle)return 0;p=*handle;if(!p->prepared){*out=-1;return 1;}*out=SND_GetPlayerLocalVariable(p->id,variable);return 1;}
