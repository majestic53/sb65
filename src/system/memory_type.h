/**
 * SB65
 * Copyright (C) 2020 David Jolly
 *
 * SB65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SB65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SB65_SYSTEM_MEMORY_TYPE_H_
#define SB65_SYSTEM_MEMORY_TYPE_H_

#include "../../inc/system/memory.h"
#include "../../inc/runtime.h"

#define RAM_LENGTH ADDRESS_LENGTH(ADDRESS_RAM_LOW, ADDRESS_RAM_HIGH)
#define STACK_LENGTH ADDRESS_LENGTH(ADDRESS_STACK_LOW, ADDRESS_STACK_HIGH)
#define ZERO_PAGE_LENGTH ADDRESS_LENGTH(ADDRESS_ZERO_PAGE_LOW, ADDRESS_ZERO_PAGE_HIGH)

#endif /* SB65_SYSTEM_MEMORY_TYPE_H_ */
