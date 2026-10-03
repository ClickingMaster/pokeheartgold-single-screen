#pragma force_active on
#include "global.h"
typedef struct Entry { u16 a,b; s8 direction; u8 value,gap,flag; } Entry;
typedef struct Global { char pad[0x24]; u8 present[8]; char pad2[8]; Entry entries[8]; char pad3[0x40]; u8 active[8],mute[8],count[8],changed[8],timer[8]; } Global;
extern Global *_021D41C4;
extern int sub_020374C0(int),sub_020374AC(int),sub_02057454(int,int),sub_02057BEC(int),sub_02057C24(int),sub_020574C4(int,int,int),sub_02057524(int);
void sub_02057550(void);
void sub_02057550(void) {
 int i; for(i=0;i<8;i++) {
 Entry *e; int direction,a,b;
 if(!_021D41C4->present[i] || !_021D41C4->active[i]) continue;
 e=&_021D41C4->entries[i];
 if(_021D41C4->timer[i]) --_021D41C4->timer[i];
 if(_021D41C4->mute[i]) direction=0; else direction=sub_020374C0(i);
 e->value=sub_020374AC(i);
 if(_021D41C4->timer[i]) continue;
 if(e->flag==1) _021D41C4->changed[i]=1;
 e->flag=0;
 if(_021D41C4->count[i]==2) { _021D41C4->count[i]=1; continue; }
 direction=sub_02057454(direction,direction); a=sub_02057BEC(i); b=sub_02057C24(i);
 if(a==0xffff||b==0xffff||direction==-1) continue;
 if(e->direction!=direction) { e->direction=direction; _021D41C4->timer[i]=4; _021D41C4->changed[i]=1; if(_021D41C4->count[i]>2) --_021D41C4->count[i]; }
 else if(!_021D41C4->count[i]) { if(sub_020574C4(a,b,i)) { e->flag=1; _021D41C4->changed[i]=1; _021D41C4->timer[i]=4; }
 else { _021D41C4->changed[i]=1; e->a=a; e->b=b; e->direction=direction; _021D41C4->timer[i]=sub_02057524(e->value); } }
 }
}
