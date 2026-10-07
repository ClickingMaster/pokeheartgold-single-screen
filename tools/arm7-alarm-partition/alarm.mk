# Genuine complete ARM7 alarm C producers, physically partitioned after compilation.
ARM7_ALARM_TOOLS := $(WORK_DIR)/tools/arm7-alarm-partition
ARM7_ALARM_HELPERS := $(wildcard $(ARM7_ALARM_TOOLS)/*.py) $(ARM7_ALARM_TOOLS)/profile.json
ARM7_ALARM_PYTHON := $(if $(PYTHON),$(PYTHON),python3)
ARM7_ALARM_GAME := $(if $(filter SOULSILVER,$(GAME_VERSION)),SOULSILVER,HEARTGOLD)
$(BUILD_DIR)/lib/src/to90_arm7_alarm_prefix.o: lib/src/to90_arm7_alarm_prefix.c $(ARM7_ALARM_HELPERS)
	@$(ARM7_ALARM_PYTHON) -B $(ARM7_ALARM_TOOLS)/build.py --role prefix --game $(ARM7_ALARM_GAME) --source $< --object $@ --gnu-profile "$(MW_COMPILE)"
	@$(call fixdep,$(BUILD_DIR)/lib/src/to90_arm7_alarm_prefix.d)
$(BUILD_DIR)/lib/asm/to90_arm7_alarm_handler.o: lib/asm/to90_arm7_alarm_handler.s $(ARM7_ALARM_HELPERS)
	@$(ARM7_ALARM_PYTHON) -B $(ARM7_ALARM_TOOLS)/build.py --role handler --game $(ARM7_ALARM_GAME) --source $< --object $@ --gnu-profile "$(MW_ASSEMBLE) $(DEPFLAGS)"
	@$(call fixdep,$(BUILD_DIR)/lib/asm/to90_arm7_alarm_handler.d)
$(BUILD_DIR)/lib/src/to90_arm7_alarm_suffix.o: lib/src/to90_arm7_alarm_suffix.c $(ARM7_ALARM_HELPERS)
	@$(ARM7_ALARM_PYTHON) -B $(ARM7_ALARM_TOOLS)/build.py --role suffix --game $(ARM7_ALARM_GAME) --source $< --object $@ --gnu-profile "$(MW_COMPILE)"
	@$(call fixdep,$(BUILD_DIR)/lib/src/to90_arm7_alarm_suffix.d)
