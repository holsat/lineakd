/***************************************************************************
                          configloader.h  -  description
                             -------------------
    begin                : Sun Jan 5 2003
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
#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <lconfig.h>
#include <loader.h>
#include <definitions.h>

/** The ConfigLoader class is a sub class of the Loader class. It is responsible for
		loading a lineakd.conf configuration file and returning an LConfig object */

class ConfigLoader : public Loader{

public:
	/** Instantiate an empty ConfigLoader. If you do this, you will either need to call setFile(string)
			or loadConfig(string) to load a configuration file. */
	//ConfigLoader();
	/** Constructor with the full path to the filename to load. 
	 *  For this to actuall work, you need to pass in a ConfigDirectives object that
	 *  contains all of the config file directives that we care about, and their default
	 *  values. If a directive is not in there, we will not load it as a configuration
	 *  directive. It will be loaded as a key name. */
	ConfigLoader(string filename, ConfigDirectives idnd);
  	virtual ~ConfigLoader();
  	/** Load a user config file. If the file could not be loaded, an empty LConfig
  		object is returned. */
  	LConfig loadConfig();
  	/** Load the config file filename. If the file could not be loaded, and empty LConfig
  		object is returned. */
  	LConfig loadConfig(string filename);
	/** Pass in a map of directives and defaults for each directive.
	      anything not specified in this list will be loaded as a command to be executed. */
	static void setDirectives(ConfigDirectives& idnd);
private:
	/** This method gets rawData in the form of each line of the config file,
			and parses each line into a key, and a value (as each configuration directive
			is of the form: key = value It returns these name, value pairs in a map */
  	map<const string, string>* processSingle(vector<string>* rawData);
  	/** Parse the name, value pairs, and load them into an LConfig object. */
  	LConfig* getConfObj(map<const string,string> *results);
	/** list of directives and defaults to parse for. */
	static ConfigDirectives dnd;
	static bool dnd_init;
   
};

/** This overloaded operator, will allow semantics of the form:
		configloader >> lconfig
		It makes the loading of a config file similar to a stream extraction operation.
		This is purely for convenience */
LConfig & operator>>(ConfigLoader &ldr, LConfig &lconf);
#endif

 
