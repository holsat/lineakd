/***************************************************************************
                          xmgr.h  -  description
                             -------------------
    begin                : Tue Feb 18 2003
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

/** This is a temporary replacement of the XKB handling code until I can get
 *  the XKB handling code working properly. */
#ifndef XMGR_H
#define XMGR_H

/* settings from configure */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

extern "C" {
#include <stdio.h>
//#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
//#include <X11/Xos.h>
//#include <X11/Xutil.h>
//#include <X11/Xatom.h>
//#include <X11/Xproto.h>
#include <X11/XKBlib.h>
//#include <X11/extensions/XKBfile.h>
}

#include <string>
#include <iostream>

#include <lineak/definitions.h>
#define MAXRETRIES 25 /* prevents possible race condition, don't set this too low!! 25 will do */

class LKbd;
class LKey;
class LButton;

/**Manager initializing and restoring X configurations.
  *@author Sheldon Lee Wen
  */

class Xmgr {
public: 
  Xmgr(string display = snull);
  ~Xmgr();
  Display* getDisplay() { return ourdisplay; }
  Window getWindow() const { return win; }
  int getScreen() const { return screen; }
  bool initialized();
  bool initialize(LKbd& myKbd);
  void cleanup(LKbd& myKbd);
  void xkbRemapped(bool value) { xkbremapped = value; }
  bool xkbRemapped() { return xkbremapped; }
  void grabKey(LKey* key);
  void grabButton(LButton* key);
  unsigned int getNumlockMask() const { return numlock_mask; }
  unsigned int getScrolllockMask() const { return scrolllock_mask; }
  unsigned int getCapslockMask() const { return capslock_mask; }

private:
  static int Xhandler(Display* theDisplay, XErrorEvent* xev);
  bool getModifiers();	
  void openXkbDisplay(string display);
  void commitXKBChanges(int tcode);
  void setXKBKey(int keycode, KeySym keysym);


  Display *ourdisplay;
  int screen;
  Window win;
  string displayname;
  unsigned long mask;
  int min, max;

  XkbDescPtr xkb;
  
  static int ret;
  static unsigned int numlock_mask;
  static unsigned int scrolllock_mask;
  static unsigned int capslock_mask;
  static bool xkbremapped;
  static bool caughtXerror;
};

#endif
