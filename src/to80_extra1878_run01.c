#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 id;u8 rest[72];} Item;
typedef struct {u8 pad[0x5b8];Item items[3];u16 current;u8 active,flag;} Work;
extern Work *_022577C0;extern int ov45_0222FB24(Work*,int,int);extern int ov45_0222E9E0(int);extern void ov45_02230384(Item*,s64*);extern void ov45_0222F154(void);extern void ov45_02230164(Work*,u32,int,Item*);

void ov45_0222FF7C(int id,Item*item);

static inline BOOL SameId(int actual,int wanted){return actual==wanted;}
void ov45_0222FF7C(int id,Item*item){s64 a,b;int n=ov45_0222FB24(_022577C0,id,0);if(n!=-1){if(SameId(ov45_0222E9E0(n),id)){}else if(_022577C0->active&&_022577C0->current==item->id){ov45_02230384(&_022577C0->items[_022577C0->current],&a);ov45_02230384(item,&b);if(a<b)return;_022577C0->flag=1;ov45_0222F154();}ov45_02230164(_022577C0,item->id,id,item);}}
