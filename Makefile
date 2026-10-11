MWCCVER        := 2.0/sp2p2
PROC           := arm946e
PROC_S         := arm5te
PROC_LD        := v5te
LCF_TEMPLATE   := ARM9-TS.lcf.template
LIBS           := -Llib -lsyscall -nostdlib
OPTFLAGS       := -O4,p

include config.mk

ALL_BUILDDIRS  := $(BUILD_DIR)/lib
include common.mk

# crt0 is now C (asm functions); keep basename crt0.o for the lcf.
CRT0_OBJ := lib/NitroSDK/src/crt0.o
include filesystem.mk

$(ASM_OBJS): private MWASFLAGS += -DPM_ASM

$(BUILD_DIR)/asm/nitrocrypto.o:  private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/nitrocrypto_run01.o $(BUILD_DIR)/src/nitrocrypto_run02.o $(BUILD_DIR)/src/nitrocrypto_run03.o $(BUILD_DIR)/src/nitrocrypto_run04.o $(BUILD_DIR)/src/nitrocrypto_run05.o $(BUILD_DIR)/src/nitrocrypto_run06.o $(BUILD_DIR)/src/nitrocrypto_run07.o $(BUILD_DIR)/src/nitrocrypto_run08.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/nitrocrypto_run01.o $(BUILD_DIR)/src/nitrocrypto_run02.o $(BUILD_DIR)/src/nitrocrypto_run03.o $(BUILD_DIR)/src/nitrocrypto_run04.o $(BUILD_DIR)/src/nitrocrypto_run05.o $(BUILD_DIR)/src/nitrocrypto_run06.o $(BUILD_DIR)/src/nitrocrypto_run07.o $(BUILD_DIR)/src/nitrocrypto_run08.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to55_nitrocrypto_residual01_sign.o $(BUILD_DIR)/src/to55_nitrocrypto_residual01_bignum.o $(BUILD_DIR)/src/to55_nitrocrypto_residual02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to55_nitrocrypto_residual01_sign.o $(BUILD_DIR)/src/to55_nitrocrypto_residual01_bignum.o $(BUILD_DIR)/src/to55_nitrocrypto_residual02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to57_nitrocrypto_residual04_prefix.o $(BUILD_DIR)/src/to57_nitrocrypto_residual04_suffix.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to57_nitrocrypto_residual04_prefix.o $(BUILD_DIR)/src/to57_nitrocrypto_residual04_suffix.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/msl/src/*.o:    private EXCCFLAGS := -Cpp_exceptions on

$(ASM_OBJS): $(WORK_DIR)/include/config.h
$(C_OBJS) $(CXX_OBJS): $(WORK_DIR)/include/global.h

ROM             := $(BUILD_DIR)/poke$(buildname).nds
BANNER          := $(ROM:%.nds=%.bnr)
BANNER_SPEC     := $(buildname)/banner.bsf
ICON_PNG        := $(buildname)/icon.png
HEADER_TEMPLATE := $(buildname)/rom_header_template.sbin

.PHONY: main sub dsprot libsyscall sdk sdk9 sdk7
.PRECIOUS: $(ROM)

MAKEFLAGS += --no-print-directory

all:
	$(MAKE) tools
	$(MAKE) patch_mwasmarm
	$(MAKE) $(ROM)

tidy:
	@$(MAKE) -C lib/dsprot tidy
	@$(MAKE) -C lib/syscall tidy
	@$(MAKE) -C sub tidy
	$(RM) -r build
	$(RM) -r $(PROJECT_CLEAN_TARGETS)
	$(RM) $(ROM)

clean: tidy clean-filesystem clean-tools
	@$(MAKE) -C lib/dsprot clean
	@$(MAKE) -C lib/syscall clean
	@$(MAKE) -C sub clean
	$(RM) $(foreach bn,$(SUPPORTED_ROMS),$(bn)/icon.nbf[pc])

SBIN_LZ        := $(SBIN)_LZ
.PHONY: main_lz

sdk9 sdk7: sdk
main files_for_compile: | sdk9
sub: | sdk7

main: $(SBIN) $(ELF)
main_lz: $(SBIN_LZ)
sub: ; @$(MAKE) -C sub

ROMSPEC        := rom.rsf
MAKEROM_FLAGS  := $(DEFINES)

$(ALL_GAME_OBJS): files_for_compile
$(ELF): files_for_compile dsprot libsyscall

dsprot:
	$(MAKE) -C lib/dsprot all install INSTALL_PREFIX=$(abspath $(WORK_DIR)/$(BUILD_DIR))

libsyscall: files_for_compile
	$(MAKE) -C lib/syscall all install INSTALL_PREFIX=$(abspath $(WORK_DIR)/$(BUILD_DIR)) GAME_CODE=$(GAME_CODE)

$(SBIN_LZ): $(BUILD_DIR)/component.files
	$(COMPSTATIC) -9 -c -f $<

$(BUILD_DIR)/component.files: main ;

$(HEADER_TEMPLATE): ;

$(ROM): $(ROMSPEC) filesystem main_lz sub $(BANNER)
	$(WINE) $(MAKEROM) $(MAKEROM_FLAGS) -DBUILD_DIR=$(BUILD_DIR) -DNITROFS_FILES="$(NITROFS_FILES:files/%=%)" -DTITLE_NAME="$(TITLE_NAME)" -DBNR="$(BANNER)" -DHEADER_TEMPLATE="$(HEADER_TEMPLATE)" $< $@
	$(FIXROM) $@ --secure-crc $(SECURE_CRC) --game-code $(GAME_CODE)
ifeq ($(COMPARE),1)
	$(SHA1SUM) -c $(buildname)/rom.sha1
endif

$(BANNER): $(BANNER_SPEC) $(ICON_PNG:%.png=%.nbfp) $(ICON_PNG:%.png=%.nbfc)
	$(WINE) $(MAKEBNR) $< $@

# TODO: move to NitroSDK makefile
FX_CONST_H := $(WORK_DIR)/lib/include/nitro/fx/fx_const.h
PROJECT_CLEAN_TARGETS += $(FX_CONST_H)
$(FX_CONST_H): $(MKFXCONST) $(TOOLSDIR)/gen_fx_consts/fx_const.csv
	$(MKFXCONST) $@

$(ALL_LIB_OBJS): $(FX_CONST_H)
sdk9: $(ALL_LIB_OBJS)
$(WORK_DIR)/include/global.h: $(FX_CONST_H) ;

# Convenience targets
heartgold:          ; @$(MAKE) GAME_VERSION=HEARTGOLD
soulsilver:         ; @$(MAKE) GAME_VERSION=SOULSILVER
compare_heartgold:  ; @$(MAKE) GAME_VERSION=HEARTGOLD  COMPARE=1
compare_soulsilver: ; @$(MAKE) GAME_VERSION=SOULSILVER COMPARE=1
clean_heartgold:    ; @$(MAKE) GAME_VERSION=HEARTGOLD clean
clean_soulsilver:   ; @$(MAKE) GAME_VERSION=SOULSILVER clean

compare:             compare_heartgold

.PHONY: heartgold soulsilver compare compare_heartgold compare_soulsilver clean_heartgold clean_soulsilver

# Fork progress: first refresh and verify the selected ROM, then report.
.PHONY: progress
progress:
	$(MAKE) COMPARE=1 all
	python3 tools/progress/progress.py --game $(buildname)

# to54 atomic NitroCrypto compiler override
$(BUILD_DIR)/src/to54_atomic_w17_01_nitrocrypto_residual03_run01.o $(BUILD_DIR)/src/to54_atomic_w17_02_nitrocrypto_residual03_run02.o $(BUILD_DIR)/src/to54_atomic_w17_03_nitrocrypto_residual03_run03.o $(BUILD_DIR)/src/to54_atomic_w17_04_nitrocrypto_residual03_run04.o $(BUILD_DIR)/src/to54_atomic_w17_05_nitrocrypto_residual03_run05.o $(BUILD_DIR)/src/to54_atomic_w17_06_nitrocrypto_residual03_run06.o $(BUILD_DIR)/src/to54_atomic_w17_07_nitrocrypto_residual03_run07.o $(BUILD_DIR)/src/to54_atomic_w17_08_nitrocrypto_residual03_run08.o $(BUILD_DIR)/src/to54_atomic_w17_09_nitrocrypto_residual03_run09.o $(BUILD_DIR)/src/to54_atomic_w17_10_nitrocrypto_residual03_run10.o $(BUILD_DIR)/src/to54_atomic_w17_11_nitrocrypto_residual03_run11.o $(BUILD_DIR)/src/to54_atomic_w17_12_nitrocrypto_residual03_run12.o $(BUILD_DIR)/src/to54_atomic_w17_13_nitrocrypto_residual03_run13.o $(BUILD_DIR)/src/to54_atomic_w17_14_nitrocrypto_residual03_run14.o $(BUILD_DIR)/src/to54_atomic_w17_15_nitrocrypto_residual03_run15.o $(BUILD_DIR)/src/to54_atomic_w17_16_nitrocrypto_residual03_run16.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to54_atomic_w17_01_nitrocrypto_residual03_run01.o $(BUILD_DIR)/src/to54_atomic_w17_02_nitrocrypto_residual03_run02.o $(BUILD_DIR)/src/to54_atomic_w17_03_nitrocrypto_residual03_run03.o $(BUILD_DIR)/src/to54_atomic_w17_04_nitrocrypto_residual03_run04.o $(BUILD_DIR)/src/to54_atomic_w17_05_nitrocrypto_residual03_run05.o $(BUILD_DIR)/src/to54_atomic_w17_06_nitrocrypto_residual03_run06.o $(BUILD_DIR)/src/to54_atomic_w17_07_nitrocrypto_residual03_run07.o $(BUILD_DIR)/src/to54_atomic_w17_08_nitrocrypto_residual03_run08.o $(BUILD_DIR)/src/to54_atomic_w17_09_nitrocrypto_residual03_run09.o $(BUILD_DIR)/src/to54_atomic_w17_10_nitrocrypto_residual03_run10.o $(BUILD_DIR)/src/to54_atomic_w17_11_nitrocrypto_residual03_run11.o $(BUILD_DIR)/src/to54_atomic_w17_12_nitrocrypto_residual03_run12.o $(BUILD_DIR)/src/to54_atomic_w17_13_nitrocrypto_residual03_run13.o $(BUILD_DIR)/src/to54_atomic_w17_14_nitrocrypto_residual03_run14.o $(BUILD_DIR)/src/to54_atomic_w17_15_nitrocrypto_residual03_run15.o $(BUILD_DIR)/src/to54_atomic_w17_16_nitrocrypto_residual03_run16.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to70_jn_spl_extra07_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run03.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run03.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run04.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run04.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run05.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run05.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run06.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run06.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run07.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run07.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run08.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run08.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run09.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run09.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run10.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run10.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run11.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run11.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run12.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run12.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run13.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run13.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run14.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run14.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run15.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run15.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run16.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run16.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run17.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run17.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run18.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run18.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run19.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run19.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run20.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run20.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run21.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run21.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run22.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run22.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run23.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run23.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run24.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run24.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run25.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run25.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run26.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run26.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run27.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run27.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run28.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run28.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to70_jn_spl_extra07_run29.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to70_jn_spl_extra07_run29.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to70_MSL_Common_mem_funcs_extra79_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_string_residual1_extra80_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_string_residual2_extra81_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_DPMath_s_copysign_extra82_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_DPMath_s_fabs_extra83_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_string_extras_extra84_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_msl_wstring_wcscpy_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_ARM_abort_exit_extra102_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_CPP_NITRO_Ai_LE_extra108_run01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_CPP_NITRO_Ai_LE_extra108_run02.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_CPP_NITRO_Ai_LE_extra108_run03.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_CPP_NITRO_Ai_LE_extra108_residual_3_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_CPP_NITRO_Ai_LE_extra108_residual_3_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to91_msl_shared_ptr_release.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_14_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_14_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_11_r01.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_11_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_16_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to90_msl_interval_demo_residual_16_r01.o: private EXCCFLAGS := -Cpp_exceptions on
include tools/msl-d1-partition/d1.mk
include tools/msl-new-partition/new.mk
include tools/dwc-iterator-advance/iterator.mk

$(BUILD_DIR)/src/to70_MSL_Common_file_io_extra109_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_printf_extra110_run01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_Common_printf_extra110_run02.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_wprintf_extra111_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_fp_ansi_extra112_run01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_fp_ansi_extra112_run02.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to70_MSL_fp_ansi_extra112_run03.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_math_extra113_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_ARM_pool_alloc_NITRO_extra115_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_alloc_extra116_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_assert_extra117_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_signal_extra118_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_strtold_extra119_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_MSL_Common_wmem_extra120_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to70_msl_mbstring_residual_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1377_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1378_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1379_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1380_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1381_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1469_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1469_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1470_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1470_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1470_run02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1470_run02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1471_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1471_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1472_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1472_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1486_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1486_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1486_run02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1486_run02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1487_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1487_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1487_run02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1487_run02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1615_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1616_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1617_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1680_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1680_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1680_run02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1680_run02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1680_run03.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1680_run03.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to80_extra1680_run04.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1680_run04.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1681_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1681_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1682_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1682_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to80_extra1718_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1719_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1834_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1835_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1836_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to80_extra1860_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to80_extra1860_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to90_extra1964_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to90_extra2110_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to90_extra2160_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to90_extra2160_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to90_extra2309_run01.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/src/to90_extra2310_run01.o: private MWCCVER := 2.0/sp2p3


$(BUILD_DIR)/src/to90_extra2469_run01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to90_extra2469_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to90_scanf_interval_v5_run01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to90_scanf_interval_v5_run02.o: private MWCCVER := 2.0/sp2p3

include tools/alarm-partition/alarm.mk

$(BUILD_DIR)/lib/NitroSDK/src/snd/SND_command.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/lib/NitroSDK/src/snd/SND_command.o: lib/NitroSDK/src/snd/SND_command.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/lib/NitroSDK/src/os/os_spinLock.o: lib/NitroSDK/src/os/os_spinLock.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/lib/NitroSDK/src/os/to91_w41407_spin_retained.o: lib/NitroSDK/src/os/to91_w41407_spin_retained.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/src/to90_w41411_palette10_run01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/src/to91_w41418_StringRead_run01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1919_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_Common_wchar_io_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_op90_to80_extra1919_residual_1_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to59_dwcaccount_residual_3_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to59_dwcaccount_residual_3_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to59_dwcaccount_residual_3_r02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to59_dwcaccount_residual_3_r02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to70_mb_extra10_residual_12_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1471_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1471_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1471_residual_2_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1471_residual_2_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1680_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1680_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1746_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r03.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2469_residual_1_r03.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to70_fx_trig_extra207_residual_1_r01.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/src/op90_to80_extra1853_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1469_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1469_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1470_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1470_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1680_residual_2_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1680_residual_2_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1680_residual_2_r02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1680_residual_2_r02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_fx_vec_normalize_residual_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_op90_to80_extra1680_residual_2_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_op90_to80_extra1680_residual_2_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_op90_to80_extra1680_residual_2_residual_1_r02.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_op90_to80_extra1680_residual_2_residual_1_r02.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1442_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_Common_string_residual0_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to57_nitrocrypto_residual04_residual_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to57_nitrocrypto_residual04_residual_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1378_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1450_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1672_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1672_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1920_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra1964_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2129_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2319_residual_1_r01.o: private MWCCVER := 2.0/base
$(BUILD_DIR)/src/op90_to90_extra2537_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to91_w41566_texmatrix_suffix_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1719_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1746_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1836_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_overlay89_r2u1_residual_2_r01.o: private OPTFLAGS := -O2
$(BUILD_DIR)/src/op90_overlay_40_residual_31_residual_4_to45_residual_r01.o: private OPTFLAGS := -O2
$(BUILD_DIR)/src/op90_to53_overlay112_004_r01.o: private OPTFLAGS := -O1
$(BUILD_DIR)/src/op90_to70_mb_extra10_residual_6_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to70_to53_overlay112_002_extra683_residual_1_r01.o: private OPTFLAGS := -O2
$(BUILD_DIR)/src/op90_to80_extra1453_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1617_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_unk_02032B84_r01.o: private OPTFLAGS := -O2
$(BUILD_DIR)/src/wifi_setup_result.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/op90_to53_overlay13_r20_r01.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/src/op90_to56_w16_residual03_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/wifi_setup_session.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/op90_to80_extra1719_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1834_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1836_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra1953_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_op90_to80_extra1378_residual_2_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_Common_ansi_files_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_Common_ansi_files_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_MSL_Common_qsort_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_Common_qsort_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_MSL_DPMath_s_frexp_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_DPMath_s_frexp_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_MSL_DPMath_s_ldexp_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_DPMath_s_ldexp_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r02.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r02.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r03.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to59_msl_strtoul_residual_r03.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_to65_nitro_unified_residual_26_r01.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/src/op90_to65_wifi_residual_11_residual_5_r01.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/src/op90_unk_02025C44_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/op90_to70_MSL_Common_strtold_extra119_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1755_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to91_w41418_StringRead_prefix_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2155_residual_1_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/op90_overlay_80_0222BDF4_residual_12_r01.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/src/op90_to80_extra1754_residual_2_r01.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/src/op90_overlay_83_residual_6_head_r01.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/lib/NitroSystem/src/gfd_linked_list_pltt_vram_man_reset.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to65_wifi_residual_11_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/gfd_linked_list_tex_vram_man_reset.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to70_overlay_93_thumb_1_to41_residual_6_extra219_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to53_overlay47_residual_5_tail_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to90_extra2160_residual_1_r01.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2160_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_MSL_DPMath_e_pow_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_MSL_DPMath_e_pow_r01.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/op90_overlay_40_residual_5_gap_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_overlay_96_residual_80_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to42_ov40_r33_a08_gap2_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to61_overlay_13_arm_2_residual_5_residual_7_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to70_to52_r5_overlay96_r90_gap4_tail_extra260_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1591_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to90_extra2552_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to91_w41573_mode_prefix_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_fx_mtx33_identity_residual_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2338_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2544_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_fx_mtx44_residual_head_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_g3x_nop_fifo_residual_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_gxasm_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_mi_swap_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_mi_uncompress_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_os_vramExclusive_clz_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_w41415_fp_init4_suffix_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to91_w41419_ExitProcess_prefix_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_op90_op90_op90_to80_extra1740_residual_4_residual_1_residual_1_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to90_os_alarm_handler_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_fx_mtx43_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_fx_mtx44_residual_rot_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_mi_memory_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2544_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2545_residual_2_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_reset_reset_system_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_reset_boot_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_reset_clear32_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_op90_to91_w41419_ExitProcess_prefix_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to90_extra2154_residual_1_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_nitrocrypto_residual00_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to58_overlay13_r18_residual02_suffix_r01.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/op90_overlay_96_residual_70_tail_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/wifi_setup_connect.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_op90_overlay89_r8_residual_03_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_overlay_80_022310C4_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_op90_to80_extra1758_residual_1_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to65_wifi_residual_11_residual_4_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to70_to65_wifi_residual_11_residual_14_extra1163_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_unk_0203A3B0_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_to80_extra1454_residual_3_r01.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/op90_to80_extra1754_residual_3_r01.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/src/op90_to80_extra1754_residual_3_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/op90_op90_overlay_02_02248728_to42_gap_residual_1_r01.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSystem/src/g2d_char_canvas_letter.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g2d_char_canvas_letter.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
