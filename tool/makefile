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

BIN=sb65

DIR_BIN=../bin/
DIR_BIN_LIB=../bin/lib/
DIR_BUILD=../build/
DIR_BUILD_TOOL=../build/tool/
DIR_ROOT=./

FLAGS=-march=native -std=c11 -Wall -Werror
FLAGS_BIN=
FLAGS_LIB=-lSDL2 -lSDL2main

LIB=libsb65.a

all: begin init build link

### SETUP ###

begin:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING SB65 TOOL'
	@echo '============================================'

init:
	mkdir -p $(DIR_BUILD_TOOL)

### LAUNCHER ###

link:
	@echo ''
	@echo '--- LINKING LAUNCHER -----------------------'
	$(CC) $(FLAGS) $(BUILD_FLAGS) $(DIR_BUILD_TOOL)tool_launcher.o $(DIR_BIN_LIB)$(LIB) $(FLAGS_LIB) -o $(DIR_BIN)$(BIN) $(FLAGS_BIN)
	@echo '--- DONE -----------------------------------'

build: build_tool

### TOOL ###

build_tool: tool_launcher.o

tool_launcher.o: $(DIR_ROOT)launcher.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT)launcher.c -o $(DIR_BUILD_TOOL)tool_launcher.o
