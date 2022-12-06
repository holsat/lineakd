/***************************************************************************
                          xosdctrl.h  -  description
                             -------------------
    begin                : Sat Feb 22 2003
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

#ifndef DCTRL_H
#define DCTRL_H

#include <string>
#include <iostream>

class LConfig;
using namespace std;
/**Controls functioning of OSD.
  *@author Sheldon Lee Wen
  */

/** Define dome defaults */
#define DEFAULT_DISPLAY_PLUGIN "internal"
#define DISPLAY_FONT        "-adobe-helvetica-bold-r-normal-*-*-240-*-*-p-*-*-*"
#define DISPLAY_COLOR       "0aff00"
#define DISPLAY_POS         "bottom"
#define DISPLAY_ALIGN       "center"
#define DISPLAY_HOFFSET     "0"
#define DISPLAY_VOFFSET     "50"
#define DISPLAY_TIMEOUT     "3"
#define DISPLAY_SOFFSET     "1"


/* Position of the display */
typedef enum
  {
      Display_top=0,    /* Top of the screen. */
      Display_bottom,  /* Bottom of the screen. */
      Display_middle  /* middle of the screen. */
  } display_pos;

/* Alignment of the display */
typedef enum
  {
      Display_left=0,
      Display_center,
      Display_right
  } display_align;


class displayCtrl {
public: 
  	displayCtrl();
  	displayCtrl(LConfig& config);
  	virtual ~displayCtrl();
  	/** Reinitilize the object */
  	virtual void init(LConfig& config);
  	/** Reinitilize the object */
  	virtual void init();
	/* Cleanup xosd */
	virtual void cleanup();
	/* Age the display and scroll */
	virtual void agenscroll();
	/* Age the display */
	virtual void age();
	/* Show the volume */
	virtual void volume(float volume);
	/* Show a command */
	virtual void show(string command);
 	/** Write property of static int MaxAudio. */
 	void setMaxAudio( const float& _newVal);
 	/** Read property of static int MaxAudio. */
 	static const float& getMaxAudio();
  	virtual void show();
  	virtual void hide();
  	virtual bool enabled();
  	/** Constant Mute volume */
  	static const float MUTE;
protected:
 	/** The maximum audio value. */
 	static float MaxAudio;
        /** Did osd initialize correctly? */
   	bool isenabled;
};
//const float xosdCtrl::MUTE = -100.00;
#endif
