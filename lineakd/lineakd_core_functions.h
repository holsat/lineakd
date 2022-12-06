/***************************************************************************
                          lineakd_core_functions.h  -  description
                             -------------------
    begin                : Mon Feb 10 2003
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
#ifndef LINEAKD_CORE_FUNCTIONS_H
#define LINEAKD_CORE_FUNCTIONS_H

/* settings from configure */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <errno.h>

#include <string>
//#include <lineak/definitions.h>
//#include <lineak/cdromctrl.h>

class LConfig;
class LDef;
class ConfigDirectives;

namespace lineakd_core_functions {

void cleanexit(int status);
void showusage(int argc, char *argv[]);
void parseDefinitions(ConfigDirectives & prefs, LDef & def);
void parseConfigs(ConfigDirectives & prefs, LConfig & myConfig);
void restart();
void do_exit();
void signalexit(int sig_num);
void signalrestart(int sig_num);
void signalchild(int sig_num);
void watch_messages(void);
void *handle_messages(void *arg);
void print_event(int event);
void initialize_raw_keyboards(LConfig & config, LDef & def);
void initialize_setkeycodes(LConfig & config, LDef & def);
}

#endif
