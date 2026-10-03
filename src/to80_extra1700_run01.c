#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 data[14];}Block;
typedef struct {u16 ids[6];u8 flags[6];u8 pad12[2];u32 values[6];Block blocks[6];}Table;
typedef struct {u8 pad0[0x254];Table a;u8 pad3d0[0x1b4];Table b;}Work;
extern u16 LCRandom(void);
void ov80_022300D4(Work *,int,int);
#define SWAPA(tab,block) {Block block;u16 id=w->tab.ids[index];u8 flag=w->tab.flags[index];u32 value=w->tab.values[index];Block *destination;destination=&w->tab.blocks[index];block=*destination;w->tab.ids[index]=w->tab.ids[random];w->tab.flags[index]=w->tab.flags[random];w->tab.values[index]=w->tab.values[random];*destination=w->tab.blocks[random];w->tab.ids[random]=id;w->tab.flags[random]=flag;w->tab.values[random]=value;w->tab.blocks[random]=block;}
#define SWAPB(tab,block) {Block *destination;Block block;u16 id=w->tab.ids[index];u8 flag=w->tab.flags[index];u32 value=w->tab.values[index];destination=&w->tab.blocks[index];block=*destination;w->tab.ids[index]=w->tab.ids[random];w->tab.flags[index]=w->tab.flags[random];w->tab.values[index]=w->tab.values[random];*destination=w->tab.blocks[random];w->tab.ids[random]=id;w->tab.flags[random]=flag;w->tab.values[random]=value;w->tab.blocks[random]=block;}
void ov80_022300D4(Work *w,int index,int side){u16 random=LCRandom()%6;if(side==0)SWAPA(a,a)else SWAPB(b,b)}
