/***************************************************************************
                          cdromctrl.h  -  description
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

#ifndef CDROMCTRL_H
#define CDROMCTRL_H

#include <lineak/definitions.h>
#include <string>
using namespace std;
/**Control the cdrom device.
  *@author Sheldon Lee Wen
  */
#if defined (__linux__)
#  define DEFAULT_CDROM_DEVICE "/dev/cdrom"
#endif
#if defined (__FreeBSD__) || defined(__FreeBSD_kernel__)
#  define DEFAULT_CDROM_DEVICE "/dev/cd0c"
#endif
#if defined (__CYGWIN__)
#  define DEFAULT_CDROM_DEVICE "D:"
#endif

class cdromCtrl {
public: 
  cdromCtrl();
  cdromCtrl(string cdrom);
  ~cdromCtrl();
  /** Eject the cdrom device. */
  void openTray();
  void openTrayScsi();
  void closeTray();
  /** Write property of string cdromdev. */
  void setCdromdev( const string& _newVal);
  /** Read property of string cdromdev. */
  const string& getCdromdev();
  /** Set the drives autoEject state. */
  void autoEject(bool istate);
  /** Get the drives status. Set's the cdromejected state. */
  int getStatus();
  
private: // Private methods
  /** Initialze the cdrom device */
  void init();
  /** Name of the cdrom device to use. */
  string cdromdev;
  /** State the tray is in. */
  //bool cdromejected;
  int fp;
  bool initialized;
};

#endif
