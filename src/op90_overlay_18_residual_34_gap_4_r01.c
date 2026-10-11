#pragma force_active on
#include "global.h"
#include <string.h>
int ov18_021F8970(int);
void *PokedexApp_LoadSortData(int,int*);
void PokedexApp_IntersectSpeciesLists(void*,int*,void*,int,void*,int,int,int),Heap_Free(void*);
void PokedexApp_FilterByType(int,void*,int*,void*,int,int);
#pragma opt_propagation off
// @fn PokedexApp_FilterByType
void PokedexApp_FilterByType(int arg,void*dst,int*out,void*src,int count,int flags){void*data;int size;int n;
 switch(ov18_021F8970(arg)){
 case 17:n=count;memcpy(dst,src,n*2);*out=n;return;
case 0:data=PokedexApp_LoadSortData(51,&size);break;
case 1:data=PokedexApp_LoadSortData(52,&size);break;
case 2:data=PokedexApp_LoadSortData(53,&size);break;
case 3:data=PokedexApp_LoadSortData(54,&size);break;
case 4:data=PokedexApp_LoadSortData(55,&size);break;
case 5:data=PokedexApp_LoadSortData(56,&size);break;
case 6:data=PokedexApp_LoadSortData(57,&size);break;
case 7:data=PokedexApp_LoadSortData(58,&size);break;
case 8:data=PokedexApp_LoadSortData(59,&size);break;
case 9:data=PokedexApp_LoadSortData(60,&size);break;
case 10:data=PokedexApp_LoadSortData(61,&size);break;
case 11:data=PokedexApp_LoadSortData(62,&size);break;
case 12:data=PokedexApp_LoadSortData(63,&size);break;
case 13:data=PokedexApp_LoadSortData(64,&size);break;
case 14:data=PokedexApp_LoadSortData(65,&size);break;
case 15:data=PokedexApp_LoadSortData(66,&size);break;
case 16:data=PokedexApp_LoadSortData(67,&size);break;
default:GF_AssertFail();break;
}PokedexApp_IntersectSpeciesLists(dst,out,data,size,src,count,0,flags);Heap_Free(data);
}
#pragma opt_propagation reset
