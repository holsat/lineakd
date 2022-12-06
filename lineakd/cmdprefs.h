/***************************************************************************
                          cmdprefs.h  -  description
                             -------------------
    begin                : Fri Feb 21 2003
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

#ifndef CMDPREFS_H
#define CMDPREFS_H
#include <config.h>
#include <lineak/configdirectives.h>
#include <lineak/msgpasser.h>

#include <string>
using namespace std;
class LConfig;
/**Class to set and store command line preferences.
  *@author Sheldon Lee Wen
  */

class CmdPrefs : public ConfigDirectives {
public: 
	CmdPrefs();
	~CmdPrefs();
  	/** Read property of bool verbose. */
  	const bool& getVerbose();
  	const bool& getVeryVerbose();
  	/** Set the command line options on the config object. */
  	void setOpts( LConfig & conf);
	/** Set the command line options to override the config file defaults */
	void setDefaults( ConfigDirectives & conf);
   	/** Parse the command line args, and set the options */
    bool parseArgs(int argc, char *argv[]);
    /** return values of the action flags */
    bool showUsage();
    bool createNewConfig();
    bool showKeyboards();
    bool showPlugins();
    bool showDefinitions();

private: // Public attributes
  /** Run in verbose mode? Set from command line. */
  bool verbose;
  bool very_verbose;
  /** Possible actions to take from parsing the command line options. */
  bool showusage;
  bool createnewconfig;
  bool showkeyboards;
  bool showplugins;
  bool showdefinitions;
  
};

#endif
