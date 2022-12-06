/***************************************************************************
                          pluginmanager.h  -  description
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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include <config.h>
#include <string>
#include <map>
#include <vector>

#include <lineak/plugin_definitions.h>
/**Looks for, loads, and keeps track of plugins.
  *@author Sheldon Lee Wen
  */

class LKbd;
class LConfig;
class LKey;
class displayCtrl;
class LCommand;

using namespace std;
using namespace lineak_plugins;

class PluginManager {
public:
	PluginManager();
	~PluginManager();

	// In the future these should probably just return a pointer to the function which is then executed outside
	// of the context of this class.

	/** Find the plugin supporting the macro for imyKey, and return a pointer to it.
	   *  If one could not be found, return NULL. */
        exec_t exec(LObject* imyKey, XEvent xev);
	/** Show volume meter on the screen. This should pick the plugin used to display, and
	      display it using that plugin. */
	//void volume(float volume, float scale);
	/** Show a string to the screen. This should pick the plugin used to display, and display it
	      using that plugin. */
	//void show(const string& command);
        /** Look for the Display_plugin variable defined. If you find it execute it's get_display function
             to get a pointer to a displayCtrl object. Then return that pointer. */
        displayCtrl* getDisplay(const LConfig &config);

  	/** Unloads all loaded plugins. */
	void unloadPlugin(const string& plugin);
  	void unloadAllPlugins();
        /** Attempt to dlopen the plugins. This called scanForPlugins to get a valid
                list of plugins */
        vector<string> loadPlugins(vector<string> myplugins);
        bool loadPlugin(const string& plugin);
        /** Execute the initialize interface of all plugins that are loaded */
	bool initializePlugins(LKbd & imyKbd, LConfig & imyConfig, PluginManager *iplugins);
        bool initializePlugin(const string& plugin, LKbd & imyKbd, LConfig & imyConfig, PluginManager *iplugins);
        /** Execute the initialize_display interface of all plugins that are loaded */
        bool initializePluginsDisplay(displayCtrl & imyDisplay);
        bool initializePluginDisplay(const string& plugin, displayCtrl & imyDisplay);
	/** Cause display plugin to initialize itself. */
	void initDisplay(const LConfig &config);

	bool defineDirectivesLists();
        bool defineDirectivesList(const string& plugin);
	ConfigDirectives& getDirectivesList() { return directivelist; }

	bool defineMacroLists();
        bool defineMacroList(const string& plugin);
	vector<string>& getMacroList() { return macrolist; }

	 /** Scan the plugins directory for plugins. */
  	vector<string> scanForPlugins(const string& directory = PLUGINDIR);
	const map<string, plugin_info> & getPluginMap() { return plugin_map; }
	void print();
	void listPlugins();
	bool hasPlugin(const string &plugin);

private:
	string plugindir;
        // List of plugin that have been loaded
	vector<string> plugins;
	// List of directives that the plugins support.
	ConfigDirectives directivelist;
	// List of macros all the plugins together support.
	vector<string> macrolist;
	/**   plugin, plugin struct */
	map<string, plugin_info> plugin_map;
};

#endif
