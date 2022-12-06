/***************************************************************************
                          xosdctrl.cpp  -  description
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

#include <lineak/lconfig.h>
#include <lineak/displayctrl.h>
#include <lineak/lineak_core_functions.h>
using namespace lineak_core_functions;

float displayCtrl::MaxAudio = 25700.00;
const float displayCtrl::MUTE = -100.0000;
extern bool verbose;

displayCtrl::displayCtrl(){
}
displayCtrl::displayCtrl(LConfig& config) {
} 
displayCtrl::~displayCtrl(){
	msg("Destructing displayCtl object");
}
/** Reinitilize the object */
void displayCtrl::init(LConfig& config) {
	msg("Initializing stdout display!");
}
/** Reinitilize the object */
void displayCtrl::init() {
	msg("Initializing stdout display!");
}
/* Cleanup xosd */
void displayCtrl::cleanup() {
	msg("Cleaning up stdout display!");
}
/* Age the display and scroll */
void displayCtrl::agenscroll() {
	msg("Aging and scrolling stdout display!");
}
/* Age the display */
void displayCtrl::age() {
	msg("Aging stdout display!");
}
/* Show the volume */
void displayCtrl::volume(float volume) {
	if (verbose)
		cout << "Setting volume on stdout display to: " << volume << endl;
}
/* Show a command */
void displayCtrl::show(string command) {
	msg("Showing command: " + command + " on stdout display" );
}
void displayCtrl::show() {
	msg("Showing stdout display!");
}
void displayCtrl::hide() {
	msg("Hiding stdout display!");
}
bool displayCtrl::enabled() {
	msg("Checking the enable status of stdout display!");
	return false;
}
/** Read property of static int MaxAudio. */
const float& displayCtrl::getMaxAudio(){
	return MaxAudio;
}
/** Write property of static int MaxAudio. */
void displayCtrl::setMaxAudio( const float& _newVal){
	MaxAudio = _newVal;
}
