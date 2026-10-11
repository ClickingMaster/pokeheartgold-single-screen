#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x18];s16 x,y,z,angle,height,pad22;s32 scale;} Picture;
typedef struct {u8 pad[20];Picture picture;} Container;
typedef struct {VecFx16 rotation;u16 pad;VecFx32 scale;} Transform;
extern Container *sub_02023F90(void*);
extern const VecFx32 *sub_02023E68(void*),*sub_02023E94(void*);
extern int GF3d_BoundsBoxTest(VecFx32*,Transform*);
int ov45_02230E78(void*);
int ov45_02230E78(void *object) {VecFx32 position;Transform transform;MtxFx33 matrix;Container *container=sub_02023F90(object);Picture *picture;if(container)picture=&container->picture;else picture=0;position=*sub_02023E68(object);transform.rotation.x=picture->angle;transform.rotation.y=picture->height;transform.rotation.z=picture->height;transform.scale.x=picture->scale>>12;transform.scale.y=picture->scale>>12;transform.scale.z=picture->scale>>12;position.x+=(s32)(((s64)picture->x*picture->scale+0x800)>>12);position.y+=(s32)(((s64)picture->y*picture->scale+0x800)>>12);position.z+=(s32)(((s64)picture->z*picture->scale+0x800)>>12);position.z-=(s32)(((s64)picture->height*picture->scale+0x800)>>12);MTX_Identity33_(&matrix);MI_Copy36B(&matrix,&NNS_G3dGlb.prmBaseRot);NNS_G3dGlb.flag&=~0xa4;NNS_G3dGlbSetBaseScale(sub_02023E94(object));if(GF3d_BoundsBoxTest(&position,&transform)==0)return 0;return 1;}
