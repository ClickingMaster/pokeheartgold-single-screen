#ifndef POKEHEARTGOLD_SAVE_PLAZA_SURVEY_H
#define POKEHEARTGOLD_SAVE_PLAZA_SURVEY_H

#include "save.h"

typedef struct PlazaSurveySave PlazaSurveySave;

u32 Save_PlazaSurvey_sizeof(void);
void Save_PlazaSurvey_Init(PlazaSurveySave *data);
PlazaSurveySave *Save_PlazaSurvey_Get(SaveData *save);
u32 PlazaSurvey_GetSurveyId(PlazaSurveySave *data);
u32 PlazaSurvey_GetAnswer(PlazaSurveySave *data);
void PlazaSurvey_SetSurveyId(PlazaSurveySave *data, u32 surveyId);
void PlazaSurvey_SetAnswer(PlazaSurveySave *data, u32 answer);

#endif // POKEHEARTGOLD_SAVE_PLAZA_SURVEY_H
