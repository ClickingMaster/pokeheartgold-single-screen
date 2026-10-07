#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void *first,*sprites[5];} Group;
typedef struct{u8 pad[0x1b4];void *first[5],*second[5];Group groups[5];void *last;} Board;
typedef struct{u8 pad[0x78];Board board;} Work;
void Sprite_SetDrawFlag(void*,BOOL);
void ov97_0221F74C(Work*);
void ov97_0221F74C(Work *w){Board *b=&w->board;u8 i,j;for(i=0;i<5;i++){Sprite_SetDrawFlag(b->first[i],FALSE);Sprite_SetDrawFlag(b->second[i],FALSE);}for(i=0;i<5;i++){Sprite_SetDrawFlag(b->groups[i].first,FALSE);for(j=0;j<5;j++)Sprite_SetDrawFlag(b->groups[i].sprites[j],FALSE);}Sprite_SetDrawFlag(b->last,FALSE);}
