/***************************************************************************
                          msgpasser.cpp  -  description
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

#include <lineak/msgpasser.h>
#include <string>
#include <iostream>
#include <lineak/definitions.h>

using namespace std;
bool msgPasser::running = true;
key_t msgPasser::key = -1; 
//int CMDSIZE = CMDSIZE;
const int msgPasser::RECIEVE_KEY = dRECIEVE_KEY;
const int msgPasser::SEND_KEY = dSEND_KEY;
const int msgPasser::HUP = dHUP;
const int msgPasser::RELOAD = dRELOAD;
const int msgPasser::EXIT = dEXIT;
const int msgPasser::TERM = dTERM;
const int msgPasser::DISABLE = dDISABLE;
const int msgPasser::ENABLE = dENABLE;
const int msgPasser::COMMAND = dCOMMAND;
const int msgPasser::GETDATA = dGETDATA;
const int msgPasser::SENDDATA = dSENDDATA;
const int msgPasser::ACK = dACK;
extern bool very_verbose;

msgPasser::msgPasser(int ikey){
  msg_to_recieve = 0;
  status = true;
  string pathname = getenv("HOME");
  pathname += LINEAKDIR;
  key = ftok(pathname.c_str(), getuid());
  int serr = errno;
  key = ftok(pathname.c_str(), ikey + getuid());
  if (key == -1) {
  	cerr << "Failed to generate a System V IPC key. Error:";
	cerr << strerror(serr) << endl;
	exit(1);
  }
  else
   	key = ikey + getuid();
   	//key = getuid();
//	start();
}

msgPasser::~msgPasser(){
}
bool msgPasser::start() {
	//msgid = msgget((key_t)key, IPC_CREAT);
	msgid = msgget((key_t)key, 0640 | IPC_CREAT);
	int serr = errno;
	if (msgid == -1) {
		cerr << "Failed to set up message queue with key: " << key << " , with error: " << serr << endl;
		cout << strerror(serr);
		exit(false);
		status = false;
	}
	return (status);
}
void msgPasser::quit() {
	if (msgid != -1) {
		if (msgctl (msgid, IPC_RMID, 0) == -1) {
			status = false;
			cerr << "msgctl(IPC_RMID) failed" << endl;
		}
	}
	msgid = -1;
}
eak_msg msgPasser::getMessage() {
	myMsg.msg_type = 0;
	strcpy(myMsg.directive.args,"");
	//myMsg.directive.args = "";
	if (msgrcv(msgid, (void *)&myMsg, (sizeof(eak_direct_t) - sizeof(long int)), msg_to_recieve, IPC_NOWAIT) == -1 ) {
		status = false;
		if (very_verbose)
		   cerr << "msgrcv failed with error: " << strerror(errno) << endl;
	}

	return myMsg;
}
eak_data msgPasser::getMessageData() {
	eak_data myMsg;
        myMsg.msg_type = 0;
	strcpy(myMsg.directive.args,"");
	if (msgrcv(msgid, (void *)&myMsg, (sizeof(eak_data_t) - sizeof(long int)), msg_to_recieve, IPC_NOWAIT) == -1 ) {
		status = false;
		if (very_verbose)
		   cerr << "msgrcv failed with error: " << strerror(errno) << endl;
	}

	return myMsg;
}
void msgPasser::sendMessage(int comm, string args) {
	myMsg.msg_type = comm;

	strcpy(myMsg.directive.args, args.c_str());
	//myMsg.directive.args = args;
	if (msgsnd(msgid, (void *)&myMsg, (sizeof(eak_direct_t) - sizeof(long int)), IPC_NOWAIT) == -1) {
		status = false;
		cerr << "msgsnd failed with error: " << strerror(errno) << endl;
	}

}
void msgPasser::sendMessage(eak_msg imsg) {
	myMsg = imsg;

	//strcpy(myMsg.directive.args, args.c_str());
	if (msgsnd(msgid, (void *)&myMsg, (sizeof(eak_direct_t) - sizeof(long int)), IPC_NOWAIT) == -1) {
		status = false;
		cerr << "msgsnd failed with error: " << strerror(errno) << endl;
	}

}

void msgPasser::sendMessage(eak_data imsg) {
	eak_data myMsg = imsg;

	if (msgsnd(msgid, (void *)&myMsg, (sizeof(eak_data_t) - sizeof(long int)), IPC_NOWAIT) == -1) {
		status = false;
		cerr << "msgsnd failed with error: " << strerror(errno) << endl;
	}

}

