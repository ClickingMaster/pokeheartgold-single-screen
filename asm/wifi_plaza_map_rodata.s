	.extern WiFiPlazaObj_HandlerIdle
	.extern WiFiPlazaObj_HandlerPlayerInput
	.extern WiFiPlazaObj_HandlerWander
	.extern WiFiPlazaObj_HandlerRiseOut
	.extern WiFiPlazaObj_HandlerDropIn
	.extern WiFiPlazaObj_HandlerSlideLeft
	.extern WiFiPlazaObj_HandlerSlideRight
	.extern WiFiPlazaObj_HandlerSlideUp
	.extern WiFiPlazaObj_HandlerSlideDown
	.extern WiFiPlazaObj_HandlerHop
	.extern WiFiPlazaObj_HandlerResetHeight
	.extern WiFiPlazaModels_GetMoverFrontPos
	.extern WiFiPlazaPlayerState_PlayInstrument
	.extern WiFiPlazaPlayerState_OwnArrival
	.extern WiFiPlazaPlayerState_OwnReturnFromAttraction
	.extern WiFiPlazaPlayerState_OwnReturnFromMinigame
	.extern WiFiPlazaPlayerState_OwnReturnFromSurvey
	.extern WiFiPlazaPlayerState_OwnWalk
	.extern WiFiPlazaPlayerState_OwnWalkLate
	.extern WiFiPlazaPlayerState_OwnExit
	.extern WiFiPlazaPlayerState_OwnEnterAttraction
	.extern WiFiPlazaPlayerState_OwnEnterMinigame
	.extern WiFiPlazaPlayerState_OwnUseSurveyMonitor
	.extern WiFiPlazaTalk_Greeter
	.extern WiFiPlazaPlayerState_OwnRideMover
	.extern WiFiPlazaPlayerState_OtherWaitForPlayer
	.extern WiFiPlazaPlayerState_OtherPresent
	.extern WiFiPlazaPlayerState_OtherLeave
	.extern WiFiPlazaPlayerState_OtherRideMover
	.extern WiFiPlazaPlayerState_OtherDoActivity
	.extern WiFiPlazaGameEntry_Register
	.extern WiFiPlazaGameEntry_EndLink
	.extern WiFiPlazaGameEntry_ReadSign
	.extern WiFiPlazaTalk_Start
	.extern WiFiPlazaTalk_Answer
	.extern WiFiPlazaTalk_InfoCounter
	.extern WiFiPlazaTalk_Reception
	.extern WiFiPlazaInstrument_InitRipple01
	.extern WiFiPlazaInstrument_InitRipple02
	.extern WiFiPlazaInstrument_InitRipple03
	.extern WiFiPlazaInstrument_InitWhirl01
	.extern WiFiPlazaInstrument_InitWhirl02
	.extern WiFiPlazaInstrument_InitWhirl03
	.extern WiFiPlazaInstrument_InitSignal
	.extern WiFiPlazaInstrument_InitPop01
	.extern WiFiPlazaInstrument_InitPop02
	.extern WiFiPlazaInstrument_InitPop03
	.extern WiFiPlazaInstrument_InitBalloon01
	.extern WiFiPlazaInstrument_InitBalloon02
	.extern WiFiPlazaInstrument_InitBalloon03
	.extern WiFiPlazaInstrument_InitSparkle01
	.extern WiFiPlazaInstrument_InitSparkle02
	.extern WiFiPlazaInstrument_InitSparkle03
	.extern WiFiPlazaInstrument_InitBell01
	.extern WiFiPlazaInstrument_InitBell02
	.extern WiFiPlazaInstrument_InitBell03
	.extern WiFiPlazaInstrument_InitDrum01
	.extern WiFiPlazaInstrument_InitDrum02
	.extern WiFiPlazaInstrument_InitDrum03
	.extern WiFiPlazaInstrument_InitCymbal01
	.extern WiFiPlazaInstrument_InitCymbal02
	.extern WiFiPlazaInstrument_InitCymbal03
	.extern WiFiPlazaInstrument_UpdateRipple
	.extern WiFiPlazaInstrument_UpdateSignal01
	.extern WiFiPlazaInstrument_UpdateSignal02
	.extern WiFiPlazaInstrument_UpdateSignal03
	.extern WiFiPlazaInstrument_UpdateWhirl
	.extern WiFiPlazaInstrument_UpdatePop
	.extern WiFiPlazaInstrument_UpdateBalloon
	.extern WiFiPlazaInstrument_UpdateSparkle
	.extern WiFiPlazaInstrument_UpdateBellDrum
	.extern WiFiPlazaInstrument_UpdateCymbal
	.extern WiFiPlazaInstrumentBurst_StartBell
	.extern WiFiPlazaInstrumentBurst_UpdateBell
	.extern WiFiPlazaInstrumentBurst_StartDrum
	.extern WiFiPlazaInstrumentBurst_UpdateDrum
	.extern WiFiPlazaInstrumentBurst_StartCymbal
	.extern WiFiPlazaInstrumentBurst_UpdateCymbal
	.extern WiFiPlazaEventEffect_UpdateNone
	.extern WiFiPlazaEventEffect_UpdatePhase2
	.extern WiFiPlazaEventEffect_UpdatePhase3
	.extern WiFiPlazaEventEffect_UpdatePhase4
	.extern WiFiPlazaEventEffect_UpdatePhase5
	.extern WiFiPlazaEventEffect_StartHideProps
	.extern WiFiPlazaEventEffect_StartPhase2
	.extern WiFiPlazaEventEffect_StartPhase3
	.extern WiFiPlazaEventEffect_StartPhase4
	.extern WiFiPlazaEventEffect_StartPhase5
	.extern WiFiPlazaTalk_Survey
	.extern WiFiPlazaEventWobble_InitSmall
	.extern WiFiPlazaEventWobble_InitLarge
	.extern WiFiPlazaEventWobble_Step
	.include "asm/macros.inc"
	.include "overlay_49.inc"
	.include "global.inc"
	.public _02258954
	.public _022589B4
	.public _022589D4
	.public _022589EA
	.public _022589F2
	.public _02258A18
	.public _02258A1A
	.public _02258A34
	.public _02258A3E
	.public _02258A4C
	.public _02258A54
	.public _02258A5E
	.public _02258A6C
	.public _02258A74
	.public _02258A7E
	.public _02258A8C
	.public _02258A94
	.public _02258A9E
	.public _02258AAC
	.public _02258B6C
	.public _02258B82
	.public _02258B8C
	.public _02258BA4
	.public _02258BBC
	.public _02258BC8
	.public _02258BDC
	.public _02258BE8
	.public _02258C00
	.public _02258C04
	.public _02258C14
	.public _02258C18
	.public _02258C24
	.public _02258C4A
	.public _02258C8C
	.public _02258C90
	.public _02258CCC
	.public _02258CE6
	.public _02258CF4
	.public _02258D06
	.public _02258D80
	.public _02258D9C
	.public _02258DA6
	.public _02258DC4
	.public _02258E44
	.public _02258E70
	.public _02258E96
	.public _02258EFC
	.public _02258F0E
	.public _02258F1C
	.public _02258F30
	.public _02258F34
	.public _02258F60
	.public _02258F78
	.public _02258FAA
	.public _02258FB4
	.public _02258FC6
	.public _02258FD0
	.public _0225902C
	.public _02259046
	.public _0225907E
	.public _022590E8
	.public _02259118
	.public _0225912C
	.public _02259138
	.public _02259144
	.public _02259150
	.public _0225915C
	.public _02259168
	.public _0225917C
	.public _02259194
	.public _022591AC
	.public _022591BC
	.public _022591C8
	.public _022591D4
	.public _022591E4
	.public _022591FE
	.public _02259248
	.public _02259266
	.public _02259278
	.public _02259318
	.public _02259348
	.public _02259356
	.public _0225938E
	.public _0225939C
	.public _022593B8
	.public _022593CE
	.public _022593E4
	.public _022593EE
	.public _02259406
	.public _02259444
	.public _0225945A
	.public _02259468
	.public _0225947E
	.public _0225948C
	.public _022594A2
	.public _022594B0
	.public _022594C6
	.public _022594D2
	.public _022594D4
	.public _022594F0
	.public _022594FA
	.public _0225950C
	.public _02259522
	.public _0225956A
	.public _0225957A
	.public _0225958E
	.public _022595B0
	.public _022595C0
	.public _022595C4
	.public _022595C8
	.public _022595E4
	.public _022595EC
	.public _02259616
	.public _02259632
	.public _02259672
	.public _02259678
	.public _02259694
	.public _0225969C
	.public _022596CC
	.public _0225970A
	.public _0225972A
	.public _02259730
	.public _0225973C
	.public _02259748
	.public _02259754
	.public _02259760
	.public _02259774
	.public _02259780
	.public _02259788
	.public _022597C8
	.public _022597D0
	.public _022597D6
	.public _022597DA
	.public _022597E4
	.public _0225980E
	.public _02259816
	.public _02259826
	.public _02259836
	.public _02259846
	.public _02259854
	.public _0225987A
	.public _02259882
	.public _02259888
	.public _0225988E
	.public _02259894
	.public _0225989E
	.public _02259916
	.public _02259930
	.public _02259950
	.public _0225995C
	.public _022599DE
	.public _022599F4
	.public _02259A38
	.public _02259A64
	.public _02259A7A
	.public _02259B02
	.public _02259C2E
	.public _02259C8C
	.public _02259CB6
	.public _02259CC8
	.public _02259CEA
	.public _02259CFE
	.public _02259D1A
	.public _02259D26
	.public _02259D42
	.public _02259D58
	.public _02259D5A
	.public _02259D68
	.public _02259D74
	.public _02259D7A
	.public _02259DBE
	.public _02259DDC
	.public _02259DF2
	.public _02259DF8
	.public _02259E08
	.public _02259E58
	.public _02259E76
	.public _02259EA8
	.public _02259ED0
	.public _02259EE2
	.public _02259EF4
	.public _0225A05C
	.public _0225A07C
	.public _0225A098
	.public _0225A0A8
	.public _0225A0B8
	.public _0225A0C8
	.public _0225A0D8
	.public _0225A0E8
	.public _0225A0F8
	.public _0225A108
	.public _0225A140
	.public _0225A150
	.public _0225A160
	.public _0225A170
	.public _0225A1E0
	.public _0225A1F0
	.public _0225A200
	.public _0225A238
	.public _0225A2E6
	.public _0225A2EA
	.public _0225A2EE
	.public _0225A2F4
	.public _0225A308
	.public _0225A318
	.public _0225A356
	.public _0225A35C
	.public _0225A388
	.public _0225A398
	.public _0225A3A8
	.public _0225A3B8
	.public _0225A3C8
	.public _0225A3D8
	.public _0225A3E8
	.public _0225A3F8
	.public _0225A408
	.public _0225A468
	.public _0225A476
	.public _0225A4CC
	.public _0225A4DC
	.public _0225A4EC
	.public _0225A4FC
	.public _0225A50C
	.public _0225A51C
	.public _0225A52C
	.public _0225A538
	.public _0225A548
	.public _0225A558
	.public _0225A57A
	.public _0225A5A8
	.public _0225A5BA
	.public _0225A636
	.public _0225A742
	.public _0225A798
	.public _0225A79C
	.public _0225A7A0
	.public _0225A7A4
	.public _0225A7A8
	.public _0225A7AC
	.public _0225A7B0
	.public _0225A7B4
	.public _0225A7B8
	.public _0225A7BC
	.public _0225A7DC
	.public _0225A80E
	.public _0225A83C
	.public _0225A964
	.public _0225A968
	.public _0225A96C
	.public _0225A970
	.public _0225A974
	.public _0225A978
	.public _0225A97C
	.public _0225A980
	.public _0225A984
	.public _0225A988
	.public _0225A9B2
	.public _0225A9E8
	.public _0225AA0A
	.public _0225AA0E
	.public _0225AA10
	.public _0225AA1E
	.public _0225AA80
	.public _0225AA9A
	.public _0225AA9E
	.public _0225AAA0
	.public _0225AB30
	.public _0225AB64
	.public _0225ABA0
	.public _0225ABC4
	.public _0225AC04
	.public _0225AC16
	.public _0225AC36
	.public _0225AC4A
	.public _0225AC56
	.public _0225AC70
	.public _0225AC90
	.public _0225ACC0
	.public _0225ACE4
	.public _0225AE0E
	.public _0225AE44
	.public _0225AE48
	.public _0225AE5C
	.public _0225AE6C
	.public _0225AE70
	.public _0225AE94
	.public _0225AEBC
	.public _0225AED0
	.public _0225AEDE
	.public _0225AEF4
	.public _0225AF00
	.public _0225AF14
	.public _0225AF20
	.public _0225AF2A
	.public _0225AF46
	.public _0225AF52
	.public _0225AFD4
	.public _0225AFE8
	.public _0225AFFC
	.public _0225B004
	.public _0225B00A
	.public _0225B010
	.public _0225B046
	.public _0225B056
	.public _0225B064
	.public _0225B068
	.public _0225B090
	.public _0225B0AC
	.public _0225B0B2
	.public _0225B0CA
	.public _0225B0DC
	.public _0225B144
	.public _0225B174
	.public _0225B1AA
	.public _0225B1FC
	.public _0225B212
	.public _0225B248
	.public _0225B280
	.public _0225B322
	.public _0225B358
	.public _0225B36A
	.public _0225B3D4
	.public _0225B3E4
	.public _0225B3F4
	.public _0225B404
	.public _0225B414
	.public _0225B434
	.public _0225B440
	.public _0225B4DC
	.public _0225B544
	.public _0225B55A
	.public _0225B568
	.public _0225B586
	.public _0225B5D4
	.public _0225B5F2
	.public _0225B5FA
	.public _0225B5FC
	.public _0225B606
	.public _0225B61C
	.public _0225B626
	.public _0225B642
	.public _0225B66E
	.public _0225B6C2
	.public _0225B6D2
	.public _0225B6E2
	.public _0225B700
	.public _0225B708
	.public _0225B70A
	.public _0225B71C
	.public _0225B726
	.public _0225B730
	.public _0225B73E
	.public _0225B748
	.public _0225B75A
	.public _0225B764
	.public _0225B77E
	.public _0225B7A4
	.public _0225B7B0
	.public _0225B7DC
	.public _0225B7E0
	.public _0225B810
	.public _0225B82C
	.public _0225B840
	.public _0225B84E
	.public _0225B876
	.public _0225B880
	.public _0225B88C
	.public _0225B890
	.public _0225B894
	.public _0225B8BA
	.public _0225B8C4
	.public _0225B8DE
	.public _0225B8EA
	.public _0225B8F6
	.public _0225B906
	.public _0225B910
	.public _0225B924
	.public _0225B930
	.public _0225B93E
	.public _0225B95A
	.public _0225B9C8
	.public _0225B9D0
	.public _0225BA04
	.public _0225BA3C
	.public _0225BA54
	.public _0225BA58
	.public _0225BAB8
	.public _0225BADC
	.public _0225BAEC
	.public _0225BB3E
	.public _0225BB4C
	.public _0225BB8E
	.public _0225BBA4
	.public _0225BBB6
	.public _0225BBBA
	.public _0225BC0E
	.public _0225BC1C
	.public _0225BC34
	.public _0225BC40
	.public _0225BD3E
	.public _0225BD60
	.public _0225BE06
	.public _0225BE3A
	.public _0225BE66
	.public _0225BE6A
	.public _0225BE70
	.public _0225BE98
	.public _0225BE9C
	.public _0225BEB8
	.public _0225BF60
	.public _0225BF78
	.public _0225BF7C
	.public _0225BF92
	.public _0225BF98
	.public _0225C006
	.public _0225C010
	.public _0225C052
	.public _0225C082
	.public _0225C16A
	.public _0225C196
	.public _0225C19A
	.public _0225C1AA
	.public _0225C1E4
	.public _0225C21A
	.public _0225C2D4
	.public _0225C2E2
	.public _0225C2FE
	.public _0225C30C
	.public _0225C324
	.public _0225C354
	.public _0225C378
	.public _0225C3BC
	.public _0225C3C8
	.public _0225C43C
	.public _0225C46C
	.public _0225C47C
	.public _0225C4B8
	.public _0225C506
	.public _0225C526
	.public _0225C546
	.public _0225C554
	.public _0225C558
	.public _0225C596
	.public _0225C5EC
	.public _0225C608
	.public _0225C60E
	.public _0225C632
	.public _0225C648
	.public _0225C64A
	.public _0225C67E
	.public _0225C708
	.public _0225C716
	.public _0225C73A
	.public _0225C748
	.public _0225C760
	.public _0225C770
	.public _0225C77C
	.public _0225C780
	.public _0225C784
	.public _0225C788
	.public _0225C796
	.public _0225C7CC
	.public _0225C7D8
	.public _0225C81C
	.public _0225C834
	.public _0225C83C
	.public _0225C840
	.public _0225C862
	.public _0225C89C
	.public _0225C8A0
	.public _0225C8A4
	.public _0225C8BE
	.public _0225C8F2
	.public _0225C8F6
	.public _0225C912
	.public _0225C932
	.public _0225C948
	.public _0225C954
	.public _0225C968
	.public _0225CA1A
	.public _0225CA2C
	.public _0225CA46
	.public _0225CA96
	.public _0225CB34
	.public _0225CB48
	.public _0225CB4C
	.public _0225CB5E
	.public _0225CB66
	.public _0225CB8C
	.public _0225CBD0
	.public _0225CBD4
	.public _0225CBD8
	.public _0225CC18
	.public _0225CCBC
	.public _0225CCD2
	.public _0225CD10
	.public _0225CD2C
	.public _0225CD3A
	.public _0225CD52
	.public _0225CD54
	.public _0225CD62
	.public _0225CD70
	.public _0225CD7E
	.public _0225CD9A
	.public _0225CDB4
	.public _0225CDC6
	.public _0225CDE2
	.public _0225CDE4
	.public _0225CE80
	.public _0225CE84
	.public _0225CECC
	.public _0225CEFA
	.public _0225CF26
	.public _0225CF3C
	.public _0225CF44
	.public _0225D048
	.public _0225D060
	.public _0225D078
	.public _0225D084
	.public _0225D0D8
	.public _0225D0F2
	.public _0225D15C
	.public _0225D180
	.public _0225D1A6
	.public _0225D1BC
	.public _0225D238
	.public _0225D240
	.public _0225D24A
	.public _0225D280
	.public _0225D2B2
	.public _0225D2C0
	.public _0225D2C8
	.public _0225D2DE
	.public _0225D2F8
	.public _0225D310
	.public _0225D324
	.public _0225D338
	.public _0225D342
	.public _0225D390
	.public _0225D39C
	.public _0225D3B2
	.public _0225D3CC
	.public _0225D3D6
	.public _0225D3F4
	.public _0225D40A
	.public _0225D414
	.public _0225D42E
	.public _0225D44E
	.public _0225D45E
	.public _0225D468
	.public _0225D47E
	.public _0225D488
	.public _0225D49C
	.public _0225D4B2
	.public _0225D50E
	.public _0225D524
	.public _0225D550
	.public _0225D552
	.public _0225D55C
	.public _0225D55E
	.public _0225D578
	.public _0225D596
	.public _0225D5BA
	.public _0225D5DE
	.public _0225D5F2
	.public _0225D616
	.public _0225D644
	.public _0225D65E
	.public _0225D684
	.public _0225D69C
	.public _0225D6BA
	.public _0225D6D4
	.public _0225D6E0
	.public _0225D704
	.public _0225D734
	.public _0225D754
	.public _0225D75C
	.public _0225D77E
	.public _0225D79E
	.public _0225D7A6
	.public _0225D7CC
	.public _0225D7EE
	.public _0225D810
	.public _0225D81E
	.public _0225D834
	.public _0225D842
	.public _0225D84A
	.public _0225D878
	.public _0225D87E
	.public _0225D8B2
	.public _0225D8E6
	.public _0225D8E8
	.public _0225D8F2
	.public _0225D8F4
	.public _0225D924
	.public _0225D934
	.public _0225D96A
	.public _0225D97C
	.public _0225D988
	.public _0225D9E0
	.public _0225D9E4
	.public _0225DA04
	.public _0225DA18
	.public _0225DA1C
	.public _0225DA68
	.public _0225DA6C
	.public _0225DA88
	.public _0225DA9E
	.public _0225DAA6
	.public _0225DAC0
	.public _0225DAE8
	.public _0225DAEC
	.public _0225DB26
	.public _0225DB30
	.public _0225DB6A
	.public _0225DB76
	.public _0225DBA0
	.public _0225DBC2
	.public _0225DBDA
	.public _0225DBF2
	.public _0225DC0A
	.public _0225DC18
	.public _0225DC20
	.public _0225DC28
	.public _0225DC44
	.public _0225DC62
	.public _0225DC68
	.public _0225DC86
	.public _0225DC8E
	.public _0225DCD0
	.public _0225DCD4
	.public _0225DCE4
	.public _0225DD44
	.public _0225DD64
	.public _0225DD82
	.public _0225DDA2
	.public _0225DDB0
	.public _0225DDCC
	.public _0225DDEE
	.public _0225DDF0
	.public _0225DE12
	.public _0225DE2E
	.public _0225DE72
	.public _0225DE8C
	.public _0225DEBA
	.public _0225DEC8
	.public _0225DEE2
	.public _0225DEFC
	.public _0225DF14
	.public _0225DF66
	.public _0225DF80
	.public _0225DF9C
	.public _0225DFBA
	.public _0225DFC6
	.public _0225DFE8
	.public _0225E014
	.public _0225E052
	.public _0225E07E
	.public _0225E094
	.public _0225E0C2
	.public _0225E0E2
	.public _0225E0F2
	.public _0225E122
	.public _0225E150
	.public _0225E17A
	.public _0225E18C
	.public _0225E1BA
	.public _0225E20E
	.public _0225E234
	.public _0225E242
	.public _0225E252
	.public _0225E284
	.public _0225E288
	.public _0225E28C
	.public _0225E290
	.public _0225E294
	.public _0225E298
	.public _0225E29C
	.public _0225E2A0
	.public _0225E2A4
	.public _0225E2A8
	.public _0225E2AC
	.public _0225E2B0
	.public _0225E2C6
	.public _0225E2DA
	.public _0225E2E8
	.public _0225E2FA
	.public _0225E310
	.public _0225E314
	.public _0225E32E
	.public _0225E340
	.public _0225E352
	.public _0225E364
	.public _0225E374
	.public _0225E386
	.public _0225E390
	.public _0225E394
	.public _0225E398
	.public _0225E39C
	.public _0225E3A8
	.public _0225E3B4
	.public _0225E3CE
	.public _0225E3F0
	.public _0225E408
	.public _0225E41C
	.public _0225E436
	.public _0225E43E
	.public _0225E470
	.public _0225E474
	.public _0225E478
	.public _0225E48E
	.public _0225E4A0
	.public _0225E4B6
	.public _0225E4C8
	.public _0225E4E0
	.public _0225E4F4
	.public _0225E50C
	.public _0225E520
	.public _0225E536
	.public _0225E548
	.public _0225E55E
	.public _0225E570
	.public _0225E57C
	.public _0225E588
	.public _0225E5B0
	.public _0225E5D0
	.public _0225E5E6
	.public _0225E5F0
	.public _0225E602
	.public _0225E60A
	.public _0225E616
	.public _0225E61C
	.public _0225E620
	.public _0225E636
	.public _0225E64C
	.public _0225E656
	.public _0225E65C
	.public _0225E66C
	.public _0225E6A8
	.public _0225E6CE
	.public _0225E6DA
	.public _0225E6DC
	.public _0225E6F4
	.public _0225E708
	.public _0225E70C
	.public _0225E710
	.public _0225E726
	.public _0225E73C
	.public _0225E746
	.public _0225E750
	.public _0225E75A
	.public _0225E75C
	.public _0225E772
	.public _0225E780
	.public _0225E790
	.public _0225E798
	.public _0225E7AE
	.public _0225E7C8
	.public _0225E7EC
	.public _0225E80A
	.public _0225E816
	.public _0225E818
	.public _0225E81C
	.public _0225E820
	.public _0225E83E
	.public _0225E854
	.public _0225E858
	.public _0225E882
	.public _0225E888
	.public _0225E88C
	.public _0225E890
	.public _0225E8AA
	.public _0225E8BE
	.public _0225E8C0
	.public _0225E8E2
	.public _0225E8FA
	.public _0225E918
	.public _0225E926
	.public _0225E938
	.public _0225E964
	.public _0225E972
	.public _0225E984
	.public _0225E992
	.public _0225E9B4
	.public _0225E9CC
	.public _0225EA08
	.public _0225EA2A
	.public _0225EA3C
	.public _0225EA5A
	.public _0225EA6C
	.public _0225EA88
	.public _0225EA9A
	.public _0225EAAC
	.public _0225EAB0
	.public _0225EACA
	.public _0225EADE
	.public _0225EAEA
	.public _0225EB20
	.public _0225EB2A
	.public _0225EB34
	.public _0225EB3E
	.public _0225EB52
	.public _0225EB78
	.public _0225EB7E
	.public _0225EB80
	.public _0225EB9C
	.public _0225EBA0
	.public _0225EBA4
	.public _0225ECCC
	.public _0225ECD0
	.public _0225ECE8
	.public _0225ECEC
	.public _0225ED06
	.public _0225ED10
	.public _0225ED1A
	.public _0225ED2E
	.public _0225ED38
	.public _0225ED62
	.public _0225ED66
	.public _0225EDAE
	.public _0225EDB8
	.public _0225EDC2
	.public _0225EDD6
	.public _0225EDE0
	.public _0225EE0A
	.public _0225EE0E
	.public _0225EE60
	.public _0225EEA8
	.public _0225EED2
	.public _0225EF02
	.public _0225EF2C
	.public _0225EF38
	.public _0225EF50
	.public _0225EF76
	.public _0225EFAA
	.public _0225EFB2
	.public _0225EFD6
	.public _0225EFDE
	.public _0225F000
	.public _0225F008
	.public _0225F03E
	.public _0225F046
	.public _0225F05C
	.public _0225F082
	.public _0225F08C
	.public _0225F0B0
	.public _0225F0B6
	.public _0225F0BC
	.public _0225F0C0
	.public _0225F0D6
	.public _0225F0EE
	.public _0225F0F8
	.public _0225F126
	.public _0225F156
	.public _0225F17A
	.public _0225F18A
	.public _0225F1D8
	.public _0225F1EC
	.public _0225F206
	.public _0225F216
	.public _0225F220
	.public _0225F234
	.public _0225F23C
	.public _0225F240
	.public _0225F244
	.public _0225F248
	.public _0225F24A
	.public _0225F256
	.public _0225F25C
	.public _0225F288
	.public _0225F28A
	.public _0225F2AA
	.public _0225F2C2
	.public _0225F2D2
	.public _0225F2E4
	.public _0225F2EA
	.public _0225F2F4
	.public _0225F3A8
	.public _0225F3AE
	.public _0225F3BA
	.public _0225F3C0
	.public _0225F420
	.public _0225F422
	.public _0225F428
	.public _0225F42C
	.public _0225F442
	.public _0225F47E
	.public _0225F4A2
	.public _0225F4B8
	.public _0225F4CE
	.public _0225F4EE
	.public _0225F4F8
	.public _0225F504
	.public _0225F510
	.public _0225F556
	.public _0225F584
	.public _0225F5C4
	.public _0225F5FA
	.public _0225F602
	.public _0225F604
	.public _0225F614
	.public _0225F62E
	.public _0225F648
	.public _0225F662
	.public _0225F67C
	.public _0225F692
	.public _0225F6B2
	.public _0225F6E4
	.public _0225F6FE
	.public _0225F704
	.public _0225F70C
	.public _0225F718
	.public _0225F74E
	.public _0225F758
	.public _0225F766
	.public _0225F768
	.public _0225F78E
	.public _0225F79E
	.public _0225F7D0
	.public _0225F7EA
	.public _0225F7F0
	.public _0225F7F8
	.public _0225F804
	.public _0225F83A
	.public _0225F844
	.public _0225F852
	.public _0225F854
	.public _0225F87A
	.public _0225F88A
	.public _0225F8C2
	.public _0225F8CC
	.public _0225F8E2
	.public _0225F902
	.public _0225F950
	.public _0225F97A
	.public _0225F9AE
	.public _0225F9C2
	.public _0225F9FA
	.public _0225FA00
	.public _0225FA04
	.public _0225FA08
	.public _0225FA0C
	.public _0225FA10
	.public _0225FA48
	.public _0225FA54
	.public _0225FA60
	.public _0225FA7C
	.public _0225FA8C
	.public _0225FA9C
	.public _0225FAC2
	.public _0225FAE8
	.public _0225FAF2
	.public _0225FB16
	.public _0225FB2E
	.public _0225FB52
	.public _0225FB58
	.public _0225FB98
	.public _0225FBA4
	.public _0225FBB0
	.public _0225FBE2
	.public _0225FC08
	.public _0225FC30
	.public _0225FC3A
	.public _0225FC5E
	.public _0225FC76
	.public _0225FC9A
	.public _0225FCA0
	.public _0225FCA4
	.public _0225FCE6
	.public _0225FCF2
	.public _0225FD16
	.public _0225FD3C
	.public _0225FD62
	.public _0225FD6C
	.public _0225FD86
	.public _0225FD9E
	.public _0225FDC2
	.public _0225FDC8
	.public _0225FDEC
	.public _0225FDFC
	.public _0225FE6A
	.public _0225FE7E
	.public _0225FF48
	.public _0225FF60
	.public _0225FF62
	.public _0225FF8C
	.public _0225FF94
	.public _0225FFDA
	.public _02260008
	.public _02260036
	.public _022600A6
	.public _022600E6
	.public _02260120
	.public _0226016A
	.public _022601C0
	.public _022601EE
	.public _02260204
	.public _0226020A
	.public _02260210
	.public _02260214
	.public _02260218
	.public _0226021C
	.public _02260220
	.public _02260224
	.public _02260228
	.public _0226022C
	.public _02260250
	.public _02260270
	.public _02260284
	.public _022602C6
	.public _022602D8
	.public _022602F2
	.public _022602FC
	.public _022602FE
	.public _02260306
	.public _02260314
	.public _02260328
	.public _02260334
	.public _0226033E
	.public _02260350
	.public _02260384
	.public _022603B6
	.public _022603D6
	.public _0226041A
	.public _02260420
	.public _02260424
	.public _02260466
	.public _02260472
	.public _02260484
	.public _022604A8
	.public _022604B0
	.public _022604B6
	.public _022604BC
	.public _022604C2
	.public _022604C8
	.public _022604D0
	.public _022604EC
	.public _02260508
	.public _0226052E
	.public _0226053C
	.public _02260544
	.public _0226057E
	.public _02260596
	.public _0226059C
	.public _022605A4
	.public _022605AA
	.public _022605CA
	.public _022605D0
	.public _022605E2
	.public _02260624
	.public _02260632
	.public _02260638
	.public _0226063E
	.public _02260644
	.public _02260648
	.public _022606B0
	.public _022606CC
	.public _022606D4
	.public _022606DA
	.public _022606E0
	.public _022606E6
	.public _022606EC
	.public _022606F0
	.public _02260702
	.public _0226071A
	.public _02260740
	.public _0226075A
	.public _02260788
	.public _022607B8
	.public _022607C0
	.public _022607FE
	.public _0226080A
	.public _0226081A
	.public _0226084C
	.public _02260858
	.public _02260864
	.public _02260870
	.public _02260874
	.public _0226088E
	.public _0226089E
	.public _022608A8
	.public _022608BC
	.public _022608C4
	.public _022608CC
	.public _022608D2
	.public _0226095E
	.public _0226097E
	.public _02260992
	.public _022609A8
	.public _022609B8
	.public _022609C6
	.public _022609CA
	.public _022609D8
	.public _022609DC
	.public _022609E0
	.public _022609E4
	.public _022609E8
	.public _022609EC
	.public _02260A0C
	.public _02260A16
	.public _02260A2E
	.public _02260A5A
	.public _02260A60
	.public _02260A64
	.public _02260AA0
	.public _02260AAC
	.public _02260ABA
	.public _02260AE8
	.public _02260AF8
	.public _02260B10
	.public _02260B14
	.public _02260B1E
	.public _02260B2C
	.public _02260B36
	.public _02260BB4
	.public _02260BD4
	.public _02260BE8
	.public _02260C04
	.public _02260C1C
	.public _02260C4A
	.public _02260C4E
	.public _02260C54
	.public _02260C6A
	.public _02260CA6
	.public _02260CAA
	.public _02260CAE
	.public _02260CB2
	.public _02260CB6
	.public _02260CD2
	.public _02260D0E
	.public _02260D12
	.public _02260D16
	.public _02260D1A
	.public _02260D1E
	.public _02260D3A
	.public _02260D76
	.public _02260D7A
	.public _02260D7E
	.public _02260D82
	.public _02260D86
	.public _02260DA4
	.public _02260DC2
	.public _02260DDA
	.public _02260DE6
	.public _02260DFA
	.public _02260E24
	.public _02260E28
	.public _02260E6A
	.public _02260E76
	.public _02260E96
	.public _02260EBE
	.public _02260EC4
	.public _02260ECA
	.public _02260ED0
	.public _02260ED4
	.public _02260EDE
	.public _02260EF6
	.public _02260F0E
	.public _02260F44
	.public _02260F52
	.public _02260F62
	.public _02260F82
	.public _02260F90
	.public _02260FE2
	.public _02261016
	.public _02261046
	.public _02261082
	.public _02261098
	.public _022610A4
	.public _022610A6
	.public _022610B0
	.public _022610D0
	.public _022610F0
	.public _02261110
	.public _02261124
	.public _02261134
	.public _02261180
	.public _02261196
	.public _022611C0
	.public _022611C8
	.public _022611CC
	.public _022611D0
	.public _0226122E
	.public _02261250
	.public _02261256
	.public _022612AE
	.public _022612BC
	.public _0226130A
	.public _02261318
	.public _02261342
	.public _02261346
	.public _022613A4
	.public _022613A8
	.public _022613C4
	.public _022613CA
	.public _0226142C
	.public _02261430
	.public _02261494
	.public _022614A8
	.public _022614E0
	.public _0226153C
	.public _02261548
	.public _02261594
	.public _022615A0
	.public _022615B0
	.public _022615CA
	.public _022615D4
	.public _022615EA
	.public _02261606
	.public _02261608
	.public _02261618
	.public _0226162E
	.public _02261638
	.public _02261696
	.public _022616CA
	.public _022616DC
	.public _02261710
	.public _02261718
	.public _0226171C
	.public _0226176A
	.public _0226177C
	.public _022617BE
	.public _0226181E
	.public _0226187C
	.public _022618AA
	.public _022618B0
	.public _022618B4
	.public _022618B8
	.public _022618BC
	.public _022618EC
	.public _022618FE
	.public _02261910
	.public _02261926
	.public _0226192C
	.public _02261970
	.public _0226197A
	.public _022619A4
	.public _022619C4
	.public _022619CE
	.public _022619D0
	.public _02261A2A
	.public _02261A3C
	.public _02261A6E
	.public _02261A7C
	.public _02261AD8
	.public _02261AFA
	.public _02261B0E
	.public _02261B48
	.public _02261B64
	.public _02261B6C
	.public _02261B70
	.public _02261BBA
	.public _02261BD0
	.public _02261BDE
	.public _02261BE8
	.public _02261BFA
	.public _02261C0C
	.public _02261C3C
	.public _02261C4E
	.public _02261C58
	.public _02261C5A
	.public _02261C84
	.public _02261CC4
	.public _02261CD6
	.public _02261CEC
	.public _02261D08
	.public _02261D2E
	.public _02261D40
	.public _02261D8E
	.public _02261DB6
	.public _02261DF8
	.public _02261E04
	.public _02261E22
	.public _02261E38
	.public _02261E3E
	.public _02261E58
	.public _02261E72
	.public _02261E90
	.public _02261EAE
	.public _02261ECC
	.public _02261EEA
	.public _02261EF0
	.public _02261F00
	.public _02261F10
	.public _02261F22
	.public _02261F36
	.public _02261F42
	.public _02261F48
	.public _02261FA4
	.public _02261FA8
	.public _02261FAC
	.public _02261FB0
	.public _02261FB4
	.public _02261FB8
	.public _02261FBC
	.public _02261FD4
	.public _02262014
	.public _0226201C
	.public _02262024
	.public _0226205E
	.public _02262096
	.public _022620B0
	.public _022620C8
	.public _022620DE
	.public _022620FE
	.public _02262114
	.public _02262128
	.public _02262134
	.public _02262140
	.public _0226214E
	.public _0226215A
	.public _02262194
	.public _022621A6
	.public _022621C0
	.public _022621DA
	.public _022621EA
	.public _02262208
	.public _0226222E
	.public _02262248
	.public _0226224E
	.public _0226225E
	.public _0226226A
	.public _02262274
	.public _0226227C
	.public _02262292
	.public _022622B2
	.public _022622CA
	.public _022622E8
	.public _022622F6
	.public _0226230E
	.public _02262330
	.public _0226233C
	.public _02262372
	.public _0226239C
	.public _022623F0
	.public _02262410
	.public _02262420
	.public _02262422
	.public _02262462
	.public _02262494
	.public _022624A8
	.public _022624D2
	.public _022624F4
	.public _022624F8
	.public _022624FC
	.public _02262500
	.public _02262504
	.public _02262526
	.public _02262556
	.public _02262586
	.public _02262592
	.public _022625A4
	.public _022625B8
	.public _022625CC
	.public _022625CE
	.public _022625DC
	.public _02262630
	.public _0226265C
	.public _0226267A
	.public _02262686
	.public _022626AC
	.public _022626BE
	.public _022626D4
	.public _02262702
	.public _02262730
	.public _02262732
	.public _0226273C
	.public _02262774
	.public _022627A4
	.public _022627BA
	.public _022627BE
	.public _022627D2
	.public _022627D4
	.public _022627DC
	.public _022627F8
	.public _02262806
	.public _02262842
	.public _02262856
	.public _0226287C
	.public _022628A4
	.public _022628B4
	.public _022628C6
	.public _022628C8
	.public _022628DE
	.public _022628EE
	.public _02262904
	.public _0226290C
	.public _02262948
	.public _02262954
	.public _0226296C
	.public _0226297E
	.public _02262980
	.public _022629BE
	.public _02262A08
	.public _02262A1A
	.public _02262A2C
	.public _02262A40
	.public _02262A4C
	.public _02262A64
	.public _02262A7A
	.public _02262A8E
	.public _02262AB8
	.public _02262AC0
	.public _02262AD8
	.public _02262AF0
	.public _02262B02
	.public _02262B0E
	.public _02262B32
	.public _02262B50
	.public _02262B56
	.public _02262B5C
	.public _02262B62
	.public _02262B6C
	.public _02262BA8
	.public _02262BE2
	.public _02262BEC
	.public _02262BF4
	.public _02262C34
	.public _02262C54
	.public _02262C6E
	.public _02262C96
	.public _02262CA4
	.public _02262CB0
	.public _02262CEA
	.public _02262CFE
	.public _02262D1E
	.public _02262D4A
	.public _02262D58
	.public _02262D68
	.public _02262D6C
	.public _02262D82
	.public _02262DB4
	.public _02262DD0
	.public _02262DF6
	.public _02262E22
	.public _02262E32
	.public _02262E36
	.public _02262E3A
	.public _02262E3E
	.public _02262E44
	.public _02262E5A
	.public _02262E70
	.public _02262E7E
	.public _02262E80
	.public _02262E8E
	.public _02262EC8
	.public _02262F18
	.public _02262F68
	.public _02262FAC
	.public _0226300C
	.public _0226302A
	.public _0226302E
	.public _02263056
	.public _02263064
	.public _02263070
	.public _022630BE
	.public _0226310A
	.public _02263178
	.public _02263184
	.public _0226319C
	.public _022631A8
	.public _022631BA
	.public _022631C4
	.public _022631D8
	.public _022631E4
	.public _022631F0
	.public _02263226
	.public _02263256
	.public _02263268
	.public _02263272
	.public _0226328E
	.public _0226329A
	.public _022632AA
	.public _022632B6
	.public _022632C8
	.public _022632EA
	.public _022632F6
	.public _02263326
	.public _0226335E
	.public _0226338C
	.public _022633BA
	.public _022633EA
	.public _022633FE
	.public _0226340A
	.public _02263428
	.public _0226343A
	.public _02263444
	.public _0226344A
	.public _02263466
	.public _022634A0
	.public _022634A4
	.public _022634A8
	.public _022634AC
	.public _022634D4
	.public _02263524
	.public _02263554
	.public _02263564
	.public _02263594
	.public _022635A0
	.public _022635AA
	.public _022635B0
	.public _022635CC
	.public _02263618
	.public _02263628
	.public _0226363A
	.public _02263644
	.public _0226365C
	.public _02263664
	.public _0226366C
	.public _0226367C
	.public _02263686
	.public _02263696
	.public _022636A8
	.public _022636E8
	.public _022636FE
	.public _02263708
	.public _0226372A
	.public _0226375C
	.public _0226378A
	.public _022637A2
	.public _022637AA
	.public _022637D8
	.public _022637E2
	.public _02263828
	.public _02263858
	.public _0226385C
	.public _02263860
	.public _02263864
	.public _02263868
	.public _0226386C
	.public _02263870
	.public _02263892
	.public _022638A2
	.public _022638AE
	.public _022638B6
	.public _022638C8
	.public _022638D2
	.public _022638EA
	.public _022638FA
	.public _02263904
	.public _02263914
	.public _02263928
	.public _02263932
	.public _0226396A
	.public _0226397C
	.public _022639AA
	.public _022639D8
	.public _02263A22
	.public _02263A28
	.public _02263A42
	.public _02263A74
	.public _02263A9C
	.public _02263AD0
	.public _02263ADA
	.public _02263AE6
	.public _02263AFE
	.public _02263B4C
	.public _02263B5E
	.public _02263BC2
	.public _02263BD8
	.public _02263C00
	.public _02263C16
	.public _02263C56
	.public _02263CB4
	.public _02263CC0
	.public _02263CCA
	.public _02263CCE
	.public _02263CF4
	.public _02263D28
	.public _02263D3A
	.public _02263D44
	.public _02263D66
	.public _02263D70
	.public _02263D80
	.public _02263D8A
	.public _02263D9A
	.public _02263DBA
	.public _02263DC4
	.public _02263DF2
	.public _02263E2A
	.public _02263E58
	.public _02263E86
	.public _02263E98
	.public _02263EA2
	.public _02263EB2
	.public _02263ECE
	.public _02263EEA
	.public _02263EFC
	.public _02263F0A
	.public _02263F12
	.public _02263F22
	.public _02263F2C
	.public _02263F3C
	.public _02263F4C
	.public _02263F88
	.public _02263FC8
	.public _02263FCC
	.public _02263FD0
	.public _0226400C
	.public _0226405A
	.public _0226409E
	.public _022640CC
	.public _022640DE
	.public _022640E8
	.public _02264102
	.public _02264112
	.public _0226411C
	.public _0226412C
	.public _02264136
	.public _02264140
	.public _0226417A
	.public _022641AA
	.public _022641DA
	.public _0226420C
	.public _02264240
	.public _02264256
	.public _02264260
	.public _02264282
	.public _0226429A
	.public _022642A4
	.public _022642AA
	.public _022642CE
	.public _02264308
	.public _0226431A
	.public _02264348
	.public _0226434C
	.public _02264350
	.public _02264354
	.public _02264358
	.public _0226435C
	.public _02264360
	.public _02264364
	.public _02264392
	.public _022643DC
	.public _022643E2
	.public _022643FC
	.public _0226442E
	.public _0226445C
	.public _02264466
	.public _02264472
	.public _0226448A
	.public _02264498
	.public _022644DA
	.public _0226450A
	.public _02264516
	.public _02264548
	.public _0226456A
	.public _0226457E
	.public _02264588
	.public _022645A8
	.public _022645C8
	.public _022645E8
	.public _02264608
	.public _02264632
	.public _0226464A
	.public _0226465A
	.public _02264664
	.public _02264670
	.public _0226467C
	.public _02264688
	.public _02264694
	.public _022646A0
	.public _022646AC
	.public _022646B8
	.public _022646BE
	.public _022646C8
	.public _022646E0
	.public _02264700
	.public _0226471E
	.public _02264736
	.public _0226473E
	.public _02264748
	.public _02264754
	.public _02264760
	.public _0226476C
	.public _02264772
	.public _0226477C
	.public _02264780
	.public _02264782
	.public _02264796
	.public _022647C0
	.public _022647EA
	.public _02264814
	.public _02264834
	.public _02264854
	.public _02264874
	.public _02264894
	.public _02264898
	.public _022648B0
	.public _022648B8
	.public _022648C2
	.public _022648CE
	.public _022648DA
	.public _022648E6
	.public _022648EC
	.public _022648F6
	.public _0226490E
	.public _0226492E
	.public _0226494E
	.public _0226496E
	.public _0226498E
	.public _022649AE
	.public _022649C2
	.public _022649EA
	.public _022649F0
	.public _02264A16
	.public _02264A1C
	.public _02264A1E
	.public _02264A54
	.public _02264A70
	.public _02264A86
	.public _02264A98
	.public _02264AB0
	.public _02264AC8
	.public _02264AE0
	.public _02264AF8
	.public _02264B10
	.public _02264B1E
	.public _02264B2C
	.public _02264B3A
	.public _02264B46
	.public _02264B56
	.public _02264B6A
	.public _02264B8A
	.public _02264B90
	.public _02264BB4
	.public _02264BE6
	.public _02264BEC
	.public _02264BF2
	.public _02264BF8
	.public _02264C20
	.public _02264C2E
	.public _02264C36
	.public _02264C44
	.public _02264C48
	.public _02264C4C
	.public _02264C7C
	.public _02264C88
	.public _02264C92
	.public _02264C9C
	.public _02264CA0
	.public _02264CA4
	.public _02264CBC
	.public _02264CE2
	.public _02264D0C
	.public _02264D10
	.public _02264D24
	.public _02264D48
	.public _02264D98
	.public _02264DBA
	.public _02264DCA
	.public _02264DD6
	.public _02264DE2
	.public _02264DEE
	.public _02264DFA
	.public _02264E06
	.public _02264E12
	.public _02264E48
	.public _02264E56
	.public _02264E6C
	.public _02264E70
	.public _02264E74
	.public _02264E76
	.public _02264E8C
	.public _02264EB6
	.public _02264EBC
	.public _02264F08
	.public _02264F0C
	.public _02264F56
	.public _02264F58
	.public _02264F5C
	.public _02264F76
	.public _02264FC0
	.public _02264FDC
	.public _02264FEA
	.public _02264FF8
	.public _02265006
	.public _0226502C
	.public _0226503C
	.public _02265040
	.public _02265070
	.public _02265092
	.public _022650A0
	.public _022650C4
	.public _022650D4
	.public _022650FA
	.public _02265108
	.public _0226510C
	.public _02265126
	.public _02265168
	.public _0226516C
	.public _02265186
	.public _022651E4
	.public _022651FE
	.public _0226525C
	.public _02265268
	.public _02265288
	.public _022652C4
	.public _022652CE
	.public _022652DA
	.public _02265354
	.public _02265358
	.public _02265386
	.public _022653AC
	.public _022653D0
	.public _022653EE
	.public _02265408
	.public _02265448
	.public _02265466
	.public _02265476
	.public _0226548C
	.public _0226549A
	.public _022654CC
	.public _022654EC
	.public _022654FC
	.public _02265512
	.public _02265520
	.public _02265552
	.public _02265572
	.public _02265582
	.public _02265598
	.public _022655A6
	.public _022655D8
	.public _02265624
	.public _0226565C
	.public _02265684
	.public _02265696
	.public _022656A2
	.public _022656E0
	.public _022656E2
	.public _022656EC
	.public _022656EE
	.public _0226570C
	.public _02265710
	.public _02265714
	.public _02265718
	.public _02265724
	.public _02265734
	.public _02265744
	.public _0226575C
	.public _0226576A
	.public _022657A8
	.public _022657AC
	.public _022657B0
	.public _022657C4
	.public _022657D8
	.public _0226582A
	.public _02265840
	.public _02265844
	.public _02265848
	.public _0226584C
	.public _02265850
	.public _02265854
	.public _02265866
	.public _0226587C
	.public _02265888
	.public _0226588C
	.public _022658A2
	.public _022658DC
	.public _022658E0
	.public _022658FE
	.public _02265918
	.public _0226591C
	.public _02265934
	.public _0226593E
	.public _02265954
	.public _02265962
	.public _02265978
	.public _0226597C
	.public _02265994
	.public _022659A6
	.public _022659C8
	.public _022659CC
	.public _022659EC
	.public _02265A40
	.public _02265A58
	.public _02265A72
	.public _02265AA6
	.public _02265AC8
	.public _02265AE8
	.public _02265AFE
	.public _02265B04
	.public _02265B08
	.public _02265B0C
	.public _02265B10
	.public _02265B78
	.public _02265B88
	.public _02265B8C
	.public _02265B90
	.public _02265BD0
	.public _02265BDC
	.public _02265BE0
	.public _02265BE4
	.public _02265C24
	.public _02265C34
	.public _02265C38
	.public _02265C3C
	.public _02265C60
	.public _02265C64
	.public _02265C70
	.public _02265C7C
	.public _02265C88
	.public _02265C94
	.public _02265CA0
	.public _02265CAC
	.public _02265CBE
	.public _02265D08
	.public _02265D0C
	.public _02265D80
	.public _02265D88
	.public _02265DA8
	.public _02265DCA
	.public _02265DF6
	.public _02265E20
	.public _02265E40
	.public _02265E44
	.public _02265E48
	.public _02265E4C
	.public _02265E50
	.public _02265ECA
	.public _02265ED6
	.public _02265EDE
	.public _02265F32
	.public _02265F88
	.public _02265FE8
	.public _02266046
	.public _02266058
	.public _0226605C
	.public _02266060
	.public _02266064
	.public _02266068
	.public _0226606C
	.public _02266070
	.public _02266074
	.public _02266078
	.public _0226607C
	.public _02266080
	.public _02266084
	.public _0226610E
	.public _0226611A
	.public _02266122
	.public _02266194
	.public _02266208
	.public _02266286
	.public _02266302
	.public _02266314
	.public _02266318
	.public _0226631C
	.public _02266320
	.public _02266324
	.public _02266328
	.public _0226632C
	.public _02266330
	.public _02266334
	.public _02266338
	.public _0226633C
	.public _02266340
	.public _02266344
	.public _02266348
	.public _0226634C
	.public _02266350
	.public _0226635C
	.public _02266368
	.public _02266374
	.public _02266380
	.public _0226638C
	.public _02266398
	.public _022663A8
	.public _022663DC
	.public _022663E0
	.public _022663E4
	.public _022663E8
	.public _022663F8
	.public _0226642C
	.public _02266430
	.public _02266434
	.public _02266438
	.public _02266448
	.public _0226647C
	.public _02266480
	.public _02266484
	.public _02266488
	.public _02266498
	.public _022664CC
	.public _022664D0
	.public _022664D4
	.public _022664D8
	.public _022664E8
	.public _0226651C
	.public _02266520
	.public _02266524
	.public _02266528
	.public _02266538
	.public _0226656C
	.public _02266570
	.public _02266574
	.public _02266578
	.public _02266588
	.public _022665C0
	.public _022665C4
	.public _022665C8
	.public _022665CC
	.public _022665DC
	.public _02266614
	.public _02266618
	.public _0226661C
	.public _02266620
	.public _02266630
	.public _02266668
	.public _0226666C
	.public _02266670
	.public _02266674
	.public _0226668C
	.public _022666A8
	.public _022666C4
	.public _022666CA
	.public _022666E2
	.public _02266716
	.public _02266724
	.public _02266738
	.public _02266740
	.public _02266744
	.public _02266750
	.public _0226675C
	.public _02266768
	.public _022667AA
	.public _022667BA
	.public _022667C6
	.public _022667D6
	.public _022667FA
	.public _02266806
	.public _02266812
	.public _02266818
	.public _0226681C
	.public _02266836
	.public _0226684E
	.public _022668A0
	.public _022668AA
	.public _022668B6
	.public _022668F0
	.public _0226691E
	.public _02266952
	.public _02266956
	.public _02266968
	.public _0226696C
	.public _02266970
	.public _02266974
	.public _0226698A
	.public _022669A0
	.public _022669AC
	.public _022669E0
	.public _02266A08
	.public _02266A18
	.public _02266A1C
	.public _02266A2C
	.public _02266A4E
	.public _02266A5C
	.public _02266A74
	.public _02266A7C
	.public _02266A80
	.public _02266A84
	.public _02266AA8
	.public _02266AAC
	.public _02266AD8
	.public _02266AE8
	.public _02266AEC
	.public _02266B04
	.public _02266B06
	.public _02266B1C
	.public _02266B44
	.public _02266B84
	.public _02266B86
	.public _02266B96
	.public _02266BB0
	.public _02266BE2
	.public _02266C2A
	.public _02266C4A
	.public _02266C58
	.public _02266C5C
	.public _02266C60
	.public _02266C64
	.public _02266C68
	.public _02266C9A
	.public _02266CA8
	.public _02266CEC
	.public _02266CF0
	.public _02266CF4
	.public _02266CF8
	.public _02266CFC
	.public _02266D00
	.public _02266D50
	.public _02266D54
	.public _02266D58
	.public _02266D5C
	.public _02266D70
	.public _02266D78
	.public _02266DD0
	.public _02266DEA
	.public _02266DFC
	.public _02266E16
	.public _02266E1E
	.public _02266E26
	.public _02266E2E
	.public _02266E5E
	.public _02266E64
	.public _02266E68
	.public _02266E6C
	.public _02266E70
	.public _02266E74
	.public _02266ECE
	.public _02266EE0
	.public _02266EE8
	.public _02266EEC
	.public _02266EF0
	.public _02266EF4
	.public _02266F08
	.public _02266F10
	.public _02266F44
	.public _02266F52
	.public _02266F9A
	.public _02266FBC
	.public _02266FD6
	.public _02266FF0
	.public _02267008
	.public _02267052
	.public _0226705C
	.public _02267060
	.public _02267064
	.public _02267068
	.public _0226706C
	.public _02267070
	.public _022670B0
	.public _022670B4
	.public _022670C8
	.public _022670D0
	.public _022670F0
	.public _022670FA
	.public _0226710A
	.public _0226710C
	.public _02267112
	.public _02267128
	.public _0226715E
	.public _022671B6
	.public _02267228
	.public _0226722E
	.public _02267230
	.public _02267262
	.public _022672AA
	.public _022672C4
	.public _022672F8
	.public _02267300
	.public _02267304
	.public _02267308
	.public _0226730C
	.public _02267310
	.public _02267314
	.public _02267318
	.public _0226731C
	.public _02267320
	.public _02267324
	.public _0226733C
	.public _0226734E
	.public _02267358
	.public _02267364
	.public _0226736C
	.public _02267382
	.public _02267396
	.public _0226739E
	.public _022673B4
	.public _022673EA
	.public _02267456
	.public _0226745C
	.public _02267464
	.public _02267468
	.public _0226746C
	.public _02267470
	.public _02267474
	.public _02267478
	.public _0226749A
	.public _022674EA
	.public _02267586
	.public _02267670
	.public _02267692
	.public _022676E2
	.public _0226777E
	.public _02267868
	.public _02267896
	.public _022678D0
	.public _022678D8
	.public _022678E8
	.public _022678FC
	.public _0226792A
	.public _02267934
	.public _02267950
	.public _022679E6
	.public _02267A04
	.public _02267A08
	.public _02267A0C
	.public _02267A10
	.public _02267A14
	.public _02267A18
	.public _02267A7C
	.public _02267A80
	.public _02267A9A
	.public _02267ADE
	.public _02267AE8
	.public _02267AEC
	.public _02267B42
	.public _02267B74
	.public _02267B84
	.public _02267BB0
	.public _02267BBE
	.public _02267BE2
	.public _02267BF4
	.public _02267C00
	.public _02267C0E
	.public _02267C14
	.public _02267C18
	.public _02267C1C
	.public _02267C34
	.public _02267C74
	.public _02267C84
	.public _02267C88
	.public _02267CC0
	.public _02267CFC
	.public _02267D16
	.public _02267D30
	.public _02267D56
	.public _02267D5A
	.public _02267D60
	.public _02267D78
	.public _02267D88
	.public _02267D8E
	.public _02267D90
	.public _02267D94
	.public _02267DC8
	.public _02267E34
	.public _02267E56
	.public _02267E62
	.public _02267E72
	.public _02267E86
	.public _02267E8C
	.public _02267E98
	.public _02267EB2
	.public _02267EB8
	.public _02267ED0
	.public _02267F04
	.public _02267F18
	.public _02267F38
	.public _02267F3C
	.public _02267F4E
	.public _02267F72
	.public _02267F88
	.public _02267F8C
	.public _02267F90
	.public _02267FD8
	.public _02267FE8
	.public _02267FF0
	.public _02268008
	.public _02268020
	.public _0226803C
	.public _02268056
	.public _022680A8
	.public _022680AC
	.public _022680B0
	.public _022680D8
	.public _02268134
	.public _0226817A
	.public _0226818A
	.public _02268192
	.public _022681A2
	.public _022681B2
	.public _022681C4
	.public _022681D4
	.public _022681E2
	.public _0226821E
	.public _02268228
	.public _0226822C
	.public _02268252
	.public _0226826A
	.public _02268270
	.public _022682C2
	.public _022682CC
	.public _022682D0
	.public _0226830C
	.public _02268330
	.public _0226834C
	.public _022683A2
	.public _022683C4
	.public _022683D6
	.public _022683EC
	.public _022683F2
	.public _022683F8
	.public _02268418
	.public _02268420
	.public _0226842E
	.public _0226843C
	.public _0226844C
	.public _0226845A
	.public _022684C6
	.public _022684D6
	.public _022684EC
	.public _022684F0
	.public _022684F8
	.public _02268510
	.public _0226852C
	.public _0226854A
	.public _02268566
	.public _02268582
	.public _022685BA
	.public _022685F4
	.public _02268618
	.public _0226861C
	.public _022686D0
	.public _022686EC
	.public _02268700
	.public _0226877A
	.public _02268820
	.public _02268834
	.public _02268840
	.public _0226884A
	.public _0226889C
	.public _022688BA
	.public _022688D8
	.public _022688FE
	.public _02268922
	.public _0226893E
	.public _02268954
	.public _02268970
	.public _02268986
	.public _02268994
	.public _02268998
	.public _022689B2
	.public _022689C8
	.public _022689CC
	.public _022689EA
	.public _022689F2
	.public _022689F6
	.public _02268A08
	.public _02268A22
	.public _02268A66
	.public _02268ABC
	.public _02268AD0
	.public _02268ADA
	.public _02268AEC
	.public _02268AFC
	.public _02268B00
	.public _02268B20
	.public _02268B40
	.public _02268B4C
	.public _02268B6E
	.public _02268B72
	.public _02268B74
	.public _02268B86
	.public _02268B88
	.public _02268B8C
	.public _02268BC4
	.public _02268BDA
	.public _02268BE2
	.public _02268BF6
	.public _02268C0C
	.public _02268C14
	.public _02268C20
	.public _02268C24
	.public _02268C28
	.public _02268C64
	.public _02268C68
	.public _02268C6C
	.public _02268C70
	.public _02268C84
	.public _02268CA4
	.public _02268CA8
	.public _02268CB8
	.public _02268CC8
	.public _02268CD8
	.public _02268CE8
	.public _02268D1E
	.public _02268D2C
	.public _02268D36
	.public _02268D40
	.public _02268D4A
	.public _02268D54
	.public _02268D5E
	.public _02268D68
	.public _02268D72
	.public _02268D78
	.public _02268D7C
	.public _02268D80
	.public _02268D84
	.public _02268D88
	.public _02268D8C
	.public _02268D90
	.public _02268DA8
	.public _02268DAC
	.public _02268DC6
	.public _02268DC8
	.public _02268E04
	.public _02268E10
	.public _02268E1C
	.public _02268E7E
	.public _02268EA8
	.public _02268ED2
	.public _02268EF2
	.public _02268F02
	.public _02268F14
	.public _02268F24
	.public _02268F34
	.public _02268F54
	.public _02268F8A
	.public _02268F9E
	.public _02268FA2
	.public _02268FA8
	.public _0226901E
	.public _02269054
	.public _0226907E
	.public _0226908C
	.public _02269094
	.public _022690BA
	.public _022690C4
	.public _022690C6
	.public _022690FA
	.public _022690FC
	.public _0226910E
	.public _02269116
	.public _02269124
	.public _02269164
	.public _0226916C
	.public _022691B4
	.public _022691C2
	.public _022691DA
	.public _022691E8
	.public _0226928C
	.public _022692A4
	.public _022692B6
	.public _022692EC
	.public _0226930E
	.public _0226931A
	.public _02269324
	.public _02269326
	.public _02269350
	.public _0226937C
	.public _0226938A
	.public _02269392
	.public _02269398
	.public _0226939C
	.public _022693A0
	.public _022693BE
	.public _022693C8
	.public _022693D0
	.public _022693EA
	.public _022693F4
	.public _0226940A
	.public _02269414
	.public _0226942C
	.public _02269466
	.public _0226946A
	.public _02269470
	.public _022694DA
	.public _022694EA
	.public _022694EC
	.public _02269518
	.public _02269526
	.public _02269572
	.public _02269580
	.public _022695BC
	.public _022695C0
	.public sWiFiPlazaWalkUpActionTiles
	.public WiFiPlazaMap_PosToVecFx32
	.public WiFiPlazaMap_VecFx32ToPos
	.public WiFiPlazaMap_LoadModelFile
	.public WiFiPlazaMap_IsModelInstanceInView
	.public WiFiPlazaMap_Create
	.public WiFiPlazaMap_Delete
	.public WiFiPlazaMap_GetWidth
	.public WiFiPlazaMap_GetCollision
	.public WiFiPlazaMap_GetTileTag
	.public WiFiPlazaMap_GetTileAttr
	.public WiFiPlazaMap_FindTileTag
	.public WiFiPlazaMap_IsStandActionTile
	.public WiFiPlazaMap_IsWalkUpActionTile
	.public WiFiPlazaMap_IsCheckActionTile
	.public WiFiPlazaMap_IsPropTile
	.public WiFiPlazaMap_GetGrid
	.public WiFiPlazaObj_CreateManager
	.public WiFiPlazaObj_DeleteManager
	.public ov49_02258B44
	.public WiFiPlazaObj_UpdateManager
	.public WiFiPlazaObj_DrawAvatars
	.public ov49_02258BE0
	.public WiFiPlazaObj_SetLightSet
	.public WiFiPlazaObj_FadeToLightSet
	.public WiFiPlazaObj_PushCommand
	.public WiFiPlazaObj_CreateOwnPlayer
	.public WiFiPlazaObj_CreateOwnPlayerAt
	.public WiFiPlazaObj_CreatePlayer
	.public WiFiPlazaObj_Delete
	.public WiFiPlazaObj_FindById
	.public WiFiPlazaObj_GetOwnPlayer
	.public WiFiPlazaObj_GetSpecial
	.public WiFiPlazaObj_SetGridPos
	.public WiFiPlazaObj_SetGridPosAndDir
	.public WiFiPlazaObj_GetGridPos
	.public WiFiPlazaObj_GetParam
	.public WiFiPlazaObj_QueueCommand
	.public WiFiPlazaObj_RunCommand
	.public WiFiPlazaObj_SetHandler
	.public WiFiPlazaObj_IsFinished
	.public WiFiPlazaObj_GetHandler
	.public WiFiPlazaObj_FindFacing
	.public WiFiPlazaObj_IsCulled
	.public WiFiPlazaObj_FindAt
	.public WiFiPlazaObj_IsOnCell
	.public WiFiPlazaObj_FindFreeNeighbor
	.public WiFiPlazaObj_SetAvatarMode
	.public WiFiPlazaObj_SetPositionFromGrid
	.public WiFiPlazaObj_SetPosition
	.public WiFiPlazaObj_GetPosition
	.public WiFiPlazaObj_SetDirection
	.public WiFiPlazaObj_SetPoseEffect
	.public WiFiPlazaObj_SetSpinEffect
	.public WiFiPlazaObj_SetPoseLoopEffect
	.public WiFiPlazaObj_SetLightFlags
	.public WiFiPlazaObj_SetVisible
	.public WiFiPlazaObj_IsVisible
	.public WiFiPlazaObj_SpawnMapObjects
	.public WiFiPlazaObj_QueueMove
	.public WiFiPlazaObj_RandomPick
	.public WiFiPlazaObj_IsTagInDirection
	.public WiFiPlazaObj_InitTween
	.public WiFiPlazaObj_UpdateTween
	.public WiFiPlazaObj_GetTweenValue
	.public WiFiPlazaObj_AllocObj
	.public WiFiPlazaObj_IsFree
	.public WiFiPlazaObj_HandlerIdle
	.public WiFiPlazaObj_HandlerPlayerInput
	.public WiFiPlazaObj_HandlerWander
	.public WiFiPlazaObj_HandlerRiseOut
	.public WiFiPlazaObj_HandlerDropIn
	.public WiFiPlazaObj_HandlerSlideLeft
	.public WiFiPlazaObj_HandlerSlideRight
	.public WiFiPlazaObj_HandlerSlideUp
	.public WiFiPlazaObj_HandlerSlideDown
	.public WiFiPlazaObj_SlideOneCell
	.public WiFiPlazaObj_HandlerHop
	.public WiFiPlazaObj_HandlerResetHeight
	.public WiFiPlazaObj_InitLight
	.public WiFiPlazaObj_StartLightFade
	.public WiFiPlazaObj_UpdateLight
	.public WiFiPlazaRoom_Init
	.public WiFiPlazaRoom_Main
	.public WiFiPlazaRoom_Exit
	.public WiFiPlazaRoom_GetLobby
	.public WiFiPlazaRoom_GetStartParams
	.public WiFiPlazaRoom_GetObjManager
	.public WiFiPlazaRoom_GetModels
	.public WiFiPlazaRoom_GetMap
	.public WiFiPlazaRoom_GetCamera
	.public WiFiPlazaRoom_GetPlayerStates
	.public WiFiPlazaRoom_SetMode
	.public WiFiPlazaRoom_GetOwnPlayerIndex
	.public WiFiPlazaRoom_IsFading
	.public WiFiPlazaRoom_SetFinished
	.public WiFiPlazaRoom_SetResult
	.public WiFiPlazaRoom_SetActionTile
	.public WiFiPlazaRoom_GetActionTile
	.public WiFiPlazaRoom_IgnorePlazaClosing
	.public WiFiPlazaRoom_SetPlayerBusy
	.public WiFiPlazaRoom_IsPlayerBusy
	.public WiFiPlazaRoom_SetCardBlocked
	.public WiFiPlazaRoom_IsCardBlocked
	.public WiFiPlazaRoom_PrintMessage
	.public WiFiPlazaRoom_PrintMessageInstant
	.public WiFiPlazaRoom_IsMessageFinished
	.public WiFiPlazaRoom_ShowWaitingIcon
	.public WiFiPlazaRoom_HideWaitingIcon
	.public WiFiPlazaRoom_IsWaitingIconShown
	.public WiFiPlazaRoom_CloseMessage
	.public WiFiPlazaRoom_PrintSign
	.public WiFiPlazaRoom_CreateListItems
	.public WiFiPlazaRoom_CreateListItemsWithValue
	.public WiFiPlazaRoom_DeleteListItems
	.public WiFiPlazaRoom_AddListItem
	.public WiFiPlazaRoom_GetListItems
	.public WiFiPlazaRoom_HasListItemValue
	.public WiFiPlazaRoom_OpenList
	.public WiFiPlazaRoom_OpenListAt
	.public WiFiPlazaRoom_ProcessList
	.public WiFiPlazaRoom_CloseList
	.public WiFiPlazaRoom_SetListArrows
	.public WiFiPlazaRoom_OpenTextWindow
	.public WiFiPlazaRoom_CloseTextWindow
	.public WiFiPlazaRoom_PrintTextWindow
	.public WiFiPlazaRoom_FillTextWindowRect
	.public WiFiPlazaRoom_OpenYesNoMenu
	.public WiFiPlazaRoom_OpenYesNoMenuDefaultNo
	.public WiFiPlazaRoom_GetYesNoResult
	.public WiFiPlazaRoom_CloseYesNoMenu
	.public WiFiPlazaRoom_GetMsgString
	.public WiFiPlazaRoom_BufferInt
	.public WiFiPlazaRoom_BufferPlayerName
	.public WiFiPlazaRoom_BufferActivityName
	.public WiFiPlazaRoom_BufferEventName
	.public WiFiPlazaRoom_BufferInstrumentName
	.public WiFiPlazaRoom_BufferJPGreeting
	.public WiFiPlazaRoom_BufferENGreeting
	.public WiFiPlazaRoom_BufferFRGreeting
	.public WiFiPlazaRoom_BufferITGreeting
	.public WiFiPlazaRoom_BufferDEGreeting
	.public WiFiPlazaRoom_BufferSPGreeting
	.public WiFiPlazaRoom_BufferTypeName
	.public WiFiPlazaRoom_ShowPlayerCard
	.public WiFiPlazaRoom_SetPlayerCardIcon
	.public WiFiPlazaRoom_RefreshPlayerCardTypes
	.public WiFiPlazaRoom_ShowOwnCard
	.public WiFiPlazaRoom_GetPlayerCardPlayer
	.public WiFiPlazaRoom_IsPlayerCardShown
	.public WiFiPlazaRoom_IsPlayerCardIdle
	.public WiFiPlazaRoom_ShowInstrumentPanel
	.public WiFiPlazaRoom_IsMoverWobbling
	.public WiFiPlazaRoom_StartInfoPanel
	.public WiFiPlazaRoom_SetPanelButtonDisabled
	.public WiFiPlazaRoom_ResetPanelButton
	.public WiFiPlazaRoom_TakePanelButtonPress
	.public WiFiPlazaRoom_StartInstrument
	.public WiFiPlazaRoom_IsInstrumentFinished
	.public WiFiPlazaRoom_VBlank
	.public WiFiPlazaRoom_InitGraphics
	.public WiFiPlazaRoom_DrawSprites
	.public WiFiPlazaRoom_FreeGraphics
	.public WiFiPlazaRoom_GraphicsVBlank
	.public WiFiPlazaRoom_Setup3D
	.public WiFiPlazaRoom_Update
	.public WiFiPlazaRoom_Draw
	.public WiFiPlazaRoom_UpdateMatchSigns
	.public WiFiPlazaMsgWindow_Init
	.public WiFiPlazaMsgWindow_Free
	.public WiFiPlazaMsgWindow_Print
	.public WiFiPlazaMsgWindow_PrintInstant
	.public WiFiPlazaMsgWindow_ShowWaitingIcon
	.public WiFiPlazaMsgWindow_HideWaitingIcon
	.public WiFiPlazaMsgWindow_DeleteWaitingIcon
	.public WiFiPlazaMsgWindow_IsWaitingIconShown
	.public WiFiPlazaMsgWindow_IsPrintFinished
	.public WiFiPlazaMsgWindow_Close
	.public WiFiPlazaSignWindow_Init
	.public WiFiPlazaSignWindow_Free
	.public WiFiPlazaSignWindow_Print
	.public WiFiPlazaListMenu_Init
	.public WiFiPlazaListMenu_Free
	.public WiFiPlazaListMenu_CreateItems
	.public WiFiPlazaListMenu_DeleteItems
	.public WiFiPlazaListMenu_AddItem
	.public WiFiPlazaListMenu_GetItems
	.public WiFiPlazaListMenu_HasItemValue
	.public WiFiPlazaListMenu_Open
	.public WiFiPlazaListMenu_ProcessInput
	.public WiFiPlazaListMenu_Close
	.public WiFiPlazaListMenu_OnCursorMove
	.public WiFiPlazaListMenu_SetShowArrows
	.public WiFiPlazaListMenu_UpdateArrows
	.public WiFiPlazaTextWindow_Init
	.public WiFiPlazaTextWindow_Free
	.public WiFiPlazaTextWindow_Open
	.public WiFiPlazaTextWindow_Close
	.public WiFiPlazaTextWindow_Print
	.public WiFiPlazaTextWindow_FillRect
	.public WiFiPlazaYesNoMenu_Init
	.public WiFiPlazaYesNoMenu_Free
	.public WiFiPlazaErrorWindow_Init
	.public WiFiPlazaErrorWindow_Free
	.public WiFiPlazaErrorWindow_Print
	.public WiFiPlazaErrorWindow_PrintCommError
	.public WiFiPlazaErrorWindow_PrintLobbyError
	.public WiFiPlazaErrorWindow_PrintDisconnected
	.public WiFiPlazaRoomMsg_Init
	.public WiFiPlazaRoomMsg_Free
	.public WiFiPlazaRoomMsg_GetString
	.public WiFiPlazaRoomMsg_BufferInt
	.public WiFiPlazaRoomMsg_BufferPlayerName
	.public WiFiPlazaRoomMsg_BufferActivityName
	.public WiFiPlazaRoomMsg_BufferEventName
	.public WiFiPlazaRoomMsg_BufferInstrumentName
	.public WiFiPlazaRoomMsg_BufferCountryName
	.public WiFiPlazaRoomMsg_BufferCityName
	.public WiFiPlazaRoomMsg_BufferECWord
	.public WiFiPlazaRoomMsg_ResetBuffers
	.public WiFiPlazaRoom_SetPanelButtonPressed
	.public WiFiPlazaInfoPanel_Init
	.public WiFiPlazaInfoPanel_Free
	.public WiFiPlazaInfoPanel_Update
	.public WiFiPlazaInfoPanel_GetState
	.public WiFiPlazaInfoPanel_RequestPlayerCard
	.public WiFiPlazaInfoPanel_SetCardIcon
	.public WiFiPlazaInfoPanel_RequestOwnCard
	.public WiFiPlazaInfoPanel_RequestInstrument
	.public WiFiPlazaInfoPanel_GetCardPlayer
	.public WiFiPlazaInfoPanel_IsPlayerCardShown
	.public WiFiPlazaInfoPanel_IsPlayerCardIdle
	.public WiFiPlazaInfoPanel_CardShowsMessage
	.public WiFiPlazaInfoPanel_HasRequest
	.public WiFiPlazaInfoPanel_ShowPlayerCard
	.public WiFiPlazaInfoPanel_HidePlayerCard
	.public WiFiPlazaInfoPanel_ShowOwnCard
	.public WiFiPlazaInfoPanel_ShowInstrument
	.public WiFiPlazaInfoPanel_HideButtonPage
	.public WiFiPlazaInfoPanel_SetButtonDisabled
	.public WiFiPlazaInfoPanel_StartPaletteLoad
	.public WiFiPlazaInfoPanel_LoadPaletteTask
	.public WiFiPlazaInfoPanel_LoadGraphics
	.public WiFiPlazaInfoPanel_FreeGraphics
	.public WiFiPlazaInfoPanel_LoadScreen
	.public WiFiPlazaPlayerCard_Init
	.public WiFiPlazaPlayerCard_Free
	.public WiFiPlazaPlayerCard_Update
	.public WiFiPlazaPlayerCard_Show
	.public WiFiPlazaPlayerCard_LoadTypeIcons
	.public WiFiPlazaPlayerCard_Clear
	.public WiFiPlazaPlayerCard_ChangeIcon
	.public WiFiPlazaPlayerCard_ShowsMessage
	.public WiFiPlazaPlayerCard_LoadIcon
	.public WiFiPlazaPlayerCard_FreeIcon
	.public WiFiPlazaPlayerCard_LoadAvatar
	.public WiFiPlazaPlayerCard_FreeAvatar
	.public WiFiPlazaInfoPanel_TintPalette
	.public WiFiPlazaPlayerCard_ClearWindows
	.public WiFiPlazaPlayerCard_Print
	.public WiFiPlazaPlayerCard_PrintRightAligned
	.public WiFiPlazaPlayerCard_CopyWindow
	.public WiFiPlazaPlayerCard_BufferName
	.public WiFiPlazaPlayerCard_BufferLocation
	.public WiFiPlazaPlayerCard_BufferMessage
	.public WiFiPlazaPlayerCard_LoadMons
	.public WiFiPlazaPlayerCard_FreeMons
	.public WiFiPlazaPlayerCard_GetMonInfo
	.public WiFiPlazaPanelButton_Init
	.public WiFiPlazaPanelButton_Free
	.public WiFiPlazaPanelButton_Update
	.public WiFiPlazaPanelButton_ShowOwnCard
	.public WiFiPlazaPanelButton_ShowInstrument
	.public WiFiPlazaPanelButton_Clear
	.public WiFiPlazaPanelButton_Draw
	.public WiFiPlazaPanelButton_OnTouch
	.public WiFiPlazaPanelButton_ResetTimer
	.public WiFiPlazaRoom_GetTextSpeed
	.public WiFiPlazaCamera_Create
	.public WiFiPlazaCamera_Delete
	.public WiFiPlazaCamera_Update
	.public WiFiPlazaCamera_SetTarget
	.public WiFiPlazaCamera_SetTargetWithOffset
	.public WiFiPlazaCamera_SetFollowObject
	.public WiFiPlazaCamera_ClearFollowObject
	.public WiFiPlazaModelSet_Create
	.public WiFiPlazaModelSet_Delete
	.public WiFiPlazaModelSet_Update
	.public WiFiPlazaModelSet_Draw
	.public WiFiPlazaModelSet_VBlank
	.public WiFiPlazaModelSet_Load
	.public WiFiPlazaModelSet_Unload
	.public WiFiPlazaModelSet_StartBaseAnim4
	.public WiFiPlazaModelSet_StopBaseAnim4
	.public WiFiPlazaModelSet_AddMover
	.public WiFiPlazaModelSet_RemoveMover
	.public WiFiPlazaMover_SetPosition
	.public WiFiPlazaMover_SetOffset
	.public WiFiPlazaMover_GetPosition
	.public WiFiPlazaMover_SetVisible
	.public WiFiPlazaMover_StartAnim1
	.public WiFiPlazaMover_StartAnim3
	.public WiFiPlazaMover_SetAngle
	.public WiFiPlazaMover_IsAnim1Playing
	.public WiFiPlazaMover_IsAnim3Playing
	.public WiFiPlazaModelSet_AddProp
	.public WiFiPlazaModelSet_RemoveProp
	.public WiFiPlazaProp_GetKind
	.public WiFiPlazaProp_SetPos
	.public WiFiPlazaProp_GetPos
	.public WiFiPlazaModelSet_StartPropAnim
	.public WiFiPlazaModelSet_StartPropAnimWithCallback
	.public WiFiPlazaModelSet_StopPropAnim
	.public WiFiPlazaModelSet_StopPropAnims
	.public WiFiPlazaModelSet_IsPropAnimActive
	.public WiFiPlazaModelSet_SetPropAnimFrame
	.public WiFiPlazaProp_IsAnimActive
	.public WiFiPlazaProp_GetAnimFrame
	.public WiFiPlazaProp_SetVisible
	.public WiFiPlazaModelSet_SetPropLights
	.public WiFiPlazaProp_SetAnimSpeed
	.public WiFiPlazaProp_SetAlpha
	.public WiFiPlazaProp_ClearAlpha
	.public WiFiPlazaProp_SetScale
	.public WiFiPlazaModels_LoadLayout
	.public WiFiPlazaModels_FreeLayout
	.public WiFiPlazaModels_LoadModel
	.public WiFiPlazaModels_FreeModel
	.public WiFiPlazaModels_AdvanceFrameLooped
	.public WiFiPlazaModels_AdvanceFrame
	.public WiFiPlazaModels_RewindFrameLooped
	.public WiFiPlazaModels_RewindFrame
	.public WiFiPlazaModelSet_LoadBase
	.public WiFiPlazaModelSet_FreeBase
	.public WiFiPlazaModelSet_InitBase
	.public WiFiPlazaModelSet_ResetBase
	.public WiFiPlazaModelSet_UpdateBase
	.public WiFiPlazaModelSet_DrawBase
	.public WiFiPlazaModelSet_AllocMover
	.public WiFiPlazaModelSet_LoadMovers
	.public WiFiPlazaModelSet_FreeMovers
	.public WiFiPlazaModelSet_UpdateMover
	.public WiFiPlazaModelSet_DrawMover
	.public WiFiPlazaModelSet_AllocProp
	.public WiFiPlazaModelSet_LoadProps
	.public WiFiPlazaModelSet_FreeProps
	.public WiFiPlazaModelSet_DrawProp
	.public WiFiPlazaModelSet_UpdateProp
	.public WiFiPlazaModels_Create
	.public WiFiPlazaModels_Delete
	.public WiFiPlazaModels_Update
	.public WiFiPlazaModels_Draw
	.public WiFiPlazaModels_VBlank
	.public WiFiPlazaModels_SetMoverX
	.public WiFiPlazaModels_SetMoverOffset
	.public WiFiPlazaModels_GetMoverFrontPos
	.public WiFiPlazaModels_StartMoverAnim1
	.public WiFiPlazaModels_StartMoverAnim3
	.public WiFiPlazaModels_SetMoverVisible
	.public WiFiPlazaModels_SetMoverAngle
	.public WiFiPlazaModels_IsMoverAnim1Playing
	.public WiFiPlazaModels_IsMoverAnim3Playing
	.public WiFiPlazaModels_StartBaseAnim4
	.public WiFiPlazaModels_StopBaseAnim4
	.public WiFiPlazaModels_TouchPropsAt
	.public WiFiPlazaModels_SetEffectActive
	.public WiFiPlazaModels_TriggerStageBursts
	.public WiFiPlazaModels_EnableKind1To4Lights
	.public WiFiPlazaModels_SetPairAnim
	.public WiFiPlazaModels_GetPairAnimState
	.public WiFiPlazaModels_ShowEffectProps
	.public WiFiPlazaModels_ShowEffectProp
	.public WiFiPlazaModels_TriggerStageShows
	.public WiFiPlazaModels_UpdateMatchSign
	.public WiFiPlazaProp_IsAt
	.public WiFiPlazaModels_OnPairAnimOpened
	.public WiFiPlazaModels_OnPairAnimClosed
	.public WiFiPlazaModels_TouchProp
	.public WiFiPlazaModels_TouchPropQuiet
	.public WiFiPlazaModels_PlayPropAnimsOnce
	.public WiFiPlazaPropShow_Init
	.public WiFiPlazaPropShow_UpdateEffect
	.public WiFiPlazaPropShow_RequestShow
	.public WiFiPlazaPropShow_RequestHide
	.public WiFiPlazaPropShow_ShowEffect
	.public WiFiPlazaPropShow_HideEffect
	.public WiFiPlazaPropShow_StartFade
	.public WiFiPlazaPropShow_UpdateFade
	.public WiFiPlazaPropShow_Trigger
	.public WiFiPlazaPropShow_UpdateStage
	.public WiFiPlazaPropShow_ShowStage
	.public WiFiPlazaPropShow_UpdateBurst
	.public WiFiPlazaPropShow_ShowBurst
	.public WiFiPlazaProp_ApplyTagShift
	.public WiFiPlazaPlayerState_CreateManager
	.public WiFiPlazaPlayerState_DeleteManager
	.public WiFiPlazaPlayerState_Update
	.public WiFiPlazaPlayerState_LateUpdate
	.public WiFiPlazaPlayerState_GetArg
	.public WiFiPlazaPlayerState_AllocWork
	.public WiFiPlazaPlayerState_FreeWork
	.public WiFiPlazaPlayerState_GetWork
	.public WiFiPlazaPlayerState_GetStep
	.public WiFiPlazaPlayerState_SetStep
	.public WiFiPlazaPlayerState_NextStep
	.public WiFiPlazaPlayerState_Set
	.public WiFiPlazaPlayerState_Push
	.public WiFiPlazaPlayerState_SetActive
	.public WiFiPlazaPlayerState_RunAll
	.public WiFiPlazaPlayerState_Init
	.public WiFiPlazaPlayerState_Clear
	.public WiFiPlazaPlayerState_Run
	.public WiFiPlazaPlayerState_SetFrame
	.public WiFiPlazaPlayerState_SetActiveFlag
	.public WiFiPlazaPlayerState_PushFrame
	.public WiFiPlazaPlayerState_PopFrame
	.public WiFiPlazaPlayerState_IsSavedFree
	.public WiFiPlazaPlayerState_HasHandler
	.public WiFiPlazaPlayerStateFrame_Init
	.public WiFiPlazaPlayerStateFrame_Clear
	.public WiFiPlazaPlayerState_InitOwn
	.public WiFiPlazaPlayerState_InitOthers
	.public WiFiPlazaPlayerState_IsDirectionHeld
	.public WiFiPlazaPlayerState_RunTileAction
	.public WiFiPlazaPlayerState_CreateOwnPlayer
	.public WiFiPlazaPlayerState_CreateOwnPlayerAt
	.public WiFiPlazaPlayerBounce_Start
	.public WiFiPlazaPlayerBounce_Update
	.public WiFiPlazaPlayerBounce_Stop
	.public WiFiPlazaPlayerBounce_IsActive
	.public WiFiPlazaPlayerState_PlayInstrument
	.public WiFiPlazaPlayerState_OwnArrival
	.public WiFiPlazaPlayerState_OwnReturnFromAttraction
	.public WiFiPlazaPlayerState_OwnReturnFromMinigame
	.public WiFiPlazaPlayerState_OwnReturnFromSurvey
	.public WiFiPlazaPlayerState_OwnWalk
	.public WiFiPlazaPlayerState_OwnWalkLate
	.public WiFiPlazaPlayerState_OwnExit
	.public WiFiPlazaPlayerState_OwnEnterAttraction
	.public WiFiPlazaPlayerState_OwnEnterMinigame
	.public WiFiPlazaPlayerState_OwnUseSurveyMonitor
	.public WiFiPlazaPlayerState_GetAttractionSlot
	.public WiFiPlazaPlayerState_GetAttractionResult
	.public WiFiPlazaPlayerState_GetAttractionLobbyState
	.public WiFiPlazaTalk_Greeter
	.public WiFiPlazaPlayerState_OwnRideMover
	.public WiFiPlazaPlayerRide_FollowCamera
	.public WiFiPlazaPlayerRide_IsMoverHere
	.public WiFiPlazaPlayerRide_JumpOn
	.public WiFiPlazaPlayerRide_Land
	.public WiFiPlazaPlayerRide_FollowMover
	.public WiFiPlazaPlayerRide_FollowMoverBounce
	.public WiFiPlazaPlayerState_InitArrivalMenu
	.public WiFiPlazaPlayerState_FreeArrivalMenu
	.public WiFiPlazaPlayerState_OtherWaitForPlayer
	.public WiFiPlazaPlayerState_OtherPresent
	.public WiFiPlazaPlayerState_OtherLeave
	.public WiFiPlazaPlayerState_OtherRideMover
	.public WiFiPlazaPlayerState_OtherDoActivity
	.public WiFiPlazaPlayerState_PlaceOtherForActivity
	.public WiFiPlazaPlayerState_FindFreeTaggedCell
	.public WiFiPlazaGameEntry_Register
	.public WiFiPlazaGameEntry_EndLink
	.public WiFiPlazaGameEntry_ReadSign
	.public WiFiPlazaGameEntry_PrintMessage
	.public WiFiPlazaGameEntry_StartSync
	.public WiFiPlazaGameEntry_InitMenu
	.public WiFiPlazaGameEntry_FreeMenu
	.public WiFiPlazaGameEntry_UpdateWaiting
	.public WiFiPlazaGameStatus_Open
	.public WiFiPlazaGameStatus_Close
	.public WiFiPlazaGameStatus_SetFrames
	.public WiFiPlazaGameStatus_Update
	.public WiFiPlazaGameStatus_UpdateInPlay
	.public WiFiPlazaGameStatus_Draw
	.public WiFiPlazaTalk_Start
	.public WiFiPlazaTalk_Answer
	.public WiFiPlazaTalk_InfoCounter
	.public WiFiPlazaTalk_Reception
	.public WiFiPlazaTalk_GetTimeOfDay
	.public WiFiPlazaTalk_GetMsg
	.public WiFiPlazaTalk_GetReunionMsg
	.public WiFiPlazaTalk_Init
	.public WiFiPlazaTalk_Free
	.public WiFiPlazaTalk_SetNextStep
	.public WiFiPlazaTalk_WaitTimer
	.public WiFiPlazaTalk_WaitMessage
	.public WiFiPlazaTalk_PrintGreeting
	.public WiFiPlazaTalk_BufferGreeting
	.public WiFiPlazaTalk_PrintTimeRemark
	.public WiFiPlazaTalk_BufferInstrument
	.public WiFiPlazaTalk_UpdatePartnerWait
	.public WiFiPlazaTalk_StartMenuTimer
	.public WiFiPlazaTalk_StopMenuTimer
	.public WiFiPlazaTalk_UpdateMenuWait
	.public WiFiPlazaTalk_CountMenuTimer
	.public WiFiPlazaTalk_SetHistory
	.public WiFiPlazaTalkMenu_Init
	.public WiFiPlazaTalkMenu_InitRandom
	.public WiFiPlazaTalkMenu_InitInfoCounter
	.public WiFiPlazaTalkMenu_InitTimeEvents
	.public WiFiPlazaTalkMenu_InitPlazaGames
	.public WiFiPlazaTalkMenu_Free
	.public WiFiPlazaTalkReply_Init
	.public WiFiPlazaTalkReply_Start
	.public WiFiPlazaTalkReply_Update
	.public WiFiPlazaTalkReply_IsDone
	.public WiFiPlazaTalkReply_GetStatus
	.public WiFiPlazaInstrument_Create
	.public WiFiPlazaInstrument_Delete
	.public WiFiPlazaInstrument_Update
	.public WiFiPlazaInstrument_Draw
	.public WiFiPlazaInstrument_Start
	.public WiFiPlazaInstrument_IsFinished
	.public WiFiPlazaInstrumentTween_Init
	.public WiFiPlazaInstrumentTween_Update
	.public WiFiPlazaInstrumentTween_GetPosition
	.public WiFiPlazaInstrumentWave_Init
	.public WiFiPlazaInstrumentWave_Update
	.public WiFiPlazaInstrumentWave_GetValue
	.public WiFiPlazaInstrument_PlaySE
	.public WiFiPlazaInstrument_LoadModels
	.public WiFiPlazaInstrument_FreeModels
	.public WiFiPlazaInstrument_LoadTextures
	.public WiFiPlazaInstrument_FreeTextures
	.public WiFiPlazaInstrument_LoadAnimations
	.public WiFiPlazaInstrument_FreeAnimations
	.public WiFiPlazaInstrument_StartEffect
	.public WiFiPlazaInstrument_UpdateEffect
	.public WiFiPlazaInstrument_DrawEffect
	.public WiFiPlazaInstrument_ClearEffect
	.public WiFiPlazaInstrument_IsEffectActive
	.public WiFiPlazaInstrument_HasModel
	.public WiFiPlazaInstrument_SetModel
	.public WiFiPlazaInstrument_DrawModel
	.public WiFiPlazaInstrument_LoopAnim
	.public WiFiPlazaInstrument_PlayAnim
	.public WiFiPlazaInstrument_LoopAnimBy
	.public WiFiPlazaInstrument_PlayAnimBy
	.public WiFiPlazaInstrument_SetAnimFrame
	.public WiFiPlazaInstrument_GetAnimFrame
	.public WiFiPlazaInstrument_InitRipple01
	.public WiFiPlazaInstrument_InitRipple02
	.public WiFiPlazaInstrument_InitRipple03
	.public WiFiPlazaInstrument_InitWhirl01
	.public WiFiPlazaInstrument_InitWhirl02
	.public WiFiPlazaInstrument_InitWhirl03
	.public WiFiPlazaInstrument_InitSignal
	.public WiFiPlazaInstrument_InitPop01
	.public WiFiPlazaInstrument_InitPop02
	.public WiFiPlazaInstrument_InitPop03
	.public WiFiPlazaInstrument_InitBalloon01
	.public WiFiPlazaInstrument_InitBalloon02
	.public WiFiPlazaInstrument_InitBalloon03
	.public WiFiPlazaInstrument_InitSparkle01
	.public WiFiPlazaInstrument_InitSparkle02
	.public WiFiPlazaInstrument_InitSparkle03
	.public WiFiPlazaInstrument_InitBell01
	.public WiFiPlazaInstrument_InitBell02
	.public WiFiPlazaInstrument_InitBell03
	.public WiFiPlazaInstrument_InitDrum01
	.public WiFiPlazaInstrument_InitDrum02
	.public WiFiPlazaInstrument_InitDrum03
	.public WiFiPlazaInstrument_InitCymbal01
	.public WiFiPlazaInstrument_InitCymbal02
	.public WiFiPlazaInstrument_InitCymbal03
	.public WiFiPlazaInstrument_UpdateRipple
	.public WiFiPlazaInstrument_UpdateSignal01
	.public WiFiPlazaInstrument_UpdateSignal02
	.public WiFiPlazaInstrument_UpdateSignal03
	.public WiFiPlazaInstrument_UpdateWhirl
	.public WiFiPlazaInstrument_UpdatePop
	.public WiFiPlazaInstrument_UpdateBalloon
	.public WiFiPlazaInstrument_UpdateSparkle
	.public WiFiPlazaInstrument_UpdateBellDrum
	.public WiFiPlazaInstrument_UpdateCymbal
	.public WiFiPlazaInstrument_DrawEffectModels
	.public WiFiPlazaInstrument_InitBalloon
	.public WiFiPlazaInstrument_StartBalloonRise
	.public WiFiPlazaInstrument_StartBalloonLift
	.public WiFiPlazaInstrument_UpdateBalloonUp
	.public WiFiPlazaInstrument_UpdateBalloonRise
	.public WiFiPlazaInstrument_UpdateBalloonLift
	.public WiFiPlazaInstrument_StartBalloonDown
	.public WiFiPlazaInstrument_StartBalloonFall
	.public WiFiPlazaInstrument_StartBalloonDrop
	.public WiFiPlazaInstrument_UpdateBalloonDown
	.public WiFiPlazaInstrument_UpdateBalloonFall
	.public WiFiPlazaInstrument_UpdateBalloonDrop
	.public WiFiPlazaInstrument_PlaceBalloons
	.public WiFiPlazaInstrument_PlaceBalloonsLifted
	.public WiFiPlazaInstrument_SetBalloonAnim
	.public WiFiPlazaInstrument_UpdateBalloonAnim
	.public WiFiPlazaInstrument_InitSparkle
	.public WiFiPlazaInstrument_UpdateSparkleModel
	.public WiFiPlazaInstrument_InitRipple
	.public WiFiPlazaInstrument_UpdateSignalSequence
	.public WiFiPlazaInstrument_InitWhirl
	.public WiFiPlazaInstrument_InitCymbalFlash
	.public WiFiPlazaInstrument_StartCymbalFlash
	.public WiFiPlazaInstrument_UpdateCymbalFlash
	.public WiFiPlazaInstrument_InitBursts
	.public WiFiPlazaInstrument_UpdateBursts
	.public WiFiPlazaInstrumentBurst_Init
	.public WiFiPlazaInstrumentBurst_Start
	.public WiFiPlazaInstrumentBurst_Update
	.public WiFiPlazaInstrumentBurst_StartBell
	.public WiFiPlazaInstrumentBurst_UpdateBell
	.public WiFiPlazaInstrumentBurst_StartDrum
	.public WiFiPlazaInstrumentBurst_UpdateDrum
	.public WiFiPlazaInstrumentBurst_StartCymbal
	.public WiFiPlazaInstrumentBurst_UpdateCymbal
	.public WiFiPlazaInstrumentBurst_StartHop
	.public WiFiPlazaLight_Create
	.public WiFiPlazaLight_Delete
	.public WiFiPlazaLight_Update
	.public WiFiPlazaLight_FadeToColorSet
	.public WiFiPlazaLight_FadeLight1
	.public WiFiPlazaLight_FadeToThirdSet
	.public WiFiPlazaLight_PackColor
	.public WiFiPlazaLight_UnpackColor
	.public WiFiPlazaLight_LerpColor
	.public WiFiPlazaLight_UnpackColorSet
	.public WiFiPlazaLight_UnpackLight1
	.public WiFiPlazaLight_Apply
	.public WiFiPlazaEvent_Create
	.public WiFiPlazaEvent_Delete
	.public WiFiPlazaEvent_Update
	.public WiFiPlazaEvent_GetMoverState
	.public WiFiPlazaEvent_GetColorSet
	.public WiFiPlazaEvent_GetLight1Set
	.public WiFiPlazaEvent_GetThirdSet
	.public WiFiPlazaEvent_TurnOffLight2
	.public WiFiPlazaEventEffect_Create
	.public WiFiPlazaEventEffect_Delete
	.public WiFiPlazaEventEffect_Update
	.public WiFiPlazaEventEffect_RunPhase
	.public WiFiPlazaEventEffect_UpdateNone
	.public WiFiPlazaEventEffect_UpdatePhase2
	.public WiFiPlazaEventEffect_UpdatePhase3
	.public WiFiPlazaEventEffect_UpdatePhase4
	.public WiFiPlazaEventEffect_UpdatePhase5
	.public WiFiPlazaEventEffect_StartPhase
	.public WiFiPlazaEventEffect_StartHideProps
	.public WiFiPlazaEventEffect_StartPhase2
	.public WiFiPlazaEventEffect_StartPhase3
	.public WiFiPlazaEventEffect_StartPhase4
	.public WiFiPlazaEventEffect_StartPhase5
	.public WiFiPlazaEvent_GetModelList
	.public WiFiPlazaEvent_StartLoopSE
	.public WiFiPlazaEvent_StopLoopSE
	.public WiFiPlazaTalk_Survey
	.public WiFiPlazaEventMovers_Create
	.public WiFiPlazaEventMovers_Delete
	.public WiFiPlazaEventMovers_Update
	.public WiFiPlazaEventMovers_GetState
	.public WiFiPlazaEventMovers_UpdateX
	.public WiFiPlazaEventMovers_StartActions
	.public WiFiPlazaEventMovers_PlaySE
	.public WiFiPlazaEventMovers_PlayCry
	.public WiFiPlazaEventWobble_Start
	.public WiFiPlazaEventWobble_Update
	.public WiFiPlazaEventWobble_InitSmall
	.public WiFiPlazaEventWobble_InitLarge
	.public WiFiPlazaEventWobble_Step
	.public WiFiPlazaEvent_UpdateMoverMarkers
	.public sWiFiPlazaPropTiles
	.public sWiFiPlazaCheckActionTiles
	.public sWiFiPlazaStandActionTiles
	.public ov49_02269654
	.public ov49_02269658
	.public ov49_0226965C
	.public ov49_02269660
	.public sWiFiPlazaObjMapSpawns
	.public sWiFiPlazaObjExitHandlers
	.public sWiFiPlazaObjHandlers
	.public sWiFiPlazaListArrowY
	.public ov49_022696EC
	.public ov49_022696F0
	.public sWiFiPlazaPanelButtonHitbox
	.public ov49_022696F8
	.public ov49_022696FC
	.public ov49_02269704
	.public sWiFiPlazaPanelButtonWindowTemplate
	.public sWiFiPlazaRoomMsgBanks
	.public sWiFiPlazaRoomGraphicsModes
	.public sWiFiPlazaRoomBgIds
	.public sWiFiPlazaRoomCharTransfer
	.public sWiFiPlazaRoomEdgeColors
	.public ov49_02269764
	.public ov49_02269774
	.public ov49_0226978C
	.public sWiFiPlazaYesNoTemplate
	.public sWiFiPlazaRoomBanks
	.public sWiFiPlazaCardWindowTemplates
	.public sWiFiPlazaRoomBgTemplates
	.public sWiFiPlazaCardAvatars
	.public sWiFiPlazaCardMonPositions
	.public ov49_022699AC
	.public sWiFiPlazaCameraAngle
	.public sWiFiPlazaPropTagShiftX
	.public ov49_02269A7C
	.public ov49_02269A7E
	.public sWiFiPlazaMoverSpawns
	.public sWiFiPlazaPropSpawns
	.public sWiFiPlazaPlayerStateOwnWalk
	.public sWiFiPlazaPlayerStateOtherPresent
	.public sWiFiPlazaPlayerStateOtherLeave
	.public sWiFiPlazaPlayerStateOtherRideMover
	.public sWiFiPlazaPlayerStateOtherActivity
	.public sWiFiPlazaPlayerStateTalkStart
	.public sWiFiPlazaPlayerStateGameEntryEndLink
	.public sWiFiPlazaPlayerStateTalkAnswer
	.public sWiFiPlazaPlayerStateOtherWait
	.public sWiFiPlazaPlayerStatePlayInstrument
	.public sWiFiPlazaPlayerStateGameEntryRegister
	.public ov49_02269B90
	.public ov49_02269BA0
	.public ov49_02269BB0
	.public ov49_02269BC0
	.public ov49_02269BD0
	.public sWiFiPlazaPlayerStateOwnEntries
	.public sWiFiPlazaArrivalMenuTemplate
	.public ov49_02269C20
	.public ov49_02269C40
	.public sWiFiPlazaWalkUpTileActions
	.public sWiFiPlazaCheckTileActions
	.public sWiFiPlazaStandTileActions
	.public sWiFiPlazaGameStatusHeights
	.public sWiFiPlazaGameEntryMenuTemplate
	.public sWiFiPlazaInfoCounterMsgs
	.public sWiFiPlazaTalkMenuTemplate
	.public sWiFiPlazaTalkGreetingMsgs
	.public sWiFiPlazaTalkMsgPairs
	.public sWiFiPlazaTalkMsgPairsHighlighted
	.public sWiFiPlazaInstrumentBalloonHeights
	.public sWiFiPlazaInstrumentBursts_Bell01
	.public sWiFiPlazaInstrumentBursts_Cymbal02
	.public sWiFiPlazaInstrumentBursts_Drum01
	.public sWiFiPlazaInstrumentBursts_Bell02
	.public sWiFiPlazaInstrumentBursts_Cymbal03
	.public sWiFiPlazaInstrumentBursts_Cymbal01
	.public sWiFiPlazaInstrumentBurstUpdateFuncs
	.public sWiFiPlazaInstrumentBursts_Drum02
	.public sWiFiPlazaInstrumentBursts_Bell03
	.public sWiFiPlazaInstrumentBursts_Drum03
	.public sWiFiPlazaInstrumentBurstStartFuncs
	.public sWiFiPlazaInstrumentSparkleStarts
	.public sWiFiPlazaInstrumentSparkleFalls
	.public sWiFiPlazaInstrumentUpdateFuncs
	.public sWiFiPlazaInstrumentInitFuncs
	.public sWiFiPlazaInstrumentStartSEs
	.public ov49_0226A678
	.public ov49_0226A67C
	.public sWiFiPlazaInstrumentModelDescs
	.public ov49_0226A730
	.public ov49_0226A734
	.public ov49_0226A738
	.public ov49_0226A73C
	.public ov49_0226A74C
	.public ov49_0226A750
	.public ov49_0226A754
	.public sWiFiPlazaInstrumentBurstModels01
	.public sWiFiPlazaInstrumentBurstModels02
	.public sWiFiPlazaInstrumentBurstModels03
	.public sWiFiPlazaLight1Colors
	.public sWiFiPlazaLightColorSets
	.public ov49_0226A81C
	.public ov49_0226A820
	.public ov49_0226A824
	.public ov49_0226A828
	.public ov49_0226A82C
	.public ov49_0226A830
	.public ov49_0226A834
	.public ov49_0226A83C
	.public ov49_0226A83D
	.public sWiFiPlazaEventEffectPhaseUpdates
	.public ov49_0226A864
	.public ov49_0226A868
	.public sWiFiPlazaEventEffectPhaseStarts
	.public ov49_0226A894
	.public ov49_0226A8B4
	.public sWiFiPlazaMoverMarkerTags
	.public sWiFiPlazaEventWobbleSteps
	.public sWiFiPlazaEventWobbleInits
	.public sWiFiPlazaMoverSounds
	.public WiFiPlazaMap_PosToVecFx32
	.public WiFiPlazaMap_VecFx32ToPos
	.public WiFiPlazaMap_Delete
	.public WiFiPlazaMap_GetWidth
	.public WiFiPlazaMap_GetCollision
	.public WiFiPlazaMap_GetTileAttr
	.public WiFiPlazaMap_GetGrid
	.public WiFiPlazaObj_DrawAvatars
	.public ov49_02258BE0
	.public WiFiPlazaObj_PushCommand
	.public WiFiPlazaObj_GetOwnPlayer
	.public WiFiPlazaObj_GetSpecial
	.public WiFiPlazaObj_IsFinished
	.public WiFiPlazaObj_GetHandler
	.public WiFiPlazaObj_IsCulled
	.public WiFiPlazaObj_SetAvatarMode
	.public WiFiPlazaObj_SetPositionFromGrid
	.public WiFiPlazaObj_SetPosition
	.public WiFiPlazaObj_GetPosition
	.public WiFiPlazaObj_SetDirection
	.public WiFiPlazaObj_SetPoseEffect
	.public WiFiPlazaObj_SetSpinEffect
	.public WiFiPlazaObj_SetPoseLoopEffect
	.public WiFiPlazaObj_SetLightFlags
	.public WiFiPlazaObj_SetVisible
	.public WiFiPlazaObj_IsVisible
	.public WiFiPlazaObj_InitTween
	.public WiFiPlazaObj_GetTweenValue
	.public WiFiPlazaObj_IsFree
	.public WiFiPlazaObj_HandlerIdle
	.public WiFiPlazaObj_HandlerSlideLeft
	.public WiFiPlazaObj_HandlerSlideRight
	.public WiFiPlazaObj_HandlerSlideUp
	.public WiFiPlazaObj_HandlerSlideDown
	.public WiFiPlazaObj_StartLightFade
	.public WiFiPlazaRoom_GetLobby
	.public WiFiPlazaRoom_GetStartParams
	.public WiFiPlazaRoom_GetObjManager
	.public WiFiPlazaRoom_GetModels
	.public WiFiPlazaRoom_GetMap
	.public WiFiPlazaRoom_GetCamera
	.public WiFiPlazaRoom_GetPlayerStates
	.public WiFiPlazaRoom_SetMode
	.public WiFiPlazaRoom_GetOwnPlayerIndex
	.public WiFiPlazaRoom_IsFading
	.public WiFiPlazaRoom_SetFinished
	.public WiFiPlazaRoom_SetResult
	.public WiFiPlazaRoom_SetActionTile
	.public WiFiPlazaRoom_GetActionTile
	.public WiFiPlazaRoom_IgnorePlazaClosing
	.public WiFiPlazaRoom_SetPlayerBusy
	.public WiFiPlazaRoom_IsPlayerBusy
	.public WiFiPlazaRoom_IsCardBlocked
	.public WiFiPlazaRoom_StartInfoPanel
	.public WiFiPlazaRoom_TakePanelButtonPress
	.public WiFiPlazaMsgWindow_HideWaitingIcon
	.public WiFiPlazaMsgWindow_DeleteWaitingIcon
	.public WiFiPlazaMsgWindow_IsWaitingIconShown
	.public WiFiPlazaListMenu_DeleteItems
	.public WiFiPlazaListMenu_GetItems
	.public WiFiPlazaListMenu_OnCursorMove
	.public WiFiPlazaListMenu_SetShowArrows
	.public WiFiPlazaTextWindow_Init
	.public WiFiPlazaTextWindow_Free
	.public WiFiPlazaYesNoMenu_Free
	.public WiFiPlazaRoom_SetPanelButtonPressed
	.public WiFiPlazaInfoPanel_GetState
	.public WiFiPlazaInfoPanel_RequestPlayerCard
	.public WiFiPlazaInfoPanel_RequestOwnCard
	.public WiFiPlazaInfoPanel_RequestInstrument
	.public WiFiPlazaInfoPanel_GetCardPlayer
	.public WiFiPlazaInfoPanel_IsPlayerCardShown
	.public WiFiPlazaInfoPanel_IsPlayerCardIdle
	.public WiFiPlazaInfoPanel_HasRequest
	.public WiFiPlazaInfoPanel_HidePlayerCard
	.public WiFiPlazaInfoPanel_HideButtonPage
	.public WiFiPlazaInfoPanel_SetButtonDisabled
	.public WiFiPlazaInfoPanel_FreeGraphics
	.public WiFiPlazaPlayerCard_Update
	.public WiFiPlazaPlayerCard_ShowsMessage
	.public WiFiPlazaPanelButton_OnTouch
	.public WiFiPlazaPanelButton_ResetTimer
	.public WiFiPlazaRoom_GetTextSpeed
	.public WiFiPlazaCamera_Delete
	.public WiFiPlazaCamera_SetTarget
	.public WiFiPlazaCamera_SetTargetWithOffset
	.public WiFiPlazaCamera_SetFollowObject
	.public WiFiPlazaCamera_ClearFollowObject
	.public WiFiPlazaModelSet_VBlank
	.public WiFiPlazaModelSet_RemoveMover
	.public WiFiPlazaMover_SetVisible
	.public WiFiPlazaMover_StartAnim1
	.public WiFiPlazaMover_StartAnim3
	.public WiFiPlazaMover_SetAngle
	.public WiFiPlazaMover_IsAnim1Playing
	.public WiFiPlazaMover_IsAnim3Playing
	.public WiFiPlazaProp_GetKind
	.public WiFiPlazaProp_SetVisible
	.public WiFiPlazaProp_SetAnimSpeed
	.public WiFiPlazaProp_SetAlpha
	.public WiFiPlazaProp_ClearAlpha
	.public WiFiPlazaModels_FreeLayout
	.public WiFiPlazaModels_FreeModel
	.public WiFiPlazaModels_RewindFrame
	.public WiFiPlazaModels_Draw
	.public WiFiPlazaModels_VBlank
	.public WiFiPlazaModels_StartBaseAnim4
	.public WiFiPlazaModels_StopBaseAnim4
	.public WiFiPlazaModels_GetPairAnimState
	.public WiFiPlazaPropShow_Init
	.public WiFiPlazaPropShow_StartFade
	.public WiFiPlazaPropShow_Trigger
	.public WiFiPlazaPlayerState_Update
	.public WiFiPlazaPlayerState_LateUpdate
	.public WiFiPlazaPlayerState_GetArg
	.public WiFiPlazaPlayerState_GetWork
	.public WiFiPlazaPlayerState_GetStep
	.public WiFiPlazaPlayerState_SetStep
	.public WiFiPlazaPlayerState_NextStep
	.public WiFiPlazaPlayerState_Init
	.public WiFiPlazaPlayerState_SetActiveFlag
	.public WiFiPlazaPlayerState_IsSavedFree
	.public WiFiPlazaPlayerState_HasHandler
	.public WiFiPlazaPlayerStateFrame_Clear
	.public WiFiPlazaPlayerBounce_Stop
	.public WiFiPlazaPlayerBounce_IsActive
	.public WiFiPlazaPlayerState_FreeArrivalMenu
	.public WiFiPlazaGameEntry_FreeMenu
	.public WiFiPlazaGameStatus_Update
	.public WiFiPlazaGameStatus_UpdateInPlay
	.public WiFiPlazaTalk_Free
	.public WiFiPlazaTalk_StartMenuTimer
	.public WiFiPlazaTalk_StopMenuTimer
	.public WiFiPlazaTalk_CountMenuTimer
	.public WiFiPlazaTalkReply_Init
	.public WiFiPlazaTalkReply_IsDone
	.public WiFiPlazaTalkReply_GetStatus
	.public WiFiPlazaInstrumentTween_GetPosition
	.public WiFiPlazaInstrumentWave_GetValue
	.public WiFiPlazaInstrument_IsEffectActive
	.public WiFiPlazaInstrument_HasModel
	.public WiFiPlazaInstrument_InitRipple01
	.public WiFiPlazaInstrument_InitRipple02
	.public WiFiPlazaInstrument_InitRipple03
	.public WiFiPlazaInstrument_InitWhirl01
	.public WiFiPlazaInstrument_InitWhirl02
	.public WiFiPlazaInstrument_InitWhirl03
	.public WiFiPlazaInstrument_InitBalloon01
	.public WiFiPlazaInstrument_InitBalloon02
	.public WiFiPlazaInstrument_InitBalloon03
	.public WiFiPlazaInstrument_InitSparkle01
	.public WiFiPlazaInstrument_InitSparkle02
	.public WiFiPlazaInstrument_InitSparkle03
	.public WiFiPlazaInstrument_UpdateSignal01
	.public WiFiPlazaInstrument_UpdateSignal02
	.public WiFiPlazaInstrument_UpdateSignal03
	.public WiFiPlazaLight_Delete
	.public WiFiPlazaLight_FadeToThirdSet
	.public WiFiPlazaLight_PackColor
	.public WiFiPlazaLight_UnpackLight1
	.public WiFiPlazaEvent_GetMoverState
	.public WiFiPlazaEventEffect_UpdateNone
	.public WiFiPlazaEventEffect_UpdatePhase2
	.public WiFiPlazaEventEffect_StartHideProps
	.public WiFiPlazaEventEffect_StartPhase2
	.public WiFiPlazaEventEffect_StartPhase3
	.public WiFiPlazaEventEffect_StartPhase4
	.public WiFiPlazaEventMovers_Delete
	.public WiFiPlazaEventWobble_Step
	.public WiFiPlazaEvent_UpdateMoverMarkers

.text
	.rodata

sWiFiPlazaWalkUpActionTiles:
	.byte 0x27, 0x28, 0x29, 0x00

sWiFiPlazaPropTiles: ; 0x02269628
	.byte 0x02, 0x03, 0x04, 0x05

sWiFiPlazaCheckActionTiles: ; 0x0226962C
	.byte 0x24, 0x25, 0x26, 0x27
	.byte 0x28, 0x29, 0x2B, 0x00

sWiFiPlazaStandActionTiles: ; 0x02269634
	.byte 0x01, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
	.byte 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
	.byte 0x21, 0x22, 0x23, 0x2C

ov49_02269654: ; 0x02269654
	.byte 0x03, 0x02, 0x01, 0x00

ov49_02269658: ; 0x02269658
	.byte 0x20, 0x40, 0x80, 0x90

ov49_0226965C: ; 0x0226965C
	.byte 0x00, 0x01, 0x02, 0x03

ov49_02269660: ; 0x02269660
	.byte 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaObjMapSpawns: ; 0x02269678
	.byte 0x40, 0x02, 0x1E, 0x01, 0xFF, 0x00, 0x00, 0x00
	.byte 0x41, 0x01, 0x1E, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x42, 0x01, 0x1D, 0x01, 0xFF, 0x00, 0x00, 0x00
	.byte 0x61, 0x01, 0x1D, 0x01, 0xFE, 0x00, 0x00, 0x00

sWiFiPlazaObjExitHandlers: ; 0x02269698
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaObj_HandlerResetHeight

sWiFiPlazaObjHandlers: ; 0x022696C0
	.word WiFiPlazaObj_HandlerIdle
	.word WiFiPlazaObj_HandlerPlayerInput
	.word WiFiPlazaObj_HandlerWander
	.word WiFiPlazaObj_HandlerRiseOut
	.word WiFiPlazaObj_HandlerDropIn
	.word WiFiPlazaObj_HandlerSlideUp
	.word WiFiPlazaObj_HandlerSlideDown
	.word WiFiPlazaObj_HandlerSlideLeft
	.word WiFiPlazaObj_HandlerSlideRight
	.word WiFiPlazaObj_HandlerHop

sWiFiPlazaListArrowY: ; 0x022696E8
	.byte 0x14, 0x88, 0x00, 0x00

ov49_022696EC: ; 0x022696EC
	.byte 0x00, 0x01, 0x00, 0x00

ov49_022696F0: ; 0x022696F0
	.byte 0x00, 0x09, 0x06, 0x00

sWiFiPlazaPanelButtonHitbox: ; 0x022696F4
	.byte 0x20, 0xA0, 0x28, 0xD8

ov49_022696F8: ; 0x022696F8
	.byte 0xE4, 0x02, 0xEC, 0x02

ov49_022696FC: ; 0x022696FC
	.byte 0x03, 0x00, 0x69, 0x00
	.byte 0x34, 0x00, 0x38, 0x00

ov49_02269704: ; 0x02269704
	.byte 0x02, 0x00, 0x68, 0x00, 0x33, 0x00, 0x37, 0x00

sWiFiPlazaPanelButtonWindowTemplate: ; 0x0226970C
	.byte 0x05, 0x01, 0x00, 0x18
	.byte 0x03, 0x05, 0xD0, 0x01

sWiFiPlazaRoomMsgBanks: ; 0x02269714
	.byte 0x08, 0x03, 0x00, 0x00, 0x09, 0x03, 0x00, 0x00, 0x20, 0x03, 0x00, 0x00
	.byte 0xFE, 0x02, 0x00, 0x00

sWiFiPlazaRoomGraphicsModes: ; 0x02269724
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00

sWiFiPlazaRoomBgIds: ; 0x02269734
	.byte 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00

sWiFiPlazaRoomCharTransfer: ; 0x02269744
	.byte 0x18, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
	.byte 0x77, 0x00, 0x00, 0x00

sWiFiPlazaRoomEdgeColors: ; 0x02269754
	.byte 0x00, 0x00, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10
	.byte 0x84, 0x10, 0x84, 0x10

ov49_02269764: ; 0x02269764
	.byte 0x02, 0x00, 0x32, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x03, 0x00, 0x36, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF

ov49_02269774: ; 0x02269774
	.byte 0xD7, 0x00, 0x00, 0x00, 0x08, 0x00, 0xD1, 0x00, 0x66, 0x00, 0x02, 0x00
	.byte 0xD7, 0x00, 0x31, 0x00, 0x01, 0x00, 0xD7, 0x00, 0x35, 0x00, 0x01, 0x00

ov49_0226978C: ; 0x0226978C
	.byte 0xE4, 0x02, 0xE5, 0x02
	.byte 0xE6, 0x02, 0xE7, 0x02, 0xE8, 0x02, 0xE9, 0x02, 0xEA, 0x02, 0xEB, 0x02, 0xEC, 0x02, 0xED, 0x02
	.byte 0xEE, 0x02, 0xEF, 0x02, 0xF0, 0x02, 0xF1, 0x02, 0xF2, 0x02, 0xF3, 0x02

sWiFiPlazaYesNoTemplate: ; 0x022697AC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaRoomBanks: ; 0x022697CC
	.byte 0x02, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00

sWiFiPlazaCardWindowTemplates: ; 0x022697F4
	.byte 0x06, 0x08, 0x02, 0x10, 0x04, 0x05, 0x40, 0x01, 0x06, 0x01, 0x07, 0x1E
	.byte 0x06, 0x05, 0x80, 0x01, 0x06, 0x01, 0x0E, 0x13, 0x04, 0x05, 0x34, 0x02, 0x06, 0x01, 0x13, 0x13
	.byte 0x04, 0x05, 0x80, 0x02, 0x06, 0x00, 0x0E, 0x16, 0x08, 0x05, 0x34, 0x02

sWiFiPlazaRoomBgTemplates: ; 0x0226981C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x00, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaCardAvatars: ; 0x0226988C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x21, 0x00, 0x23, 0x00, 0x61, 0x00, 0x01, 0x00, 0x26, 0x00, 0x24, 0x00, 0x03, 0x00, 0x3C, 0x00
	.byte 0x26, 0x00, 0x19, 0x00, 0x05, 0x00, 0x06, 0x00, 0x2C, 0x00, 0x20, 0x00, 0x0B, 0x00, 0x18, 0x00
	.byte 0x26, 0x00, 0x2C, 0x00, 0x1F, 0x00, 0x39, 0x00, 0x28, 0x00, 0x28, 0x00, 0x32, 0x00, 0x30, 0x00
	.byte 0x2C, 0x00, 0x1A, 0x00, 0x33, 0x00, 0x0E, 0x00, 0x2C, 0x00, 0x25, 0x00, 0x3E, 0x00, 0x20, 0x00
	.byte 0x1D, 0x00, 0x28, 0x00, 0x46, 0x00, 0x31, 0x00, 0x2A, 0x00, 0x2C, 0x00, 0x06, 0x00, 0x03, 0x00
	.byte 0x27, 0x00, 0x22, 0x00, 0x07, 0x00, 0x0A, 0x00, 0x28, 0x00, 0x24, 0x00, 0x0D, 0x00, 0x24, 0x00
	.byte 0x26, 0x00, 0x29, 0x00, 0x0E, 0x00, 0x19, 0x00, 0x24, 0x00, 0x2A, 0x00, 0x23, 0x00, 0x55, 0x00
	.byte 0x26, 0x00, 0x26, 0x00, 0x25, 0x00, 0x23, 0x00, 0x26, 0x00, 0x27, 0x00, 0x2A, 0x00, 0x12, 0x00
	.byte 0x26, 0x00, 0x23, 0x00, 0x3F, 0x00, 0x21, 0x00, 0x28, 0x00, 0x2C, 0x00

sWiFiPlazaCardMonPositions: ; 0x0226991C
	.byte 0x00, 0x00, 0x02, 0x00
	.byte 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x19, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
	.byte 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x09, 0x00, 0x00, 0x00, 0x19, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00
	.byte 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x1B, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x80, 0x09, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00

ov49_022699AC: ; 0x022699AC
	.byte 0x00, 0x00, 0x01, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x06, 0x04, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x01, 0x07, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x02, 0x0A, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x06, 0x0D, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x05, 0x10, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x03, 0x13, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x04, 0x16, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x00, 0x19, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x07, 0x1C, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x04, 0x1F, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x01, 0x22, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x05, 0x25, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x05, 0x28, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x02, 0x2B, 0x00
	.byte 0xF8, 0xFF, 0xFA, 0xFF, 0x00, 0x03, 0x2E, 0x00, 0xF8, 0xFF, 0xFA, 0xFF, 0x01, 0x00, 0x67, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x6D, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x73, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x79, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaCameraAngle: ; 0x02269A6C
	.byte 0x02, 0xD6, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWiFiPlazaPropTagShiftX: ; 0x02269A74
	.byte 0xF5, 0xFF, 0xFF, 0xFF, 0x0C, 0x00, 0x00, 0x00

ov49_02269A7C: ; 0x02269A7C
	.byte 0xFE, 0xFF

ov49_02269A7E: ; 0x02269A7E
	.byte 0x0A, 0x00
	.byte 0x0E, 0x00, 0x0A, 0x00, 0x1E, 0x00, 0x0A, 0x00

sWiFiPlazaMoverSpawns: ; 0x02269A88
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00

sWiFiPlazaPropSpawns: ; 0x02269AAC
	.byte 0x18, 0x00, 0x00, 0x00
	.byte 0x19, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x03, 0x00, 0x1B, 0x00, 0x03, 0x00
	.byte 0x1C, 0x00, 0x04, 0x00, 0x21, 0x00, 0x05, 0x00, 0x46, 0x00, 0x06, 0x00, 0x47, 0x00, 0x07, 0x00
	.byte 0x48, 0x00, 0x08, 0x00, 0x4C, 0x00, 0x09, 0x00, 0x49, 0x00, 0x0A, 0x00, 0x4A, 0x00, 0x0A, 0x00
	.byte 0x4B, 0x00, 0x0A, 0x00, 0x4D, 0x00, 0x0B, 0x00, 0x4E, 0x00, 0x0C, 0x00, 0x4D, 0x00, 0x0D, 0x00
	.byte 0x4E, 0x00, 0x0D, 0x00, 0x4D, 0x00, 0x0E, 0x00, 0x4E, 0x00, 0x0E, 0x00, 0x19, 0x00, 0x0F, 0x00
	.byte 0x1A, 0x00, 0x0F, 0x00, 0x1B, 0x00, 0x0F, 0x00, 0x1B, 0x00, 0x0F, 0x00, 0x1C, 0x00, 0x0F, 0x00
	.byte 0x19, 0x00, 0x10, 0x00, 0x1A, 0x00, 0x10, 0x00, 0x1B, 0x00, 0x10, 0x00, 0x1B, 0x00, 0x10, 0x00
	.byte 0x1C, 0x00, 0x10, 0x00, 0x5C, 0x00, 0x0D, 0x00, 0x5D, 0x00, 0x0D, 0x00, 0x5C, 0x00, 0x0E, 0x00
	.byte 0x5D, 0x00, 0x0E, 0x00, 0x62, 0x00, 0x11, 0x00

sWiFiPlazaPlayerStateOwnWalk: ; 0x02269B38
	.word WiFiPlazaPlayerState_OwnWalk
	.word WiFiPlazaPlayerState_OwnWalkLate

sWiFiPlazaPlayerStateOtherPresent: ; 0x02269B40
	.word WiFiPlazaPlayerState_OtherPresent, 0

sWiFiPlazaPlayerStateOtherLeave: ; 0x02269B48
	.word WiFiPlazaPlayerState_OtherLeave, 0

sWiFiPlazaPlayerStateOtherRideMover: ; 0x02269B50
	.word WiFiPlazaPlayerState_OtherRideMover, 0

sWiFiPlazaPlayerStateOtherActivity: ; 0x02269B58
	.word WiFiPlazaPlayerState_OtherDoActivity, 0

sWiFiPlazaPlayerStateTalkStart: ; 0x02269B60
	.word WiFiPlazaTalk_Start, 0

sWiFiPlazaPlayerStateGameEntryEndLink: ; 0x02269B68
	.word WiFiPlazaGameEntry_EndLink, 0

sWiFiPlazaPlayerStateTalkAnswer: ; 0x02269B70
	.word WiFiPlazaTalk_Answer, 0

sWiFiPlazaPlayerStateOtherWait: ; 0x02269B78
	.word WiFiPlazaPlayerState_OtherWaitForPlayer, 0

sWiFiPlazaPlayerStatePlayInstrument: ; 0x02269B80
	.word WiFiPlazaPlayerState_PlayInstrument, 0

sWiFiPlazaPlayerStateGameEntryRegister: ; 0x02269B88
	.word WiFiPlazaGameEntry_Register, 0

ov49_02269B90: ; 0x02269B90
	.byte 0x4F, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00

ov49_02269BA0: ; 0x02269BA0
	.byte 0x57, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00, 0x00

ov49_02269BB0: ; 0x02269BB0
	.byte 0x32, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00

ov49_02269BC0: ; 0x02269BC0
	.byte 0x53, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00

ov49_02269BD0: ; 0x02269BD0
	.byte 0x36, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00

sWiFiPlazaPlayerStateOwnEntries: ; 0x02269BE0
	.word WiFiPlazaPlayerState_OwnArrival, 0
	.word WiFiPlazaPlayerState_OwnReturnFromAttraction, 0
	.word WiFiPlazaPlayerState_OwnReturnFromMinigame, 0
	.word WiFiPlazaPlayerState_OwnReturnFromSurvey, 0

sWiFiPlazaArrivalMenuTemplate: ; 0x02269C00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x12, 0x00, 0x07, 0x00, 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00

ov49_02269C20: ; 0x02269C20
	.byte 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00
	.byte 0x26, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00

ov49_02269C40: ; 0x02269C40
	.byte 0x2A, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00
	.byte 0x2E, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00

sWiFiPlazaWalkUpTileActions: ; 0x02269C60
	.byte 0x27, 0x28, 0x29, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaGameEntry_ReadSign
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.word 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaCheckTileActions: ; 0x02269C90
	.byte 0x24, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaTalk_Greeter
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x25, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaTalk_InfoCounter
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x26, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaTalk_Reception
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x27, 0x28, 0x29, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaGameEntry_ReadSign
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x2B, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.word WiFiPlazaTalk_Survey
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.word 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaStandTileActions: ; 0x02269D20
	.byte 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnExit
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x16, 0x17, 0x18, 0x19, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnEnterAttraction
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1A, 0x1B, 0x1C, 0x1D, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnEnterAttraction
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnEnterAttraction
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnEnterAttraction
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x21, 0x22, 0x23, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnRideMover
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1E, 0x1F, 0x20, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnEnterMinigame
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x2C, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word WiFiPlazaPlayerState_OwnUseSurveyMonitor
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.word 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaGameStatusHeights: ; 0x02269DF8
	.byte 0x08, 0x0A, 0x00, 0x00

sWiFiPlazaGameEntryMenuTemplate: ; 0x02269DFC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaInfoCounterMsgs: ; 0x02269E1C
	.byte 0x1F, 0x20, 0x21, 0x22
	.byte 0x23, 0x28, 0x24, 0x25

sWiFiPlazaTalkMenuTemplate: ; 0x02269E24
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWiFiPlazaTalkGreetingMsgs: ; 0x02269E44
	.byte 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00
	.byte 0x1F, 0x00, 0x1F, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x05, 0x00, 0x07, 0x00
	.byte 0x01, 0x00, 0x09, 0x00, 0x20, 0x00, 0x02, 0x00, 0x20, 0x00, 0x0B, 0x00, 0x0D, 0x00, 0x0F, 0x00
	.byte 0x20, 0x00, 0x11, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x21, 0x00, 0x13, 0x00, 0x15, 0x00
	.byte 0x0C, 0x00, 0x17, 0x00, 0x0E, 0x00, 0x06, 0x00, 0x0E, 0x00, 0x14, 0x00, 0x22, 0x00, 0x19, 0x00
	.byte 0x0E, 0x00, 0x1B, 0x00, 0x10, 0x00, 0x08, 0x00, 0x10, 0x00, 0x16, 0x00, 0x1A, 0x00, 0x23, 0x00
	.byte 0x10, 0x00, 0x1D, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00
	.byte 0x1F, 0x00, 0x1F, 0x00, 0x12, 0x00, 0x0A, 0x00, 0x12, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x1E, 0x00
	.byte 0x12, 0x00, 0x24, 0x00

sWiFiPlazaTalkMsgPairs: ; 0x02269EC4
	.byte 0x28, 0x00

sWiFiPlazaTalkMsgPairsHighlighted: ; 0x02269EC6
	.byte 0x25, 0x02, 0x29, 0x00, 0x26, 0x02, 0x2B, 0x00, 0x27, 0x02
	.byte 0x2C, 0x00, 0x28, 0x02, 0x2E, 0x00, 0x2A, 0x02, 0x2F, 0x00, 0x2B, 0x02, 0x30, 0x00, 0x2C, 0x02
	.byte 0x31, 0x00, 0x2D, 0x02, 0x32, 0x00, 0x2E, 0x02, 0x33, 0x00, 0x2F, 0x02, 0x34, 0x00, 0x30, 0x02
	.byte 0x35, 0x00, 0x31, 0x02, 0x36, 0x00, 0x32, 0x02, 0x37, 0x00, 0x33, 0x02, 0x38, 0x00, 0x34, 0x02
	.byte 0x39, 0x00, 0x35, 0x02, 0x3A, 0x00, 0x36, 0x02, 0x3B, 0x00, 0x37, 0x02, 0x3C, 0x00, 0x38, 0x02
	.byte 0x3D, 0x00, 0x39, 0x02, 0x3E, 0x00, 0x3A, 0x02, 0x3F, 0x00, 0x3B, 0x02, 0x40, 0x00, 0x3C, 0x02
	.byte 0x55, 0x00, 0x3D, 0x02, 0x56, 0x00, 0x3E, 0x02, 0x57, 0x00, 0x3F, 0x02, 0x58, 0x00, 0x40, 0x02
	.byte 0x59, 0x00, 0x41, 0x02, 0x5A, 0x00, 0x42, 0x02, 0x5B, 0x00, 0x43, 0x02, 0x5C, 0x00, 0x44, 0x02
	.byte 0x5D, 0x00, 0x45, 0x02, 0x5E, 0x00, 0x46, 0x02, 0xEC, 0x00, 0x47, 0x02, 0xED, 0x00, 0x48, 0x02
	.byte 0xEE, 0x00, 0x49, 0x02, 0xEF, 0x00, 0x4A, 0x02, 0xF0, 0x00, 0x4B, 0x02, 0xF1, 0x00, 0x4C, 0x02
	.byte 0xF2, 0x00, 0x4D, 0x02, 0xF3, 0x00, 0x4E, 0x02, 0xF4, 0x00, 0x4F, 0x02, 0xF5, 0x00, 0x50, 0x02
	.byte 0xF6, 0x00, 0x51, 0x02, 0xF7, 0x00, 0x52, 0x02, 0xF8, 0x00, 0x53, 0x02, 0xF9, 0x00, 0x54, 0x02
	.byte 0xFA, 0x00, 0x55, 0x02, 0xFB, 0x00, 0x56, 0x02, 0xFC, 0x00, 0x57, 0x02, 0xFD, 0x00, 0x58, 0x02
	.byte 0xFE, 0x00, 0x59, 0x02, 0xFF, 0x00, 0x5A, 0x02, 0x00, 0x01, 0x5B, 0x02, 0x01, 0x01, 0x5C, 0x02
	.byte 0x02, 0x01, 0x5D, 0x02, 0x03, 0x01, 0x5E, 0x02, 0x04, 0x01, 0x5F, 0x02, 0x05, 0x01, 0x60, 0x02
	.byte 0x06, 0x01, 0x61, 0x02, 0x07, 0x01, 0x62, 0x02, 0x08, 0x01, 0x63, 0x02, 0x09, 0x01, 0x64, 0x02
	.byte 0x0A, 0x01, 0x65, 0x02, 0x0B, 0x01, 0x66, 0x02, 0x0C, 0x01, 0x67, 0x02, 0x0D, 0x01, 0x68, 0x02
	.byte 0x0E, 0x01, 0x69, 0x02, 0x0F, 0x01, 0x6A, 0x02, 0x10, 0x01, 0x6B, 0x02, 0x11, 0x01, 0x6C, 0x02
	.byte 0x12, 0x01, 0x6D, 0x02, 0x13, 0x01, 0x6E, 0x02, 0x64, 0x01, 0x6F, 0x02, 0x65, 0x01, 0x70, 0x02
	.byte 0x66, 0x01, 0x71, 0x02, 0x67, 0x01, 0x72, 0x02, 0x68, 0x01, 0x73, 0x02, 0x69, 0x01, 0x74, 0x02
	.byte 0x6A, 0x01, 0x75, 0x02, 0x6B, 0x01, 0x76, 0x02, 0x6C, 0x01, 0x77, 0x02, 0x6D, 0x01, 0x78, 0x02
	.byte 0x6E, 0x01, 0x79, 0x02, 0x6F, 0x01, 0x7A, 0x02, 0x70, 0x01, 0x7B, 0x02, 0x71, 0x01, 0x7C, 0x02
	.byte 0x72, 0x01, 0x7D, 0x02, 0x73, 0x01, 0x7E, 0x02, 0x74, 0x01, 0x7F, 0x02, 0x75, 0x01, 0x80, 0x02
	.byte 0x76, 0x01, 0x81, 0x02, 0x77, 0x01, 0x82, 0x02, 0x78, 0x01, 0x83, 0x02, 0x79, 0x01, 0x84, 0x02
	.byte 0x7A, 0x01, 0x85, 0x02, 0x7B, 0x01, 0x86, 0x02, 0x7C, 0x01, 0x87, 0x02, 0x7D, 0x01, 0x88, 0x02
	.byte 0x7E, 0x01, 0x89, 0x02, 0x7F, 0x01, 0x8A, 0x02, 0x80, 0x01, 0x8B, 0x02, 0x81, 0x01, 0x8C, 0x02
	.byte 0x82, 0x01, 0x8D, 0x02, 0x83, 0x01, 0x8E, 0x02, 0x84, 0x01, 0x8F, 0x02, 0x85, 0x01, 0x90, 0x02
	.byte 0x86, 0x01, 0x91, 0x02, 0x87, 0x01, 0x92, 0x02, 0x88, 0x01, 0x93, 0x02, 0x89, 0x01, 0x94, 0x02
	.byte 0x8A, 0x01, 0x95, 0x02, 0x8B, 0x01, 0x96, 0x02, 0x8C, 0x01, 0x97, 0x02, 0xDD, 0x01, 0x98, 0x02
	.byte 0xDE, 0x01, 0x99, 0x02, 0xDF, 0x01, 0x9A, 0x02, 0xE0, 0x01, 0x9B, 0x02, 0xE1, 0x01, 0x9C, 0x02
	.byte 0xE2, 0x01, 0x9D, 0x02, 0xE3, 0x01, 0x9E, 0x02, 0xE4, 0x01, 0x9F, 0x02, 0xE5, 0x01, 0xA0, 0x02
	.byte 0xE6, 0x01, 0xA1, 0x02, 0xFB, 0x01, 0xA2, 0x02, 0xFC, 0x01, 0xA3, 0x02, 0xFD, 0x01, 0xA4, 0x02
	.byte 0xFE, 0x01, 0xA5, 0x02, 0xFF, 0x01, 0xA6, 0x02, 0x00, 0x02, 0xA7, 0x02, 0x01, 0x02, 0x8D, 0x03
	.byte 0x02, 0x02, 0xA8, 0x02, 0xAB, 0x02, 0xAD, 0x02, 0xAC, 0x02, 0xAE, 0x02, 0xAF, 0x02, 0xB0, 0x02
	.byte 0x41, 0x00, 0xB1, 0x02, 0x42, 0x00, 0xB2, 0x02, 0x43, 0x00, 0xB3, 0x02, 0x44, 0x00, 0xB4, 0x02
	.byte 0x45, 0x00, 0xB5, 0x02, 0x46, 0x00, 0xB6, 0x02, 0x47, 0x00, 0xB7, 0x02, 0x48, 0x00, 0xB8, 0x02
	.byte 0x49, 0x00, 0xB9, 0x02, 0x4A, 0x00, 0xBA, 0x02, 0x4B, 0x00, 0xBB, 0x02, 0x4C, 0x00, 0xBC, 0x02
	.byte 0x4D, 0x00, 0xBD, 0x02, 0x4E, 0x00, 0xBE, 0x02, 0x4F, 0x00, 0xBF, 0x02, 0x50, 0x00, 0xC0, 0x02
	.byte 0x51, 0x00, 0xC1, 0x02, 0x52, 0x00, 0xC2, 0x02, 0x53, 0x00, 0xC3, 0x02, 0x54, 0x00, 0xC4, 0x02
	.byte 0x5F, 0x00, 0xC5, 0x02, 0x60, 0x00, 0xC6, 0x02, 0x61, 0x00, 0xC7, 0x02, 0x62, 0x00, 0xC8, 0x02
	.byte 0x63, 0x00, 0xC9, 0x02, 0x64, 0x00, 0xCA, 0x02, 0x65, 0x00, 0xCB, 0x02, 0x66, 0x00, 0xCC, 0x02
	.byte 0x67, 0x00, 0xCD, 0x02, 0x68, 0x00, 0xCE, 0x02, 0x69, 0x00, 0xCF, 0x02, 0x6A, 0x00, 0xD0, 0x02
	.byte 0x6B, 0x00, 0xD1, 0x02, 0x6C, 0x00, 0xD2, 0x02, 0x6D, 0x00, 0xD3, 0x02, 0x6E, 0x00, 0xD4, 0x02
	.byte 0x6F, 0x00, 0xD5, 0x02, 0x70, 0x00, 0xD6, 0x02, 0x71, 0x00, 0xD7, 0x02, 0x72, 0x00, 0xD8, 0x02
	.byte 0x14, 0x01, 0xD9, 0x02, 0x15, 0x01, 0xDA, 0x02, 0x16, 0x01, 0xDB, 0x02, 0x17, 0x01, 0xDC, 0x02
	.byte 0x18, 0x01, 0xDD, 0x02, 0x19, 0x01, 0xDE, 0x02, 0x1A, 0x01, 0xDF, 0x02, 0x1B, 0x01, 0xE0, 0x02
	.byte 0x1C, 0x01, 0xE1, 0x02, 0x1D, 0x01, 0xE2, 0x02, 0x1E, 0x01, 0xE3, 0x02, 0x1F, 0x01, 0xE4, 0x02
	.byte 0x20, 0x01, 0xE5, 0x02, 0x21, 0x01, 0xE6, 0x02, 0x22, 0x01, 0xE7, 0x02, 0x23, 0x01, 0xE8, 0x02
	.byte 0x24, 0x01, 0xE9, 0x02, 0x25, 0x01, 0xEA, 0x02, 0x26, 0x01, 0xEB, 0x02, 0x27, 0x01, 0xEC, 0x02
	.byte 0x28, 0x01, 0xED, 0x02, 0x29, 0x01, 0xEE, 0x02, 0x2A, 0x01, 0xEF, 0x02, 0x2B, 0x01, 0xF0, 0x02
	.byte 0x2C, 0x01, 0xF1, 0x02, 0x2D, 0x01, 0xF2, 0x02, 0x2E, 0x01, 0xF3, 0x02, 0x2F, 0x01, 0xF4, 0x02
	.byte 0x30, 0x01, 0xF5, 0x02, 0x31, 0x01, 0xF6, 0x02, 0x32, 0x01, 0xF7, 0x02, 0x33, 0x01, 0xF8, 0x02
	.byte 0x34, 0x01, 0xF9, 0x02, 0x35, 0x01, 0xFA, 0x02, 0x36, 0x01, 0xFB, 0x02, 0x37, 0x01, 0xFC, 0x02
	.byte 0x38, 0x01, 0xFD, 0x02, 0x39, 0x01, 0xFE, 0x02, 0x3A, 0x01, 0xFF, 0x02, 0x3B, 0x01, 0x00, 0x03
	.byte 0x3C, 0x01, 0x01, 0x03, 0x3D, 0x01, 0x02, 0x03, 0x3E, 0x01, 0x03, 0x03, 0x3F, 0x01, 0x04, 0x03
	.byte 0x40, 0x01, 0x05, 0x03, 0x41, 0x01, 0x06, 0x03, 0x42, 0x01, 0x07, 0x03, 0x43, 0x01, 0x08, 0x03
	.byte 0x44, 0x01, 0x09, 0x03, 0x45, 0x01, 0x0A, 0x03, 0x46, 0x01, 0x0B, 0x03, 0x47, 0x01, 0x0C, 0x03
	.byte 0x48, 0x01, 0x0D, 0x03, 0x49, 0x01, 0x0E, 0x03, 0x4A, 0x01, 0x0F, 0x03, 0x4B, 0x01, 0x10, 0x03
	.byte 0x4C, 0x01, 0x11, 0x03, 0x4D, 0x01, 0x12, 0x03, 0x4E, 0x01, 0x13, 0x03, 0x4F, 0x01, 0x14, 0x03
	.byte 0x50, 0x01, 0x15, 0x03, 0x51, 0x01, 0x16, 0x03, 0x52, 0x01, 0x17, 0x03, 0x53, 0x01, 0x18, 0x03
	.byte 0x54, 0x01, 0x19, 0x03, 0x55, 0x01, 0x1A, 0x03, 0x56, 0x01, 0x1B, 0x03, 0x57, 0x01, 0x1C, 0x03
	.byte 0x58, 0x01, 0x1D, 0x03, 0x59, 0x01, 0x1E, 0x03, 0x5A, 0x01, 0x1F, 0x03, 0x5B, 0x01, 0x20, 0x03
	.byte 0x5C, 0x01, 0x21, 0x03, 0x5D, 0x01, 0x22, 0x03, 0x5E, 0x01, 0x23, 0x03, 0x5F, 0x01, 0x24, 0x03
	.byte 0x60, 0x01, 0x25, 0x03, 0x61, 0x01, 0x26, 0x03, 0x62, 0x01, 0x27, 0x03, 0x63, 0x01, 0x28, 0x03
	.byte 0x8D, 0x01, 0x29, 0x03, 0x8E, 0x01, 0x2A, 0x03, 0x8F, 0x01, 0x2B, 0x03, 0x90, 0x01, 0x2C, 0x03
	.byte 0x91, 0x01, 0x2D, 0x03, 0x92, 0x01, 0x2E, 0x03, 0x93, 0x01, 0x2F, 0x03, 0x94, 0x01, 0x30, 0x03
	.byte 0x95, 0x01, 0x31, 0x03, 0x96, 0x01, 0x32, 0x03, 0x97, 0x01, 0x33, 0x03, 0x98, 0x01, 0x34, 0x03
	.byte 0x99, 0x01, 0x35, 0x03, 0x9A, 0x01, 0x36, 0x03, 0x9B, 0x01, 0x37, 0x03, 0x9C, 0x01, 0x38, 0x03
	.byte 0x9D, 0x01, 0x39, 0x03, 0x9E, 0x01, 0x3A, 0x03, 0x9F, 0x01, 0x3B, 0x03, 0xA0, 0x01, 0x3C, 0x03
	.byte 0xA1, 0x01, 0x3D, 0x03, 0xA2, 0x01, 0x3E, 0x03, 0xA3, 0x01, 0x3F, 0x03, 0xA4, 0x01, 0x40, 0x03
	.byte 0xA5, 0x01, 0x41, 0x03, 0xA6, 0x01, 0x42, 0x03, 0xA7, 0x01, 0x43, 0x03, 0xA8, 0x01, 0x44, 0x03
	.byte 0xA9, 0x01, 0x45, 0x03, 0xAA, 0x01, 0x46, 0x03, 0xAB, 0x01, 0x47, 0x03, 0xAC, 0x01, 0x48, 0x03
	.byte 0xAD, 0x01, 0x49, 0x03, 0xAE, 0x01, 0x4A, 0x03, 0xAF, 0x01, 0x4B, 0x03, 0xB0, 0x01, 0x4C, 0x03
	.byte 0xB1, 0x01, 0x4D, 0x03, 0xB2, 0x01, 0x4E, 0x03, 0xB3, 0x01, 0x4F, 0x03, 0xB4, 0x01, 0x50, 0x03
	.byte 0xB5, 0x01, 0x51, 0x03, 0xB6, 0x01, 0x52, 0x03, 0xB7, 0x01, 0x53, 0x03, 0xB8, 0x01, 0x54, 0x03
	.byte 0xB9, 0x01, 0x55, 0x03, 0xBA, 0x01, 0x56, 0x03, 0xBB, 0x01, 0x57, 0x03, 0xBC, 0x01, 0x58, 0x03
	.byte 0xBD, 0x01, 0x59, 0x03, 0xBE, 0x01, 0x5A, 0x03, 0xBF, 0x01, 0x5B, 0x03, 0xC0, 0x01, 0x5C, 0x03
	.byte 0xC1, 0x01, 0x5D, 0x03, 0xC2, 0x01, 0x5E, 0x03, 0xC3, 0x01, 0x5F, 0x03, 0xC4, 0x01, 0x60, 0x03
	.byte 0xC5, 0x01, 0x61, 0x03, 0xC6, 0x01, 0x62, 0x03, 0xC7, 0x01, 0x63, 0x03, 0xC8, 0x01, 0x64, 0x03
	.byte 0xC9, 0x01, 0x65, 0x03, 0xCA, 0x01, 0x66, 0x03, 0xCB, 0x01, 0x67, 0x03, 0xCC, 0x01, 0x68, 0x03
	.byte 0xCD, 0x01, 0x69, 0x03, 0xCE, 0x01, 0x6A, 0x03, 0xCF, 0x01, 0x6B, 0x03, 0xD0, 0x01, 0x6C, 0x03
	.byte 0xD1, 0x01, 0x6D, 0x03, 0xD2, 0x01, 0x6E, 0x03, 0xD3, 0x01, 0x6F, 0x03, 0xD4, 0x01, 0x70, 0x03
	.byte 0xD5, 0x01, 0x71, 0x03, 0xD6, 0x01, 0x72, 0x03, 0xD7, 0x01, 0x73, 0x03, 0xD8, 0x01, 0x74, 0x03
	.byte 0xD9, 0x01, 0x75, 0x03, 0xDA, 0x01, 0x76, 0x03, 0xDB, 0x01, 0x77, 0x03, 0xDC, 0x01, 0x78, 0x03
	.byte 0xE7, 0x01, 0x79, 0x03, 0xE8, 0x01, 0x7A, 0x03, 0xE9, 0x01, 0x7B, 0x03, 0xEA, 0x01, 0x7C, 0x03
	.byte 0xEB, 0x01, 0x7D, 0x03, 0xEC, 0x01, 0x7E, 0x03, 0xED, 0x01, 0x7F, 0x03, 0xEE, 0x01, 0x80, 0x03
	.byte 0xEF, 0x01, 0x81, 0x03, 0xF0, 0x01, 0x82, 0x03, 0xF1, 0x01, 0x83, 0x03, 0xF2, 0x01, 0x84, 0x03
	.byte 0xF3, 0x01, 0x85, 0x03, 0xF4, 0x01, 0x86, 0x03, 0xF5, 0x01, 0x87, 0x03, 0xF6, 0x01, 0x88, 0x03
	.byte 0xF7, 0x01, 0x89, 0x03, 0xF8, 0x01, 0x8A, 0x03, 0xF9, 0x01, 0x8B, 0x03, 0xFA, 0x01, 0x8C, 0x03

sWiFiPlazaInstrumentBalloonHeights: ; 0x0226A450
	.byte 0x30, 0x54, 0x80, 0x00

sWiFiPlazaInstrumentBursts_Bell01: ; 0x0226A454
	.byte 0x00, 0x00, 0x00, 0x02, 0x18, 0x00, 0x02, 0x02

sWiFiPlazaInstrumentBursts_Cymbal02: ; 0x0226A45C
	.byte 0x00, 0x00, 0x00, 0x02
	.byte 0x18, 0x00, 0x02, 0x02

sWiFiPlazaInstrumentBursts_Drum01: ; 0x0226A464
	.byte 0x00, 0x00, 0x00, 0x02, 0x18, 0x00, 0x02, 0x02

sWiFiPlazaInstrumentBursts_Bell02: ; 0x0226A46C
	.byte 0x00, 0x00, 0x00, 0x03
	.byte 0x18, 0x00, 0x02, 0x03

sWiFiPlazaInstrumentBursts_Cymbal03: ; 0x0226A474
	.byte 0x00, 0x00, 0x00, 0x04, 0x14, 0x00, 0x02, 0x04

sWiFiPlazaInstrumentBursts_Cymbal01: ; 0x0226A47C
	.byte 0x00, 0x00, 0x00, 0x02
	.byte 0x18, 0x00, 0x01, 0x02

sWiFiPlazaInstrumentBurstUpdateFuncs: ; 0x0226A484
	.word WiFiPlazaInstrumentBurst_UpdateBell
	.word WiFiPlazaInstrumentBurst_UpdateDrum
	.word WiFiPlazaInstrumentBurst_UpdateCymbal
	.byte 0x02, 0x00, 0x38, 0x00, 0x0E, 0x00, 0x38, 0x00, 0x08, 0x00, 0x3C, 0x00

sWiFiPlazaInstrumentBursts_Drum02: ; 0x0226A49C
	.byte 0x00, 0x00, 0x00, 0x03
	.byte 0x10, 0x00, 0x02, 0x03, 0x28, 0x00, 0x00, 0x03, 0x02, 0x00, 0x2C, 0x00, 0x0E, 0x00, 0x2C, 0x00
	.byte 0x08, 0x00, 0x30, 0x00

sWiFiPlazaInstrumentBursts_Bell03: ; 0x0226A4B4
	.byte 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x02, 0x04, 0x20, 0x00, 0x00, 0x04

sWiFiPlazaInstrumentBursts_Drum03: ; 0x0226A4C0
	.byte 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x02, 0x04, 0x23, 0x00, 0x00, 0x04

sWiFiPlazaInstrumentBurstStartFuncs: ; 0x0226A4CC
	.word WiFiPlazaInstrumentBurst_StartBell
	.word WiFiPlazaInstrumentBurst_StartDrum
	.word WiFiPlazaInstrumentBurst_StartCymbal

sWiFiPlazaInstrumentSparkleStarts: ; 0x0226A4D8
	.byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
	.byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0xFF
	.byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xE0, 0xFE, 0xFF, 0x00, 0xE0, 0xFF, 0xFF
	.byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0xFF

sWiFiPlazaInstrumentSparkleFalls: ; 0x0226A508
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF
	.byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x80, 0xFF, 0xFF
	.byte 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00

sWiFiPlazaInstrumentUpdateFuncs: ; 0x0226A538
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateBellDrum
	.word WiFiPlazaInstrument_UpdateCymbal
	.word WiFiPlazaInstrument_UpdateCymbal
	.word WiFiPlazaInstrument_UpdateCymbal
	.word WiFiPlazaInstrument_UpdateRipple
	.word WiFiPlazaInstrument_UpdateRipple
	.word WiFiPlazaInstrument_UpdateRipple
	.word WiFiPlazaInstrument_UpdateSignal01
	.word WiFiPlazaInstrument_UpdateSignal02
	.word WiFiPlazaInstrument_UpdateSignal03
	.word WiFiPlazaInstrument_UpdateWhirl
	.word WiFiPlazaInstrument_UpdateWhirl
	.word WiFiPlazaInstrument_UpdateWhirl
	.word WiFiPlazaInstrument_UpdatePop
	.word WiFiPlazaInstrument_UpdatePop
	.word WiFiPlazaInstrument_UpdatePop
	.word WiFiPlazaInstrument_UpdateSparkle
	.word WiFiPlazaInstrument_UpdateSparkle
	.word WiFiPlazaInstrument_UpdateSparkle
	.word WiFiPlazaInstrument_UpdateBalloon
	.word WiFiPlazaInstrument_UpdateBalloon
	.word WiFiPlazaInstrument_UpdateBalloon

sWiFiPlazaInstrumentInitFuncs: ; 0x0226A5A4
	.word WiFiPlazaInstrument_InitBell01
	.word WiFiPlazaInstrument_InitBell02
	.word WiFiPlazaInstrument_InitBell03
	.word WiFiPlazaInstrument_InitDrum01
	.word WiFiPlazaInstrument_InitDrum02
	.word WiFiPlazaInstrument_InitDrum03
	.word WiFiPlazaInstrument_InitCymbal01
	.word WiFiPlazaInstrument_InitCymbal02
	.word WiFiPlazaInstrument_InitCymbal03
	.word WiFiPlazaInstrument_InitRipple01
	.word WiFiPlazaInstrument_InitRipple02
	.word WiFiPlazaInstrument_InitRipple03
	.word WiFiPlazaInstrument_InitSignal
	.word WiFiPlazaInstrument_InitSignal
	.word WiFiPlazaInstrument_InitSignal
	.word WiFiPlazaInstrument_InitWhirl01
	.word WiFiPlazaInstrument_InitWhirl02
	.word WiFiPlazaInstrument_InitWhirl03
	.word WiFiPlazaInstrument_InitPop01
	.word WiFiPlazaInstrument_InitPop02
	.word WiFiPlazaInstrument_InitPop03
	.word WiFiPlazaInstrument_InitSparkle01
	.word WiFiPlazaInstrument_InitSparkle02
	.word WiFiPlazaInstrument_InitSparkle03
	.word WiFiPlazaInstrument_InitBalloon01
	.word WiFiPlazaInstrument_InitBalloon02
	.word WiFiPlazaInstrument_InitBalloon03

sWiFiPlazaInstrumentStartSEs: ; 0x0226A610
	.byte 0x9C, 0x05, 0x00, 0x00, 0x9D, 0x05, 0x00, 0x00, 0x9E, 0x05, 0x00, 0x00, 0xA2, 0x05, 0x00, 0x00
	.byte 0xA3, 0x05, 0x00, 0x00, 0xA4, 0x05, 0x00, 0x00, 0x9F, 0x05, 0x00, 0x00, 0xA0, 0x05, 0x00, 0x00
	.byte 0xA1, 0x05, 0x00, 0x00, 0xAA, 0x05, 0x00, 0x00, 0xAB, 0x05, 0x00, 0x00, 0xAC, 0x05, 0x00, 0x00
	.byte 0xB0, 0x05, 0x00, 0x00, 0xB1, 0x05, 0x00, 0x00, 0xB2, 0x05, 0x00, 0x00, 0xA5, 0x05, 0x00, 0x00
	.byte 0xA6, 0x05, 0x00, 0x00, 0xA7, 0x05, 0x00, 0x00, 0xB6, 0x05, 0x00, 0x00, 0xB7, 0x05, 0x00, 0x00
	.byte 0xB8, 0x05, 0x00, 0x00, 0xB9, 0x05, 0x00, 0x00, 0xBA, 0x05, 0x00, 0x00, 0xBB, 0x05, 0x00, 0x00
	.byte 0xA9, 0x05, 0x00, 0x00, 0xA9, 0x05, 0x00, 0x00

ov49_0226A678: ; 0x0226A678
	.byte 0xA9, 0x05, 0x00, 0x00

ov49_0226A67C: ; 0x0226A67C
	.byte 0x00, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00
	.byte 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x00

sWiFiPlazaInstrumentModelDescs: ; 0x0226A70C
	.byte 0x00, 0x00, 0x00, 0x11
	.byte 0x00, 0x01, 0x00, 0x11, 0x00, 0x02, 0x00, 0x11, 0x01, 0x03, 0x01, 0x02, 0x01, 0x04, 0x01, 0x02
	.byte 0x01, 0x05, 0x01, 0x02, 0x01, 0x06, 0x03, 0x11, 0x01, 0x07, 0x03, 0x11, 0x01, 0x08, 0x03, 0x11

ov49_0226A730: ; 0x0226A730
	.byte 0x05, 0x0B, 0x06, 0x11

ov49_0226A734: ; 0x0226A734
	.byte 0x03, 0x09, 0x04, 0x11

ov49_0226A738: ; 0x0226A738
	.byte 0x04, 0x0A, 0x05, 0x11

ov49_0226A73C: ; 0x0226A73C
	.byte 0x06, 0x0C, 0x07, 0x11
	.byte 0x07, 0x0D, 0x08, 0x11, 0x08, 0x0E, 0x09, 0x11, 0x09, 0x0F, 0x0A, 0x11

ov49_0226A74C: ; 0x0226A74C
	.byte 0x0A, 0x10, 0x0B, 0x11

ov49_0226A750: ; 0x0226A750
	.byte 0x0B, 0x11, 0x0C, 0x11

ov49_0226A754: ; 0x0226A754
	.byte 0x0C, 0x12, 0x0D, 0x11, 0x02, 0x13, 0x11, 0x11, 0x02, 0x14, 0x11, 0x11
	.byte 0x02, 0x15, 0x11, 0x11, 0x02, 0x16, 0x11, 0x11, 0x02, 0x17, 0x11, 0x11, 0x02, 0x18, 0x11, 0x11
	.byte 0x02, 0x19, 0x11, 0x11, 0x02, 0x1A, 0x11, 0x11, 0x02, 0x1B, 0x11, 0x11, 0x02, 0x1C, 0x11, 0x11
	.byte 0x02, 0x1D, 0x11, 0x11, 0x02, 0x1E, 0x11, 0x11, 0x02, 0x1F, 0x11, 0x11, 0x02, 0x20, 0x11, 0x11
	.byte 0x02, 0x21, 0x11, 0x11, 0x02, 0x22, 0x11, 0x11, 0x02, 0x23, 0x11, 0x11, 0x02, 0x24, 0x11, 0x11
	.byte 0x0D, 0x25, 0x0E, 0x11, 0x0E, 0x26, 0x0F, 0x11

sWiFiPlazaInstrumentBurstModels01: ; 0x0226A7A8
	.byte 0x13, 0x14, 0x13, 0x14, 0x13, 0x14, 0x13, 0x14
	.byte 0x13, 0x14, 0x13, 0x14, 0x13, 0x14, 0x13, 0x14

sWiFiPlazaInstrumentBurstModels02: ; 0x0226A7B8
	.byte 0x13, 0x14, 0x15, 0x13, 0x1C, 0x1D, 0x1B, 0x19
	.byte 0x19, 0x1D, 0x14, 0x13, 0x1D, 0x1B, 0x17, 0x13

sWiFiPlazaInstrumentBurstModels03: ; 0x0226A7C8
	.byte 0x13, 0x14, 0x1F, 0x23, 0x20, 0x22, 0x1B, 0x1E
	.byte 0x24, 0x1D, 0x18, 0x21, 0x1E, 0x15, 0x22, 0x1F

sWiFiPlazaLight1Colors: ; 0x0226A7D8
	.byte 0xCE, 0x39, 0x94, 0x52, 0xFF, 0x7F, 0x08, 0x21

sWiFiPlazaLightColorSets: ; 0x0226A7E0
	.byte 0xF7, 0x66, 0x10, 0x42, 0xCE, 0x39, 0x94, 0x52, 0x10, 0x42, 0x4A, 0x41, 0x10, 0x42, 0xCE, 0x39
	.byte 0x94, 0x52, 0x10, 0x42, 0x08, 0x31, 0x10, 0x42, 0xCE, 0x39, 0x94, 0x52, 0x10, 0x42, 0xA5, 0x20
	.byte 0x10, 0x42, 0xAD, 0x39, 0x94, 0x52, 0x10, 0x42, 0x63, 0x1C, 0x10, 0x42, 0x4A, 0x31, 0x94, 0x52
	.byte 0xCE, 0x41, 0x00, 0x18, 0x10, 0x42, 0x08, 0x29, 0x94, 0x52, 0x8C, 0x41

ov49_0226A81C: ; 0x0226A81C
	.byte 0x04, 0x00, 0x00, 0x00

ov49_0226A820: ; 0x0226A820
	.byte 0x03, 0x00, 0x00, 0x00

ov49_0226A824: ; 0x0226A824
	.byte 0x0F, 0x10, 0x00, 0x00

ov49_0226A828: ; 0x0226A828
	.byte 0x09, 0x0A, 0x00, 0x00

ov49_0226A82C: ; 0x0226A82C
	.byte 0x0B, 0x0C, 0x00, 0x00

ov49_0226A830: ; 0x0226A830
	.byte 0x01, 0x02, 0x00, 0x00

ov49_0226A834: ; 0x0226A834
	.byte 0x05, 0x0D, 0x06, 0x07, 0x0E, 0x08, 0x00, 0x00

ov49_0226A83C: ; 0x0226A83C
	.byte 0x00

ov49_0226A83D: ; 0x0226A83D
	.byte 0x05, 0x01, 0x06
	.byte 0x02, 0x00, 0x03, 0x01, 0x04, 0x02, 0x05, 0x03, 0x06, 0x04, 0x00, 0x00

sWiFiPlazaEventEffectPhaseUpdates: ; 0x0226A84C
	.word WiFiPlazaEventEffect_UpdateNone
	.word WiFiPlazaEventEffect_UpdateNone
	.word WiFiPlazaEventEffect_UpdatePhase2
	.word WiFiPlazaEventEffect_UpdatePhase3
	.word WiFiPlazaEventEffect_UpdatePhase4
	.word WiFiPlazaEventEffect_UpdatePhase5

ov49_0226A864: ; 0x0226A864
	.byte 0x0A, 0x00, 0x00, 0x00

ov49_0226A868: ; 0x0226A868
	.byte 0x00, 0x10, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00

sWiFiPlazaEventEffectPhaseStarts: ; 0x0226A87C
	.word WiFiPlazaEventEffect_StartHideProps
	.word WiFiPlazaEventEffect_StartHideProps
	.word WiFiPlazaEventEffect_StartPhase2
	.word WiFiPlazaEventEffect_StartPhase3
	.word WiFiPlazaEventEffect_StartPhase4
	.word WiFiPlazaEventEffect_StartPhase5

ov49_0226A894: ; 0x0226A894
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00

ov49_0226A8B4: ; 0x0226A8B4
	.byte 0x06, 0x0D, 0x05, 0x0B, 0x07, 0x0E, 0x08, 0x0C, 0x01, 0x00, 0x02, 0x0A
	.byte 0x10, 0x09, 0x04, 0x03, 0x0F, 0x00, 0x00, 0x00

sWiFiPlazaMoverMarkerTags: ; 0x0226A8C8
	.byte 0x3F, 0x3E, 0x3D, 0x00

sWiFiPlazaEventWobbleSteps: ; 0x0226A8CC
	.word WiFiPlazaEventWobble_Step
	.word WiFiPlazaEventWobble_Step

sWiFiPlazaEventWobbleInits: ; 0x0226A8D4
	.word WiFiPlazaEventWobble_InitSmall
	.word WiFiPlazaEventWobble_InitLarge

sWiFiPlazaMoverSounds: ; 0x0226A8DC
	.byte 0xFF, 0x00, 0x06, 0x00
	.byte 0xFE, 0xFF, 0xCB, 0x05, 0xCE, 0x05, 0xC5, 0x05, 0x89, 0x01, 0x82, 0x00, 0xCB, 0x05, 0xCD, 0x05
	.byte 0xCF, 0x05, 0xCB, 0x05, 0x19, 0x00, 0xCE, 0x01, 0xFE, 0xFF, 0xCB, 0x05, 0xFE, 0xFF, 0xD0, 0x05
	.byte 0x01, 0x00, 0xC7, 0x01, 0xD1, 0x05, 0xCC, 0x05, 0xC5, 0x05, 0xCB, 0x05, 0x97, 0x00, 0x97, 0x00
	.byte 0xFE, 0xFF, 0xCB, 0x05, 0xD2, 0x05, 0xCB, 0x05
	; 0x0226A918
