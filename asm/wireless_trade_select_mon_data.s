#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.public WirelessTradeSelectMon_Exit
	.public WirelessTradeSelectMon_Init
	.public WirelessTradeSelectMon_Main
	.public _0221BF9C
	.public _0221BFA0
	.public _0221BFA4
	.public _0221BFA8
	.public _0221BFAC
	.public _0221BFB0
	.public _0221BFB4
	.public _0221BFB8
	.public _0221C080
	.public _0221C084
	.public _0221C088
	.public _0221C1AC
	.public _0221C1B0
	.public _0221C1B4
	.public _0221C1B8
	.public _0221C1BC
	.public _0221C1C0
	.public _0221C240
	.public _0221C244
	.public _0221C248
	.public _0221C24C
	.public _0221C26E
	.public _0221C270
	.public _0221C284
	.public _0221C28A
	.public _0221C28C
	.public _0221C298
	.public _0221C2A8
	.public _0221C2B0
	.public _0221C2B8
	.public _0221C2D6
	.public _0221C2F4
	.public _0221C330
	.public _0221C380
	.public _0221C39C
	.public _0221C3AA
	.public _0221C3B4
	.public _0221C3CA
	.public _0221C3F0
	.public _0221C412
	.public _0221C430
	.public _0221C466
	.public _0221C482
	.public _0221C48A
	.public _0221C51A
	.public _0221C542
	.public _0221C558
	.public _0221C578
	.public _0221C584
	.public _0221C59C
	.public _0221C5C0
	.public _0221C5C4
	.public _0221C5C8
	.public _0221C5CC
	.public _0221C5D0
	.public _0221C5D4
	.public _0221C5D8
	.public _0221C5DC
	.public _0221C5F2
	.public _0221C5FE
	.public _0221C626
	.public _0221C64A
	.public _0221C678
	.public _0221C686
	.public _0221C68E
	.public _0221C6A4
	.public _0221C6A6
	.public _0221C6CA
	.public _0221C6D8
	.public _0221C6E6
	.public _0221C6FA
	.public _0221C70A
	.public _0221C722
	.public _0221C736
	.public _0221C748
	.public _0221C76C
	.public _0221C774
	.public _0221C77A
	.public _0221C77C
	.public _0221C782
	.public _0221C78E
	.public _0221C796
	.public _0221C7B2
	.public _0221C7D0
	.public _0221C7E4
	.public _0221C804
	.public _0221C812
	.public _0221C82E
	.public _0221C83A
	.public _0221C84C
	.public _0221C87E
	.public _0221C88E
	.public _0221C890
	.public _0221C8CA
	.public _0221C90C
	.public _0221C94C
	.public _0221C99E
	.public _0221C9A4
	.public _0221C9A8
	.public _0221C9AC
	.public _0221C9B0
	.public _0221C9B4
	.public _0221C9B8
	.public _0221C9BC
	.public _0221C9C0
	.public _0221C9C4
	.public _0221C9C8
	.public _0221C9CC
	.public _0221C9D0
	.public _0221C9D4
	.public _0221CA50
	.public _0221CA60
	.public _0221CAA6
	.public _0221CACE
	.public _0221CAD4
	.public _0221CAD8
	.public _0221CB48
	.public _0221CB4C
	.public _0221CB50
	.public _0221CB54
	.public _0221CB58
	.public _0221CBD4
	.public _0221CBEE
	.public _0221CBF0
	.public _0221CBF4
	.public _0221CBF8
	.public _0221CBFC
	.public _0221CC00
	.public _0221CC04
	.public _0221CC08
	.public _0221CC22
	.public _0221CC38
	.public _0221CC40
	.public _0221CC46
	.public _0221CC4C
	.public _0221CC72
	.public _0221CCA8
	.public _0221CCAC
	.public _0221CCE4
	.public _0221CD22
	.public _0221CD38
	.public _0221CD4A
	.public _0221CD60
	.public _0221CD64
	.public _0221CD68
	.public _0221CD6C
	.public _0221CD70
	.public _0221CE14
	.public _0221CE18
	.public _0221CE6C
	.public _0221CE84
	.public _0221CE88
	.public _0221CE8C
	.public _0221CE90
	.public _0221CE94
	.public _0221CEA2
	.public _0221CEB4
	.public _0221CFFC
	.public _0221D0C0
	.public _0221D0C4
	.public _0221D0C8
	.public _0221D0CC
	.public _0221D0D0
	.public _0221D0D4
	.public _0221D0D8
	.public _0221D0DC
	.public _0221D0E0
	.public _0221D0E4
	.public _0221D0E8
	.public _0221D1B0
	.public _0221D1B4
	.public _0221D1B8
	.public _0221D1BC
	.public _0221D1C0
	.public _0221D1C4
	.public _0221D232
	.public _0221D250
	.public _0221D256
	.public _0221D3E4
	.public _0221D424
	.public _0221D5C0
	.public _0221D5F8
	.public _0221D656
	.public _0221D66E
	.public _0221D6D4
	.public _0221D74C
	.public _0221D76A
	.public _0221D78E
	.public _0221D7B0
	.public _0221D836
	.public _0221D842
	.public _0221D8AC
	.public _0221D8B0
	.public _0221D8B4
	.public _0221D8B8
	.public _0221D8BC
	.public _0221D8C0
	.public _0221D92C
	.public _0221D9EC
	.public _0221DA54
	.public _0221DABA
	.public _0221DB0C
	.public _0221DB94
	.public _0221DBE4
	.public _0221DBE8
	.public _0221DBEC
	.public _0221DBF0
	.public _0221DC08
	.public _0221DC12
	.public _0221DC1C
	.public _0221DC26
	.public _0221DC2C
	.public _0221DC30
	.public _0221DC8E
	.public _0221DC96
	.public _0221DCAA
	.public _0221DCAC
	.public _0221DCB0
	.public _0221DCB4
	.public _0221DCB8
	.public _0221DCD2
	.public _0221DD12
	.public _0221DD20
	.public _0221DD28
	.public _0221DD30
	.public _0221DD46
	.public _0221DD78
	.public _0221DD96
	.public _0221DDAC
	.public _0221DDB0
	.public _0221DDB4
	.public _0221DDB8
	.public _0221DDBC
	.public _0221DDFA
	.public _0221DE00
	.public _0221DE08
	.public _0221DE0C
	.public _0221DE3C
	.public _0221DE50
	.public _0221DE54
	.public _0221DE8A
	.public _0221DED8
	.public _0221DF18
	.public _0221DF52
	.public _0221DF82
	.public _0221DF8E
	.public _0221DF9A
	.public _0221DFA0
	.public _0221DFA4
	.public _0221DFA8
	.public _0221DFAC
	.public _0221DFB0
	.public _0221DFB4
	.public _0221DFE4
	.public _0221E008
	.public _0221E014
	.public _0221E030
	.public _0221E034
	.public _0221E038
	.public _0221E03C
	.public _0221E040
	.public _0221E044
	.public _0221E048
	.public _0221E04C
	.public _0221E062
	.public _0221E09C
	.public _0221E0D6
	.public _0221E130
	.public _0221E134
	.public _0221E138
	.public _0221E13C
	.public _0221E140
	.public _0221E188
	.public _0221E18C
	.public _0221E190
	.public _0221E1C6
	.public _0221E1D0
	.public _0221E218
	.public _0221E250
	.public _0221E258
	.public _0221E25C
	.public _0221E260
	.public _0221E264
	.public _0221E268
	.public _0221E26C
	.public _0221E2B4
	.public _0221E2B8
	.public _0221E2BC
	.public _0221E2D8
	.public _0221E316
	.public _0221E31C
	.public _0221E320
	.public _0221E324
	.public _0221E328
	.public _0221E32C
	.public _0221E350
	.public _0221E386
	.public _0221E390
	.public _0221E398
	.public _0221E3A6
	.public _0221E3AC
	.public _0221E3B0
	.public _0221E3B4
	.public _0221E3B8
	.public _0221E3EE
	.public _0221E3F8
	.public _0221E42C
	.public _0221E450
	.public _0221E458
	.public _0221E45C
	.public _0221E460
	.public _0221E464
	.public _0221E468
	.public _0221E46C
	.public _0221E470
	.public _0221E494
	.public _0221E4C0
	.public _0221E506
	.public _0221E530
	.public _0221E534
	.public _0221E538
	.public _0221E53C
	.public _0221E540
	.public _0221E544
	.public _0221E548
	.public _0221E54C
	.public _0221E550
	.public _0221E554
	.public _0221E580
	.public _0221E5AA
	.public _0221E5EC
	.public _0221E5F0
	.public _0221E5F4
	.public _0221E5F8
	.public _0221E5FC
	.public _0221E632
	.public _0221E63C
	.public _0221E644
	.public _0221E668
	.public _0221E670
	.public _0221E674
	.public _0221E678
	.public _0221E67C
	.public _0221E680
	.public _0221E6B6
	.public _0221E6C0
	.public _0221E6CA
	.public _0221E6EA
	.public _0221E6F0
	.public _0221E708
	.public _0221E70E
	.public _0221E714
	.public _0221E718
	.public _0221E71C
	.public _0221E720
	.public _0221E724
	.public _0221E728
	.public _0221E73C
	.public _0221E75A
	.public _0221E770
	.public _0221E77C
	.public _0221E7C2
	.public _0221E7FE
	.public _0221E808
	.public _0221E828
	.public _0221E82E
	.public _0221E838
	.public _0221E83C
	.public _0221E840
	.public _0221E844
	.public _0221E848
	.public _0221E84C
	.public _0221E850
	.public _0221E854
	.public _0221E898
	.public _0221E8CC
	.public _0221E8D0
	.public _0221E8D4
	.public _0221E8D8
	.public _0221E8DC
	.public _0221E8E0
	.public _0221E8E4
	.public _0221E8E8
	.public _0221E908
	.public _0221E90E
	.public _0221E918
	.public _0221E91C
	.public _0221E920
	.public _0221E924
	.public _0221E99C
	.public _0221E9A0
	.public _0221E9A4
	.public _0221EA5C
	.public _0221EA60
	.public _0221EA64
	.public _0221EA68
	.public _0221EA6C
	.public _0221EA70
	.public _0221EA94
	.public _0221EA9E
	.public _0221EAEA
	.public _0221EB32
	.public _0221EB78
	.public _0221EB80
	.public _0221EB84
	.public _0221EB88
	.public _0221EB8C
	.public _0221EB90
	.public _0221EB94
	.public _0221EB98
	.public _0221EBEC
	.public _0221EBF0
	.public _0221EBF4
	.public _0221EC14
	.public _0221EC1A
	.public _0221EC24
	.public _0221EC28
	.public _0221EC2C
	.public _0221EC30
	.public _0221EC46
	.public _0221EC70
	.public _0221EC84
	.public _0221ECBC
	.public _0221ECD0
	.public _0221ED54
	.public _0221ED58
	.public _0221ED5C
	.public _0221ED60
	.public _0221ED64
	.public _0221ED68
	.public _0221ED6C
	.public _0221ED70
	.public _0221ED74
	.public _0221ED78
	.public _0221ED7C
	.public _0221EDF2
	.public _0221EE14
	.public _0221EEE0
	.public _0221EEE8
	.public _0221EEEC
	.public _0221EEF0
	.public _0221EEF4
	.public _0221EEF8
	.public _0221EEFC
	.public _0221EF00
	.public _0221EF04
	.public _0221EF08
	.public _0221EF34
	.public _0221EF80
	.public _0221EFA6
	.public _0221EFB2
	.public _0221F004
	.public _0221F008
	.public _0221F00C
	.public _0221F010
	.public _0221F014
	.public _0221F018
	.public _0221F01C
	.public _0221F020
	.public _0221F024
	.public _0221F0B0
	.public _0221F0B4
	.public _0221F0B8
	.public _0221F0BC
	.public _0221F0C0
	.public _0221F11C
	.public _0221F152
	.public _0221F15C
	.public _0221F17A
	.public _0221F1A4
	.public _0221F1AC
	.public _0221F1C8
	.public _0221F1E4
	.public _0221F1F6
	.public _0221F238
	.public _0221F240
	.public _0221F244
	.public _0221F248
	.public _0221F24C
	.public _0221F250
	.public _0221F254
	.public _0221F29C
	.public _0221F2A0
	.public _0221F2A4
	.public _0221F2DA
	.public _0221F2E4
	.public _0221F2FC
	.public _0221F33E
	.public _0221F344
	.public _0221F348
	.public _0221F34C
	.public _0221F350
	.public _0221F37C
	.public _0221F382
	.public _0221F398
	.public _0221F39C
	.public _0221F3A0
	.public _0221F3A4
	.public _0221F3A8
	.public _0221F3AC
	.public _0221F3E8
	.public _0221F3EC
	.public _0221F3F0
	.public _0221F498
	.public _0221F49C
	.public _0221F4A0
	.public _0221F4A4
	.public _0221F4A8
	.public _0221F4AC
	.public _0221F4CC
	.public _0221F4D0
	.public _0221F4F4
	.public _0221F540
	.public _0221F586
	.public _0221F58C
	.public _0221F590
	.public _0221F594
	.public _0221F598
	.public _0221F59C
	.public _0221F614
	.public _0221F62C
	.public _0221F664
	.public _0221F6BC
	.public _0221F70C
	.public _0221F710
	.public _0221F744
	.public _0221F774
	.public _0221F778
	.public _0221F77C
	.public _0221F796
	.public _0221F798
	.public _0221F7B8
	.public _0221F7C0
	.public _0221F7C8
	.public _0221F7D2
	.public _0221F7D8
	.public _0221F7DC
	.public _0221F7F6
	.public _0221F80E
	.public _0221F814
	.public _0221F826
	.public _0221F83E
	.public _0221F844
	.public _0221F848
	.public _0221F84C
	.public _0221F860
	.public _0221F886
	.public _0221F888
	.public _0221F88C
	.public _0221F8AC
	.public _0221F8B0
	.public _0221F8C8
	.public _0221F8CC
	.public _0221FA3A
	.public _0221FA84
	.public _0221FAD0
	.public _0221FAD4
	.public _0221FAD8
	.public _0221FADC
	.public _0221FAE6
	.public _0221FAF8
	.public _0221FB70
	.public _0221FB8C
	.public _0221FBBA
	.public _0221FBC6
	.public _0221FBFC
	.public _0221FC00
	.public _0221FC04
	.public _0221FC6A
	.public _0221FC7A
	.public _0221FC82
	.public _0221FC9A
	.public _0221FD10
	.public _0221FD14
	.public _0221FD18
	.public _0221FD1C
	.public _0221FD2C
	.public _0221FD30
	.public sWirelessTradeSelectMonSummaryPages
	.public WirelessTradeSelectMon_LoadMonIconGfx
	.public WirelessTradeSelectMon_PrintPlayerNames
	.public WirelessTradeSelectMon_ReloadScreen
	.public WirelessTradeSelectMon_PrintPartyNicknames
	.public WirelessTradeSelectMon_BufferPartyNicknames
	.public WirelessTradeSelectMon_PrintNicknameList
	.public WirelessTradeSelectMon_LoadPartyIcons
	.public WirelessTradeSelectMon_ExchangeData
	.public WirelessTradeSelectMon_GetMonInfo
	.public WirelessTradeSelectMon_SetBallSprite
	.public WirelessTradeSelectMon_ShowPartnerMon
	.public WirelessTradeSelectMon_ShowSelectedMon
	.public WirelessTradeSelectMon_RunSelect
	.public WirelessTradeSelectMon_FreeSprites
	.public WirelessTradeSelectMon_FreeIconCharData
	.public WirelessTradeSelectMon_VBlankCB
	.public WirelessTradeSelectMon_SetGfxBanks
	.public WirelessTradeSelectMon_InitBgs
	.public WirelessTradeSelectMon_InitWork
	.public WirelessTradeSelectMon_FreeBgs
	.public WirelessTradeSelectMon_LoadScreenData
	.public WirelessTradeSelectMon_CommitNameBg
	.public WirelessTradeSelectMon_LoadBgGraphics
	.public WirelessTradeSelectMon_InitObjTransfer
	.public WirelessTradeSelectMon_InitSpriteResources
	.public WirelessTradeSelectMon_LoadMonPic
	.public WirelessTradeSelectMon_PrintMonNickname
	.public WirelessTradeSelectMon_GetGenderSymbol
	.public WirelessTradeSelectMon_ShowMonDetails
	.public WirelessTradeSelectMon_HideMonDetails
	.public WirelessTradeSelectMon_CreateSprites
	.public WirelessTradeSelectMon_ReadDpad
	.public WirelessTradeSelectMon_PlayCryOnTouch
	.public WirelessTradeSelectMon_UpdateCursorBlink
	.public WirelessTradeSelectMon_GetNextCursorPos
	.public WirelessTradeSelectMon_SetCursorSprite
	.public WirelessTradeSelectMon_MoveCursor
	.public WirelessTradeSelectMon_SendCommand
	.public WirelessTradeSelectMon_SendCommandIfChanged
	.public WirelessTradeSelectMon_GetPartyChunk
	.public WirelessTradeSelectMon_SendPartyChunk
	.public WirelessTradeSelectMon_SendSpecialRibbons
	.public WirelessTradeSelectMon_SendPalPad
	.public WirelessTradeSelectMon_SendChatot
	.public WirelessTradeSelectMon_HandleKeys
	.public WirelessTradeSelectMon_HandleTouch
	.public WirelessTradeSelectMon_SelectMon
	.public WirelessTradeSelectMon_OpenSummary
	.public WirelessTradeSelectMon_AskQuit
	.public WirelessTradeSelectMon_HandleQuitPrompt
	.public WirelessTradeSelectMon_ShowResultMessage
	.public WirelessTradeSelectMon_WaitResultMessage
	.public WirelessTradeSelectMon_PrintRosterMessage
	.public WirelessTradeSelectMon_HandleGiveUpPrompt
	.public WirelessTradeSelectMon_HandleDeletePrompt
	.public WirelessTradeSelectMon_HandleRosterListInput
	.public WirelessTradeSelectMon_ShowRosterList
	.public WirelessTradeSelectMon_HandleRosterFullPrompt
	.public WirelessTradeSelectMon_HandleRegisterPrompt
	.public WirelessTradeSelectMon_WaitQuitSync
	.public WirelessTradeSelectMon_RegisterNextFriend
	.public WirelessTradeSelectMon_StartFriendRegistration
	.public WirelessTradeSelectMon_WaitQuitAnswers
	.public WirelessTradeSelectMon_CreateTouchMenu
	.public WirelessTradeSelectMon_ShowOwnMonMenu
	.public WirelessTradeSelectMon_HandleOwnMonMenu
	.public WirelessTradeSelectMon_OfferTrade
	.public WirelessTradeSelectMon_WaitOfferAnswers
	.public WirelessTradeSelectMon_StartTradeSlide
	.public WirelessTradeSelectMon_InitSlide
	.public WirelessTradeSelectMon_SlideMonIcon
	.public WirelessTradeSelectMon_UpdateTradeSlide
	.public WirelessTradeSelectMon_RestoreIcons
	.public WirelessTradeSelectMon_AskTradeConfirm
	.public WirelessTradeSelectMon_AcceptTrade
	.public WirelessTradeSelectMon_PrintMessageInWindow
	.public WirelessTradeSelectMon_HandleTradeConfirm
	.public WirelessTradeSelectMon_AskDetachCapsule
	.public WirelessTradeSelectMon_HandleCapsulePrompt
	.public WirelessTradeSelectMon_WaitConfirmAnswers
	.public WirelessTradeSelectMon_FinishTrade
	.public WirelessTradeSelectMon_ShowPartnerMonMenu
	.public WirelessTradeSelectMon_DestroyTouchMenu
	.public WirelessTradeSelectMon_HandlePartnerMonMenu
	.public WirelessTradeSelectMon_TradeMons
	.public WirelessTradeSelectMon_ScrollBg
	.public WirelessTradeSelectMon_SetSpritePos
	.public WirelessTradeSelectMon_ShowWaitingIcon
	.public WirelessTradeSelectMon_HideWaitingIcon
	.public WirelessTradeSelectMon_HasOtherNonEggMon
	.public WirelessTradeSelectMon_FindBadEggParty
	.public WirelessTradeSelectMon_StartTimeout
	.public WirelessTradeSelectMon_UpdateTimeout
	.public WirelessTradeSelectMon_StartMenuDim
	.public WirelessTradeSelectMon_EndMenuDim
	.public WirelessTradeSelectMon_AddWindows
	.public WirelessTradeSelectMon_RemoveWindows
	.public WirelessTradeSelectMon_PrintString
	.public WirelessTradeSelectMon_PrintMessage
	.public WirelessTradeSelectMon_CreateYesNoPrompt
	.public WirelessTradeSelectMon_HandleYesNoPrompt
	.public WirelessTradeSelectMon_CreateRosterListMenu
	.public WirelessTradeSelectMon_RosterListCursorCB
	.public sWirelessTradeSelectMonTouchMenuTemplate
	.public sWirelessTradeSelectMonObjCharTransferTemplate
	.public sWirelessTradeSelectMonGraphicsModes
	.public sWirelessTradeSelectMonCursorAnims
	.public ov65_0221FD6C
	.public ov65_0221FD70
	.public sWirelessTradeSelectMonBgTemplate_Sub0
	.public sWirelessTradeSelectMonBgTemplate_Sub1
	.public sWirelessTradeSelectMonBgTemplate_Main1
	.public sWirelessTradeSelectMonBgTemplate_Sub2
	.public sWirelessTradeSelectMonBgTemplate_Main0
	.public sWirelessTradeSelectMonBgTemplate_Main2
	.public sWirelessTradeSelectMonBgTemplate_Main3
	.public sWirelessTradeSelectMonGraphicsBanks
	.public sWirelessTradeSelectMonSlotHitboxes
	.public sWirelessTradeSelectMonDetailSpritePos
	.public ov65_0221FEA8
	.public sWirelessTradeSelectMonBallPalettes
	.public sWirelessTradeSelectMonSlotPos
	.public ov65_0221FF50
	.public sWirelessTradeSelectMonCursorMoves
	.public sWirelessTradeSelectMonRosterListTemplate
	.public sWirelessTradeSelectMonNameWindowPos
	.public sWirelessTradeSelectMonDetailWindows
	.include "overlay_65.inc"
	.include "global.inc"

	.text
	.public WirelessTradeSelectMon_CommitNameBg
	.public WirelessTradeSelectMon_GetGenderSymbol
	.public WirelessTradeSelectMon_SendCommand
	.public WirelessTradeSelectMon_GetPartyChunk
	.public WirelessTradeSelectMon_SendPartyChunk
	.public WirelessTradeSelectMon_SendSpecialRibbons
	.public WirelessTradeSelectMon_SendChatot
	.public WirelessTradeSelectMon_SelectMon
	.public WirelessTradeSelectMon_PrintRosterMessage
	.public WirelessTradeSelectMon_WaitQuitSync
	.public WirelessTradeSelectMon_InitSlide
	.public WirelessTradeSelectMon_AcceptTrade
	.public WirelessTradeSelectMon_DestroyTouchMenu
	.public WirelessTradeSelectMon_ScrollBg
	.public WirelessTradeSelectMon_SetSpritePos
	.public WirelessTradeSelectMon_ShowWaitingIcon
	.public WirelessTradeSelectMon_StartTimeout
	.public WirelessTradeSelectMon_EndMenuDim
	.public WirelessTradeSelectMon_RosterListCursorCB
	.rodata

sWirelessTradeSelectMonSummaryPages:
	.byte 0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00

sWirelessTradeSelectMonTouchMenuTemplate: ; 0x0221FD3C
	.byte 0x02, 0x00, 0x00, 0x0C
	.byte 0x00, 0x00, 0x80, 0x01, 0xCE, 0x01, 0x4E, 0x00

sWirelessTradeSelectMonObjCharTransferTemplate: ; 0x0221FD48
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00

sWirelessTradeSelectMonGraphicsModes: ; 0x0221FD58
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonCursorAnims: ; 0x0221FD68
	.byte 0x00, 0x00, 0x00, 0x00

ov65_0221FD6C: ; 0x0221FD6C
	.byte 0x01, 0x00, 0x00, 0x00

ov65_0221FD70: ; 0x0221FD70
	.byte 0x02, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Sub0: ; 0x0221FD80
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1F, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Sub1: ; 0x0221FD9C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Main1: ; 0x0221FDB8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x02, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Sub2: ; 0x0221FDD4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Main0: ; 0x0221FDF0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Main2: ; 0x0221FE0C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x04
	.byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBgTemplate_Main3: ; 0x0221FE28
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x04, 0x00, 0x03, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonGraphicsBanks: ; 0x0221FE44
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonSlotHitboxes: ; 0x0221FE6C
	.byte 0x20, 0x47, 0x00, 0x3F
	.byte 0x20, 0x47, 0x40, 0x7F, 0x48, 0x6F, 0x00, 0x3F, 0x48, 0x6F, 0x40, 0x7F, 0x70, 0x97, 0x00, 0x3F
	.byte 0x70, 0x97, 0x40, 0x7F, 0x20, 0x47, 0x80, 0xBF, 0x20, 0x47, 0xC0, 0xFF, 0x48, 0x6F, 0x80, 0xBF
	.byte 0x48, 0x6F, 0xC0, 0xFF, 0x70, 0x97, 0x80, 0xBF, 0x70, 0x97, 0xC0, 0xFF, 0xA0, 0xC0, 0xC8, 0x00
	.byte 0xFF, 0x00, 0x00, 0x00

sWirelessTradeSelectMonDetailSpritePos: ; 0x0221FEA4
	.byte 0x3F, 0x00, 0x00, 0x00

ov65_0221FEA8: ; 0x0221FEA8
	.byte 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xC0, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0xE3, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00

sWirelessTradeSelectMonBallPalettes: ; 0x0221FEEC
	.byte 0x31, 0x00, 0x00, 0x00
	.byte 0x33, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00
	.byte 0x32, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00
	.byte 0x33, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00
	.byte 0x34, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00
	.byte 0x36, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00
	.byte 0x3A, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00

sWirelessTradeSelectMonSlotPos: ; 0x0221FF4C
	.byte 0x00, 0x00, 0x00, 0x00

ov65_0221FF50: ; 0x0221FF50
	.byte 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x48, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x70, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x20, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x48, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x70, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00
	.byte 0xA0, 0x00, 0x00, 0x00

sWirelessTradeSelectMonCursorMoves: ; 0x0221FFB4
	.byte 0x04, 0x02, 0x0C, 0x0C, 0x00, 0x00, 0x02, 0x04, 0x0C, 0x0C, 0x00, 0x00
	.byte 0x07, 0x06, 0x01, 0x00, 0x00, 0x00, 0x01, 0x06, 0x07, 0x00, 0x00, 0x00, 0x05, 0x03, 0x0C, 0x0C
	.byte 0x00, 0x00, 0x03, 0x05, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x07, 0x06, 0x01, 0x00, 0x00, 0x06, 0x07
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x09, 0x08, 0x07, 0x06, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01
	.byte 0x00, 0x00, 0x05, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02, 0x09, 0x08, 0x07, 0x00, 0x00, 0x08, 0x09
	.byte 0x06, 0x06, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03
	.byte 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x0A, 0x08
	.byte 0x06, 0x00, 0x00, 0x00, 0x0A, 0x08, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x0A, 0x0C, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x09, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x06, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x09, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x0B, 0x09, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02
	.byte 0x00, 0x00, 0x00, 0x00, 0x0B, 0x09, 0x07, 0x06, 0x00, 0x00, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonRosterListTemplate: ; 0x022200EC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00

sWirelessTradeSelectMonNameWindowPos: ; 0x0222010C
	.byte 0x00, 0x00, 0x07, 0x00
	.byte 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x11, 0x00
	.byte 0x08, 0x00, 0x11, 0x00, 0x10, 0x00, 0x07, 0x00, 0x18, 0x00, 0x07, 0x00, 0x10, 0x00, 0x0C, 0x00
	.byte 0x18, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x11, 0x00, 0x18, 0x00, 0x11, 0x00, 0x04, 0x00, 0x08, 0x00
	.byte 0x14, 0x00, 0x08, 0x00

sWirelessTradeSelectMonDetailWindows: ; 0x02220144
	.byte 0x04, 0x00, 0x01, 0x00, 0x09, 0x00, 0x02, 0x00, 0x01, 0x00, 0x14, 0x00
	.byte 0x01, 0x00, 0x09, 0x00, 0x02, 0x00, 0x13, 0x00, 0x04, 0x00, 0x03, 0x00, 0x08, 0x00, 0x02, 0x00
	.byte 0x25, 0x00, 0x14, 0x00, 0x03, 0x00, 0x08, 0x00, 0x02, 0x00, 0x35, 0x00, 0x02, 0x00, 0x13, 0x00
	.byte 0x07, 0x00, 0x02, 0x00, 0x45, 0x00, 0x12, 0x00, 0x13, 0x00, 0x07, 0x00, 0x02, 0x00, 0x53, 0x00
	.byte 0x02, 0x00, 0x15, 0x00, 0x0C, 0x00, 0x02, 0x00, 0x61, 0x00, 0x12, 0x00, 0x15, 0x00, 0x0C, 0x00
	.byte 0x02, 0x00, 0x79, 0x00
	; 0x02220194
