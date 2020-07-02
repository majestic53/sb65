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

DIR_BIN_INC=../bin/inc/
DIR_BIN_LIB=../bin/lib/
DIR_BUILD=../build/
DIR_INC=../inc/
DIR_INC_COMMON=../inc/common/
DIR_SRC=./
DIR_SRC_COMMON=./common/

FLAGS=-march=native -std=c11 -Wall -Werror

LIB=libsb65.a

all: begin build archive

### SETUP ###

begin:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING SB65 LIBRARY'
	@echo '============================================'

### LIBRARY ###

archive:
	@echo ''
	@echo '--- ARCHIVING OBJECTS ----------------------'
	ar rcs $(DIR_BIN_LIB)$(LIB) $(DIR_BUILD)base_runtime.o $(DIR_BUILD)base_sb65.o \
		$(DIR_BUILD)common_buffer.o
	cp $(DIR_INC)sb65.h $(DIR_BIN_INC)
	@echo '--- DONE -----------------------------------'

build: build_base build_common

### BASE ###

build_base: base_runtime.o base_sb65.o

base_runtime.o: $(DIR_SRC)runtime.c $(DIR_INC)runtime.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC)runtime.c -o $(DIR_BUILD)base_runtime.o

base_sb65.o: $(DIR_SRC)sb65.c $(DIR_INC)sb65.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC)sb65.c -o $(DIR_BUILD)base_sb65.o

### COMMON ###

build_common: common_buffer.o

common_buffer.o: $(DIR_SRC_COMMON)buffer.c $(DIR_INC_COMMON)buffer.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_COMMON)buffer.c -o $(DIR_BUILD)common_buffer.o
