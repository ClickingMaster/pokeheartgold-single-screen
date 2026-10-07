#pragma force_active on
#include "global.h"
#include <string.h>
#pragma opt_repositioncode off
#pragma force_active on
typedef struct Particle{u32 a,b,c;void*sprite;int fx10,fx14,fx18,fx1c,fx20,fx24,fx28,remainder,lifetime,unknown,period,pad3c,period2,period3;void*task;}Particle;
typedef struct Host{u8 pad[0x88];Particle particles[80];}Host;
typedef struct Table21{int values[21];}Table21;
extern Table21 _0223B3BC;
extern u16 LCRandom(void);
extern const VecFx32*Sprite_GetMatrixPtr(void*);
extern void Sprite_SetMatrix(void*,const VecFx32*);
extern void Sprite_SetDrawFlag(void*,int);
extern void ov74_0222BA48(void*,void*);
extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov74_0222B3A4(Host*);
void ov74_0222B3A4(Host*host){Table21 copy;u32 copyPtr;Table21 table;VecFx32 pos;int i;volatile Host*cursor=host;Particle*taskData;i=0;copyPtr=(u32)copy.values;taskData=&host->particles[0];table=_0223B3BC;do{int v;float value;
 cursor->particles[0].fx10=0x80000;cursor->particles[0].fx14=0x60000;cursor->particles[0].fx1c=0x4000;cursor->particles[0].fx20=0x5000;cursor->particles[0].remainder=i%4;cursor->particles[0].unknown=0;cursor->particles[0].b=0;
 v=i/10+4;if(v>0)value=0.5f+(float)(v<<12);else value=(float)(v<<12)-0.5f;cursor->particles[0].fx1c=(int)value;
 if(i>50)cursor->particles[0].lifetime=i+260+(int)LCRandom()%5;
 else if(i>40)cursor->particles[0].lifetime=i+250+(int)LCRandom()%5;
 else if(i>30)cursor->particles[0].lifetime=i+240+(int)LCRandom()%5;
 else if(i>25)cursor->particles[0].lifetime=i+230+(int)LCRandom()%5;
 else if(i>20)cursor->particles[0].lifetime=i+220+(int)LCRandom()%5;
 else{copy=table;cursor->particles[0].lifetime=*(int*)copyPtr;}
 cursor->particles[0].lifetime=(int)(0.7*((cursor->particles[0].lifetime+1)/2));cursor->particles[0].lifetime++;
 cursor->particles[0].period=15;cursor->particles[0].period2=cursor->particles[0].period;cursor->particles[0].period3=cursor->particles[0].period;
 pos=*Sprite_GetMatrixPtr(cursor->particles[0].sprite);
 if((int)LCRandom()%224+16>0)value=0.5f+(float)(((int)LCRandom()%224+16)<<12);else value=(float)(((int)LCRandom()%224+16)<<12)-0.5f;pos.x=(int)value;
 pos.y=0;Sprite_SetMatrix(cursor->particles[0].sprite,&pos);Sprite_SetDrawFlag(cursor->particles[0].sprite,0);cursor->particles[0].task=SysTask_CreateOnMainQueue(ov74_0222BA48,taskData,6);
 cursor=(Host*)((u8*)cursor+0x4c);taskData=(Particle*)((u8*)taskData+0x4c);copyPtr+=4;i++;
 }while(i<80);}
