/*
    LX200 AP Driver
    Copyright (C) 2007 Markus Wildi

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef LX200APDRIVER_H
#define LX200APDRIVER_H
#endif

#define getAPDeclinationAxis(fd, x)           getCommandString(fd, x, "#:pS#")
#define getAPVersionNumber(fd, x)             getCommandString(fd, x, "#:V#")
#define setAPPark( fd)                        write(fd, "#:KA", 4)
#define setAPUnPark( fd)                      write(fd, "#:PO", 4)
#define setAPLongFormat( fd)                  write(fd, "#:U", 3)
#define setAPClearBuffer( fd)                 write(fd, "#", 1) /* AP key pad manual startup sequence */
#define setAPBackLashCompensation(fd, x,y,z)  setCommandXYZ(fd, x,y,z, "#:Br")
#define setAPMotionStop( fd)                  write(fd, "#:Q", 3)

#ifdef __cplusplus
extern "C" {
#endif

void set_lx200ap_debug(int value);
int check_lx200ap_connection(int fd) ;
int initializeAPmount(int fd) ;
int getAPUTCOffset (int fd, double *value) ;
int setAPObjectAZ(int fd, double az);
int setAPObjectAlt(int fd, double alt);
int setAPUTCOffset(int fd, double hours) ;
int setAPSlewMode(int fd, int slewMode) ;
int APSyncCM(int fd, char *matchedObject) ;
int APSyncCMR(int fd, char *matchedObject) ;
int selectAPMoveToRate(int fd, int moveToRate) ;
int selectAPSlewRate(int fd, int slewRate) ;
int selectAPTrackingMode(int fd, int trackMode) ;
int swapAPButtons(int fd, int currentSwap) ;
int setAPObjectRA(int fd, double ra) ;
int setAPObjectDEC(int fd, double dec) ;
int setAPSiteLongitude(int fd, double Long) ;
int setAPSiteLatitude(int fd, double Lat) ;

#ifdef __cplusplus
}
#endif
