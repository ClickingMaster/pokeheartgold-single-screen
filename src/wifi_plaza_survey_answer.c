#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0777.h"

#include "sound_playback.h"
#include "system.h"
#include "wifi_plaza_survey.h"

enum WiFiPlazaSurveyAnswerState {
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_INTRO,
    WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO,
    WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO,
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO,
    WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS,
    WIFI_PLAZA_SURVEY_ANSWER_SELECT,
    WIFI_PLAZA_SURVEY_ANSWER_SUBMIT,
    WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT,
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS,
    WIFI_PLAZA_SURVEY_ANSWER_FINISH,
    WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE,
};

BOOL WiFiPlazaSurveyAnswer_Run(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    switch (answer->state) {
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00094));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO:
        WiFiPlazaSurvey_CreateYesNoMenu(msgBox, work, heapID);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO:
        switch (WiFiPlazaSurvey_HandleYesNoMenu(msgBox, heapID)) {
        case 0:
            answer->state = WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO;
            break;
        case LIST_CANCEL:
            answer->state = WIFI_PLAZA_SURVEY_ANSWER_FINISH;
            break;
        }
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00092 : msg_0777_00093));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS:
        WiFiPlazaSurveyAnswer_PrintAnswers(answer, messages, work);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetQuestionText(&answer->question, messages));
        WiFiPlazaSurveyAnswer_UpdateCursor(answer);
        WiFiPlazaSurveyAnswer_StartCursorAnim(answer);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_SELECT;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_SELECT:
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
    case WIFI_PLAZA_SURVEY_ANSWER_SUBMIT:
        WiFiPlazaWork_SetSurveyAnswer(args->lobby, answer->question.surveyId, answer->cursor);
        WiFiPlazaLobbyClient_SubmitSurveyAnswer(answer->cursor);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00096));
        WiFiPlazaSurvey_StartWaitingIcon(msgBox);
        PlaySE(SEQ_SE_PL_BREC80);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT:
        if (WiFiPlazaLobbyClient_GetSurveySubmitState() == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_BUTTON9);
        WiFiPlazaSurvey_StopWaitingIcon(msgBox);
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00097));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00098 : msg_0777_00099));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_FINISH;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_FINISH:
        WiFiPlazaSurvey_ClearMessageBox(msgBox);
        return TRUE;
    case WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE:
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
