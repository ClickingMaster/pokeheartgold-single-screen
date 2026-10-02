#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[10];u16 flags:6;u16 value:10;u8 rest[8];}Entry;
typedef struct{u8 p[0x20];u32 *map;u8 p2[0x120-0x24];u8 *data;}Work;
extern int MapHeader_GetMapSec(u32),ov02_0224F108(Work*,Entry*,void*);
extern void ReadWholeNarcMemberByIdPair(void*,int,int),ov02_0224F058(Work*,void*);

int ov02_0224EF94(Work*w);

int ov02_0224EF94(Work*w){u32 info[7];int order[100];int *scan;Entry *records;int i,j,n;int *cursor;ReadWholeNarcMemberByIdPair(w->data+20,222,MapHeader_GetMapSec(*w->map)+1);ReadWholeNarcMemberByIdPair(w->data+620,222,0);ov02_0224F058(w,info);records=(Entry*)(w->data+20);n=0;cursor=order;for(i=0;i<12;i++){*cursor++=i+30;n++;}i=0;cursor=&order[n];do{*cursor++=i;n++;i++;}while(i<30);i=42;cursor=&order[n];do{*cursor++=i;n++;i++;}while(i<100);j=0;scan=order;do{Entry*p=&records[*scan];if(p->value!=0&&ov02_0224F108(w,p,info))return p->value;j++;scan++;}while(j<100);GF_AssertFail();return 0;}
