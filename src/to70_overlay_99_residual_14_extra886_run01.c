#pragma force_active on
#include "global.h"
typedef struct{u16 a,b[3];}Row;
extern Row*ov99_021E9430(void*,u8,int);
typedef union{u32 value;struct{u32 tile:9;u32 rotation:5;u32 rest:18;};}TileCell;
typedef union{u16 raw;struct{u16 tile:10;u16 rotation:5;u16 rest:1;};}SourceCell;
void ov99_021E9464(void*,TileCell*,int,int,int,int);
void ov99_021E9464(void*p,TileCell*out,int group,int row,int col,int arg){int v;Row*table,*entry;if(p==NULL)GF_AssertFail();if(row>=5)GF_AssertFail();if(col>=3)GF_AssertFail();table=ov99_021E9430(p,(u8)group,arg);entry=&table[row];v=entry->b[col];out->tile=v&1023;out->rotation=v>>10;}
