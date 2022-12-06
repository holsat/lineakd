/***************************************************************************
                          lockctrl.cpp  -  description
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

#include <lineak/lockctrl.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/definitions.h>

//#define USE_LOCKFILE
#undef USE_LOCKFILE

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
extern int errno;
}
#include <iostream>
#include <fstream>


#ifdef USE_LOCKFILE
	#define LOCKDIR "/.lineak/"
	using namespace lineak_util_functions;
#else
	#include <lineak/lineak_core_functions.h>
	using namespace lineak_core_functions;
#endif

bool lockCtrl::locked = false;

lockCtrl::lockCtrl(string process){
	proc = process;
#ifdef USE_LOCKFILE
  lockfile = getenv("HOME");
  lockfile += LOCKDIR;
  lockfile += proc;
  lockfile += ".lock";
#endif
  locks = 0;
}
lockCtrl::~lockCtrl(){
}
/** Create the lock */
bool lockCtrl::lock(){
#ifdef USE_LOCKFILE
  if (file_exists(lockfile)) {
  	cerr << "The lockfile: " << lockfile << " already exists. To obtain a new lock, you must remove this file." << endl;
  	locks++;
    return false;
  }

  ofstream ofout;
  ofout.open(lockfile.c_str(), ios::out | ios::trunc );
  if (!ofout) {
    	cerr << "Cannot create the lockfile. Cannot lock the daemon." << endl;
    	ofout.clear();
    	return false;
  }

  ofout << getpid();
  if (!ofout) {
    	cerr << "Cannot set lock" << endl;
    return false;
  }
  ofout.close();
  ofout.clear();
#endif
  locked = true;
  locks++;
  
  return true;
}
/** Remove the lock */
void lockCtrl::unlock(){
#ifdef USE_LOCKFILE
    if (!remove_file(lockfile))
      cerr << "Cannot remove lock " << lockfile << endl;
#endif
	locks--;
	locked = false;
}
/** Is the lock set? */
bool lockCtrl::isLocked(){
#ifdef USE_LOCKFILE
  return (file_exists(lockfile));
#else
  return (is_running(proc));
#endif
  // In the future we may want to return based on the lock variable counting.
}
