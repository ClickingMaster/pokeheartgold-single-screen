#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_020863F4.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text
	.public _021027C8
	.public _021027DC
	.public _021027F4
	.public sub_020863F4
	.public sub_02086490
	.public sub_020866CC
	.public sub_02086758
	.public sub_020868A0
	.public sub_020869BC
	.public sub_02086AB4
	.public sub_02086AE4
	.public sub_02086B2C
	.public sub_02086B6C
	.public sub_02086BB4
	.public sub_02086C80
	.public sub_02086C8C
	.public sub_02086D98
	.public sub_02086DA4
	.public sub_02086DE4
	.public sub_02086F44
	.public sub_02086FCC
	.public sub_02086FE8
	.public sub_02087028
	.public sub_02087064
	.public sub_02087090
	.public sub_020871C4
	.public sub_02087230

	thumb_func_start sub_020869BC
sub_020869BC: ; 0x020869BC
	push {r3, r4, r5, r6, lr}
	sub sp, #0x34
	mov r2, #0x2f
	lsl r2, r2, #4
	add r5, r0, #0
	add r0, r2, #4
	ldr r6, [r5, r2]
	ldr r4, [r5, r0]
	mov r0, #0
	add r1, sp, #0
	strh r0, [r1]
	strh r0, [r1, #2]
	strh r0, [r1, #4]
	strh r0, [r1, #6]
	add r2, #0xfa
	mov r1, #0xa
	str r1, [sp, #8]
	mov r1, #1
	str r1, [sp, #0x10]
	sub r1, r1, #2
	str r0, [sp, #0xc]
	str r0, [sp, #0x2c]
	str r0, [sp, #0x30]
	str r0, [sp, #0xc]
	str r2, [sp, #0x14]
	str r2, [sp, #0x18]
	str r2, [sp, #0x1c]
	str r2, [sp, #0x20]
	str r1, [sp, #0x24]
	str r1, [sp, #0x28]
	add r0, r6, #0
	add r1, r4, #0
	add r2, sp, #0
	bl SpriteSystem_NewSprite
	mov r1, #0x9d
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #1
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r1, r4, #0
	add r2, sp, #0
	bl SpriteSystem_NewSprite
	mov r2, #0x29
	lsl r2, r2, #4
	add r1, r2, #0
	str r0, [r5, r2]
	add r1, #0xde
	ldrb r3, [r5, r1]
	add r1, r2, #0
	add r1, #0xdf
	ldrb r1, [r5, r1]
	add r0, r2, #0
	sub r0, #0x1c
	add r3, r3, r1
	lsr r1, r3, #0x1f
	add r1, r3, r1
	add r3, r2, #0
	add r3, #0xdc
	add r2, #0xdd
	lsl r1, r1, #0xf
	ldrb r3, [r5, r3]
	ldrb r2, [r5, r2]
	ldr r0, [r5, r0]
	asr r1, r1, #0x10
	add r3, r3, r2
	lsr r2, r3, #0x1f
	add r2, r3, r2
	lsl r2, r2, #0xf
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	mov r0, #0x9d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
	mov r0, #0x9d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl ManagedSprite_TickFrame
	mov r2, #0x29
	lsl r2, r2, #4
	add r1, r2, #0
	add r1, #0xe2
	ldrb r3, [r5, r1]
	add r1, r2, #0
	add r1, #0xe3
	ldrb r1, [r5, r1]
	ldr r0, [r5, r2]
	add r3, r3, r1
	lsr r1, r3, #0x1f
	add r1, r3, r1
	add r3, r2, #0
	add r3, #0xe0
	add r2, #0xe1
	lsl r1, r1, #0xf
	ldrb r3, [r5, r3]
	ldrb r2, [r5, r2]
	asr r1, r1, #0x10
	add r3, r3, r2
	lsr r2, r3, #0x1f
	add r2, r3, r2
	lsl r2, r2, #0xf
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	mov r0, #0x29
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
	mov r0, #0x29
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl ManagedSprite_TickFrame
	add sp, #0x34
	pop {r3, r4, r5, r6, pc}
	thumb_func_end sub_020869BC
