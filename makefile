# Build definitions
CXX=avr-g++
CFLAGS=-Os -Wpedantic --std=c++17

# Targets
.PHONY: all includesclean configure

all: includes

# Includes
MODULES=portlib peripheral util

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

$(foreach MODULE,$(MODULES), $(eval $(call generate_include,$(MODULE))))

includes: $(INCLUDES)

clean:
	rm -rf $(INCLUDE_DIRS)

# Build

# Configure
configure: .nvimrc

.nvimrc:
	@echo "Generating local .nvimrc..."
	@echo "let g:syntastic_cpp_compiler = '$(CXX)'" > .nvimrc
