/*
	Copyright (c) 2017, daltomi <daltomi@disroot.org>

	This file is part of huebiboot.

	huebiboot is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	huebiboot is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with huebiboot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "utils.hpp"


int FormatString (char const ** str, char const * format, ...)
{
	va_list list;
	va_start (list, format);
	char const * ptr = *str;
	int const size = vasprintf ((char**)str, format, list);
	va_end (list);
	FreeAndNil (& ptr);
	return size;
}



void * MallocAlign (long const size, char ** memoryToFree)
{
	size_t const align = sysconf(_SC_PAGESIZE);
	*memoryToFree = (char*) malloc (size + align);
	assert (*memoryToFree != nullptr);
	char * mem = (char *) *memoryToFree + align - 1;
	return (void *) (mem - (size_t) mem % align);
}


bool CheckFileExist (char const * const fileName)
{
	return 0 == access (fileName, F_OK);
}
