/***************************************************************************
                          configloader.cpp  -  description
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
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <configloader.h>
#include <lineak_util_functions.h>
#include <lineak_core_functions.h>
#include <configdirectives.h>

using namespace lineak_util_functions;
using namespace lineak_core_functions;
ConfigDirectives ConfigLoader::dnd;
bool ConfigLoader::dnd_init = false;
extern bool very_verbose;

//ConfigLoader::ConfigLoader() {
//}
ConfigLoader::ConfigLoader(string filename, ConfigDirectives idnd) : Loader(filename) {
	ConfigLoader::setDirectives(idnd);
	//LConfig::
}
ConfigLoader::~ConfigLoader() {
}
/** Process a config file with a single section.
 *  This method recieves the entire config file data as a vector of strings. Each entry in the vector
 *    represents a single line of the config file.
 *
 *  This method splits the config file entries on the '=' character and stores
 *    the results in a map that is passed to getConfObj()
 **/
map<const string, string>* ConfigLoader::processSingle(vector<string>* rawData) {
        if (rawData->empty())
    		return pnull;

       map<const string, string> *parsedData = new map<const string, string>;
       parsedData->clear();
       string tmpStore = snull;
       string key=snull;
       string data=snull;
       vector<string>::size_type loc = 0;

       for(vector<string>::size_type i = 0; i < rawData->size(); i++) {
	    // Assign the current line of input to tmpStore.
            tmpStore = (*rawData)[i];

	    /** Split the entry on the first '=' character. 
	     *  Ignore lines that do not have a '=' character. 
	     *  Strip the key and data of extraneous spaces at the front and back.
	     *  */
            loc = tmpStore.find('=');

            if (loc == 0) continue;
            key = strip_space(tmpStore.substr(0,loc));
	    if (very_verbose) cout << "key = " << key << endl;
	    data = tmpStore.substr(loc+1, tmpStore.size());
	    if (very_verbose) cout << "data = " << data << endl;
	    //cout << "doing strip space" << endl;
            data = strip_space(tmpStore.substr(loc+1, tmpStore.size()));
	    if (very_verbose) cout << "data = " << data << endl;

	    // Put the key and data in the map.
            (*parsedData)[key] =  data;
       }
      delete (rawData);
      return parsedData;
}

/** Load the config for the file */
LConfig ConfigLoader::loadConfig() {
	if (!dnd_init) {
		error("Parsing the config file with no configuration directives!");
		LConfig conf;
		return conf;
	}
      	return (*(getConfObj(processSingle(loadFile()))));
}
/** Load the config for the file filename */
LConfig ConfigLoader::loadConfig(string filename){
	LConfig tmp;
	if ( setFile(filename) && dnd_init)
		return (loadConfig());
	return tmp;
}
void ConfigLoader::setDirectives(ConfigDirectives& idnd) {
	dnd.clear();
	dnd = idnd;
	dnd_init = true;
}
/** Load the parsed information and return a config object. */
LConfig* ConfigLoader::getConfObj(map<const string, string> *results){

    // If we don't have any values loaded. Return pnull
    if (results == pnull ) {
        error("Could not load config file values!!");
    	return (new LConfig);
    }
    if (results->empty()) {
        error("Could not load config file values!!");
        delete (results);
	return (new LConfig);
    }

	ConfigDirectives d;

	/** If we have initialized the configuration directives that
	 *  we should be looking for from both the program using this
	 *  class and the plugins, then search for these keys in the
	 *  results map and add their values to the LConfig objects
	 *  ConfigDirectives object.
	 *  */
	if (dnd_init) {
		// Set d to contain all of the directives and defaults we have thus far.
		d = dnd;
		string value;
		string key;
		/** For each directive we know about, see if the config file has a value defined.
		    If there is one defined, then update the directives with the config file defined value.
		*/
		vector<string> keys = d.getKeys();
		for (vector<string>::iterator it = keys.begin(); it != keys.end(); it++) {
			key = *it;
			// If the config file has a value for this key, set value to that value and update it.
			value = (*results)[key];
			if (value != "" && value != snull) {
				d.addValue(key,value);
			}
			// Remove this key from the results map so that we do not process it as a command.
			results->erase(key);
		}
		/** Go through any tuples that may be string,int in the config directives. We put them back in
		 *  as string,string tuples though if we find a key in the results (which is all strings). */
		keys = d.getIntKeys();
		for (vector<string>::iterator it = keys.begin(); it != keys.end(); it++) {
			key = *it;
			// If the config file has a value for this key, set value to that value and update it.
			value = (*results)[key];
			if (value != "" && value != snull) {
				d.addValue(key,value);
			}
			// Remove this key from the results map so that we do not process it as a command.
			results->erase(key);
		}
			
	} else {
		error("Creating a config when we have not initialized the directives we know about");
		LConfig *conf = new LConfig(d);
		return (conf);
	}

	// Initialize and fill in the values for the config object here.
	LConfig *conf = new LConfig(d);

	string keyname = snull;
	string parsed = snull;
	string modifiers = snull;
	string display = snull;
	//unsigned int modifier = 0;
	int index,index2;
	keycommand_info info;
	
	/** Now go through the rest of the file
	 *  At this point, all of the configuration directives has been removed
	 *  from the results table.
	 *  We should only have [Display]key+modifier = command type entries. 
	 *
	 *  This for loop will parse the remaining text entries and build keycommand_info structures.
	 *  Once a structure is built it will assign it to the LConfig object with the parsed name
	 *  of the key entry as the key the LConfig object will use. 
	 *
	 *  The 'key' portion of the results map should look like this:
	 *     [Display]key+modifier1+modifier2+...
	 *  The command in the results map will be an unparsed string. This string gets parsed by
	 *  the LCommand object. */
	for (map<const string, string>::const_iterator i = results->begin(); i != results->end(); i++) {
	        info.modifiers = 0;
	        info.parsed_name = "";
		info.config_name = "";
		info.display_name = "";
	
	        // Strip spaces from the front and back of the first part.	
		keyname = strip_space(i->first);
		// Maybe we need to respect spaces here? i.e. = i->first instead of keyname.
	        parsed = keyname;	
                /** parse the keyname for a display name option. Strip off the display name portion. */
		if (keyname.find('[') != string::npos &&
		    keyname.find(']') != string::npos) {
		   index = keyname.find('[');
		   index2 = keyname.find(']');
		   display = strip_space(keyname.substr(index,index2));
		   /** Set the display name properly */
		   info.display_name = display.substr(index+1,index2-index-1);
		   /** Set the parsed keyname to the remaining key portion */
		   parsed.erase(index,index2-index+1);
		   parsed = strip_space(parsed);
		}
	        //cout << "New keyname = " << keyname << endl;	
                index = 0;
		/** parse the keyname for modifiers. Strip off the modifier portion. */
		if (keyname.find('+') != string::npos) {
		   index = keyname.find('+');
		   modifiers = keyname.substr(index+1, keyname.size()-(index+1)); 
		   info.modifiers = lineak_core_functions::getModifierNumericValue(modifiers);
		   /** Set the parsed keyname to the current parsed keyname minus the modifier portion. */
		   parsed.erase(parsed.find('+'), (parsed.size()-((parsed.find('+')))) );

		}
		   
		info.config_name = keyname;   
		info.parsed_name = strip_space(parsed);   
		info.command = LCommand((i->second));
		/** Add the key and command mapping */
		conf->addKeycomm(parsed, info);				
    }
    msg("done configloader");
    // Explicitly set the filename
    conf->setFilename(getFile());

    delete (results);
    return (conf);
}

LConfig & operator>>(ConfigLoader &ldr, LConfig &lconf) {
        lconf.clear();
	return (lconf = ldr.loadConfig());
}

