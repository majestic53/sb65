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

DIR_BUILD=../../build/test/
DIR_SRC=./
DIR_SRC_SYSTEM=./system/

FLAGS=-march=native -std=c11 -Wall -Werror

LIB=libsb65_test.a

all: build archive

### LIBRARY ###

archive:
	@echo ''
	@echo '--- ARCHIVING OBJECTS ----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)base_sanity.o \
		$(DIR_BUILD)system_processor.o
	@echo '--- DONE -----------------------------------'

build: build_base build_system

### BASE ###

build_base: base_sanity.o

base_sanity.o: $(DIR_SRC)sanity.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC)sanity.c -o $(DIR_BUILD)base_sanity.o

### SYSTEM ###

build_system: system_processor.o

system_processor.o: $(DIR_SRC_SYSTEM)processor.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_SYSTEM)processor.c -o $(DIR_BUILD)system_processor.o
