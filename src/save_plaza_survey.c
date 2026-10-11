#include "save_plaza_survey.h"

#include "global.h"

#include "constants/save_arrays.h"

// The last Wi-Fi Plaza survey the player answered.
struct PlazaSurveySave {
    u32 surveyId; // 0xFFFFFFFF: none
    u32 answer;
};

u32 Save_PlazaSurvey_sizeof(void) {
    return sizeof(PlazaSurveySave);
}

void Save_PlazaSurvey_Init(PlazaSurveySave *data) {
    MI_CpuClear32(data, Save_PlazaSurvey_sizeof());
    data->surveyId = 0xFFFFFFFF;
}

PlazaSurveySave *Save_PlazaSurvey_Get(SaveData *save) {
    return SaveArray_Get(save, SAVE_PLAZA_SURVEY);
}

u32 PlazaSurvey_GetSurveyId(PlazaSurveySave *data) {
    return data->surveyId;
}

u32 PlazaSurvey_GetAnswer(PlazaSurveySave *data) {
    return data->answer;
}

void PlazaSurvey_SetSurveyId(PlazaSurveySave *data, u32 surveyId) {
    data->surveyId = surveyId;
}

void PlazaSurvey_SetAnswer(PlazaSurveySave *data, u32 answer) {
    data->answer = answer;
}
