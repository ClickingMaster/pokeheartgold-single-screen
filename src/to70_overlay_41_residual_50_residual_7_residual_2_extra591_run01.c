#pragma force_active on
#include "global.h"
#include "system.h"
#include "touchscreen.h"
extern int ov41_0224A448(void *);
extern void TouchHitboxController_IsTriggered(void *);
typedef struct {u8 pad[0x68];void *controller;u8 pad2[0x14];int *mode;u16 disabled;} Work;
int ov41_0224A4EC(Work *w);
void ov41_0224A54C(Work *w);
int ov41_0224A4EC(Work *w){if(w->disabled)return 0;if(*w->mode==1){if(System_GetTouchHeld())return 0;if(gSystem.heldKeys)*w->mode=0;}else{if(gSystem.heldKeys)return 0;if(System_GetTouchHeld())*w->mode=1;}return 0;}
void ov41_0224A54C(Work *w){if(!w->controller)GF_AssertFail();if(ov41_0224A4EC(w)==0){if(*w->mode==0){ov41_0224A448(w);return;}TouchHitboxController_IsTriggered(w->controller);}}
