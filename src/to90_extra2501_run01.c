#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {float acceleration,weight;u8 padding[14],range,spare,type,tail[3];} Athlete;
typedef struct {u8 prefix[24];Athlete athletes[3];u8 gap[31],selected;VecFx32 velocity;u8 gap2[5],swapping,swapPhase,pad,event,gap3[8],locked,release,pad2;s32 heldFrames;VecFx32 initial,current;u8 gap4[8],id,tail[3];} Player;
typedef struct {u32 down,previous,reserved;} Latch;
typedef struct {u8 prefix[0x30];Player players[4];Latch latches[4];u32 moving[4];u8 finished,spare,timeout,gap[25];u32 score;u8 gap2[0x260];u16 timer;} Game;
typedef struct {u8 prefix[0x30];Player player;} PlayerWindow;
typedef struct {u32 down;u8 x,y,gap[2],phase,tail[27];} Frame;
typedef struct {VecFx32 limitZero,zero,delta,combined;} Workspace;
extern void *PokeathlonCourse_GetDataCopyArea(void*);extern Game *PokeathlonCourse_GetHeapAllocPtr4(void*);extern Frame *ov96_021E8A20(void*);
extern int ov96_021E5F24(void*),ov96_021FDE6C(Game*,u8,u8),ov96_021FDE08(Game*,u8,u8,u8);
extern void ov96_021FDA30(Game*,Frame*),ov96_021FFE5C(Game*,u8,int),ov96_021E8228(void*,u8,u8,int,int),ov96_02200EF4(void*,u32,u16),ov96_021FDE7C(void*),ov96_021FF72C(const VecFx32*,const VecFx32*,VecFx32*);
extern float ov96_021FF6DC(Player*,int),_fflt(int);
static inline void clearDrag(PlayerWindow *window,Game *enabled){enabled->moving[0]=0;window->player.heldFrames=0;window->player.initial.x=0;window->player.initial.y=0;window->player.initial.z=0;window->player.current=window->player.initial;}
void ov96_021FD4D0(void*);
void ov96_021FD4D0(void *course){
 int i;Game *game;u8 *frames;int clear;float acceleration,maximum;Game *latches;PlayerWindow *window;Game *enabled;u8 *cursor;Player *player;VecFx32 *initial,*current,*velocity,*velocityAgain;Workspace workspace;frames=PokeathlonCourse_GetDataCopyArea(course);game=PokeathlonCourse_GetHeapAllocPtr4(course);
 if(ov96_021E5F24(course))return;
 if(game->finished){frames+=40;ov96_021FDA30(game,ov96_021E8A20(frames));return;}
 {Frame *destination=ov96_021E8A20(frames+80);*destination=*ov96_021E8A20(frames);}
 if(game->timer)game->timer--;ov96_021E8A20(frames+40);
 i=0;latches=game;window=(PlayerWindow*)game;enabled=game;cursor=frames+80;player=game->players;initial=&game->players[0].initial;current=&game->players[0].current;velocity=&game->players[0].velocity;velocityAgain=&game->players[0].velocity;
 for(;i<4;latches=(Game*)((u8*)latches+sizeof(Latch)),cursor+=40,window=(PlayerWindow*)((u8*)window+sizeof(Player)),player++,enabled=(Game*)((u8*)enabled+4),initial=(VecFx32*)((u8*)initial+sizeof(Player)),current=(VecFx32*)((u8*)current+sizeof(Player)),velocity=(VecFx32*)((u8*)velocity+sizeof(Player)),velocityAgain=(VecFx32*)((u8*)velocityAgain+sizeof(Player)),i++){
  Frame *frame=ov96_021E8A20(cursor);
  if(frame->down){if(latches->latches[0].down&&latches->latches[0].previous)latches->latches[0].down=0;else if(!latches->latches[0].down&&!latches->latches[0].previous){latches->latches[0].down=1;latches->latches[0].previous=1;}}
  else{latches->latches[0].down=0;latches->latches[0].previous=0;}
  ov96_021FFE5C(game,(u8)i,ov96_021FDE6C(game,frame->x,frame->y));
  if(window->player.swapping){if(window->player.swapPhase==1&&frame->phase==1){window->player.selected=(window->player.selected+1)%3;window->player.swapPhase=2;}else if(window->player.swapPhase==2&&frame->phase==2){window->player.swapPhase=0;window->player.swapping=0;window->player.gap3[2]=0;}clearDrag(window,enabled);continue;}
  if(window->player.athletes[window->player.selected].type==2){clearDrag(window,enabled);continue;}
  if(window->player.locked){clearDrag(window,enabled);continue;}
  clear=0;if(latches->latches[0].down){if(ov96_021FDE6C(game,frame->x,frame->y)){window->player.swapping=1;window->player.swapPhase=1;ov96_021E8228(course,window->player.id,window->player.selected,7,1);}else if(ov96_021FDE08(game,(u8)i,frame->x,frame->y)){enabled->moving[0]=1;window->player.initial.x=frame->x<<12;window->player.initial.y=frame->y<<12;window->player.initial.z=0;window->player.current=window->player.initial;}}
  else if(latches->latches[0].previous){if(window->player.heldFrames<255)window->player.heldFrames++;else game->timeout=1;if(!enabled->moving[0])continue;window->player.current.x=frame->x<<12;window->player.current.y=frame->y<<12;}
  else{if(enabled->moving[0]){if(window->player.heldFrames<=30)window->player.release=1;enabled->moving[0]=0;game->timeout=0;}clear=1;}
  if(window->player.release){float weight,speed;fx32 scale;{fx32 *zero=(fx32*)&workspace.zero;*zero++=0;*zero++=0;*zero=0;}
   acceleration=window->player.athletes[window->player.selected].acceleration;weight=window->player.athletes[window->player.selected].weight;maximum=ov96_021FF6DC(player,window->player.selected);scale=(fx32)(4096.0*((double)(acceleration*weight)/8.0));
   VEC_Subtract(current,initial,&workspace.delta);if(workspace.delta.x<0){if(workspace.delta.x<-65536){window->player.swapping=1;window->player.swapPhase=1;ov96_021E8228(course,window->player.id,window->player.selected,7,1);}workspace.delta.x=0;workspace.delta.y=0;workspace.delta.z=0;}
   VEC_MultAdd(scale,&workspace.delta,&workspace.zero,&workspace.delta);(void)_fflt(VEC_Mag(&workspace.delta)/4096);workspace.combined=window->player.velocity;ov96_021FF72C(&workspace.combined,&workspace.delta,&workspace.combined);speed=_fflt(VEC_Mag(&workspace.combined));
   if(speed>4096.0f*maximum){{fx32 *zero=(fx32*)&workspace.limitZero;*zero++=0;*zero++=0;*zero=0;}VEC_Normalize(&workspace.combined,&workspace.combined);VEC_MultAdd((fx32)(4096.0f*maximum),&workspace.combined,&workspace.limitZero,velocity);}else ov96_021FF72C(velocityAgain,&workspace.delta,velocity);
   {fx32 magnitude=VEC_Mag(&player->velocity);if(magnitude>=((int)player->athletes[player->selected].range-3)*4096)ov96_021E8228(course,player->id,player->selected,6,1);}
   window->player.release=0;
  }
  if(clear){window->player.heldFrames=0;window->player.initial.x=0;window->player.initial.y=0;window->player.initial.z=0;window->player.current=window->player.initial;}
 }
 ov96_02200EF4(course,game->score,game->timer);ov96_021FDE7C(course);if(game->timer==0)game->finished=1;else game->finished=0;frames+=40;ov96_021FDA30(game,ov96_021E8A20(frames));
}
