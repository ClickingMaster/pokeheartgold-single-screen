#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[9],kind;u16 score;u32 multiplier;} Entry;
typedef struct {u8 v[4];} Bytes;
extern const Bytes ov96_0221D99C;
extern Entry*PokeathlonCourseScoreList_GetRanked(void*,int);
void ov96_0221A730(void*);
void ov96_0221A730(void*w){u8 kinds[4];u8 counts[4];Bytes table;u8 i,j;Entry*e;u16 value;u8 last;table.v[0]=ov96_0221D99C.v[0];table.v[1]=ov96_0221D99C.v[1];last=ov96_0221D99C.v[3];table.v[2]=ov96_0221D99C.v[2];table.v[3]=last;for(i=0;i<4;i++){kinds[i]=PokeathlonCourseScoreList_GetRanked(w,i)->kind;counts[i]=0;}for(i=0;i<4;i++){u8 kind=kinds[i];e=PokeathlonCourseScoreList_GetRanked(w,i);for(j=0;j<4;j++){if(i!=j && kind==kinds[j])counts[i]++;}value=table.v[kind+counts[i]]+e->multiplier*5;if(value>200.0)value=200;e->score=value;}}
