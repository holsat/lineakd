/***************************************************************************
                          commandexec.cpp  -  description
                             -------------------
    begin                : ?  8? 19 2003
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

#include <config.h>

extern "C" {
#include <sys/types.h>
#include <unistd.h>
}
#include <algorithm>
#include <cctype>

#include <lineak/lcommand.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/commandexec.h>
#include <lineak/displayctrl.h>
#include <lineak/lobject.h>

extern bool verbose;
extern bool global_enable;

displayCtrl* CommandExec::display = NULL;

CommandExec::CommandExec() {
	//display = NULL;
}

CommandExec::~CommandExec(){
}
/** Execute the command for the keycode. */
bool CommandExec::exec(LObject* imyKey, XEvent xev){
        LCommand command;
        string dname;
		
	if (imyKey->getType() == CODE || imyKey->getType() == SYM) {
	    command = imyKey->getCommand(xev.xkey.state);
	    dname = imyKey->getCommandDisplayName(xev.xkey.state);   
	}
        if (imyKey->getType() == BUTTON) {
	    command = imyKey->getCommand(xev.xbutton.state);
            dname = imyKey->getCommandDisplayName(xev.xbutton.state);
	}
	if (dname == "" or dname == snull) {
          if (imyKey->isUsedAsToggle())
              dname = imyKey->getNextToggleName();
          else
              dname = imyKey->getName();
        }

	//LCommand command = imyKey->getCommand(xev.xkey.state);
	/* Give verbose output about the command and display name */
	if (verbose) {
		cout << "----------------------------------------" << endl;
		//cout << "For keycode: " << keycode << endl;
		cout << " Key: " << endl << *(imyKey) << endl;
	  	cout << "	On Screen Display: " << dname << endl;
		cout << "	Command: " << command << endl;
		cout << "----------------------------------------" << endl;
	}
  	/* lookup the key in our EAKeylist */
  	if (!command.isEmpty())    {
		//cout.setf(ios::boolalpha);
		if (verbose) cout << "commandexec global_enable = " << global_enable << endl;
                if (global_enable) {
                        if (verbose) cout << "... that's the " <<  imyKey->getName() << " key" << endl;
//			        if (myDisplay != NULL)
//                                   myDisplay->show(dname);
				string comm = lineak_util_functions::unescape(command.getCommand(), "\"#");
				if (comm == "") {
					if (verbose) cerr << "Attempting to run an empty command." << endl;
					return false;
				}
                                if (!fork()) {
                                        //string comm = lineak_util_functions::unescape(command.getCommand(), "\"#");
                                        comm += " &";
                                        /* child process that tries to run the command, and then exits */
                                        /* all specials done, let's go for it... ;) */
                                        if (verbose) cout << "... running " << comm << endl;
					if (verbose) cout << "... displaying " << dname << endl;
                                        system(comm.c_str());
                                        exit(true);
                                }
                                /* NOTE: no, we don't wait() for the child. we just ignore the SIGCLD signal */
                        /** Rotate the key state if it's a toggleable key */
			if (imyKey->isUsedAsToggle())
				imyKey->toggleState();
			/** Use the OSD to show the output */
			cout << "Showing OSD" << endl;
			display->show(dname);
			cout << "Returning" << endl;
                }
  	}
        return true;
}
