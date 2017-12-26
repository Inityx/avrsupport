# Build definitions
CXX=g++
CFLAGS=-Wpedantic --std=c++17
CFLAGS_RELEASE=-Os
CFLAGS_TEST=-O0

ifneq ($(CI), true)
    GREEN=\u001b[32m
    RESET=\u001b[0m
endif

# Structure
MODULES=portlib peripheral emulated chips utility

# Docs
DOXYGEN=doxygen

# Targets
.PHONY: all includes test test_clean clean configure
.PHONY: docs docs_view docs_clean

all: includes

# Includes
define generate_include
INCLUDE_DIRS+=include/$(1)
INCLUDES+=$$(addprefix include/$(1)/, $$(notdir $$(wildcard src/$(1)/*.hpp)))

include/$(1)/%.hpp: src/$(1)/%.hpp | include/$(1)
	@echo " CP $(1)/$$(notdir $$(basename $$@))"
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

test: includes $(TEST_BUILD) test_clean

test/build/%.out: test/%.cpp
	@printf " Testing $(notdir $(basename $@))... "
	@$(CXX) $< -o $@ $(CFLAGS) $(CFLAGS_TEST) -I include/
	@$@
	@echo -e "$(GREEN)Success$(RESET)."

test_clean:
	@echo " Cleaning up tests"
	@rm -f $(TEST_BUILD)

# Clean
clean:
	@echo " RM $(INCLUDE_DIRS)"
	@rm -rf $(INCLUDE_DIRS)

# Configure
configure: .nvimrc

.nvimrc:
	@echo "Generating local .nvimrc..."
	@echo "let g:syntastic_cpp_compiler = '$(CXX)'" > .nvimrc

# Docs
DOXYFILE=Doxyfile
DOCS=docs/build
HTML=$(DOCS)/html
docs:
	$(DOXYGEN) $(DOXYFILE)

docs_view: docs
	xdg-open $(HTML)/index.html

docs_clean:
	rm -rf $(HTML)
