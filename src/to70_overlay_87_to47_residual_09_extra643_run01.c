#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x360];u8 tiles[9];}Work;
extern int LCRandom(void);
void ov87_021E7294(Work*);
void ov87_021E7294(Work*w){int failures,i,j,k;u8 color=LCRandom()%4;failures=0;for(i=0;i<9;i++){retry: j=LCRandom()%9;if(w->tiles[j]==176){failures=0;w->tiles[j]=color;if(i==2||i==4||i==6){color++;if(color==4)color=0;}}else{failures++;if(failures<30)goto retry;failures=0;for(k=0;k<9;k++){if(w->tiles[k]==176){w->tiles[k]=color;if(i==2||i==4||i==6){color++;if(color==4)color=0;}break;}}}}}
