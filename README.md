# [WIP] huebiboot


<img src="https://github.com/daltomi/huebiboot/blob/master/screenshot/scr00.png"/>

<img src="https://github.com/daltomi/huebiboot/blob/master/screenshot/scr01.png"/>

<img src="https://github.com/daltomi/huebiboot/blob/master/screenshot/scr02.png"/>

<img src="https://github.com/daltomi/huebiboot/blob/master/screenshot/scr03.png"/>


Permite crear un Disco USB (pendrive) MultiBoot Hibrido base.

Esto permite iniciar el pendrive en computadoras con o sin UEFI.

### Basado en:

[Multiboot USB drive - ArchLinux Wiki](https://wiki.archlinux.org/index.php/Multiboot_USB_drive#Hybrid_UEFI_GPT_.2B_BIOS_GPT.2FMBR_boot)

### Requisitos de hardware:

  - Un pendrive vacío de 4 GiB o más. 
  
  :fire: PERDERÁ TODOS LOS DATOS DEL DISPOSITIVO.

### Requisitos de software externo:

| Programa  			| Descripción 								|
| ------------- 		| ------------- 							|
| **sgdisk**  			| *Crear tabla de particiones GPT*  		|
| **mkfs.**{vfat,ntfs,ext3,ext4}  | *Dar formato a las particiones*	|
| **grub-install** 		|*GRUB2: Instalar grub bios y grub efi*		|

Nota: *huebiboot* buscará dichos programas al inicio.

### Librerias:

- libudev
- libmount
- fltk-1.3.x

### Compilación:

```bash
make && sudo ./huebiboot
```

### Tabla de partición y formato:

  - Tabla de partición tipo GPT
  - Partición 1: 'BIOS Boot' de 1 MiB.
  - partición 2: 'EFI' de 50 MiB.
  - partición 3: A elección (FAT32,NTFS,EXT3,EXT4) el resto de espacio disponible del disco.

### Modo de uso (Administrador):

```bash
sudo huebiboot
```

### Participar:

Por favor, utilizar la rama `desarrollo`.

