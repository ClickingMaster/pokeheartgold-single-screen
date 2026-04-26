#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/trainer_class.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_command.inc"
	.include "global.inc"

	.text

	thumb_func_start ov12_02248184
ov12_02248184: ; 0x02248184
	cmp r1, #0xff
	bne _0224818E
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_0224818E:
	bx lr
	thumb_func_end ov12_02248184

	thumb_func_start ov12_02248190
ov12_02248190: ; 0x02248190
	cmp r1, #0xff
	bne _0224819A
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_0224819A:
	bx lr
	thumb_func_end ov12_02248190

	thumb_func_start ov12_0224819C
ov12_0224819C: ; 0x0224819C
	push {r3, r4, r5, lr}
	add r5, r1, #0
	cmp r2, #0xff
	bne _022481AC
	mov r0, #0x4b
	lsl r0, r0, #2
	ldr r4, [r5, r0]
	b _022481C8
_022481AC:
	bl GetBattlerIDBySide
	add r1, r0, #0
	mov r0, #0xc0
	mul r0, r1
	add r2, r5, r0
	ldr r0, _022481CC ; =0x00002D67
	lsl r1, r1, #0x18
	ldrb r4, [r2, r0]
	add r0, r5, #0
	lsr r1, r1, #0x18
	add r2, r4, #0
	bl BattlerSetAbility
_022481C8:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022481CC: .word 0x00002D67
	thumb_func_end ov12_0224819C

	thumb_func_start ov12_022481D0
ov12_022481D0: ; 0x022481D0
	cmp r1, #0xff
	bne _022481DA
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_022481DA:
	bx lr
	thumb_func_end ov12_022481D0

	thumb_func_start ov12_022481DC
ov12_022481DC: ; 0x022481DC
	cmp r1, #0xff
	bne _022481E6
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_022481E6:
	bx lr
	thumb_func_end ov12_022481DC

	thumb_func_start ov12_022481E8
ov12_022481E8: ; 0x022481E8
	push {r4, lr}
	add r4, r1, #0
	bl GetBattlerIDBySide
	ldr r1, _022481FC ; =0x0000219C
	add r2, r4, r0
	ldrb r1, [r2, r1]
	lsl r1, r1, #8
	orr r0, r1
	pop {r4, pc}
	.balign 4, 0
_022481FC: .word 0x0000219C
	thumb_func_end ov12_022481E8

	thumb_func_start ov12_02248200
ov12_02248200: ; 0x02248200
	cmp r1, #0xff
	bne _0224820A
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_0224820A:
	bx lr
	thumb_func_end ov12_02248200

	thumb_func_start ov12_0224820C
ov12_0224820C: ; 0x0224820C
	cmp r1, #0xff
	bne _02248216
	mov r1, #0x13
	lsl r1, r1, #4
	ldr r0, [r0, r1]
_02248216:
	bx lr
	thumb_func_end ov12_0224820C

	thumb_func_start ov12_02248218
ov12_02248218: ; 0x02248218
	ldr r3, _0224821C ; =GetBattlerIDBySide
	bx r3
	.balign 4, 0
_0224821C: .word GetBattlerIDBySide
	thumb_func_end ov12_02248218

	thumb_func_start ov12_02248220
ov12_02248220: ; 0x02248220
	ldr r3, _02248224 ; =GetBattlerIDBySide
	bx r3
	.balign 4, 0
_02248224: .word GetBattlerIDBySide
	thumb_func_end ov12_02248220

	thumb_func_start ov12_02248228
ov12_02248228: ; 0x02248228
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x78
	add r7, r0, #0
	add r5, r1, #0
	str r2, [sp, #0x18]
	bl BattleSystem_GetMessageLoader
	str r0, [sp, #0x28]
	add r0, r7, #0
	bl BattleSystem_GetMessageBuffer
	str r0, [sp, #0x20]
	add r0, r7, #0
	bl BattleSystem_GetMessageFormat
	str r0, [sp, #0x24]
	add r0, r7, #0
	bl BattleSystem_GetBgConfig
	str r0, [sp, #0x1c]
	add r0, r7, #0
	bl BattleSystem_GetSpriteSystem
	add r6, r0, #0
	add r0, r7, #0
	bl BattleSystem_GetSpriteManager
	add r4, r0, #0
	add r0, r7, #0
	bl BattleSystem_GetPaletteData
	add r7, r0, #0
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _022484A8 ; =0x00004E35
	mov r3, #8
	str r0, [sp, #8]
	add r2, r3, #0
	add r0, r6, #0
	add r1, r4, #0
	add r3, #0xf8
	bl SpriteSystem_LoadCharResObj
	mov r0, #8
	str r0, [sp]
	mov r0, #0x52
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r1, #2
	str r1, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	ldr r0, _022484AC ; =0x00004E30
	add r2, r6, #0
	str r0, [sp, #0x14]
	add r0, r7, #0
	add r3, r4, #0
	bl SpriteSystem_LoadPaletteBuffer
	mov r0, #1
	str r0, [sp]
	ldr r0, _022484B0 ; =0x00004E2D
	mov r3, #8
	str r0, [sp, #4]
	add r2, r3, #0
	add r0, r6, #0
	add r1, r4, #0
	add r3, #0xf9
	bl SpriteSystem_LoadCellResObj
	mov r0, #1
	str r0, [sp]
	ldr r0, _022484B0 ; =0x00004E2D
	mov r3, #8
	str r0, [sp, #4]
	add r2, r3, #0
	add r0, r6, #0
	add r1, r4, #0
	add r3, #0xfa
	bl SpriteSystem_LoadAnimResObj
	ldr r2, _022484B4 ; =ov12_0226C428
	add r0, r6, #0
	add r1, r4, #0
	bl SpriteSystem_NewSprite
	str r0, [r5, #0xc]
	bl ManagedSprite_TickFrame
	ldr r0, [sp, #0x18]
	bl Pokemon_GetIconNaix
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _022484B8 ; =0x00004E36
	add r1, r4, #0
	str r0, [sp, #8]
	add r0, r6, #0
	mov r2, #0x14
	bl SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType
	bl sub_02074490
	mov r1, #0x14
	str r1, [sp]
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #3
	str r0, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	ldr r0, _022484BC ; =0x00004E31
	mov r1, #2
	str r0, [sp, #0x14]
	add r0, r7, #0
	add r2, r6, #0
	add r3, r4, #0
	bl SpriteSystem_LoadPaletteBuffer
	bl sub_0207449C
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _022484C0 ; =0x00004E2E
	add r1, r4, #0
	str r0, [sp, #4]
	add r0, r6, #0
	mov r2, #0x14
	bl SpriteSystem_LoadCellResObj
	bl sub_020744A8
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _022484C0 ; =0x00004E2E
	add r1, r4, #0
	str r0, [sp, #4]
	add r0, r6, #0
	mov r2, #0x14
	bl SpriteSystem_LoadAnimResObj
	ldr r2, _022484C4 ; =ov12_0226C45C
	add r0, r6, #0
	add r1, r4, #0
	bl SpriteSystem_NewSprite
	str r0, [r5, #0x10]
	ldr r0, [sp, #0x18]
	bl Pokemon_GetIconPalette
	add r1, r0, #0
	ldr r0, [r5, #0x10]
	ldr r0, [r0]
	bl Sprite_SetPalOffsetRespectVramOffset
	ldr r0, [r5, #0x10]
	bl ManagedSprite_TickFrame
	mov r0, #1
	mov r1, #5
	bl FontSystem_NewInit
	str r0, [r5, #0x50]
	ldr r0, [sp, #0x18]
	mov r1, #0xb0
	mov r2, #0
	bl GetMonData
	cmp r0, #0
	bne _02248390
	mov r0, #2
	b _0224839A
_02248390:
	ldr r0, [sp, #0x18]
	mov r1, #0x6f
	mov r2, #0
	bl GetMonData
_0224839A:
	cmp r0, #0
	bne _022483AC
	mov r1, #0x3b
	ldr r0, [sp, #0x28]
	lsl r1, r1, #4
	bl NewString_ReadMsgData
	add r6, r0, #0
	b _022483C6
_022483AC:
	cmp r0, #1
	bne _022483BC
	ldr r0, [sp, #0x28]
	ldr r1, _022484C8 ; =0x000003B1
	bl NewString_ReadMsgData
	add r6, r0, #0
	b _022483C6
_022483BC:
	ldr r0, [sp, #0x28]
	ldr r1, _022484CC ; =0x000003B2
	bl NewString_ReadMsgData
	add r6, r0, #0
_022483C6:
	ldr r0, [sp, #0x18]
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, [sp, #0x24]
	mov r1, #0
	bl BufferBoxMonNickname
	ldr r0, [sp, #0x18]
	mov r1, #0xa1
	mov r2, #0
	bl GetMonData
	add r2, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r1, #1
	ldr r0, [sp, #0x24]
	mov r3, #3
	str r1, [sp, #4]
	bl BufferIntegerAsString
	ldr r0, [sp, #0x24]
	ldr r1, [sp, #0x20]
	add r2, r6, #0
	bl StringExpandPlaceholders
	add r0, r6, #0
	bl String_Delete
	add r0, sp, #0x68
	bl InitWindow
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #0x1c]
	add r1, sp, #0x68
	mov r2, #0xc
	mov r3, #4
	bl AddTextWindowTopLeftCorner
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _022484D0 ; =0x00010200
	ldr r2, [sp, #0x20]
	str r0, [sp, #8]
	add r0, sp, #0x68
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, sp, #0x68
	mov r1, #1
	mov r2, #5
	bl sub_02013688
	mov r1, #1
	add r2, r1, #0
	add r3, sp, #0x5c
	bl sub_02021AC8
	ldr r0, [r5, #0x50]
	str r0, [sp, #0x2c]
	add r0, sp, #0x68
	str r0, [sp, #0x30]
	add r0, r4, #0
	bl SpriteManager_GetSpriteList
	str r0, [sp, #0x34]
	ldr r1, _022484AC ; =0x00004E30
	add r0, r4, #0
	bl SpriteManager_FindPlttResourceProxy
	mov r1, #0
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x60]
	str r1, [sp, #0x3c]
	str r0, [sp, #0x40]
	mov r0, #0xb0
	str r0, [sp, #0x44]
	mov r0, #8
	str r0, [sp, #0x48]
	mov r0, #0x64
	str r0, [sp, #0x50]
	mov r0, #1
	str r0, [sp, #0x54]
	mov r0, #5
	str r0, [sp, #0x58]
	add r0, sp, #0x2c
	str r1, [sp, #0x4c]
	bl sub_020135D8
	add r2, r5, #0
	add r3, sp, #0x5c
	str r0, [r5, #0x14]
	ldmia r3!, {r0, r1}
	add r2, #0x18
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #1
	str r0, [r2]
	ldr r0, [r5, #0x14]
	bl sub_020138E0
	add r0, sp, #0x68
	bl RemoveWindow
	add sp, #0x78
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022484A8: .word 0x00004E35
_022484AC: .word 0x00004E30
_022484B0: .word 0x00004E2D
_022484B4: .word ov12_0226C428
_022484B8: .word 0x00004E36
_022484BC: .word 0x00004E31
_022484C0: .word 0x00004E2E
_022484C4: .word ov12_0226C45C
_022484C8: .word 0x000003B1
_022484CC: .word 0x000003B2
_022484D0: .word 0x00010200
	thumb_func_end ov12_02248228

	thumb_func_start ov12_022484D4
ov12_022484D4: ; 0x022484D4
	push {r3, r4, r5, lr}
	add r5, r1, #0
	bl BattleSystem_GetSpriteManager
	add r4, r0, #0
	ldr r0, [r5, #0xc]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r5, #0x10]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r5, #0x14]
	bl FontOAM_Delete
	add r0, r5, #0
	add r0, #0x18
	bl sub_02021B5C
	ldr r1, _02248540 ; =0x00004E35
	add r0, r4, #0
	bl SpriteManager_UnloadCharObjById
	ldr r1, _02248544 ; =0x00004E30
	add r0, r4, #0
	bl SpriteManager_UnloadPlttObjById
	ldr r1, _02248548 ; =0x00004E2D
	add r0, r4, #0
	bl SpriteManager_UnloadCellObjById
	ldr r1, _02248548 ; =0x00004E2D
	add r0, r4, #0
	bl SpriteManager_UnloadAnimObjById
	ldr r1, _0224854C ; =0x00004E36
	add r0, r4, #0
	bl SpriteManager_UnloadCharObjById
	ldr r1, _02248550 ; =0x00004E31
	add r0, r4, #0
	bl SpriteManager_UnloadPlttObjById
	ldr r1, _02248554 ; =0x00004E2E
	add r0, r4, #0
	bl SpriteManager_UnloadCellObjById
	ldr r1, _02248554 ; =0x00004E2E
	add r0, r4, #0
	bl SpriteManager_UnloadAnimObjById
	ldr r0, [r5, #0x50]
	bl sub_020135AC
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02248540: .word 0x00004E35
_02248544: .word 0x00004E30
_02248548: .word 0x00004E2D
_0224854C: .word 0x00004E36
_02248550: .word 0x00004E31
_02248554: .word 0x00004E2E
	thumb_func_end ov12_022484D4

	thumb_func_start UpdateFrienshipFainted
UpdateFrienshipFainted: ; 0x02248558
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	add r1, r2, #0
	add r5, r0, #0
	str r2, [sp]
	bl BattleSystem_GetFieldSide
	cmp r0, #0
	bne _0224863C
	add r0, r5, #0
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	beq _022485A8
	add r0, r5, #0
	mov r1, #3
	bl BattleSystem_GetBattlerFromBattlerType
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	add r0, r5, #0
	mov r1, #5
	bl BattleSystem_GetBattlerFromBattlerType
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r2, #0xc0
	add r0, r1, #0
	mul r0, r2
	mul r2, r6
	ldr r3, _02248640 ; =0x00002D74
	add r0, r4, r0
	add r2, r4, r2
	ldrb r0, [r0, r3]
	ldrb r2, [r2, r3]
	cmp r0, r2
	bls _022485B4
	add r6, r1, #0
	b _022485B4
_022485A8:
	add r0, r5, #0
	mov r1, #1
	bl BattleSystem_GetBattlerFromBattlerType
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_022485B4:
	ldr r1, [sp]
	add r0, r5, #0
	add r2, r1, #0
	add r3, r4, r2
	ldr r2, _02248644 ; =0x0000219C
	ldrb r2, [r3, r2]
	bl BattleSystem_GetPartyMon
	add r7, r0, #0
	ldr r0, [sp]
	mov r1, #0xc0
	add r2, r0, #0
	mul r2, r1
	add r0, r4, r2
	ldr r2, _02248640 ; =0x00002D74
	mul r1, r6
	add r1, r4, r1
	ldrb r0, [r0, r2]
	ldrb r1, [r1, r2]
	cmp r1, r0
	bls _02248620
	sub r0, r1, r0
	cmp r0, #0x1e
	blt _02248602
	add r0, r5, #0
	bl BattleSystem_GetLocation
	add r2, r0, #0
	lsl r2, r2, #0x10
	add r0, r7, #0
	mov r1, #8
	lsr r2, r2, #0x10
	bl MonApplyFriendshipMod
	add r0, r7, #0
	mov r1, #6
	bl ApplyMonMoodModifier
	pop {r3, r4, r5, r6, r7, pc}
_02248602:
	add r0, r5, #0
	bl BattleSystem_GetLocation
	add r2, r0, #0
	lsl r2, r2, #0x10
	add r0, r7, #0
	mov r1, #6
	lsr r2, r2, #0x10
	bl MonApplyFriendshipMod
	add r0, r7, #0
	mov r1, #4
	bl ApplyMonMoodModifier
	pop {r3, r4, r5, r6, r7, pc}
_02248620:
	add r0, r5, #0
	bl BattleSystem_GetLocation
	add r2, r0, #0
	lsl r2, r2, #0x10
	add r0, r7, #0
	mov r1, #6
	lsr r2, r2, #0x10
	bl MonApplyFriendshipMod
	add r0, r7, #0
	mov r1, #4
	bl ApplyMonMoodModifier
_0224863C:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02248640: .word 0x00002D74
_02248644: .word 0x0000219C
	thumb_func_end UpdateFrienshipFainted

	thumb_func_start BattlerSetAbility
BattlerSetAbility: ; 0x02248648
	add r1, r0, r1
	mov r0, #0x39
	lsl r0, r0, #4
	strb r2, [r1, r0]
	bx lr
	.balign 4, 0
	thumb_func_end BattlerSetAbility

	thumb_func_start ov12_02248654
ov12_02248654: ; 0x02248654
	lsl r1, r1, #1
	add r1, r0, r1
	mov r0, #0xe5
	lsl r0, r0, #2
	strh r2, [r1, r0]
	bx lr
	thumb_func_end ov12_02248654

    .rodata

.public ov12_0226C2EC

ov12_0226C2EC: ; 0x0226C2EC
	.byte 20, 15, 10, 15

.public sTrumpCardPowerTable

sTrumpCardPowerTable: ; 0x0226C2F0
	.byte 200, 80, 60, 50, 40

.public sProtectSuccessChance
	.balign 4, 0
sProtectSuccessChance: ; 0x0226C2F8
	.short 0xFFFF
	.short 0x7FFF
	.short 0x3FFF
	.short 0x1FFF

.public sFlailDamageTable

; First byte: HP threshold as 64ths of max
; Second byte: Damage value
sFlailDamageTable: ; 0x0226C300
	.byte 1, 200
	.byte 5, 150
	.byte 12, 100
	.byte 21, 80
	.byte 42, 40
	.byte 64, 20

.public sPickupTable2

sPickupTable2: ; 0x0226C30C
	.short ITEM_HYPER_POTION, ITEM_NUGGET
	.short ITEM_KINGS_ROCK,ITEM_FULL_RESTORE, ITEM_ETHER, ITEM_IRON_BALL, ITEM_TM56, ITEM_ELIXIR, ITEM_TM86, ITEM_LEFTOVERS
	.short ITEM_TM26

	.balign 4, 0
ov12_0226C324: ; 0x0226C324
	.word NUM_BATTLE_STATS
	.word STAT_ATK
	.word STAT_DEF
	.word STAT_SPATK
	.word STAT_SPDEF
	.word STAT_SPEED

.public ov12_0226C33C
ov12_0226C33C: ; 0x0226C33C
	.word MON_DATA_MAX_HP
	.word MON_DATA_ATK
	.word MON_DATA_DEF
	.word MON_DATA_SP_ATK
	.word MON_DATA_SP_DEF
	.word MON_DATA_SPEED

.public ov12_0226C354
ov12_0226C354: ; 0x0226C354
	.word MON_DATA_MAX_HP
	.word MON_DATA_ATK
	.word MON_DATA_DEF
	.word MON_DATA_SP_ATK
	.word MON_DATA_SP_DEF
	.word MON_DATA_SPEED

.public ov12_0226C36C
ov12_0226C36C: ; 0x0226C36C
	.word NUM_BATTLE_STATS
	.word STAT_ATK
	.word STAT_DEF
	.word STAT_SPATK
	.word STAT_SPDEF
	.word STAT_SPEED

.public ov12_0226C384
ov12_0226C384: ; 0x0226C384
	.word MON_DATA_MAX_HP
	.word MON_DATA_ATK
	.word MON_DATA_DEF
	.word MON_DATA_SP_ATK
	.word MON_DATA_SP_DEF
	.word MON_DATA_SPEED

.public sLowKickDamageTable

sLowKickDamageTable:
	.short 100, 20
	.short 250, 40
	.short 500, 60
	.short 1000, 80
	.short 2000, 100
	.short 0xFFFF, 0xFFFF

.public sNaturePowerMoveTable

sNaturePowerMoveTable: ; 0x0226C3B4
	.short MOVE_EARTHQUAKE, MOVE_EARTHQUAKE, MOVE_SEED_BOMB, MOVE_SEED_BOMB, MOVE_ROCK_SLIDE, MOVE_ROCK_SLIDE
	.short MOVE_BLIZZARD, MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_TRI_ATTACK, MOVE_MUD_BOMB, MOVE_AIR_SLASH, MOVE_TRI_ATTACK

.public ov12_0226C3CE
	; Stat change multipliers
ov12_0226C3CE: ; 0x0226C3CE
	.byte 10, 40
	.byte 10, 35
	.byte 10, 30
	.byte 10, 25
	.byte 10, 20
	.byte 10, 15
	.byte 10, 10
	.byte 15, 10
	.byte 20, 10
	.byte 25, 10
	.byte 30, 10
	.byte 35, 10
	.byte 40, 10

.public sMoonBallPokemon

sMoonBallPokemon: ; 0x0226C3E8
	.short SPECIES_NIDORAN_F
	.short SPECIES_NIDORINA
	.short SPECIES_NIDOQUEEN
	.short SPECIES_NIDORAN_M
	.short SPECIES_NIDORINO
	.short SPECIES_NIDOKING
	.short SPECIES_CLEFFA
	.short SPECIES_CLEFAIRY
	.short SPECIES_CLEFABLE
	.short SPECIES_IGGLYBUFF
	.short SPECIES_JIGGLYPUFF
	.short SPECIES_WIGGLYTUFF
	.short SPECIES_SKITTY
	.short SPECIES_DELCATTY

.public sPickupTable1

sPickupTable1: ; 0x0226C404
	.short ITEM_POTION, ITEM_ANTIDOTE, ITEM_SUPER_POTION, ITEM_GREAT_BALL, ITEM_REPEL, ITEM_ESCAPE_ROPE
	.short ITEM_FULL_HEAL, ITEM_HYPER_POTION, ITEM_ULTRA_BALL, ITEM_REVIVE, ITEM_RARE_CANDY, ITEM_SUN_STONE, ITEM_MOON_STONE, ITEM_HEART_SCALE
	.short ITEM_FULL_RESTORE, ITEM_MAX_REVIVE, ITEM_PP_UP, ITEM_MAX_ELIXIR

ov12_0226C428: ; 0x0226C428
	.short 0x80, 0x00, 0x00
	.short 0
	.word 200, 0, 1
	.word 20021, 20016, 20013, 20013, -1, -1
	.word 1, 0

ov12_0226C45C: ; 0x0226C45C
	.short 0x98, 0x18, 0x00
	.short 0
	.word 100, 0, 1
	.word 20022, 20017, 20014, 20014, -1, -1
	.word 1, 0

.public sSecretPowerEffectTable

sSecretPowerEffectTable: ; 0x0226C490
	.word 0x8000001B
	.word 0x8000001B
	.word 0x80000001
	.word 0x80000001
	.word 0x80000008
	.word 0x80000008
	.word 0x80000004
	.word 0x80000016
	.word 0x80000004
	.word 0x80000005
	.word 0x80000018
	.word 0x8000001C
	.word 0x80000005

.public sPrizeMoneyTbl

sPrizeMoneyTbl: ; 0x0226C4C4
	.short TRAINERCLASS_PKMN_TRAINER_ETHAN, 0
	.short TRAINERCLASS_PKMN_TRAINER_LYRA, 0
	.short TRAINERCLASS_YOUNGSTER, 4
	.short TRAINERCLASS_LASS, 4
	.short TRAINERCLASS_CAMPER, 4
	.short TRAINERCLASS_PICNICKER, 4
	.short TRAINERCLASS_BUG_CATCHER, 4
	.short TRAINERCLASS_AROMA_LADY, 8
	.short TRAINERCLASS_TWINS, 4
	.short TRAINERCLASS_HIKER, 8
	.short TRAINERCLASS_BATTLE_GIRL, 4
	.short TRAINERCLASS_FISHERMAN, 8
	.short TRAINERCLASS_CYCLIST_M, 8
	.short TRAINERCLASS_CYCLIST_F, 8
	.short TRAINERCLASS_BLACK_BELT, 6
	.short TRAINERCLASS_ARTIST, 12
	.short TRAINERCLASS_PKMN_BREEDER_M, 12
	.short TRAINERCLASS_PKMN_BREEDER_F, 12
	.short TRAINERCLASS_COWGIRL, 4
	.short TRAINERCLASS_JOGGER, 8
	.short TRAINERCLASS_POKEFAN_M, 16
	.short TRAINERCLASS_POKEFAN, 16
	.short TRAINERCLASS_POKE_KID, 2
	.short TRAINERCLASS_RIVAL, 16
	.short TRAINERCLASS_ACE_TRAINER_M, 15
	.short TRAINERCLASS_ACE_TRAINER_F, 15
	.short TRAINERCLASS_WAITRESS, 8
	.short TRAINERCLASS_VETERAN, 20
	.short TRAINERCLASS_NINJA_BOY, 2
	.short TRAINERCLASS_DRAGON_TAMER, 8
	.short TRAINERCLASS_BIRD_KEEPER_GS, 8
	.short TRAINERCLASS_JUGGLER, 8
	.short TRAINERCLASS_RICH_BOY, 40
	.short TRAINERCLASS_LADY, 40
	.short TRAINERCLASS_GENTLEMAN, 50
	.short TRAINERCLASS_SOCIALITE, 50
	.short TRAINERCLASS_BEAUTY, 14
	.short TRAINERCLASS_COLLECTOR, 16
	.short TRAINERCLASS_POLICEMAN, 10
	.short TRAINERCLASS_PKMN_RANGER_M, 15
	.short TRAINERCLASS_PKMN_RANGER_F, 15
	.short TRAINERCLASS_SCIENTIST, 12
	.short TRAINERCLASS_SWIMMER_M, 4
	.short TRAINERCLASS_SWIMMER_F, 4
	.short TRAINERCLASS_TUBER_M, 1
	.short TRAINERCLASS_TUBER_F, 1
	.short TRAINERCLASS_SAILOR, 8
	.short TRAINERCLASS_KIMONO_GIRL, 30
	.short TRAINERCLASS_RUIN_MANIAC, 12
	.short TRAINERCLASS_PSYCHIC_M, 8
	.short TRAINERCLASS_PSYCHIC_F, 8
	.short TRAINERCLASS_PI, 30
	.short TRAINERCLASS_GUITARIST, 6
	.short TRAINERCLASS_ACE_TRAINER_M_GS, 15
	.short TRAINERCLASS_ACE_TRAINER_F_GS, 15
	.short TRAINERCLASS_TEAM_ROCKET, 10
	.short TRAINERCLASS_SKIER, 8
	.short TRAINERCLASS_ROUGHNECK, 6
	.short TRAINERCLASS_CLOWN, 6
	.short TRAINERCLASS_WORKER, 10
	.short TRAINERCLASS_SCHOOL_KID_M, 5
	.short TRAINERCLASS_SCHOOL_KID_F, 5
	.short TRAINERCLASS_TEAM_ROCKET_F, 10
	.short TRAINERCLASS_BURGLAR, 4
	.short TRAINERCLASS_FIREBREATHER, 8
	.short TRAINERCLASS_BIKER, 4
	.short TRAINERCLASS_LEADER_FALKNER, 30
	.short TRAINERCLASS_LEADER_BUGSY, 30
	.short TRAINERCLASS_POKE_MANIAC, 16
	.short TRAINERCLASS_LEADER_WHITNEY, 30
	.short TRAINERCLASS_LEADER_MORTY, 30
	.short TRAINERCLASS_RANCHER, 10
	.short TRAINERCLASS_LEADER_PRYCE, 30
	.short TRAINERCLASS_LEADER_JASMINE, 30
	.short TRAINERCLASS_LEADER_CHUCK, 30
	.short TRAINERCLASS_LEADER_CLAIR, 30
	.short TRAINERCLASS_TEACHER, 12
	.short TRAINERCLASS_SUPER_NERD, 12
	.short TRAINERCLASS_SAGE, 12
	.short TRAINERCLASS_MEDIUM, 12
	.short TRAINERCLASS_PARASOL_LADY, 8
	.short TRAINERCLASS_WAITER, 8
	.short TRAINERCLASS_CHAMPION, 50
	.short TRAINERCLASS_CAMERAMAN, 8
	.short TRAINERCLASS_REPORTER, 10
	.short TRAINERCLASS_IDOL, 18
	.short TRAINERCLASS_ELITE_FOUR_WILL, 30
	.short TRAINERCLASS_ELITE_FOUR_KAREN, 30
	.short TRAINERCLASS_ELITE_FOUR_KOGA, 30
	.short TRAINERCLASS_LEADER_BROCK, 30
	.short TRAINERCLASS_PKMN_TRAINER_CHERYL, 30
	.short TRAINERCLASS_PKMN_TRAINER_RILEY, 30
	.short TRAINERCLASS_PKMN_TRAINER_BUCK, 30
	.short TRAINERCLASS_PKMN_TRAINER_MIRA, 30
	.short TRAINERCLASS_PKMN_TRAINER_MARLEY, 30
	.short TRAINERCLASS_PKMN_TRAINER_FTR_LUCAS, 25
	.short TRAINERCLASS_PKMN_TRAINER_FTR_DAWN, 25
	.short TRAINERCLASS_TOWER_TYCOON, 0
	.short TRAINERCLASS_LEADER_MISTY, 30
	.short TRAINERCLASS_HALL_MATRON, 0
	.short TRAINERCLASS_FACTORY_HEAD, 0
	.short TRAINERCLASS_ARCADE_STAR, 0
	.short TRAINERCLASS_CASTLE_VALET, 0
	.short TRAINERCLASS_LEADER_LT_SURGE, 30
	.short TRAINERCLASS_LEADER_ERIKA, 30
	.short TRAINERCLASS_LEADER_JANINE, 30
	.short TRAINERCLASS_LEADER_SABRINA, 30
	.short TRAINERCLASS_LEADER_BLAINE, 30
	.short TRAINERCLASS_PKMN_TRAINER_RED, 50
	.short TRAINERCLASS_LEADER_BLUE, 40
	.short TRAINERCLASS_ELDER, 30
	.short TRAINERCLASS_ELITE_FOUR_BRUNO, 30
	.short TRAINERCLASS_SCIENTIST_GS, 8
	.short TRAINERCLASS_EXECUTIVE_ARIANA, 20
	.short TRAINERCLASS_BOARDER, 8
	.short TRAINERCLASS_EXECUTIVE_ARCHER, 20
	.short TRAINERCLASS_EXECUTIVE_PROTON, 10
	.short TRAINERCLASS_EXECUTIVE_PETREL, 10
	.short TRAINERCLASS_PASSERBY, 25
	.short TRAINERCLASS_MYSTERY_MAN, 30
	.short TRAINERCLASS_DOUBLE_TEAM, 30
	.short TRAINERCLASS_YOUNG_COUPLE, 16
	.short TRAINERCLASS_PKMN_TRAINER_LANCE, 0
	.short TRAINERCLASS_ROCKET_BOSS, 45
	.short TRAINERCLASS_PKMN_TRAINER_LUCAS_DP, 0
	.short TRAINERCLASS_PKMN_TRAINER_DAWN_DP, 0
	.short TRAINERCLASS_PKMN_TRAINER_LUCAS_PT, 0
	.short TRAINERCLASS_PKMN_TRAINER_DAWN_PT, 0
	.short TRAINERCLASS_BIRD_KEEPER, 8

.public sBattleScriptCommandTable;

sBattleScriptCommandTable: ; 0x0226C6C8
	.word BtlCmd_PlayEncounterAnimation
	.word BtlCmd_SetPokemonEncounter
	.word BtlCmd_PokemonSlideIn
	.word BtlCmd_PokemonSendOut
	.word BtlCmd_RecallPokemon ;5
	.word BtlCmd_DeletePokemon
	.word BtlCmd_SetTrainerEncounter
	.word BtlCmd_ThrowPokeball
	.word BtlCmd_TrainerSlideOut
	.word BtlCmd_TrainerSlideIn ;10
	.word BtlCmd_BackgroundSlideIn
	.word BtlCmd_HealthbarSlideIn
	.word BtlCmd_HealthbarSlideInDelay
	.word BtlCmd_HealthbarSlideOut
	.word BtlCmd_Wait ;15
	.word BtlCmd_CalcDamage
	.word BtlCmd_CalcDamageRaw
	.word BtlCmd_PrintAttackMessage
	.word BtlCmd_PrintMessage
	.word BtlCmd_PrintGlobalMessage ;20
	.word BtlCmd_PrintBufferedMessage
	.word BtlCmd_BufferMessage
	.word BtlCmd_BufferLocalMessage
	.word BtlCmd_PlayMoveAnimation
	.word BtlCmd_PlayMoveAnimationOnMons ;25
	.word BtlCmd_FlickerMon
	.word BtlCmd_UpdateHealthbarValue
	.word BtlCmd_UpdateHealthbar
	.word BtlCmd_TryFaintMon
	.word BtlCmd_PlayFaintAnimation
	.word BtlCmd_WaitButtonABTime
	.word BtlCmd_PlaySound
	.word BtlCmd_CompareVarToValue
	.word BtlCmd_CompareMonDataToValue
	.word BtlCmd_FadeOutBattle
	.word BtlCmd_GoToSubscript
	.word BtlCmd_GoToEffectScript
	.word BtlCmd_GoToMoveScript
	.word BtlCmd_CalcCrit
	.word BtlCmd_CalcExpGain
	.word BtlCmd_StartGetExpTask
	.word BtlCmd_WaitGetExpTask
	.word BtlCmd_WaitGetExpTaskLoop
	.word BtlCmd_ShowParty
	.word BtlCmd_WaitMonSelection
	.word BtlCmd_SwitchAndUpdateMon
	.word BtlCmd_GoToIfAnySwitches
	.word BtlCmd_StartCatchMonTask
	.word BtlCmd_WaitCatchMonTask
	.word BtlCmd_SetMultiHit
	.word BtlCmd_UpdateVar
	.word BtlCmd_ChangeStatStage
	.word BtlCmd_UpdateMonData
	.word BtlCmd_ClearVolatileStatus
	.word BtlCmd_ToggleVanish
	.word BtlCmd_CheckAbility
	.word BtlCmd_Random
	.word BtlCmd_UpdateVar2
	.word BtlCmd_UpdateMonDataFromVar
	.word BtlCmd_Goto
	.word BtlCmd_Call
	.word BtlCmd_CallFromVar
	.word BtlCmd_SetMirrorMove
	.word BtlCmd_ResetAllStatChanges
	.word BtlCmd_LockMoveChoice
	.word BtlCmd_UnlockMoveChoice
	.word BtlCmd_SetHealthbarStatus
	.word BtlCmd_PrintTrainerMessage
	.word BtlCmd_PayPrizeMoney
	.word BtlCmd_PlayBattleAnimation
	.word BtlCmd_PlayBattleAnimationOnMons
	.word BtlCmd_PlayBattleAnimationFromVar
	.word BtlCmd_PrintRecallMessage
	.word BtlCmd_PrintSendOutMessage
	.word BtlCmd_PrintEncounterMessage
	.word BtlCmd_PrintFirstSendOutMessage
	.word BtlCmd_PrintBufferedTrainerMessage
	.word BtlCmd_TryConversion
	.word BtlCmd_CompareVarToVar
	.word BtlCmd_CompareMonDataToVar
	.word BtlCmd_AddPayDayMoney
	.word BtlCmd_TryLightScreen
	.word BtlCmd_TryReflect
	.word BtlCmd_TryMist
	.word BtlCmd_TryOHKOMove
	.word BtlCmd_DivideVarByValue
	.word BtlCmd_DivideVarByVar
	.word BtlCmd_TryMimic
	.word BtlCmd_Metronome
	.word BtlCmd_TryDisable
	.word BtlCmd_Counter
	.word BtlCmd_MirrorCoat
	.word BtlCmd_TryEncore
	.word BtlCmd_TryConversion2
	.word BtlCmd_TrySketch
	.word BtlCmd_TrySleepTalk
	.word BtlCmd_CalcFlailPower
	.word BtlCmd_TrySpite
	.word BtlCmd_TryPartyStatusRefresh
	.word BtlCmd_TryStealItem
	.word BtlCmd_TryProtection
	.word BtlCmd_TrySubstitute
	.word BtlCmd_TryWhirlwind
	.word BtlCmd_Transform
	.word BtlCmd_TrySpikes
	.word BtlCmd_CheckSpikes
	.word BtlCmd_TryPerishSong
	.word BtlCmd_GetTurnOrderBySpeed
	.word BtlCmd_GoToIfValidMon
	.word BtlCmd_EndOfTurnWeatherEffect
	.word BtlCmd_CalcRolloutPower
	.word BtlCmd_CalcFuryCutterPower
	.word BtlCmd_TryAttract
	.word BtlCmd_TrySafeguard
	.word BtlCmd_Present
	.word BtlCmd_CalcMagnitudePower
	.word BtlCmd_TryReplaceFaintedMon
	.word BtlCmd_RapidSpin
	.word BtlCmd_WeatherHPRecovery
	.word BtlCmd_CalcHiddenPowerParams
	.word BtlCmd_CopyStatStages
	.word BtlCmd_TryFutureSight
	.word BtlCmd_CheckMoveHit
	.word BtlCmd_TryTeleport
	.word BtlCmd_BeatUp
	.word BtlCmd_FollowMe
	.word BtlCmd_TryHelpingHand
	.word BtlCmd_TrySwapItems
	.word BtlCmd_TryWish
	.word BtlCmd_TryAssist
	.word BtlCmd_TrySetMagicCoat
	.word BtlCmd_MagicCoat
	.word BtlCmd_CalcRevengeDamageMul
	.word BtlCmd_TryBreakScreens
	.word BtlCmd_TryYawn
	.word BtlCmd_TryKnockOff
	.word BtlCmd_CalcHPFalloffPower
	.word BtlCmd_TryImprison
	.word BtlCmd_TryGrudge
	.word BtlCmd_TrySnatch
	.word BtlCmd_CalcWeightBasedPower
	.word BtlCmd_CalcWeatherBallParams
	.word BtlCmd_TryPursuit
	.word BtlCmd_ApplyTypeEffectiveness
	.word BtlCmd_IfTurnFlag
	.word BtlCmd_SetTurnFlag
	.word BtlCmd_CalcGyroBallPower
	.word BtlCmd_TryMetalBurst
	.word BtlCmd_CalcPaybackPower
	.word BtlCmd_CalcTrumpCardPower
	.word BtlCmd_CalcWringOutPower
	.word BtlCmd_TryMeFirst
	.word BtlCmd_TryCopycat
	.word BtlCmd_CalcPunishmentPower
	.word BtlCmd_TrySuckerPunch
	.word BtlCmd_CheckSideCondition
	.word BtlCmd_TryFeint
	.word BtlCmd_TryPyschoShift
	.word BtlCmd_TryLastResort
	.word BtlCmd_TryToxicSpikes
	.word BtlCmd_CheckToxicSpikes
	.word BtlCmd_CheckIgnorableAbility
	.word BtlCmd_IfSameSide
	.word BtlCmd_GenerateEndOfBattleItem
	.word BtlCmd_TrickRoom
	.word BtlCmd_IfMovedThisTurn
	.word BtlCmd_CheckItemHoldEffect
	.word BtlCmd_GetItemHoldEffect
	.word BtlCmd_GetItemEffectParam
	.word BtlCmd_TryCamouflage
	.word BtlCmd_GetTerrainMove
	.word BtlCmd_GetTerrainSecondaryEffect
	.word BtlCmd_CalcNaturalGiftParams
	.word BtlCmd_TryPluck
	.word BtlCmd_TryFling
	.word BtlCmd_YesNoMenu
	.word BtlCmd_WaitYesNoResult
	.word BtlCmd_ChoosePokemonMenu
	.word BtlCmd_WaitPokemonMenuResult
	.word BtlCmd_SetLinkBattleResult
	.word BtlCmd_CheckStealthRock
	.word BtlCmd_CheckEffectActivation
	.word BtlCmd_CheckChatterActivation
	.word BtlCmd_GetCurrentMoveData
	.word BtlCmd_SetMosaic
	.word BtlCmd_ChangeForm
	.word BtlCmd_SetBattleBackground
	.word BtlCmd_UseBagItem
	.word BtlCmd_TryEscape
	.word BtlCmd_ShowBattleStartPartyGauge
	.word BtlCmd_HideBattleStartPartyGauge
	.word BtlCmd_ShowPartyGauge
	.word BtlCmd_HidePartyGauge
	.word BtlCmd_LoadPartyGaugeGraphics
	.word BtlCmd_FreePartyGaugeGraphics
	.word BtlCmd_IncrementGameStat
	.word BtlCmd_RestoreSprite
	.word BtlCmd_TriggerAbilityOnHit
	.word BtlCmd_SpriteToOAM
	.word BtlCmd_OAMToSprite
	.word BtlCmd_CheckWhiteout
	.word BtlCmd_BoostRandomStatBy2
	.word BtlCmd_RemoveItem
	.word BtlCmd_TryRecycle
	.word BtlCmd_CheckItemHoldEffectOnHit
	.word BtlCmd_PrintBattleResultMessage
	.word BtlCmd_PrintEscapeMessage
	.word BtlCmd_PrintForfeitMessage
	.word BtlCmd_CheckHoldOnWith1HP
	.word BtlCmd_TryRestoreStatusOnSwitch
	.word BtlCmd_CheckSubstitute
	.word BtlCmd_CheckIgnoreWeather
	.word BtlCmd_SetRandomTarget
	.word BtlCmd_CheckItemHoldEffectOnUTurn
	.word BtlCmd_RefreshSprite
	.word BtlCmd_PlayMoveHitSound
	.word BtlCmd_PlayBGM
	.word BtlCmd_CheckSafariGameDone
	.word BtlCmd_WaitTime
	.word BtlCmd_CheckCurMoveIsType
	.word BtlCmd_LoadArchivedMonData
	.word BtlCmd_RefreshMonData
	.word BtlCmd_222
	.word BtlCmd_223
	.word BtlCmd_EndScript

.public sPickupWeightTable

sPickupWeightTable: ; 0x0226CA4C
	.byte 30, 40, 50, 60, 70, 80, 90, 94, 98

.public sHoneyGatherChanceTable

	.balign 4, 0
sHoneyGatherChanceTable: ; 0x0226CA58
	.byte 5, 10, 15, 20, 25, 30, 35, 40, 45, 50

.public sCamouflageTypeTable

	.balign 4, 0
sCamouflageTypeTable: ; 0x0226CA64
	.byte TYPE_GROUND, TYPE_GROUND, TYPE_GRASS, TYPE_GRASS, TYPE_ROCK, TYPE_ROCK, TYPE_ICE, TYPE_WATER, TYPE_ICE, TYPE_NORMAL, TYPE_GROUND, TYPE_FLYING
	.byte TYPE_NORMAL
