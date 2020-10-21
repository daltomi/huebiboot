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

#pragma once

#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>
#include <pthread.h>
#include <limits.h>
#include <signal.h>

#include "error.hpp"
#include "UI/ui.hpp"
#include "device.hpp"
#include "process.hpp"
#include "device.hpp"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))


template<typename T>
static void FreeAndNil (T ** object)
{
	delete *object;
	*object = nullptr;
}

static void FreeAndNil (char const ** object)
{
	free ((void*) *object);
	*object = nullptr;
}

static void FreeAndNil (char ** object)
{
	free (*object);
	*object = nullptr;
}



int FormatString (char const ** str, char const * format, ...);

void * MallocAlign (long const size, char ** memoryToFree);

bool CheckFileExist (char const * const fileName);

