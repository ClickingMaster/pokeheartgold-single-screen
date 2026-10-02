#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 low:4,high:4,rest:8;} Bits;
typedef struct {u8 pad[0x44];u8 type;u8 pad2[4];u8 mode,value;} State;
u16 ov59_02239E0C(void *,Bits *,int);u8 ov59_02239EA8(State *);
u16 ov59_02239E0C(void *unused,Bits *data,int mode){u8 high=data->high-1;int nibble=data->high;int low;switch(mode){case 2:low=data->low;if(low==6)return 122;if(low==5)return 121;if(nibble==0)return low+123;return low*3+high+106;case 1:low=data->low;if(low==6)return 122;return low*4+high+86;case 0:default:low=data->low;if(low==6)return 131;if(low==5)return high+128;return low*6+high+56;}}
u8 ov59_02239EA8(State *s){switch(s->mode){case 0:return s->value*2;case 1:return s->value*2+1;case 2:if(s->type==1)return 7;return 8;case 3:return 8;}return 0;}
