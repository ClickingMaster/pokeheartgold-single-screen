# Finite genuine D1 producer; generated EH and D2 remain outside the linked child.
.PHONY: to91_msl_shared_ptr_char_d1_rebuild
to91_msl_shared_ptr_char_d1_rebuild:

$(BUILD_DIR)/src/to91_msl_shared_ptr_char_d1.o: private MWCCVER := 2.0/sp2p3
$(BUILD_DIR)/src/to91_msl_shared_ptr_char_d1.o: private EXCCFLAGS := -Cpp_exceptions on
$(BUILD_DIR)/src/to91_msl_shared_ptr_char_d1.o: src/to91_msl_shared_ptr_char_d1.cpp tools/msl-d1-partition/build.py tools/msl-d1-partition/shape.py tools/msl-d1-partition/select-d1.py tools/msl-d1-partition/producer-shape.json tools/msl-d1-partition/profile.json to91_msl_shared_ptr_char_d1_rebuild
	@mkdir -p $(dir $@)
	python3 tools/msl-d1-partition/build.py --source $< --object $@ --game $(patsubst %.us,%,$(buildname)) --nodep "$(NODEP)" --command "$(BUILD_CPP)"
