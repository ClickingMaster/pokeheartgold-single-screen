#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[64];void*player;u8 p44[160];void*follower;};int FollowMon_IsVisible(struct S*);void*PlayerAvatar_GetMapObject(void*);int MapObject_GetXCoord(void*);int MapObject_GetZCoord(void*);int ov01_02206268(struct S*);
int ov01_02206268(struct S*s){void*p,*f;int x,z,fx,fz;if(!FollowMon_IsVisible(s))return 0;p=PlayerAvatar_GetMapObject(s->player);f=s->follower;x=MapObject_GetXCoord(p);z=MapObject_GetZCoord(p);fx=MapObject_GetXCoord(f);fz=MapObject_GetZCoord(f);if(x==fx){if(z+1==fz || z-1==fz)return 1;}else if(z==fz){if(x+1==fx || x-1==fx)return 1;}return 0;}
