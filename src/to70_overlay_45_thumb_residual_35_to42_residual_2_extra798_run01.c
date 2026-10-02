#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];int type;} Entry;
typedef struct {u8 pad[0x12d];u8 kind;} Work;
extern Work *_022577C0;extern int ov45_0222FB24(Work *,int,int);extern Entry *ov45_0222F954(Work *,int);extern int ov45_0222F7B0(int);extern void ov45_0222FA74(Work *,int,int);extern void ov45_0222FAF8(Work *,int,int);extern void ov45_0222F928(Work *,int,Entry *);
void ov45_0222FEC4(int,Entry *);
void ov45_0222FEC4(int n,Entry *entry){if(ov45_0222FB24(_022577C0,n,0)!=-1){if(_022577C0->kind!=4){int current,old=ov45_0222F954(_022577C0,n)->type;current=entry->type;int expected=ov45_0222F7B0(_022577C0->kind);if(old==5&&current==expected)ov45_0222FA74(_022577C0,n,1);else if(old==expected&&current!=expected)ov45_0222FAF8(_022577C0,n,1);}ov45_0222F928(_022577C0,n,entry);}}
