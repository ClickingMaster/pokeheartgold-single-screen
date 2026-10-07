#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int x,y;} Position;
extern const Position ov08_02225534[],ov08_02225564[];
extern void ov08_02220A8C(void *,int,int),ov08_02220B90(int,void *,int,int),ov08_02220C08(int,void *,int,int),ov08_02220C3C(int,void *,int,int);
typedef struct {u8 data[0x2040];} State;
void ov08_02220CF4(State *);
void ov08_02220CF4(State *s){
 int i;u8 *a,*bits;const Position *first=ov08_02225534,*second=ov08_02225564;u8 *views;
 for(i=0,a=s->data,views=s->data,bits=s->data+27;i<6;bits+=80,a+=80,first++,views+=4,second++,i++){
  if(*(u16 *)(a+8)!=0){
   ov08_02220A8C(*(void **)(views+0x1fd4),first->x,first->y);
   ov08_02220B90((((u32)*bits<<25)>>28),*(void **)(views+0x1fec),second->x,second->y);
   ov08_02220C08(*(u16 *)(a+30),*(void **)(views+0x1fb8),first->x+8,first->y+8);
   ov08_02220C3C(a[49],*(void **)(views+0x2038),first->x+16,first->y+8);
  }
 }
}
