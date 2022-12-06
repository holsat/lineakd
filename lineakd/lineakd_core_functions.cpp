/***************************************************************************
                          lineakd_core_functions.cpp  -  description
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
#include <lineakd_core_functions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lineak_util_functions.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

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
#include <sys/wait.h>
}

#include <lineak/definitions.h>
#include <lineak/configdirectives.h>
#include <lineak/saver.h>
#include <lineak/configloader.h>
#include <lineak/defloader.h>
#include <lineak/ldef.h>
#include <lineak/xmgr.h>
#include <cmdprefs.h>
//#include <lineak/soundctrl.h>
#include <lineak/cdromctrl.h>
#include <lineak/lkbd.h>
#include <lineak/lkey.h>
#include <lineak/msgpasser.h>
#include <lineak/lockctrl.h>
#include <lineak/lcommand.h>
#include <lineak/pluginmanager.h>
//#include <dpluginmanager.h>
#include <lineak/commandexec.h>
#include <lineak/displayctrl.h>

using namespace lineak_util_functions;
using namespace lineak_core_functions;
using namespace std;

extern LKbd myKbd;
extern LConfig myConfig;
extern LDef myDef;
extern Xmgr *X;
extern CmdPrefs cmdprefs;
extern bool verbose;
extern bool very_verbose;
extern bool global_enable;
extern PluginManager *plugins;
extern displayCtrl *myDisplay;
extern ConfigDirectives dnd;

/** Thread for the message queue */
pthread_t m_thread;
sem_t x_sem, exit_sem, hup_sem, doexit_sem;
int mainpid = 0, watchpid = 0;
bool doneexit = false;

/* parse .conf file */
void lineakd_core_functions::parseConfigs(ConfigDirectives & prefs, LConfig & myConfig) {
  if(!parseconffile(prefs, myConfig)) {
	  if (verbose) cout << "Exiting due to failed config file parsing" << endl;
  	cleanexit(false);
  }
}

/* parse keyboard definition file */
void lineakd_core_functions::parseDefinitions(ConfigDirectives & prefs, LDef & def) {
	if(!parsedeffile(prefs,def))
		cleanexit(false);
}

/* show usage information on stdout */
void lineakd_core_functions::showusage(int argc, char *argv[]) {
	cout << "\nLinEAK v" << VERSION << " -- Linux support for Easy Access and Internet Keyboards";
	cout << "\n";
	printf("Usage: %s [options...]\n", argv[0]);
	cout << "Valid options are:\n";
#ifdef HAVE_GETOPT_LONG
	cout << "    -l,        --kbd-list     	    Show all supported keyboard types\n";
	cout << "    -L,        --plugin-list        Show all plugins and macros.\n";
	cout << "    -D,        --directives-list    Show all of the default, implicit configuration directives.\n";
	cout << "    -f FILE,   --conffile=FILE      Specify the .conf file\n";
	cout << "    -e FILE,   --deffile=FILE       Specify the .def file\n";
	cout << "    -d DEVICE, --cdrom-dev=DEVICE   Specify the CDROM device\n";
	cout << "    -m DEVICE, --mixer-dev=DEVICE   Specify the mixer device\n";
	cout << "    -c TYPE,   --config=TYPE        Create a new lineakd.conf file for keyboard TYPE\n";
	cout << "                                      (warning: old one will be overwritten!)\n";
	cout << "    -v,        --verbose            Print verbose messages\n";
	cout << "    -vv,       --very-verbose       Print very verbose messages. Mostly for debugging purposes.\n";
	cout << "    -h,        --help               Print this message\n";
	cout << "    -r,        --reload             Reload the configuration files.\n";
        cout << "    -z         --sleep              Disable handling of keyboard events\n";
        cout << "    -k         --awaken             Enable handling of keyboard events\n";
	cout << "    -x,        --exit               Tell the daemon to exit.\n";
	cout << "    -n,        --display-font         Set the font the on-screen display will use. It must be in X format.\n";
        cout << "                                      i.e. -bitstream-charter-black-r-normal-*-*-240-*-*-p-*-iso8859-1\n";
	cout << "    -o,        --display-color        Set the color of the on-screen display. It must be given as a color code.\n";
        cout << "                                      i.e. #0aff00\n";
	cout << "    -t,        --display-timeout      Set the amount of time that the on-screen display keeps it's text on screen.\n";
	cout << "    -p,        --display-pos          Set the vertical position where osd will display.\n";
        cout << "                                      Valid values are: top, middle, bottom\n";
	cout << "    -a,        --display-align        Set the horizontal alignment for the on-screen display.\n";
        cout << "                                      Valid values are: left, center, right\n";
	cout << "    -y,        --display-hoffset      Set the horizontal offset from the alignment (see above).\n";
	cout << "    -w,        --display-voffset      Set the vertical offset from the position (see osd-pos above).\n";
	cout << "    -s,        --display-soffset      Set the offset for the shadow behind displayed text.\n";
#else
	cout << "    -l            Show all supported keyboard types\n";
	cout << "    -L,           Show all plugins and macros.\n";
	cout << "    -D,           Show all of the default, implicit configuration directives.\n";
	cout << "    -f FILE       Specify the .conf file\n";
	cout << "    -e FILE       Specify the .def file\n";
	cout << "    -d DEVICE     Specify the CDROM device\n";
	cout << "    -m DEVICE     Specify the mixer device\n";
	cout << "    -c TYPE       Create a new lineakd.conf file for TYPE keyboard\n";
	cout << "                    (warning: old one will be overwritten!)\n";
	cout << "    -v,           Print verbose messages\n";
	cout << "    -vv,          Print very verbose messages. Mostly for debugging purposes.\n";
	cout << "    -h            Print this message\n";
	cout << "    -r,           Reload the configuration files.\n";
        cout << "    -z           Disable handling of keyboard events\n";
        cout << "    -k           Enable handling of keyboard events\n";
	cout << "    -x,           Tell the daemon to exit.\n";
	cout << "    -n,           Set the font the on-screen display will use. It must be in X format.\n";
        cout << "                    i.e. -bitstream-charter-black-r-normal-*-*-240-*-*-p-*-iso8859-1\n";
	cout << "    -o,           Set the color of the on-screen display. It must be given as a color code.\n";
        cout << "                    i.e. 0aff00\n";
	cout << "    -t,           Set the amount of time that the on-screen display keeps it's text on screen.\n";
	cout << "    -p,           Set the vertical position where osd will display.\n";
        cout << "                    Valid values are: top, middle, bottom\n";
	cout << "    -a,           Set the horizontal alignment for the on-screen display.\n";
        cout << "                    Valid values are: left, center, right\n";
	cout << "    -y,           Set the horizontal offset from the alignment (see above).\n";
	cout << "    -w,           Set the vertical offset from the position (see osd-pos above).\n";
	cout << "    -s,           Set the offset for the shadow behind displayed text.\n";
#endif
  	cout << "\nNOTE: if '-f', '-e', '-d', or '-m' parameters are used in conjunction with '-c', please\n      make sure you specify those BEFORE '-c' !\n";
        cout << endl;
}

/* these functions provide for a somewhat cleaner exit */
void lineakd_core_functions::cleanexit (int status) {
//cout << "In cleanexit\n";
  	if (X != 0)
    	delete (X);
    	//plugins->unloadAllPlugins();
    	msgPasser myMsg;
    	if (!myMsg.start()) error("cleanexit(): Cannot establish message queue to send an exit message.!");
  	myMsg.quit();
  	lockCtrl lock("lineakd");
  	lock.unlock();
        if (verbose) cout << "Exiting lineakd" << endl;
  	exit(status);
}
void lineakd_core_functions::signalchild(int sig_num) {
  pid_t child;

  /*   If more than one child exits at approximately the same time, the signals */
  /*   may get merged. Handle this case correctly. */
  while ((child = waitpid(-1, NULL, WNOHANG)) > 0)
    {
      if (very_verbose)
        cout << "Catch CHLD signal -> pid " << child << " terminated with signal: " << sig_num << endl;
    }

}
void lineakd_core_functions::signalexit(int sig_num) {
        static bool signaldone = false;
	int pid = getpid();
	sem_wait(&exit_sem);
	if (!signaldone && (pid == mainpid || pid == watchpid)) {
	   signaldone = true;
	   cout.flush();
	   // let all of the other threads continue so that only one sends the EXIT signal.
	   sem_post(&exit_sem);
	   if (very_verbose) cout << "Semaphore posted\n";
  	   if (very_verbose) cout << "Recieved signal " << sig_num << endl;
	   if (very_verbose) cout << "Sending of EXIT message handled by thread: " << pid << endl;
	   cout.flush();
  	   msgPasser myMsg;
  	   if (!myMsg.start()) error("signalexit(): Cannot establish message queue to send an exit message.!");
	   if (very_verbose) cout << "Sending EXIT message!" << endl;
  	   myMsg.sendMessage(msgPasser::EXIT,"exit");
	   signaldone = false;
	   sleep(1);
	} else 
	   sem_post(&exit_sem);

	if (pid == watchpid)
	  watch_messages();

	if (very_verbose) cout << "pid: " << pid << " exiting signalexit()\n";
}
void lineakd_core_functions::signalrestart(int sig_num) {
	msgPasser myMsg;
        stringstream ss;
        ss << "signalrestart(";
        ss << sig_num << "): Cannot establish message queue to send a HUP message.!";
        string e = ss.str();
	if (!myMsg.start()) error(e);
	myMsg.sendMessage(msgPasser::HUP,"hup");
}
/* this is our "clean-up-that-mess" signal handler ;-) */
void lineakd_core_functions::do_exit() {
  sigset_t mask_set; /* our signal masking set. */
  sigset_t old_set;  /* the old mask set.   */
  static bool plugincleanup = false;

  cout.flush();
  if (verbose) cout << endl;
  if (verbose) cout << endl;
  if (very_verbose) cout << "**** do_exit() initiated **** \n";
  cout.flush();
  /** Cancel the message thread */
//  pthread_cancel(m_thread);
//  cout << "pthread_cancelled\n";
  if (very_verbose) cout << "**** sem_wait **** " << getpid() << endl;
  cout.flush();
  /** This is a section that we only want called once. */
  /* Put a semaphore here so that we can set doneexit atomically. */
  sem_wait(&doexit_sem);

  if (!doneexit) {
   	doneexit = true;

   	/* mask (block) any further signals while we're inside this signal handler. */
  	sigfillset(&mask_set);
  	sigprocmask(SIG_SETMASK, &mask_set, &old_set);

   	/* now do some cleaning... */
  	msg("Cleaning up xkb mappings");
   	/* Clean up X */
        if (X != 0) {
  		X->cleanup(myKbd);
                sleep(1);
  		X->cleanup(myKbd);
		sleep(1);
                delete(X);
                X = 0;
        }

        if (!plugincleanup) {
           plugincleanup = true;
           if ( plugins != NULL) {
              msg("Unloading All Plugins!");
              plugins->unloadAllPlugins();
              msg("Unloading All Plugins DONE!");
              delete(plugins);
              plugins = NULL;
              msg("Plugins all taken care of!");
           }
        }

        /* .. restore the old signal mask .. */
  	//sigprocmask(SIG_SETMASK, &old_set, NULL);

        /* Remove the message queue */
  	msg("Removing message queue");
	msgPasser myMsg;
	if (!myMsg.start()) error("do_exit(): Cannot establish message queue to shutdown the queue.!");
        myMsg.quit();

        /** Remove the lock */
        msg("Removing lock");
        lockCtrl lock("lineakd");
        lock.unlock();
//      exit(true);

  }
  /** Allow the other threads to continue */
  sem_post(&doexit_sem);
  //sleep(2);
  /** Destroy the semaphore */
  sem_destroy(&doexit_sem);
  sem_destroy(&hup_sem);
  sem_destroy(&exit_sem);
  sem_destroy(&x_sem);
  exit(true);
}

/* Reload .conf .def files and re-init program */
void lineakd_core_functions::restart() {
  static bool donehup = false;
  sigset_t mask_set; /* our signal masking set. */
  sigset_t old_set;  /* the old mask set.   */

  //sem_wait(&hup_sem);
  if (!donehup) {
		donehup = true;
    	        //sem_post(&x_sem);
		/* mask (block) any further signals while we're inside this signal handler. */
  		sigfillset(&mask_set);
  		sigprocmask(SIG_SETMASK, &mask_set, &old_set);
   		//bool verbose = cmdprefs.getVerbose();

   		msg("Recieved a message to load a new configuration");
                msg("Unmapping keys");
                X->cleanup(myKbd);

                msg("Unloading all plugins");
                plugins->unloadAllPlugins();

                /** Get a list of the potential plugins in the plugin directory */
                vector<string> pluginlist = plugins->scanForPlugins();
                msg("Scaning for and loading plugins");
                /** Load the plugins */
                plugins->loadPlugins(pluginlist);
                /** Define the list of macros we support */
                plugins->defineMacroLists();
                /** Define the configurate directives we support */
                plugins->defineDirectivesLists();
                /** Get a list of macros we support so that we can inform other classes */
                vector<string> macrolist = plugins->getMacroList();

                /** Get a list of directives and their defaults that we support here */
                dnd = plugins->getDirectivesList();

                /** Add to the list of directives and defaults that we support implicitly. */
	        if (!dnd.isSet(_CD_KEYBOARD_TYPE))    dnd.addValue(_CD_KEYBOARD_TYPE, snull);
                if (!dnd.isSet(_CD_USERCONFFILE))     dnd.addValue(_CD_USERCONFFILE, snull);
	        if (!dnd.isSet(_CD_CDROM_DEVICE))     dnd.addValue(_CD_CDROM_DEVICE, DEFAULT_CDROM_DEVICE);
	        if (!dnd.isSet(_CD_MIXER_DEVICE))     dnd.addValue(_CD_MIXER_DEVICE, DEFAULT_MIXER_DEVICE);
	        if (!dnd.isSet("Screensaver"))        dnd.addValue("Screensaver", snull);
                if (!dnd.isSet("Display_plugin"))     dnd.addValue("Display_plugin",DEFAULT_DISPLAY_PLUGIN);
                if (!dnd.isSet("Display_font"))       dnd.addValue("Display_font", DISPLAY_FONT);
	        if (!dnd.isSet("Display_color"))      dnd.addValue("Display_color", DISPLAY_COLOR);
	        if (!dnd.isSet("Display_pos"))        dnd.addValue("Display_pos", DISPLAY_POS);
                if (!dnd.isSet("Display_align"))      dnd.addValue("Display_align", DISPLAY_ALIGN);
                if (!dnd.isSet("Display_timeout"))    dnd.addValue("Display_timeout", DISPLAY_TIMEOUT);
                if (!dnd.isSet("Display_hoffset"))    dnd.addValue("Display_hoffset", DISPLAY_HOFFSET);
                if (!dnd.isSet("Display_voffset"))    dnd.addValue("Display_voffset", DISPLAY_VOFFSET);
                if (!dnd.isSet("Display_soffset"))    dnd.addValue("Display_soffset", DISPLAY_SOFFSET);
                if (!dnd.isSet("keystate_numlock"))   dnd.addValue("keystate_numlock", snull);
                if (!dnd.isSet("keystate_capslock"))  dnd.addValue("keystate_capslock", snull);
                if (!dnd.isSet("keystate_scrolllock")) dnd.addValue("keystate_scrolllock", snull);

                /** Set the macrolist for all LCommand objects */
                LCommand::setMacros(macrolist);

                /** Reconcile the directives and defaults from the plugins, with those from the
                command line. Those on the command line overwrite those specified here. */
                cmdprefs.setDefaults(dnd);

  		/* reload .conf and .def file */
  		msg("*** reloading config file " + myConfig.getFilename());
                cout << "Config file to load is: " << myConfig.getFilename() << endl;
                ConfigLoader ldr(myConfig.getFilename(),dnd);
		myConfig.clear();
                myConfig = ldr.loadConfig();
                myConfig.print(cout);
  		//parseConfigs(dnd, myConfig);
		msg("*** setting config file options " + myConfig.getFilename());
                cmdprefs.setOpts(myConfig);
  		msg("*** reloading definition file " + myDef.getFilename());
  		parseDefinitions(cmdprefs, myDef);

  		/* re-init the EAKeyboard */
  		msg("*** re-initializing the keyboard");
    	        myKbd = myDef.getKeyboard(myConfig[_CD_KEYBOARD_TYPE]);
		/** Update our keyboard with our configured commands */
		myKbd.setCommands(myConfig);

  		/* re-init X/Xkb with our EAK keycodes/keysyms */
  		msg("*** reconfiguring Xkb keymap with new keys and buttons");
		X->initialize(myKbd);

  		/* .. restore the old signal mask .. */
  		msg("*** rehash done");
  		sigprocmask(SIG_SETMASK, &old_set, NULL);
		plugins->initializePlugins(myKbd, myConfig,plugins);

  		msg("*** Restarting On Screen Display...");
		myDisplay = plugins->getDisplay(myConfig);
  		//myDisplay->init(myConfig);
		msg("*** InitializePluginsDisplay...");
                plugins->initializePluginsDisplay(*myDisplay);
		// Set the display controller.
		CommandExec::setDisplayCtrl(myDisplay);

    	donehup=false;
  }

  	return;
}

/**********************************************************************************************/

void lineakd_core_functions::watch_messages() {
  /** Create a new thread */
  int res;
  pthread_attr_t thread_attr;
  res = pthread_attr_init(&thread_attr);
  // Initialise the semaphores here
  res = sem_init(&x_sem, 0,1);
  mainpid = getpid();
  if (very_verbose) cout << "Main thread is: " << mainpid << endl;
  if (sem_init(&exit_sem,0,1) != 0)
     perror("exit_sem: Semaphore initialization failed");
  if (sem_init(&doexit_sem,0,1) != 0)
     perror("doexit_sem: Semaphore initialization failed");
  if (sem_init(&hup_sem,0,1) != 0)
     perror("hup_sem: Semaphore initialization failed");
  if (res != 0) {
   	perror("Semaphore initialization failed");
   	cleanexit(false);
  }
//	if (res != 0) {
//		perror("Attribute creation failed for message handling thread");
//		cleanexit(false);
//	}
	res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if (res != 0) {
		perror("Setting detached attribute failed for message handling thread");
		cleanexit(false);
	}
	res = pthread_create(&m_thread, NULL, handle_messages, NULL);
	if (res != 0) {
		perror("Thread creation failed for message handling");
		cleanexit(false);
	}
	(void)pthread_attr_destroy(&thread_attr);
}

void *lineakd_core_functions::handle_messages(void *arg) {
	/** Handle messages */
	msgPasser myMsg;
	eak_msg msg;

	if (!myMsg.start()) {
		error("Failed to setup the message queue in the message handling thread.!");
		fatal("Trying to exit now!");
		cleanexit(false);
	}
		
        cout.flush();
	watchpid = getpid();
	if (very_verbose) cout << "**** Message handler thread is: " << watchpid << endl;
        int res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	if (res != 0) {
		perror("Thread cancel state failed");
		exit(false);
	}
	res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

	if (res != 0) {
		perror("Thread cancel type failed");
		exit(false);
	}

	while (true) {
  	        //lineak_core_functions::msg("Waiting for message");
     	        msg = myMsg.getMessage();
                if (msg.msg_type == msgPasser::EXIT) {
			lineak_core_functions::msg("Exit message");
                        do_exit();
      	        }
      	        if(msg.msg_type == msgPasser::HUP) {
			lineak_core_functions::msg("Restart message");
                        restart();
                }
      	        if(msg.msg_type == msgPasser::TERM) {
       		        pthread_exit(NULL);
                }
                if(msg.msg_type == msgPasser::DISABLE) {
			lineak_core_functions::msg("Disable message");
                        global_enable = false;
                }
                if(msg.msg_type == msgPasser::ENABLE) {
			lineak_core_functions::msg("Enable message");
                        global_enable = true;
                }
		// parse the possible command types.
		// Some of these don't work, and will not until all of the objects can be serialized
		// and transmitted over IPC.
		if (msg.msg_type == msgPasser::COMMAND) {
		        //LObject *key;
			//string *scomm;
			//char * ccomm;
			//LCommand *lcomm;
			lineak_core_functions::msg("Run external command");
			switch (msg.directive.data_type) {
				// empty command
				case NODATA:
					lineak_core_functions::msg("Empty command");
					break;
				// command passed as an LObject
				case LOBJECT:
					lineak_core_functions::msg("LObject command");
					//key = static_cast<LObject*>(msg.directive.data);
					//lineak_core_functions::msg((key->getCommand()).getCommand());
					//delete(key);
					break;
				case LOBJECT_BYNAME:
					lineak_core_functions::msg("LObject command");
                                        
					//scomm = static_cast<string*>(msg.directive.data);
					//lineak_core_functions::msg("LObject name: " + *scomm);
                                        //delete(scomm);
                                        break;
				// command passed as a string
				case STRING_COMMAND:
					lineak_core_functions::msg("String command");
					//scomm = static_cast<string*>(msg.directive.data);
					//lineak_core_functions::msg(*scomm);
					cout << msg.directive.args << endl;
					//delete(scomm);
					break;
				case CHAR_COMMAND:
					lineak_core_functions::msg("char command");
					//ccomm = static_cast<char*>(msg.directive.data);
					//lineak_core_functions::msg(*((const char*)ccomm));
					//if (verbose) cout << ccomm << endl;
					//delete(ccomm);
					break;
				case LCOMMAND:
					lineak_core_functions::msg("LCommand command");
					//lcomm = static_cast<LCommand*>(msg.directive.data);
					//lineak_core_functions::msg(lcomm->getCommand());
					//delete(lcomm);
					break;
				default:
					lineak_core_functions::msg("Unknown data_type");
			}
		}
		/* This concept will work when we can [un]serialize these classes and send over IPC. */
		if (msg.msg_type == msgPasser::GETDATA) {
		        lineak_core_functions::msg("Return data");
                        cout << "GETDATA" << endl;
			switch (msg.directive.data_type) {
				case NODATA:
					break;
				case LOBJECT_BYNAME:
					{
					// Get the name of the object requested.
					string name = msg.directive.args;
                                        string data, d;
                                        stringstream ss (data, stringstream::in | stringstream::out);
					LObject *obj = myKbd.getObject(name);
                                        ss << *obj;
                                        d = ss.str();
					eak_data retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = LOBJECT;
					strcpy(retmsg.directive.args, d.c_str());
           cout << "Sending a key back." << endl;
           cout << "Key string: " << d << endl;
					myMsg.sendMessage(retmsg); }
					break; 
				case LCONFIG: 
					{
					// Get the name of the object requested.
					//string name = msg.directive.args;
					//LConfig *obj = &myConfig;
					eak_msg retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = LCONFIG;
					//retmsg.directive.data = static_cast<void*>(obj);
					//strcpy(retmsg.directive.args, name.c_str());
					myMsg.sendMessage(retmsg); }
					break;
				case LDEF:
					{
					// Get the name of the object requested.
					//string name = msg.directive.args;
					//LDef *obj = &myDef;
					eak_msg retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = LDEF;
					//retmsg.directive.data = static_cast<void*>(obj);
					//strcpy(retmsg.directive.args, name.c_str());
					myMsg.sendMessage(retmsg); }
					break;
				case LKBD:
					{
					// Get the name of the object requested.
					//string name = msg.directive.args;
					//LKbd *obj = &myKbd;
					eak_msg retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = LKBD;
					//retmsg.directive.data = static_cast<void*>(obj);
					//strcpy(retmsg.directive.args, name.c_str());
					myMsg.sendMessage(retmsg); }
					break;
				case PLUGIN_MANAGER:
					{
					// Get the name of the object requested.
					//string name = msg.directive.args;
					//PluginManager *obj = plugins;
					eak_msg retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = PLUGIN_MANAGER;
					//retmsg.directive.data = static_cast<void*>(obj);
					//strcpy(retmsg.directive.args, name.c_str());
					myMsg.sendMessage(retmsg); }
					break;
				case LCOMMAND:
					{
					// Get the name of the object requested.
					string name = msg.directive.args;
					//LCommand *obj = &(myKbd.getObject(name)->getCommand());
					eak_msg retmsg;
					retmsg.msg_type = msgPasser::SENDDATA;
					retmsg.directive.data_type = LCOMMAND;
					//retmsg.directive.data = static_cast<void*>(obj);
					//strcpy(retmsg.directive.args, name.c_str());
					myMsg.sendMessage(retmsg); 
					}
					break;
				default:
					break;
			}
		} /*  */

                msg.msg_type = 0;
                strcpy(msg.directive.args,"");
                sleep(1);
	}
  pthread_exit(NULL);
}
void lineakd_core_functions::print_event(int event) {
   switch(event) {
     case KeyPress:
         cout << "KeyPress: Event" << endl;
         break;
     case KeyRelease:
         cout << "KeyRelease: Event" << endl;
         break;
     case ButtonPress:
         cout << "ButtonPress: Event" << endl;
         break;
     case ButtonRelease:
         cout << "ButtonRelease: Event" << endl;
         break;
     case MotionNotify:
         cout << "MotionNotify: Event" << endl;
         break;
     case EnterNotify:
         cout << "EnterNotify: Event" << endl;
         break;
     case LeaveNotify:
         cout << "LeaveNotify: Event" << endl;
         break;
     case FocusIn:
         cout << "FocusIn: Event" << endl;
         break;
     case FocusOut:
         cout << "FocusOut: Event" << endl;
         break;
     case KeymapNotify:
         cout << "KeymapNotify: Event" << endl;
         break;
     case Expose:
         cout << "Expose: Event" << endl;
         break;
     case GraphicsExpose:
         cout << "GraphicsExpose: Event" << endl;
         break;
     case NoExpose:
         cout << "NoExpose: Event" << endl;
         break;
     case VisibilityNotify:
         cout << "VisibilityNotify: Event" << endl;
         break;
     case CreateNotify:
         cout << "CreateNotify: Event" << endl;
         break;
     case DestroyNotify:
         cout << "DestroyNotify: Event" << endl;
         break;
     case UnmapNotify:
         cout << "UnmapNotify: Event" << endl;
         break;
     case MapNotify:
         cout << "MapNotify: Event" << endl;
         break;
     case MapRequest:
         cout << "MapRequest: Event" << endl;
         break;
     case ReparentNotify:
         cout << "ReparentNotify: Event" << endl;
         break;
     case ConfigureNotify:
         cout << "ConfigureNotify: Event" << endl;
         break;
     case ConfigureRequest:
         cout << "ConfigureRequest: Event" << endl;
         break;
     case GravityNotify:
         cout << "GravityNotify: Event" << endl;
         break;
     case ResizeRequest:
         cout << "ResizeRequest: Event" << endl;
         break;
     case CirculateNotify:
         cout << "CirculateNotify: Event" << endl;
         break;
     case CirculateRequest:
         cout << "CirculateRequest: Event" << endl;
         break;
     case PropertyNotify:
         cout << "PropertyNotify: Event" << endl;
         break;
     case SelectionClear:
         cout << "SelectionClear: Event" << endl;
         break;
     case SelectionRequest:
         cout << "SelectionRequest: Event" << endl;
         break;
     case SelectionNotify:
         cout << "SelectionNotify: Event" << endl;
         break;
     case ColormapNotify:
         cout << "ColormapNotify: Event" << endl;
         break;
     case ClientMessage:
         cout << "ClientMessage: Event" << endl;
         break;
     case MappingNotify:
         cout << "MappingNotify: Event" << endl;
         break;
     case LASTEvent:
         cout << "LASTEvent: Event" << endl;
         break;
     default:
         cout << "Unknown: Event" << endl;
         break;
   }
}
void lineakd_core_functions::initialize_raw_keyboards(LConfig & config, LDef & def) {

   LKbd & kb = def.getKeyboard(config[_CD_KEYBOARD_TYPE]);
   string coms = kb.getRawCommand("INITIALIZE");

   if ( (! kb.isEmpty()) && coms.length() > 0 ) {
      send_commands("send_to_keyboard", coms);
   }

}
void lineakd_core_functions::initialize_setkeycodes(LConfig & config, LDef & def) {

   LKbd & kb = def.getKeyboard(config[_CD_KEYBOARD_TYPE]);
   string coms = kb.getRawCommand("SETKEYCODES");

   if ( (! kb.isEmpty()) && coms.length() > 0 ) {
      send_commands("setkeycodes", coms);
   }

}
