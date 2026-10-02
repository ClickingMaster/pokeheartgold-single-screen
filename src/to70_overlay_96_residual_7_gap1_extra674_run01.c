#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[10];u16 half;int value;}Record;
typedef struct{int id;u8 p[5];u8 rank;}Rank;
typedef struct{u8 p[0x614];void *source;u8 gap[0x8b4-0x618];u16 half[4][4];u16 values[4][4];u8 first[4],second[4];}Work;
extern int ov96_021E5F24(Work*);
extern Record *ov96_021E9370(void*,int);
extern Rank *ov96_021E94EC(void*,int);

void ov96_021E7658(Work*w,int col);

void ov96_021E7658(Work*w,int col){u8 id=ov96_021E5F24(w),i;Rank *p=0;for(i=0;i<4;i++){Record*r=ov96_021E9370(w->source,i);w->half[i][col]=r->half;w->values[i][col]=r->value;}for(i=0;i<4;i++){p=ov96_021E94EC(w->source,i);if(p->id==id)break;}if(i==4){GF_AssertFail();return;}if(p->rank==0){w->first[col]=1;w->second[col]=0;return;}w->first[col]=0;{Rank*q=ov96_021E94EC(w->source,3);u8 rank=q->rank;if(rank==p->rank)w->second[col]=1;else w->second[col]=0;}}
