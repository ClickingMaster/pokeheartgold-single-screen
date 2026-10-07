#include "constants/maps.h"
#include "constants/phone_scripts.h"
#include "constants/sndseq.h"

#include "overlay_2/overlay_02_gear_phone.h"

#include "map_header.h"
#include "math_util.h"
#include "msgdata.h"
#include "phonebook_dat.h"
#include "sav_system_info.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "unk_02092BE8.h"

#include "to91_phone_internal.h"

extern u32 _u32_div_f(u32 a1, u32 a2);
extern s32 _s32_div_f(s32 a1, s32 a2);
asm u32 ov02_02251FDC(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	str r0, [sp, #8]
	ldr r0, [r0, #0x38]
	add r6, r1, #0
	str r2, [sp, #0xc]
	bl Save_VarsFlags_Get
	str r0, [sp, #0x1c]
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_PlayerData_GetProfile
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_Misc_Get
	str r0, [sp, #0x18]
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x30]
	bl SavePokegear_FindEmptyPhonebookSlot
	str r0, [sp, #0x2c]
	ldr r0, [sp, #8]
	mov r1, #4
	ldr r0, [r0, #0x30]
	bl SavePokegear_AllocAndCopyPhonebook
	str r0, [sp, #0x20]
	ldr r1, [sp, #0x2c]
	mov r0, #4
	bl Heap_AllocAtEnd
	ldr r2, [sp, #0x2c]
	mov r1, #0xff
	add r4, r0, #0
	bl MI_CpuFill8
	bl LCRandom
	mov r1, #0xfa
	lsl r1, r1, #2
	bl _s32_div_f
	lsl r0, r1, #0x10
	lsr r1, r0, #0x10
	mov r0, #0x7d
	lsl r0, r0, #2
	cmp r1, r0
	bhs _02252046
	mov r0, #0
	str r0, [sp, #0x24]
	b _02252058
_02252046:
	mov r0, #0x32
	lsl r0, r0, #4
	cmp r1, r0
	bhs _02252054
	mov r0, #1
	str r0, [sp, #0x24]
	b _02252058
_02252054:
	mov r0, #2
	str r0, [sp, #0x24]
_02252058:
	ldr r0, [sp, #0x2c]
	mov r7, #0
	add r2, r7, #0
	cmp r0, #0
	bls _022520A2
_02252062:
	ldr r0, [sp, #0x20]
	mov r1, #0x14
	ldrb r3, [r0, r2]
	ldr r0, [r6, #4]
	mul r1, r3
	add r5, r0, r1
	ldrb r1, [r5, #0xf]
	ldr r0, [sp, #0x24]
	cmp r0, r1
	bne _02252096
	ldrb r0, [r5, #1]
	cmp r0, #0
	beq _0225208C
	cmp r0, #0xc
	beq _0225208C
	cmp r0, #0xb
	beq _0225208C
	cmp r0, #0xa
	beq _0225208C
	cmp r0, #0xe
	bne _02252096
_0225208C:
	add r1, r7, #1
	add r0, r7, #0
	lsl r1, r1, #0x18
	lsr r7, r1, #0x18
	strb r3, [r4, r0]
_02252096:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	ldr r0, [sp, #0x2c]
	cmp r2, r0
	blo _02252062
_022520A2:
	cmp r7, #0
	bne _022520B8
	ldr r0, [sp, #0x20]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	add sp, #0x30
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_022520B8:
	ldr r0, [sp, #0x20]
	ldr r2, [sp, #0x2c]
	mov r1, #0xff
	bl MI_CpuFill8
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_SysInfo_RTC_Get
	ldr r0, [r0, #0x14]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	bl Save_VarsFlags_CheckBugContestFlag
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AAD4
	cmp r0, r7
	bne _022520F6
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AA9C
_022520F6:
	mov r5, #0
	cmp r7, #0
	bls _02252164
_022520FC:
	ldrb r2, [r4, r5]
	mov r1, #0x14
	ldr r0, [r6, #4]
	mul r1, r2
	add r0, r0, r1
	ldrh r1, [r0, #6]
	ldr r0, [sp, #0xc]
	cmp r0, r1
	beq _0225215A
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AA44
	cmp r0, #0
	bne _0225215A
	ldrb r0, [r4, r5]
	cmp r0, #8
	bne _02252134
	ldr r0, [sp, #0x14]
	bl sub_02095FF8
	cmp r0, #0
	bne _0225215A
	ldr r0, [sp, #0x1c]
	bl Save_VarsFlags_IsInRocketTakeover
	cmp r0, #0
	bne _0225215A
_02252134:
	ldrb r1, [r4, r5]
	mov r2, #0x14
	ldr r0, [r6, #4]
	mul r2, r1
	add r0, r0, r2
	ldrh r0, [r0, #6]
	cmp r0, #0x60
	bne _0225214A
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0225215A
_0225214A:
	ldr r0, [sp, #0x28]
	add r2, r0, #0
	add r2, r2, #1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x28]
	ldr r2, [sp, #0x20]
	strb r1, [r2, r0]
_0225215A:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r7
	blo _022520FC
_02252164:
	ldr r0, [sp, #0x28]
	cmp r0, #0
	beq _022521AC
	bl MTRandom
	ldr r2, [sp, #0x28]
	mov r1, #0x64
	mul r1, r2
	bl _u32_div_f
	add r0, r1, #0
	mov r1, #0x64
	bl _u32_div_f
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0
	lsl r1, r1, #0x18
	str r0, [sp]
	lsr r2, r1, #0x18
	str r0, [sp, #4]
	ldr r1, [sp, #0x20]
	ldr r0, [sp, #8]
	ldrb r1, [r1, r2]
	ldr r2, [sp, #0x24]
	add r3, r7, #0
	bl GearPhoneRingManager_SetCallerParams
	ldr r0, [sp, #8]
	bl GearPhoneRingManager_StartRinging
	ldr r0, [sp, #8]
	mov r1, #1
	bl ov02_022522AC
_022521AC:
	ldr r0, [sp, #0x20]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [sp, #0x28]
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
}
