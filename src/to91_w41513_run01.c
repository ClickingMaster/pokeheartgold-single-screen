#include "global.h"
#pragma force_active on
typedef struct{int values[4];u8 count,type,reserved;struct{u8 selector:7;u8 flag:1;} flags;} Packet;
int ov45_0222EC90(int);void ov45_0222EEF0(int,int,int);
void ov45_0222AC14(void*,int,int,int,int,int,int,int);
void ov45_0222AC14(void*unused,int selector,int count,int first,int second,int third,int fourth,int flag){Packet p={0};u32 kind;(void)unused;switch((u32)selector){case 0:case 1:case 2:kind=2;break;case 3:case 4:kind=3;break;case 5:kind=4;break;case 6:kind=5;break;default:return;}p.type=kind;p.values[0]=ov45_0222EC90(first);p.values[1]=ov45_0222EC90(second);p.values[2]=ov45_0222EC90(third);p.values[3]=ov45_0222EC90(fourth);p.count=count;p.flags.selector=selector;p.flags.flag=flag;ov45_0222EEF0(4,(int)&p,20);}
