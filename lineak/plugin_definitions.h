/***************************************************************************
                          plugin_definitions.h  -  description
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

#ifndef PLUGINDEFINITIONS_H
#   define PLUGINDEFINITIONS_H

#   include <config.h>
#   include <lineak/lconfig.h>
#   include <lineak/configdirectives.h>
#   include <lineak/lobject.h>

#   include <iostream>
#   include <stdlib.h>
extern "C" {
#include <X11/Xlib.h>
}

using namespace std;
class LObject;
class LKbd;
class displayCtrl;
class PluginManager;

namespace lineak_plugins
{

   struct identifier_info
   {
      string description;
      string identifier;
      string type;
      string version;
   };

   ostream & operator<<(ostream &out, identifier_info &rhs);
   
  /* Struct returned by the macrolist interface */
   struct macro_info
   {
      int num_macros;
      string *macro_list;
      string *macro_info;
   };

   ostream & operator<<(ostream &out, macro_info &rhs);

  /** Struct returned by the directivelist interface */
   struct directive_info
   {
      ConfigDirectives *directives;
   };

   ostream & operator<<(ostream &out, directive_info &rhs);
   
  /** This is the information that we are passing to the
   * initialize interface. */
   struct init_info
   {
      LKbd *kbd;
      LConfig *config;
      PluginManager *plugins;
      bool verbose;
      bool very_verbose;
      bool global_enable;
     //bool have_display;
   };

   ostream & operator<<(ostream &out, init_info &rhs);

  /** General interfaces */
   typedef identifier_info *(*identifier_t) ();
   typedef int (*initialize_t) (init_info init);
   typedef int (*initialize_display_t) (displayCtrl * myDisplay);
   typedef void (*cleanup_t) ();

  /** INterfaces for macro plugins */
   typedef int (*exec_t) (LObject * imyKey, XEvent xev);
   typedef macro_info *(*macrolist_t) ();

  /** Interfaces for display plugins */
   typedef directive_info *(*directivelist_t) ();
   typedef displayCtrl *(*get_display_t) ();
   typedef void (*display_show_t) ();
   typedef void (*display_volume_t) ();


   struct plugin_info
   {
      void *handle;
      string filename;          // this is the filename of the plugin.
      identifier_info *identifier;
     // Interfaces mostly used for macro plugins.
      exec_t exec;
      macrolist_t macrolist;
     // Interfaces mostly used for diplay plugins.
      get_display_t get_display;
     //display_show_t show;
     //display_volume_t volume;
     // Interfaces used by all plugins.
      cleanup_t cleanup;
      initialize_t initialize;
      initialize_display_t initialize_display;
      directivelist_t directivelist;
     // Flags for interfaces.
      bool initialized_display;
      bool directives_defined;
      bool macros_defined;
      bool loaded;
      bool initialized;
     // Lists of which macros and directives a plugin supports.
        vector < string > macros;
      ConfigDirectives directives;
   };

   ostream & operator<<(ostream &out, plugin_info &rhs);
}
#endif
