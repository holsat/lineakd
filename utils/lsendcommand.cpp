/* lsendcommand.cpp ************************************************/
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/lobject.h>
#include <lineak/lkey.h>
#include <lineak/msgpasser.h>

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>

#if __GNUC__ >= 3
   #include <ostream>
   #include <iostream>
   #include <sstream>
#else
   #include <ostream.h>
   #include <iostream.h>
   #include <sstream.h>
#endif


using namespace std;
using namespace lineak_core_functions;
using namespace lineak_util_functions;


int main(int argc, char **argv)
{
    if (argc < 2)
    	return 1;

    char *arg1 = argv[1];
    char *arg2;

    string type = arg1;
    string argument = "";

    if (argc > 2) {
      argument = string(argv[2]);
    }

    //cout << "type = " << type << endl;
    //cout << "argument = " << argument << endl;
    eak_msg msg;
    msgPasser myMsg;
    myMsg.start();

    if ( type == "RECIEVE_KEY" ) {
	    msg.msg_type = msgPasser::GETDATA;
            msg.directive.data_type=LOBJECT_BYNAME;
            strcpy(msg.directive.args,"Mute");
            myMsg.sendMessage(msg);
            sleep(2);
            eak_data data = myMsg.getMessageData();
            cout << data.directive.args << endl;
	    string data1 = data.directive.args;
	    stringstream ss(data1);
	    LKey key;
	    ss >> key;
    }
    if ( type == "SEND_KEY" ) {
	    msg.msg_type = msgPasser::SEND_KEY;
    }
    if ( type == "HUP") {
	    cout << "Sending an HUP message" << endl;
	    //msg.msg_type = msgPasser::HUP;
	    myMsg.sendMessage(msgPasser::HUP,"hup");
    }
    if ( type == "RELOAD") {
	    cout << "Sending an RELOAD message" << endl;
	    //msg.msg_type = msgPasser::RELOAD;
	    myMsg.sendMessage(msgPasser::HUP,"hup");
    }
    if ( type == "EXIT") {
	    cout << "Sending an EXIT message" << endl;
	    //msg.msg_type = msgPasser::EXIT;
	    myMsg.sendMessage(msgPasser::EXIT,"exit");
    }
    if ( type == "TERM") {
	    cout << "Sending a TERMINATE message" << endl;
	    //msg.msg_type = msgPasser::TERM;
	    myMsg.sendMessage(msgPasser::TERM,"term");
    }
    if ( type == "DISABLE") {
	    cout << "Sending the DISABLE message" << endl;
	    //msg.msg_type = msgPasser::DISABLE;
	    myMsg.sendMessage(msgPasser::DISABLE,"disable");
    }
    if ( type == "ENABLE") {
	    cout << "Sending the ENABLE message" << endl;
	    //msg.msg_type = msgPasser::ENABLE;
	    myMsg.sendMessage(msgPasser::ENABLE,"enable");
    }
    if ( type == "COMMAND") {
	    cout << "Sending a COMMAND message" << endl;
	    //cout << "argument = " << argument << endl;
	    //char command[ARGSIZE] = "Testing 1 2 3\0";
	    msg.msg_type = msgPasser::COMMAND;
	    msg.directive.data_type=STRING_COMMAND;
	    //strcpy(msg.command,command);
	    //msg.directive.data = argv[2];
	    //cout << "data = " << msg.directive.data << endl;
	    //cout << "data = " << (char*)msg.directive.data << endl;
	    myMsg.sendMessage(msg);
     //       sleep(1);
/*	    while (msg.msg_type != msgPasser::ACK) {
		    sleep(1);
		    msg = myMsg.getMessage();
		    cout << "Got a: " << msg.msg_type << " message" << endl;
	    } */
    }
    if ( type == "GETDATA") {
	    msg.msg_type = msgPasser::GETDATA;
    }
    if ( type == "SENDDATA") {
	    msg.msg_type = msgPasser::SENDDATA;
    }
			    
    return 0;
}
