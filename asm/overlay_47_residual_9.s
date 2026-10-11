	.include "asm/macros.inc"
	.include "overlay_47.inc"
	.include "global.inc"
	.include "overlay47_exact_bank.inc"

	.text
	.public WiFiPlazaSurvey_VBlankCB
	.public WiFiPlazaSurvey_InitGraphics
	.public WiFiPlazaSurvey_FreeGraphics
	.public WiFiPlazaSurvey_RenderSprites
	.public WiFiPlazaSurvey_DoVBlankUpdates
	.public WiFiPlazaSurvey_FreeBgs
	.public WiFiPlazaSurvey_FreeSpriteSystem
	.public WiFiPlazaSurvey_FreeMessages
	.public WiFiPlazaSurvey_GetMessage
	.public WiFiPlazaSurvey_GetQuestionTextFromMsg
	.public WiFiPlazaSurvey_GetAnswerTextFromMsg
	.public WiFiPlazaSurvey_BufferPlayerName
	.public WiFiPlazaSurveyAnswer_Init
	.public WiFiPlazaSurveyAnswer_Main
	.public WiFiPlazaSurveyAnswer_UpdateCursor
	.public WiFiPlazaSurveyAnswer_StartCursorAnim
	.public WiFiPlazaSurvey_InitMessageBox
	.public WiFiPlazaSurvey_FreeMessageBox
	.public WiFiPlazaSurvey_PrintMessage
	.public WiFiPlazaSurvey_PrintMessageInstant
	.public WiFiPlazaSurvey_StartWaitingIcon
	.public WiFiPlazaSurvey_StopWaitingIcon
	.public WiFiPlazaSurvey_IsMessageFinished
	.public WiFiPlazaSurvey_ClearMessageBox
	.public WiFiPlazaSurvey_CreateYesNoMenu
	.public WiFiPlazaSurvey_HandleYesNoMenu
	.public WiFiPlazaSurvey_DestroyYesNoMenu
	.public WiFiPlazaSurveyResults_Init
	.public WiFiPlazaSurveyResults_Main
	.public WiFiPlazaSurveyResults_Teardown
	.public WiFiPlazaSurveyResults_PrintTitle
	.public WiFiPlazaSurveyResults_StartBars
	.public WiFiPlazaSurvey_FreeQuestion
	.public WiFiPlazaSurvey_GetQuestionText
	.public WiFiPlazaSurvey_GetAnswerText
	.public WiFiPlazaSurvey_GetBarLength
	.public WiFiPlazaSurvey_CountPlazaAnswers

	thumb_func_start WiFiPlazaSurvey_LoadLastWeekTotals
WiFiPlazaSurvey_LoadLastWeekTotals: ; 0x02259D74
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r6, #0
	str r6, [r5, #0xc]
	add r4, r5, #0
	str r6, [r5, #0x10]
	add r4, #0xc
	add r7, r6, #0
_02259D84:
	add r0, r6, #0
	add r0, #8
	bl ov45_0222F544
	str r0, [r5]
	ldr r1, [r4]
	ldr r2, [r4, #4]
	add r0, r1, r0
	adc r2, r7
	str r0, [r4]
	add r6, r6, #1
	add r5, r5, #4
	str r2, [r4, #4]
	cmp r6, #3
	blt _02259D84
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end WiFiPlazaSurvey_LoadLastWeekTotals
