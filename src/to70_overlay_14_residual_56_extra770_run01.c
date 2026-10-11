#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x40c8];u8 images[1][1024];}Data;typedef struct {u8 p[0x34];Data*data;}Work;typedef NNSG2dCharacterData Image;
extern void*AllocAtEndAndReadWholeNarcMemberByIdPair(int,int,int);extern int PCBox_GetWallpaperGfxIndex(Work*,int);extern void Heap_Free(void*),PCBox_ColorWallpaperThumbnail(Work*,void*,int,int,int),PCBox_DrawBoxThumbnailMons(Work*,int,void*);
void PCBox_DrawBoxThumbnail(Work*,int);
void PCBox_DrawBoxThumbnail(Work*w,int n){Image*img;void*allocation=AllocAtEndAndReadWholeNarcMemberByIdPair(19,70,10);NNS_G2dGetUnpackedCharacterData(allocation,&img);MI_CpuCopy8(img->pRawData,w->data->images[n],1024);Heap_Free(allocation);PCBox_ColorWallpaperThumbnail(w,w->data->images[n],PCBox_GetWallpaperGfxIndex(w,n),8,1024);PCBox_DrawBoxThumbnailMons(w,n,w->data->images[n]);}
