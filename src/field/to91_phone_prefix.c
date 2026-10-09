#include "constants/maps.h"
#include "constants/phone_scripts.h"
#include "constants/sndseq.h"

#include "overlay_2/overlay_02_gear_phone.h"

#include "map_header.h"
#include "math_util.h"
#include "msgdata.h"
#include "phonebook_dat.h"
#include "sav_system_info.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "unk_02092BE8.h"

#include "to91_phone_internal.h"

#pragma force_active on

String *GetPhoneBookEntryName(GearPhoneRingManager *gearPhone, enum HeapID heapID) {
    String *str;
    if (!gearPhone->active || gearPhone->callerId >= NUM_PHONE_CONTACTS) {
        str = String_New(8, heapID);
    } else {
        int phoneMsg = GetPhoneMessageGmm(gearPhone->callerId);
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, phoneMsg, heapID);
        str = NewString_ReadMsgData(msgData, 0);
        DestroyMsgData(msgData);
    }
    return str;
}

void GearPhoneRingManager_SetCallerParams(GearPhoneRingManager *gearPhone, u8 callerId, u32 a2, u8 a3, u8 isScriptedCall, u8 callScriptID) {
    // a4 set to 2 when passed from script
    // a5 related to message id? when passed from script
    if (callerId >= NUM_PHONE_CONTACTS) {
        gearPhone->callerId = 0xFF;
        gearPhone->unk_arr5[0] = 0xFF;
        gearPhone->unk_arr5[1] = 0;
        GF_ASSERT(FALSE);
    } else {
        gearPhone->callerId = callerId;
        gearPhone->unk_arr5[0] = a2;
        gearPhone->unk_arr5[1] = a3;
    }

    gearPhone->isScriptedCall = isScriptedCall;
    gearPhone->callScriptID = callScriptID;
}

u8 ov02_02251EE8(GearPhoneRingManager *gearPhone, Unk_PokegearSTRUCT_14 *a1) {
    MI_CpuFill8(a1, 0, 5);
    if (gearPhone->callerId >= NUM_PHONE_CONTACTS) {
        GF_ASSERT(FALSE);
        a1->unk_0 = 0xFF;
        return 0xFF;
    }
    a1->unk_0 = gearPhone->unk_arr5[0];
    a1->unk_1 = gearPhone->unk_arr5[1];
    a1->isScriptedCall = gearPhone->isScriptedCall;
    a1->callScriptID = gearPhone->callScriptID;
    a1->callTriggerID = gearPhone->callTriggerID;
    return gearPhone->callerId;
}

BOOL ov02_02251F20(GearPhoneRingManager *gearPhone) {
    Location *position = LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(gearPhone->saveData));

    if (!MapHeader_CanReceivePhoneCalls(position->mapId)) {
        return FALSE;
    }

    if (gearPhone->active || gearPhone->unk_var8 < gearPhone->unk_varC) {
        return FALSE;
    }

    PhoneBook *phoneBook = AllocAndReadPhoneBook(HEAP_ID_FIELD1);
    u32 var = ov02_022521C0(gearPhone, phoneBook, position->mapId);
    if (var) {
        FreePhoneBook(phoneBook);
        if (var == 2) {
            gearPhone->sys->unkD2_7 = TRUE;
            StartMapSceneScript(gearPhone->sys, 0x7FF, 0);
            return TRUE;
        }
        ov02_022522AC(gearPhone, 1);
        return FALSE;
    }
    u32 rand = MTRandom() % 100;

    if (rand > gearPhone->unk_var12) {
        FreePhoneBook(phoneBook);
        gearPhone->unk_var8 = 0;
        return FALSE;
    }

    var = ov02_02251FDC(gearPhone, phoneBook, position->mapId);
    FreePhoneBook(phoneBook);

    if (var == 0) {
        gearPhone->unk_var8 = 0;
    }
    return FALSE;
}
