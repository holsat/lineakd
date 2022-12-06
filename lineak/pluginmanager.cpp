/***************************************************************************
                          pluginmanager.cpp  -  description
                             -------------------
    begin                : ? 10? 27 2003
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

#include <lineak/pluginmanager.h>
#include <lineak/plugin_definitions.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lcommand.h>
#include <map>
#include <iostream>
#include <algorithm>

#include <lineak/displayctrl.h>
#include <lineak/lkbd.h>
#include <lineak/lconfig.h>

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
}

using namespace lineak_core_functions;
extern bool verbose;
extern bool very_verbose;
extern bool global_enable;

PluginManager::PluginManager() {
}
PluginManager::~PluginManager(){
      if (!plugin_map.empty())
         error("Destructing PluginManager and the plugins have not been unloaded!");
}
bool PluginManager::loadPlugin(const string& plugin) {
        initialize_display_t initialize_display;
        initialize_t initialize;
        cleanup_t cleanup;
	identifier_t identifier;
	identifier_info *plugin_ident;
	directivelist_t directive_list;
	macrolist_t macro_list;

        get_display_t get_display;
	//display_show_t show;
	//display_volume_t volume;
	exec_t exec;

	string name;
	string type;
	string version;
        string description;

        void *handle;
        const char *errmsg;
        plugin_info plinfo;

  	if (plugin == "" || plugin == snull) {
		msg("No plugin to load!");
		return false;
	}

	msg("Loading plugin: "+plugin);
	handle = dlopen(plugin.c_str(),RTLD_NOW);
         // If we cannot dlopen the plugin return false;
	if (handle == NULL) {
		cerr << "Failed to load plugin " << plugin << ": " << dlerror() << endl;
		dlerror();
		//dlclose(handle);
		return false;
	}
	dlerror();
	/** Once we have dlopened the plugin. We must we able to get all of the requisite
	interfaces, or else we skip the plugin all together. The only non-requisite interface
	is the initialize_display interface */

	// Attempt to get the plugin identifier. This will prevent us from loading the same plugin more than once.
	identifier = (identifier_t)dlsym(handle, "identifier");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find identifier() interface: " + string(errmsg));
//		show = NULL;
//		volume = NULL;
                get_display = NULL;
		exec = NULL;
		dlerror();
		dlclose(handle);
		return false;
	} else {
		plugin_ident = identifier();
		name = plugin_ident->identifier;
		type = plugin_ident->type;
		version = plugin_ident->version;
                description = plugin_ident->description;
                msg("plugin name:" + name);
                msg("plugin type:" + type);
		msg("plugin version: " + version); 
		msg("plugin_ident: " + description);
		int Vsize = string(VERSION).size();
		//cout << "Comparing: " << VERSION << " to the first " << Vsize << " characters of: " << version.substr(0,Vsize) << endl;
                if (VERSION != version.substr(0,Vsize)) { 
		//if (VERSION != version) {
			msg("Warning: Version mismatch. Unloading plugin.");
                        dlerror();
                        dlclose(handle);
                        return false;
                }
		if (type == "MACRO"  || type == "DISPLAY" && (name != "" && name != snull))  {
        		// Check to see if that plugin is already registered. If so, skip to the next one.
			if (hasPlugin(name)) {
				error("Plugin: " + name + " is already registered!");
				dlerror();
				dlclose(handle);
				return false;
			}
		} else {
			error("The plugin identifier is empty or is of the incorrect type. This is an invalid plugin");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror();

	// Attempt to open the initialize interface. I have elected not to use _init b/c it may be borked
	// on certain versions of Linux and across platform.
	initialize = (initialize_t)dlsym(handle,"initialize");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find initalize() interface: " + string(errmsg));
		initialize = NULL;
		dlerror();
		dlclose(handle);
		return false;
	}
	dlerror();

	// Attempt to resolve the cleanup interface for the plugin.
	cleanup = (cleanup_t)dlsym(handle,"cleanup");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find cleanup() interface: " + string(errmsg));
		cleanup = NULL;
		dlerror();
		dlclose(handle);
		return false;
	}
	dlerror();

	// Attempt to initialize the osd portion of the plugin.
	initialize_display = (initialize_display_t)dlsym(handle,"initialize_display");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find initalize_display() interface: " + string(errmsg));
		initialize_display = NULL;
	}
	dlerror();

	/** NON Critical interfaces. IE If we don't find these, then we can still proceed. */
        /*
        // Attempt to open the exec interface.
	volume = (display_volume_t)dlsym(handle, "volume");
	if((errmsg = dlerror()) != NULL) {
		error("Didn't find volume interface: "+ string(errmsg));
		volume = NULL;
		if (type == "DISPLAY") {
			error("DISPLAY plugin: " + name + "could not load the volume interface");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror(); */

        get_display = (get_display_t)dlsym(handle, "get_display");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find get_display interface: "+ string(errmsg));
		get_display = NULL;
		if (type == "DISPLAY") {
			error("DISPLAY plugin: " + name + "could not load the volume interface");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror();
        /*
	// Attempt to open the show interface.
	show = (display_show_t)dlsym(handle, "volume");
	if((errmsg = dlerror()) != NULL) {
	        error("Didn't find show interface: "+ string(errmsg));
		show = NULL;
		if (type == "DISPLAY") {
			error("DISPLAY plugin: " + name + "could not load the show interface");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror(); */
	// Attempt to open the exec interface.
	exec = (exec_t)dlsym(handle, "exec");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find exec() interface: " + string(errmsg));
		exec = NULL;
		if (type == "MACRO") {
			error("MACRO plugin: " + name + "could not load the exec interface");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror();
	// Attempt to open the macrolist interface
	directive_list = (directivelist_t)dlsym(handle,"directivelist");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find directivelist() interface: " + string(errmsg));
		directive_list = NULL;
		//dlerror();
		//dlclose(handle);
		//return false;
	}
	dlerror();
	// Attempt to open the macrolist interface
	macro_list = (macrolist_t)dlsym(handle,"macrolist");
	if((errmsg = dlerror()) != NULL) {
		verror("Didn't find macrolist() interface: " + string(errmsg));
		macro_list = NULL;
		if (type == "MACRO") {
			error("MACRO plugin: " + name + "could not open the macrolist interface. This means we cannot get the list of macros that this plugin supports. Closing plugin.");
			dlerror();
			dlclose(handle);
			return false;
		}
	}
	dlerror();

	/** Build the data structure and add it to the plugin_map */
	plinfo.filename = plugin;
	plinfo.handle = handle;
	plinfo.identifier = plugin_ident;
	plinfo.initialize = initialize;
	plinfo.initialize_display = initialize_display;
	plinfo.cleanup = cleanup;
	plinfo.directivelist = directive_list;
	plinfo.macrolist = macro_list;

	//plinfo.volume = volume;
        plinfo.get_display = get_display;
	//plinfo.show = show;
	plinfo.exec = exec;

	plinfo.directives_defined = false;
	plinfo.initialized_display = false;
	plinfo.macros_defined = false;
	//plinfo.have_display = false;
	plinfo.loaded = true;
	plinfo.initialized = false;
	plinfo.macros.clear();
	plinfo.directives.clear();
        vmsg("Entering plugin: " + name);
//	if (verbose) cout << "plinfo ---------" << endl;
//	if (verbose) cout << plinfo << endl;
	plugin_map[name] = plinfo;

	return true;
}
vector<string> PluginManager::loadPlugins(vector<string> myplugins) {
	string plugin;
	vector<string> loadedplugins;
        map<string, plugin_info>::size_type size = plugin_map.size();
        vector<string>::size_type numplugins = myplugins.size();
	//bool loadflag = true;

	if (myplugins.empty()) {
		msg("No plugins to load!!");
                return loadedplugins;
        }
        // Attempt to load all of the plugins.
        vector<string>::iterator it =myplugins.begin();
        for (;it!= myplugins.end(); it++) {
		plugin = *it;
		if(!loadPlugin(plugin)) {
			error("Plugin: " + plugin + " failed to load!");
		}
		else
			loadedplugins.push_back(plugin);
    	}
    	if (plugin_map.size() != (size + numplugins)) {
    		error("Could not load all plugins");
        return loadedplugins;
    }

    return loadedplugins;
}
/** Unload a plugin */
void PluginManager::unloadPlugin(const string& plugin) {
	vmsg("unloadPlugin: enter for plugin " + plugin);
	void* handle = NULL;
	cleanup_t cleanup = NULL;
	vmsg("PluginManager: cleaning up plugin " + plugin);
	handle = plugin_map[plugin].handle;
	cleanup = plugin_map[plugin].cleanup;
	if (cleanup != NULL)
                 cleanup();
        vmsg("PluginManager: closing plugin " + plugin);
        if (handle != NULL) {
                dlclose(handle);
        }
	msg("PluginManager: unloading plugin " + plugin);
	//plugin_map.erase(plugin);
        vmsg("UnloadPlugin: exit");
}
/** Unloads all loaded plugins. */
void PluginManager::unloadAllPlugins(){
        string name = "";
   	map<string,plugin_info>::iterator it = plugin_map.begin();
	int n = plugin_map.size();
	if (verbose) cout << "Plugins to unload: " << n << endl;
        msg("PluginManager is unloading plugins");
	//for (int i=0 ;it != plugin_map.end(); it++) {
	for (unsigned int i=0 ;i < plugin_map.size(); i++) {
                if (very_verbose) cout << "Calling unload for plugin: " << it->first << endl;
		unloadPlugin(it->first);
		it++;
	}
	vmsg("PluginManager finished unloading plugins");
        // Remove all plugin entries in the map.
	plugin_map.clear();
	msg("PluginManager unloading completed");

}
/** Initialize a particular plugin */
bool PluginManager::initializePlugin(const string& plugin_ident, LKbd & imyKbd, LConfig & imyConfig, PluginManager *iplugins) {
	/** Check to see that the plugin is loaded */
	if(plugin_ident != "" && plugin_ident != snull && 
	   (hasPlugin(plugin_ident))) {
		if (plugin_map[plugin_ident].loaded == true) {
			initialize_t initialize;
			msg("Initializing Plugin:" + plugin_ident);
			initialize = plugin_map[plugin_ident].initialize;
			// Insurance
			if (initialize != NULL) {
				// Get the initialization information
				init_info init;
				init.kbd = &imyKbd;
				init.config = &imyConfig;
				init.plugins = iplugins;
				init.verbose = verbose;
				init.very_verbose = very_verbose;
				init.global_enable = global_enable;
				/* #ifdef HAVE_XOSD
				init.have_display = true;
				#else
				init.have_display = false;
				#endif */
				// Call the plugins initialize interface
				if (initialize(init)) {
				   plugin_map[plugin_ident].initialized = true;
				}
				else {
				   error("Plugin: " + plugin_ident + " failed to initialize. Removing plugin.");
				   unloadPlugin(plugin_ident);
				   return true;
				}
				return true;
			}
			else {
				error("initializePlugin: Could not find interface initialize() for plugin: " + plugin_ident);
				return false;
			}
		}
		else {
			error("initializePlugin: " + plugin_ident + " has not been loaded");
			return false;
		}
	}
	else {
		error("initializePlugin: Operating on an empty plugin.");
		return false;
	}
	return true;
}
/** Call the initialize interface for all plugins that we know about internally. */
bool PluginManager::initializePlugins(LKbd & imyKbd, LConfig & imyConfig,PluginManager *iplugins) {
        map<string,plugin_info>::iterator it = plugin_map.begin();
	bool initflag = true;
	/** For all plugins, initialize them. If any does not initialize, return false */
        for (;it != plugin_map.end(); it++) {
                if (!initializePlugin((it->first),imyKbd, imyConfig,iplugins))
			initflag = false;
        }
    	return initflag;
}

bool PluginManager::initializePluginDisplay(const string& plugin, displayCtrl & imyDisplay) {
	/** Check to see that the plugin is loaded */
	if(plugin != "" && plugin != snull) {
		if (plugin_map.find(plugin) == plugin_map.end())
			return false;
		if (plugin_map[plugin].loaded == true ) {
			msg("Initializing OSD for plugin: " + plugin);
        		initialize_display_t initialize_display;
			initialize_display = plugin_map[plugin].initialize_display;
	        	if (initialize_display != NULL) {
        	        	if (!initialize_display(&imyDisplay))
					return false;
				plugin_map[plugin].initialized_display = true;
				return true;
			}
			else {
				msg("initializePluginDisplay: Could not find interface initialize_display() for plugin: " + plugin);
				return false;
			}
		}
		else {
			error( "initializePluginDisplay: " + plugin + " has not been loaded");
			return false;
		}
	}
	else {
		error("initializePluginDisplay: Operating on an empty plugin.");
		return false;
	}
	return true;
}
/** Call the initialize interface for all plugins. */
bool PluginManager::initializePluginsDisplay(displayCtrl & imyDisplay) {
        map<string,plugin_info>::iterator it = plugin_map.begin();
	bool initflag = true;
        for (;it != plugin_map.end(); it++) {
		if (verbose) cout << "InitializePluginsDisplay = " << it->first << endl;
        	if(!initializePluginDisplay((it->first), imyDisplay))
			initflag = false;
        }
    	return initflag;
}
//#endif
/** Scan the plugins directory for plugins. */
vector<string> PluginManager::scanForPlugins(const string& directory){
	DIR *dp;
	struct dirent *entry;
	//struct stat statbuf;
        vector<string> plugins;
	string fullpath = directory + '/';
	if ((dp = opendir(directory.c_str())) == NULL) {
		error("Cannot open plugin directory: " + directory);
		return plugins;
	}
	//chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		char* file = entry->d_name;
		struct stat statbuf;
		lstat(file, &statbuf);
		if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 ) {
			continue;
		}
		//if(S_ISLNK(statbuf.st_mode) || S_ISREG(statbuf.st_mode)) {
		if(!S_ISLNK(statbuf.st_mode)) {
                        // Only pickup .so files.
                        string sname = fullpath + string(entry->d_name);
                        //if (sname.find('.so')) { 
                        //   cout << "Plugin! " << sname << endl;
                        //   plugins.push_back(sname);
                        // }
                        if (dlopen(sname.c_str(),RTLD_NOW) != NULL)
			   plugins.push_back(sname);
                }
	}
	closedir(dp);
	return plugins;
}
/** Get the list of macros defined per plugin */
bool PluginManager::defineMacroList(const string& plugin) {
	/** Check to see that the plugin is loaded */
	if(plugin != "" && plugin != snull) {
		plugin_info info = plugin_map[plugin];
		if (info.loaded == true ) {
			macrolist_t macrolistfn;
			macro_info* macinfo;
			msg("Defining Macro Lists for Plugin:" + plugin);
			macrolistfn = info.macrolist;
			// Insurance
			if (macrolistfn != NULL) {
				string *array;
				string tmp;
				macinfo = macrolistfn();
				if (macinfo == NULL) {
					msg("Couldn't get macinfo for plugin: " + plugin);
					return false;
				}
				array = macinfo->macro_list;
				// If the plugin defined no macros and is a MACRO plugin, unload it and return false.
				// If it is not a MACRO plugin, just return. It is not an error for a non-macro plugin to
				// not hanve any macros defined.
				if (macinfo->num_macros == 0) {
					if (info.identifier->type == "MACRO") {
						unloadPlugin(plugin);
						return false;
					}
					return true;
				}
       		                for (int i=0; i < macinfo->num_macros; i++) {
       			                tmp = array[i];
			                // Get the vector of macros for this plugin, and add this macro to the end
        		                // Add this plugins macros to the list of all supported macros.
					msg("Adding macro: " + tmp);
                                        (plugin_map[plugin].macros).push_back(tmp);
					macrolist.push_back(tmp);
       		                }
				plugin_map[plugin].macros_defined = true;
				return true;
			}
			else {
				error("Macrolist for plugin: " + plugin + " is empty");
				return false;
			}
		}
		else {
			error("defineMacroList:" + plugin + " has not been loaded or initialized");
			return false;
		}
	}
	else {
		error("defineMacroList: Operating on an empty plugin.");
		return false;
	}
	return true;
}
/** Get the list of macros defined across all plugins. */
bool PluginManager::defineMacroLists() {
	if (!plugin_map.empty()) {
		map<string,plugin_info>::iterator it = plugin_map.begin();
		/** For each plugin */
		bool macroflag = true;
		for (;it != plugin_map.end(); it++) {
			if (!defineMacroList(string(it->first))) {
				macroflag = false;
			}
		}
		return macroflag;
	}
	return false;
}
/** Define the directives supported by a plugin */
bool PluginManager::defineDirectivesList(const string& plugin) {
	/** Check to see that the plugin is loaded */
	if(plugin != "" && plugin != snull) {
		plugin_info info = plugin_map[plugin];
		if (info.loaded == true ) {
			directivelist_t directivelistfn;
			directive_info* dirinfo;
			msg("Defining Directives Lists for Plugin:" + plugin);
			directivelistfn = info.directivelist;
			// Insurance
			if (directivelistfn != NULL) {
//				char **array;
//				char **defaults;
				ConfigDirectives* directives;
				string key, def;
				dirinfo = directivelistfn();
				if (dirinfo == NULL) {
					msg("Couldn't get directives info for plugin: " + plugin);
					return false;
				}
				//array = dirinfo->directive_list;
				//defaults = dirinfo->directive_defaults;
				directives = dirinfo->directives;

				// This should really just unload the plugin and return.
				if ((directives->getKeys()).size() == 0) {
					//unloadPlugin(plugin);
					return true;
				}
				const vector<string>& keys = directives->getKeys();
       		                for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); it++) {
       			                key = *it;
					def = directives->getValue(key);
			                // Get the vector of macros for this plugin, and add this macro to the end
        		                // Add this plugins macros to the list of all supported macros.
					msg("Adding directive: "+key);
                                        (plugin_map[plugin].directives).addValue(key, def);
					// If the directive has not been defined by any other plugin...
					if (!directivelist.isSet(key)) {
						directivelist.addValue(key, def);
					}
					msg("Finished adding directive: "+key);
       		                }
				plugin_map[plugin].directives_defined = true;
				return true;
			}
			else {
				error("Directives list for plugin: " + plugin + " is empty");
				return false;
			}
		}
		else {
			error("defineDirectivesList:" + plugin + " has not been loaded or initialized");
			return false;
		}
	}
	else {
		error("defineDirectivesList: Operating on an empty plugin.");
		return false;
	}
	return true;
}
/** Define the directives that are supported across all plugins */
bool PluginManager::defineDirectivesLists() {
	if (!plugin_map.empty()) {
		map<string,plugin_info>::iterator it = plugin_map.begin();
		/** For each plugin */
		bool directiveflag = true;
		for (;it != plugin_map.end(); it++) {
			if (!defineDirectivesList(string(it->first))) {
				directiveflag = false;
			}
		}
		return directiveflag;
	}
	return false;
}

// Execute a command.
exec_t PluginManager::exec(LObject* imyKey, XEvent xev) {
         map<string,plugin_info>::iterator it = plugin_map.begin();
 	 vector<string>::iterator is;
         vector<string>::iterator siter;
         exec_t exec;
	 //cout << "plugin_manager exec, getting command from the key" << endl;
	 LCommand command;

	 if (imyKey != NULL) {
		if (imyKey->getType() == CODE || imyKey->getType() == SYM)
		   command = imyKey->getCommand(xev.xkey.state);
		if (imyKey->getType() == BUTTON)
		   command = imyKey->getCommand(xev.xbutton.state);
	 } else
            return NULL;
         //cout << "Looking for macro..." << endl;
	 //cout << command << endl;
         //cout << "plugin_manager global_enable = " << global_enable << endl;
 	/** Go through the plugin_map and for each plugin, look at their list of supported macros.
 	    If we find a match, then call that plugins exec */
	if (command.isEmpty()) {
	   error("The command we want to search the plugin list for is empty!");
	   return NULL;
	}
	   
	for (;it != plugin_map.end(); it++) {
	   //cout << "Looking for a plugin to handle this" << endl;
           /** iter will either point to the end() or the element of the current plugin that
               contains the macro. */
           siter = find((it->second).macros.begin(),(it->second).macros.end(),command.getMacroType());
           if (siter != (it->second).macros.end()) {
	      //cout << "Found a plugin: " << *siter << endl;
              break;
	   }

	}
	//cout << "Attempting to execute for plugin: " << *siter << endl;
        if ( it != plugin_map.end() ) {
                msg( "Plugin: " + string(it->first) + " to execute macro " + command.getMacroType());
                exec = (it->second).exec;
		if (exec != NULL) {
			//cout << "Calling exec\n" << endl;
                        return (exec);
                        //cout << " Calling exec(imyKey)" << endl;
			//exec(imyKey);
                        //return true;
                }
	}
	//cout << "returning from plugin_manager exec" << endl;
        // No plugins could be found.
        return NULL;
}
displayCtrl* PluginManager::getDisplay(const LConfig &config) {
    string displayp = config.getValue("Display_plugin");
    //displayCtrl *rdisplay = NULL;

    if (displayp == DEFAULT_DISPLAY_PLUGIN)
       return (NULL);
    //Search for the plugin that corresponds to "Display_plugin" and call it's get_display function.
    else if (hasPlugin(displayp)) {
       plugin_info plinfo = plugin_map[displayp];
       if (plinfo.filename != "" && plinfo.initialized && plinfo.loaded) {
          get_display_t get_display = plinfo.get_display;
          if ( get_display != NULL)
             return (get_display());
	  else
	     msg("Could not return a display controller. Display plugin not found");
       }
    }
    else
       msg("Display plugin not found");

    return (NULL);
}
void PluginManager::initDisplay(const LConfig &config) {
	string displayp = config.getValue("Display_plugin");
	if (displayp != DEFAULT_DISPLAY_PLUGIN && hasPlugin(displayp)) {
		plugin_info plinfo = plugin_map[displayp];
		if (plinfo.filename != "" && plinfo.initialized && plinfo.loaded) {
			get_display_t get_display = plinfo.get_display;
			if ( get_display != NULL) {
				displayCtrl * dis = get_display();
				dis->init();
			}
		}
	}
}

void PluginManager::print() {
	map<string,plugin_info>::iterator it = plugin_map.begin();
	for (; it != plugin_map.end(); it++) {
		cout << "Plugin " << it->first << " of type " << ((it->second).identifier)->type << endl;
	        cout << "  Description: " << ((it->second).identifier)->description << endl;
		if ( !(it->second).macros.empty() ) {
			vector<string>::iterator mit = (it->second).macros.begin();
			for (; mit != (it->second).macros.end(); mit++) {
                   		cout << "  " << *mit << endl;
			}
		}
	}
}
void PluginManager::listPlugins() {
	msg("Listing Plugins");
	map<string,plugin_info>::iterator it = plugin_map.begin();
	for (; it != plugin_map.end(); it++) {
	   cout << "Plugin " << it->first << " of type " << ((it->second).identifier)->type << endl;
           cout << "  Description: " << ((it->second).identifier)->description << endl;
        }
	cout << endl;
}
bool PluginManager::hasPlugin(const string &plugin) {
      if (plugin_map.find(plugin) != plugin_map.end())
	  return true;
      return false; 
}
