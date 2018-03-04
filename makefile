# Build definitions
CXX:=avr-g++
TESTING_CXX:=clang++
CFLAGS:=-Wpedantic --std=c++17
CFLAGS_RELEASE:=-Os
CFLAGS_TEST:=-O0 -Wno-keyword-macro

ifneq ($(CI), true)
    ANSI_SUCC:=\u001b[32m
    ANSI_INFO:=\u001b[36m
    ANSI_ERR:=\u001b[31m
    ANSI_RS:=\u001b[0m
endif

ANSI_CRBL:=\u001b[1K\r

# Structure
MODULES:=emulated mapping peripheral portlib serial/interface serial/usart serial/usi utility

# Docs
DOXYGEN:=doxygen

# Targets

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
	@mkdir -p $$@

endef

$(foreach MODULE, $(MODULES), $(eval $(call generate_include,$(MODULE))))

includes: includes_title $(INCLUDES)
includes_title:
	@echo -e "$(ANSI_INFO)### Copy headers$(ANSI_RS)"

# Test
TEST_SRC:=$(notdir $(wildcard test/*.cpp))
TEST_BINS:=$(addprefix test/build/, $(patsubst %.cpp, %.out, $(TEST_SRC)))

test: clean includes test_title $(TEST_BINS)

test_title:
	@echo -e "$(ANSI_INFO)### Unit tests$(ANSI_RS)"

test/build/%.out: NAME=$(notdir $(basename $@))
test/build/%.out: test/%.cpp
	@printf " Building $(NAME)... "
	@$(TESTING_CXX) $< -o $@ $(CFLAGS) $(CFLAGS_TEST) -I include/
	@printf "$(ANSI_CRBL) Testing $(NAME)... "
	@$@
	@echo -e "$(ANSI_CRBL) $(ANSI_SUCC)Passed$(ANSI_RS) $(NAME)"

test_clean:
	@echo -e "$(ANSI_INFO)### Clean tests$(ANSI_RS)"
	@echo " RM test/build/*"
	@rm -rf test/build/*

# Clean
clean: test_clean
	@echo -e "$(ANSI_INFO)### Clean build$(ANSI_RS)"
	@echo " RM include/*"
	@rm -rf include/*

# Configure
configure: .nvimrc

.nvimrc:
	@echo "Generating local .nvimrc..."
	@echo "let g:syntastic_cpp_compiler = '$(CXX)'" > .nvimrc

# Docs
DOXYFILE:=Doxyfile
DOCS:=docs/build
HTML:=$(DOCS)/html
docs:
	$(DOXYGEN) $(DOXYFILE)

docs_view: docs
	xdg-open $(HTML)/index.html

docs_clean:
	rm -rf $(HTML)

# Phony
.PHONY: all includes clean configure
.PHONY: test test_clean
.PHONY: docs docs_view docs_clean
