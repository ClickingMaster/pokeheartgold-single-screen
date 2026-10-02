#pragma force_active on
#include "global.h"
typedef struct W {void *bg;u8 p[0xea0];void(*cb)(struct W*);} W;
extern W ov74_0223D454;extern struct {u8 p[36];int enable;} ov74_0223E2D4;
typedef struct {u8 b[34];} Table;extern Table _0223B73C;
extern void GF_RunVramTransferTasks(void);extern void OamManager_ApplyAndResetBuffers(void);extern void DoScheduledBgGpuUpdates(void*);
void ov74_02235A74(void);int ov74_02235AC4(int);
int ov74_02235AC4(int id) {Table data=_0223B73C;int i;u8*p;i=0;if(data.b[0]!=-1){p=data.b;do{if(id==*p)return (data.b+1)[i*2];p+=2;i++;}while(*p!=-1);}return 0;}
