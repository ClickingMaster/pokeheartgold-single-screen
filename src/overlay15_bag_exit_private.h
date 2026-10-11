#ifndef TO46_OVERLAY15_PREFIX_PRIVATE_H
#define TO46_OVERLAY15_PREFIX_PRIVATE_H

#include "global.h"

#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_15.h"
#include "overlay_manager.h"
#include "pm_string.h"

void BagApp_Free3D(void *work);
void BagApp_FreeSprites(void *work);
void ov15_021FA0D8(void *work);
void BagApp_SaveCursorPositions(void *work);
void BagApp_RemoveWindows(void *work);
void BagApp_FreeBgLayers(void *bgConfig);
void BagApp_FreeItemMenuStrings(void *work);
void ov15_021FE504(void *work);
void ov15_021FE8A4(void *work);
void BagApp_FreeItemNameStrings(void *work);
void GF_TouchpadStopSampling(void);
void GF_DestroyVramTransferManager(void);
void sub_02004B10(void);

#endif
