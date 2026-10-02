#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 id;u8 value,pad;} Entry;
typedef struct {u8 p[8];void *save;u8 q[0x20-12];void *man;} State;
extern Entry *sub_02096864(void *);extern void *sub_0209686C(void *,int);extern void ov42_02228FE0(void *,u16,u8,int),ov80_02239900(void *,void *),ov80_02239510(State *,void *,int),ov80_02239828(State *);
void ov80_02238A18(State *);
void ov80_02238A18(State *s){u32 local[8];Entry *entries=sub_02096864(s->save);int i;for(i=0;i<24;i++){if(entries[i].id!=65535)ov42_02228FE0(s->man,entries[i].id,entries[i].value,101);}for(i=0;i<32;i++){void *entry=sub_0209686C(s->save,i);if(*(u16*)((u8*)entry+12)!=65535){ov80_02239900(entry,local);ov80_02239510(s,local,i);}}ov80_02239828(s);}
