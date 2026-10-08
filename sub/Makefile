BUILD_DIR    := build
# Because mwldarm expects absolute paths to be WIN32 paths,
# all paths referring up from BUILD_DIR must be relative.
BACK_REL     := ..
TOOLSDIR     := ../tools
ELFNAME      := ichneumon_sub
MWCCVER      := 2.0/sp2p3
PROC         := arm7tdmi
PROC_S       := arm4t
PROC_LD      := v4t
LCF_TEMPLATE := ARM7-TS.lcf.template
OPTFLAGS     := -O4,s
GLB_DEFINES  := -DSDK_ARM7

include ../common.mk

tidy:
	$(RM) -r $(BUILD_DIR)

clean: tidy

all:
	$(MAKE) patch_mwasmarm 
	$(MAKE) target

target: $(SBIN) $(ELF)

compare: COMPARE := 1
compare: all

include $(WORK_DIR)/tools/arm7-alarm-partition/alarm.mk

# Keep the original reset flag labels on their single BSS allocation.
$(BUILD_DIR)/lib/src/OS_reset.o: lib/src/OS_reset.c $(BUILD_DIR)/lib/src/OS_reset.d $(WORK_DIR)/tools/nitro-section-renames/add_arm7_reset_aliases.py
	@echo $(BUILD_C) $@ $<
	@$(BUILD_C) $@ $<
	@python3 $(WORK_DIR)/tools/nitro-section-renames/add_arm7_reset_aliases.py "$@" "$@.reset-aliases"
	@mv "$@.reset-aliases" "$@"
	@$(call fixdep,$(BUILD_DIR)/lib/src/OS_reset.d)
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_37_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_32_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_36_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_51_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_62_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_62_r02.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_58_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_63_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_12_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_15_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_16_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_17_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to90_arm7_sub_ext_wram_residual_45_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_to91_w41462_default_suffix_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_op90_to90_arm7_sub_ext_wram_residual_21_residual_2_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/src/op90_op90_to90_arm7_sub_ext_wram_residual_21_residual_2_r02.o: private MWCCVER := 2.0/sp1
