#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 data[20];} Item;
typedef struct {u8 pad[400];Item items[7];u8 other[180];} Owner;
void sub_02069784(void *),sub_02069818(void *),ov85_021E7734(Owner *);
extern const int ov85_021EA558[],ov85_021EA580[];
void sub_020696C4(void *,int,void *,int,int,int),sub_02069714(void *),sub_020697DC(void *,int,void *,int,int,int),sub_02069864(void *,void *,int),sub_02069884(void *,void *),ov85_021E7660(Owner *);
void ov85_021E7660(Owner *w){volatile int i;const int *ids1,*ids2;u8*a,*b,*c,*base=(u8 *)w->items;
sub_020696C4(base,0,*(void **)((u8 *)w+3456),0,102,0);sub_02069714(base);
sub_020696C4(base+20,0,*(void **)((u8 *)w+3456),5,102,0);sub_02069714(base+20);
a=base+40;ids1=ov85_021EA558;ids2=ov85_021EA580;b=base;i=0;b+=140;c=a;
do{int next;sub_020696C4(a,0,*(void **)((u8 *)w+3456),*ids1,102,0);sub_02069714(a);sub_020697DC(b,0,*(void **)((u8 *)w+3456),*ids2,102,0);sub_02069864(b,c,102);sub_02069884(b,c);next=i+1;a+=20;i=next;b+=36;ids1++;c+=20;ids2++;}while(i<5);}
void ov85_021E7734(Owner *w){int i;u8*a,*b,*base=(u8 *)w->items;sub_02069784(base);sub_02069784(base+20);a=base;b=base;i=0;a+=40;b+=140;do{sub_02069784(a);sub_02069818(b);i++;a+=20;b+=36;}while(i<5);}
