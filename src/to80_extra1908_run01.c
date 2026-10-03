#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {int a,id,kind;} Entry;typedef struct {u8 p[13];u8 count,used,kind;u8 q[0x188];struct {void *sprite;u8 p[8];u16 value;u8 q[38];} slots[32];u8 r[0xb8];Entry *entries;} W;
extern int Sprite_GetDrawFlag(void*);int sub_020947C0(W*,int);
int sub_020947C0(W*w,int id) {int i,found;if(w->slots[id].sprite && Sprite_GetDrawFlag(w->slots[id].sprite)==0)return 3;if(w->slots[id].value)return 3;found=-1;i=0;for(;found==-1&&i<w->count;i++){if(w->kind==w->entries[i].kind && id==w->entries[i].id)found=i;}if(found==-1){if(w->used==w->count)return 0;for(i=0;i<w->count;i++){if(w->entries[i].id==-1)return 1;}}return 2;}
