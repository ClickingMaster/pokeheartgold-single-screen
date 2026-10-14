	.extern WiFiPlaza_HandleChatNews
	.extern WiFiPlaza_HandleTapToyTradeNews
	.extern WiFiPlaza_HandleMinigameEntryNews
	.extern WiFiPlaza_HandleFootprintStampNews
	.extern WiFiPlaza_HandleVisitorProfilesNews
	.extern WiFiPlaza_HandlePlazaNewsNews
	.extern WiFiPlaza_HandleMinigameResultNews
	.extern WiFiPlaza_HandleUnusedNews
	.extern WiFiPlaza_OnTalkRequest
	.extern WiFiPlaza_OnTalkAnswer
	.extern WiFiPlaza_OnTalkState
	.extern WiFiPlaza_OnTalkCancel
	.extern WiFiPlaza_OnLobbyMessage
	.extern WiFiPlaza_RecvTapToyPlayed
	.extern WiFiPlaza_RecvJoinGroup
	.extern WiFiPlaza_RecvGroupAction
	.extern WiFiPlazaApps_NewMainArgs
	.extern WiFiPlazaApps_FreeMainArgs
	.extern WiFiPlazaApps_StartMain
	.extern WiFiPlazaApps_EndMain
	.extern WiFiPlazaApps_NewVisitorProfilesArgs
	.extern WiFiPlazaApps_FreeVisitorProfilesArgs
	.extern WiFiPlazaApps_StartVisitorProfiles
	.extern WiFiPlazaApps_EndVisitorProfiles
	.extern WiFiPlazaApps_NewPlazaNewsArgs
	.extern WiFiPlazaApps_FreePlazaNewsArgs
	.extern WiFiPlazaApps_StartPlazaNews
	.extern WiFiPlazaApps_EndPlazaNews
	.extern WiFiPlazaApps_NewConnectArgs
	.extern WiFiPlazaApps_FreeConnectArgs
	.extern WiFiPlazaApps_StartConnect
	.extern WiFiPlazaApps_EndConnect
	.extern WiFiPlazaApps_NewSwalotPlopArgs
	.extern WiFiPlazaApps_FreeSwalotPlopArgs
	.extern WiFiPlazaApps_StartSwalotPlop
	.extern WiFiPlazaApps_EndSwalotPlop
	.extern WiFiPlazaApps_NewMimeJrTopArgs
	.extern WiFiPlazaApps_FreeMimeJrTopArgs
	.extern WiFiPlazaApps_StartMimeJrTop
	.extern WiFiPlazaApps_EndMimeJrTop
	.extern WiFiPlazaApps_NewWobbuffetPopArgs
	.extern WiFiPlazaApps_FreeWobbuffetPopArgs
	.extern WiFiPlazaApps_StartWobbuffetPop
	.extern WiFiPlazaApps_EndWobbuffetPop
	.extern WiFiPlazaApps_NewFootprintStampArgs
	.extern WiFiPlazaApps_FreeFootprintStampArgs
	.extern WiFiPlazaApps_EndFootprintStamp
	.extern WiFiPlazaApps_StartFootprintStampBoard0
	.extern WiFiPlazaApps_StartFootprintStampBoard1
	.extern WiFiPlazaApps_NewDisconnectArgs
	.extern WiFiPlazaApps_FreeDisconnectArgs
	.extern WiFiPlazaApps_StartDisconnect
	.extern WiFiPlazaApps_StartClosedDisconnect
	.extern WiFiPlazaApps_EndDisconnect
	.extern WiFiPlazaApps_NewSurveyAnswerArgs
	.extern WiFiPlazaApps_NewSurveyResultsArgs
	.extern WiFiPlazaApps_FreeSurveyArgs
	.extern WiFiPlazaApps_StartSurvey
	.extern WiFiPlazaApps_EndSurvey
	.extern WiFiPlazaTime_Add
	.extern WiFiPlazaNewsEvent_FormatChat
	.extern WiFiPlazaNewsEvent_FormatTapToyTrade
	.extern WiFiPlazaNewsEvent_FormatMinigameEntry
	.extern WiFiPlazaNewsEvent_FormatFootprintStamp
	.extern WiFiPlazaNewsEvent_FormatChecking
	.extern WiFiPlazaNewsEvent_FormatLobbyEventNotice
	.extern WiFiPlazaNewsEvent_FormatSpecialVisitor
	.extern WiFiPlazaNewsEvent_FormatMinigameResult
	.extern WiFiPlazaNewsEvent_GetChatSlot
	.extern WiFiPlazaNewsEvent_GetTapToyTradeSlot
	.extern WiFiPlazaNewsEvent_GetMinigameEntrySlot
	.extern WiFiPlazaNewsEvent_GetFootprintStampSlot
	.extern WiFiPlazaNewsEvent_GetCheckingSlot
	.extern WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot
	.extern WiFiPlazaNewsEvent_GetSpecialVisitorSlot
	.extern WiFiPlazaNewsEvent_GetMinigameResultSlot
	.extern WiFiPlazaPlayerObject_ShowStandingSprite
	.extern WiFiPlazaPlayerObject_PlayStandingSequence
	.extern ov45_02230BFC
	.extern WiFiPlazaPlayerObject_PlayWalkingSequence
	.extern WiFiPlazaPlayerObject_EffectTurnAround
	.extern ov45_02230D20
	.extern ov45_02230D5C
	.public IncrementGameStat119
	.public IncrementGameStat47
	.public _02254A28
	.public _02254E20
	.public _022577C0
	.public WiFiPlaza_Init
	.public WiFiPlaza_Main
	.public WiFiPlaza_Exit
	.public WiFiPlaza_VBlankTask
	.public WiFiPlazaWork_New
	.public WiFiPlazaWork_Delete
	.public WiFiPlazaWork_Update
	.public ov45_0222A208
	.public WiFiPlazaWork_GetPlaces
	.public WiFiPlazaWork_TakePlayerEntered
	.public WiFiPlazaWork_TakePlayerLeft
	.public WiFiPlazaWork_IsEventFlagSet
	.public WiFiPlazaWork_TakePlayerUpdated
	.public ov45_0222A2CC
	.public ov45_0222A2E0
	.public ov45_0222A2F8
	.public ov45_0222A310
	.public WiFiPlazaWork_IsLobbyOpen
	.public WiFiPlazaWork_IsClosing
	.public WiFiPlazaWork_IsClosed
	.public WiFiPlazaWork_GetFireworksState
	.public WiFiPlazaWork_IsParadeStarted
	.public ov45_0222A394
	.public WiFiPlazaWork_IsLightingStarted
	.public WiFiPlazaWork_GetColorEvent
	.public WiFiPlazaWork_GetLight1Event
	.public ov45_0222A3EC
	.public WiFiPlazaWork_ClearGameMembers
	.public WiFiPlazaWork_SetGameMember
	.public WiFiPlazaWork_SetOwnGameMember
	.public WiFiPlazaWork_PlayBGM
	.public WiFiPlazaWork_SetLowVolume
	.public WiFiPlazaWork_GetOwnSlot
	.public WiFiPlazaWork_GetOwnId
	.public WiFiPlazaWork_IsSlotOccupied
	.public WiFiPlazaWork_GetPlayer
	.public WiFiPlazaWork_GetMyPlayer
	.public WiFiPlazaWork_SetActivity
	.public WiFiPlazaWork_AddHistory
	.public WiFiPlazaWork_SetTapToy
	.public WiFiPlazaWork_SetFavoriteTypes
	.public WiFiPlazaWork_SetSurveyAnswer
	.public WiFiPlazaPlayer_CopyToProfile
	.public WiFiPlazaPlayer_GetHistoryId
	.public WiFiPlazaPlayer_GetHistoryValue
	.public WiFiPlazaPlayer_GetAvatarIndex
	.public WiFiPlazaPlayer_GetGender
	.public WiFiPlazaPlayer_GetLanguage
	.public WiFiPlazaPlayer_IsLanguageValid
	.public WiFiPlazaPlayer_GetAvatar
	.public WiFiPlazaPlayer_GetCountry
	.public WiFiPlazaPlayer_GetRegion
	.public WiFiPlazaPlayer_GetTapToy
	.public WiFiPlazaPlayer_HasTapToy
	.public WiFiPlazaPlayer_GetFavoriteType
	.public WiFiPlazaPlayer_GetSurvey
	.public WiFiPlazaWork_IsSpecialVisitor
	.public WiFiPlazaWork_GetSpecialVisitors
	.public WiFiPlazaWork_HasWordSet
	.public WiFiPlazaWork_GetWordSet
	.public WiFiPlaza_GetChannelPlayerCount
	.public WiFiPlaza_ChannelSlotToSlot
	.public WiFiPlaza_SendChatNews
	.public WiFiPlaza_SendTapToyTradeNews
	.public WiFiPlaza_SendActivityNews
	.public ov45_0222AC58
	.public WiFiPlaza_SendMinigameResultNews
	.public ov45_0222AD2C
	.public ov45_0222AD3C
	.public WiFiPlazaWork_GetOpenDuration
	.public WiFiPlazaWork_HasPlayedTapToy
	.public WiFiPlazaWork_SendTapToyPlayed
	.public WiFiPlazaWork_GetGroupState
	.public WiFiPlazaWork_GetGroupCounter
	.public WiFiPlazaWork_FindGroupPlace
	.public WiFiPlazaWork_JoinGroup
	.public WiFiPlazaWork_LeaveGroup
	.public WiFiPlazaWork_RequestGroupAction
	.public WiFiPlazaWork_GetGroupActionRequest
	.public WiFiPlazaWork_ClearGroupActionRequest
	.public WiFiPlaza_SplitGroupPlace
	.public WiFiPlazaWork_SetGroupMarkerPassed
	.public WiFiPlazaWork_GetGroupMarkerCount
	.public WiFiPlazaWork_SendJoinGroup
	.public WiFiPlazaWork_SendGroupAction
	.public WiFiPlazaWork_ResetTalk
	.public WiFiPlazaWork_StartTalk
	.public WiFiPlazaWork_AdvanceTalk
	.public WiFiPlazaWork_CancelTalkRequest
	.public WiFiPlazaWork_EndTalk
	.public WiFiPlazaWork_IsTalking
	.public WiFiPlazaWork_WasAskedToTalk
	.public WiFiPlazaWork_CanTalkWith
	.public WiFiPlazaWork_IsTalkOver
	.public WiFiPlazaWork_GetTalkTimeoutCounter
	.public WiFiPlazaWork_IncPartnerTalkCount
	.public WiFiPlazaWork_IncTalkCount
	.public WiFiPlazaWork_SetTalkedWith
	.public ov45_0222B118
	.public ov45_0222B134
	.public WiFiPlazaWork_PickTapToy
	.public WiFiPlazaWork_GetFireworksPhase
	.public WiFiPlazaWork_GetFireworksCounter
	.public WiFiPlazaWork_SetSurveyTally
	.public WiFiPlazaWork_GetSurveyTally
	.public WiFiPlazaWork_NewPlayerInfo
	.public WiFiPlazaWork_FreePlayerInfo
	.public WiFiPlaza_IsNameTerminated
	.public WiFiPlaza_OnPlayerEnter
	.public WiFiPlaza_OnPlayerLeave
	.public WiFiPlaza_OnPlayerUpdate
	.public WiFiPlaza_OnLobbyEvent
	.public WiFiPlaza_OnOwnRecord
	.public WiFiPlaza_FillPlaces
	.public WiFiPlazaWork_CheckClosingTime
	.public WiFiPlazaProfile_InitFromSave
	.public WiFiPlazaWork_SendOwnRecord
	.public WiFiPlazaProfile_UpdateChecksum
	.public WiFiPlazaProfile_IsChecksumValid
	.public WiFiPlazaProfile_SetFromLobbyRecord
	.public WiFiPlazaEventSchedule_Update
	.public WiFiPlazaEventSchedule_Init
	.public WiFiPlazaEventSchedule_ApplyVolume
	.public WiFiPlaza_NewPlayerProfile
	.public WiFiPlaza_FreePlayerProfile
	.public WiFiPlazaProfile_CopyToPlayerProfile
	.public WiFiPlaza_ClearTapToyPlayedFlags
	.public WiFiPlazaTalkSession_Reset
	.public WiFiPlazaTalkSession_Abort
	.public WiFiPlazaTalkSession_HasTalkedWith
	.public WiFiPlazaTalkSession_SetTalkedWith
	.public WiFiPlazaTalkSession_ClearTalkedWith
	.public WiFiPlazaTalkSession_IncTalkCount
	.public WiFiPlazaTalkSession_ResetTalkCount
	.public WiFiPlazaWork_SendTalkAnswer
	.public WiFiPlazaWork_SendTalkEnd
	.public WiFiPlazaTalkSession_UpdateTimeout
	.public WiFiPlazaTalkSession_IsWithinTimeout
	.public WiFiPlaza_HandleChatNews
	.public WiFiPlaza_HandleTapToyTradeNews
	.public WiFiPlaza_HandleMinigameEntryNews
	.public WiFiPlaza_HandleFootprintStampNews
	.public WiFiPlaza_HandleVisitorProfilesNews
	.public WiFiPlaza_HandlePlazaNewsNews
	.public WiFiPlaza_HandleMinigameResultNews
	.public WiFiPlazaWork_AddLobbyEventNotice
	.public WiFiPlazaGroups_Init
	.public WiFiPlazaGroups_Update
	.public WiFiPlazaGroups_Join
	.public WiFiPlazaGroups_Leave
	.public WiFiPlazaGroups_GetPlaceId
	.public WiFiPlazaGroups_GetState
	.public WiFiPlazaGroups_GetCounter
	.public WiFiPlazaGroups_RequestAction
	.public WiFiPlazaGroups_GetActionRequest
	.public WiFiPlazaGroups_ClearActionRequest
	.public WiFiPlazaGroups_FindPlace
	.public WiFiPlazaGroups_SetMarkerPassed
	.public WiFiPlazaGroups_HasPassedMarker
	.public WiFiPlazaGroups_GetMarkerCount
	.public WiFiPlaza_OnTalkRequest
	.public WiFiPlaza_OnTalkAnswer
	.public WiFiPlaza_OnTalkState
	.public WiFiPlaza_OnTalkCancel
	.public WiFiPlaza_OnLobbyMessage
	.public WiFiPlaza_RecvTapToyPlayed
	.public WiFiPlaza_RecvJoinGroup
	.public WiFiPlaza_RecvGroupAction
	.public WiFiPlazaGroupWeights_Init
	.public WiFiPlazaGroupWeights_ReportPlace
	.public WiFiPlazaGroupWeights_PickGroup
	.public WiFiPlaza_SetSlotFlag
	.public WiFiPlaza_GetSlotFlag
	.public WiFiPlazaWordSet_Init
	.public WiFiPlazaWordSet_Free
	.public WiFiPlazaWordSet_Set
	.public WiFiPlazaWordSet_HasWords
	.public WiFiPlazaWordSet_GetWords
	.public WiFiPlaza_MakeWordsFromCode
	.public WiFiPlazaTimer_Init
	.public WiFiPlazaTimer_Start
	.public WiFiPlazaTimer_Update
	.public WiFiPlazaHistory_Init
	.public WiFiPlazaHistory_Add
	.public WiFiPlazaHistory_ReplaceValue
	.public WiFiPlazaHistory_RemoveOldest
	.public WiFiPlazaHistory_GetId
	.public WiFiPlazaHistory_GetValue
	.public WiFiPlazaHistory_UpdateChecksum
	.public WiFiPlazaHistory_CheckChecksum
	.public WiFiPlaza_AvatarToIndex
	.public WiFiPlaza_IndexToAvatar
	.public WiFiPlazaApps_New
	.public WiFiPlazaApps_Delete
	.public WiFiPlazaApps_StartFirst
	.public WiFiPlazaApps_Run
	.public WiFiPlazaApps_StartOverlay
	.public WiFiPlazaApps_InitArgs
	.public WiFiPlazaApps_FreeArgs
	.public WiFiPlazaApps_Start
	.public WiFiPlazaApps_End
	.public WiFiPlazaGameArgs_NewProfiles
	.public WiFiPlazaGameArgs_FreeProfiles
	.public WiFiPlazaGameArgs_Setup
	.public WiFiPlazaApps_NewMainArgs
	.public WiFiPlazaApps_FreeMainArgs
	.public WiFiPlazaApps_StartMain
	.public WiFiPlazaApps_EndMain
	.public WiFiPlazaApps_NewVisitorProfilesArgs
	.public WiFiPlazaApps_FreeVisitorProfilesArgs
	.public WiFiPlazaApps_StartVisitorProfiles
	.public WiFiPlazaApps_EndVisitorProfiles
	.public WiFiPlazaApps_NewPlazaNewsArgs
	.public WiFiPlazaApps_FreePlazaNewsArgs
	.public WiFiPlazaApps_StartPlazaNews
	.public WiFiPlazaApps_EndPlazaNews
	.public WiFiPlazaApps_NewConnectArgs
	.public WiFiPlazaApps_FreeConnectArgs
	.public WiFiPlazaApps_StartConnect
	.public WiFiPlazaApps_EndConnect
	.public WiFiPlazaApps_NewSwalotPlopArgs
	.public WiFiPlazaApps_FreeSwalotPlopArgs
	.public WiFiPlazaApps_StartSwalotPlop
	.public WiFiPlazaApps_EndSwalotPlop
	.public WiFiPlazaApps_NewMimeJrTopArgs
	.public WiFiPlazaApps_FreeMimeJrTopArgs
	.public WiFiPlazaApps_StartMimeJrTop
	.public WiFiPlazaApps_EndMimeJrTop
	.public WiFiPlazaApps_NewWobbuffetPopArgs
	.public WiFiPlazaApps_FreeWobbuffetPopArgs
	.public WiFiPlazaApps_StartWobbuffetPop
	.public WiFiPlazaApps_EndWobbuffetPop
	.public WiFiPlazaApps_NewFootprintStampArgs
	.public WiFiPlazaApps_FreeFootprintStampArgs
	.public WiFiPlazaApps_EndFootprintStamp
	.public WiFiPlazaApps_StartFootprintStampBoard0
	.public WiFiPlazaApps_StartFootprintStampBoard1
	.public WiFiPlazaApps_NewDisconnectArgs
	.public WiFiPlazaApps_FreeDisconnectArgs
	.public WiFiPlazaApps_StartDisconnect
	.public WiFiPlazaApps_StartClosedDisconnect
	.public WiFiPlazaApps_StartDisconnectOverlay
	.public WiFiPlazaApps_NewSurveyAnswerArgs
	.public WiFiPlazaApps_NewSurveyResultsArgs
	.public WiFiPlazaApps_FreeSurveyArgs
	.public WiFiPlazaApps_StartSurvey
	.public WiFiPlazaApps_EndSurvey
	.public WiFiPlaza_SecondsToTime
	.public WiFiPlazaTime_Add
	.public WiFiPlazaTime_Subtract
	.public WiFiPlazaPlaces_Add
	.public WiFiPlazaPlaces_GetCountry
	.public WiFiPlazaPlaces_GetRegion
	.public WiFiPlazaPlaces_GetValue
	.public WiFiPlazaPlaces_IsOccupied
	.public WiFiPlaza_DisableModelLighting
	.public WiFiPlaza_IsRegionValid
	.public WiFiPlaza_GetTypeByChoice
	.public WiFiPlaza_GetErrorMessageClass
	.public WiFiPlaza_IsErrorRecoverable
	.public WiFiPlaza_HasConnectionFailed
	.public WiFiPlazaNews_New
	.public WiFiPlazaNews_Delete
	.public WiFiPlazaNews_Update
	.public WiFiPlazaNews_AddPlayer
	.public WiFiPlazaNews_RemovePlayer
	.public WiFiPlazaNews_SetOldestPlayer
	.public WiFiPlazaNews_AddChatEvent
	.public WiFiPlazaNews_AddTapToyTradeEvent
	.public WiFiPlazaNews_AddMinigameEntryEvent
	.public WiFiPlazaNews_AddFootprintStampEvent
	.public WiFiPlazaNews_AddVisitorProfilesEvent
	.public WiFiPlazaNews_AddPlazaNewsEvent
	.public WiFiPlazaNews_AddLobbyEventNotice
	.public WiFiPlazaNews_AddSpecialVisitorEvent
	.public WiFiPlazaNews_AddMinigameResultEvent
	.public WiFiPlazaNews_HasEvent
	.public WiFiPlazaNews_GetEventType
	.public WiFiPlazaNews_FormatEvent
	.public WiFiPlazaNews_GetEventSlot
	.public WiFiPlazaNewsSlots_Init
	.public WiFiPlazaNewsSlots_Set
	.public WiFiPlazaNewsSlots_AddPlayer
	.public WiFiPlazaNewsSlots_RemovePlayer
	.public WiFiPlazaNewsSlots_SetFlags
	.public WiFiPlazaNewsSlots_SetChange
	.public WiFiPlazaNewsSlots_SetLastJoined
	.public WiFiPlazaNewsSlots_IsOccupied
	.public WiFiPlazaNewsSlots_GetGender
	.public WiFiPlazaNewsSlots_IsOwn
	.public WiFiPlazaNewsSlots_IsOldest
	.public WiFiPlazaNewsSlots_GetFlags
	.public WiFiPlazaNewsSlots_IsLastJoined
	.public WiFiPlazaNewsEventList_Init
	.public WiFiPlazaNewsEventList_Free
	.public WiFiPlazaNewsEventList_Update
	.public WiFiPlazaNewsEventList_RemoveFirst
	.public WiFiPlazaNewsEventList_GetFreeEvent
	.public WiFiPlazaNewsEvent_InsertAfter
	.public WiFiPlazaNewsEventList_Insert
	.public WiFiPlazaNewsEventList_Remove
	.public WiFiPlazaNewsEvent_Set
	.public WiFiPlazaNewsEvent_FormatChat
	.public WiFiPlazaNewsEvent_FormatTapToyTrade
	.public WiFiPlazaNewsEvent_FormatMinigameEntry
	.public WiFiPlazaNewsEvent_FormatFootprintStamp
	.public WiFiPlazaNewsEvent_FormatChecking
	.public WiFiPlazaNewsEvent_FormatLobbyEventNotice
	.public WiFiPlazaNewsEvent_FormatSpecialVisitor
	.public WiFiPlazaNewsEvent_FormatMinigameResult
	.public WiFiPlazaNewsEvent_GetChatSlot
	.public WiFiPlazaNewsEvent_GetTapToyTradeSlot
	.public WiFiPlazaNewsEvent_GetMinigameEntrySlot
	.public WiFiPlazaNewsEvent_GetFootprintStampSlot
	.public WiFiPlazaNewsEvent_GetCheckingSlot
	.public WiFiPlazaNewsEvent_GetMinigameResultSlot
	.public WiFiPlaza_IsMinigamePlayerCountValid
	.public WiFiPlazaLobbyClient_Init
	.public WiFiPlazaLobbyClient_Free
	.public WiFiPlazaLobbyClient_Update
	.public WiFiPlazaLobbyClient_GetLastError
	.public WiFiPlazaLobbyClient_Initialize
	.public WiFiPlazaLobbyClient_InitializeEx
	.public WiFiPlazaLobbyClient_IsInLobby
	.public WiFiPlazaLobbyClient_Shutdown
	.public WiFiPlazaLobbyClient_IsDisconnected
	.public WiFiPlazaLobbyClient_GetState
	.public ov45_0222E9BC
	.public WiFiPlazaLobbyClient_GetOwnId
	.public WiFiPlazaLobbyClient_SetOwnPlayer
	.public WiFiPlazaLobbyClient_GetPlayer
	.public WiFiPlazaLobbyClient_GetEntryTime
	.public WiFiPlazaLobbyClient_GetOldestPlayer
	.public WiFiPlazaLobbyClient_GetOldestOtherPlayer
	.public WiFiPlazaLobbyClient_JoinChannel
	.public WiFiPlazaLobbyClient_GetJoinChannelResult
	.public WiFiPlazaLobbyClient_LeaveChannel
	.public WiFiPlazaLobbyClient_IsNotInChannel
	.public WiFiPlazaLobbyClient_CanJoinChannel
	.public WiFiPlazaLobbyClient_GetPlayerList
	.public WiFiPlazaLobbyClient_GetChannelPlayerList
	.public WiFiPlazaLobbyClient_IdToSlot
	.public WiFiPlazaLobbyClient_IdToChannelSlot
	.public WiFiPlazaLobbyClient_SlotToId
	.public WiFiPlazaLobbyClient_ChannelSlotToId
	.public WiFiPlazaLobbyClient_GetTime
	.public WiFiPlazaLobbyClient_GetScheduleValue
	.public WiFiPlazaLobbyClient_StartAddOwnLocation
	.public WiFiPlazaLobbyClient_IsOwnLocationAdded
	.public WiFiPlazaLobbyClient_GetLocationCountry
	.public WiFiPlazaLobbyClient_GetLocationRegion
	.public WiFiPlazaLobbyClient_SetLobbyCommands
	.public WiFiPlazaLobbyClient_SetChannelCommands
	.public WiFiPlazaLobbyClient_ClearLobbyCommands
	.public WiFiPlazaLobbyClient_ClearChannelCommands
	.public WiFiPlazaLobbyClient_SendToLobby
	.public WiFiPlazaLobbyClient_SendToPlayer
	.public WiFiPlazaLobbyClient_SendToChannel
	.public WiFiPlazaLobbyClient_HostMatch
	.public WiFiPlazaLobbyClient_StopMatch
	.public WiFiPlazaLobbyClient_CloseMatch
	.public WiFiPlazaLobbyClient_IsCurrentMatchClosed
	.public WiFiPlazaLobbyClient_WasMatchCancelled
	.public WiFiPlazaLobbyClient_IsMatchActive
	.public WiFiPlazaLobbyClient_IsMatchOpen
	.public WiFiPlazaLobbyClient_GetMatchFreePlaces
	.public WiFiPlazaLobbyClient_GetMatchPlayerCount
	.public WiFiPlazaLobbyClient_JoinMatch
	.public WiFiPlazaLobbyClient_GetMatchTimer
	.public WiFiPlazaLobbyClient_GetMatchHostId
	.public WiFiPlazaLobbyClient_IsMatchHost
	.public WiFiPlazaLobbyClient_IsSpecialVisitor
	.public WiFiPlazaLobbyClient_GetSpecialVisitorCode
	.public WiFiPlazaLobbyClient_SubmitSurveyAnswer
	.public WiFiPlazaLobbyClient_GetSurveySubmitState
	.public WiFiPlazaLobbyClient_GetSurveyValue
	.public WiFiPlazaLobbyClient_GetSurveyText
	.public ov45_0222F6B0
	.public WiFiPlazaLobbyClient_EndMatch
	.public WiFiPlazaLobbyClient_CheckResult
	.public WiFiPlazaLobbyClient_ClearErrors
	.public WiFiPlazaLobbyClient_AddOwnLocation
	.public WiFiPlazaLobbyClient_AllocPlayerData
	.public WiFiPlazaLobbyClient_FreePlayerData
	.public WiFiPlazaLobbyClient_SetPlayerData
	.public WiFiPlazaLobbyClient_CopyPlayerData
	.public WiFiPlazaLobbyClient_GetPlayerData
	.public WiFiPlazaLobbyClient_ClearPlayerData
	.public WiFiPlazaLobbyClient_SetPlayerRecord
	.public WiFiPlazaLobbyClient_GetPlayerRecord
	.public WiFiPlazaLobbyClient_ClearPlayerRecord
	.public WiFiPlazaLobbyClient_AllocIdLists
	.public WiFiPlazaLobbyClient_FreeIdLists
	.public WiFiPlazaLobbyClient_ClearIdList
	.public WiFiPlazaLobbyClient_AddToIdList
	.public WiFiPlazaLobbyClient_RemoveFromIdList
	.public WiFiPlazaLobbyClient_FindInIdList
	.public WiFiPlazaLobbyClient_GetIdListEntry
	.public WiFiPlazaLobbyClient_RebuildChannelList
	.public WiFiPlazaLobbyClient_HasChannelRoom
	.public WiFiPlazaLobbyClient_OnEnterChannel
	.public WiFiPlazaLobbyClient_OnPlayerEnter
	.public WiFiPlazaLobbyClient_OnPlayerLeave
	.public WiFiPlazaLobbyClient_OnMessage
	.public WiFiPlazaLobbyClient_OnLocationsChanged
	.public WiFiPlazaLobbyClient_OnPlayerRecordChanged
	.public WiFiPlazaLobbyClient_OnPlayerUpdate
	.public WiFiPlazaLobbyClient_OnMatchInfo
	.public WiFiPlazaLobbyClient_OnMatchClosed
	.public WiFiPlazaLobbyClient_OnLobbyEvent
	.public WiFiPlazaLobbyClient_OnOwnRecord
	.public ov45_02230090
	.public WiFiPlazaLobbyClient_OnSpecialVisitorsReceived
	.public WiFiPlazaLobbyClient_OnSurveyReceived
	.public WiFiPlazaLobbyClient_OnSurveySubmitted
	.public ov45_02230130
	.public WiFiPlazaLobbyClient_InitMatches
	.public WiFiPlazaLobbyMatches_Set
	.public WiFiPlazaLobbyMatches_Remove
	.public WiFiPlazaLobbyMatches_IsActive
	.public WiFiPlazaLobbyMatches_CalcTimer
	.public WiFiPlazaLobbyMatches_Get
	.public WiFiPlazaLobbyMatches_Update
	.public WiFiPlazaLobbyMatchInfo_SetStartTime
	.public WiFiPlazaLobbyMatchInfo_GetStartTime
	.public WiFiPlazaLobbyClient_OnMatchDisconnect
	.public WiFiPlazaLobbySchedule_FindValue
	.public WiFiPlazaLobbyIdValueList_Init
	.public WiFiPlazaLobbyIdValueList_Free
	.public WiFiPlazaLobbyIdValueList_Set
	.public WiFiPlazaLobbyIdValueList_Contains
	.public WiFiPlazaLobbyIdValueList_GetValue
	.public WiFiPlazaLobbyClient_CopySurveyData
	.public WiFiPlazaPlayerObjects_New
	.public WiFiPlazaPlayerObjects_Delete
	.public WiFiPlazaPlayerObjects_Update
	.public WiFiPlazaPlayerObjects_Draw
	.public WiFiPlazaPlayerObjects_SetModelAlpha
	.public WiFiPlazaPlayerObjects_Add
	.public WiFiPlazaPlayerObject_Delete
	.public WiFiPlazaPlayerObject_SetMode
	.public WiFiPlazaPlayerObject_IsCulled
	.public WiFiPlazaPlayerObject_SetGridPosition
	.public WiFiPlazaPlayerObject_SetPosition
	.public WiFiPlazaPlayerObject_GetPosition
	.public WiFiPlazaPlayerObject_SetStandingSequence
	.public WiFiPlazaPlayerObject_StartEffect
	.public WiFiPlazaPlayerObject_SetVisible
	.public WiFiPlazaPlayerObject_IsVisible
	.public WiFiPlaza_GetSpriteConfig
	.public WiFiPlaza_CalcSequenceFrame
	.public WiFiPlazaObjectModel_Load
	.public WiFiPlazaObjectModel_Free
	.public WiFiPlazaObjectModel_SetAlpha
	.public WiFiPlaza_InitModelInstance
	.public WiFiPlazaPlayerObjects_GetFreeObject
	.public WiFiPlazaPlayerObject_IsUsed
	.public WiFiPlazaPlayerObject_Clear
	.public WiFiPlazaPlayerObject_FollowPlayer
	.public WiFiPlazaPlayerObject_ShowStandingSprite
	.public WiFiPlazaPlayerObject_PlayStandingSequence
	.public ov45_02230BFC
	.public WiFiPlazaPlayerObject_PlayWalkingSequence
	.public WiFiPlazaPlayerObject_UpdateEffect
	.public WiFiPlazaPlayerObject_EffectTurnAround
	.public ov45_02230D20
	.public ov45_02230D5C
	.public ov45_02230DC4
	.public WiFiPlazaPlayerObject_UpdateCulling
	.public WiFiPlazaPlayerObject_ApplyVisibility
	.public WiFiPlazaPlayerObject_ApplyLightFlags
	.public WiFiPlaza_IsSpriteInView
	.public WiFiPlaza_GetLocalTime
	.public WiFiPlaza_GetLongitude
	.public ov45_02254A3C
	.public ov45_02254A60
	.public ov45_02254A84
	.public ov45_02254AC4
	.public ov45_02254B04
	.public ov45_02254B14
	.public ov45_02254B24
	.public ov45_02254B34
	.public ov45_02254B44
	.public ov45_02254B54
	.public ov45_02254B64
	.public ov45_02254B74
	.public ov45_02254B84
	.public ov45_02254B94
	.public ov45_02254BA4
	.public ov45_02254BB4
	.public ov45_02254BC8
	.public ov45_02254BCA
	.public ov45_02254BD2
	.public ov45_02254BDC
	.public ov45_02254BE8
	.public ov45_02254C0C
	.public ov45_02254C30
	.public ov45_02254C34
	.public ov45_02254C38
	.public ov45_02254C3C
	.public ov45_02254C48
	.public ov45_02254C98
	.public ov45_02254E54
	.public ov45_02254E88
	.public ov45_02254EBC
	.public ov45_02254EF0
	.public sStr_b_lby_wlddata
	.public sStr_pokemonplatds
	.public sStr_IIup73
	.public ov45_02254F1C
	.public ov45_02254F28
	.public WiFiPlazaWork_VBlankDummy
	.public WiFiPlazaWork_HasError
	.public WiFiPlazaWork_GetNews
	.public WiFiPlazaWork_GetCurrentTimePtr
	.public WiFiPlazaWork_GetSaveData
	.public WiFiPlazaWork_StartGameEntryCooldown
	.public WiFiPlazaWork_IsGameEntryCoolingDown
	.public ov45_0222A424
	.public ov45_0222A430
	.public WiFiPlazaWork_GetGameMembers
	.public WiFiPlazaWork_SetAppQuitForClosing
	.public WiFiPlazaWork_DidAppQuitForClosing
	.public ov45_0222A4C8
	.public WiFiPlazaPlayer_GetActivity
	.public WiFiPlazaPlayer_GetLobbyId
	.public WiFiPlazaPlayer_GetTrainerID
	.public WiFiPlazaPlayer_GetLocalTime
	.public WiFiPlazaPlayer_GetRawLanguage
	.public WiFiPlazaWork_GetOwnPlayerProfile
	.public WiFiPlazaWork_GetGroupDuration
	.public WiFiPlazaWork_GetTalkPartner
	.public WiFiPlazaWork_IsFirstTalk
	.public WiFiPlazaWork_GetTalkState
	.public WiFiPlazaWork_GetTalkPartnerMessage
	.public ov45_0222B0A4
	.public ov45_0222B0B0
	.public WiFiPlaza_ClearEventFlags
	.public WiFiPlazaUnk3A0_StartCooldown
	.public WiFiPlazaUnk3A0_IsCoolingDown
	.public WiFiPlazaUnk3A0_UpdateCooldown
	.public WiFiPlazaUnk3A0_SetAppQuitForClosing
	.public WiFiPlazaUnk3A0_DidAppQuitForClosing
	.public WiFiPlazaTalkSession_Init
	.public WiFiPlazaTalkSession_SetMessage
	.public WiFiPlazaTalkSession_StartTimeout
	.public WiFiPlazaTalkSession_StopTimeout
	.public WiFiPlazaTalkSession_GetTimeoutCounter
	.public WiFiPlaza_HandleUnusedNews
	.public WiFiPlazaGroups_Start
	.public WiFiPlazaTimer_GetCounter
	.public WiFiPlazaTimer_GetPhase
	.public WiFiPlazaHistory_IsCorrupt
	.public WiFiPlazaApps_VBlank
	.public WiFiPlazaApps_EndDisconnect
	.public WiFiPlazaNews_SetOpenTime
	.public WiFiPlazaNews_RemoveCurrentEvent
	.public WiFiPlazaNews_GetOpenTime
	.public WiFiPlazaNews_IsSlotOccupied
	.public WiFiPlazaNews_GetSlotGender
	.public WiFiPlazaNews_IsOwnSlot
	.public WiFiPlazaNews_IsOldestSlot
	.public WiFiPlazaNews_IsLastJoinedSlot
	.public WiFiPlazaNews_GetSlotChange
	.public WiFiPlazaNewsSlots_GetChange
	.public WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot
	.public WiFiPlazaNewsEvent_GetSpecialVisitorSlot
	.public WiFiPlazaLobbyClient_ToErrorCode
	.public WiFiPlazaLobbyClient_ChannelFromLib
	.public WiFiPlazaLobbyClient_ChannelToLib
	.public ov45_0222FDD4
	.public WiFiPlazaLobbyClient_OnMatchClientDummy
	.public ov45_022306F4
	.public WiFiPlazaPlayerObject_SetLightFlags
	.public WiFiPlazaPlayerObject_StopEffect
	.public WiFiPlazaPlayerObject_SetEffectStep
	.public WiFiPlaza_GetSpriteSequence
	.public WiFiPlazaObjectModel_GetAlpha
	.public WiFiPlazaPlayerObject_DrawModel
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_45_thumb.inc"
	.include "global.inc"

.text
	.rodata

_02254A28:
	.byte 0x02, 0x00, 0x50, 0x00, 0x01, 0x00, 0x02, 0x00
	.byte 0x03, 0x00, 0x8E, 0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x1E, 0x00

ov45_02254A3C: ; 0x02254A3C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x68, 0x01, 0x00, 0x00, 0xD0, 0x02, 0x00, 0x00, 0xB0, 0x04, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00
	.byte 0x80, 0x07, 0x00, 0x00, 0x60, 0x09, 0x00, 0x00, 0xC8, 0x0A, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00

ov45_02254A60: ; 0x02254A60
	.word WiFiPlaza_HandleChatNews
	.word WiFiPlaza_HandleTapToyTradeNews
	.word WiFiPlaza_HandleMinigameEntryNews
	.word WiFiPlaza_HandleFootprintStampNews
	.word WiFiPlaza_HandleVisitorProfilesNews
	.word WiFiPlaza_HandlePlazaNewsNews
	.word WiFiPlaza_HandleUnusedNews
	.word WiFiPlaza_HandleUnusedNews
	.word WiFiPlaza_HandleMinigameResultNews

ov45_02254A84: ; 0x02254A84
	.byte 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00
	.byte 0x1F, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00
	.byte 0x46, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x07, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x01, 0x00
	.byte 0x0E, 0x00, 0x01, 0x00, 0x23, 0x00, 0x01, 0x00, 0x25, 0x00, 0x01, 0x00, 0x2A, 0x00, 0x01, 0x00
	.byte 0x3F, 0x00, 0x01, 0x00

ov45_02254AC4: ; 0x02254AC4
	.word WiFiPlaza_OnTalkRequest
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_OnTalkAnswer
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_OnTalkState
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_OnTalkCancel
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_OnLobbyMessage
	.byte 0x14, 0x00, 0x00, 0x00
	.word WiFiPlaza_RecvTapToyPlayed
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_RecvJoinGroup
	.byte 0x04, 0x00, 0x00, 0x00
	.word WiFiPlaza_RecvGroupAction
	.byte 0x04, 0x00, 0x00, 0x00

ov45_02254B04: ; 0x02254B04
	.word WiFiPlazaConnect_DisconnectInit
	.word WiFiPlazaConnect_DisconnectMain
	.word WiFiPlazaConnect_DisconnectExit
	.word FS_OVERLAY_ID(OVY_46)

ov45_02254B14: ; 0x02254B14
	.word FootprintStamp_Init
	.word FootprintStamp_Main
	.word FootprintStamp_Exit
	.word FS_OVERLAY_ID(OVY_89)

ov45_02254B24: ; 0x02254B24
	.word FootprintStamp_Init
	.word FootprintStamp_Main
	.word FootprintStamp_Exit
	.word FS_OVERLAY_ID(OVY_89)

ov45_02254B34: ; 0x02254B34
	.word WobbuffetPopApp_Init
	.word WobbuffetPopApp_Main
	.word WobbuffetPopApp_Exit
	.word FS_OVERLAY_ID(OVY_93)

ov45_02254B44: ; 0x02254B44
	.word WiFiPlazaSurvey_Init
	.word WiFiPlazaSurvey_Main
	.word WiFiPlazaSurvey_Exit
	.word FS_OVERLAY_ID(OVY_47)

ov45_02254B54: ; 0x02254B54
	.word SwalotPlop_Init
	.word SwalotPlop_Main
	.word SwalotPlop_Exit
	.word FS_OVERLAY_ID(OVY_91)

ov45_02254B64: ; 0x02254B64
	.word WiFiPlazaConnect_Init
	.word WiFiPlazaConnect_Main
	.word WiFiPlazaConnect_Exit
	.word FS_OVERLAY_ID(OVY_46)

ov45_02254B74: ; 0x02254B74
	.word PlazaNews_Init
	.word PlazaNews_Main
	.word PlazaNews_Exit
	.word FS_OVERLAY_ID(OVY_88)

ov45_02254B84: ; 0x02254B84
	.word VisitorProfiles_Init
	.word VisitorProfiles_Main
	.word VisitorProfiles_Exit
	.word FS_OVERLAY_ID(OVY_48)

ov45_02254B94: ; 0x02254B94
	.word WiFiPlazaRoom_Init
	.word WiFiPlazaRoom_Main
	.word WiFiPlazaRoom_Exit
	.word FS_OVERLAY_ID(OVY_49)

ov45_02254BA4: ; 0x02254BA4
	.word MimeJrTop_Init
	.word MimeJrTop_Main
	.word MimeJrTop_Exit
	.word FS_OVERLAY_ID(OVY_92)

ov45_02254BB4: ; 0x02254BB4
	.byte 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0A, 0x0B
	.byte 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x00, 0x00

ov45_02254BC8: ; 0x02254BC8
	.byte 0x0B, 0x0A

ov45_02254BCA: ; 0x02254BCA
	.byte 0x09, 0x00, 0x16, 0x00, 0x18, 0x00
	.byte 0x19, 0x00

ov45_02254BD2: ; 0x02254BD2
	.byte 0x1A, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x1C, 0x00, 0x17, 0x00

ov45_02254BDC: ; 0x02254BDC
	.byte 0x04, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

ov45_02254BE8: ; 0x02254BE8
	.word WiFiPlazaNewsEvent_GetChatSlot
	.word WiFiPlazaNewsEvent_GetTapToyTradeSlot
	.word WiFiPlazaNewsEvent_GetMinigameEntrySlot
	.word WiFiPlazaNewsEvent_GetFootprintStampSlot
	.word WiFiPlazaNewsEvent_GetCheckingSlot
	.word WiFiPlazaNewsEvent_GetCheckingSlot
	.word WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot
	.word WiFiPlazaNewsEvent_GetSpecialVisitorSlot
	.word WiFiPlazaNewsEvent_GetMinigameResultSlot

ov45_02254C0C: ; 0x02254C0C
	.word WiFiPlazaNewsEvent_FormatChat
	.word WiFiPlazaNewsEvent_FormatTapToyTrade
	.word WiFiPlazaNewsEvent_FormatMinigameEntry
	.word WiFiPlazaNewsEvent_FormatFootprintStamp
	.word WiFiPlazaNewsEvent_FormatChecking
	.word WiFiPlazaNewsEvent_FormatChecking
	.word WiFiPlazaNewsEvent_FormatLobbyEventNotice
	.word WiFiPlazaNewsEvent_FormatSpecialVisitor
	.word WiFiPlazaNewsEvent_FormatMinigameResult

ov45_02254C30: ; 0x02254C30
	.byte 0x08, 0x08, 0x04, 0x04

ov45_02254C34: ; 0x02254C34
	.byte 0x01, 0x02, 0x00, 0x03

ov45_02254C38: ; 0x02254C38
	.byte 0x18, 0x01, 0x19, 0x01

ov45_02254C3C: ; 0x02254C3C
	.byte 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00

ov45_02254C48: ; 0x02254C48
	.byte 0x00, 0x00, 0x45, 0x80, 0x61, 0x00, 0x46, 0x80
	.byte 0x03, 0x00, 0x02, 0x00, 0x05, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x0A, 0x00, 0x1F, 0x00, 0x24, 0x00
	.byte 0x32, 0x00, 0x2F, 0x00, 0x33, 0x00, 0x30, 0x00, 0x3E, 0x00, 0x1B, 0x00, 0x46, 0x00, 0x21, 0x00
	.byte 0x06, 0x00, 0x05, 0x00, 0x07, 0x00, 0x06, 0x00, 0x0D, 0x00, 0x0D, 0x00, 0x0E, 0x00, 0x0E, 0x00
	.byte 0x23, 0x00, 0x26, 0x00, 0x25, 0x00, 0x28, 0x00, 0x2A, 0x00, 0x2B, 0x00, 0x3F, 0x00, 0x1C, 0x00
	.byte 0x1E, 0x01, 0x66, 0x00, 0x1D, 0x01, 0x65, 0x00

ov45_02254C98: ; 0x02254C98
	.byte 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x20, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00
	.byte 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00
	.byte 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00

	.data

_02254E20:
	.word WiFiPlazaApps_FreeMainArgs
	.word WiFiPlazaApps_FreeVisitorProfilesArgs
	.word WiFiPlazaApps_FreePlazaNewsArgs
	.word WiFiPlazaApps_FreeFootprintStampArgs
	.word WiFiPlazaApps_FreeFootprintStampArgs
	.word WiFiPlazaApps_FreeConnectArgs
	.word WiFiPlazaApps_FreeSwalotPlopArgs
	.word WiFiPlazaApps_FreeMimeJrTopArgs
	.word WiFiPlazaApps_FreeWobbuffetPopArgs
	.word WiFiPlazaApps_FreeDisconnectArgs
	.word WiFiPlazaApps_FreeDisconnectArgs
	.word WiFiPlazaApps_FreeSurveyArgs
	.word WiFiPlazaApps_FreeSurveyArgs

ov45_02254E54: ; 0x02254E54
	.word WiFiPlazaApps_StartMain
	.word WiFiPlazaApps_StartVisitorProfiles
	.word WiFiPlazaApps_StartPlazaNews
	.word WiFiPlazaApps_StartFootprintStampBoard0
	.word WiFiPlazaApps_StartFootprintStampBoard1
	.word WiFiPlazaApps_StartConnect
	.word WiFiPlazaApps_StartSwalotPlop
	.word WiFiPlazaApps_StartMimeJrTop
	.word WiFiPlazaApps_StartWobbuffetPop
	.word WiFiPlazaApps_StartDisconnect
	.word WiFiPlazaApps_StartClosedDisconnect
	.word WiFiPlazaApps_StartSurvey
	.word WiFiPlazaApps_StartSurvey

ov45_02254E88: ; 0x02254E88
	.word WiFiPlazaApps_NewMainArgs
	.word WiFiPlazaApps_NewVisitorProfilesArgs
	.word WiFiPlazaApps_NewPlazaNewsArgs
	.word WiFiPlazaApps_NewFootprintStampArgs
	.word WiFiPlazaApps_NewFootprintStampArgs
	.word WiFiPlazaApps_NewConnectArgs
	.word WiFiPlazaApps_NewSwalotPlopArgs
	.word WiFiPlazaApps_NewMimeJrTopArgs
	.word WiFiPlazaApps_NewWobbuffetPopArgs
	.word WiFiPlazaApps_NewDisconnectArgs
	.word WiFiPlazaApps_NewDisconnectArgs
	.word WiFiPlazaApps_NewSurveyAnswerArgs
	.word WiFiPlazaApps_NewSurveyResultsArgs

ov45_02254EBC: ; 0x02254EBC
	.word WiFiPlazaApps_EndMain
	.word WiFiPlazaApps_EndVisitorProfiles
	.word WiFiPlazaApps_EndPlazaNews
	.word WiFiPlazaApps_EndFootprintStamp
	.word WiFiPlazaApps_EndFootprintStamp
	.word WiFiPlazaApps_EndConnect
	.word WiFiPlazaApps_EndSwalotPlop
	.word WiFiPlazaApps_EndMimeJrTop
	.word WiFiPlazaApps_EndWobbuffetPop
	.word WiFiPlazaApps_EndDisconnect
	.word WiFiPlazaApps_EndDisconnect
	.word WiFiPlazaApps_EndSurvey
	.word WiFiPlazaApps_EndSurvey

ov45_02254EF0: ; 0x02254EF0
	.word sStr_b_lby_wlddata

	.balign 4, 0
sStr_b_lby_wlddata:
	.asciz "b_lby_wlddata"

	.balign 4, 0
sStr_pokemonplatds: ; 0x02254F04
	.asciz "pokemonplatds"

	.balign 4, 0
sStr_IIup73: ; 0x02254F14
	.asciz "IIup73"

	.balign 4, 0
ov45_02254F1C: ; 0x02254F1C
	.word WiFiPlazaPlayerObject_EffectTurnAround
	.word ov45_02230D20
	.word ov45_02230D5C

ov45_02254F28: ; 0x02254F28
	.word WiFiPlazaPlayerObject_ShowStandingSprite
	.word ov45_02230BFC
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayWalkingSequence
	.word WiFiPlazaPlayerObject_ShowStandingSprite
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word 0

	.bss

_022577C0:
	.space 0x4
