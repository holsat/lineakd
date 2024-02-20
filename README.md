                 
# Lineakd - The Linux Easy Access Keyboard Daemon

This is a quick guide to the lineakd package.

## Status

This project is here for historical purposes. It no longer builds on modern linux distrubutions due to it's usage of old versions of the gnu libtool and autotools. Anyone is welcome to update the build files to something more modern.

## History
   
Lineakd was originally written by Mark Smulders. However, at around version 0.4 Mark became too busy to continue to maintain and develop lineakd. At about that time, I was already adding functionality to lineakd, enough so that I had a fairly decent grasp on the code base. As Mark dropped off, I picked up the slack, but at the expense of my own project, klineakconfig, a GUI configuration tool for lineakd.

## Quick Setup
   
For those who want to setup their configuration quickly, here are the steps:

  run 'lineakd -l' to see a list of available keyboards.
  
  run 'lineakd -c TYPE' to create a standard configuration file for the keyboard with an identifier of TYPE that you found by running lineakd -l
    The configuration file will be $HOME/.lineak/lineakd.conf
    
  Edit the config file to specify commands for the keys. The commands will initially be empty.
  This file can be modified to suit your needs.
  Do NOT, however, add extra keys or keynames!
  You can only change the values of the keys that are already present!
  If you have a key on your keyboard that is not in the file, please post a definition file for it to lineak-devel@lists.sourceforge.net.

    
## Usage

Below is the usage message of lineakd:

Usage: lineakd [options...]
Valid options are:
    -l,        --kbd-list           Show all supported keyboard types
    
    This option will list all of the keyboards that lineakd can find in lineakkb.def files. It will print out these keyboards by unique identifier and then a description. Eg:
 
 [TYPE]         [Full name]

 ACEAKV12       Acer AirKey V (12 keys)
 APK7           Apple Pro Keyboard (7 keys)
 BTC5107        BTC 5107
 BTC5113RF      BTC 5113RF MultiMedia
 BTC5126T       BTC 5126T
 BTC9000        BTC 9000

    The unique identifier is necessary when asking lineakd to generate a config file, and must be present in the config file in a 
       KeyboardType = IDENTIFIER
    directive.

    -L,        --plugin-list        List the loaded plugins and their macros.
    -D,        --directives-list    Show all of the default, implicit configuration directives.
    
    -f FILE,   --conffile=FILE      Specify the .conf file
    -e FILE,   --deffile=FILE       Specify the .def file
    
    These options allow you to specify the paths to the config and defintion files.
    
    -d DEVICE, --cdrom-dev=DEVICE   Specify the CDROM device
    -m DEVICE, --mixer-dev=DEVICE   Specify the mixer device
    
    These options allow you to specify the devices to use for the CDROM and mixer. These are only used when the default plugin is also used.
    
    -c TYPE,   --config=TYPE        Create a new lineakd.conf file for keyboard TYPE
                                      (warning: old one will be overwritten!)
    
    This options tells lineakd to generate a config file in the users ~/.lineak/ directory. The name of the file will be lineakd.conf and it will contain default configuration directives. 
    TYPE must be the unique identifier of a keyboard definition. These can be gotten by running lineakd -l or lineakd --kdb-list
    
    -v,        --verbose            Print verbose messages
                                      (only visible when not running in background)
				      
    If you use this option, lineakd will print out a lot of debugging messages that are very informative.
    
    -h,        --help               Prints the usage message.
    
    -r,        --reload             Reload the configuration files.
    
    This tells lineakd to reload the configuration and keyboard definition files. It will not however reload plugins. If you have added or removed a plugin, you must stop and start the daemon.
    
    -z         --sleep              Disable handling of keyboard events
    -k         --awaken             Enable handling of keyboard events
    
    These options disable (-z) and enable (-k) the processing of events. When the daemon is disabled events will stiil be seen, but no commands will be executed until lineakd is run with the -k or --awaken option.
    
    -x,        --exit               Tell the daemon to exit.
    
    This option will cause lineakd to exit.
    
    -n,        --display-font          Set the font the on-screen display will use. It must be in X format.
                                       i.e. -bitstream-charter-black-r-normal-*-*-240-*-*-p-*-iso8859-1
    -o,        --display-color         Set the color of the on-screen display. It must be given as a color code.
                                       i.e. #0aff00
    -t,        --display-timeout       Set the amount of time that the on-screen display keeps it's text on screen.
    -p,        --display-pos           Set the vertical position where osd will display.
                                       Valid values are: top, middle, bottom
    -a,        --display-align         Set the horizontal alignment for the on-screen display.
                                       Valid values are: left, center, right
    -y,        --display-hoffset       Set the horizontal offset from the alignment (see above).
    -w,        --display-voffset       Set the vertical offset from the position (see osd-pos above).
    -s,        --display-soffset       Set the offset for the shadow behind displayed text.

    These options allow one to specify on the command line, or override what is specified in a config file, various display preferences for the on screen display. These are only useful when using a display plugin such as the lineak_xosdplugin
    


## Configuration
   
Lineak uses two types of configuration files: lineakkb.def files and lineakd.conf files.

### lineakkb.def

lineakkb.def files contain keyboard descriptions that map key and button names, to keycodes and button codes respectively. A lineakkb.def file can be placed either in a system directory, or in a users home directory (in the .lineak/ subdirectory). The lineakd daemon will first read the system lineakkb.def file, usually at /etc/lineakkb.def and then read the ~/.lineak/lineakkb.def file. It will attempt to reconcile the entries, using the local entries to override the system entries.
	      
Here is an example of a keyboard definition from a lineakkb.def file:
	
[LTCDP]
   brandname = "Logitech"
   modelname = "Cordless Desktop Pro"
   [KEYS]
      Sleep                 = 223
      Internet              = 178
      Mail                  = 236
   [END KEYS]
   [BUTTONS]
      Thumb                 = 2
   [END BUTTONS]
[END LTCDP]
	
The structure is important. Adding a new keyboard is as easy as determining the special keys that you keyboard has, using something like xev, and writing your own definition file. Note that the daemon will only register keys that you also have defined in your lineakd.conf file.

### lineakd.conf

lineakd.conf files can live either in a system directory, or in the users home directory under the .lineak subdirectory. lineakd will attempt to load a users config file before looking for a system wide configuration file.

####   Directives
   
Lineakd supports various configuration file directives. Plugins also can specify their own directives. The standard directives that are supported follow with their defaults:

KeyboardType            = 
CdromDevice             = 
MixerDevice             = 
Display_plugin = internal
Display_font = -adobe-helvetica-bold-r-normal-*-*-240-*-*-p-*-*-*
Display_color = 0aff00
Display_timeout = 3
Display_pos = bottom
Display_align = center
Display_hoffset = 0
Display_voffset = 50
Display_soffset = 1
keystate_capslock =
keystate_numlock =
keystate_scrolllock =

KeyboardType is the only mandatory defintion. If KeyboardType is not defined, the daemon will not function properly.

The CdromDevice and MixerDevice settings specify the device files to use. These are only useful when used in conjunction with a plugin that requires them. One such plugin is the defaultplugin.

The Display_* directives control the functioning of the on screen display.
    Display_plugin specifies with of the installed display plugins to use. Currently the only OSD plugin is the xosd plugin.
    
    Display_font specifies which font name to use for the on screen display. Currently the xosd plugin requires font names in X format.
    
    Display_color specifies the color of the font. As xosd currently requires the color in a numeric format, that is the default. You can use something like kcolorchooser or the gimp to get color values for you.
    
    Display_timeout specifies the amount of time the display should remain on screen. After this number of seconds the display will be removed.
    
    Display_pos specifies the vertical position for the OSD. Possible values are bottom, middle and top.
    
    Display_align specifies the horizontal position for the OSD. Possible values are left, center and right.
    
The keystate_* directives control modifiers. By default, lineakd does not pay attention to the modifiers NumLock, CapsLock and ScrollLock. Set these directives to "enable" to use modifiers. (Note: as of beta3, these options do nothing. We automatically use modifiers if they are defined on a per command basis.)

#### Toggleable keys
   
lineakd supports toggle keys. A toggleable key is one that can have a various number of states for each push of the key. For example, on many keyboards the  Play and Pause keys are the same. When you push the key once it plays, the next time it pauses. The same thing goes for the Mute key. You must define a toggle key as key1|key2 in the definition (lineakkb.def) file. We can define a single action for this key in the config file by specifying the keyname in the form key1|key2 - this effectively treats the toggleable key as a normal key. It tells lineakd that we don't want to use this a a toggleable key, maybe because the command we are going to bind to it supports the notion of toggling. We can also specify seperate actions for the Play and Pause 	states by assigning actions to key1 and key2. eg. 

	On the LTCDP there is the key 'Play|Pause' In my
	config file I can specify either:
	
	Play|Pause     = "/usr/bin/xmms -t"

	To treat this as a normal key, or, if I want seperate functions for the play and pause 
	states:

	Play            = "/usr/bin/xmms -p"
	Pause           = "/usr/bin/xmms -u"
   
Note that there can be more than two states for a toggleable key. For example key1|key2|key3 could be defined.

#### Modifiers
   
lineakd now supports a variable number of modifiers to a key. However, this only applies to non-toggleable keys so a key such as Play|Pause will not work with modifiers if you use it as a toggleable key by putting entries like this:
	Play = something
	Pause = something else
in your config file. However, if you use the key as a non-toggleable key, i.e. like this:
	Play|Pause = something
then it becomes possible to use modifiers.

Currently we support the following modifiers:
	control 
	alt 
	shift 
	mod2 
	mod3 
	mod4 
	mod5
To use a modifier, we do something like this in the config file:
	Sleep+control = something
	Sleep+alt = something different
	Sleep+shift = something more different
	Sleep = sleepiness
	etc.

Modifiers to buttons, if they are defined in the lineakkb.def file, work the same way.

#### On-Screen Display

lineakd supports custom on screen display messages through the configuration file. However, some plugins may choose to disregard custom on screen display messages if they feel it does not apply. Currently you specify the on screen display message for a command by placing it within square brackets at the beginning of the line. E.g.
        [On Screen Message] Go = some command
        [Another Message] Home+alt = some command
        [Boring Message] Home = some command


#### Plugins

The lineakd daemon does not contain any internal macros or actual on screen display functionality. This functionality is implemented in plugins that are loaded when the lineakd daemon is first run. NOTE: Restarts of the daemon with lineakd -r will not reload plugins, or load new plugins. 

The previous functionality that existed in versions of lineakd up uptin 0.8 has been moved into two plugins: lineak_defaultplugin, which contains all of the EAK_* macros and lineak_xosdplugin which contains all of the XOSD on screen display functionality. There is now also a lineak_kdeplugin plugin that brings with it a total of 23 macros or more for handling KDE funtionality via dcop. Some of these macros themselves take arguments which expands their functionality. See the respective plugin packages for more information on MACRO usage.


## Helper Applications

The lineakd package now installs two helper applications, send_to_keyboard and evtest. 

evtest: Allows you to see the event layer events your keyboard is generating. This is for debugging to help you determine if indeed your keyboard is sending events, and which events it is sending. It uses the new event layer.

send_to_keyboard (only for arm, x86 and x86_64): This is a simple program which, for now is just for the IBM keyboards. It allows you to send actual control codes to the keyboard device to enable or disable certain functions. For example, some of the IBM keyboards require:
  send_to_keyboard ea 17
in order to enable the extra keys. This helper application is actually used from lineakd. Within your lineakkb.def file you can specify a configuration option after the keyboard and model name to initialize the keyboard with. As requirements for this sort of thing increases I'll enable the handling of different options. So far we only look for INITIALIZE.

e.g. 

[IBMRA7993]
  brandname = "IBM"
  modelname = "Rapid Access Keyboard"
  RAWCOMMAND[INITIALIZE] = "ea 71"   # <--- lineakd will at this point fork and execute: `send_to_keyboard ea 17`
  [KEYS]
    WWW              = 178
    WorldBook        = 151
    Option           = 176
    Help             = 166
    Standby          = 165
    AudioMute        = 146
    AudioStop        = 164
    AudioPlay|Pause  = 153
    Back             = 163
    Forward          = 161
    AudioLowerVolume = 158
    AudioRaiseVolume = 160
    Custom1          = 174
  [END KEYS]

[END IBMRA7993]


# Notes

## How to get lineakd to send keyevent from easy access keys

You have a few options.
1) Try the EAK_SENDKEYS or EAK_SENDKEYS_ROOT macros in the lineak_defaultplugins package.
2) Use the xsendkeys or xsendkeycode commands in the utils subdirectory.
3) Download and install the xvkbd program from http://homepage3.nifty.com/tsato/xvkbd/
e.g.

	Back = "xvkbd -text "\\A\\[Left]""
	Forward = "xvkbd -text "\\A\\[Right]""
	Reload = "xvkbd -xsendevent -text "\\Cr""
	Stop = "xvkbd -xsendevent -text "\\e""
	fnF1 = "xvkbd -xsendevent -text "\\[Up]""
	fnF2 = "xvkbd -xsendevent -text "\\[Down]""
	fnF3 = "xvkbd -xsendevent -text "\\Cx""
	fnF4 = "xvkbd -xsendevent -text "\\Cv""
	fnF5 = "xvkbd -xsendevent -text "\\Cc""

## How to kill lineakd

In the event that lineakd hangs (which it probably will at some point), you have to kill all of the processes manually. You can do this by executing the following command:

	kill -9 `ps -ef | grep lineakd | grep -v grep | awk ' { print $2 }'`

Then, after a hang or crash (or segfault, etc) you should clean up the shared message queue.

Execute:

	ipcs -q

To see any queues that are hanging around. You'll see something like this (but be relatively sure it's not part of another process, if unsure, look at which queues are there before you run lineakd, or just forget about this step):

	------ Message Queues --------
	key        msqid      owner      perms      used-bytes   messages
	0x0000050c 884736     sheldonl   640        0            0

To get rid of the queue, type:

	ipcrm msg <msqid>

so, for the example type:

	ipcrm msg 884736

If you have a keyboard defined in lineakkb.def that has a toggle key like the one for the LTCDP keyboard, please update the file with the new key format and send it back to me.

## Known bugs

Linux 2.6 can cause changes in how the keycodes are interpreted. In some cases, keys that use to work, no longer work at all. If this happens to you try the following:
1) run xev and see if X reports any keycodes for the key(s) that is/are not working. If there are keycodes being produced, you'll have to update the lineakkb.def file (usually in /etc) with the correct keycodes for your keyboard. 
2) If xev does not report any keycodes for one or more of your keys, then try looking in /var/log/messages. If you have error messages in that file from atkbd.c about unknown scancodes, etc. Then somewhere in your startup scripts (at the end of /etc/rc.sysinit for example). Try binding those scancodes to keycodes with the setkeycodes command. If in doubt just use the keycode from your /etc/lineakkb.def file. Then X should produce a keycode for the key. You'll still have to follow the steps under the procedure 1 above to get lineak to recognize the keys, but it'll work.
3) If you see nothing in your /var/log/messages, and xev produces no keycodes, you can try running showkey -s as root and see if the keypresses show any scancodes. If they do, follow thre relevant part of procedure 2 above. If not, see 4.
4) If none of the above work, send an email to the linux-usb mailing list (if your keyboard is USB) or to the atkbd.c maintainer and pray they do something about it. Otherwise your screwed. Take your POS keyboard back to the store and get a supported one. :(

USB keyboards may not work or just partially. If possible, use PS/2 for the time being. If this isn't possible, email the X.org development or Linux USB mailing list. Tell them the make and model keyboard you have, and what scancodes are not generating events. Use:
     showkey -s 
    while running as root to see what scancodes a key produces.

The EAK_SLEEP action has no functionality yet.

Due to the fact that not all IDE CD-ROM drives provide status info on the tray position, the following can occur:
	* if lineakd is started with the cdrom tray in open position, the eject button
	  has to be pressed twice the first time to close it.
	* if you opened the cdrom tray with the eject button and let the tray close
	  automatically after a while (hardware timer), the eject button has to be
	  pressed twice to open it again.

Not everything may be platform independant. There may be some things specific to linux or an X11 version.


## Helper Applications

### evtest

evtest is an event device testing program for Linux. It allows you to determine
the event layer events that your keyboard is generating. This is for debugging
purposes to determine if indeed your keyboard is sending events, and which
events it is sending. This program uses the event layer on the linux 2.6 series
of kernels.


### send_to_keyboard  

send_to_keyboard is  a  simple program which allows you to send actual control 
codes to the keyboard device to enable or disable certain functions. For example, 
some of the IBM keyboards require:
   send_to_keyboard ea 17

   in order to enable the extra keys. This helper application is actually used 
from lineakd(1) by specifying the following directive in your lineakkb.def file.
   RAWCOMMAND[INITIALIZE] = "<args>"

   The args to enable the IBM keyboard would be "ea 17". Lineakd passes those
arguments to the send_to_keyboard helper application when it starts up. However,
in order to use the send_to_keyboard functionality from lineakd, it must be
installed suid root. This can be done by compiling lineakd with the:
   --enable-suid-helpers option to the configure script.

   For more information, please read the send_to_keyboard man page.


### xsendkeycode

This program sends a key event to X11 using a keycode and a flag that specifies 
the keydown or keyup state. It is not used from lineakd, but exists as a tool 
for scripting, or as a command to use in a lineakd configuration file. It can
be run as follows:

    xsendkeycode 38 1

This will send the letter 'a'. For more infomation on detailed usage, please 
read the xsendkeycode manpage.

### xsendkeys

xsendkeys  is  a  simple program which allows you to send a key or key 
combination to X11. For example: xsendkeys a will send the letter a. If you 
wanted to send a capital a ’A’ you would have to send: xsendkeys Shift_L+a`
For detailed usage please read the xsendkeys manpage.
	
