/***************************************************************************
                          cdromctrl.cpp  -  description
                             -------------------
    begin                : Sat Feb 22 2003
    copyright            : (C) 2003 by Sheldon Lee Wen
    email                : leewsb@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <lineak/cdromctrl.h>
#include <lineak/lineak_core_functions.h>

extern "C" {
#include <fcntl.h>
#include <sys/ioctl.h>
#if defined (__FreeBSD_kernel__) || defined(__FreeBSD_kernel__)
# include <sys/cdio.h>
# define CDROMEJECT CDIOCEJECT /*_IO('c', 107)*/
# define CDROMCLOSETRAY CDIOCCLOSE
#endif
#if defined (__linux__)
#  include <linux/version.h>
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,70)
     typedef unsigned char u8;
#  endif
#  undef __STRICT_ANSI__
#  include <linux/cdrom.h>
#  define __STRICT_ANSI__
#  include <scsi/scsi.h>
#  include <scsi/sg.h>
#  include <scsi/scsi_ioctl.h>
#endif
#if defined (__CYGWIN__)
#  include <windows.h>
#  include <mmsystem.h>
#  define CDROMEJECT CDROMEJECT /*Place holder until I figure out what this is*/
#  define CDROMCLOSETRAY CDROMCLOSETRAY
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

}
#include <iostream>

extern bool verbose;
//using namespace lineak_core_functions;

cdromCtrl::cdromCtrl(){
   cdromdev = snull;
//   cdromejected = false;
   initialized = false;
   fp = -1;
}
cdromCtrl::cdromCtrl(string cdrom){
   cdromdev = cdrom;
//   cdromejected = false;
   initialized = false;
   fp = -1;
   init();
}
cdromCtrl::~cdromCtrl(){
   if (initialized)
      close(fp);
}
/** Read property of string cdromdev. */
const string& cdromCtrl::getCdromdev(){
	return cdromdev;
}
/** Write property of string cdromdev. */
void cdromCtrl::setCdromdev( const string& _newVal){
	cdromdev = _newVal;
//	cdromejected = false;
        if (initialized) {
	   close(fp);
	   initialized = false;
	   fp = -1;
	}	   
	init();
}
/** Initialze the cdrom device. Disable sw eject */
void cdromCtrl::init(){
    #if ! defined (__CYGWIN__)   
   if (verbose) cout << "CD-ROM init, using " << cdromdev << " as the CD-ROM device" << endl;
   /* do we have a CD-ROM device configured?  (REDUNDANT extra check) */
   if ( cdromdev != snull) {
      /* try to open the device .. */
      if ( (fp = open(cdromdev.c_str(), O_RDONLY | O_NONBLOCK)) != -1 ) {
         /* tell the drive to disable auto-eject */
	 autoEject(false);
         initialized = true;
      } else {
         cerr << "... oops! unable to open the CD-ROM device " << cdromdev;
         lineak_core_functions::error("(CD-ROM init)");
	 initialized = false;
      }
   } else
      lineak_core_functions::msg("no CD-ROM device configured! (CD-ROM init)");
   #endif
}
/** Close the cdrom tray. */
void cdromCtrl::closeTray() {
   /* do we have a CD-ROM device configured?  (extra check, not really nescessary) */
   if ( cdromdev != snull && initialized) {
      /* disable the drives software eject again.. */
      //autoEject(fp,false);
      // NOTE WE USE TO ENABLE IT HERE!
      /* close the cdrom tray */
      lineak_core_functions::msg("... closing the CD-ROM tray");
      /** Close the cdrom tray */
      #if ! defined (__CYGWIN__)
      if ( (ioctl(fp, CDROMCLOSETRAY)) == -1 )
         lineak_core_functions::error("... oops! failed to close the CD-ROM");
      #endif
      //autoEject(fp, true);
   } else {
      lineak_core_functions::error(string("... oops! unable to open the CD-ROM device ") + cdromdev);
   }
}
/** Open the cdrom tray. */
void cdromCtrl::openTray(){
   /* do we have a CD-ROM device configured?  (extra check, not really nescessary) */
   if ( cdromdev != snull && initialized) {
      /* disable the drives auto eject */
      //autoEject(fp,true);
      //autoEject(fp,false);
         
      lineak_core_functions::msg("... ejecting the CD-ROM tray");
      /* eject the cdrom tray */
      #if ! defined (__CYGWIN__)
      if ( (ioctl(fp, CDROMEJECT)) == -1 )
         lineak_core_functions::error("... oops! failed to eject the CD-ROM");
      #endif
      //autoEject(fp,true);   
   } else {
        lineak_core_functions::error(string("... oops! unable to open the CD-ROM tray ") + cdromdev);
   }
}
/** OPen the tray for a scsi device */
void cdromCtrl::openTrayScsi(){
#if defined (__linux__)
   /* do we have a CD-ROM device configured?  (extra check, not really nescessary) */
   if ( cdromdev != snull && initialized) {
        lineak_core_functions::msg("... ejecting the SCSI CD-ROM tray");
   
        int status;
        struct sdata {
                int  inlen;
                int  outlen;
                char cmd[256];
        } scsi_cmd;

        scsi_cmd.inlen  = 0;
        scsi_cmd.outlen = 0;
        scsi_cmd.cmd[0] = ALLOW_MEDIUM_REMOVAL;
        scsi_cmd.cmd[1] = 0;
        scsi_cmd.cmd[2] = 0;
        scsi_cmd.cmd[3] = 0;
        scsi_cmd.cmd[4] = 0;
        scsi_cmd.cmd[5] = 0;
        status = ioctl(fp, SCSI_IOCTL_SEND_COMMAND, (void *)&scsi_cmd);
        if (status != 0)
           lineak_core_functions::error("Could not send SCSI command to the CDROM");

//                return 0;

        scsi_cmd.inlen  = 0;
        scsi_cmd.outlen = 0;
        scsi_cmd.cmd[0] = START_STOP;
        scsi_cmd.cmd[1] = 0;
        scsi_cmd.cmd[2] = 0;
        scsi_cmd.cmd[3] = 0;
        scsi_cmd.cmd[4] = 1;
        scsi_cmd.cmd[5] = 0;
        status = ioctl(fp, SCSI_IOCTL_SEND_COMMAND, (void *)&scsi_cmd);
        if (status != 0)
           lineak_core_functions::error("Could not send SCSI command to the CDROM");
//                return 0;

        scsi_cmd.inlen  = 0;
        scsi_cmd.outlen = 0;
        scsi_cmd.cmd[0] = START_STOP;
        scsi_cmd.cmd[1] = 0;
        scsi_cmd.cmd[2] = 0;
        scsi_cmd.cmd[3] = 0;
        scsi_cmd.cmd[4] = 2;
        scsi_cmd.cmd[5] = 0;
        status = ioctl(fp, SCSI_IOCTL_SEND_COMMAND, (void *)&scsi_cmd);
        if (status != 0)
            lineak_core_functions::error("Could not send SCSI command to the CDROM");
 //               return 0;

        /* force kernel to reread partition table when new disc inserted */
        status = ioctl(fp, BLKRRPART);
 //       return (status == 0);
   }
#endif
}
/** Set or disable the auto-eject mode. If auto-eject mode is enabled, the cdrom device
    will eject when we issue a close(fp); */
void cdromCtrl::autoEject(bool istate) {
   int retcode;
   
   /** If we want to enable autoeject */   
   if (istate) {
      #if defined (__FreeBSD__) || defined (__FreeBSD_kernel__)
      retcode = ioctl(fp, CDIOCALLOW);
      #endif
      #if defined (__linux__)
      retcode = ioctl(fp, CDROMEJECT_SW, 1);
      #endif
   }
   /** If we want to disable autoeject */
   else {
      /* disable the drives software eject again.. */
      #if defined (__FreeBSD__) || defined (__FreeBSD_kernel__)
      retcode = ioctl(fp, CDIOCPREVENT);
      #endif
      #if defined (__linux__)
      retcode = ioctl(fp, CDROMEJECT_SW, 0);
      #endif
   }
   /** Print out any error messages. */
   if (retcode != 0) {
      if (istate) {
         cerr << "Error enabling auto-eject mode: " << strerror(errno) << endl;
      }
      else {
         cerr << "Error disabling auto-eject mode: " << strerror(errno) << endl;
      }
   }
}
int cdromCtrl::getStatus() {
   int status = 0;
      /* Get the drive status */
      #if defined (__FreeBSD__) || defined (__FreeBSD_kernel__)
         lineak_core_functions::error("Get status for FreeBSD not implemented.");
      #endif
      #if defined (__linux__)
         lineak_core_functions::msg("Checking the cdrom status");
         status = ioctl(fp, CDROM_DRIVE_STATUS);
         if (status != -1 && status != 0) {
	    if (status == CDS_TRAY_OPEN ) {
	       lineak_core_functions::msg("CD-ROM tray is open");
	       //cout << "Status is: " << status << endl;
	       //cdromejected = true;
	    }
	    else {	    
	       lineak_core_functions::msg("CD-ROM tray is closed");
	       //cdromejected = false;
	    }
	 }
	 else
	    lineak_core_functions::error("Could not check cdrom status");
	    
      #endif
   return status;
}
