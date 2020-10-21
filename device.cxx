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


#include "device.hpp"

static struct udev * m_udev = nullptr;
static struct udev_device * m_device = nullptr;
static struct udev_monitor * m_monitor = nullptr;

static int isDisk (struct udev_device * dev);
static struct udev_device * isDiskUSB (struct udev_device * dev );
static void makeDeviceProperty ();
static void setDeviceSize ();
static off_t getDeviceSize ();
static void setReadhead ();





int isDisk(struct udev_device* dev)
{
	char const * stype = udev_device_get_devtype (dev);
	return (stype && (0 == strncmp (stype, "disk", 4))) ? 1 : 0;
}



struct udev_device* isDiskUSB(struct udev_device* dev )
{
   struct udev_device * parent;
   parent = udev_device_get_parent_with_subsystem_devtype(
			dev,
			"usb",
			"usb_device");

   return parent;
}



DeviceProperty & GetDeviceProperty()
{
	static DeviceProperty dev_prop; // one shot.
	return dev_prop;
}



char * GetDevicePath ()
{
	static char node_path [40] = {'\0'};
	return node_path;
}



const char* GetDeviceAttribute(const char* attr)
{
	return  udev_device_get_sysattr_value (m_device, attr);
}



void makeDeviceProperty()
{
	GetDeviceProperty() = (DeviceProperty){0};

	strncpy(GetDeviceProperty ().node_path,    GetDevicePath () , 40);
	strncpy(GetDeviceProperty ().id_vendor,    GetDeviceAttribute ("idVendor"), 40);
	strncpy(GetDeviceProperty ().id_product,   GetDeviceAttribute ("idProduct"), 40);
	strncpy(GetDeviceProperty ().product,      GetDeviceAttribute ("product"), 40);
	strncpy(GetDeviceProperty ().manufacturer, GetDeviceAttribute ("manufacturer"), 40);
	strncpy(GetDeviceProperty ().version,      GetDeviceAttribute ("version"), 40);
	strncpy(GetDeviceProperty ().max_power,    GetDeviceAttribute ("bMaxPower"), 40);
	strncpy(GetDeviceProperty ().bus,          GetDeviceAttribute ("busnum"), 40);

	const char * serial = GetDeviceAttribute ("serial");

	// Validar el serial
	unsigned char const * pc = (unsigned char const *) serial;

	for (; pc && *pc != '\0'; pc++)
	{
		if (*pc < 0x20 || *pc > 0x7f || *pc == ',')
		{
			serial = "NN"; // serial no valido
			break;
		}
	}

	strncpy(GetDeviceProperty().serial, serial, 40);

	/* XXX: testeando una falla en la funcion setDeviceSize */
	sleep (1);
	/**/

	setDeviceSize ();
	setReadhead ();
}



int32_t ReceiveEvents()
{
	assert(NULL != m_monitor);

	int32_t ret = RECEVENT_NONE;

	struct udev_device* dev = udev_monitor_receive_device (m_monitor);

	if ( 0 == isDisk(dev) )
	{
		udev_device_unref (dev);
		return ret;
	}

	struct udev_device * parent;

	strncpy (GetDevicePath (), udev_device_get_devnode(dev), 40);

	const char * action = udev_device_get_action(dev);

	if ((parent = isDiskUSB(dev)))
	{
		m_device = parent;

		if ( !strncmp(action, "add", 3))
		{
			ret = RECEVENT_ADD;
			makeDeviceProperty();
		}
		else if ( !strncmp(action, "remove", 6) )
		{
			ret = RECEVENT_REMOVE;
		}
	}

	udev_device_unref(dev);

	return ret;
}



int32_t GetMonitorEvent()
{
	assert (NULL != m_udev);

	m_monitor = udev_monitor_new_from_netlink (m_udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype (m_monitor, "block", NULL);
	udev_monitor_enable_receiving (m_monitor);
	return udev_monitor_get_fd (m_monitor);
}



void InitializeUdev ()
{
	m_udev = udev_new ();
	assert (NULL != m_udev);
}



void UnitializeUdev ()
{
	if (m_monitor)
	{
		udev_monitor_unref (m_monitor);
		m_monitor = nullptr;
	}

	if (m_udev)
	{
		udev_unref (m_udev);
		m_udev = nullptr;
	}
}



off_t getDeviceSize ()
{
	off_t size = -1;

	int fd = open (GetDevicePath (), O_RDONLY, 0);

	assert (fd != -1);

	int ret = ioctl (fd, BLKGETSIZE64, & size);

	assert (-1 != ret);

	close (fd);

	return size;
}



void setReadhead ()
{
	long readhead = -1;

	int fd = open (GetDevicePath (), O_RDONLY, 0);

	assert (fd != -1);

	int ret = ioctl (fd, BLKRAGET, & readhead );

	assert (-1 != ret);

	assert (-1 != readhead);

	close (fd);

	GetDeviceProperty().readhead = readhead;
}



void setDeviceSize ()
{
	GetDeviceProperty ().totalbytes = getDeviceSize ();

	float bytes = (float) GetDeviceProperty().totalbytes;

	/*
	 * Basado en: https://codegolf.stackexchange.com/a/52202
	 * */
	char const * units = " kMGTPEZY";
	while ((bytes /= 1024 ) >= .95)
	{
		++ units;
	}
	char const * const format = (*units-' ') ? "%.1f %ciB" : "%.0fB";
	/* * */

	char const * str = nullptr;

	FormatString (& str, format, bytes * 1024, *units);
	
	GetDeviceProperty().size = str;

}

/* vim: set ts=4 sw=4 tw=500 noet :*/
