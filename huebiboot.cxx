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

#include "huebiboot.hpp"

/* extern  */
void OnBrowserEvent (Fl_Browser * browser, void * data)
{
	FixFlBrowser ();

}


/* extern  */
void OnButtonEvent (Fl_Button * button, void * data)
{
	if (button == gui->buttonQuit)
	{
		QuitApp ();
	}
	else if (button == gui->buttonCancel)
	{
		QuitApp ();
	}
	else if (button == gui->buttonFormatCancel)
	{
		FormatDeviceCancel ();	
	}
	else if (button == gui->buttonFormat)
	{
		FormatDevice ();
	}
	else if (button == gui->buttonToolInfo)
	{
		info = new UIInfo ();
		FillInformationInfo ();
		ShowWindowModal (info);
		FreeAndNil (& info);
	}
	else if (button == gui->buttonToolHelp)
	{
		printf ("%d\n", gui->selectFormat);
	}
	else if (button == gui->buttonToolBackup)
	{
		BackupDevice ();
	}
	else if (button == gui->buttonBackupCancel)
	{
		continueBackup = false;
	}
	else if (button == info->buttonCloseInfo)
	{
		info->hide ();
	}
	else if (button == info->buttonPrevInfo)
	{
		SelectPrevItemDeviceList ();
		FillInformationInfo ();
		FixFlBrowser ();
	}
	else if (button == info->buttonNextInfo)
	{
		SelectNextItemDeviceList ();
		FillInformationInfo ();
		FixFlBrowser ();
	}
	else if (button == info->buttonSaveInfo)
	{
		SaveFileInfo ();
	}
}




void FormatDeviceCancel ()
{
	pthread_cancel (currentThread);
		
	UMountPartition (mountPart2, mountDir2);
	UMountPartition (mountPart3, mountDir3);
	
	ChangeGuiState (GUI_STATE::FORMAT);

	msg = new UIMsg ();
	msg->Error ("huebiboot - Error", "Se canceló el formato." , "Aceptar");
	ShowWindowModal (msg);
	FreeAndNil (& msg);
	Fl::check ();
}




void SelectNextItemDeviceList ()
{
	int const idx  = gui->browserDevices->value ();
	gui->browserDevices->value (idx + 1);
}



void SelectPrevItemDeviceList ()
{
	int const idx  = gui->browserDevices->value ();
	gui->browserDevices->value (idx - 1);
}



void ValidateNavButtonInfo ()
{
	if (not info)
	{
		return;
	}

	int const size = gui->browserDevices->size ();

	if (size == 1)
	{
		info->buttonPrevInfo->deactivate ();
		info->buttonNextInfo->deactivate ();
		return;
	}

	int const idx  = gui->browserDevices->value ();

	(idx < size) ? info->buttonNextInfo->activate ()
				: info->buttonNextInfo->deactivate ();

	(idx > 1) ? info->buttonPrevInfo->activate ()
			: info->buttonPrevInfo->deactivate ();
}



char const * const GetHomeDirectory ()
{
    static struct passwd const * const pwd = getpwuid (geteuid ());

	/*
	 * getpwuid retorna una estructura estática mientras
	 * no se llame a endpwent () o nuevamente a getpwuid ();
	 * */
    return pwd->pw_dir;
}



void * SaveFileBackupThread (void * data)
{
	DeviceProperty const * const devProp =  GetItemFromDeviceList ();
	char * saveFileName = (char *) data;
	UIMsg * msg;
	char const * error = nullptr;
	int fileSave = -1;
	int fileDevice = -1;

	errno = 0;

	if (-1 == (fileDevice = open (devProp->node_path, O_RDONLY | O_DIRECT | O_EXCL)))
	{
		error = "Ocurrió un error al abrir el dispositivo.\nError : %s.";
		FreeAndNil (& saveFileName);
		goto ERROR;
	}

	errno = 0;

	fileSave = creat (saveFileName, S_IRUSR | S_IWUSR);

	FreeAndNil (& saveFileName);

	if (fileSave == -1)
	{
		close (fileDevice);

		error = "Ocurrió un error al crear el archivo de copia de seguridad.\n"
				"Error: %s.";
ERROR:
		Fl::lock ();
		char const * str = nullptr;
		FormatString (& str, error, strerror (errno));
		msg = new UIMsg ();
		msg->Error ("huebiboot - Error", str , "Aceptar");
		ShowWindowModal (msg);
		FreeAndNil (& msg);
		FreeAndNil (& str);
		ChangeGuiState (GUI_STATE::BACKUP);
		Fl::unlock ();

		return nullptr;
	}


	/*
	 * Nota: lo minimo debe ser 128 KiB.
	 * */
	long const buff_size = MAX (128 * 1024, devProp->readhead * 512);
	char * memoryToFree = nullptr;
	char * buff = (char *) MallocAlign (buff_size, & memoryToFree);
	off_t max_read = devProp->totalbytes;
	ssize_t len = 0;

	errno = 0;

	float const total = (float) max_read;
	float percent = 0;
	char labelPercent [25] = {'\0'};

	while (max_read && (len != -1) && continueBackup)
	{
		len = read (fileDevice, buff, MIN (max_read, buff_size));

		if (len < 0)
		{
			error = "Ocurrió un error al leer el dispositivo.\nError: %s";
			close (fileSave);
			close (fileDevice);
			FreeAndNil (& memoryToFree);
			goto ERROR;
		}

		Fl::lock ();
		snprintf (labelPercent, 25, "Completado: %.1f %%", percent);
		gui->boxBackupBytes->label (labelPercent);
		Fl::check ();
		Fl::unlock ();

		write (fileSave, buff, len);

		max_read -= len;
		percent = ((float)(total - max_read) / total) * 100;
	}

	close (fileSave);
	close (fileDevice);
	FreeAndNil (& memoryToFree);

	if (not continueBackup)
	{
		error = "Se canceló la copia de seguridad.\n";
		goto ERROR;
	}

	Fl::lock ();
	msg = new UIMsg ();
	msg->Information ("huebiboot - Información", "La copia de seguridad ya terminó." , "Aceptar");
	ShowWindowModal (msg);
	FreeAndNil (& msg);

	ChangeGuiState (GUI_STATE::BACKUP);
	Fl::unlock ();
	return nullptr;
}




struct libmnt_context * NewMountContext (char const * const source,
										char const * const target)
{

	struct libmnt_context * cxt = mnt_new_context ();
	assert (cxt != nullptr);

	mnt_context_set_source (cxt, source);
	mnt_context_set_target (cxt, target);
	return cxt;
}


void MountPartition (char const * const source,
					char const * const target)
{
	struct libmnt_context * cxt = NewMountContext (source, target);
	mnt_context_mount (cxt);
	mnt_free_context (cxt);
}



void UMountPartition (char const * const source,
					char const * const target)
{
	if (not (source || target)) return;

	struct libmnt_context * cxt = NewMountContext (source, target);
	mnt_context_umount (cxt);
	mnt_free_context (cxt);
}



struct utsname * GetSysInfo ()
{
	static struct utsname info;

	int ret = uname (& info);

	assert (-1 != ret);

	return & info;
}



char const * GetTempDir ()
{
	char * tempDir = nullptr;

	FormatString ((char const **)& tempDir, "%s", "/tmp/huebibootXXXXXX");

	char * tmp = mkdtemp (tempDir);

	assert (tmp != nullptr);

	return tempDir;
}




void FormatDevice ()
{
	FindItemData item = GetFindItemFromDeviceList ();
	char const * const devName = item.devProp->node_path;
	char const * str = nullptr;
	FormatString (& str, "¿Está seguro de continuar?\n\n"
			"Se dará formato al dispositivo: %s", devName);

	(msg = new UIMsg ())->QuestionWarning ("huebiboot - Formatear", str, "Si" ,"No");
	ShowWindowModal (msg);
	unsigned char const reply = msg->reply;
	FreeAndNil (& msg);
	FreeAndNil (& str);

	if (reply == UIMsg::NO)
	{
		return;
	}

	ChangeGuiState (GUI_STATE::FORMAT);

	pthread_create (& currentThread, nullptr, & FormatDeviceThread,
				(void *) nullptr);

	pthread_detach (currentThread);
}




void * FormatDeviceThread (void * data)
{
	FindItemData item = GetFindItemFromDeviceList ();
	char const * const devName = item.devProp->node_path;

	char const * str = nullptr;

	char const * infoLabel [] = {
		"Eliminando las particiones ...",
		"Creando las particiones ...",
		"Creando MBR hibrido ...",
		"Cambiando atributo de la part. # 3: legacy_boot ...",
		"Formateando la partición #2 ...",
		"Formateando la partición #3 con %s ...(puede durar unos minutos)",
		"Montando las particiones 2 y 3 ...",
		"Instalando grub en las particiones 2 y 3 ...(puede durar unos minutos)",
		"Desmontando las particiones 2 y 3 ..."
	};
	
	size_t const maxInfo = sizeof (infoLabel) / sizeof(infoLabel[0]);
	unsigned short nowInfo = 0;
	char const * title = nullptr;
	char const * progress = nullptr;
	char const * command = nullptr;
	char const * procName = nullptr;
	char const * label = nullptr;
	char const * sformat = nullptr;
	char const * efi = (!strncmp (GetSysInfo ()->machine, "i686", 4)) ? "i386-efi" : "x86_64-efi";
	char const * bios = "i386-pc";
	char const * bootDir = nullptr;

	Error error;
	char const * tempDir = GetTempDir ();

	if (CheckShell ().id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}

	FormatString (& title, "Formateando %s %s",
			item.devProp->manufacturer,
			item.devProp->node_path);

	FormatString (& progress, "%u/%zu",  nowInfo + 1, maxInfo);

	// Valores de inicio.
	gui->boxFormatTitle->label (title);
	gui->formatProgress->minimum (1);
	gui->formatProgress->maximum (sizeof(infoLabel) / sizeof(infoLabel[0]));
	gui->formatProgress->value (1);
	gui->formatProgress->label (progress);


/*SGDISK */
	procName = GetProccessName (PROCID::SGDISK);


/* Eliminando las particiones */
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	FormatString (& command, "%s -Z %s", procName, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/* Creando las particiones ... */
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (2);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	FormatString (& command, "%s -n 1::+1M -t 1:EF02 -n 2::+50M"
			" -t 2:EF00 -n 3:: -t 3:0700 %s", procName, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/* Creando MBR hibrido ... */
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (3);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	FormatString (& command, "%s -h 1,2,3 %s", procName, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/* Cambiando atributo de la part. # 3: legacy_boot ...*/
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (4);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	FormatString (& command, "%s -A 3:set:2 %s", procName, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/* Formateando la partición #2 ... */
	procName = GetProccessName (PROCID::FAT);
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (5);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	FormatString (& command, "%s -F 32 %s2", procName, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/* Formateando la partición #3 */
	switch (gui->selectFormat)
	{
		case UIMain::FORMATS::FAT:
			procName = GetProccessName (PROCID::FAT);
			FormatString (& command, "%s -F 32", procName); // 32b
			sformat = "FAT";
			break;
		case UIMain::FORMATS::NTFS:
			procName = GetProccessName (PROCID::NTFS);
			FormatString (& command, "%s -f", procName); //fast
			sformat = "NTFS";
			break;
		case UIMain::FORMATS::EXT3:
			procName = GetProccessName (PROCID::EXT3);
			FormatString (& command, "%s -qF", procName); //quiet;force
			sformat = "EXT3";
			break;
		case UIMain::FORMATS::EXT4:
			procName = GetProccessName (PROCID::EXT4);
			FormatString (& command, "%s -qF", procName); //quiet;force
			sformat = "EXT4";
			break;
	}

	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	FormatString (& label, infoLabel [nowInfo++], sformat);
	gui->boxFormatInfo->label (label);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (6);
	Fl::check ();
	FormatString (& command, "%s %s3", command, devName);
	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
	FreeAndNil (& label);

/**/


/*  Montando las particiones 2 y 3 ...*/
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (7);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();

	errno = 0;

	/* part. 2 */
	FormatString (& mountDir2, "%s/2", tempDir);
	if (0 != mkdir (mountDir2, S_IRUSR | S_IWUSR))
	{
		goto ERROR_MKDIR;
	}
	FormatString (& mountPart2, "%s2", devName);
	MountPartition (mountPart2, mountDir2);

	/* part. 3 */
	FormatString (& mountDir3, "%s/3", tempDir);
	if (0 != mkdir (mountDir3, S_IRUSR | S_IWUSR))
	{

ERROR_MKDIR:
		FormatString (& error.msg_format, "Ocurrió un error al crear el directorio"
				" temporal.\n.Error: %s", strerror (errno));
		
		FreeAndNil (& bootDir);
		FreeAndNil (& mountDir2);
		FreeAndNil (& mountDir3);
		goto SHOW_ERR;
	}

	FormatString (& mountPart3, "%s3", devName);
	MountPartition (mountPart3, mountDir3);
	
/* Instalando grub en las particiones 2 y 3 */
	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (8);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();
	
	FormatString (& bootDir, "%s/boot", mountDir3);
	if (0 != mkdir (bootDir, S_IRUSR | S_IWUSR))
	{
		goto ERROR_MKDIR;
	}

	FormatString (& command, "%s --target=%s --efi-directory=%s"
			" --boot-directory=%s --removable --recheck",
			GetProccessName (PROCID::GRUB),	efi, mountDir2, bootDir);

	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}

	FormatString (& command, "%s --target=%s --boot-directory=%s --recheck %s",
			GetProccessName (PROCID::GRUB), bios, bootDir, devName);

	error = ExecuteProcess (command);
	if (error.id == ErrorId::FAILED)
	{
		goto SHOW_ERR;
	}
/**/


/*	Desmontando las particiones 2 y 3 */

	FormatString (& progress, "%u/%zu", nowInfo + 1, maxInfo);
	gui->formatProgress->label (progress);
	gui->formatProgress->value (9);
	gui->boxFormatInfo->label (infoLabel [nowInfo++]);
	Fl::check ();

	UMountPartition (mountPart2, mountDir2);
	UMountPartition (mountPart3, mountDir3);


	FreeAndNil (& mountDir2); 
	FreeAndNil (& mountDir3); 
	FreeAndNil (& mountPart2); 
	FreeAndNil (& mountPart3); 
	FreeAndNil (& bootDir);
	FreeAndNil (& command);
	FreeAndNil (& title);
	FreeAndNil (& tempDir);
	FreeAndNil (& progress);

	Fl::lock();
	(msg = new UIMsg ())->Information ("huebiboot - Información",
			"El formato terminó correctamente.", "Aceptar");
	ShowWindowModal (msg);
	FreeAndNil (& msg);
	ChangeGuiState (GUI_STATE::FORMAT);
	Fl::check();
	Fl::unlock();

	return nullptr;

SHOW_ERR:

	UMountPartition (mountPart2, mountDir2);
	UMountPartition (mountPart3, mountDir3);
	
	str = (error.msg) ? error.msg : error.msg_format;

	Fl::lock();
	(msg = new UIMsg())->Error ("huebiboot - Error", str, "Terminar");
	ShowWindowModal (msg);
	FreeAndNil (& msg);
	Fl::unlock();
	FreeAndNil (& error.msg_format);
	FreeAndNil (& command);
	FreeAndNil (& mountDir2); 
	FreeAndNil (& mountDir3); 
	FreeAndNil (& mountPart2); 
	FreeAndNil (& mountPart3); 
	FreeAndNil (& bootDir);
	FreeAndNil (& title);
	FreeAndNil (& tempDir);
	FreeAndNil (& progress);
	exit (EXIT_FAILURE);
}



void BackupDevice ()
{
	DeviceProperty const * const devProp =  GetItemFromDeviceList ();

	char const * defaultFileName = nullptr;

	FormatString (& defaultFileName, "%s_%s.img",
			devProp->node_path, devProp->manufacturer);

	char const * saveFileName = GetSaveFileName (defaultFileName);

	FreeAndNil (& defaultFileName);

	if (not saveFileName)
	{
		return;
	}

	static char const * fileName = nullptr;

	FormatString (& fileName, "Archivo : %s", fl_filename_name (saveFileName));

	gui->boxBackupFile->label (fileName);

	ChangeGuiState (GUI_STATE::BACKUP);

	pthread_t threadId;

	pthread_create (& threadId, nullptr, & SaveFileBackupThread,
				(void *) saveFileName);

	continueBackup = true;

	pthread_detach (threadId);
}



char const * GetSaveFileName (char const * const defaultFileName)
{
	char const * saveFileName = nullptr;

	Fl_Native_File_Chooser chooser (Fl_Native_File_Chooser::Type::BROWSE_SAVE_FILE);

	chooser.directory (GetHomeDirectory() );
	chooser.preset_file (defaultFileName);

	if (0 == chooser.show ())
	{
		bool yesSave = true;

		if (CheckFileExist (chooser.filename ()))
		{
			char const * fileName = nullptr;
			FormatString (& fileName, "El archivo '%s' ya existe.\n"
				"¿Desea reemplazarlo?", fl_filename_name (chooser.filename ()));

			(msg = new UIMsg())->Question ("huebiboot",
				fileName, "Si", "No");

			ShowWindowModal (msg);

			yesSave = (msg->reply == UIMsg::YES);

			FreeAndNil (& msg);
			FreeAndNil (& fileName);
		}

		if (yesSave)
			saveFileName = strndup (chooser.filename (), PATH_MAX);
	}

	return saveFileName;
}



void SaveFileInfo ()
{
	assert (info != nullptr);

	char const * defaultFileName = nullptr;

	FormatString (& defaultFileName, "%s_%s.txt",
		info->outPath->value (), info->outManufacturer->value ());

	char const * saveFileName = GetSaveFileName (defaultFileName);

	FreeAndNil (& defaultFileName);

	if (saveFileName == nullptr)
	{
		return;
	}

	errno = 0;

	FILE * file = fopen (saveFileName, "w");

	if (not file)
	{
		char const * str = nullptr;

		FormatString (& str, "Imposible guardar el archivo '%s'.\n\n"
			"Error: %s",fl_filename_name (saveFileName), strerror (errno));

		(msg = new UIMsg ())->Error ("huebiboot - Error", str, "Aceptar");
		ShowWindowModal (msg);
		FreeAndNil (& msg);
		FreeAndNil (& str);
		FreeAndNil (& saveFileName);
		return;
	}

	char const * const format = "Generado por huebiboot.\n\n" //TODO: version
		"Device\t\t: %s\n"
		"Size\t\t: %s\n"
		"ID Vendor\t: %s\n"
		"ID Product\t: %s\n"
		"Product\t\t: %s\n"
		"Manufacturer: %s\n"
		"Serial\t\t: %s\n"
		"Version\t\t: %s\n"
		"Max. Power\t: %s\n"
		"Bus:\t\t: %s\n";


	char const * str = nullptr;
	
	FormatString (& str, format,
		info->outPath->value(),
		info->outSize->value (),
		info->outVendor->value (),
		info->outIdProduct->value (),
		info->outProduct->value (),
		info->outManufacturer->value (),
		info->outSerial->value (),
		info->outVersion->value (),
		info->outPower->value (),
		info->outBus->value ());

	fwrite (str, strlen (str), 1, file);

	fclose (file);

	FreeAndNil (& str);

	char const * info = nullptr;
	
	FormatString (& info, "El archivo '%s' se ha\nguardado correctamente.",
			fl_filename_name (saveFileName));

	(msg = new UIMsg ())->Information ("huebiboot - Información", info, "Aceptar");

	ShowWindowModal (msg);
	FreeAndNil (& msg);
	FreeAndNil (& saveFileName);
}



void FillInformationInfo ()
{
	if (info == nullptr) return;

	/*
	 * Cuando se quita el último dispositivo
	 * se debe ocultar 'info'.
	 * */
	if (not gui->browserDevices->value ())
	{
		info->hide ();
		return;
	}

	ValidateNavButtonInfo ();

	FindItemData const item =  GetFindItemFromDeviceList ();

	DeviceProperty const * const devProp = item.devProp;

	info->outTitle->label (gui->browserDevices->text (item.idx));
	info->outPath->value (devProp->node_path);
	info->outVendor->value (devProp->id_vendor);
	info->outProduct->value (devProp->product);
	info->outIdProduct->value (devProp->id_product);
	info->outManufacturer->value (devProp->manufacturer);
	info->outSerial->value (devProp->serial);
	info->outVersion->value (devProp->version);
	info->outPower->value (devProp->max_power);
	info->outBus->value (devProp->bus);
	info->outSize->value (devProp->size);
}



/*
 * - Se esperaba que la selección de Fl_Browser se mantenga
 * activa siempre pero el widget quita la selecciónn del item
 * cuando se realiza un click en un área vacía del mismo.
 * - Ésta func. reactiva la última selección.
 * */
void FixFlBrowser ()
{
	static int last_select = 0;

	int const curr_select = gui->browserDevices->value ();

	last_select = (curr_select) ? curr_select : last_select;

	gui->browserDevices->value (last_select);
}



void TerminateApp ()
{
	UnitializeUdev ();

	DeleteAllDeviceList ();

	exit (EXIT_SUCCESS);
}


void QuitApp ()
{
	(msg = new UIMsg ())->Question("Salir", "¿Está seguro de salir?", "Si" ,"No");

	ShowWindowModal(msg);

	bool quit = (msg->reply == UIMsg::YES);

	FreeAndNil (& msg);

	if (not quit)
	{
		return;
	}

	TerminateApp ();
}



GUI_STATE tryToCorrectGuiState (GUI_STATE const newState)
{
	/* Cuando se conecta un dispositivo. */
	if ( (newState == GUI_STATE::MAIN) and
		 (currentState == GUI_STATE::SPLASH))
			return GUI_STATE::MAIN;

	/* Cuando se desconecta un dispositivo pero existen
	 * 	otros conectados. */
	if ( (newState == GUI_STATE::SPLASH) and
		 (currentState == GUI_STATE::MAIN) and
		 (gui->browserDevices->size () != 0))
			return GUI_STATE::MAIN;


	/* Cuando se desconecta el último dispositivo. */
	if ( (newState == GUI_STATE::SPLASH) and
		 (currentState == GUI_STATE::MAIN) and
		 (gui->browserDevices->size () == 0))
			return GUI_STATE::SPLASH;

/* Formatear */

	/* Cuando se conecta un dispositivo. */
	if ( (newState == GUI_STATE::MAIN) and
		 (currentState == GUI_STATE::FORMAT))
			return GUI_STATE::FORMAT;

	/* Cuando se selecciona Formatear */
	if ( (newState == GUI_STATE::FORMAT) and
		 (currentState == GUI_STATE::MAIN) and
		 (gui->browserDevices->size () != 0))
			return GUI_STATE::FORMAT;

	/* Cuando se desconecta el último dispositivo */
	if ( (newState == GUI_STATE::SPLASH) and
		 (currentState == GUI_STATE::FORMAT) and
		 (gui->browserDevices->size () == 0))
	{
			FormatDeviceCancel ();
			return GUI_STATE::SPLASH;
	}

	/* Cuando se desconecta un dispositivo pero existen
	 * 	otros conectados. */
	if ( (newState == GUI_STATE::SPLASH) and
		 (currentState == GUI_STATE::FORMAT) and
		 (gui->browserDevices->size () != 0))
	{
			FormatDeviceCancel ();
			return GUI_STATE::MAIN;
	}

	/* Cuando Formatear termina.*/
	if ( (newState == GUI_STATE::FORMAT) and
		 (currentState == GUI_STATE::FORMAT)) 
			return GUI_STATE::MAIN;


/* Copia de seguridad. */

	/* Cuando se selecciona Copia de seguridad. */
	if ( (newState == GUI_STATE::BACKUP) and
		 (currentState == GUI_STATE::MAIN))
			return GUI_STATE::BACKUP;

	/* Cuando Copia de seguridad termina y existen otros
	 *  dispositivos. */
	if ( (newState == GUI_STATE::BACKUP) and
		 (currentState == GUI_STATE::BACKUP) and
		 (gui->browserDevices->size () != 0))
			return GUI_STATE::MAIN;

	/* Cuando Copia de seguridad termina y no existen otros
	 * 	dispositivos. */
	if ( (newState == GUI_STATE::BACKUP) and
		 (currentState == GUI_STATE::BACKUP) and
		 (gui->browserDevices->size () == 0))
			return GUI_STATE::SPLASH;

	/* Cuando se desconecta un dispositivo pero existe otro en
	 * 	Copia de seguridad.
	 * */
	if ( (newState == GUI_STATE::SPLASH) and
		 (currentState == GUI_STATE::BACKUP))
			return GUI_STATE::BACKUP;

	/* Cuando se conecta un dispositivo pero existe otro en
	 * 	Copia de seguridad.
	 * */
	if ((newState == GUI_STATE::MAIN) and
		 (currentState == GUI_STATE::BACKUP))
			return GUI_STATE::BACKUP;

	return newState;
}



void ChangeGuiState (GUI_STATE const newState)
{
	currentState = tryToCorrectGuiState (newState);

	switch (currentState)
	{
		case GUI_STATE::SPLASH:
		{
			gui->groupFormat->hide ();
			gui->groupMain->hide ();
			gui->groupBackup->hide ();
			gui->groupSplash->show ();
			gui->boxSplashNotFound->hide ();
			Fl::add_timeout (SEC_WAIT_TO_CONNECT, OnTimeOutSplash, nullptr);
			break;
		}
		case GUI_STATE::MAIN:
		{
			gui->groupFormat->hide ();
			gui->groupSplash->hide ();
			gui->groupBackup->hide ();
			gui->groupMain->show ();
			break;
		}
		case GUI_STATE::BACKUP:
		{
			gui->groupSplash->hide ();
			gui->groupMain->hide ();
			gui->boxBackupBytes->label ("");
			gui->groupBackup->show ();
			break;
		}
		case GUI_STATE::FORMAT:
		{
			gui->groupSplash->hide ();
			gui->groupMain->hide ();
			gui->groupFormat->show ();
		}
	}

	Fl::check ();
}



void OnTimeOutSplash (void * data)
{
	static short waitShowNotFound = SEC_WAIT_TO_CONNECT;

	if (--waitShowNotFound == 0)
	{
		waitShowNotFound = SEC_WAIT_TO_CONNECT;
		gui->boxSplashNotFound->show();
		Fl::remove_timeout (OnTimeOutSplash, NULL);
	}
	else
	{
		Fl::repeat_timeout (SEC_WAIT_TO_CONNECT, OnTimeOutSplash, nullptr);
	}
}



void AddItemToDeviceList (DeviceProperty const & devProp)
{
	char const * item = MakeItemDeviceList (devProp);

	gui->browserDevices->add (item, (void*)new DeviceProperty{devProp});

	gui->browserDevices->value (gui->browserDevices->size ());

	FixFlBrowser ();

	FreeAndNil (& item);

	ChangeGuiState (GUI_STATE::MAIN);
}



void RemoveItemFromDeviceList (char const * devPath)
{
	FindItemData find = FindItemFromDeviceList (devPath);

	if (find.idx != -1)
	{
		FreeAndNil (& find.devProp->size); // FormatString
		FreeAndNil (& find.devProp);
		gui->browserDevices->remove (find.idx);
	}

	gui->browserDevices->value (gui->browserDevices->size ());

	FixFlBrowser ();

	ChangeGuiState (GUI_STATE::SPLASH);
}



char const * const MakeItemDeviceList (DeviceProperty const & devProp)
{
	char const * str = nullptr;
	
	FormatString (& str, "%s\t%s\t%s",
		devProp.manufacturer,
		devProp.product,
		devProp.node_path);

	return str;
}



FindItemData FindItemFromDeviceList (char const * devPath)
{
	for (int i = 1; i <= gui->browserDevices->size (); ++i)
	{
		DeviceProperty * devProp = (DeviceProperty *) GetItemFromDeviceList (i);

		if ( !strncmp (devProp->node_path, devPath, 40))
		{
			return {devProp, i};
		}
	}

	return {nullptr, -1};
}



void DeleteAllDeviceList ()
{
	for (int i = 1; i <= gui->browserDevices->size (); ++i)
	{
		DeviceProperty * devProp = (DeviceProperty *) GetItemFromDeviceList (i);
		FreeAndNil (& devProp->size); // FormatString
		FreeAndNil (& devProp);
	}
	gui->browserDevices->clear ();
}



DeviceProperty const * GetItemFromDeviceList (int idx)
{
	return (DeviceProperty *) gui->browserDevices->data (idx);
}


/*
 * El actual item seleccionado.
 * */
DeviceProperty const * GetItemFromDeviceList ()
{
	return GetItemFromDeviceList (gui->browserDevices->value ());
}


/*
 * El actual item seleccionado.
 * */
FindItemData const GetFindItemFromDeviceList ()
{
	int const idx = gui->browserDevices->value ();
	DeviceProperty * devProp = (DeviceProperty *) GetItemFromDeviceList (idx);

	return FindItemData {devProp, idx};
}



void MonitorEvent (FL_SOCKET fd, void * data)
{
	int32_t event = ReceiveEvents ();

	switch (event)
	{
		case RECEVENT_ADD:
			AddItemToDeviceList (GetDeviceProperty ());
			FillInformationInfo ();
			break;

		case RECEVENT_REMOVE:
			RemoveItemFromDeviceList (GetDevicePath ());
			FillInformationInfo ();
			break;
	}
}



void CheckRootPermission ()
{
	if (geteuid() != 0)
	{
		(msg = new UIMsg ())->Information ("huebiboot - Información",
			"Debe tener permisos de Administrador.", "Terminar");
		ShowWindowModal (msg);
		FreeAndNil (& msg);
		exit (EXIT_FAILURE);
	}
}



int main (int argc, char * argv [])
{
	CheckRootPermission ();

	if (not CheckExternalProcess ())
	{
		return EXIT_FAILURE;
	}

	(gui = new UIMain ())->show ();

	InitializeUdev ();

	Fl::add_fd (GetMonitorEvent (), FL_READ, & MonitorEvent, NULL);

	ChangeGuiState (GUI_STATE::SPLASH);

	atexit (TerminateApp);

	return Fl::run ();
}
/* vim: set ts=4 sw=4 tw=500 noet :*/
