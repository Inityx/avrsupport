# Build definitions
CXX=g++
CFLAGS=-Os -Wpedantic --std=c++17

GREEN=\u001b[32m
RESET=\u001b[0m

# Structure
MODULES=portlib peripheral emulated

# Targets
.PHONY: all includes test test_clean clean configure

all: includes test

# Includes
define generate_include
INCLUDE_DIRS+=include/$(1)
INCLUDES+=$$(addprefix include/$(1)/, $$(notdir $$(wildcard src/$(1)/*)))

include/$(1)/%.hpp: src/$(1)/%.hpp | include/$(1)
	@echo " CP $$(notdir $$(basename $$@))"
	@cp $$< $$@

include/$(1):
	@echo " MD $$@"
	@mkdir $$@

endef

$(foreach MODULE, $(MODULES), $(eval $(call generate_include,$(MODULE))))

includes: $(INCLUDES)

# Test
TEST_SRC=$(notdir $(wildcard test/*.cpp))
TEST_BUILD=$(addprefix test/build/, $(patsubst %.cpp, %.out, $(TEST_SRC)))

test: $(TEST_BUILD) test_clean

test/build/%.out: test/%.cpp
	@printf " Testing $(notdir $(basename $@))..."
	@$(CXX) $< -o $@ $(CFLAGS) -iquote include/
	@$@
	@echo -e " $(GREEN)Success$(RESET)."

test_clean:
	@echo " Cleaning up tests"
	@rm -f $(TEST_BUILD)

# Clean
clean:
	rm -rf $(INCLUDE_DIRS)

# Configure
configure: .nvimrc

.nvimrc:
	@echo "Generating local .nvimrc..."
	@echo "let g:syntastic_cpp_compiler = '$(CXX)'" > .nvimrc
