#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 p0[0x24];u32 a[32];u16 b[40];u32 c[71];u32 d[32];u16 e[32];u8 f[32];u32 mask;} W;
extern W *ov00_021F955C(void);
extern void ov00_021F3590(void);
int ov00_021F3398(int,int);
int ov00_021F3398(int index,int count){
 int result,i,n;W *w;
 if(!ov00_021F955C())return 0;
 result=ov00_021F955C()->c[index];
 w=ov00_021F955C();w->mask &= ~(1<<ov00_021F955C()->f[index]);
 ov00_021F3590();
 if(index<count-1){n=count-index-1;for(i=0;i<n;i++){
 ov00_021F955C()->a[index+i]=ov00_021F955C()->a[index+i+1];
 ov00_021F955C()->b[index+i]=ov00_021F955C()->b[index+i+1];
 ov00_021F955C()->c[index+i]=ov00_021F955C()->c[index+i+1];
 ov00_021F955C()->d[index+i]=ov00_021F955C()->d[index+i+1];
 ov00_021F955C()->e[index+i]=ov00_021F955C()->e[index+i+1];
 ov00_021F955C()->f[index+i]=ov00_021F955C()->f[index+i+1];}}
 if(count>0){i=count-1;ov00_021F955C()->a[i]=0;ov00_021F955C()->b[i]=0;ov00_021F955C()->c[i]=0;ov00_021F955C()->d[i]=0;ov00_021F955C()->e[i]=0;ov00_021F955C()->f[i]=0;}return result;}
