#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 move,unused;} Move;
typedef struct {u8 pad[4];u8 mode,selected,unused,special;u16 unused2,parameter;u8 pad2[0xc];Move moves[128];u8 pad3[0x4c];void *party;u8 output[4];u8 pad4[0x488];u8 flag,kind;u8 pad5[0xe];u8 records[2][9];} Work;
extern int sub_02030BD0(int,void *);extern int ov80_02231A14(Work *);extern void ov80_022372D8(int,int,int,int,void *);extern void ov80_02237334(int,int,int,int,int,void *);extern int ov80_0223793C(Work *);extern void *Party_GetMonByIndex(void *,int);extern u32 GetMonData(void *,int,void *);extern void ov80_02237448(int,int,int,int,int,void *,int);
void ov80_022313C8(Work *);
void ov80_022313C8(Work *w){int bonus=0;int mode=1;int value;Move *moves;int select;if(w->mode)mode=2;value=sub_02030BD0(w->kind,w->records[w->mode]);w->special=ov80_02231A14(w);ov80_022372D8(w->kind,mode,value,w->selected,w->moves);ov80_02237334(w->mode,mode,w->parameter,value,w->selected,w->moves);moves=w->moves;select=w->selected&127;if((u16)(moves[select].move+0xfecd)<=1)value=(u8)ov80_0223793C(w);if(moves[select].move==307)bonus=1;if(moves[select].move==308)bonus=2;ov80_02237448(1,w->flag,value,w->selected,(u16)GetMonData(Party_GetMonByIndex(w->party,0),5,0),w->output,bonus);}
