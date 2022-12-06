/***************************************************************************
                          cmdprefs.cpp  -  description
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

#include <cmdprefs.h>
#include <lineak/definitions.h>
#include <lineak/lconfig.h>
#include <lineak/configdirectives.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/msgpasser.h>
#include <unistd.h>

extern "C" {
 #ifdef HAVE_GETOPT_H
 #  define HAVE_GETOPT_LONG
 #  include <getopt.h>
 #endif
}

CmdPrefs::CmdPrefs(){
	showusage = false;
	createnewconfig = false;
	showplugins = false;
	showkeyboards = false;
	showdefinitions = false;
}
CmdPrefs::~CmdPrefs(){
}
/** Read property of bool verbose. */
const bool& CmdPrefs::getVerbose(){
	return verbose;
}
/** Read property of bool verbose. */
const bool& CmdPrefs::getVeryVerbose(){
        return very_verbose;
}
bool CmdPrefs::showUsage() {
	return showusage;
}
bool CmdPrefs::createNewConfig() {
	return createnewconfig;
}
bool CmdPrefs::showKeyboards() {
	return showkeyboards;
}
bool CmdPrefs::showPlugins() {
	return showplugins;
}
bool CmdPrefs::showDefinitions() {
	return showdefinitions;
}
/** Set the global options in the keyboard config.
      Command line options override config file options.
        */
void CmdPrefs::setOpts( LConfig & conf){
	if (isSet(_CD_CDROM_DEVICE))
		conf[_CD_CDROM_DEVICE] = getValue(_CD_CDROM_DEVICE);
	if (isSet(_CD_KEYBOARD_TYPE))
		conf[_CD_KEYBOARD_TYPE] = getValue(_CD_KEYBOARD_TYPE);
	if (isSet(_CD_MIXER_DEVICE))
		conf[_CD_MIXER_DEVICE] = getValue(_CD_MIXER_DEVICE);

  if (isSet("Display_font"))
    conf["Display_font"] = getValue("Display_font");
  if (isSet("Display_color"))
    conf["Display_color"] = getValue("Display_color");
  if (isSet("Display_timeout"))
    conf.setValue("Display_timeout", getValue("Display_timeout"));
  // Must use a flag to see if it was set b/c the types are enums
  if (isSet("Display_pos"))
    conf.setValue("Display_pos", getValue("Display_pos"));
  if (isSet("Display_align"))
    conf.setValue("Display_align", getValue("Display_align"));

  if (isSet("Display_hoffset"))
    conf.setValue("Display_hoffset", getValue("Display_hoffset"));
  if (isSet("Display_voffset"))
    conf.setValue("Display_voffset" , getValue("Display_voffset"));
  if (isSet("Display_soffset"))
    conf.setValue("Display_soffset" , getValue("Display_soffset"));
}
/** Set the global options in the keyboard config.
      Command line options override config file options. */
void CmdPrefs::setDefaults( ConfigDirectives & conf){

	for (map<string,string>::iterator it = directives.begin(); it!= directives.end(); it++) {
		conf.addValue((it->first),(it->second));
	}
	for (map<string,int>::iterator it = si_directives.begin(); it!= si_directives.end(); it++) {
		conf.addValue((it->first),(it->second));
	}
/*
	if (isSet("CdromDevice"))
		conf["CdromDevice"] = getValue("CdromDevice");
	if (isSet("KeyboardType"))
		conf["KeyboardType"] = getValue("KeyboardType");
	if (isSet("MixerDevice"))
		conf["MixerDevice"] = getValue("MixerDevice");

  if (isSet("Display_font"))
    conf["Display_font"] = getValue("Display_font");
  if (isSet("Display_color"))
    conf["Display_color"] = getValue("Display_color");
  if (isSet("Display_timeout"))
    conf.setValue("Display_timeout", getValue("Display_timeout"));
  //Must use a flag to see if it was set b/c the types are enums
  if (isSet("Display_pos"))
    conf.setValue("Display_pos", getValue("Display_pos"));
  if (isSet("Display_align"))
    conf.setValue("Display_align", getValue("Display_align"));

  if (isSet("Display_hoffset"))
    conf.setValue("Display_hoffset", getValue("Display_hoffset"));
  if (isSet("Display_voffset"))
    conf.setValue("Display_voffset" , getValue("Display_voffset"));
  if (isSet("Display_soffset"))
    conf.setValue("Display_soffset" , getValue("Display_soffset"));
    */
}

bool CmdPrefs::parseArgs(int argc, char *argv[]) {

  using namespace lineak_core_functions;
  int myopt;
  bool newconf=false;
  string position;
  string align;
  const char *flags = "hDd:m:c:bv::rzkxf:e:u:lLn:o:t:p:a:y:w:s:";

#ifdef HAVE_GETOPT_LONG
	int option_index = 0;
	static struct option long_options[] = {
      {"help",             0, 0, 'h'},
      {"cdrom-dev",    1, 0, 'd'},
      {"mixer-dev",     1, 0, 'm'},
      {"create",            1, 0, 'c'},
      {"verbose",         0, 0, 'v'},
      {"kbd-list",         0, 0, 'l'},
      {"plugin-list",         0, 0, 'L'},
      {"directives-list",     0, 0, 'D'},
      {"conffile",          1, 0, 'f'},
      {"deffile",            1, 0, 'e'},
      {"userdeffile",     1,0,'u'},
      //{"veryverbose",          0,0, 'V'},
      {"reload",            0,0, 'r'},
      {"sleep",            0,0, 'z'},
      {"awaken",            0,0, 'k'},
      {"exit",                0,0, 'x'},
      {"display-font",        1, 0, 'n'},
      {"display-color",       1, 0, 'o'},
      {"display-timeout",  1, 0, 't'},
      {"display-pos",         1, 0, 'p'},
      {"display-align",      1, 0, 'a'},
      {"display-hoffset",   1, 0, 'y'},
      {"display-voffset",   1, 0, 'w'},
      {"display-soffset",    1, 0, 's'},
      {0, 0, 0, 0}
	};
	while ((myopt = getopt_long(argc, argv, flags, long_options, &option_index)) != -1) {
#else
 	while ((myopt = getopt(argc, argv, flags)) != -1) {
#endif
	/* parse each option */
  switch (myopt) {
  	case 'd':
		  if (newconf) { // -d parameter has to appear BEFORE -c
				cerr << "*** -d parameter ignored, must be specified BEFORE -c" << endl;
				break;
		  }
	    addValue(_CD_CDROM_DEVICE, optarg);
      break;
    case 'm':
		  if (newconf) { // -m parameter has to appear BEFORE -c
				cerr << "*** -m parameter ignored, must be specified BEFORE -c" << endl;
				break;
		  }
		  addValue(_CD_MIXER_DEVICE, optarg);
      break;
    case 'e':
		  if (newconf) { // -e parameter has to appear BEFORE -c
				cerr << "*** -e parameter ignored, must be specified BEFORE -c" << endl;
				break;
		  }
		  addValue(_CD_SYSDEFFILE, optarg);
      break;
    case 'u':
		  if (newconf) { // -e parameter has to appear BEFORE -c
				cerr << "*** -u parameter ignored, must be specified BEFORE -c" << endl;
				break;
		  }
		  addValue(_CD_USERDEFFILE, optarg);
      break;
    case 'f':
		  if (newconf) { // -f parameter has to appear BEFORE -c
				cerr << "*** -f parameter ignored, must be specified BEFORE -c" << endl;;
				break;
		  }
		  addValue(_CD_USERCONFFILE, optarg);
      break;
   	case 'h':
    	showusage = true;
      break;
   	case 'c':
      cout << endl << "NOTE: if '-f', '-e','u', '-d', or '-m' parameters are used in conjunction with '-c', please\n      make sure you specify those BEFORE '-c' !\n" << endl;
   		addValue(_CD_KEYBOARD_TYPE, optarg);
      createnewconfig = true;
     	newconf = true;
      break;
   	case 'v':
           verbose = true;
           very_verbose = false;
           if (optarg != NULL) {
              if (string(optarg) == "v" ) {
                 very_verbose = true;
                 //cout << "We are being very verbose!" << endl;
              } 
           }
          
      break;
    case 'r':
     	if (lineak_core_functions::is_running("lineakd")) {
      	msgPasser message;
        if (verbose)
       		cout << "Sending restart message\n";
        message.start();
        message.sendMessage(msgPasser::HUP,"hup");
      }
      exit(true);
      break;
    case 'z':
     	if (lineak_core_functions::is_running("lineakd")) {
      	msgPasser message;
        if (verbose)
       		cout << "Disabling the global status\n";
        message.start();
        message.sendMessage(msgPasser::DISABLE,"disable");
      }
      exit(true);
      break;
    case 'k':
     	if (lineak_core_functions::is_running("lineakd")) {
      	msgPasser message;
        if (verbose)
       		cout << "Enabling the global status\n";
        message.start();
        message.sendMessage(msgPasser::ENABLE,"enable");
      }
      exit(true);
      break;
    case 'x':
      if (lineak_core_functions::is_running("lineakd")) {
      	msgPasser message;
        if (verbose)
        	cout << "Sending exit message\n";
        message.start();
        message.sendMessage(msgPasser::EXIT,"exit");
      }
      exit(true);
      break;
    case 'l':
    	showkeyboards = true;
      break;
    case 'L':
        showplugins = true;
      break;
    case 'D':
        showdefinitions = true;
      break;
    case 'n':
      addValue("Display_font", optarg);
      break;
    case 'o':
      addValue("Display_color", optarg);
      break;
    case 't':
      addValue("Display_timeout", optarg);
      break;
    case 'p':
      addValue("Display_pos", optarg);
      break;
    case 'a':
      addValue("Display_align", optarg);
      break;
   	case 'y':
    	addValue("Display_hoffset", (optarg));
     	break;
   	case 'w':
     	addValue("Display_voffset", (optarg));
     	break;
   	case 's':
    	addValue("Display_soffset", (optarg));
     	break;
   	default:
   		cout << "Incorrect arguments" << endl;
   		exit(false);
    	break;
  	}
  }

  /* check for a single additional argument */
  if ((argc - optind) > 1) {
    cerr <<  argv[0] << ": too many arguments" << endl;
	return false;
  }

  return true;
}

