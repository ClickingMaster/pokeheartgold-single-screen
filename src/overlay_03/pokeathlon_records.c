#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0277.h"

#include "field_system.h"
#include "font.h"
#include "overlay_03.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"

typedef struct PokeathlonRecordsWork {
    int state;
    enum PokeathlonData pokeathlonDataType;
    FieldSystem *fieldSystem;
    BgConfig *bgConfig;
    Window window;
    PokeathlonSave *pokeathlonSave;
    PokeathlonStatistics *pokeathlonStruct;
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *strings[4];
} PokeathlonRecordsWork;

static BOOL Task_PokeathlonRecords(TaskManager *taskManager);
static BOOL PokeathlonRecords_WaitInput(PokeathlonRecordsWork *data);
static void PokeathlonRecords_Init(PokeathlonRecordsWork *data);
static void PokeathlonRecords_Free(PokeathlonRecordsWork *data);
static void PokeathlonRecords_PrintPage(PokeathlonRecordsWork *data, enum PokeathlonData dataType);
static void PokeathlonRecords_PrintHiddenRecord(PokeathlonRecordsWork *data, int y);
static void PokeathlonRecords_PrintRecord(PokeathlonRecordsWork *data, int y, int strno);
static u32 PokeathlonRecords_GetValue(PokeathlonStatistics *pokeathlonStruct, int strno);

void PokeathlonRecords_Show(TaskManager *taskManager, enum PokeathlonData dataType) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    PokeathlonRecordsWork *data = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokeathlonRecordsWork));
    data->state = 0;
    data->pokeathlonDataType = dataType;
    data->bgConfig = fieldSystem->bgConfig;
    data->fieldSystem = fieldSystem;
    data->pokeathlonSave = Save_Pokeathlon_Get(fieldSystem->saveData);
    data->pokeathlonStruct = PokeathlonSave_GetUnkB00(data->pokeathlonSave);
    TaskManager_Call(fieldSystem->taskman, &Task_PokeathlonRecords, data);
}

static BOOL Task_PokeathlonRecords(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    PokeathlonRecordsWork *data = (PokeathlonRecordsWork *)TaskManager_GetEnvironment(taskManager);
    switch (data->state) {
    case 0:
        PokeathlonRecords_Init(data);
        PokeathlonRecords_PrintPage(data, data->pokeathlonDataType);
        data->state++;
        break;
    case 1:
        if (PokeathlonRecords_WaitInput(data)) {
            PokeathlonRecords_Free(data);
            Heap_Free(data);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL PokeathlonRecords_WaitInput(PokeathlonRecordsWork *data) {
    if (System_GetTouchNew()) {
        PlaySE(SEQ_SE_DP_DECIDE);
        MenuInputStateMgr_SetState(&data->fieldSystem->menuInputState, MENU_INPUT_STATE_TOUCH);
        return TRUE;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_Y | PAD_BUTTON_X) & gSystem.newKeys) {
        MenuInputStateMgr_SetState(&data->fieldSystem->menuInputState, MENU_INPUT_STATE_BUTTONS);
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return TRUE;
    }
    return FALSE;
}

static void PokeathlonRecords_Init(PokeathlonRecordsWork *data) {
    AddWindowParameterized(data->bgConfig, &data->window, 3, 2, 1, 28, 22, 13, 1);
    LoadUserFrameGfx1(data->bgConfig, GF_BG_LYR_MAIN_3, 985, HEAP_ID_FIELD2, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&data->window, TRUE, 985, HEAP_ID_FIELD2);
    FillWindowPixelBuffer(&data->window, 15);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0277_bin, HEAP_ID_FIELD1);
    data->messageFormat = MessageFormat_New_Custom(3, 16, HEAP_ID_FIELD1);
    data->strings[0] = String_New(128, HEAP_ID_FIELD1);
    data->strings[1] = String_New(128, HEAP_ID_FIELD1);
    data->strings[2] = NewString_ReadMsgData(data->msgData, 1);
    data->strings[3] = NewString_ReadMsgData(data->msgData, 3);
}

static void PokeathlonRecords_Free(PokeathlonRecordsWork *data) {
    String_Delete(data->strings[3]);
    String_Delete(data->strings[2]);
    String_Delete(data->strings[1]);
    String_Delete(data->strings[0]);
    MessageFormat_Delete(data->messageFormat);
    DestroyMsgData(data->msgData);
    EraseFrameAndWindow1(&data->window, TRUE);
    RemoveWindow(&data->window);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, 3);
}

static u8 sPokeathlonRecordFlags[] = { // 32?
    0,
    1,
    2,
    25,
    21,
    22,
    0,
    19,
    20,
    27,
    5,
    7,
    18,
    8,
    9,
    17,
    6,
    16,
    10,
    24,
    26,
    3,
    4,
    14,
    23,
    13,
    11,
    12,
    15,
    0,
    0,
    0
};

static BOOL PokeathlonRecords_IsRecordShown(PokeathlonRecordsWork *data, int index_02259880) {
    u8 index = sPokeathlonRecordFlags[index_02259880];
    if (index == 0) {
        return TRUE;
    }
    return PokeathlonSave_GetUnkB78_AtIndex(data->pokeathlonSave, index - 1);
}

static const int sPokeathlonRecordCounts[3] = {
    10,
    10,
    9
};

static void PokeathlonRecords_PrintPage(PokeathlonRecordsWork *data, enum PokeathlonData dataType) {
    for (int i = 0; i < sPokeathlonRecordCounts[dataType]; i++) {
        int j = dataType * 10 + i;
        int k = 8 + 16 * i;
        if (PokeathlonRecords_IsRecordShown(data, j)) {
            PokeathlonRecords_PrintRecord(data, k, j);
        } else {
            PokeathlonRecords_PrintHiddenRecord(data, k);
        }
    }

    ScheduleWindowCopyToVram(&data->window);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, 3);
}

static void PokeathlonRecords_PrintHiddenRecord(PokeathlonRecordsWork *data, int y) {
    AddTextPrinterParameterizedWithColor(&data->window, 0, data->strings[3], 8, y, TEXT_SPEED_NOTRANSFER, 0x1020F, NULL);
    AddTextPrinterParameterizedWithColor(&data->window, 0, data->strings[2], 8, y, TEXT_SPEED_NOTRANSFER, 0x1020F, NULL);
}

static void PokeathlonRecords_PrintRecord(PokeathlonRecordsWork *data, int y, int strno) {
    ReadMsgDataIntoString(data->msgData, strno + 4, data->strings[1]);
    AddTextPrinterParameterizedWithColor(&data->window, 0, data->strings[1], 8, y, TEXT_SPEED_NOTRANSFER, 0x1020F, NULL);
    u32 seconds = PokeathlonRecords_GetValue(data->pokeathlonStruct, strno);
    if (seconds >= 9999999) {
        seconds = 9999999;
    }
    if (strno == 9) {
        ReadMsgDataIntoString(data->msgData, msg_0277_00002, data->strings[0]); // {STRVAR_1 52, 0, 0}:{STRVAR_1 51, 1, 0}
        u16 minutes = seconds / 60;
        u16 displaySeconds = seconds % 60;
        if (minutes > 999) {
            minutes = 999;
        }
        BufferIntegerAsString(data->messageFormat, 0, minutes, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(data->messageFormat, 1, displaySeconds, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        StringExpandPlaceholders(data->messageFormat, data->strings[1], data->strings[0]);
    } else {
        String16_FormatInteger(data->strings[1], seconds, 7, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    }
    u32 width = 224 - FontID_String_GetWidth(0, data->strings[1], 0);
    AddTextPrinterParameterizedWithColor(&data->window, 0, data->strings[1], width - 8, y, TEXT_SPEED_NOTRANSFER, 0x1020F, NULL);
}

static u32 PokeathlonRecords_GetValue(PokeathlonStatistics *pokeathlonStruct, int strno) {
    if (strno >= 10 && strno <= 19) {
        strno -= 10;
        return pokeathlonStruct->eventFirstPlaces[strno];
    }

    switch (strno) {
    case 0:
        return pokeathlonStruct->coursesJoined;
    case 1:
        return pokeathlonStruct->courseFirstPlaces;
    case 2:
        return pokeathlonStruct->courseLastPlaces;
    case 3:
        return pokeathlonStruct->bonusesEarned;
    case 4:
        int i;
        int var_r3 = 0;
        for (i = 0; i < 10; i++) {
            var_r3 += pokeathlonStruct->eventFirstPlaces[i];
        }
        return var_r3;
    case 5:
        return pokeathlonStruct->eventLastPlaces;
    case 6:
        return pokeathlonStruct->linkCoursesJoined;
    case 7:
        return pokeathlonStruct->linkCourseFirstPlaces;
    case 8:
        return pokeathlonStruct->linkCourseLastPlaces;
    case 9:
        return pokeathlonStruct->playTime;
    case 20:
        return pokeathlonStruct->monInstructions;
    case 21:
        return pokeathlonStruct->monDashes;
    case 22:
        return pokeathlonStruct->monJumps;
    case 23:
        return pokeathlonStruct->monTackles;
    case 24:
        return pokeathlonStruct->monSwitches;
    case 25:
        return pokeathlonStruct->monSelfImpedes;
    case 26:
        return pokeathlonStruct->monPointsEarned;
    case 27:
        return pokeathlonStruct->monFailures;
    case 28:
        return pokeathlonStruct->monFalls;
    default:
        return 0;
    }
}
