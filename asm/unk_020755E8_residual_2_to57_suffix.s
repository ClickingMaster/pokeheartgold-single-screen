#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "unk_020755E8.inc"
	.include "global.inc"

	.public _020FFEC0
	.public _020FFED8
	.public _020FFF34
	.public _020FFF90
	.public _020FFFE4

	.text
	.public sub_020755E8
	.public sub_02075630
	.public sub_02075770
	.public sub_020757AC
	.public sub_02075804
	.public sub_0207584C
	.public sub_020758D0
	.public sub_020759CC
	.public sub_02075A04
	.public sub_02075A7C
	.public sub_02075D08
	.public sub_02075D3C
	.public sub_02075D4C
	.public sub_02075E14
	.public sub_02076C90
	.public sub_02076E64
	.public sub_020771A0
	.public sub_020771E8
	.public sub_02077270
	.public sub_020772F8
	.public sub_02077340
	.public sub_02077394
	.public sub_020755E8
	.public sub_02075630
	.public sub_02075770
	.public sub_020757AC
	.public sub_02075804
	.public sub_0207584C
	.public sub_020758D0
	.public sub_020759CC
	.public sub_02075A04
	.public sub_02075D08
	.public sub_02075D3C
	.public sub_02075D4C
	.public sub_020771A0
	.public sub_020771E8
	.public sub_020772F8
	.public sub_02077340
	.public sub_02077394

	thumb_func_start sub_02076E64
sub_02076E64: ; 0x02076E64
	push {r4, r5, r6, lr}
	sub sp, #0xf0
	add r5, r0, #0
	add r4, r1, #0
	bl GfGfx_DisableEngineAPlanes
	ldr r6, _02077180 ; =_020FFF34
	add r3, sp, #0x20
	mov r2, #5
_02076E76:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02076E76
	add r0, sp, #0x20
	bl GfGfx_SetBanks
	mov r1, #6
	mov r2, #2
	mov r0, #0
	lsl r1, r1, #0x18
	lsl r2, r2, #0x12
	bl MIi_CpuClear32
	mov r1, #0x62
	mov r2, #2
	mov r0, #0
	lsl r1, r1, #0x14
	lsl r2, r2, #0x10
	bl MIi_CpuClear32
	mov r1, #0x19
	mov r2, #1
	mov r0, #0
	lsl r1, r1, #0x16
	lsl r2, r2, #0x12
	bl MIi_CpuClear32
	mov r1, #0x66
	mov r2, #2
	mov r0, #0
	lsl r1, r1, #0x14
	lsl r2, r2, #0x10
	bl MIi_CpuClear32
	ldr r6, _02077184 ; =_020FFED8
	add r3, sp, #0x10
	add r2, r3, #0
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r6, _02077188 ; =_020FFF90
	add r3, sp, #0x9c
	mov r2, #0xa
_02076ED6:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02076ED6
	ldr r0, [r6]
	mov r1, #1
	str r0, [r3]
	add r0, r4, #0
	add r2, sp, #0x9c
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #1
	bl BgClearTilemapBufferAndCommit
	add r0, r4, #0
	mov r1, #2
	add r2, sp, #0xb8
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	add r0, r4, #0
	mov r1, #3
	add r2, sp, #0xd4
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	ldr r1, _0207718C ; =0x04000008
	mov r0, #3
	ldrh r2, [r1]
	bic r2, r0
	mov r0, #1
	orr r2, r0
	strh r2, [r1]
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	ldr r6, _02077190 ; =_020FFFE4
	add r3, sp, #0x48
	mov r2, #0xa
_02076F38:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02076F38
	ldr r0, [r6]
	mov r1, #4
	str r0, [r3]
	add r0, r4, #0
	add r2, sp, #0x48
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #4
	bl BgClearTilemapBufferAndCommit
	add r0, r4, #0
	mov r1, #5
	add r2, sp, #0x64
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #5
	bl BgClearTilemapBufferAndCommit
	add r0, r4, #0
	mov r1, #6
	add r2, sp, #0x80
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r5, #0x2c]
	bl Options_GetFrame
	add r6, r0, #0
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldr r0, [r5, #0x5c]
	mov r1, #1
	str r0, [sp, #4]
	add r0, r4, #0
	add r2, r1, #0
	mov r3, #0xa
	bl sub_0200EB80
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r5, #0x5c]
	add r2, r4, #0
	str r0, [sp, #0xc]
	mov r0, #0x73
	mov r3, #3
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	ldr r0, [r5, #0x5c]
	add r2, r4, #0
	str r0, [sp, #0xc]
	mov r0, #0x73
	mov r3, #3
	bl GfGfxLoader_LoadScrnData
	mov r1, #0
	str r1, [sp]
	mov r0, #0x40
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0x73
	mov r2, #8
	bl PaletteData_LoadNarc
	add r0, r6, #0
	bl sub_0200E640
	add r2, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0xa0
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0x26
	bl PaletteData_LoadNarc
	mov r0, #0
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0xb0
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0x10
	mov r2, #8
	bl PaletteData_LoadNarc
	ldr r0, [r5, #0x5c]
	mov r1, #2
	str r0, [sp]
	ldr r0, [r5]
	mov r2, #1
	mov r3, #0
	bl sub_0200E398
	bl sub_0200E3D8
	add r2, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0x80
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0x26
	bl PaletteData_LoadNarc
	mov r0, #1
	str r0, [sp]
	mov r0, #0xa0
	str r0, [sp, #4]
	mov r2, #0
	str r2, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0xef
	bl PaletteData_LoadNarc
	mov r0, #1
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0x90
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0xef
	mov r2, #0xf
	bl PaletteData_LoadNarc
	mov r0, #1
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0xf0
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	ldr r3, [r5, #0x5c]
	mov r1, #0x10
	mov r2, #9
	bl PaletteData_LoadNarc
	ldr r1, [r5, #0x5c]
	mov r0, #0xef
	bl NARC_New
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	ldr r1, [r5, #0x5c]
	add r2, r4, #0
	str r1, [sp, #0xc]
	mov r1, #0x10
	mov r3, #4
	add r6, r0, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r0, [r5, #0x5c]
	mov r1, #0x11
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r2, r4, #0
	mov r3, #4
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r0, [r5, #0x5c]
	mov r1, #1
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r2, r4, #0
	mov r3, #5
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r0, [r5, #0x5c]
	mov r1, #0xa
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r2, r4, #0
	mov r3, #5
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	add r0, r6, #0
	bl NARC_Delete
	mov r0, #5
	mov r1, #0
	bl ToggleBgLayer
	mov r0, #6
	mov r1, #0
	bl ToggleBgLayer
	mov r0, #1
	lsl r0, r0, #0x1a
	ldr r1, [r0]
	ldr r2, _02077194 ; =0xFFFF1FFF
	add r3, r1, #0
	and r3, r2
	lsr r1, r0, #0xd
	orr r1, r3
	ldr r3, _02077198 ; =0x04001000
	str r1, [r0]
	ldr r1, [r3]
	and r1, r2
	str r1, [r3]
	add r3, r0, #0
	add r3, #0x48
	ldrh r4, [r3]
	mov r1, #0x3f
	mov r2, #0x1f
	bic r4, r1
	orr r2, r4
	strh r2, [r3]
	add r0, #0x4a
	ldrh r2, [r0]
	bic r2, r1
	mov r1, #0x12
	orr r1, r2
	strh r1, [r0]
	add r0, r5, #0
	mov r1, #0
	add r0, #0x72
	strb r1, [r0]
	add r0, r5, #0
	add r0, #0x73
	strb r1, [r0]
	add r0, r5, #0
	mov r1, #0xff
	add r0, #0x74
	strb r1, [r0]
	add r0, r5, #0
	mov r1, #0xa0
	add r0, #0x75
	strb r1, [r0]
	bl GfGfx_BothDispOn
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, _0207719C ; =sub_02077270
	add r1, r5, #0
	bl Main_SetVBlankIntrCB
	add sp, #0xf0
	pop {r4, r5, r6, pc}
	nop
_02077180: .word _020FFF34
_02077184: .word _020FFED8
_02077188: .word _020FFF90
_0207718C: .word 0x04000008
_02077190: .word _020FFFE4
_02077194: .word 0xFFFF1FFF
_02077198: .word 0x04001000
_0207719C: .word sub_02077270
	thumb_func_end sub_02076E64
