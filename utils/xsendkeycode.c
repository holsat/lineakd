/* xsendkey.c ************************************************/
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

char *ProgramName;

Display *dpy;
int scr;
Window win;
unsigned int width, height;

int main(int argc, char **argv)
{
    Window ret_win;
    int x, y;
    unsigned int border_width, depth;
    int keycode , is_down=1;

    ProgramName = argv[0];
    if (argc < 2)
	return 1;
    /*if (isdigit(argv[1][0]))
        printf("%d is a digit\n",argv[1][0]);
*/
    keycode = atoi (argv[1]);
    if (argc == 3)
	is_down = atoi (argv[2]) ? 1 : 0;
    dpy = XOpenDisplay("");
    if (!dpy) {
	fprintf(stderr, "%s: Cannot connect to display ...\n", ProgramName);
	return 1;
    }
    scr = DefaultScreen(dpy);
    win = RootWindow(dpy, scr);
    XGetGeometry(dpy, win, &ret_win, &x, &y, &width, &height, &border_width, &depth);
    
    fprintf(stderr,"%dx%d keycode %d is_down %d\n", width, height, keycode, is_down);
    XTestFakeKeyEvent(dpy, keycode, is_down, 0);
    XSync(dpy, 1);
    return 0;
}
