/***************************************************************************
                          lineak_core_functions.cpp  -  description
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
#include <lineak/lineak_core_functions.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>



extern "C" {
#ifdef HAVE_GETOPT_H
#  define HAVE_GETOPT_LONG
#  include <getopt.h>
#endif
#ifndef X_NOT_STDC_ENV
#	include <stdlib.h>
#else
	extern char *getenv();
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include <X11/Xlib.h>
}

#include <lineak/definitions.h>
#include <lineak/saver.h>
#include <lineak/configloader.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/defloader.h>
#include <lineak/ldef.h>
#include <lineak/lkbd.h>
#include <lineak/lkey.h>
#include <lineak/lockctrl.h>
#include <lineak/lcommand.h>
#include <lineak/msgpasser.h>
#include <lineak/lobject.h>

//#include "commandexec.h"

extern bool verbose;
extern bool very_verbose;

using namespace lineak_util_functions;
using namespace std;


void lineak_core_functions::msg(const string message) {
   lineak_core_functions::msg(message.c_str());
}
void lineak_core_functions::error(const string message) {
   lineak_core_functions::error(message.c_str());
}
void lineak_core_functions::fatal(const string message) {
   lineak_core_functions::fatal(message.c_str());
}

void lineak_core_functions::vmsg(const string message) {
	   lineak_core_functions::vmsg(message.c_str());
}
void lineak_core_functions::verror(const string message) {
	   lineak_core_functions::verror(message.c_str());
}
void lineak_core_functions::vfatal(const string message) {
	   lineak_core_functions::vfatal(message.c_str());
}

void lineak_core_functions::msg(const char* message) {
    if (verbose) {
       cout << message << endl;
    }
}
void lineak_core_functions::error(const char* message) {
    if (verbose) {
       cerr << message << endl;
    }
}
void lineak_core_functions::fatal(const char* message) {
    if (verbose) {
       cerr << "Fatal Error: " << message << endl;
       msgPasser message;
       message.start();
       message.sendMessage(msgPasser::EXIT,"exit");
    }
}

void lineak_core_functions::vmsg(const char* message) {
    if (very_verbose) {
        cout << message << endl;
    }
}
void lineak_core_functions::verror(const char* message) {
    if (very_verbose) {
        cerr << message << endl;
    }
}
void lineak_core_functions::vfatal(const char* message) {
    if (very_verbose) {
        cerr << "Fatal Error: " << message << endl;
        msgPasser message;
        message.start();
        message.sendMessage(msgPasser::EXIT,"exit");
    }
}

/* Parse the configuration file.
		prefs: is a ConfigDirectives object. It is loaded with values obtained from the command line, and
			other system directives and defaults.
		       In this function, it can contain an alternative location for the users config file.
		myConfig: is a configuration object. It will be loaded with values from the config loader.
		
    Return false if we should exist b/c no config file exists, or we could not load
		any configuration information.
		Return true if we were successful in loading configuration information.
		*/

bool lineak_core_functions::parseconffile(ConfigDirectives & prefs, LConfig & myConfig) {
  string conffilename;
  string homedir = getenv("HOME");
  /* set (globally) full path of config file ($HOME/.lineak/CONFIGFILE) */
  if (prefs.getValue(_CD_USERCONFFILE) == snull) {
  	if (!dir_exists(homedir + LINEAKDIR))
   		create_homedir();
   	conffilename = homedir + CONFFILE;
  } else
  	conffilename = prefs.getValue(_CD_USERCONFFILE);

	/* does the file exist? */
  if (conffilename == snull || !file_exists(conffilename)) {
		conffilename = SYS_CONFFILE;
		if (!file_exists(conffilename)) {
	   	/* file does not exist */
			cerr << "*** A configuration file was not found! ***" << endl;
			cerr << "       Please run lineakd -l to list the supported keyboards." << endl;
			cerr << "       Then run \"lineakd -c TYPE\" to create a configuration file for your keyboard." << endl;
			cerr << "       Once that is done, edit " << (homedir + CONFFILE) << " or " <<  SYS_CONFFILE << " to map your keys to commands, then run lineakd again." << endl;
	   	return(false);
		}
  }
//FIXME: reconcile command line directives, with directives and defaults from the plugins, as well as our program
//	       directives and defaults. Then pass them here to the configldr object.
// prefs at this point is made up of first: directives and defaults from all of the plugins.
// second: any values specified on the command line are added to or overwrite the values in the directives and defaults.
// Now, we will pass this to the configloader, who will likewise overwrite and
   vmsg("Instantiating ConfigLoader");
   msg("Loading a config file");
   //prefs.print(cout);
   msg("conffilename = :" + conffilename);
   ConfigLoader configldr(conffilename, prefs);
   //cout << "Loading a config file. Got a configldr\n";
   //(configldr.loadConfig()).print(cout);
   //cout << "Doing it again.\n" << endl;
   //configldr >> myConfig;
   myConfig = configldr.loadConfig();
   //sleep(2);
   msg("Displaying config.");
   if (verbose) 
	   myConfig.print(cout);
   
   vmsg("Checking to see if it is empty");
   if (myConfig.isEmpty()) {
		cerr << "*** Configuration file " << conffilename << " could not be loaded" << endl;
		cerr << "		Have you defined actions for your keys?" << endl;
		return(false);
   }
   vmsg("Returning from parseconffile");
   return (true); 
}

/* Parse the keyboard definition file.
		prefs: is a ConfigDirectives object. It can be loaded with values obtained from the command line
		       or loaded with other relavant values from another program. In this function, it can contain
		       an alternative locations for the user and the system definitions files.

		Return false if we should exit b/c of no definitions loaded.
		Return true if we are all good. */
bool lineak_core_functions::parsedeffile(ConfigDirectives & prefs, LDef & def) {
  string usrdeffilename = prefs.getValue(_CD_USERDEFFILE); //userdefconf;
  string deffilename = prefs.getValue(_CD_SYSDEFFILE); //defconf;
  string homedir = getenv("HOME");
  LDef udef;

   if (!dir_exists(homedir + LINEAKDIR))
      create_homedir();
   
   if (usrdeffilename == snull) {
		usrdeffilename = homedir + LINEAKDIR;
		usrdeffilename += DEFFILE;
   }
 
   if (file_exists(usrdeffilename)) {
      msg("Parsing: " + usrdeffilename);
      /* it exists, parse the contents */
      DefLoader defldr(usrdeffilename);
      udef = defldr.loadDef();
      if ( udef.isEmpty() )
      /* error parsing file */
      cerr << "*** Error occurred while reading definition data from " << usrdeffilename << endl;
   }
  
   if (deffilename == snull ) {
      /* SET full path of definition files ($sysconfdir/lineakkb.def) */
      deffilename = CONFDIR;
      deffilename += "/";
      deffilename += DEFFILE;
   }
   
   if ( deffilename != usrdeffilename ) {
      msg("Parsing: " + deffilename);
      /* does the file exist? */
      if (file_exists(deffilename)) {
	/* it exists, parse the contents */
	DefLoader sysdefldr(deffilename);
	def = sysdefldr.loadDef();
        if ( def.isEmpty() ) {
	   /* error parsing file */
	   cerr << "*** Error occurred while reading definition data from " << deffilename << endl;
	}
      }
   }

  /* If we cannot load any keyboard definitions */
  if ( def.isEmpty()  && udef.isEmpty() ) {
     /* file does not exist */
     cerr << "*** FATAL ERROR: No keyboard defintions could be loaded!" << endl;
     return(false);
  }
  if (!udef.isEmpty())   {
     /** Append the user definition data to the system data */
     def.addKeyboards(udef);
  }
  vmsg("Outputting the definition file!");
  if (very_verbose) cout << def;
   return (true);
}
/*
void lineak_core_functions::showplugins(PluginManager & plugins) {
        cout << "\nLinEAK v" << VERSION << " -- Loaded Plugins:\n" << endl;
	vector<string>::iterator it = plugins.begin();
	while (it != plugins.end()) {
		cout << "  " << *it << endl;
		it++;
	}
	cout << endl;
	cout << "\nLinEAK v" << VERSION << " -- Loaded Macros:\n" << endl;
	it = macros.begin();
	while (it != macros.end()) {
                cout << "  " << *it << endl;
                it++;
        }
}
*/
void lineak_core_functions::showkeyboards(LDef & def) {
	map<string,LKbd*> &tmp = def.getTable();

	cout << "\nLinEAK v" << VERSION << " -- supported keyboards:\n" << endl;
	cout << " [TYPE]\t\t[Full name]\n" << endl;
    for (map<string,LKbd*>::const_iterator m = tmp.begin(); m != tmp.end(); m++) {
        if (m->first != snull)
    	   printf(" %s%s%s %s\n",(m->first).c_str(),strlen((m->first).c_str())>=7?"\t":"\t\t",strcasecmp(strip((m->second)->brand, "\"").c_str(),"other")==0?"":strip((m->second)->brand, "\"").c_str(),strip((m->second)->model, "\"").c_str());
    }
}
void lineak_core_functions::create_homedir(void) {
	string picsdir;
	string homedir = getenv("HOME");
	/** Make sure that the ~/.lineak directory exists */
	string lineakdir = homedir + LINEAKDIR;
	if (!dir_exists(lineakdir)) {
		if (mkdir(lineakdir.c_str(), 0755) == -1) {
        	cout << "*** FATAL ERROR: unable to create directory" << lineakdir << endl;
        	exit(1);
    	}
  	}
  picsdir = homedir + PICSDIR;
	if (!dir_exists(picsdir)) {
		if (mkdir(picsdir.c_str(), 0755) == -1) {
        	cout << "*** ERROR: unable to create directory" << picsdir << endl;
   	}
 	}
}
/** We assume at this point that the global definitions objects are created */
void lineak_core_functions::create_new_conf(ConfigDirectives & prefs, LDef & def) {
	//string conffilename = prefs.getValue("conffilename");
	//string cdromdev = prefs.getValue("CdromDevice");
	//string mixdev = prefs.getValue("MixerDevice");
	//LDef def = idef;
	string homedir = getenv("HOME");
	string kbtype = prefs.getValue(_CD_KEYBOARD_TYPE);

  	LCommand blank;
	LConfig config;

	// Make sure we have definitions loaded and the kbtype is not empty.
	if (def.isEmpty() || kbtype == snull) {
		cerr << "Attempting to create a config file with no keyboard definitions loaded" << endl;
		exit(false);
	}
	// Attempt to get the definition for the keyboard.
	if (!def.hasKeyboard(kbtype)) {
		cerr << "*** ERROR: Invalid keyboard type: " << kbtype<< " \nTo find out the supported keyboard types, use: 'lineakd -l'\n" << endl;
		exit(false);
	}
  	create_homedir();
	/* set (globally) full path of config file ($HOME/.lineak/CONFIGFILE) */
  	if (prefs.getValue(_CD_USERCONFFILE) == snull)
   		prefs.addValue(_CD_USERCONFFILE, string(homedir + CONFFILE));

  /* no CD-ROM device specified? */
//   if (cdromdev == snull) {
// 		cout << "*** No CD-ROM device specified, using the default /dev/cdrom" << endl;
// 		cdromdev = DEFAULT_CDROM_DEVICE;
// 	}
  /* no mixer device specified? */
//   if (mixdev == snull) {
// 		cout << "*** No mixer device specified, using the default /dev/mixer" << endl;
// 		mixdev = DEFAULT_MIXER_DEVICE;
// 	}

	//cout << "create_new_conf: kbtype = " << kbtype << endl;
        //cout << "printing prefs from create_new_conf" << endl;
	//cout << prefs << endl;
	//cout << "printing defs from create_new_conf" << endl;
	//cout << def << endl;
	
  	config = LConfig (prefs);

  	LKbd & kbd = def.getKeyboard(kbtype);
	keycommand_info info;
/**  	const map< string, LObject*> & keytable = kbd.getObjects(); */
        /** Setup the keyboard objects in the kbd object */
	/**
  	for (map< string, LObject*>::const_iterator m = keytable.begin(); m != keytable.end(); m++) {
	        info.config_name = m->first;
		info.parsed_name = m->first;
		info.modifiers = 0;
		info.command = blank;
		config.addKeycomm(m->first,info);
        }*/
	const vector<string> keylist = kbd.getNames();
	for (vector<string>::const_iterator m = keylist.begin(); m != keylist.end(); m++) {
                info.config_name = *m;
                info.parsed_name = *m;
                info.modifiers = 0;
                info.command = blank;
                config.addKeycomm(*m,info);
        }
	Saver tmp(config.getFilename());

	if (!tmp.saveFile(config))
		exit(false);
	else {
		cout << "\n*** Creating fresh configuration in " << config.getFilename() << "\n      for keyboard type: " << kbtype << endl;
		cout << "NOTE: Now please edit the file and bind commands to the keys,\n      or use klineakconfig :)\n" << endl;
        }
	//cout << "create_new_conf: kbtype = " << kbtype << endl;
        //cout << "printing prefs from create_new_conf second time" << endl;
        //cout << prefs << endl;
        //cout << "printing defs from create_new_conf second time" << endl;
        //cout << def << endl;
	//cout << "printing config from create_new_conf second time" << endl;
	//cout << config << endl;

}

/* check if there's already a running lineakd for this $USER */
bool lineak_core_functions::is_running(string process) {
    uid_t myuid;
    pid_t mypid, pid;
    DIR *dir;
    struct dirent *d;
    struct stat s;
    string str, path;

    mypid = getpid();
    myuid = getuid();
   
   if ((dir = opendir("/proc")) == NULL)
   {
      cout << "Cannot open /proc" << endl;
      return -1;
   }
         
   if (verbose)
	   cout << "Looking for " << process << endl;

   while ((d = readdir(dir)) != NULL)
   {
      // See if this is a process
      if ((pid = atoi(d->d_name)) == 0)
         continue;
     //if (verbose) cout << "pid = " << pid << " mypid = " << mypid << endl;
      // Ignore the pid of the current running process
      if (pid == mypid) {
         //if (verbose) cout << "oops it's me, moving on." << endl;
         continue;
      }
  
      path = "/proc/" + string(d->d_name) + "/stat";
	    
      // Only processes with the same UID as the current running process
      if (stat(path.c_str(), &s) != 0 && s.st_uid != myuid)
         continue;
        
      // Open the status file and verify the program name
      ifstream in(path.c_str());
      if (in.is_open()) {
         in >> str;
         in >> str;
         in.close();
//	 if (verbose) 
//		 cout << "Stat: " << str << endl;

	 // Only look for the first 15 characters as linux cuts off the stat entries after 15 characters. 
         if (str.find("(" + process.substr(0,15)) != string::npos && (mypid != pid)) {
    		if (verbose) cout << "*** " << process << " is running (pid " << pid << ")" << endl;
                if (verbose) cout << "*** mypid: " << mypid << endl;
                if (verbose) cout << str << endl;
                // turn this on to see what the situation is like when we are starting.
                /* for (int i=0; i != 50; i++) {
                        sleep (5);
                } */
                return true;
	 }
      }
   }
   return false;
}
string lineak_core_functions::getModifierString(unsigned int imod) {
	// Print out the modifiers
	string modifier_string[] = { "control", "shift", "alt", "mod2", "mod3", "mod4", "mod5"};
	string str = "";
		
	if (imod & ControlMask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[0];
	}
	if (imod & ShiftMask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[1];
	}
	if (imod & Mod1Mask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[2];
	}
	if (imod & Mod2Mask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[3];
	}
	if (imod & Mod3Mask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[4];
	}
	if (imod & Mod4Mask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[5];
	}
	if (imod & Mod5Mask)
	{
		if (str[0])
		str+="+";
		str+=modifier_string[6];
	}                                                
	return str;
}
string lineak_core_functions::getEventTypeString(EventType_t event_type) {
        switch (event_type) {
		case PRESS:
			return "PRESS";
			break;
		case RELEASE:
			return "RELEASE";
			break;
		default:
			return "UNKNOWN";
	}
	return "UNKNOWN";
}
EventType_t lineak_core_functions::getEventType(string event_type) {
	if (event_type == "PRESS")
		return PRESS;
	if (event_type == "RELEASE")
		return RELEASE;
	//Return an unknown
	return UNKNOWN_EVENT;
}
string lineak_core_functions::getTypeString(KeyType_t type) {
	switch (type) {
		case SYM:
			return "SYM";
			break;
		case CODE:
			return "CODE";
			break;
		case BUTTON:
			return "BUTTON";
			break;
		default:
			return "UNKNOWN";
	}
	return "UNKNOWN";
}
KeyType_t lineak_core_functions::getType(string type) {
        if (type == "SYM")
		return SYM;
        if (type == "CODE")
		return CODE;
        if (type == "BUTTON")
		return BUTTON;
	return UNKNOWN_KEY;
}
unsigned int lineak_core_functions::getModifierNumericValue(const string modstr) {
	string modifiers = modstr;
	unsigned int modifier = 0;
        string::size_type j = 0;
	vector<string> mods;

        // If there is no modifier, return 0	
	if ( modstr == "" || modstr == "default" )
	   return 0;

	// Add a '+' to the end so we have something to parse on in the 
	// following while loop in the event that there is only one modifier.
	if (modifiers[modifiers.size()-1] != '+') {
	   modifiers += '+';
	}
	// Break the string up and put the individual modifiers into a vector 
	// of strings.
	while (modifiers.find('+') != string::npos) {
		j = modifiers.find('+');
		//cout << "adding modifier: " << modifiers.substr(0,j) << " to key:" << keyname << endl;
		mods.push_back(modifiers.substr(0,j));
		modifiers.erase(0,j+1);
	}
	for (vector<string>::iterator it = mods.begin(); it != mods.end(); it++) {
	    
	      /* build the modifier */
	      if (*it == "control" || *it == "Control_L" || *it == "Control_R" )
	         modifier |= ControlMask;
	      else if (*it == "shift" || *it == "Shift_L" || *it == "Shift_R" )
	         modifier |= ShiftMask;
	      else if (*it == "mod1" || *it == "alt" || *it == "Alt_L" || *it == "Alt_R" ) {
	         modifier |= Mod1Mask;
		 //cout << "adding Mod1Mask modifier to " << keyname << endl;
	      }
	      else if (*it == "mod2")
	         modifier |= Mod2Mask;
	      else if (*it == "mod3")
	         modifier |= Mod3Mask;
	      else if (*it == "mod4")
	         modifier |= Mod4Mask;
	      else if (*it == "mod5")
	         modifier |= Mod5Mask;
	      //else if (*it == "release")
	      //   event_type = RELEASE;
	      //else
	      //{
		//type = SYM;
		//keysym = XStringToKeysym (line2);
		//if (keysym == 0)
		//break;
	      //} 
             /* build the modifier */ /*
              if (strcmp_nocase(*it,"control"))
                 modifier |= ControlMask;
              else if (strcmp_nocase(*it,"shift"))
                 modifier |= ShiftMask;
              else if (strcmp_nocase(*it,"mod1") || strcmp_nocase(*it, "alt")) {
                 modifier |= Mod1Mask;
                 //cout << "adding Mod1Mask modifier to " << keyname << endl;
              }
              else if (strcmp_nocase(*it,"mod2"))
                 modifier |= Mod2Mask;
              else if (strcmp_nocase(*it,"mod3"))
                 modifier |= Mod3Mask;
              else if (strcmp_nocase(*it,"mod4"))
                 modifier |= Mod4Mask;
              else if (strcmp_nocase(*it,"mod5"))
                 modifier |= Mod5Mask;
*/
	}
	return modifier;
}

void lineak_core_functions::enable_IBMRA7993(void) {
   lineak_core_functions::send_commands("send_to_keyboard", "ea 71");
}

void lineak_core_functions::send_commands(string command, string args) {
  string comm;
  //comm += SBINDIR;
  //comm += "/";
  comm += command;
  comm += " ";
  comm += args;
  comm += " &";

  if (!fork()) {

        /* child process that tries to run the command, and then exits */
        /* all specials done, let's go for it... ;) */
        if (verbose) cout << "... running " << comm << endl;
        //system("which setkeycodes");
        system(comm.c_str());
        exit(true);
  }

}

