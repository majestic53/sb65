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
DIR_INC_SYSTEM=../inc/system/
DIR_SRC=./
DIR_SRC_COMMON=./common/
DIR_SRC_SYSTEM=./system/

FLAGS=-march=native -std=c11 -Wall -Werror

LIB=libsb65.a

all: build archive

### LIBRARY ###

archive:
	@echo ''
	@echo '--- ARCHIVING OBJECTS ----------------------'
	ar rcs $(DIR_BIN_LIB)$(LIB) $(DIR_BUILD)base_runtime.o $(DIR_BUILD)base_sb65.o \
		$(DIR_BUILD)common_buffer.o $(DIR_BUILD)common_display.o \
		$(DIR_BUILD)system_device.o $(DIR_BUILD)system_memory.o $(DIR_BUILD)system_processor.o $(DIR_BUILD)system_video.o
	cp $(DIR_INC)sb65.h $(DIR_BIN_INC)
	@echo '--- DONE -----------------------------------'

build: build_base build_common build_system

### BASE ###

build_base: base_runtime.o base_sb65.o

base_runtime.o: $(DIR_SRC)runtime.c $(DIR_INC)runtime.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC)runtime.c -o $(DIR_BUILD)base_runtime.o

base_sb65.o: $(DIR_SRC)sb65.c $(DIR_INC)sb65.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC)sb65.c -o $(DIR_BUILD)base_sb65.o

### COMMON ###

build_common: common_buffer.o common_display.o

common_buffer.o: $(DIR_SRC_COMMON)buffer.c $(DIR_INC_COMMON)buffer.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_COMMON)buffer.c -o $(DIR_BUILD)common_buffer.o

common_display.o: $(DIR_SRC_COMMON)display.c $(DIR_INC_COMMON)display.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_COMMON)display.c -o $(DIR_BUILD)common_display.o

### SYSTEM ###

build_system: system_device.o system_memory.o system_processor.o system_video.o

system_device.o: $(DIR_SRC_SYSTEM)device.c $(DIR_INC_SYSTEM)device.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_SYSTEM)device.c -o $(DIR_BUILD)system_device.o

system_memory.o: $(DIR_SRC_SYSTEM)memory.c $(DIR_INC_SYSTEM)memory.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_SYSTEM)memory.c -o $(DIR_BUILD)system_memory.o

system_processor.o: $(DIR_SRC_SYSTEM)processor.c $(DIR_INC_SYSTEM)processor.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_SYSTEM)processor.c -o $(DIR_BUILD)system_processor.o

system_video.o: $(DIR_SRC_SYSTEM)video.c $(DIR_INC_SYSTEM)video.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SRC_SYSTEM)video.c -o $(DIR_BUILD)system_video.o
