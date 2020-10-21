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

#include "utils.hpp"
#include "UI/ui.hpp"
#include "process.hpp"
#include "device.hpp"

#include <libmount.h>
#include <sys/utsname.h>

	
#define SEC_WAIT_TO_CONNECT 2 // segundos

struct FindItemData {
	DeviceProperty * devProp;
	int idx;
};


enum class GUI_STATE { SPLASH, MAIN, BACKUP, FORMAT };

static UIMain * gui = nullptr;
static UIInfo * info = nullptr;
static UIMsg * msg = nullptr;
static volatile bool continueBackup = true;
//static volatile bool continueFormat = true;
static GUI_STATE currentState = GUI_STATE::SPLASH;

static pthread_t currentThread;

static char const * mountDir2 = nullptr;
static char const * mountDir3 = nullptr;
static char const * mountPart2 = nullptr;
static char const * mountPart3 = nullptr;



static void OnTimeOutSplash (void * data);

static void QuitApp ();

void TerminateApp ();

static void AddItemToDeviceList (DeviceProperty const & devProp);

static void RemoveItemFromDeviceList (char const * devPath);

static void DeleteAllDeviceList ();

static FindItemData FindItemFromDeviceList (char const * devPath);

static DeviceProperty const * GetItemFromDeviceList (int idx);

static DeviceProperty const * GetItemFromDeviceList ();

static FindItemData const GetFindItemFromDeviceList (void);

static void SelectNextItemDeviceList ();

static void SelectPrevItemDeviceList ();

static void MonitorEvent (FL_SOCKET fd, void * data);

static void FixFlBrowser ();

static char const * const MakeItemDeviceList (DeviceProperty const & devProp);

static void CheckRootPermission ();

static void ValidateNavButtonInfo ();

static void FillInformationInfo ();

static void SaveFileInfo ();

static void * SaveFileBackupThread (void * data);

static void ChangeGuiState (GUI_STATE const state);

static char const * GetSaveFileName (char const * const defaultFileName);

static void BackupDevice ();

static char const * const GetHomeDirectory ();

static GUI_STATE tryToCorrectGuiState (GUI_STATE const state);

static void MountPartition (char const * const source,
							char const * const target);

static void UMountPartition (char const * const source,
							char const * const target);


static struct libmnt_context * NewMountContext (char const * const source,
												char const * const target);

static char const * GetTempDir ();

static struct utsname * GetSysInfo ();

static void * FormatDeviceThread (void * data);

static void FormatDevice ();

static void FormatDeviceCancel ();

/* vim: set ts=4 sw=4 tw=500 noet :*/
