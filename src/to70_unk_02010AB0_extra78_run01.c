#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int steps,delay,timer,current,target,increment,screen;} Bright;
extern void SetMasterBrightness(int,int);
BOOL sub_02010BF4(Bright *);
BOOL sub_02010BF4(Bright *b){BOOL done=FALSE;if(++b->timer>=b->delay){b->timer=0;if(b->steps-1>0){b->steps--;b->current+=b->increment;}else{b->current=b->target;done=TRUE;}SetMasterBrightness(b->screen,b->current/128);}return done;}
