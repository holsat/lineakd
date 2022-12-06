/***************************************************************************
                          definitions.h  -  description
                             -------------------
    begin                : Sun Jan 5 2003
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
#ifndef DEFINITIONS_H
#define DEFINITIONS_H
// *INDENT-OFF*

#include <string>
using namespace std;

class LCommand;

#define snull  ""
#define pnull 0
//#define MAXRETRIES 25 /* prevents possible race condition, don't set this too low!! 25 will do */

#define CONF_HEADER \
"# LinEAK - Linux support for Easy Access and Internet Keyboards\n" \
"#  Copyright (c) 2001,2002, 2003  Sheldon Lee Wen <leewsb@hotmail.com> (Current Maintainer)\n" \
"#  	and Mark Smulders <Mark@PIRnet.nl>\n" \
"#  http://lineak.sourceforge.net\n" \
"#\n" \
"# lineakd configuration file\n" \
"#\n" \
"# example key configuration:\n" \
"# \tplay\t= \"xmms --play-pause\"\n" \
"# \teject\t= EAK_EJECT\n" \
"#\n" \
"# Lineakd supports the following modifier keys:\n" \
"#    control alt shift mod2 mod3 mod4 mod5\n" \
"\n"

#define DEF_HEADER \
"# LinEAK - Linux support for Easy Access and Internet Keyboards\n" \
"#  Copyright (c) 2001,2002, 2003 Sheldon Lee Wen <leewsb@hotmail.com>\n" \
"# 	and Mark Smulders <Mark@PIRnet.nl>\n" \
"#  http://lineak.sourceforge.net\n" \
"#\n" \
"# keyboard definition file:\n" \
"#   this file defines the extra keys of the keyboard types,\n" \
"#   and the corresponding keycode.\n" \
"#\n" \
"# if your keyboard is not in here, please send me an e-mail so\n" \
"# I can add support for it.\n" \
"#\n" \
"# WARNING: this definition file is NOT compatible with\n" \
"#          lineakd 0.1 and 0.2\n" \
"#          lineakconfig 0.1\n" \
"\n"

/* the conf and def filenames */
/* A System wide config file */
#define SYS_CONFFILE "/etc/lineak/lineakd.conf"
#define LINEAKDIR "/.lineak/"
#define PICSDIR "/.lineak/Pics/"
#define CONFFILE "/.lineak/lineakd.conf" /* (relative from $HOME dir) */
#define BAKFILE "/.lineak/.lineakd.conf.backup"
#define DEFFILE  "lineakkb.def"
#define PIDFILE "/.lineak/lineakd.pid"

/** Definitions for accessing standard information from a
 *  ConfigDirectives object */
#define _CD_KEYBOARD_TYPE "KeyboardType"
#define _CD_USERDEFFILE "userdeffile"
#define _CD_SYSDEFFILE  "deffilename"
#define _CD_USERCONFFILE "conffilename"
#define _CD_CDROM_DEVICE "CdromDevice"
#define _CD_MIXER_DEVICE "MixerDevice"
#define _CD_RAW_COMMANDS "RAWCommands"

//#define DEFAULT_CDROM_DEVICE "/dev/cdrom"
#define DEFAULT_MIXER_DEVICE "/dev/mixer"

/* for volume settings using the mixer device */
#define MIXERDEV DEFAULT_MIXER_DEVICE
#define VOLUP_VALUE 5
#define VOLDOWN_VALUE -5
#define VOLMUTE_VALUE 0
#define MAX_VOLUME 100

//#define MAXRETRIES 25 /* prevents possible race condition, don't set this too low!! 25 will do */

// *INDENT-ON*
#endif
