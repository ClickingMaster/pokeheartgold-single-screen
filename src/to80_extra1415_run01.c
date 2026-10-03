#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[21];u8 state;u8 pad2[502];int value;u8 pad3[596];void(*callback)(int,int,int,int,int,void*);void *context;} Work;
extern Work *ov00_021F955C(void);
extern void ov00_021F6D30(void),ov00_021F381C(void),ov00_021F3CF8(int),ov00_021F5D0C(int,int,int);
extern int ov00_021F0404(int),ov00_021F82EC(void);
void ov00_021F6EC4(int);
void ov00_021F6EC4(int mode){if(!mode){ov00_021F6D30();return;}ov00_021F381C();if(ov00_021F955C()->state==2 || ov00_021F955C()->state==3){int empty=ov00_021F955C()->value==0;Work *w=ov00_021F955C();int value=ov00_021F0404(ov00_021F955C()->value);w->callback(0,1,empty,0,value,ov00_021F955C()->context);return;}if(ov00_021F955C()->state==0){if(mode==1){ov00_021F3CF8(0);if(ov00_021F82EC())return; }}else if(ov00_021F955C()->state==1 && mode==1)ov00_021F5D0C(0,0,0);}
