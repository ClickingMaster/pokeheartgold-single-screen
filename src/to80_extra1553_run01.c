#pragma force_active on
#include "global.h"
typedef struct {void*bg;void*sprites[96];u8 pad[0x184-388];u8 started,next,column,side,done,kind,delay; s8 timer;u8 p18c,active;} S;
extern const u8 ov80_0223DBE0[2][6];
extern void ov80_0223AE14(void*,int,int,int,int,int,void*,int,int,int);
extern int ov80_0223AE6C(void*);
int ov80_0223B1D4(S*);
int ov80_0223B1D4(S*s){int i,result;if(!s->active)return 1;
 if(s->started<96 && --s->timer<=0){int position,tile;s->timer=s->delay;
 position=s->next*16+8;tile=ov80_0223DBE0[s->side][s->column]*32;ov80_0223AE14(s->sprites[s->started],position,position,tile,tile+32,s->kind,s->bg,16,32,s->p18c);
 position=(7-s->next)*16+8;tile=ov80_0223DBE0[s->side^1][s->column]*32;ov80_0223AE14(s->sprites[s->started+1],position,position,tile,tile+32,s->kind,s->bg,16,32,s->p18c);
 position=(s->next+8)*16+8;tile=ov80_0223DBE0[s->side][s->column]*32;ov80_0223AE14(s->sprites[s->started+2],position,position,tile,tile+32,s->kind,s->bg,16,32,s->p18c);
 position=(15-s->next)*16+8;tile=ov80_0223DBE0[s->side^1][s->column]*32;ov80_0223AE14(s->sprites[s->started+3],position,position,tile,tile+32,s->kind,s->bg,16,32,s->p18c);
 s->started+=4;s->column++;if(s->column%6==0){s->side^=1;s->next++;s->column=0;}}
 for(i=s->done;i<s->started;i++){result=ov80_0223AE6C(s->sprites[i]);if(result==1)s->done++;}
 if(s->done>=96 && result==1){s->active=0;return 1;}return 0;
}
