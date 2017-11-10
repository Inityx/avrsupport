# Build definitions
CXX=avr-g++
CFLAGS=-Os -Wpedantic --std=c++17
DEVICE_DEFS=-DF_CPU=8000000 -mmcu=attiny84

# Source structure
SRC_PORT_DIR=src/portlib
SRC_UTIL_DIR=src/util
INC_PORT_DIR=include/portlib
INC_UTIL_DIR=include/util

PORTS=$(wildcard $(SRC_PORT_DIR)/*.hpp)
UTILS=$(wildcard $(SRC_UTIL_DIR)/*.hpp)

INCLUDE_PORTS=$(addprefix $(INC_PORT_DIR)/, $(notdir $(PORTS)))
INCLUDE_UTILS=$(addprefix $(INC_UTIL_DIR)/, $(notdir $(UTILS)))


# Targets
.PHONY: all includes clean configure

all: includes

includes: $(INCLUDE_PORTS) $(INCLUDE_UTILS)

clean:
	rm -rf $(INC_PORT_DIR) $(INC_UTIL_DIR)


# Build
$(INC_PORT_DIR)/%.hpp: $(SRC_PORT_DIR)/%.hpp | $(INC_PORT_DIR)
	@echo " CP $(notdir $(basename $@))"
	@cp $< $@

$(INC_UTIL_DIR)/%.hpp: $(SRC_UTIL_DIR)/%.hpp | $(INC_UTIL_DIR)
	@echo " CP $(notdir $(basename $@))"
	@cp $< $@

$(INC_PORT_DIR):
	@echo " MD $@"
	@mkdir $(INC_PORT_DIR)

$(INC_UTIL_DIR):
	@echo " MD $@"
	@mkdir $(INC_UTIL_DIR)


# Configure
configure: .nvimrc

.nvimrc:
	@echo "Generating local .nvimrc..."
	@echo "let g:syntastic_cpp_compiler = '$(CXX)'" > .nvimrc
	@echo "let g:syntastic_cpp_compiler_options = ' $(DEVICE_DEFS) $(CFLAGS)'" >> .nvimrc
