/***************************************************************************
                          lconfig.h  -  description
                             -------------------
    begin                : Fri Jan 17 2003
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

#ifndef LCONFIG_H
#define LCONFIG_H

#if __GNUC__ >= 3
   #include <fstream>
#else
   #include <fstream.h>
#endif

#include <config.h>

#include <map>
#include <vector>
#include <iostream>
#include <lineak/definitions.h>
#include <lineak/configdirectives.h>
#include <lineak/lconfigdata.h>
#include <lineak/lcommand.h>

class LKey;
class LKbd;
//class LCommand;
class LConfigData;
class LObject;

struct keycommand_info
{
   string config_name;
   string parsed_name;
   string display_name;
   unsigned int modifiers;
   LCommand command;
};

/**Lineak user config file. The LConfig class is a subclass of the LConfigData class.
  *An LConfig object contains all of the configuration file directives information in a
  *ConfigDirectives object. (Directives are all of the global configuration options that
  *are not keys and command. e.g. CdromDevice.) It also contains commands for each key
  *in the form of a map of LCommand objects, that is keyed on the key name (string).
  *@author Sheldon Lee Wen
  */
class LConfig : public LConfigData {
public:

	/**
	  There are 3 ways to instantiate an LConfig object.
		1) Default constructor, will be an empty object.
		2) Copy constructor.
		3) Constructor taking a ConfigDirectives object. This is the recommended way.
	*/
	LConfig();
	LConfig(const LConfig &rhs);
	LConfig(ConfigDirectives &directives);
	LConfig & operator=(const LConfig &rhs);
	virtual ~LConfig();
	
	/**
	The setValue and getValue/getIntValue methods are exactly the same as the
	ConfigDirectives methods addValue and getValue/getIntValue. In fact, they are
	just a wrapper around them for the private ConfigDirectives variable.
	The [] operator just returns a reference a the ConfigDirectives string value
	*/
  	void setValue(string key, string value);
  	void setValue(string key, int value);
  	string getValue(string key) const;
  	int getIntValue(string key) const;
  	string& operator[](string key) { return directives[key]; }

	/** Add a configuration for a key */
	void addKeycomm(const string key, keycommand_info info);
	/** Get all of the configurations for a key */
	vector<keycommand_info>& getKeycomm(const string key);
	/** Remove the keycommand_info entry for a key. */
	void removeKeycomm(const string key, keycommand_info info);
	/** Remove all keycommand_info entries for a key, includig the key itself. */
	void removeKey(const string key);
        /** Update all of the data for a key entry. */
	void updateKeycommData(const string key, vector<keycommand_info> info);
	/** Get all keys for the Keycomm data */
	vector<string> getKeycommKeys();
	/** Do we have a configuration for a key? This returns whether or not
	 *  we know about a key. */
	bool haveKeycomm(const string key);
	/** Get the name of the file that was parsed. */
	inline string getFilename() const { return directives.getValue(_CD_USERCONFFILE); }

	/** The setFilename method is used to set the name of the file that the LConfig
	    was parsed from. It is necessary that the filename and keyboard type be set
	    for the isEmpty() method to work. */
	void setFilename(string file);
	/** Is the config object empty? For this to be a no, the directives object must
	    contain values for the keys conffilename and KeyboardType */
  	const bool isEmpty();
	/** Overwrite the directives list */
	inline void setDirectives(const ConfigDirectives& directivelist) { directives = directivelist; }
	/** Get all keys in the ConfigDirectives list. */
	vector<string> getDirectiveKeys() { return directives.getKeys(); }
	vector<string> getDirectiveIntKeys() { return directives.getIntKeys(); }

	/** print */
	void print(ostream& out);
	/** clear the config object */
	void clear();
	/** clear all key configuration information */
	void clearKeycomms();

private:
	map<const string, vector<keycommand_info> > keycommtable;
	ConfigDirectives directives;
	//vector<string> vec;
	//tv exists just as an empty vector to return in the
	//case that we have no value to return
	vector<keycommand_info> tv;
};

#endif
