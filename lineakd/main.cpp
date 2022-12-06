/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Jan  1 14:03:54 EST 2003
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>

#if __GNUC__ >= 3
   #include <ostream>
   #include <iostream>
   #include <sstream>
#else
   	#include <ostream.h>
#endif

#if defined (__FreeBSD__)
 #define SIGCLD SIGCHLD
#else
 #define SIGCLD SIGCHLD
#endif
// lineakd.c definitions, not sure if they are necessary here
/* system header files */

extern "C" {
#include <signal.h>
#include <X11/Xlib.h>
//#include <X11/Xlibint.h>
#include <X11/Xutil.h>
//#include <X11/Xatom.h>
//#include <X11/Intrinsic.h>
//#include <X11/Xos.h>
//#include <X11/Xmu/Error.h>
//#include <X11/Xmu/Xmu.h>
}

#ifndef X_NOT_STDC_ENV
  #include <stdlib.h>
#else
  extern char *getenv();
#endif

#include <lineak/displayctrl.h>
displayCtrl *myDisplay = 0;

#include <lineakd_core_functions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/configloader.h>
#include <lineak/defloader.h>
#include <lineak/lkbd.h>
#include <lineak/lkey.h>
#include <lineak/lbutton.h>
#include <lineak/ldef.h>
#include <lineak/saver.h>
#include <lineak/xmgr.h>
#include <cmdprefs.h>
//#include <lineak/soundctrl.h>
#include <lineak/cdromctrl.h>
#include <lineak/msgpasser.h>
#include <lineak/lockctrl.h>
#include <lineak/pluginmanager.h>
#include <lineak/commandexec.h>
#include <lineak/plugin_definitions.h>

using namespace lineak_util_functions;
using namespace lineak_core_functions;
using namespace lineakd_core_functions;
using namespace std;

/** Lineakd global objects */
LKbd myKbd(snull,snull,snull);
LConfig myConfig;
LDef myDef;
PluginManager *plugins = NULL;
Xmgr *X = 0;
CmdPrefs cmdprefs;
vector<string> macrolist;
ConfigDirectives dnd;
extern bool verbose;
extern bool very_verbose;
extern bool global_enable;

int main(int argc, char *argv[])
{
  /** Make sure that the ~/.lineak directory exists */
  string homedir = getenv("HOME");

  /** We should be able to do this stuff w/o having an X connection */
  /* parse command line arguments */
  if ( !cmdprefs.parseArgs(argc,argv) )
       exit(false);

  verbose = cmdprefs.getVerbose();
  very_verbose = cmdprefs.getVeryVerbose();

  if (verbose) cout << "Lineakd V" << VERSION << endl;
  if (very_verbose) cout << "Lineakd running in very verbose mode.!" << endl;

  if (cmdprefs.showUsage()) {
       showusage(argc, argv);
       exit(true);
  }
  if (cmdprefs.showKeyboards()) {
        if (!parsedeffile(cmdprefs, myDef))
             exit(false);
        showkeyboards(myDef);
        exit(true);
  }
  /** Get a list of the potential plugins in the plugin directory */
  plugins = new PluginManager;
  vector<string> pluginlist = plugins->scanForPlugins();

  /** Load the plugins */
  plugins->loadPlugins(pluginlist);
  if (verbose) {
	  msg("");
	  msg("Listing Plugins");
	  plugins->listPlugins();
  }

  msg("");
  msg("Defining Macro Lists");
  /** Define the list of macros we support */
  plugins->defineMacroLists();
  //if (verbose) plugins->listPlugins();
  /** Define the configurate directives we support */
  msg("");
  msg("Defining Directives Lists");
  plugins->defineDirectivesLists();
  //if (verbose) plugins->listPlugins();
   /** Get a list of macros we support so that we can inform other classes */
  macrolist = plugins->getMacroList();
  //if (verbose) plugins->listPlugins();

  /* If all we have to do is look for plugins and macros */
  if (cmdprefs.showPlugins()) {
          if (!parsedeffile(cmdprefs, myDef))
               exit(false);
	  plugins->print();
	  exit(true);
  }
  
  /** Get a list of directives and their defaults that we support here */
  dnd = plugins->getDirectivesList();

  /** Add to the list of directives and defaults that we support implicitly. */
   if (!dnd.isSet(_CD_KEYBOARD_TYPE))    dnd.addValue(_CD_KEYBOARD_TYPE, snull);
   if (!dnd.isSet(_CD_USERCONFFILE))     dnd.addValue(_CD_USERCONFFILE, snull);
   if (!dnd.isSet(_CD_CDROM_DEVICE))     dnd.addValue(_CD_CDROM_DEVICE, DEFAULT_CDROM_DEVICE);
   if (!dnd.isSet(_CD_MIXER_DEVICE))     dnd.addValue(_CD_MIXER_DEVICE, DEFAULT_MIXER_DEVICE);
   if (!dnd.isSet(_CD_RAW_COMMANDS))     dnd.addValue(_CD_RAW_COMMANDS, snull);
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
 
   if (cmdprefs.showDefinitions()) {
      cout << dnd;
      exit(true); 
   }
   ///** Add the SYM keyword to the macro list. SYM will allow us to run a command when a known keysym is pressed. */
   //macrolist.push_back("SYM");
   /** Set the macrolist for all LCommand objects */
   LCommand::setMacros(macrolist);

   /** Reconcile the directives and defaults from the plugins, with those from the
        command line. Those on the command line overwrite those specified here. */
  cmdprefs.setDefaults(dnd);

  if(cmdprefs.createNewConfig()) {
	/* read the configuration and keyboard definition file */
	if (!parsedeffile(cmdprefs, myDef))
		exit(false);
	create_new_conf(dnd, myDef);
	exit(true);
  }

// cout << "Checking locks" << endl;
// FIXME: is_running() is borked for multithreading.
  /** Make sure that we aren't already running. By default, isLocked() now calls isRunning() */
  // Right now this is broken b/c we are multithreaded. We need to fix this.
  /*
  lockCtrl lock("lineakd");
  if (lock.isLocked()) {
    	cerr << "Failed to start, lineakd is already running." << endl;
    	exit(false);
  }
  else
  	lock.lock();
  */

  /**************************************************************************************************
      WARINING!!! End of Displayless functions. A reference to the display required for
  		  proper functioning from this point on. 

   **************************************************************************************************/

  if (!X) {
     char *c_str = getenv("DISPLAY");
     if (c_str != NULL) {
	string dis = string(c_str);
        X = new Xmgr(dis);
        if ((!X->initialized()) || (X->getDisplay() == NULL)) {
           cerr << "Could not open display: " << getenv("DISPLAY") << endl;
  	   cleanexit(false);
        }
     }
     else {
        cerr << "FATAL ERROR: $DISPLAY is not defined. Could not open a display." << endl;
	cleanexit(false);
     }  
  }

  /**************************************************************************************************
      WARNING!!! From here on in, you should be using cleanexit() if you need to exit without
  		 concern for threading issues.

  		 cleanexit will take care of deleting the X and myDisplay objects, as well as removing
    		 the message queue and unlocking.

      WARNING!!! Once watch_messages is called, we should be calling do_exit() instead, to deal with
     		 threading issues.

    *************************************************************************************************/

  /** Create the users .lineak directory */
  create_homedir();
  /* everything went according to plan thisfar... some signal-handlers for cleaner exiting */
  signal(SIGTERM, signalexit);
  signal(SIGABRT, signalexit);
  signal(SIGINT, signalexit);
  /* and one so we won't have to wait() for child processes ;) */
  //signal(SIGCLD,SIG_IGN);
  signal(SIGCLD,signalchild);
  /* and for a rehash when we catch SIGHUP */
  signal(SIGHUP,signalrestart);
  /* read the configuration and keyboard definition file */
  parseConfigs(dnd, myConfig);
  /** Update the config object with any command line options. We do this again b/c the command line args trump
   *  the config file options. */
  cmdprefs.setOpts(myConfig);
  parseDefinitions(cmdprefs, myDef);
  /* build our EAKeyboard with the data from deffile and conffile */
  /** get the keyboard from our config file */
  myKbd = myDef.getKeyboard(myConfig[_CD_KEYBOARD_TYPE]);
  /** Attempt to initialize our plugins */
  plugins->initializePlugins(myKbd, myConfig, plugins);

  if (myKbd.isEmpty()) {
   	cerr << "Could not load a keyboard definition for the configured keyboard " << 	myConfig[_CD_KEYBOARD_TYPE] << endl;
   	cleanexit(false);
  }
  /** Update our keyboard with our configured commands */
  myKbd.setCommands(myConfig);

  /** Send appropriate control codes to the keyboard. */
  initialize_raw_keyboards(myConfig,myDef);
  initialize_setkeycodes(myConfig,myDef);

  /* 
   if (very_verbose) {
    cout << myKbd;
    for (map<string,LObject*>::const_iterator i = myKbd.getObjects().begin(); i != myKbd.getObjects().end(); i++) {
      LObject *m = const_cast<LObject*>(i->second);
      if (m->getType() == CODE || m->getType() == SYM) {
         LKey *key = static_cast<LKey*>(m);
	 cout << *key;
      } else if ( m->getType() == BUTTON ) {
      	 LButton *btn = static_cast<LButton*>(m);
	 cout << *btn;
      }
      else 
         cout << *m;
    }
   }
   */
  /** Start the message queue */
  msgPasser messages;
  if (!messages.start()) {
	  error("Could not establish IPC via the message queue.");
	  fatal("Please resolve the problem and retry");
	  cleanexit(false);
  } 
  /** Setup the message watching thread. This thread responds to stop and restart messages.
      From here on out, we should use do_exit() to quit the program */
  watch_messages();

  /* init X/Xkb with our EAK keycodes/keysyms */
  msg("Initializing the display");

  if (!X->initialize(myKbd)) {
     cerr << "Could not initialize all keys. Retrying.\n";
     if (!(X->initialize(myKbd))) {
     	cerr << "Retry of keyboard initialization failed. Exiting.\n";
  	do_exit();
     }
  } 

  Display* display;
  display = X->getDisplay();

  /* initialize OSD system */
  myDisplay = plugins->getDisplay(myConfig);

  if (myDisplay != NULL) {
        myDisplay->init();
  } else {
        myDisplay = new displayCtrl(myConfig);
        myDisplay->init();
  }
  msg("Initializing Plugin Display");
  plugins->initializePluginsDisplay(*myDisplay);

  /* alright, we're going to loop forever now. only signals can interrupt us. */
  CommandExec exec;
  exec_t execute = NULL;
  XEvent xev;
  //XKeyEvent *e;
  unsigned int numlock_mask = X->getNumlockMask();
  unsigned int scrolllock_mask = X->getScrolllockMask();
  unsigned int capslock_mask = X->getCapslockMask();
  /** Set the display controller */
  CommandExec::setDisplayCtrl(myDisplay);

  while(true) {
     //if (very_verbose) cout << "Waiting for next event." << endl;
     vmsg("Waiting for next event.");
     /* wait for next event */
     XNextEvent(display, &xev);
     //if (very_verbose) cout << "Got an event." << endl;
     vmsg("Got an event.");
     /* Should we age the display? */
     if (myDisplay != NULL) {
        if ((xev.type != MappingNotify) && myDisplay->enabled()) {
            //cout << "agenscroll()" << endl;
            myDisplay->agenscroll();
        }
     }
     // If something is taking the keyboard, or it is being remapped.
     // Relinquish our hold on the keyboard mappings.
     // And shutdown the on screen display.
     // This is something needed to make it play nicely with things
     // like VMWare.
     if (xev.type == MappingNotify) {
           msg("Setting xkbRemapped flag");
           X->xkbRemapped(true); // =1;
           msg("Cleaning up key mappings from MappingNotify event");
           X->cleanup(myKbd);
	   //msg("Cleaning up the on screen display.");
	   //myDisplay->cleanup();
           // If we are getting the display back.
           // Get out modifiers and grab our keys again.
           // Reinitialize the OSD.
           msg("Clearing xkbRemapped flag");
           X->xkbRemapped(false);
           //X->getModifiers();
           msg("Reclaiming keyboard map from MappingNotify event");
           X->initialize(myKbd);
	   //msg("Restarting the OSD.");
	   //myDisplay = plugins->getDisplay(myConfig);
	   //plugins->initDisplay(myConfig);
	   //if (myDisplay != NULL) {
	   //    vmsg("Reiniting plugin provided On Screen Display");
	       // Doesn't work. It initialized the default plugin.  
	   //    myDisplay->init();
	   //} else {
	   //    vmsg("Initializing internal display.");
	   //    myDisplay = new displayCtrl(myConfig);
	   //    myDisplay->init();
	   //}
	   // Initialize the plugin displays again.
	   msg("Initializing Plugin Display");
	   plugins->initializePluginsDisplay(*myDisplay);
	   exec.setDisplayCtrl(myDisplay);
     }
     vmsg("Doing something with it!");
     if (global_enable) {
	vmsg("Global Enabled");
        switch (xev.type)
	{
	   case KeyPress:
	   {
	         
	         xev.xkey.state &= ~(numlock_mask | capslock_mask | scrolllock_mask);
	         LKey *obj = static_cast<LKey*>(myKbd.getObject((int)xev.xkey.keycode));

		 if (very_verbose) {
		    cout << "Key press !" << endl;
		    cout << "xev.xkey.keycode=" << xev.xkey.keycode << endl;
		    cout << "xev.xkey.state=" << xev.xkey.state << endl;
		    cout << "has modifier=" << obj->hasModifier(xev.xkey.state) << endl;
		 } 

	         if (obj != NULL) {
		    vmsg("Got an object!");
                    msg("Looking for SYM and PRESS");
	            if (obj->getType() == SYM && obj->getEventType() == PRESS) {
		      if (verbose) {
		         cout << "xev.xkey.keycode = " << xev.xkey.keycode << endl;
		         cout << "obj->getKeyCode() = " << obj->getKeyCode() << endl;
		         cout << "XKeysymToKeycode (display, (KeySym)obj->getKeySym() = " << XKeysymToKeycode (display, (KeySym)obj->getKeySym()) << endl;
		      }
		       if (xev.xkey.keycode == XKeysymToKeycode (display, (KeySym)obj->getKeySym())
		           && (obj->hasModifier(xev.xkey.state) || obj->isUsedAsToggle()))
		       {
		          execute = plugins->exec(obj, xev);
	                  if (execute != NULL)
                             execute(obj,xev);
			  else 
		             exec.exec(obj,xev);
		       }
	            }
	            else {
		       msg("Looking for CODE and PRESS");
	               if (obj->getType() == CODE && obj->getEventType() == PRESS)
	               {
		          if (very_verbose) cout << xev.xkey.keycode << " = " << obj->getKeyCode() << endl;
			  
			  if (obj->hasModifier(xev.xkey.state))
			     vmsg("true");
			  else
			     vmsg("false");
			  
		          if ((int)xev.xkey.keycode == obj->getKeyCode()
		               && (obj->hasModifier((int)xev.xkey.state) || obj->isUsedAsToggle()))
		          {
			     vmsg("getting to the right place!");
			     execute = plugins->exec(obj,xev);
	                     if (execute != NULL)
                                execute(obj,xev);
			     else 
				exec.exec(obj,xev);
		          }
	               }
	            }
		 }
	         break;
	   } 
	   case KeyRelease:
	   {
	       xev.xkey.state &= ~(numlock_mask | capslock_mask | scrolllock_mask);
	       LKey *obj = static_cast<LKey*>(myKbd.getObject((int)xev.xkey.keycode));

	       if (very_verbose) {
	         cout << "Key release !" << endl;
		 cout << "xev.xkey.keycode=" << xev.xkey.keycode << endl;
		 cout << "xev.xkey.state=" << xev.xkey.state << endl;
		 cout << "has modifier=" << obj->hasModifier(xev.xkey.state) << endl;
	       } 

	      if (obj != NULL) { 
	         if (obj->getType() == SYM && obj->getEventType() == RELEASE) {
		    if (xev.xkey.keycode == XKeysymToKeycode (display, (KeySym)obj->getKeySym())
		        && (obj->hasModifier(xev.xkey.state) || obj->isUsedAsToggle())) {
		       execute = plugins->exec(obj,xev);
	               if (execute != NULL)
                          execute(obj,xev);
		       else 
			  exec.exec(obj,xev);
		    }
	         }
	         else {
	            if (obj->getType() == CODE && obj->getEventType() == RELEASE) {
		       if ((int)xev.xkey.keycode == obj->getKeyCode()
		          && (obj->hasModifier((int)xev.xkey.state) || obj->isUsedAsToggle()))
		       {
		          execute = plugins->exec(obj,xev);
	                  if (execute != NULL)
                             execute(obj,xev);
			  else
			     exec.exec(obj,xev);
		       }
	            }
	         }
              }
	      break;
	   } 
	   case ButtonPress:
	   {
                LButton *obj = static_cast<LButton*>(myKbd.getObject((unsigned int)xev.xbutton.button));
		
	        if (very_verbose) {
	           cout << "Button press !" << endl;
	           cout << "xev.xbutton.button=" << xev.xbutton.button << endl;
	           cout << "xev.xbutton.state=" << xev.xbutton.state << endl;
		   cout << "has modifier=" << obj->hasModifier(xev.xkey.state) << endl;
	        } 

	        xev.xbutton.state &= ~(numlock_mask | capslock_mask | scrolllock_mask
			       | Button1Mask | Button2Mask | Button3Mask
			       | Button4Mask | Button5Mask);
                if (obj != NULL) {
	           if (obj->getType() == BUTTON && obj->getEventType() == PRESS) {
		      if (xev.xbutton.button == obj->getButton() 
			  && (obj->hasModifier(xev.xbutton.state) || obj->isUsedAsToggle()))
		      {
		       execute = plugins->exec(obj,xev);
	               if (execute != NULL)
                          execute(obj, xev);
		       else
			  exec.exec(obj,xev);
		      }
		}
	     }
	     break;
           }
	   case ButtonRelease:
	   {

             LButton *obj = static_cast<LButton*>(myKbd.getObject((unsigned int)xev.xbutton.button));

	     if (very_verbose) {
	        cout << "Button release !" << endl;
	        cout << "xev.xbutton.button=" << xev.xbutton.button << endl;
	        cout << "xev.xbutton.state=" << xev.xbutton.state << endl;
		cout << "has modifier=" << obj->hasModifier(xev.xkey.state) << endl;
	     }

	     xev.xbutton.state &= ~(numlock_mask | capslock_mask | scrolllock_mask
			       | Button1Mask | Button2Mask | Button3Mask
			       | Button4Mask | Button5Mask);

             if (obj != NULL) {
	     if (obj->getType() == BUTTON && obj->getEventType() == RELEASE)
		{
		  if (xev.xbutton.button == obj->getButton()
		      && ( obj->hasModifier(xev.xbutton.state) || obj->isUsedAsToggle()))
		    {
	               execute = plugins->exec(obj,xev);
	               if (execute != NULL)
                          execute(obj,xev);
		       else
			  exec.exec(obj,xev);
		    }
		}
	    }
	     break;
	  }
	   default:
	     break;
        } // End switch
        if (very_verbose)
	     lineakd_core_functions::print_event(xev.type);
        
     } // End global_enable
     else {
	     //if (very_verbose) cout << "Global is not enabled." << endl;
	     vmsg("Global is not enabled.");
     }

     if (myDisplay != NULL) {
        if (myDisplay->enabled()) {
	   //msg("age()");
           myDisplay->age();
        }
     }
  } // End while(true) loop.
  /* that's all folks.. We should never get here!!! */
  do_exit();
  return true;
}

