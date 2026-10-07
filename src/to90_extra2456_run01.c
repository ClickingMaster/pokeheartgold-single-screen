#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
typedef struct List {int n;char **items;int x,y;} List;
void *ov00_021EC3A8(u32);void ov00_021EC3D8(void *);char *ov00_021FF3D4(const char *);
extern const char ov45_022566DC[];
int ov45_0224A708(void *,int,int,int,int,int,int,List *);
int ov45_0224AA2C(void *,int,int,char **,int,int,int,int);
int ov45_0224AA2C(void *ctx,int arg,int count,char **input,int x,int y,int z,int w) {
 int i,j;List *list=ov00_021EC3A8(16);
 if(!list)return 0;
 memset(list,0,sizeof(*list));
 list->y=y;list->n=count;list->x=x;
 if(y)list->n--;
 list->items=ov00_021EC3A8(list->n*4);
 if(!list->items){ov00_021EC3D8(list);return 0;}
 i=0;j=i;
 for(;i<count;i++) {
  char *s=input[i];
  if(strcmp(s,ov45_022566DC)!=0) {
   list->items[j]=ov00_021FF3D4(s);
   if(!list->items[j]) {
    for(j--;j>=0;j--)ov00_021EC3D8(list->items[j]);
    ov00_021EC3D8(list->items);ov00_021EC3D8(list);return 0;
   }
   j++;
  }
 }
 list->n=j;
 return ov45_0224A708(ctx,13,arg,0,z,0,w,list);
}
