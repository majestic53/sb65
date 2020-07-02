# SB65
# Copyright (C) 2020 David Jolly
#
# SB65 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SB65 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

DIR_BIN=./bin/
DIR_BIN_INC=./bin/inc/
DIR_BIN_LIB=./bin/lib/
DIR_BUILD=./build/
DIR_ROOT=./
DIR_SRC=./src/
DIR_TOOL=./tool/

BUILD_DBG=BUILD_FLAGS=-g
BUILD_REL=BUILD_FLAGS=-O3\ -DNDEBUG

all: release

### BUILD ###

debug: begin clean init lib_debug tool_debug uninit

release: begin clean init lib_release tool_release uninit

### SETUP ###

begin:
	@echo '============================================'
	@echo 'BUILDING SB65'
	@echo '============================================'

clean:
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_BUILD)

init:
	mkdir -p $(DIR_BIN_INC)
	mkdir -p $(DIR_BIN_LIB)
	mkdir -p $(DIR_BUILD)

uninit:
	@echo ''
	@echo '============================================'
	@echo 'COMPLETE'
	@echo '============================================'

### LIBRARY ###

lib_debug:
	@echo ''
	cd $(DIR_SRC) && make $(BUILD_DBG)

lib_release:
	@echo ''
	cd $(DIR_SRC) && make $(BUILD_REL)

### TOOL ###

tool_debug:
	@echo ''
	cd $(DIR_TOOL) && make $(BUILD_DBG)

tool_release:
	@echo ''
	cd $(DIR_TOOL) && make $(BUILD_REL)

### MISC ###

lines:
	@echo ''
	@echo '============================================'
	@echo 'CALCULATING LINE COUNT'
	@echo '============================================'
	cloc $(DIR_ROOT)

static:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING STATIC ANALYSIS'
	@echo '============================================'
	cppcheck --enable=all --std=c11 $(DIR_SRC) $(DIR_TOOL)
