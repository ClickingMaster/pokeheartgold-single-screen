#include "to51_unk_02058D4C_private.h"

BOOL sub_02058D4C(void *param0) {
    Work *v0 = TaskManager_GetEnvironment(param0);
    FieldSystemMini *fieldSystem = TaskManager_GetFieldSystem(param0);

    switch (v0->unk_34) {
    case 0:
        v0->unk_43--;

        if (v0->unk_43 == 0) {
            v0->unk_34 = 1;
            sub_02057E08();
        }
        break;
    case 1:
        ReadMsgDataIntoString(v0->unk_2C, 1, v0->unk_0C);
        v0->unk_30 = sub_02059478(v0, v0->unk_0C);
        v0->unk_34 = 2;
        break;
    case 2:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            sub_02037AC0(93);
            v0->unk_34 = 3;
        }
        break;
    case 3:
        if (sub_02037B38(93)) {
            v0->unk_34 = 7;
            sub_0205F55C(v0->fieldSystem->mapObjectManager);
            v0->unk_08(1, v0->unk_50);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            v0->unk_34 = 4;
            sub_02037AC0(92);
            v0->unk_43 = 5;
        }
        break;
    case 4:
        if (sub_02037B38(93)) {
            v0->unk_34 = 7;
            sub_0205F55C(v0->fieldSystem->mapObjectManager);
            v0->unk_08(1, v0->unk_50);
        }

        v0->unk_43--;

        if (v0->unk_43 == 0) {
            v0->unk_34 = 8;
        }
        break;
    case 7:
        sub_02059650(v0);
        Heap_Free(v0);
        return 1;
    case 5:
        sub_02059650(v0);
        Heap_Free(v0);
        sub_02057F70();
        return 1;
    case 8:
        if (sub_02037B38(93)) {
            v0->unk_34 = 5;
            v0->unk_08(1, v0->unk_50);
        } else {
            v0->unk_08(0, v0->unk_50);
            v0->unk_34 = 5;
        }
        break;
    case 9:
        v0->unk_34 = 10;
        v0->unk_44 = 5;
        break;
    case 10:
        if (v0->unk_44 != 0) {
            v0->unk_44--;
        } else {
            if (MapObject_IsMovementPaused(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar))) {
                v0->unk_34 = 11;
            }
        }
        break;
    case 11:
        sub_02057E08();
        ReadMsgDataIntoString(v0->unk_2C, 13, v0->unk_0C);

        v0->unk_30 = sub_02059478(v0, v0->unk_0C);
        v0->unk_34 = 12;
        break;
    case 12:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            v0->unk_34 = 13;
        }
        break;
    case 13:
        FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
        v0->unk_34 = 14;
        break;
    case 14:
        v0->unk_43--;

        if (v0->unk_43 == 0) {
            sub_02057E08();
            v0->unk_34 = 15;
        }
        break;
    case 15:
        sub_020594C8(v0, 0);
        sub_02058B84(v0, HEAP_ID_FIELD2);
        v0->unk_34 = 16;
        break;
    case 16:
        if (sub_02058C80(v0, v0->fieldSystem)) {
            switch (v0->unk_38) {
            case 0:
                v0->unk_34 = 20;
                break;
            case 1:
                v0->unk_34 = 19;
                break;
            case 2:
                v0->unk_34 = 17;
                break;
            }
        }
        break;
    case 17:
        sub_02058AEC(v0, v0->fieldSystem, SaveArray_Party_Get(v0->fieldSystem->saveData), v0->unk_3C, 0, HEAP_ID_FIELD2);
        v0->unk_34 = 18;
        break;
    case 18:
        if (sub_02058CD8(v0, v0->fieldSystem)) {
            v0->unk_34 = 15;
        }
        break;
    case 19:
        FieldSystem_LoadFieldOverlay(v0->fieldSystem);

        if (v0->unk_88 != 3) {
            v0->unk_43 = 5;
            v0->unk_34 = 21;
        } else {
            v0->unk_34 = 26;
        }
        break;
    case 21:
        sub_0203996C(v0->unk_3D);

        if (sub_02058D04(v0)) {
            v0->unk_43 = 5;
            v0->unk_34 = 0;
        }
        break;
    case 20:
        FieldSystem_LoadFieldOverlay(v0->fieldSystem);

        if (v0->unk_88 != 3) {
            v0->unk_34 = 22;
        } else {
            v0->unk_34 = 26;
        }
        break;
    case 22:
        if (sub_02058D04(v0)) {
            v0->unk_34 = 8;
        }
        break;
    case 23:
        v0->unk_44--;

        if (v0->unk_44 == 0) {
            v0->unk_34 = 24;
        }
        break;
    case 24:
        sub_02057E08();
        ReadMsgDataIntoString(v0->unk_2C, 19, v0->unk_0C);
        v0->unk_30 = sub_02059478(v0, v0->unk_0C);
        v0->unk_34 = 25;
        break;
    case 25:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            v0->unk_34 = 13;
        }
        break;
    case 26:
        if (sub_02058D04(v0)) {
            if (sub_02058D24()) {
                v0->unk_34 = 5;
            } else {
                v0->unk_82 = (v0->unk_38 != 0);
                sub_020596A8(v0, v0->unk_82);
                sub_02037AC0(0);
                BufferPlayersName(v0->unk_28, 0, v0->unk_74);
                ReadMsgDataIntoString(v0->unk_2C, 14, v0->unk_0C);
                StringExpandPlaceholders(v0->unk_28, v0->unk_10, v0->unk_0C);
                v0->unk_30 = sub_02059478(v0, v0->unk_10);
                v0->unk_34 = 27;
            }
        }
        break;
    case 27:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            if (sub_02058D24()) {
                v0->unk_34 = 5;
            } else if (sub_02037B38(0)) {
                sub_020596F0(v0);
                v0->unk_34 = 28;
            }
        }
        break;
    case 28:
        if (sub_02059738(v0)) {
            sub_02037AC0(1);
            v0->unk_34 = 29;
        }
        break;
    case 29:
        if (sub_02037B38(1)) {
            v0->unk_83 = sub_02059748(v0);

            if (v0->unk_82 && v0->unk_83) {
                ReadMsgDataIntoString(v0->unk_2C, 20, v0->unk_0C);
                v0->unk_30 = sub_02059478(v0, v0->unk_0C);
                v0->unk_34 = 30;
            } else {
                v0->unk_34 = 42;
            }
        }
        break;
    case 30:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            v0->unk_89 = 0;
            ReadMsgDataIntoString(v0->unk_2C, 17, v0->unk_0C);
            v0->unk_30 = sub_02059478(v0, v0->unk_0C);
            v0->unk_84 = 0;
            v0->unk_34 = 31;
        }
        break;
    case 31:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            sub_020597A8(v0);
            sub_02059820(v0, v0->unk_84);
            v0->unk_34 = 32;
        }
        break;
    case 32:
        switch (sub_02059A08(v0)) {
        case 1:
            sub_02059AD8(v0);
            v0->unk_84 = v0->unk_81;
            v0->unk_34 = 36;
            break;
        case 2:
            sub_02059AD8(v0);
            v0->unk_84 = 255;
            ReadMsgDataIntoString(v0->unk_2C, 15, v0->unk_0C);
            v0->unk_30 = sub_02059478(v0, v0->unk_0C);
            sub_02037AC0(2);
            v0->unk_34 = 39;
            break;
        }
        break;
    case 36:
        BufferBoxMonSpeciesName(v0->unk_28, 1, Mon_GetBoxMon(Party_GetMonByIndex(v0->unk_50, v0->unk_84)));
        ReadMsgDataIntoString(v0->unk_2C, 18, v0->unk_0C);
        StringExpandPlaceholders(v0->unk_28, v0->unk_10, v0->unk_0C);
        v0->unk_30 = sub_02059478(v0, v0->unk_10);
        v0->unk_34 = 37;
        break;
    case 37:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            sub_0205993C(v0, 0);
            v0->unk_34 = 38;
        }
        break;
    case 38:
        switch (sub_02059A08(v0)) {
        case 2:
            sub_02059AD8(v0);
            ReadMsgDataIntoString(v0->unk_2C, 17, v0->unk_0C);
            v0->unk_30 = sub_02059478(v0, v0->unk_0C);
            v0->unk_34 = 31;
            break;
        case 1:
            if (v0->unk_81 == 1) {
                sub_02059AD8(v0);
                ReadMsgDataIntoString(v0->unk_2C, 14, v0->unk_0C);
                StringExpandPlaceholders(v0->unk_28, v0->unk_10, v0->unk_0C);
                v0->unk_30 = sub_02059478(v0, v0->unk_10);
                sub_02037AC0(2);
                v0->unk_34 = 39;
            } else {
                FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
                v0->unk_34 = 33;
            }
            break;
        }
        break;
    case 33:
        if (IsPaletteFadeFinished()) {
            sub_020594C8(v0, 0);
            sub_02058AEC(v0, v0->fieldSystem, v0->unk_50, v0->unk_84, 1, HEAP_ID_FIELD2);
            v0->unk_34 = 34;
        }
        break;
    case 34:
        if (sub_02058CD8(v0, v0->fieldSystem)) {
            FieldSystem_LoadFieldOverlay(v0->fieldSystem);
            v0->unk_34 = 35;
        }
        break;
    case 35:
        if (sub_02058D04(v0)) {
            v0->unk_34 = 36;
        }
        break;
    case 39:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            if (sub_02037B38(2)) {
                sub_0205975C(v0);
                v0->unk_34 = 41;
            }
        }
        break;
    case 41:
        if (sub_02059798(v0)) {
            if (v0->unk_84 == 255) {
                sub_02037AC0(4);
                v0->unk_34 = 44;
            } else if (v0->unk_85 == 255) {
                v0->unk_34 = 42;
            } else {
                sub_020597D4(v0);
                sub_02037AC0(93);
                v0->unk_34 = 2;
            }
        }
        break;
    case 42:
        ReadMsgDataIntoString(v0->unk_2C, 15, v0->unk_0C);
        v0->unk_30 = sub_02059478(v0, v0->unk_0C);
        v0->unk_43 = 0;
        v0->unk_34 = 43;
        break;
    case 43:
        if (DialogBox_IsPrintFinished(v0->unk_30)) {
            if (++(v0->unk_43) > 60) {
                sub_02037AC0(4);
                v0->unk_34 = 44;
            }
        }
        break;
    case 44:
        if (sub_02037B38(4)) {
            ClearFrameAndWindow2(&(v0->unk_14), 0);
            v0->unk_08(0, NULL);
            v0->unk_34 = 5;
        }
        break;
    }

    return 0;
}
