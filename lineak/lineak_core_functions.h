/***************************************************************************
                          lineak_core_functions.h  -  description
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
#ifndef LINEAK_CORE_FUNCTIONS_H
#define LINEAK_CORE_FUNCTIONS_H

/* settings from configure */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <errno.h>

#include <string>
#include <lineak/lobject.h>
#include <lineak/definitions.h>

class LConfig;
class LDef;
class ConfigDirectives;

namespace lineak_core_functions {

bool parsedeffile(ConfigDirectives & prefs, LDef & def);
bool parseconffile(ConfigDirectives & prefs, LConfig & myConfig);
void create_new_conf(ConfigDirectives & prefs, LDef & def);
void showkeyboards(LDef & def);
void create_homedir(void);
bool is_running(string process);

void msg(const char *message);
void error(const char *message);
void fatal(const char *message);

void vmsg(const char *message);
void verror(const char *message);
void vfatal(const char *message);

void msg(const string message);
void error(const string message);
void fatal(const string message);

void vmsg(const string message);
void verror(const string message);
void vfatal(const string message);

string getModifierString(unsigned int imod);
string getEventTypeString(EventType_t event_type);
EventType_t getEventType(string event_type);
string getTypeString(KeyType_t type);
KeyType_t getType(string type);
unsigned int getModifierNumericValue(const string modstr);

void enable_IBMRA7993(void);
void send_commands(string command, string args);
}

#endif
