/*******************************************************************************
  Copyright(c) 2011 Gerry Rozema. All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
*******************************************************************************/

#ifndef USBDEVICE_H
#define USBDEVICE_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <usb.h>

#include "indibase.h"

class INDI::USBDevice
{
protected:
	struct usb_device *dev;
	struct usb_dev_handle *usb_handle;

	int ProductId;
	int VendorId;

	int OutputType;
	int OutputEndpoint;
	int InputType;
	int InputEndpoint;

	struct usb_device * FindDevice(int,int,int);

public:
	int WriteInterrupt(char *,int,int);
	int ReadInterrupt(char *,int,int);

	int ControlMessage();

	int WriteBulk(char *,int,int);
	int ReadBulk(char *,int,int);
	int FindEndpoints();
	int Open();
        USBDevice(void);
        USBDevice(struct usb_device *);
        virtual ~USBDevice(void);
};

#endif // USBDEVICE_H
