#pragma force_active on
#include "global.h"
typedef struct{u16 first:6,count:2,threshold:8;} Entry;
typedef struct{Entry a,b;} Record;
typedef struct{u8 p[0x3c];u8 threshold;u8 p2[7];void*msg;u8 p3[8];void*str;u8 p4[0x24];u8 win1[16],win2[16];u8 p5[0x2c];void*sprites[14];u8 p6[0x24];u8 group2[0x30],group6[1];} Work;
extern Record ov113_021E6BD0[];
void ov113_021E6238(Work*,void*,int);
void ManagedSprite_SetDrawFlag(void*,int);
void ReadMsgDataIntoString(void*,int,void*);
void ov113_021E6274(void*,void*,int,int,int);
void ov113_021E629C(void*,void*,int,int);
void ov113_021E613C(Work*,int);
void ov113_021E613C(Work*w,int index){Record*r;int i;if(index==0){ov113_021E6238(w,w->group6,6);ov113_021E6238(w,w->group2,2);for(i=0;i<14;i++)ManagedSprite_SetDrawFlag(w->sprites[i],0);}
r=&ov113_021E6BD0[index];
if(r->a.threshold<=w->threshold){for(i=0;i<r->a.count;i++){ReadMsgDataIntoString(w->msg,r->a.first+i,w->str);if(r->a.first+i==15)ov113_021E6274(w->win1,w->str,3,4,0);else ov113_021E629C(w->win1,w->str,4,24);}}
if(r->b.threshold<=w->threshold){for(i=0;i<r->b.count;i++){ReadMsgDataIntoString(w->msg,r->b.first+i,w->str);ov113_021E629C(w->win2,w->str,4,24);}}}
