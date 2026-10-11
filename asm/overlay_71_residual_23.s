	.include "asm/macros.inc"
	.public TradeSequence_Exit
	.public TradeSequence_Init
	.public TradeSequence_Main
	.public _02246970
	.public _022469A6
	.public _022469EE
	.public _02246A20
	.public _02246A2C
	.public _02246A44
	.public _02246B06
	.public _02246B0C
	.public _02246B14
	.public _02246B18
	.public _02246B1C
	.public _02246B20
	.public _02246B24
	.public _02246B52
	.public _02246BEC
	.public _02246C24
	.public _02246C2C
	.public _02246C34
	.public _02246C38
	.public _02246C3C
	.public _02246C40
	.public _02246C44
	.public _02246C64
	.public _02246C68
	.public _02246CA6
	.public _02246CD6
	.public _02246D28
	.public _02246D30
	.public _02246D34
	.public _02246D38
	.public _02246D3C
	.public _02246D52
	.public _02246D66
	.public _02246D78
	.public _02246D9A
	.public _02246DD8
	.public _02246E1A
	.public _02246E5C
	.public _02246E7C
	.public _02246E80
	.public _02246E94
	.public _02246EA0
	.public _02246EDE
	.public _02246EE0
	.public _02246F52
	.public _02246F58
	.public _02246F5C
	.public _02246F8E
	.public _02246FC2
	.public _02246FD0
	.public _02246FEA
	.public _02246FF8
	.public _02246FFC
	.public _02247034
	.public _02247058
	.public _02247060
	.public _02247064
	.public _0224708A
	.public _02247096
	.public _022470A0
	.public _022470AC
	.public _022470B4
	.public _022470D0
	.public _02247120
	.public _0224713C
	.public _02247140
	.public _02247164
	.public _02247166
	.public _022471A0
	.public _022471B8
	.public _02247202
	.public _0224720C
	.public _0224720E
	.public _02247228
	.public _0224722C
	.public _02247242
	.public _02247250
	.public _02247252
	.public _022472BA
	.public _022472C0
	.public _0224730E
	.public _0224731C
	.public _0224737C
	.public _022473B8
	.public _022473CC
	.public _022473D8
	.public _022473EC
	.public _02247402
	.public _02247412
	.public _02247418
	.public _0224741C
	.public _02247420
	.public _02247434
	.public _02247444
	.public _02247448
	.public _02247482
	.public _02247492
	.public _022474A6
	.public _022474B8
	.public _022474EA
	.public _022474F8
	.public _02247502
	.public _022475AC
	.public _022475C0
	.public _0224760C
	.public _02247668
	.public _0224766A
	.public _02247674
	.public _02247676
	.public _022476AC
	.public _02247722
	.public _022477E8
	.public _02247818
	.public _02247828
	.public _02247838
	.public _02247848
	.public _022478B4
	.public _0224791E
	.public _02247980
	.public _0224798C
	.public _022479B4
	.public _022479BE
	.public _022479C4
	.public _02247C0C
	.public _02247C10
	.public _02247C14
	.public _02247C18
	.public _02247C1C
	.public _02247C20
	.public _02247C24
	.public _02247C28
	.public _02247C2C
	.public _02247C30
	.public _02247C5A
	.public _02247C7A
	.public _02247C84
	.public _02247C90
	.public _02247C9A
	.public _02247C9C
	.public _02247CBC
	.public _02247CC0
	.public _02247D04
	.public _02247D80
	.public _02247DB4
	.public _02247DD4
	.public _02247DE4
	.public _02247E06
	.public _02247E10
	.public _02247E2A
	.public _02247E50
	.public _02247E7C
	.public _02247E8E
	.public _02247EAE
	.public _02247EC6
	.public _02247ECC
	.public _02247F78
	.public _02247F7C
	.public _02247F80
	.public _02247F84
	.public _02247F88
	.public _02247F8C
	.public _02247F90
	.public _02247F94
	.public _02247F98
	.public _022480A4
	.public _022480AE
	.public _02248106
	.public _02248108
	.public _0224810C
	.public _0224811A
	.public _0224813C
	.public _02248162
	.public _02248178
	.public _022481BC
	.public _022481C0
	.public _022481C4
	.public _022481D2
	.public _022481E8
	.public _02248208
	.public _0224822E
	.public _0224825C
	.public _02248272
	.public _02248278
	.public _02248280
	.public _022482E2
	.public _022482E4
	.public _022482E8
	.public _0224834E
	.public _02248354
	.public _02248386
	.public _02248394
	.public _022483A4
	.public _022483D0
	.public _022483DA
	.public _022483E0
	.public _02248404
	.public _0224844C
	.public _02248462
	.public _0224846E
	.public _0224847A
	.public _02248480
	.public _0224849C
	.public _022484A6
	.public _022484B2
	.public _022484F6
	.public _0224850C
	.public _02248522
	.public _0224852A
	.public _0224854A
	.public _02248554
	.public _0224856A
	.public _02248588
	.public _022485A4
	.public _022485B8
	.public _022485E6
	.public _022485F4
	.public _022485FC
	.public _02248600
	.public _0224880A
	.public _02248880
	.public _02248884
	.public _02248888
	.public _0224888C
	.public _02248890
	.public _02248894
	.public _02248898
	.public _022488B2
	.public _022489C4
	.public _022489C8
	.public _022489CC
	.public _022489D0
	.public _022489D4
	.public _022489D8
	.public _022489DC
	.public _022489E0
	.public _022489E4
	.public _022489E8
	.public _022489EC
	.public _022489F0
	.public _022489F4
	.public _02248A06
	.public _02248AA2
	.public _02248B20
	.public _02248B2C
	.public _02248B36
	.public _02248B40
	.public _02248BB4
	.public _02248BB8
	.public _02248BD8
	.public _02248C1C
	.public _02248C20
	.public _02248C58
	.public _02248C66
	.public _02248CA6
	.public _02248D0A
	.public _02248D78
	.public _02248D7C
	.public _02248E02
	.public _02248E2E
	.public _02248E70
	.public _02248E74
	.public _02248E9A
	.public _02248F38
	.public _02248F84
	.public _02248F88
	.public _02248FBA
	.public _02248FBE
	.public _02249000
	.public _0224902A
	.public _0224905E
	.public _02249068
	.public _02249090
	.public _022490B6
	.public _0224911E
	.public _0224914A
	.public _02249156
	.public _022491E4
	.public _02249200
	.public _02249204
	.public _02249252
	.public _022492F4
	.public _022492F8
	.public _0224933C
	.public _02249354
	.public _0224939A
	.public _022493B0
	.public _022493B4
	.public _022493F8
	.public _02249410
	.public _02249446
	.public _0224945C
	.public _0224949C
	.public _022494CA
	.public _022494FE
	.public _02249524
	.public _0224952E
	.public _02249534
	.public _02249554
	.public _02249598
	.public _0224959C
	.public _022495BA
	.public _022495C4
	.public _022495D0
	.public _022495FA
	.public _02249624
	.public _0224964E
	.public _0224965C
	.public _02249664
	.public _02249668
	.public _0224966C
	.public _022496BA
	.public _022496C2
	.public _022496CE
	.public _022496DA
	.public _022496E6
	.public _022496F0
	.public _022497B4
	.public _022497B8
	.public _022497BC
	.public _022497C0
	.public _022497C4
	.public _022498B4
	.public _022498B8
	.public _022498BC
	.public _022498C0
	.public _022498C4
	.public _022498C8
	.public _022498CC
	.public _022498D0
	.public _022498D4
	.public _022498D8
	.public _022498DC
	.public _022498EC
	.public _0224997E
	.public _022499A2
	.public _02249A34
	.public _02249A38
	.public _02249A3C
	.public _02249A40
	.public _02249A44
	.public _02249A48
	.public _02249A4C
	.public _02249A5A
	.public _02249A96
	.public _02249AB2
	.public _02249AEC
	.public _02249B74
	.public _02249B7C
	.public _02249B88
	.public _02249B92
	.public _02249BC6
	.public _02249BE0
	.public _02249C04
	.public _02249C4A
	.public _02249C7A
	.public _02249C8C
	.public _02249CE4
	.public _02249CE8
	.public _02249CEC
	.public _02249D0E
	.public _02249D24
	.public _02249D4C
	.public _02249D50
	.public _02249D54
	.public _02249D7C
	.public _02249D98
	.public _02249DCA
	.public _02249DF6
	.public _02249E04
	.public _02249E0C
	.public _02249E10
	.public _02249E14
	.public _02249E18
	.public _02249E1C
	.public _02249E20
	.public _02249E52
	.public _0224A064
	.public _0224A068
	.public _0224A06C
	.public _0224A070
	.public _0224A074
	.public _0224A078
	.public _0224A07C
	.public _0224A0EC
	.public _0224A124
	.public _0224A128
	.public _0224A15A
	.public _0224A180
	.public _0224A184
	.public _0224A188
	.public _0224A1C0
	.public _0224A1DE
	.public _0224A1E6
	.public _0224A206
	.public _0224A230
	.public _0224A25C
	.public _0224A26A
	.public _0224A270
	.public _0224A274
	.public _0224A2C2
	.public _0224A2CA
	.public _0224A2D6
	.public _0224A2E2
	.public _0224A2EE
	.public _0224A2F8
	.public _0224A3C8
	.public _0224A3CC
	.public _0224A3D0
	.public _0224A3D4
	.public _0224A4D0
	.public _0224A4D4
	.public _0224A4D8
	.public _0224A4DC
	.public _0224A4E0
	.public _0224A4E4
	.public _0224A4E8
	.public _0224A4EC
	.public _0224A4F0
	.public _0224A4F4
	.public _0224A4F8
	.public _0224A4FC
	.public _0224A50C
	.public _0224A59E
	.public _0224A5C2
	.public _0224A664
	.public _0224A668
	.public _0224A66C
	.public _0224A670
	.public _0224A674
	.public _0224A678
	.public _0224A67C
	.public _0224A680
	.public _0224A68E
	.public _0224A6D4
	.public _0224A6FC
	.public _0224A730
	.public _0224A73A
	.public _0224A744
	.public _0224A7BA
	.public _0224A7CC
	.public _0224A7DE
	.public _0224A7F0
	.public _0224A7F8
	.public _0224A81E
	.public _0224A836
	.public _0224A858
	.public _0224A892
	.public _0224A8A4
	.public _0224A8CE
	.public _0224A910
	.public _0224A914
	.public _0224A918
	.public _0224A91C
	.public _0224A94E
	.public _0224A96E
	.public _0224A978
	.public _0224A982
	.public _0224A988
	.public _0224A9B0
	.public _0224A9BA
	.public _0224A9C0
	.public _0224A9F6
	.public _0224AA24
	.public _0224AA40
	.public _0224AA4E
	.public _0224AA6C
	.public _0224AA86
	.public _0224AA98
	.public _0224AAB0
	.public _0224AAD4
	.public _0224AAE8
	.public _0224AAF2
	.public _0224AB10
	.public _0224AB38
	.public _0224AB62
	.public _0224AB76
	.public _0224AD52
	.public _0224AD94
	.public _0224AD98
	.public _0224AD9C
	.public _0224ADA0
	.public _0224ADA4
	.public _0224ADA8
	.public _0224ADAC
	.public _0224ADC0
	.public _0224AEC8
	.public _0224AECC
	.public _0224AED0
	.public _0224AED4
	.public _0224AED8
	.public _0224AEDC
	.public _0224AEE0
	.public _0224AEE4
	.public _0224AEE8
	.public _0224AEEC
	.public _0224AEF0
	.public _0224AF06
	.public _0224AF48
	.public _0224AF4C
	.public _0224AF50
	.public _0224AF54
	.public _0224AF98
	.public _0224AFD2
	.public _0224B020
	.public _0224B024
	.public _0224B04E
	.public _0224B082
	.public _0224B0DC
	.public _0224B0E0
	.public _0224B0E4
	.public _0224B108
	.public _0224B136
	.public _0224B194
	.public _0224B1C8
	.public _0224B1E2
	.public _0224B1F8
	.public _0224B220
	.public _0224B22A
	.public _0224B230
	.public _0224B272
	.public _0224B430
	.public _0224B434
	.public _0224B438
	.public _0224B43C
	.public _0224B440
	.public _0224B444
	.public _0224B448
	.public _0224B44C
	.public _0224B476
	.public _0224B496
	.public _0224B4A0
	.public _0224B4AE
	.public _0224B4DC
	.public _0224B504
	.public _0224B55A
	.public _0224B566
	.public _0224B576
	.public _0224B596
	.public _0224B5A2
	.public _0224B5AE
	.public _0224B5B8
	.public _0224B5BA
	.public _0224B5DA
	.public _0224B5DE
	.public _0224B62C
	.public _0224B68E
	.public _0224B6AE
	.public _0224B6D8
	.public _0224B6E6
	.public _0224B6EC
	.public _0224B702
	.public _0224B710
	.public _0224B71A
	.public _0224B7C8
	.public _0224B7CC
	.public _0224B7D0
	.public _0224B7D4
	.public _0224B7D8
	.public _0224B7DC
	.public _0224B7E0
	.public _0224B7E4
	.public _0224B7E8
	.public _0224B8F4
	.public _0224B8FE
	.public _0224B956
	.public _0224B958
	.public _0224B95C
	.public _0224B96A
	.public _0224B98C
	.public _0224B9B2
	.public _0224B9C8
	.public _0224BA3A
	.public _0224BA3E
	.public _0224BA40
	.public _0224BA44
	.public _0224BA62
	.public _0224BA86
	.public _0224BA9C
	.public _0224BAB8
	.public _0224BADA
	.public _0224BAE6
	.public _0224BB06
	.public _0224BB16
	.public _0224BB2E
	.public _0224BB44
	.public _0224BB8C
	.public _0224BB9E
	.public _0224BBBA
	.public _0224BBCA
	.public _0224BBCC
	.public _0224BBD0
	.public sTradeSequenceBgZoomCenterY
	.public _0224C020
	.public sTradeSequenceDeferredFreeCount
	.public TradeSequence_BoxMonCantFlip
	.public TradeSequence_DrawSpritesTask
	.public TradeSequence_CreatePulses
	.public TradeSequence_StopPulses
	.public TradeSequence_DestroyPulses
	.public TradeSequence_PulsesTask
	.public TradeSequence_SetPulseColor
	.public TradeSequence_StartPulse
	.public TradeSequence_DestroyPulse
	.public TradeSequence_PulseTask
	.public TradeSequence_StartBgZoom
	.public TradeSequence_BgZoomTask
	.public TradeSequence_ApplyBgZoom
	.public TradeSequence_LoadMonPicToBg
	.public TradeSequence_LoadMonPicTilemap
	.public TradeSequence_LoadCellAnim
	.public TradeSequence_FreeCellAnim
	.public TradeSequence_InitSpriteHeader
	.public TradeSequence_CreateSprite
	.public TradeSequence_GetBgConfig
	.public TradeSequence_GetMsgFormat
	.public TradeSequence_GetMsgData
	.public TradeSequence_GetSentBoxMon
	.public TradeSequence_GetReceivedBoxMon
	.public TradeSequence_GetTimeOfDay
	.public TradeSequence_GetSentSpecies
	.public TradeSequence_GetSentForm
	.public TradeSequence_GetReceivedSpecies
	.public TradeSequence_GetReceivedForm
	.public TradeSequence_GetFrameType
	.public TradeSequence_GetTradeMode
	.public TradeSequence_ResetDeferredFrees
	.public TradeSequence_FlushDeferredFrees
	.public TradeSequence_DeferFree
	.public TradeSequence_CreateModels
	.public TradeSequence_DestroyModels
	.public TradeSequence_DrawModels
	.public TradeSequence_DrawModel
	.public TradeSequence_InitModel
	.public TradeSequence_FreeModel
	.public TradeSequence_LoadModel
	.public TradeSequence_GetModelPosition
	.public TradeSequence_SetModelPosition
	.public TradeSequence_GetModelRotation
	.public TradeSequence_SetModelRotation
	.public TradeSequence_SetModelVisible
	.public TradeSequence_SetModelAlpha
	.public TradeSequence_SetModelScale
	.public TradeSequence_IsModelInView
	.public TradeSequence_GetCameraPos
	.public TradeSequence_SetCameraAngle
	.public TradeSequence_AdjustCameraAngle
	.public TradeSequence_SetCameraPerspectiveType
	.public TradeSequence_SetCameraPerspectiveAngle
	.public TradeSequence_InitCamera
	.public TradeSequence_FreeCamera
	.public TradeSequenceSend_Init
	.public TradeSequenceSend_Exit
	.public TradeSequenceSend_Main
	.public TradeSequenceSend_Draw
	.public TradeSequenceSend_SetupScreens
	.public TradeSequenceSend_WaitFadeIn
	.public TradeSequenceSend_ShowMessages
	.public TradeSequenceSend_ReplaceMonWithModel
	.public TradeSequenceSend_Init3D
	.public TradeSequenceSend_CreateMonPic
	.public TradeSequenceSend_CreateSprites
	.public TradeSequenceSend_DeleteSprites
	.public TradeSequenceSend_StartBrightness
	.public TradeSequenceSend_IsBrightnessDone
	.public TradeSequenceSend_StopBrightness
	.public TradeSequenceSend_BrightnessTask
	.public TradeSequenceSend_StartBounce
	.public TradeSequenceSend_IsBounceDone
	.public TradeSequenceSend_GetBounceCount
	.public TradeSequenceSend_StopBounce
	.public TradeSequenceSend_BounceTask
	.public TradeSequenceDepart_Init
	.public TradeSequenceDepart_Exit
	.public TradeSequenceDepart_Main
	.public TradeSequenceDepart_Draw
	.public TradeSequenceDepart_SetupScreens
	.public TradeSequenceDepart_WaitBgZoom
	.public TradeSequenceDepart_FlyOffTopScreen
	.public TradeSequenceDepart_FlyThroughBottomScreen
	.public TradeSequenceDepart_InitBgs
	.public TradeSequenceDepart_FreeBgs
	.public TradeSequenceDepart_Init3D
	.public TradeSequenceDepart_Free3D
	.public TradeSequenceDepart_CreateStreaks
	.public TradeSequenceDepart_DeleteStreaks
	.public TradeSequenceDepart_RequestScreenSwap
	.public TradeSequenceDepart_ScreenSwapTask
	.public TradeSequenceDepart_StartStreakFall
	.public TradeSequenceDepart_StreakFallTask
	.public TradeSequenceDepart_StopStreakFall
	.public TradeSequenceDepart_StartScroll
	.public TradeSequenceDepart_ScrollTask
	.public TradeSequenceDepart_StopScroll
	.public TradeSequenceDepart_StartAlphaFade
	.public TradeSequenceDepart_AlphaFadeTask
	.public TradeSequenceDepart_StartFlight
	.public TradeSequenceDepart_SetAngleSpeed
	.public TradeSequenceDepart_SetRadius
	.public TradeSequenceDepart_SetSpin
	.public TradeSequenceDepart_UpdateFlight
	.public TradeSequenceDepart_FlightTask_Appear
	.public TradeSequenceDepart_FlightTask_FlyAway
	.public TradeSequenceDepart_IsFlightOutOfView
	.public TradeSequenceDepart_IsFlightFlyingAway
	.public TradeSequenceDepart_RestartFlightFromBelow
	.public TradeSequenceDepart_FlightTask_Rise
	.public TradeSequenceDepart_FlightTask_Hover
	.public TradeSequenceDepart_FlightTask_Sink
	.public TradeSequenceDepart_FlightTask_Pause
	.public TradeSequenceDepart_FlightTask_Leave
	.public TradeSequenceRecede_Init
	.public TradeSequenceRecede_Exit
	.public TradeSequenceRecede_Main
	.public TradeSequenceRecede_Draw
	.public TradeSequenceRecede_SetupScreens
	.public TradeSequenceRecede_Run
	.public TradeSequenceRecede_InitBgs
	.public TradeSequenceRecede_FreeBgs
	.public TradeSequenceRecede_Init3D
	.public TradeSequenceRecede_Free3D
	.public TradeSequenceRecede_CreateSprite
	.public TradeSequenceRecede_DeleteSprite
	.public TradeSequenceRecede_InitFlight
	.public TradeSequenceRecede_StopFlight
	.public TradeSequenceRecede_StartFlight
	.public TradeSequenceRecede_IsFlightDone
	.public TradeSequenceRecede_SetFlightTargets
	.public TradeSequenceRecede_UpdateFlight
	.public TradeSequenceRecede_FlightTask
	.public TradeSequenceRecede_RequestScreenSwap
	.public TradeSequenceRecede_ScreenSwapTask
	.public TradeSequenceExchange_Init
	.public TradeSequenceExchange_Exit
	.public TradeSequenceExchange_Main
	.public TradeSequenceExchange_SetupScreens
	.public TradeSequenceExchange_Slide
	.public TradeSequenceExchange_SlideTask
	.public TradeSequenceExchange_InitBgs
	.public TradeSequenceExchange_FreeBgs
	.public TradeSequenceExchange_SetMonPicPositions
	.public TradeSequenceApproach_Init
	.public TradeSequenceApproach_Exit
	.public TradeSequenceApproach_Main
	.public TradeSequenceApproach_SetupScreens
	.public TradeSequenceApproach_Run
	.public TradeSequenceApproach_InitBgs
	.public TradeSequenceApproach_FreeBgs
	.public TradeSequenceApproach_Init3D
	.public TradeSequenceApproach_Free3D
	.public TradeSequenceApproach_CreateSprite
	.public TradeSequenceApproach_DeleteSprite
	.public TradeSequenceApproach_InitFlight
	.public TradeSequenceApproach_StopFlight
	.public TradeSequenceApproach_StartFlight
	.public TradeSequenceApproach_IsFlightDone
	.public TradeSequenceApproach_SetFlightTargets
	.public TradeSequenceApproach_UpdateFlight
	.public TradeSequenceApproach_FlightTask
	.public TradeSequenceApproach_RequestScreenSwap
	.public TradeSequenceApproach_VBlankCallback
	.public TradeSequenceArrive_Init
	.public TradeSequenceArrive_Exit
	.public TradeSequenceArrive_Main
	.public TradeSequenceArrive_Draw
	.public TradeSequenceArrive_SetupScreens
	.public TradeSequenceArrive_LowerAndFade
	.public TradeSequenceArrive_ZoomOut
	.public TradeSequenceArrive_InitBgs
	.public TradeSequenceArrive_FreeBgs
	.public TradeSequenceArrive_Init3D
	.public TradeSequenceArrive_Free3D
	.public TradeSequenceArrive_StartScroll
	.public TradeSequenceArrive_ScrollTask
	.public TradeSequenceArrive_StopScroll
	.public TradeSequenceArrive_StartLower
	.public TradeSequenceArrive_LowerTask
	.public TradeSequenceArrive_StopLower
	.public TradeSequenceArrive_StartAlphaFade
	.public TradeSequenceArrive_AlphaFadeTask
	.public TradeSequenceArrive_StopAlphaFade
	.public TradeSequenceReceive_Init
	.public TradeSequenceReceive_Exit
	.public TradeSequenceReceive_Main
	.public TradeSequenceReceive_Draw
	.public TradeSequenceReceive_SetupScreens
	.public TradeSequenceReceive_WaitFadeIn
	.public TradeSequenceReceive_ShowMon
	.public TradeSequenceReceive_ShowMessages
	.public TradeSequenceReceive_WaitFramesAndFanfare
	.public TradeSequenceReceive_Init3D
	.public TradeSequenceReceive_CreateMonPic
	.public TradeSequenceReceive_CreateSprites
	.public TradeSequenceReceive_DeleteSprites
	.public TradeSequenceReceive_StartBrightness
	.public TradeSequenceReceive_IsBrightnessDone
	.public TradeSequenceReceive_StopBrightness
	.public TradeSequenceReceive_BrightnessTask
	.public TradeSequenceReceive_StartBounce
	.public TradeSequenceReceive_StopBounce
	.public TradeSequenceReceive_UpdateSpin
	.public TradeSequenceReceive_SetSpin
	.public TradeSequenceReceive_BounceTask
	.public sTradeSequenceMonPicSettings
	.public sTradeSequenceSceneInit
	.public sTradeSequenceSceneMain
	.public sTradeSequenceSceneExit
	.public sTradeSequenceSceneModes
	.public sTradeSequenceSendModelRotation
	.public sTradeSequenceSendStates
	.public sTradeSequenceSendGraphicsModes
	.public sTradeSequenceSendBgTemplate_Text
	.public sTradeSequenceSendBgTemplate_Background
	.public sTradeSequenceSendGraphicsBanks
	.public sTradeSequenceDepartPaletteOffsets
	.public sTradeSequenceDepartGraphicsModes
	.public sTradeSequenceDepartStates
	.public sTradeSequenceDepartEdgeColors
	.public sTradeSequenceDepartBgTemplate_Layer2
	.public sTradeSequenceDepartBgTemplate_Layer3
	.public sTradeSequenceDepartGraphicsBanks
	.public sTradeSequenceRecedeStates
	.public sTradeSequenceRecedeGraphicsModes
	.public sTradeSequenceRecedeEdgeColors
	.public sTradeSequenceRecedeBgTemplate
	.public sTradeSequenceRecedeGraphicsBanks
	.public sTradeSequenceExchangeStates
	.public sTradeSequenceExchangeGraphicsModes
	.public sTradeSequenceExchangeBgTemplate_SentMon
	.public sTradeSequenceExchangeBgTemplate_ReceivedMon
	.public sTradeSequenceExchangeBgTemplate_Background
	.public sTradeSequenceExchangeGraphicsBanks
	.public sTradeSequenceApproachStates
	.public sTradeSequenceApproachGraphicsModes
	.public sTradeSequenceApproachEdgeColors
	.public sTradeSequenceApproachBgTemplate
	.public sTradeSequenceApproachGraphicsBanks
	.public sTradeSequenceArrivePaletteOffsets
	.public sTradeSequenceArriveStates
	.public sTradeSequenceArriveGraphicsModes
	.public sTradeSequenceArriveEdgeColors
	.public sTradeSequenceArriveBgTemplate_Layer2
	.public sTradeSequenceArriveBgTemplate_Layer3
	.public sTradeSequenceArriveGraphicsBanks
	.public sTradeSequenceReceiveModelRotation
	.public sTradeSequenceReceiveStates
	.public sTradeSequenceReceiveGraphicsModes
	.public sTradeSequenceReceiveBgTemplate_Text
	.public sTradeSequenceReceiveBgTemplate_Background
	.public sTradeSequenceReceiveGraphicsBanks
	.public sTradeSequenceDeferredFrees
	.public ov71_0224C0C4
	.public sTradeSequenceDepartCameraAngle
	.public sTradeSequenceArriveCameraAngle
	.include "overlay_71.inc"
	.include "global.inc"

	.text
	.public TradeSequence_BoxMonCantFlip
	.public TradeSequence_StopPulses
	.public TradeSequence_SetPulseColor
	.public TradeSequence_DestroyPulse
	.public TradeSequence_FreeCellAnim
	.public TradeSequence_GetBgConfig
	.public TradeSequence_GetMsgFormat
	.public TradeSequence_GetMsgData
	.public TradeSequence_GetSentBoxMon
	.public TradeSequence_GetReceivedBoxMon
	.public TradeSequence_GetTimeOfDay
	.public TradeSequence_GetSentSpecies
	.public TradeSequence_GetSentForm
	.public TradeSequence_GetReceivedSpecies
	.public TradeSequence_GetReceivedForm
	.public TradeSequence_GetFrameType
	.public TradeSequence_GetTradeMode
	.public TradeSequence_ResetDeferredFrees
	.public TradeSequence_FlushDeferredFrees
	.public TradeSequence_DeferFree
	.public TradeSequence_FreeModel
	.public TradeSequence_GetModelPosition
	.public TradeSequence_SetModelPosition
	.public TradeSequence_GetModelRotation
	.public TradeSequence_SetModelRotation
	.public TradeSequence_SetModelVisible
	.public TradeSequence_SetModelScale
	.public TradeSequence_GetCameraPos
	.public TradeSequence_SetCameraAngle
	.public TradeSequence_AdjustCameraAngle
	.public TradeSequence_SetCameraPerspectiveType
	.public TradeSequence_SetCameraPerspectiveAngle
	.public TradeSequence_FreeCamera
	.public TradeSequenceSend_Main
	.public TradeSequenceSend_WaitFadeIn
	.public TradeSequenceSend_DeleteSprites
	.public TradeSequenceSend_IsBrightnessDone
	.public TradeSequenceSend_StopBrightness
	.public TradeSequenceSend_IsBounceDone
	.public TradeSequenceSend_GetBounceCount
	.public TradeSequenceSend_StopBounce
	.public TradeSequenceDepart_Main
	.public TradeSequenceDepart_Draw
	.public TradeSequenceDepart_Free3D
	.public TradeSequenceDepart_DeleteStreaks
	.public TradeSequenceDepart_RequestScreenSwap
	.public TradeSequenceDepart_StopStreakFall
	.public TradeSequenceDepart_StopScroll
	.public TradeSequenceDepart_IsFlightOutOfView
	.public TradeSequenceDepart_IsFlightFlyingAway
	.public TradeSequenceRecede_Init
	.public TradeSequenceRecede_Exit
	.public TradeSequenceRecede_Main
	.public TradeSequenceRecede_Draw
	.public TradeSequenceRecede_FreeBgs
	.public TradeSequenceRecede_Free3D
	.public TradeSequenceRecede_DeleteSprite
	.public TradeSequenceRecede_InitFlight
	.public TradeSequenceRecede_StopFlight
	.public TradeSequenceRecede_IsFlightDone
	.public TradeSequenceRecede_RequestScreenSwap
	.public TradeSequenceExchange_Init
	.public TradeSequenceExchange_Exit
	.public TradeSequenceExchange_Main
	.public TradeSequenceExchange_SetupScreens
	.public TradeSequenceExchange_FreeBgs
	.public TradeSequenceExchange_SetMonPicPositions
	.public TradeSequenceApproach_Exit
	.public TradeSequenceApproach_Main
	.public TradeSequenceApproach_FreeBgs
	.public TradeSequenceApproach_Free3D
	.public TradeSequenceApproach_DeleteSprite
	.public TradeSequenceApproach_InitFlight
	.public TradeSequenceApproach_StopFlight
	.public TradeSequenceApproach_IsFlightDone
	.public TradeSequenceApproach_RequestScreenSwap
	.public TradeSequenceArrive_Init
	.public TradeSequenceArrive_Exit
	.public TradeSequenceArrive_Main
	.public TradeSequenceArrive_SetupScreens
	.public TradeSequenceArrive_FreeBgs
	.public TradeSequenceArrive_Free3D
	.public TradeSequenceArrive_StopScroll
	.public TradeSequenceArrive_StopLower
	.public TradeSequenceArrive_AlphaFadeTask
	.public TradeSequenceArrive_StopAlphaFade
	.public TradeSequenceReceive_WaitFadeIn
	.public TradeSequenceReceive_WaitFramesAndFanfare
	.public TradeSequenceReceive_DeleteSprites
	.public TradeSequenceReceive_IsBrightnessDone
	.public TradeSequenceReceive_StopBrightness
	.public TradeSequenceReceive_BrightnessTask
	.public TradeSequenceReceive_StopBounce
	.public TradeSequenceReceive_SetSpin

	thumb_func_start TradeSequenceDepart_RestartFlightFromBelow
TradeSequenceDepart_RestartFlightFromBelow: ; 0x0224926C
	push {r3, r4, r5, lr}
	sub sp, #8
	add r5, r0, #0
	bl SysTask_GetData
	add r4, r0, #0
	mov r1, #0
	str r1, [r4, #0x78]
	add r0, sp, #0
	strh r1, [r0, #4]
	ldrh r1, [r0, #4]
	strh r1, [r0, #2]
	strh r1, [r0]
	ldr r0, [r4, #4]
	mov r1, #1
	bl TradeSequence_SetCameraPerspectiveType
	ldr r0, [r4, #4]
	add r1, sp, #0
	bl TradeSequence_SetCameraAngle
	ldr r1, _022492F4 ; =0x000058CC
	mov r0, #2
	lsl r0, r0, #0x12
	str r1, [r4, #0x2c]
	sub r0, r1, r0
	str r0, [r4, #0x28]
	add r0, r4, #0
	mov r1, #0
	mov r3, #0x14
	add r0, #0x80
	str r3, [r0]
	ldr r0, [r4, #0x28]
	add r2, r1, #0
	str r0, [r4, #0x18]
	add r0, r4, #0
	bl TradeSequenceDepart_SetRadius
	mov r2, #0x56
	ldrsh r2, [r4, r2]
	add r0, r4, #0
	mov r1, #1
	neg r2, r2
	mov r3, #0x1e
	bl TradeSequenceDepart_SetSpin
	mov r1, #2
	add r2, r1, #0
	add r0, r4, #0
	add r2, #0xfe
	mov r3, #0x32
	bl TradeSequenceDepart_SetSpin
	mov r1, #2
	ldr r0, [r4]
	lsl r1, r1, #0xc
	bl TradeSequence_SetModelScale
	ldr r0, [r4]
	mov r1, #1
	bl TradeSequence_SetModelVisible
	ldr r1, _022492F8 ; =TradeSequenceDepart_FlightTask_Rise
	add r0, r5, #0
	bl SysTask_SetFunc
	add sp, #8
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022492F4: .word 0x000058CC
_022492F8: .word TradeSequenceDepart_FlightTask_Rise
	thumb_func_end TradeSequenceDepart_RestartFlightFromBelow


	thumb_func_start TradeSequenceDepart_FlightTask_Rise
TradeSequenceDepart_FlightTask_Rise: ; 0x022492FC
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl TradeSequenceDepart_UpdateFlight
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #0
	beq _0224933C
	ldr r1, [r4, #0x28]
	ldr r0, [r4, #0x2c]
	add r0, r1, r0
	str r0, [r4, #0x28]
	str r0, [r4, #0x18]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	sub r1, r0, #1
	add r0, r4, #0
	add r0, #0x80
	str r1, [r0]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #0
	bne _0224933C
	ldr r1, _02249354 ; =TradeSequenceDepart_FlightTask_Hover
	add r0, r5, #0
	bl SysTask_SetFunc
_0224933C:
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	bl TradeSequence_SetModelPosition
	ldr r0, [r4]
	add r4, #0x20
	add r1, r4, #0
	bl TradeSequence_SetModelRotation
	pop {r3, r4, r5, pc}
	nop
_02249354: .word TradeSequenceDepart_FlightTask_Hover
	thumb_func_end TradeSequenceDepart_FlightTask_Rise


	thumb_func_start TradeSequenceDepart_FlightTask_Hover
TradeSequenceDepart_FlightTask_Hover: ; 0x02249358
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl TradeSequenceDepart_UpdateFlight
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x80
	str r1, [r0]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #0x6e
	ble _0224939A
	ldr r1, [r4, #0x28]
	ldr r0, _022493B0 ; =0xFFFC5800
	sub r0, r0, r1
	mov r1, #0x35
	bl _s32_div_f
	str r0, [r4, #0x2c]
	add r0, r4, #0
	mov r1, #0x35
	add r0, #0x80
	str r1, [r0]
	ldr r1, _022493B4 ; =TradeSequenceDepart_FlightTask_Sink
	add r0, r5, #0
	bl SysTask_SetFunc
_0224939A:
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	bl TradeSequence_SetModelPosition
	ldr r0, [r4]
	add r4, #0x20
	add r1, r4, #0
	bl TradeSequence_SetModelRotation
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022493B0: .word 0xFFFC5800
_022493B4: .word TradeSequenceDepart_FlightTask_Sink
	thumb_func_end TradeSequenceDepart_FlightTask_Hover


	thumb_func_start TradeSequenceDepart_FlightTask_Sink
TradeSequenceDepart_FlightTask_Sink: ; 0x022493B8
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl TradeSequenceDepart_UpdateFlight
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #0
	beq _022493F8
	ldr r1, [r4, #0x28]
	ldr r0, [r4, #0x2c]
	add r0, r1, r0
	str r0, [r4, #0x28]
	str r0, [r4, #0x18]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	sub r1, r0, #1
	add r0, r4, #0
	add r0, #0x80
	str r1, [r0]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #0
	bne _022493F8
	ldr r1, _02249410 ; =TradeSequenceDepart_FlightTask_Pause
	add r0, r5, #0
	bl SysTask_SetFunc
_022493F8:
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	bl TradeSequence_SetModelPosition
	ldr r0, [r4]
	add r4, #0x20
	add r1, r4, #0
	bl TradeSequence_SetModelRotation
	pop {r3, r4, r5, pc}
	nop
_02249410: .word TradeSequenceDepart_FlightTask_Pause
	thumb_func_end TradeSequenceDepart_FlightTask_Sink


	thumb_func_start TradeSequenceDepart_FlightTask_Pause
TradeSequenceDepart_FlightTask_Pause: ; 0x02249414
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl TradeSequenceDepart_UpdateFlight
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x80
	str r1, [r0]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #1
	ble _02249446
	mov r0, #2
	lsl r0, r0, #0x10
	str r0, [r4, #0x2c]
	ldr r1, _0224945C ; =TradeSequenceDepart_FlightTask_Leave
	add r0, r5, #0
	bl SysTask_SetFunc
_02249446:
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	bl TradeSequence_SetModelPosition
	ldr r0, [r4]
	add r4, #0x20
	add r1, r4, #0
	bl TradeSequence_SetModelRotation
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0224945C: .word TradeSequenceDepart_FlightTask_Leave
	thumb_func_end TradeSequenceDepart_FlightTask_Pause


	thumb_func_start TradeSequenceDepart_FlightTask_Leave
TradeSequenceDepart_FlightTask_Leave: ; 0x02249460
	push {r4, lr}
	add r4, r1, #0
	ldr r0, [r4, #0x78]
	cmp r0, #0
	bne _0224949C
	add r0, r4, #0
	bl TradeSequenceDepart_UpdateFlight
	ldr r1, [r4, #0x28]
	ldr r0, [r4, #0x2c]
	add r0, r1, r0
	str r0, [r4, #0x28]
	str r0, [r4, #0x18]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	bl TradeSequence_SetModelPosition
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x20
	bl TradeSequence_SetModelRotation
	ldr r0, [r4]
	bl TradeSequence_IsModelInView
	cmp r0, #0
	bne _0224949C
	mov r0, #1
	str r0, [r4, #0x78]
_0224949C:
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end TradeSequenceDepart_FlightTask_Leave
