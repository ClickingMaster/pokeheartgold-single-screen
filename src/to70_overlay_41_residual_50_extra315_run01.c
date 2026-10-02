#pragma force_active on
#include "global.h"
#pragma force_active on
int ov41_0224989C(const s8*,int);void ov41_022498E8(const s8*,int,int,u8*),ov41_02249978(u8*,int,int,int,int);
int ov41_0224989C(const s8*data,int stride){int x,y;for(x=0;x<80;x++){for(y=0;y<80;y++){int index=x+y*stride;u8 mask=15<<((index%2)*4);if(data[index/2]&mask)return x;}}return 80;}
