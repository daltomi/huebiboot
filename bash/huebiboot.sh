#!/usr/bin/bash
# Autor: daltomi <daltomi@disroot.org> <github.com/daltomi>
#
# Permite crear un Disco USB (pendrive) MultiBoot Hibrido:
# Esto permite iniciar el pendrive en computadoras con o sin UEFI.
#
# Basado en:
# https://wiki.archlinux.org/index.php/Multiboot_USB_drive#Hybrid_UEFI_GPT_.2B_BIOS_GPT.2FMBR_boot
#
# Requisitos de hardware:
#  - Un pendrive de 4 GiB o más.
#
# Requisitos de software:
#  - sgdisk, mkfs.vfat, grub2, udisks2.
#
# Formato:
#  - Tabla de partición tipo GPT
#  - Partición 1: 'BIOS Boot' de 1 MiB.
#  - partición 2: 'EFI' de 50 MiB.
#  - partición 3: 'FAT32 con el resto de espacio disponible del disco.
ROJO='\e[0;31m'
AZUL='\e[44m'
ALERTA='\e[5m'
BLANCO='\e[0;37m'
NORMAL='\e[0m'
TITULO='\e[42m'
DISPOSITIVO=$1
echo
echo -e " _____________"
echo -e "  ${TITULO} _________ ${NORMAL}"
echo -e "  ${TITULO} huebiboot ${NORMAL}"
echo -e "    ${TITULO}       ${NORMAL}"
echo -e "    ${TITULO}|||||||${NORMAL}"
echo
[[ $EUID -ne 0 ]] && echo -e "${ROJO}   Éste script debe ser ejecutado por root.${NORMAL}" && exit 1
[[ -z $DISPOSITIVO ]] && echo -e "${ROJO}   Falta nombre de dispositivo.${NORMAL}\n   Ejemplo: multiboot.sh /dev/sd(X)\n" && exit 1
function desmontar () {
	udisksctl unmount --block-device ${DISPOSITIVO}2 &>/dev/null
	udisksctl unmount --block-device ${DISPOSITIVO}3 &>/dev/null
}
trap 'echo -e "\n\n${ROJO}Se recibió una señal. Terminado.${NORMAL}";desmontar;exit 1' SIGTERM SIGINT
echo -n -e "${BLANCO} ${AZUL}[*] Verificando si el dispositivo existe...                               "
stat $DISPOSITIVO &>/dev/null
[[ $? == 1 ]] && echo -e "\n${ROJO}   No se encontró el dispositivo: $DISPOSITIVO${NORMAL}\n" && exit 1
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Buscando programas...                                                 "
function buscar_programa () {
	command -p $1 &>/dev/null
	[[ $? != 1 ]] && echo -e "\n${ROJO}   No se encontró el programa '$1'.${NORMAL}\n" && exit 1
}
buscar_programa sgdisk
buscar_programa udisksctl
buscar_programa grub-install
echo -e ": Ok ${NORMAL}"
echo -e "${BLANCO} ${AZUL}[*] Iniciando formato...                                                  : ?  "
echo -e "${ROJO}${ALERTA}\n  ¡¡¡ALERTA!!! ${NORMAL} ${ROJO} SE DARÁ FORMATO COMPLETO AL DISPOSITIVO: '$DISPOSITIVO'${NORMAL}"
echo -n -e "${ROJO}   ¿Está seguro de continuar? si/no: ${NORMAL}"
read respuesta
[[ $respuesta != "si" ]] && exit 1
echo -n -e "${BLANCO} ${AZUL}[*] Eliminando las particiones....                                        "
sgdisk -Z $DISPOSITIVO 1>/dev/null || exit $?
echo -e ": Ok ${NORMAL}"
INICIO_LARGO=$(sgdisk -f $DISPOSITIVO | tail -n1)
INICIO=$(sgdisk -F $DISPOSITIVO | tail -n1)
FIN=$(sgdisk -E $DISPOSITIVO | tail -n1)
ALINEACION=$(sgdisk -D $DISPOSITIVO | tail -n1)
echo -n -e "${BLANCO} ${AZUL}[*] Creando las particiones....                                           "
sgdisk -n 1:$INICIO:$(($INICIO + (1 * $ALINEACION) - 1)) -t 1:EF02 $DISPOSITIVO 1>/dev/null || exit $?
INICIO=$(sgdisk -F $DISPOSITIVO | tail -n1)
sgdisk -n 2:$INICIO:$(($INICIO + (50 * $ALINEACION) - 1)) -t 2:EF00 $DISPOSITIVO 1>/dev/null || exit $?
INICIO=$(sgdisk -F $DISPOSITIVO | tail -n1)
sgdisk -n 3:$INICIO:$(($FIN - ($ALINEACION - $INICIO_LARGO))) -t 3:0700 $DISPOSITIVO 1>/dev/null || exit $?
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Creando MBR hibrido...                                                "
sgdisk -h 1,2,3 $DISPOSITIVO 1>/dev/null || exit $?
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Cambiando atributo de la part. # 3: legacy_boot...                    "
sgdisk -A 3:set:2 $DISPOSITIVO 1>/dev/null || exit $?
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Formateando la partición #2 ...                                       "
mkfs.vfat -F 32 ${DISPOSITIVO}2 &>/dev/null || exit $?
sync
echo -e ": Ok ${NORMAL}"
echo -e "${BLANCO} ${AZUL}[*] Opciones de formato para la partición #3 ...                          : ?  ${NORMAL}"
TIPOS_FORMATOS="vfat ntfs ext3 ext4"
PS3="Elija el tipo de formato para la partición #3: "
PROGRAMA=""
select tipo in $TIPOS_FORMATOS; do
	if [ $tipo ]; then
		case $tipo in
			vfat)
				PROGRAMA="mkfs.vfat"
				buscar_programa $PROGRAMA
				PROGRAMA="${PROGRAMA} -F 32"
				;;
			ntfs)
				PROGRAMA="mkfs.ntfs"
				buscar_programa $PROGRAMA
				PROGRAMA="${PROGRAMA} -f" # fast
				;;
			ext3)
				PROGRAMA="mkfs.ext3"
				buscar_programa $PROGRAMA
				PROGRAMA="${PROGRAMA} -qF" # quiet,force
				;;
			ext4)
				PROGRAMA="mkfs.ext4"
				buscar_programa $PROGRAMA
				PROGRAMA="${PROGRAMA} -qF" # quiet,force
				;;
		esac
		break
	else
		echo "Opcion no válida."
		echo
	fi
done
echo -n -e "${BLANCO} ${AZUL}[*] Formateando la partición #3 con $tipo ...(puede durar unos minutos)    "
$PROGRAMA ${DISPOSITIVO}3 &>/dev/null || exit $?
sync
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Montando las particiones 2 y 3 ...                                    "
MONTADO_2=$(udisksctl mount --block-device ${DISPOSITIVO}2 | awk '{print $4}') || exit $?
MONTADO_3=$(udisksctl mount --block-device ${DISPOSITIVO}3 | awk '{print $4}') || exit $?
MONTADO_2=${MONTADO_2:0:-1}
MONTADO_3=${MONTADO_3:0:-1}
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Instalando grub en las particiones 2 y 3 ...(puede durar unos minutos)"
OBJETIVO_EFI=$([[ $(uname -m) == "i686" ]] && echo "i386-efi" || echo "x86_64-efi")
OBJETIVO_BIOS="i386-pc"
mkdir $MONTADO_3/boot
grub-install --target=$OBJETIVO_EFI --efi-directory=$MONTADO_2 --boot-directory=$MONTADO_3/boot --removable --recheck &>/dev/null || exit $?
grub-install --target=$OBJETIVO_BIOS --boot-directory=$MONTADO_3/boot --recheck $DISPOSITIVO &>/dev/null || exit $?
sync
echo -e ": Ok ${NORMAL}"
echo -n -e "${BLANCO} ${AZUL}[*] Desmontando las particiones 2 y 3 ...                                 "
desmontar
echo -e ": Ok ${NORMAL}"
echo -e "${BLANCO} ${AZUL}[*] Listo.                                                                     ${NORMAL}"

# vim: set ts=4 sw=4 tw=500 noet :
