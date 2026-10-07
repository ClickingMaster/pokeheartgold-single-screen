# Finite native scalar and array allocation producers; generated data/EH remains assembly-owned.
.PHONY: to91_msl_scalar_new_rebuild to91_msl_array_new_rebuild
to91_msl_scalar_new_rebuild to91_msl_array_new_rebuild:

$(BUILD_DIR)/src/to91_msl_scalar_new.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/src/to91_msl_scalar_new.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to91_msl_scalar_new.o: src/to91_msl_scalar_new.cpp tools/msl-new-partition/build.py tools/msl-new-partition/shape.py tools/msl-new-partition/select-scalar.py tools/msl-new-partition/scalar-shape.json tools/msl-new-partition/profile.json to91_msl_scalar_new_rebuild
	@mkdir -p $(dir $@)
	python3 tools/msl-new-partition/build.py --producer scalar --source $< --object $@ --game $(patsubst %.us,%,$(buildname)) --nodep "$(NODEP)" --command "$(BUILD_CPP)"

$(BUILD_DIR)/src/to91_msl_array_new.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/src/to91_msl_array_new.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to91_msl_array_new.o: src/to91_msl_array_new.cpp tools/msl-new-partition/build.py tools/msl-new-partition/shape.py tools/msl-new-partition/select-array.py tools/msl-new-partition/array-shape.json tools/msl-new-partition/profile.json to91_msl_array_new_rebuild
	@mkdir -p $(dir $@)
	python3 tools/msl-new-partition/build.py --producer array --source $< --object $@ --game $(patsubst %.us,%,$(buildname)) --nodep "$(NODEP)" --command "$(BUILD_CPP)"
