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

BIN=test_

DIR_BUILD=../../build/
DIR_BUILD_TEST=../../build/test/
DIR_ROOT=./
DIR_SYSTEM=./system/

FLAGS=-march=native -std=c11 -Wall -Werror
FLAGS_BIN=
FLAGS_LIB=-lcheck

TEST_FLAGS=

all: begin init build link run

### SETUP ###

begin:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING SB65 TEST'
	@echo '============================================'

init:
	mkdir -p $(DIR_BUILD_TEST)

### LAUNCHER ###

link:
	@echo ''
	@echo '--- LINKING LAUNCHER -----------------------'
	$(CC) $(FLAGS) $(BUILD_FLAGS) $(DIR_BUILD_TEST)test_system_processor.o $(DIR_BUILD)system_processor.o $(FLAGS_LIB) -o $(DIR_BUILD_TEST)$(BIN)system_processor $(FLAGS_BIN)
	@echo '--- DONE -----------------------------------'

build: build_test_system

### SYSTEM ###

build_test_system: test_system_processor.o

test_system_processor.o: $(DIR_SYSTEM)processor.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_SYSTEM)processor.c -o $(DIR_BUILD_TEST)test_system_processor.o

### RUN ###

run:
	@echo ''
	@echo '--- RUNNING TEST ---------------------------'
	@cd $(DIR_BUILD_TEST) && if ./$(DIR_BUILD_TEST)$(BIN)system_processor $(TEST_FLAGS); \
	then \
		echo '--- PASSED ---------------------------------'; \
	else \
		echo '--- FAILED ---------------------------------'; \
		exit 1; \
	fi
