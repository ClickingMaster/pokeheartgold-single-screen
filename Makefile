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

# The toolchain is console-only. Without an X/Wayland display, Wine's display drivers retry for ~8 s in
# every process start ("Failed to read display config"), which dominates each compile and assemble.
# Disabling them changes no output. Exported, so sub/, lib/dsprot, lib/syscall and the Python producers
# get it too; a WINEDLLOVERRIDES already in the environment wins.
ifneq ($(WINE),)
export WINEDLLOVERRIDES ?= winex11.drv,winewayland.drv=d
endif

# crt0 is now C (asm functions); keep basename crt0.o for the lcf.
CRT0_OBJ := lib/NitroSDK/src/crt0.o
include filesystem.mk

$(ASM_OBJS): private MWASFLAGS += -DPM_ASM

$(BUILD_DIR)/asm/nitrocrypto.o:  private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_sign.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_exp.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_arith.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_bits.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_bin2bn.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_word.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_expand.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_ctx.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_fix_top.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_free.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_bn_num_bits.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_alloc.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_sign.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_exp.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_arith.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_bits.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_bin2bn.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_word.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_expand.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_ctx.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_fix_top.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_free.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_bn_num_bits.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/nitrocrypto_alloc.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
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

$(BUILD_DIR)/lib/src/spl_manager.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_draw.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_emitter.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_manager.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_draw.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_emitter.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_draw_ptcl.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_draw_ptcl.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_generate.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_anim.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_field.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_list.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_random.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/spl_generate.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_anim.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_field.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_list.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/src/spl_random.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/lib/MSL_C/src/msl_mem_funcs.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_string_strcmp.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_string_strtok.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_s_copysign.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_s_fabs.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_string_extras.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_wstring_wcscpy.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_abort_exit.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_shared_ptr_create.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_shared_ptr_destroy.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_shared_ptr_release_weak.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_vector_length_error.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_vector_length_error.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_shared_ptr_release.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_unexpected.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_unexpected.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_unwind_stack.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/lib/MSL_C/src/msl_unwind_stack.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_handle_unexpected.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_handle_unexpected.o: private EXCCFLAGS := -Cpp_exceptions on
include tools/msl-d1-partition/d1.mk
include tools/msl-new-partition/new.mk
include tools/dwc-iterator-advance/iterator.mk

$(BUILD_DIR)/lib/MSL_C/src/msl_file_io.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_printf_formatter.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_wprintf_formatter.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_rounddec.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_str2dec.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_num2dec.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_math.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_pool_alloc.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_alloc.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_assert.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_signal.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_strtold.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_wmem.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_mbstring_mbstowcs.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_abort_exit_core.o: private MWCCVER := 2.0/sp2p3




$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_compare_dec.o: private MWCCVER := 2.0/sp2p3









$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_num2dec_internal.o: private MWCCVER := 2.0/sp2p3




$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_dec2num.o: private MWCCVER := 2.0/sp2p3



$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_two_exp.o: private MWCCVER := 2.0/sp2p3



$(BUILD_DIR)/lib/MSL_C/src/msl_direct_io_write.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_filepos_fseek_core.o: private MWCCVER := 2.0/sp2p3


$(BUILD_DIR)/lib/MSL_C/src/msl_arm_math.o: private MWCCVER := 2.0/sp2p3

$(BUILD_DIR)/lib/MSL_C/src/msl_w_pow.o: private MWCCVER := 2.0/sp2p3



$(BUILD_DIR)/lib/MSL_C/src/msl_scanf_vsscanf.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_scanf_sscanf.o: private MWCCVER := 2.0/sp2p3

include tools/alarm-partition/alarm.mk

$(BUILD_DIR)/lib/NitroSDK/src/snd/SND_command.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/lib/NitroSDK/src/snd/SND_command.o: lib/NitroSDK/src/snd/SND_command.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/lib/NitroSDK/src/os/os_spinLock.o: lib/NitroSDK/src/os/os_spinLock.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/lib/NitroSDK/src/os/os_spin_lock_id.o: lib/NitroSDK/src/os/os_spin_lock_id.snd-section-map.json tools/nitro-section-renames/rename_snd_sections.py

$(BUILD_DIR)/src/pokeathlon/pokeathlon_course_mon_palette.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))

$(BUILD_DIR)/lib/MSL_C/src/msl_scanf_string_read.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_task_thread.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_wchar_io.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_rc4_init.o: private MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_rc4_init.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_disconnect.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_parent_calc_block.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_trig_internal.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/lib/NitroSDK/src/mi/mi_compress_search.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_vec_normalize.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx44_perspective.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_string_strcpy.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_read_segment.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_cgtool_maya_texmtx.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_cgtool_maya_texmtx_r.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_anm_btp_init.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_direct_io_read.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_cgtool_3dsmax_texmtx.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/wifi_setup_keyboard_slide_in.o: private MWCCVER := 2.0/base
$(BUILD_DIR)/lib/NitroSystem/src/g3d_cgtool_xsi_texmtx.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_cgtool_xsi_texmtx_t.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_parent_send_block.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/application/footprint_stamp_sprites.o: private OPTFLAGS := -O2
$(BUILD_DIR)/src/application/vs_recorder/vs_recorder_box_list_title.o: private OPTFLAGS := -O2
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_beacon_fixed.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_beacon_volat.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_fp_ansi_minus_dec.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/wifi_setup_result.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/wifi_setup_md5.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/src/wifi_setup_ap_select.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/wifi_setup_session.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_parent_recv_child.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_ansi_files.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_ansi_files.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_qsort.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_qsort.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_s_frexp.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_s_frexp.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_s_ldexp.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_s_ldexp.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoul_core.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoul_core.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoull_core.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoull_core.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoul.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_strtoul.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/lib/NitroSDK/src/rtc/rtc_convert_date_time_to_second.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/lib/libCPS/src/cps_startup.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/src/g2d_cell_culling.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/lib/MSL_C/src/msl_strtold_core.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_anm_bta_sincos.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_scanf_sformatter.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/std_sprintf.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/frontier/frontier_cmd_create_sprites.o: private MWCCVER := 2.0/sp2
$(BUILD_DIR)/lib/NitroSystem/src/g3d_anm_bca_trans.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/lib/NitroSystem/src/gfd_linked_list_pltt_vram_man_reset.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroWiFi/src/soc_gethostbyname.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/gfd_linked_list_tex_vram_man_reset.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/wifi_plaza_survey_answer.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/MSL_C/src/msl_e_pow.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_e_pow.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/application/vs_recorder/vs_recorder_row_text.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/pokeathlon/pokeathlon_relay_run_pic_frames.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/wifi_setup_address_entry_init.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/pokeathlon/pokeathlon_pennant_capture_pic_sprites.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/pokeathlon/pokeathlon_circle_push_pic_sprites.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/wifi_setup_keypad_layout.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/wifi_setup_keyboard_layout.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx33_identity.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx33_rot_yz.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx43_identity.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx44_identity.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/gx/g3x_nop_clear_fifo.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/gx/gxasm.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mi/mi_swap.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mi/mi_uncompress.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/os/os_vram_exclusive_clz.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_semihosting.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_throw.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/ov45_02239df0.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSDK/src/os/os_alarm_irq_handler.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx43_rot.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx44_rot.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/mi/mi_memory.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/fx/fx_mtx43_scale.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/dgt_hash2_arm4_small.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/os/os_reset_system.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/os/os_reset_boot.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/os/os_reset_clear32.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/MSL_C/src/msl_transfer_control.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSDK/src/math/math_qsort.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/src/nitrocrypto_rc4.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/wifi_setup_worker.o: private MWCCVER := 2.0/sp1
$(BUILD_DIR)/src/pokeathlon/pokeathlon_hurdle_dash_runner.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/wifi_setup_connect.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/application/footprint_stamp_texture.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSystem/src/g2d_renderer_mtx_cache.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/libCPS/src/cps_reset.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/libCPS/src/cps_ip_frag.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSDK/src/mb/mb_init.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g3d_anm_bca_scale.o: private MWCCVER := 2.0/sp1p5
$(BUILD_DIR)/lib/NitroSystem/src/g3d_anm_bca_scale.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/NitroSystem/src/g2d_char_canvas_letter.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/lib/NitroSystem/src/g2d_char_canvas_letter.o: private MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
