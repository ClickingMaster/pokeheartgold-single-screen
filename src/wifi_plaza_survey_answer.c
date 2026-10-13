#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0777.h"

#include "sound_playback.h"
#include "system.h"
#include "wifi_plaza_survey.h"

// Built without -ipa file (Makefile), so it cannot join wifi_plaza_survey.c.

#pragma force_active on

BOOL WiFiPlazaSurveyAnswer_Run(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    switch (answer->state) {
    case 0:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00094));
        answer->nextState = 1;
        answer->state = 10;
        break;
    case 1:
        WiFiPlazaSurvey_CreateYesNoMenu(msgBox, work, heapID);
        answer->state = 2;
        break;
    case 2:
        switch (WiFiPlazaSurvey_HandleYesNoMenu(msgBox, heapID)) {
        case 0:
            answer->state = 3;
            break;
        case LIST_CANCEL:
            answer->state = 9;
            break;
        }
        break;
    case 3:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00092 : msg_0777_00093));
        answer->nextState = 4;
        answer->state = 10;
        break;
    case 4:
        WiFiPlazaSurveyAnswer_PrintAnswers(answer, messages, work);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetQuestionText(&answer->question, messages));
        WiFiPlazaSurveyAnswer_UpdateCursor(answer);
        WiFiPlazaSurveyAnswer_StartCursorAnim(answer);
        answer->state = 5;
        break;
    case 5:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            answer->state++;
            break;
        } else if (gSystem.newKeys & PAD_KEY_UP) {
            if ((int)answer->cursor - 1 >= 0) {
                PlaySE(SEQ_SE_DP_SELECT78);
                answer->cursor--;
            }
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            if ((int)answer->cursor + 1 < WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
                PlaySE(SEQ_SE_DP_SELECT78);
                answer->cursor++;
            }
        }
        WiFiPlazaSurveyAnswer_UpdateCursor(answer);
        break;
    case 6:
        WiFiPlazaWork_SetSurveyAnswer(args->lobby, answer->question.surveyId, answer->cursor);
        WiFiPlazaLobbyClient_SubmitSurveyAnswer(answer->cursor);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00096));
        WiFiPlazaSurvey_StartWaitingIcon(msgBox);
        PlaySE(SEQ_SE_PL_BREC80);
        answer->state = 7;
        break;
    case 7:
        if (WiFiPlazaLobbyClient_GetSurveySubmitState() == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_BUTTON9);
        WiFiPlazaSurvey_StopWaitingIcon(msgBox);
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00097));
        answer->nextState = 8;
        answer->state = 10;
        break;
    case 8:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00098 : msg_0777_00099));
        answer->nextState = 9;
        answer->state = 10;
        break;
    case 9:
        WiFiPlazaSurvey_ClearMessageBox(msgBox);
        return TRUE;
    case 10:
        if (WiFiPlazaSurvey_IsMessageFinished(msgBox) == TRUE) {
            answer->state = answer->nextState;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

#pragma force_active reset
