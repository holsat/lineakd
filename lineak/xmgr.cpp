/**************************************************************************
                          xmgr.cpp  -  description
                             -------------------
    begin                : Tue Feb 18 2003
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

#include <lineak/xmgr.h>
#include <lineak/lkbd.h>
#include <lineak/lkey.h>
#include <lineak/lbutton.h>

extern "C" {
   #include <stdlib.h>
   #include <string.h>
   #include <signal.h>
   #include <X11/Xlib.h>
   #include <X11/keysym.h>
}

#include <lineakd_core_functions.h>
#include <lineak/lineak_core_functions.h>

using namespace lineak_core_functions;
extern bool verbose;
extern bool very_verbose;

bool Xmgr::caughtXerror = false;
bool Xmgr::xkbremapped = false;
unsigned int Xmgr::numlock_mask = 0;
unsigned int Xmgr::scrolllock_mask = 0;
unsigned int Xmgr::capslock_mask = 0;
int Xmgr::ret = 0;

Xmgr::Xmgr(string display) : displayname(display) {
  mask = NoEventMask;
  ret = 0;
  screen = DefaultScreen (display.c_str());
 // mask |= KeyPressMask | KeyReleaseMask | PointerMotionMask | KeymapStateMask;

  if (! (ourdisplay = XOpenDisplay (displayname.c_str())) ) {
     error("Could not open the display. You should exit!");
     //exit(1);
  }
  else {
     win = DefaultRootWindow (ourdisplay);
     XSetErrorHandler(&Xhandler);
     getModifiers();
     XDisplayKeycodes (ourdisplay, &min, &max);
     openXkbDisplay(display);
  }
}

Xmgr::~Xmgr(){
   if (xkb != NULL)
      XkbFreeClientMap(xkb, XkbAllMapComponentsMask, True);

}

void Xmgr::openXkbDisplay(string display) {
   Display *dpy;	
   int mjr,mnr,error, evBaseRtrn, opcodeRtrn;
   mjr = XkbMajorVersion;
   mnr = XkbMinorVersion;

   evBaseRtrn=0;
   opcodeRtrn=0;
  
   char *TEMPdpyName = NULL;
   TEMPdpyName = (char *)malloc((strlen(display.c_str())+1)*sizeof(char));
   sprintf(TEMPdpyName, "%s",display.c_str());
   
   dpy = XkbOpenDisplay(TEMPdpyName, &evBaseRtrn,NULL,&mjr,&mnr,&error);
   free(TEMPdpyName);
   TEMPdpyName=NULL;

   if (dpy == NULL) {
      switch (error) {
         case XkbOD_BadLibraryVersion:
              cerr << "This program was compiled with XKB version " << XkbMajorVersion << "." << XkbMinorVersion;
              cerr << "     your X library supports incompatible version " << mjr << "." <<  mnr << endl;
              break;
         case XkbOD_ConnectionRefused:
              cerr << "Cannot open display " << display << endl;
              break;
         case XkbOD_NonXkbServer:
              cerr << "XKB extension not present on " << display << endl;
              break;
         case XkbOD_BadServerVersion:
              cerr << "This program was compiled with XKB version " << XkbMajorVersion << "." << XkbMinorVersion;
              cerr << "Server " << display << " uses incompatible version " << mjr << "." << mnr << endl;
              break;
         default:
              cout << "Unknown error " << error << " from XkbOpenDisplay" << endl;
              break;
      }
   } else {
//       if (opcodeRtrn)
//             XkbQueryExtension(dpy, &opcodeRtrn, &evBaseRtrn,NULL,&mjr,&mnr);
          if (!XkbQueryExtension(dpy, &opcodeRtrn, &evBaseRtrn,NULL,&mjr,&mnr)) {
               cerr << "Cannot initialize the Xkb extension." << endl;
          }
   }

   /* get the xkb map, and back it up. */
   xkb = XkbGetMap(ourdisplay, XkbAllMapComponentsMask, XkbUseCoreKbd);
   if (!xkb) {
      cerr << "FATAL ERROR: XkbGetMap failed" << endl;
   }
       
}
void Xmgr::commitXKBChanges(int tcode) {
   XkbMapChangesRec mapChangeRec;

   /* Commit the change back to the server */
   bzero(&mapChangeRec, sizeof(mapChangeRec));
   mapChangeRec.changed = XkbKeySymsMask | XkbKeyTypesMask;
   mapChangeRec.first_key_sym = tcode;
   mapChangeRec.num_key_syms = 1;
//   mapChangeRec.first_key_act = tcode;
//   mapChangeRec.num_key_acts = 1;
   mapChangeRec.first_type = 0;
   mapChangeRec.num_types = xkb->map->num_types;
   if ( ! XkbChangeMap(ourdisplay, xkb, &mapChangeRec) ) {
        cerr << "FATAL ERROR: map changed failed" << endl;
   }
}
void Xmgr::setXKBKey(int keycode, KeySym keysym) {
   XkbMessageAction xma;
   XkbMapChangesRec mapChangeRec;
   int types[1];
   /* construct the message action struct */
   xma.type = XkbSA_ActionMessage;
   xma.flags = XkbSA_MessageOnPress;
   strcpy((char *)xma.message, " ");
   if (!XkbKeycodeInRange(xkb, keycode)) {
      printf("The keycode %d cannot be used, as it's not between the min(%d) and max(%d) keycode of your keyboard.\n"
             "Please increase the 'maximum' value in /usr/X11R6/lib/X11/xkb/keycodes/xfree86, then restart X.\n", keycode, xkb-> min_key_code, xkb->max_key_code);
   }
   /* assign a group to the key code */
   types[0] = XkbOneLevelIndex;
   if ( XkbChangeTypesOfKey(xkb, keycode, 1, XkbGroup1Mask, types, NULL) != Success ) {
	   cerr << "FATAL ERROR: XkbChangeTypesOfKey failed" << endl;
   }
   /* change the keysym */
   if ( XkbResizeKeySyms( xkb, keycode, 1 ) == NULL ) {
	   cerr << "FATAL ERROR: resize keysym failed" << endl;
   }
   /* assign our new keysym to the keycode */
   *XkbKeySymsPtr(xkb,keycode) = keysym;
   /* add one key action to it */
   if ( XkbResizeKeyActions(xkb,keycode,1) == NULL ) {
	   cerr << "FATAL ERROR: resize key action failed" << endl;
   }
   commitXKBChanges(keycode);
   commitXKBChanges(keycode);
   commitXKBChanges(keycode);

   /* assign the message action to the key code */
   (&(xkb->server->acts[ xkb->server->key_acts[keycode] ]))->msg = xma;
   /* commit the change back to the server. */
   bzero(&mapChangeRec, sizeof(mapChangeRec));
   mapChangeRec.changed = XkbKeyActionsMask;
   mapChangeRec.first_key_act = keycode;
   mapChangeRec.num_key_acts = 1;
   if ( XkbChangeMap(ourdisplay, xkb, &mapChangeRec) ) {
	   if (verbose) cout << "Xkb map change done for keycode " << keycode << endl;
   } else {
	   cerr << "FATAL ERROR: map changed failed" << endl;
   }
   /* Select the ActionMessage event under any circumstances */
   if ( !XkbSelectEvents(ourdisplay, XkbUseCoreKbd, XkbActionMessageMask, XkbActionMessageMask)) {
	   cerr << "FATAL ERROR: Couldn't select desired XKB events." << endl;
	   cerr << "FATAL ERROR: Xkb is not initialized." << endl;
   }
}

/** Did we sucessfully initialize the display and xkbd? */
bool Xmgr::initialized() {
	if (ourdisplay == NULL)
		return false;
	return true;
}
/* X error handler. When a BadValue error occurs during XkbSetMap, we want to retry */
/* Right now we are just ignoring errors */
int Xmgr::Xhandler(Display* theDisplay, XErrorEvent* xev) {
        //if (( (int)(xev->request_code) ==150 || (int)(xev->request_code) ==149) && (int)(xev->minor_code) ==9) {
    	   caughtXerror = true;
	   ret = (int)(xev->request_code);
           cerr << "*** Xlib error caught ***" << endl;
	   cerr << "Major opcode of failed request: " << (int)(xev->request_code) << " (XKEYBOARD)" << endl;
	   cerr << "Minor opcode of failed request: " << (int)(xev->minor_code) << " (XkbSetMap)" << endl;
	   cerr << "Resource ID of failed request: " << xev->resourceid << endl;
           cerr << "Serial number of failed request: " << xev->serial << endl;
           cerr << "Error code: " << (int)(xev->error_code) << endl;
           cerr << "Type: " << xev->type << endl;
	   cerr << "Maybe we should exit now?" << endl;
        //}
  	return ret;
}
/* Get the modifier masks from the system */
bool Xmgr::getModifiers ()
{
   if (ourdisplay) {
        msg("Getting modifiers for display.");
  	XModifierKeymap *modmap;
  	KeyCode nlock, slock;
	static int mask_table[8] = {
	    ShiftMask, LockMask, ControlMask, Mod1Mask,
	    Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
	};

	nlock = XKeysymToKeycode (ourdisplay, XK_Num_Lock);
	slock = XKeysymToKeycode (ourdisplay, XK_Scroll_Lock);

	/*
	 *    * Find out the masks for the NumLock and ScrollLock modifiers,
	 *       * so that we can bind the grabs for when they are enabled too.
	 *          */
    	modmap = XGetModifierMapping (ourdisplay);

        if (modmap != NULL && modmap->max_keypermod > 0)
	{
	   for (int i = 0; i < 8 * modmap->max_keypermod; i++)
	   {
	      if (modmap->modifiermap[i] == nlock && nlock != 0)
	          numlock_mask = mask_table[i / modmap->max_keypermod];
	      else if (modmap->modifiermap[i] == slock && slock != 0)
	          scrolllock_mask = mask_table[i / modmap->max_keypermod];
	   }
	}

	capslock_mask = LockMask;
        if (very_verbose) msg(string("numlock_mask = " + numlock_mask));
	if (very_verbose) msg(string("scrolllock_mask = " + scrolllock_mask));
	if (very_verbose) msg(string("capslock_mask = " + capslock_mask));
	
        if (modmap)
	   XFreeModifiermap (modmap);

	return true;
   }
   return false;
}

/* initialize X (xkb) so it understands the extra keys */
void Xmgr::cleanup(LKbd& myKbd) {
  if (ourdisplay) {

	vector<int> keycodes = myKbd.getKeyCodes();

  	/* Go through all the keycodes and ungrab them. */
   	for (vector<int>::size_type i=0; i != keycodes.size(); ) {
		int keycode = keycodes[i];
    	   /* Ungrab the key */
           XUngrabKey(ourdisplay, keycode, AnyModifier, DefaultRootWindow(ourdisplay));
           i++;
  	}
	vector<unsigned int> buttoncodes = myKbd.getButtonCodes();

  	/* Go through all the buttons and ungrab them. */
   	for (vector<unsigned int>::size_type i=0; i != buttoncodes.size(); ) {
		unsigned int buttoncode = buttoncodes[i];

    	   /* Ungrab the button */
           XUngrabButton(ourdisplay, buttoncode, AnyModifier, DefaultRootWindow(ourdisplay));
           i++;
  	}
  }
}
/* initialize X (xkb) so it understands the extra keys */
bool Xmgr::initialize(LKbd& myKbd) {
   if (ourdisplay) {
   	//getModifiers();
	
        XAllowEvents(ourdisplay, AsyncKeyboard, CurrentTime);
        XSelectInput(ourdisplay,DefaultRootWindow(ourdisplay),mask);
//        vector<int> keycodes = myKbd.getKeyCodes();
	vector<string> keys = myKbd.getNames();
        for (vector<string>::iterator it = keys.begin(); it != keys.end(); it++) {
           LObject *obj = myKbd.getObject(*it); 
           if (obj->getType() == SYM)
           {
	      LKey *key = static_cast<LKey*>(obj);
              grabKey (key);
           }
           else if (obj->getType() == BUTTON)
           {
	      LButton *btn = static_cast<LButton*>(obj);
              grabButton (btn);
           }
           else
           {  // It is either a key of type CODE or nothing we know.
	      LKey *key = static_cast<LKey*>(obj);
              if ((key->getKeyCode()) >= min && (key->getKeyCode()) <= max)
              {
                 grabKey (key);
              }
              else
              {
                 cerr << "--- X manager initialization error ---" << endl;

                 if (very_verbose)
                 {
                    cout << *key;
                 }

                 fprintf (stderr,
                       "  The keycode %d cannot be used, as it's not between the\n"
                       "  min(%d) and max(%d) keycode of your keyboard.\n"
                       "  Please increase the 'maximum' value in\n"
                       "    /usr/X11R6/lib/X11/xkb/keycodes/xfree86,\n"
                       "  then restart X.\n", key->getKeyCode(), min, max);
                 exit (false);
              }
           }
        }
   }
   else
        return false;

 return true;
}

void Xmgr::grabKey (LKey* key)
{
  KeyCode keycode = key->getKeyCode();
  unsigned int modifier = 0; // = key->getModifier();
  int ret=0;

  if (verbose)
     cout << "grabKey for " << key->getName() << endl;

  vector<unsigned int> mods;
  
  /** Experimental */
  /** If the key is a toggleable key, it will not have any modifiers.
   *  In this case we need to grab the default key code, so we need
   *  mods[0] to be 0.
   *  */
  if (key->isUsedAsToggle()) {
     mods.push_back(0);
     mods.push_back(0);
  }
  else  
     mods = key->getModifiers();

  for (vector<unsigned int>::size_type i=0; i < mods.size(); i++) {
     if (very_verbose) cout << "mods[" << i << "] = " << mods[i] << endl;
     modifier = mods[i];
     /** End experimental */
     modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);
     if (very_verbose) cout << "modifier = " << modifier << endl;

     
     XGrabKey (ourdisplay, keycode, modifier, (win ? win : DefaultRootWindow (ourdisplay)), False, GrabModeAsync, GrabModeAsync);

     if (modifier == AnyModifier)
       return;

     if (numlock_mask)
        XGrabKey (ourdisplay, keycode, modifier | numlock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (capslock_mask)
        XGrabKey (ourdisplay, keycode, modifier | capslock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (scrolllock_mask)
        XGrabKey (ourdisplay, keycode, modifier | scrolllock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (numlock_mask && capslock_mask)
        XGrabKey (ourdisplay, keycode, modifier | numlock_mask | capslock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (numlock_mask && scrolllock_mask)
        XGrabKey (ourdisplay, keycode, modifier | numlock_mask | scrolllock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (capslock_mask && scrolllock_mask)
        XGrabKey (ourdisplay, keycode, modifier | capslock_mask | scrolllock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)),
              False, GrabModeAsync, GrabModeAsync);

     if (numlock_mask && capslock_mask && scrolllock_mask)
        XGrabKey (ourdisplay, keycode,
              modifier | numlock_mask | capslock_mask | scrolllock_mask,
              (win ? win : DefaultRootWindow (ourdisplay)), False, GrabModeAsync,
              GrabModeAsync);

     if (caughtXerror) {
         cerr << "********************************" << endl;
         cerr << "Got and X error." << endl;
         cerr << "Return code = " << ret << endl;
         cerr << "We should do something about it." << endl;
         cerr << "********************************" << endl;
     }

     
   }
}
void Xmgr::grabButton (LButton* key)
{
  unsigned int button = key->getButton();
  unsigned int modifier = 0; // = key->getModifier();
  vector<unsigned int> mods = key->getModifiers();
  
  if (verbose)
     cout << "grabButton for " << key->getName() << endl;

  for (vector<unsigned int>::size_type i=0; i < mods.size(); i++) {
        if (very_verbose) cout << "mods[" << i << "] = " << mods[i] << endl;
        modifier &= mods[i];
	modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);
        if (very_verbose) cout << "modifier = " << modifier << endl;
	
	XGrabButton (ourdisplay, button, modifier, (win ? win : DefaultRootWindow (ourdisplay)),
		False, ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeAsync, None, None);
	
	if (modifier == AnyModifier)
	return;
	
	if (numlock_mask)
	XGrabButton (ourdisplay, button, modifier | numlock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	
	if (capslock_mask)
	XGrabButton (ourdisplay, button, modifier | capslock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	if (scrolllock_mask)
	XGrabButton (ourdisplay, button, modifier | scrolllock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	if (numlock_mask && capslock_mask)
	XGrabButton (ourdisplay, button, modifier | numlock_mask | capslock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	if (numlock_mask && scrolllock_mask)
	XGrabButton (ourdisplay, button, modifier | numlock_mask | scrolllock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	if (capslock_mask && scrolllock_mask)
	XGrabButton (ourdisplay, button, modifier | capslock_mask | scrolllock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)),
			False, ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None);
	
	if (numlock_mask && capslock_mask && scrolllock_mask)
	XGrabButton (ourdisplay, button,
			modifier | numlock_mask | capslock_mask | scrolllock_mask,
			(win ? win : DefaultRootWindow (ourdisplay)), False,
			ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
			GrabModeAsync, None, None);
   }
}

