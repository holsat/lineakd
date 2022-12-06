/***************************************************************************
                          msgpasser.h  -  description
                             -------------------
    begin                : Sun Feb 23 2003
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

#ifndef MSGPASSER_H
#define MSGPASSER_H

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
}
#include <lineak/definitions.h>

/* Message Queue definitions */
#define CMDSIZE 1000
#define ARGSIZE 200
#define dSEND_KEY 292
#define dRECIEVE_KEY 291

#define dHUP 1
#define dRELOAD 2
#define dEXIT 3
#define dTERM 15
#define dDISABLE 16
#define dENABLE 17
#define dCOMMAND 20
#define dGETDATA 21
#define dSENDDATA 22
#define dACK 50 


// Types of data that are going to be passed in the message queue.
typedef enum { NODATA, LOBJECT, LOBJECT_BYNAME, LCONFIG, LDEF, LKBD, PLUGIN_MANAGER, STRING_COMMAND, CHAR_COMMAND, LCOMMAND } eak_data_types;

struct eak_direct_t {
        char args[ARGSIZE];
	eak_data_types data_type;
//	void * data;
	size_t size;
};


struct eak_data_t {
        char args[CMDSIZE];
	eak_data_types data_type;
	size_t size;
};

struct eak_msg {
        long int msg_type;
        eak_direct_t directive;
};

struct eak_data {
        long int msg_type;
        eak_data_t directive;
};

/**Pass messages via a message queue.
  *@author Sheldon Lee Wen
  */

class msgPasser {
public: 
	msgPasser(int ikey = RECIEVE_KEY);
	~msgPasser();
	/** Get a message from the queue */
	eak_msg getMessage();
	eak_data getMessageData();
	/** Send a message to the message queue */
	void sendMessage(int comm, string args);
	void sendMessage(eak_msg imsg);
	void sendMessage(eak_data imsg);
	/** Remove the message queue. */
	void quit();
	/** Create the message queue */
	bool start();
	inline bool getStatus() { return status; status = true; }
	/** Static values for all msg passing classes */
	//static int CMDSIZE;
	//static int ARGSIZE;
public:
	static const int RECIEVE_KEY;
	static const int SEND_KEY;
	static const int HUP;
	static const int RELOAD;
	static const int EXIT;
	static const int TERM;
	static const int DISABLE;
	static const int ENABLE;
	static const int COMMAND;
	static const int GETDATA;
	static const int SENDDATA;
	static const int ACK;
protected:
	static bool running;
	static key_t key;
	bool status;
	int msgid;
	struct eak_msg myMsg;
	long int msg_to_recieve;
	
	
};

#endif
