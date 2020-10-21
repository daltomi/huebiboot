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

#include "process.hpp"

static char const * listOfProcess [END_PROCID + 1] = {
	"/usr/bin/sgdisk",			/* Crear la tabla de particion GPT y las 3 part.	*/
	"/usr/bin/mkfs.vfat",		/* Dar formato a cada part. 						*/
	"/usr/bin/mkfs.ntfs",		/* Dar formato a cada part. 						*/
	"/usr/bin/mkfs.ext3",		/* Dar formato a cada part. 						*/
	"/usr/bin/mkfs.ext4",		/* Dar formato a cada part. 						*/
	"/usr/bin/grub-install",	/* Instalar grub (mbr y efi). 						*/
	nullptr
};




bool CheckExternalProcess ()
{
	char const ** item = listOfProcess;
	bool is_ok = true;
	char const * error = nullptr;

	FormatString (& error, "%s","Los siguientes programas externos no se encontraron:");

	while (*item)
	{
		if (access (*item, F_OK) != 0)
		{
			FormatString (& error, "%s\n%s", error, *item);
			is_ok = false;
		}
		++ item;
	}
	
	if (not is_ok)
	{
		UIMsg * msg = new UIMsg ();
		msg->Error ("huebiboot - Error", error, "Terminar");
		ShowWindowModal (msg);
		FreeAndNil (& msg);
	}

	FreeAndNil (& error);
	
	return is_ok;
}
	


char const * const GetProccessName (enum PROCID id)
{
	return listOfProcess [id];
}




int CloseProcess (FILE * pipe)
{
	return pclose (pipe);
}




Error CheckShell ()
{
	Error error {nullptr, nullptr, ErrorId::NONE};

	if (system (NULL) == 0)
	{
		error.msg = "No existe una shell para system ().";
		error.id = ErrorId::FAILED;
	}
	return error;
}




Error ExecuteProcess (char const * const command)
{
	Error error {nullptr, nullptr, ErrorId::NONE};

	int const ret = system (command);

	if (ret == -1)
	{
		error.msg = "Error interno a system().";
		error.id = ErrorId::FAILED;
		return error;
	}

	if ( 127 == WEXITSTATUS(ret))
	{
		error.msg = "system () no pudo ejecutar el comando.";
		error.id = ErrorId::FAILED;
		return error;
	}

	if (WIFEXITED(ret))
	{
		if (WEXITSTATUS(ret) != 0)
		{
			error.msg = "El comando terminó con un error.";
			error.id = ErrorId::FAILED;
		}
	} 
	else if (WIFSIGNALED(ret))
	{
		error.id = ErrorId::FAILED;

		if (WTERMSIG(ret) == SIGINT || 
			WTERMSIG(ret) == SIGQUIT ||
			WTERMSIG(ret) == SIGTERM) // killall command	
		{
			FormatString (& error.msg_format, "Se recibió la señal: %s",
							strsignal (WTERMSIG(ret)));
		}
		else
		{
			FormatString (& error.msg_format, "Se recibió una señal no evaluada.\n%s",
							strsignal (WTERMSIG(ret)));
		}
	}

	return error;
}


/* vim: set ts=4 sw=4 tw=500 noet :*/
