#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 item,padding;} Item;
typedef struct {Item*items;u16 pad; s16 start;u8 pad8,count,pad10[2];} Row;
typedef struct {u32 pad;Row rows[8];u8 selected,pad65;u16 value,kind;} Menu;
typedef struct {u8 prefix[0x234];Menu*menu;u8 rest[0x40c];int mode;u8 tail[0x1c0];u8 output[1];} Context;
int ov15_021FA68C(Context*,int);void ov15_021F9F08(Context*);int ov15_021FA074(Context*);
void ov15_021FD574(Context*,int,int,int),ov15_021FF364(Context*,int,int,int),ov15_02200030(Context*,int),ov15_021FF6BC(Context*,int,int,int),ov15_02200140(Context*,Row*,int,int),ov15_021FD404(Context*,int,int),ov15_021FA170(Context*),ov15_021FA0E4(Context*,int),ov15_021FDF88(Context*),ov15_021FDAF4(void*,int,int),ov15_021FD774(Context*,int);
BOOL PlaySE(u16);
int ov15_021FD7D0(Context*,int,int,int,int);int ov15_021FA73C(Context*,int,u8*,int,int,int);
int ov15_021FA73C(Context*ctx,int action,u8*changed,int ignored4,int ignored5,int force){int result=1;Row*row;int selected,index;switch(action){case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:selected=ov15_021FA68C(ctx,action);if(selected==-1)return result;if(selected==ctx->menu->selected&&!force)return result;ctx->menu->selected=selected;ov15_021F9F08(ctx);row=&ctx->menu->rows[ctx->menu->selected];ov15_021FD574(ctx,0,ov15_021FA074(ctx),0);ov15_021FF364(ctx,row->start,-1,0);ov15_02200030(ctx,ctx->menu->selected);ov15_021FF6BC(ctx,row->count,row->start,0);ov15_02200140(ctx,row,ov15_021FA074(ctx),1);ov15_021FD404(ctx,1,ctx->menu->selected);PlaySE(1500);ov15_021FA170(ctx);if(ctx->mode>=8)ov15_021FA0E4(ctx,ctx->mode);ov15_021FDF88(ctx);ov15_021FDAF4(ctx->output,ctx->menu->selected+1,7);break;
 case 8:case 9:case 10:case 11:case 12:case 13:row=&ctx->menu->rows[ctx->menu->selected];index=row->start+(action-8);if(index<row->count){ctx->menu->value=row->items[index].item;*changed=1;PlaySE(1500);}ov15_021FA170(ctx);break;
 case 15:if(ctx->menu->rows[ctx->menu->selected].count>6){PlaySE(1500);result=ov15_021FD7D0(ctx,18,9,8,30);}break;
 case 14:if(ctx->menu->rows[ctx->menu->selected].count>6){PlaySE(1500);result=ov15_021FD7D0(ctx,17,9,8,31);}break;
 case 16:ctx->menu->value=0;ctx->menu->kind=5;ov15_021FD774(ctx,force);PlaySE(2368);result=ov15_021FD7D0(ctx,19,9,8,36);break;}return result;}
