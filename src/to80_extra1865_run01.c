#pragma force_active on
#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct {u32 a;u32 *ids;} List;
extern void ov45_0222EC10(List*);extern int WiFiPlazaPlayer_GetCountry(void*);extern int WiFiPlazaPlayer_GetRegion(void*);extern void ov45_0222D638(void*,u16,u8,int);extern void *ov45_0222EA2C(u32);extern u32 ov45_0222EDC4(u8);extern u8 ov45_0222EDF0(u8);
void ov45_0222B79C(u8*,void*);
void ov45_0222B79C(u8*w,void*out) {List list;int n,i;void *p;memset(out,0,200);ov45_0222EC10(&list);{u8*profile=w+32;w=profile;}n=WiFiPlazaPlayer_GetCountry(w);ov45_0222D638(out,(u16)n,(u8)WiFiPlazaPlayer_GetRegion(w),0);for(i=0;i<20;i++){if(list.ids[i]!=-1){p=ov45_0222EA2C(list.ids[i]);n=WiFiPlazaPlayer_GetCountry(p);ov45_0222D638(out,(u16)n,(u8)WiFiPlazaPlayer_GetRegion(p),0);}}for(i=0;i<20;i++){u8 a=(u8)ov45_0222EDC4((u8)i);ov45_0222D638(out,a,ov45_0222EDF0((u8)i),1);}}
