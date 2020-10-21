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

#include <string.h>
#include <assert.h>
#include <cinttypes>

#include <unistd.h>
#include <libudev.h>
#include <blkid/blkid.h>
#include <sys/ioctl.h>

/* Estas macros esta definidas en blockdev.h */

#define BLKRAGET		_IO(0x12,99)   /* get current read ahead setting */
#define BLKGETSIZE64	_IOR(0x12,114,size_t) /* return device size in bytes (u64 *arg) */


#include "utils.hpp"

// Constantes para ReceiveEvents.
#define RECEVENT_NONE 0		// ningun evento.
#define RECEVENT_ADD  1		// nuevo dispositivo.
#define RECEVENT_REMOVE 2	// dispositivo removido.


typedef struct
{
	char node_path		[41];
	char id_vendor		[41];
	char id_product		[41];
	char product		[41];
	char manufacturer	[41];
	char serial			[41];
	char version		[41];
	char max_power		[41];
	char bus			[41];
	char const * size;
	off_t totalbytes;
	long readhead;

}DeviceProperty;

typedef void (*ListDevicesCallback)(void);

/* huebibot.hpp */
extern void TerminateApp ();


void InitializeUdev ();

void UnitializeUdev ();

int32_t GetMonitorEvent ();

const char * GetDeviceAttribute (const char * attr);

int32_t ReceiveEvents ();

DeviceProperty & GetDeviceProperty();

char * GetDevicePath ();


/* vim: set ts=4 sw=4 tw=500 noet :*/
