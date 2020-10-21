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

enum PROCID : char { SGDISK, FAT, NTFS, EXT3, EXT4, GRUB, END_PROCID};

bool CheckExternalProcess ();

char const * const GetProccessName (enum PROCID id);

//static void ShowProcessError (PROCID const id, char const * const error);
//
//FILE * OpenProcess (PROCID const id, char const * const command);
//
//ssize_t ReadProcess (FILE * pipe, char ** line);
//
//int CloseProcess (FILE * pipe);

Error ExecuteProcess (char const * const command);

Error CheckShell ();
/* vim: set ts=4 sw=4 tw=500 noet :*/
