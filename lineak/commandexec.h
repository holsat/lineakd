/***************************************************************************
                          commandexec.h  -  description
                             -------------------
    begin                : ?  8? 19 2003
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

#ifndef COMMANDEXEC_H
#define COMMANDEXEC_H

extern "C" {
#include <X11/Xlib.h>
}

#include <config.h>
#include <definitions.h>
#include <string>
#include <vector>

using namespace std;

class LObject;
class LCommand;
class displayCtrl;

/**Execute a command for a given key.
  *@author Sheldon Lee Wen
  */

class CommandExec {
public:
  CommandExec();
  ~CommandExec();
  /** Execute the command for the keycode. */
  static void setDisplayCtrl(displayCtrl* idisplay) {
	  if (idisplay != NULL) display = idisplay;
  }
  bool exec(LObject* imyKey, XEvent xev);
private: // Private methods
  static displayCtrl* display;
};

#endif
