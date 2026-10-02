#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1090];int active;u8 q[0x10a4-0x1094];int kind;u8 r[0x10d0-0x10a8];int enabled;u16 a,b;} Work;
extern Work*_0221A680;extern void*sub_0203993C(void*);extern int sub_02034084(void*),sub_02037454(void);extern void ov00_021E77F4(void*,int,int),ov00_021E6994(void),ov00_021E7A1C(void(*)(void));
void ov00_021E69A8(void*);
void ov00_021E69A8(void*arg){int count=1,online,type;online=sub_02034084(sub_0203993C(arg));if(online){count=sub_02037454()-1;if(count<1)count=1;_0221A680->a=1;_0221A680->enabled=1;_0221A680->b=1;}if(_0221A680->active==0){switch(_0221A680->kind){case 2:type=1;break;case 3:type=2;break;case 4:type=3;break;case 5:type=4;break;case 0:case 1:default:if(online==0)type=4;else type=3;break;}ov00_021E77F4(arg,type,count);ov00_021E7A1C(ov00_021E6994);_0221A680->active=1;}}
