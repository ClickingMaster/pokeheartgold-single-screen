#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 item,padding;} Item;
typedef struct {Item*items;u16 pad; s16 start;u8 pad8,count,pad10[2];} Row;
typedef struct {u32 pad;Row rows[8];u8 selected,pad65;u16 value,kind;} Menu;
typedef struct {u8 prefix[0x234];Menu*menu;u8 rest[0x40c];int mode;u8 tail[0x1c0];u8 output[1];} Context;
int BagApp_FindPocketIndex(Context*,int);void BagApp_LoadPocketItemNames(Context*);int BagApp_GetPageItemCount(Context*);
void BagApp_LoadSubScreenLayout(Context*,int,int,int),BagApp_DrawItemList(Context*,int,int,int),BagApp_LoadPocketObjPalette(Context*,int),BagApp_PrintPageNumber(Context*,int,int,int),BagApp_DrawItemIcons(Context*,Row*,int,int),BagApp_LoadPocketSubBgPalette(Context*,int,int),BagApp_PrintCursorDescription(Context*),BagApp_SetPocketPositionFromButton(Context*,int),BagApp_SetBagModelPocketAnim(Context*),BagApp_StartCameraTween(void*,int,int),BagApp_SetMenuInputState(Context*,int);
BOOL PlaySE(u16);
int BagApp_StartButtonFlash(Context*,int,int,int,int);int BagApp_PressButton(Context*,int,u8*,int,int,int);
int BagApp_PressButton(Context*ctx,int action,u8*changed,int ignored4,int ignored5,int force){int result=1;Row*row;int selected,index;switch(action){case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:selected=BagApp_FindPocketIndex(ctx,action);if(selected==-1)return result;if(selected==ctx->menu->selected&&!force)return result;ctx->menu->selected=selected;BagApp_LoadPocketItemNames(ctx);row=&ctx->menu->rows[ctx->menu->selected];BagApp_LoadSubScreenLayout(ctx,0,BagApp_GetPageItemCount(ctx),0);BagApp_DrawItemList(ctx,row->start,-1,0);BagApp_LoadPocketObjPalette(ctx,ctx->menu->selected);BagApp_PrintPageNumber(ctx,row->count,row->start,0);BagApp_DrawItemIcons(ctx,row,BagApp_GetPageItemCount(ctx),1);BagApp_LoadPocketSubBgPalette(ctx,1,ctx->menu->selected);PlaySE(1500);BagApp_PrintCursorDescription(ctx);if(ctx->mode>=8)BagApp_SetPocketPositionFromButton(ctx,ctx->mode);BagApp_SetBagModelPocketAnim(ctx);BagApp_StartCameraTween(ctx->output,ctx->menu->selected+1,7);break;
 case 8:case 9:case 10:case 11:case 12:case 13:row=&ctx->menu->rows[ctx->menu->selected];index=row->start+(action-8);if(index<row->count){ctx->menu->value=row->items[index].item;*changed=1;PlaySE(1500);}BagApp_PrintCursorDescription(ctx);break;
 case 15:if(ctx->menu->rows[ctx->menu->selected].count>6){PlaySE(1500);result=BagApp_StartButtonFlash(ctx,18,9,8,30);}break;
 case 14:if(ctx->menu->rows[ctx->menu->selected].count>6){PlaySE(1500);result=BagApp_StartButtonFlash(ctx,17,9,8,31);}break;
 case 16:ctx->menu->value=0;ctx->menu->kind=5;BagApp_SetMenuInputState(ctx,force);PlaySE(2368);result=BagApp_StartButtonFlash(ctx,19,9,8,36);break;}return result;}
