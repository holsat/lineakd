/***************************************************************************
                          lockctrl.h  -  description
                             -------------------
    begin                : Mon Mar 10 2003
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

#ifndef LOCKCTRL_H
#define LOCKCTRL_H

#include <string>
/**Controls setting, and removing the lockfile, as well as checking whether or not a lineakd process is running.
  *@author Sheldon Lee Wen
  */
using namespace std;

class lockCtrl
{
public:
  lockCtrl (string process);
  ~lockCtrl ();
  /** Create the lock */
  bool lock ();
  /** Relock. Remove and replace the lock */
  bool relock ()
  {
    unlock ();
    return (lock ());
  }
  /** Is the lock set? */
  bool isLocked ();
  /** Remove the lock */
  void unlock ();
private:
  static bool locked;
  string lockfile;
  string proc;
  int locks;
};

#endif
