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


#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>

enum ErrorId { FAILED = 0, NONE = 1 };

struct Error {
	char const * msg;
	char const * msg_format;
	enum ErrorId id;
};


#define ErrLog(...) {								\
	char * msg = NULL;								\
	asprintf (&msg, __VA_ARGS__);					\
	fprintf(stderr, L"@Func:%s #Line:%d $Msg:%s",	\
			__func__ , __LINE__ , msg);				\
	free (msg);										\
}

#define CheckIf(valor, accion) {		            \
	if ((valor)) {									\
		accion;                                     \
		Log((log));									\
	}                                               \
}

