#pragma force_active on
#include "global.h"
#include "bag_types_def.h"
typedef struct Work { u8 prefix[0x234]; BagView *view; } Work;
int BagApp_FindPocketIndex(Work *w, int pocket);

// @fn BagApp_FindPocketIndex
int BagApp_FindPocketIndex(Work *w, int pocket) {
int i; for (i = 0; i < 8; i++) { if (pocket == w->view->pockets[i].pocketId && w->view->pockets[i].slots != NULL) return i; } return -1;
}
