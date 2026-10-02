#pragma force_active on
#include "global.h"
#pragma force_active on
struct Point {u16 x,y;};
extern u8 gSystem[];
extern BOOL ov91_0225ED6C(void *);
extern void ov91_02260CB4(void *);
extern void ov91_022614D4(void *);
extern void ov91_0225EDC8(void *,struct Point);
void ov91_0225EC7C(u8 *);
void ov91_0225EC7C(u8 *state) {struct Point point; if(*(int *)(state+0x20)==0 && ov91_0225ED6C(state)==1) {*(int *)(state+0x20)=1;ov91_02260CB4(state+0x8544);ov91_022614D4(state+0x21a8);} if(*(int *)(state+0x20)==1) {u16 count=*(u16 *)(state+0x86ec); if(count==1) {point.x=*(u16 *)(state+0x86ee);point.y=*(u16 *)(state+0x86f0);ov91_0225EDC8(state+0x20,point);}else if(count>=2) {point.x=*(u16 *)(state+0x86ee);point.y=*(u16 *)(state+0x86f0);ov91_0225EDC8(state+0x20,point);count=*(u16 *)(state+0x86ec);point.x=*(u16 *)(state+(count-1)*8+0x86ee);point.y=*(u16 *)(state+(count-1)*8+0x86f0);ov91_0225EDC8(state+0x20,point);}else {point.x=*(u16 *)(gSystem+0x60);point.y=*(u16 *)(gSystem+0x62);ov91_0225EDC8(state+0x20,point);} } }
