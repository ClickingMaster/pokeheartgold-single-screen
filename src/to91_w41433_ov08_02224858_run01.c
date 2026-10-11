#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct {u8 prefix[0x2c];Window *windows;u8 reserved30;u8 flag;} Work;
extern u8 *sBattleBagButtonWindowLists[];
void BattleBagMenu_ScrollButtonWindows(void *,u8,int);
void BattleBagMenu_ScrollButtonWindows(void *arg,u8 index,int mode) {
 Work *w=arg;
 const u8 *list;u8 direction,pixels;u16 i;
 if(index>=6&&index<=11&&w->flag==0)list=sBattleBagButtonWindowLists[index+11];
 else list=sBattleBagButtonWindowLists[index];
 if(!list)return;
 switch(mode){case 0:case 2:direction=1;pixels=2;break;case 1:direction=0;pixels=4;break;}
 for(i=0;i<8;i++){
  if(list[i]==255)break;
  ScrollWindow(&w->windows[list[i]],direction,pixels,0);
  ScheduleWindowCopyToVram(&w->windows[list[i]]);
 }
}
