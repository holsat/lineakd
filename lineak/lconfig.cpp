/***************************************************************************
                          lconfig.cpp  -  description
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

#include <lineak/lconfig.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/configdirectives.h>
#include <lineak/lkbd.h>
#include <lineak/lobject.h>
#include <lineak/lkey.h>
#include <lineak/lcommand.h>

using namespace lineak_util_functions;
using namespace lineak_core_functions;

LConfig::LConfig() {
}
LConfig::LConfig(const LConfig & rhs) : LConfigData() {
	if (this != &rhs) {
		keycommtable = rhs.keycommtable;
		
                directives.clear();
    		directives = rhs.directives;
//                vec.clear();
//                vec = rhs.vec;
	}
}
LConfig & LConfig::operator=(const LConfig & rhs) {
	if (this != &rhs) {
		keycommtable = rhs.keycommtable;
		
                directives.clear();
    		directives = rhs.directives;
//                vec.clear();
//                vec = rhs.vec;
	}
	return *this;
}
LConfig::LConfig(ConfigDirectives &dirs) {
	directives = dirs;
	if (directives.isEmpty())
		error("Creating a config with no ConfigDirectives!");
}
LConfig::~LConfig(){
}
/** Empty all data */
void LConfig::clear() {
  directives.clear();
  keycommtable.clear();
}
/** Empty data for all key configurations */
void LConfig::clearKeycomms() {
  keycommtable.clear();
}
/** Get a directives value */
string LConfig::getValue(string key) const {
	return directives.getValue(key);
}
/** Set a directives value */
void LConfig::setValue(string key, string value) {
	directives.addValue(key,value);
}
/** Get a directives integer value */
int LConfig::getIntValue(string key) const {
	return directives.getIntValue(key);
}
/** Set a directives integer value */
void LConfig::setValue(string key, int value) {
	directives.addValue(key,value);
}
/** Set the configfilename */
void LConfig::setFilename(string file) {
	directives.addValue(_CD_USERCONFFILE, file);
}
/** Add a keycomm to the configuration. */
void LConfig::addKeycomm(const string key, keycommand_info info) {
    map<const string, vector<keycommand_info> >::iterator it = keycommtable.begin();
    it = keycommtable.find(key);
    if (key.find('+') != string::npos)
       cerr << "WARNING, YOU ARE ADDING AN UNPARSED KEYNAME TO THE LCONFIG. THIS WILL NOT WORK!!!" << endl;
    if ( it == keycommtable.end() ) {
       vector<keycommand_info> tvec;
       tvec.push_back(info);
       keycommtable[key] = tvec;
    }
    else {
       // Check to make sure you are not adding a duplicate, that is exactly the same.
       vector<keycommand_info> tmp = keycommtable[key];
       vector<keycommand_info>::iterator mit = tmp.begin();
       for (; mit != tmp.end(); mit++) {
	       if ( mit->config_name == info.config_name &&
		    mit->parsed_name == info.parsed_name &&
		    mit->modifiers == info.modifiers &&
		    mit->command.getCommand() == info.command.getCommand() ) break;
       }
       
       //mit = tmp.find(info);
       if ( mit == tmp.end() ) {
          //cout << "Adding keycommand_info" << endl;
          keycommtable[key].push_back(info); }
       else {
	  // This was the default before.
//FIXME    if ( /** if the vector already contains this keycommand_info, we are adding a duplicate */ )
	  cout << "Attempting to add duplicate entry: WARNING I MAY BE BREAKING MODIFIERS HERE!" << endl;
	  cout << "string = " << key << " command = " << info.command.getCommand() << endl;
          //keycommtable[key].push_back(info);
       }
    }
}
/** Remove an entry from a key. */
void LConfig::removeKeycomm(const string key, keycommand_info info) {
   map<const string, vector<keycommand_info> >::iterator it = keycommtable.begin();
   it = keycommtable.find(key);
   if ( it != keycommtable.end() ) {
      // Find the entry.
      vector<keycommand_info>::iterator mit = keycommtable[key].begin();
      for (; mit != keycommtable[key].end(); mit++) {
	     // Find a matching entry.
	     if ( mit->config_name == info.config_name &&
                  mit->parsed_name == info.parsed_name &&
                  mit->modifiers == info.modifiers &&
                  mit->command.getCommand() == info.command.getCommand() )	   
		// Remove matching entry.
                keycommtable[key].erase(mit);      
      }
   }
}

/** Remove a key */
void LConfig::removeKey(const string key) {
   if (haveKeycomm(key) && key != "")
	   keycommtable.erase(key);
}

/** Return a vector of keycommand_info's that correspond to a key. 
 *  We return by reference to allow updates. */
vector<keycommand_info>& LConfig::getKeycomm(const string key) {
    map<const string, vector<keycommand_info> >::iterator it = keycommtable.begin();
    it = keycommtable.find(key);
    if ( it != keycommtable.end() ) {
       return (keycommtable[key]);
    }
    else {
       tv.clear();
       return tv;
    }
}
vector<string> LConfig::getKeycommKeys() {
     vector<string> ret;
     map<const string, vector<keycommand_info> >::iterator it = keycommtable.begin();
     for (; it!=keycommtable.end(); it++) {
	     ret.push_back(it->first);
     }
     return ret;
}

bool LConfig::haveKeycomm(const string key) {
     map<const string, vector<keycommand_info> >::iterator it = keycommtable.begin();
     for (; it!=keycommtable.end(); it++) {
	     if (it->first == key)
		     return true;
     }
     return false;
}

/** Update all of the data for a key entry. */
void LConfig::updateKeycommData(const string key, vector<keycommand_info> info) {
    if ( keycommtable.find(key) != keycommtable.end() ) {
	    keycommtable[key] = info;
    }
    else {
	    //msg("Adding a new key through an update.");
	    cout << "Adding key: " << key << " through an update" << endl;
	    keycommtable[key] = info;
    }
}
/** Is this an empty object? Our critieria of being empty is:
	if both the conffilename and KeyboardType values are undefined or snull
	if the command table is empty
	*/
const bool LConfig::isEmpty(){
	msg("Checking some isSet's to see if they are empty.");
	if ((!directives.isSet(_CD_KEYBOARD_TYPE) && !directives.isSet(_CD_USERCONFFILE)) || keycommtable.empty() )
		return false;
        msg("Checking the KeyboardType and conffilename to see if they are empty.");
	return ((directives.getValue(_CD_KEYBOARD_TYPE) == snull) && (directives.getValue(_CD_USERCONFFILE) == snull));
}

void LConfig::print(ostream &out) {
        //cout << "printing lconfig\n";
	//string value;
    	out << CONF_HEADER;
	//string kbtype, cd, mix, conf;

    	// WE MAY WANT TO JUST DO:  out << directives << endl;
	directives.print(out);
	out << endl;
	// Output all of the commands now.
    	LCommand command;
	for (map<const string, vector<keycommand_info> >::const_iterator i = keycommtable.begin(); i != keycommtable.end(); i++ ) {
		// Get the keycommand_info struct.
	        const vector<keycommand_info>& comms = (i->second);
	        // For all of the structs in our map....	
	        for (unsigned int j = 0; j < (i->second).size(); j++) {
		     // Print out the config file name
		     out << comms[j].config_name << " = ";
		     // Print out the command
		     command = comms[j].command;
		     out << command.getCommand() << endl;
		}
    	}
}
